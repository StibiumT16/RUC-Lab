/*
 * mm.c - The slowest, least memory-efficient malloc package.
 * 
 * In this too young too simple sometimes naive approach, a block is not
 * allocated by decrementing the brk pointer.  A block has no payload. There
 * are only headers and footers.  Blocks are always coalesced and reused.
 * Realloc is not implemented indirectly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this footer comment with your copied header
 * comment that gives a low level description of your copied solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef DEBUG
#include <assert.h>
#else
#define assert(...) NULL
#define fprintf(...) NULL
#endif

#include "mm.h"
#include "memlib.h"

#define inline inline static

/*********************************************************
 * NOTE TO STUDENTS: After you did everything else, please
 * provide no team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Menci",
    /* First member's full name */
    "Menci",
    /* First member's email address */
    "huanghaorui301@gmail.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

// Since the heap's max size is 20MiB, a pointer's length is 25 bits
// Align it to 4 bytes, so it only take 23 bits.

// length + parent + left child + right child = 4 * 23 = 92 bits
// color + is-free + previous-is-free
// A header take 92 + 3 = 95 bits = 12 bytes
// A free block is at least 12 + 3 = 15 bytes, rounded up to 16 bytes.

// struct block_header_t {
//     // 0 ~ 2 bytes: [is_free, length]
//     bool is_free : 1;
//     size_t length : 23;
//
//     // 3 ~ 5 bytes: [previous_is_free, parent]
//     bool previous_is_free : 1;
//     block_header_t *parent : 23;
//
//     // 6 ~ 8 bytes: [color, left_child]
//     bool color : 1;
//     block_header_t *left_child : 23;
//
//     // 9 ~ 11 bytes: [extra_bit, right_child]
//     bool extra_bit : 1;
//     block_header_t *right_child : 23;
// };
// Type 0
// If the block's address is 8-aligned, the 0 ~ 7  bytes are the first part  (8 bytes)
//                                      the 8 ~ 11 bytes are the second part (4 bytes)
// Type 1
// If the block's address is 4-aligned, the 0 ~ 3  bytes are the first part  (4 bytes)
//                                      the 4 ~ 11 bytes are the second part (8 bytes)
//
// The 0 ~ 2 bytes are always on the header side, so we can locate the footer side
// with only the block pointer.
//
// Since we need to return a 8-aligned payload address:
// For a 4-aligned block, the first part and second part are together on the beginning of the block.
// For a 8-aligned block, the first part is on the beginning of the block while the second part follows the payload (but before the footer).
//
// But we need to identify a block is type 0 or type 1 from the payload pointer.
// We use the bit just before the payload (the lowest bit of the byte just before the payload) to store the block type.
// And use an "extra bit" in the last byte of the block header to store this bit's origin value.

typedef uint8_t *block_t;

typedef enum {
    BLOCK_8_ALIGNED = 0,
    BLOCK_4_ALIGNED = 1
} block_type_t;

#define MINIMAL_BLOCK_SIZE \
    (12 + 3)

#define get_block_type(block) \
    ((block_type_t)!!(((intptr_t)(block)) & 0b100))

#define read_24(p24) \
    ((*(uint32_t *)(p24)) & 0x00FFFFFF)

#define read_bit(pb) \
    ((bool)((*((uint8_t *)(pb)) & 1)))

#define read_24_transformed(p24) \
    ((read_24(p24) >> 1) << 2)

#define to_pointer(x) \
    ((uint8_t *)mem_heap_lo() + (x))

#define from_pointer(p) \
    (size_t)((p) - (uint8_t *)mem_heap_lo())

#define write_24_preserve_low_bit(p24, value) \
    ((*(uint32_t *)(p24)) = ((value) << 1) | ((*(uint32_t *)(p24)) & 0xFF000001))

#define write_bit(pb, value) \
    ((*(uint8_t *)(pb)) = ((*(uint8_t *)(pb)) & ~1) | (value))

inline size_t get_block_length(block_t block) {
    return read_24_transformed(&block[0]);
}

inline bool get_block_is_free(block_t block) {
    return read_bit(&block[0]);
}

inline void set_block_length_footer(block_t block, size_t value) {
    uint32_t transformed_value = value >> 2;
    write_24_preserve_low_bit(&block[value - 3], transformed_value);
}

inline void set_block_length(block_t block, size_t value) {
    assert(value != 0);

    uint32_t transformed_value = value >> 2;
    write_24_preserve_low_bit(&block[0], transformed_value);
    
    if (get_block_is_free(block) == true) {
        set_block_length_footer(block, value);
    }
}

inline void set_block_is_free_flag_only(block_t block, bool value) {
    write_bit(&block[0], value);
}

inline void init_block_header_type_bit(block_t block) {
    if (get_block_type(block) == BLOCK_8_ALIGNED) {
        write_bit(&block[7], 0);
    } else {
        write_bit(&block[11], 1);
    }
}

inline void *get_block_payload(block_t block) {
    if (get_block_type(block) == BLOCK_8_ALIGNED) {
        return (void *)(8 + (uint8_t *)block);
    } else {
        return (void *)(12 + (uint8_t *)block);
    }
}

inline block_t get_payload_block(void *payload) {
    uint8_t *p = (uint8_t *)payload;
    uint8_t type = read_bit(p - 1);
    if (type == 0) {
        return (block_t)(p - 8);
    } else {
        return (block_t)(p - 12);
    }
}

// We access the second part with the offset from the logical block header's first byte.
// So the "second part address" should be offseted.
inline uint8_t *get_block_header_second_part_offseted(block_t block) {
    bool is_free = get_block_is_free(block);
    size_t length = get_block_length(block);
    uint8_t *block_end = (uint8_t *)block + length;
    uint8_t *block_end_skip_footer = block_end - (is_free ? 3 : 0);
    return block_end_skip_footer - 12;
}

// index: 1 ~ 3
inline uint32_t read_block_header_integer(block_t block, size_t index) {
    if (get_block_type(block) == BLOCK_8_ALIGNED) {
        uint8_t *second_part = get_block_header_second_part_offseted(block);
        
        // 8-aligned
        switch (index) {
        case 1: // 3 ~ 5
            return read_24_transformed(&block[3]);
        case 2: { // 6 ~ 7, 8 ~ 8
            uint32_t x0 = *(uint8_t *)&block[6];
            uint32_t x1 = *(uint8_t *)&block[7];
            uint32_t x2 = *(uint8_t *)&second_part[8];
            bool t = read_bit(&second_part[9]);
            x1 = (x1 & ~1) | t;
            uint32_t result = (x2 << 16) | (x1 << 8) | x0;
            return (result >> 1) << 2;
        }
        case 3: // 9 ~ 11
            return read_24_transformed(&second_part[9]);
        }
    } else {
        // 4-aligned
        switch (index) {
        case 1: // 3 ~ 5
            return read_24_transformed(&block[3]);
        case 2: // 6 ~ 8
            return read_24_transformed(&block[6]);
        case 3: { // 9 ~ 11
            uint32_t x0 = *(uint8_t *)&block[9];
            uint32_t x1 = *(uint8_t *)&block[10];
            uint32_t x2 = *(uint8_t *)&block[11];
            bool t = x0 & 1;
            x2 = (x2 & ~1) | t;
            uint32_t result = (x2 << 16) | (x1 << 8) | x0;
            return (result >> 1) << 2;
        }
        }
    }
    return -1;
}

// index: 1 ~ 3
inline void write_block_header_integer(block_t block, size_t index, uint32_t value) {
    uint32_t transformed_value = value >> 2;
    if (get_block_type(block) == BLOCK_8_ALIGNED) {
        uint8_t *second_part = get_block_header_second_part_offseted(block);
        
        // 8-aligned
        switch (index) {
        case 1: // 3 ~ 5
            write_24_preserve_low_bit(&block[3], transformed_value);
            return;
        case 2: { // 6 ~ 7, 8 ~ 8
            uint32_t write_value = (transformed_value << 1) | read_bit(&block[6]);
            uint32_t x0 = (uint8_t)write_value;
            uint32_t x1 = (uint8_t)(write_value >> 8);
            uint32_t x2 = (uint8_t)(write_value >> 16);
            bool t = x1 & 1;
            x1 &= ~1; // type = 0
            *(uint8_t *)&block[6] = x0;
            *(uint8_t *)&block[7] = x1;
            *(uint8_t *)&second_part[8] = x2;
            write_bit(&second_part[9], t);
            return;
        }
        case 3: // 9 ~ 11
            write_24_preserve_low_bit(&second_part[9], transformed_value);
            return;
        }
    } else {
        // 4-aligned
        switch (index) {
        case 1: // 3 ~ 3, 4 ~ 5
            write_24_preserve_low_bit(&block[3], transformed_value);
            return;
        case 2: // 6 ~ 8
            write_24_preserve_low_bit(&block[6], transformed_value);
            return;
        case 3: { // 9 ~ 11
            uint32_t write_value = transformed_value << 1;
            uint32_t x0 = (uint8_t)write_value;
            uint32_t x1 = (uint8_t)(write_value >> 8);
            uint32_t x2 = (uint8_t)(write_value >> 16);
            bool t = x2 & 1;
            x2 |= 1; // type = 1
            *(uint8_t *)&block[9] = (x0 & ~1) | t;
            *(uint8_t *)&block[10] = x1;
            *(uint8_t *)&block[11] = x2;
            return;
        }
        }
    }
}

// index: 1 ~ 2
inline uint32_t read_block_header_bit(block_t block, size_t index) {
    switch (index) {
    case 1: // 3
        return read_bit(&block[3]);
    case 2: // 6 ~ 8
        return read_bit(&block[6]);
    }
    return -1;
}

// index: 1 ~ 2
inline void write_block_header_bit(block_t block, size_t index, bool value) {
    // 8-aligned
    switch (index) {
    case 1: // 3
        write_bit(&block[3], value);
        return;
    case 2: // 6 ~ 8
        write_bit(&block[6], value);
        return;
    }
}

typedef uint8_t *rb_node_t;

typedef enum {
    RB_RED = 0,
    RB_BLACK = 1
} rb_color_t;

typedef enum {
    RB_LEFT = 0,
    RB_RIGHT = 1
} rb_which_t;

#define rb_parent(node) \
    ((rb_node_t)to_pointer(read_block_header_integer((block_t)(node), 1)))

#define rb_child(node, child) \
    ((rb_node_t)to_pointer(read_block_header_integer((block_t)(node), 2 + (child))))

#define rb_color(node) \
    ((rb_color_t)read_block_header_bit((block_t)(node), 2))

#define rb_value(node) \
    ((size_t)get_block_length((block_t)(node)))

#define rb_set_parent_unchecked(node, value) \
    write_block_header_integer((block_t)(node), 1, from_pointer(value))
#define rb_set_parent(node, value) \
    ((node) == rbnull ? NULL : rb_set_parent_unchecked(node, value))

#define rb_set_child_unchecked(node, child, value) \
    write_block_header_integer((block_t)(node), 2 + (child), from_pointer(value))
#define rb_set_child(node, child, value) \
    ((node) == rbnull ? NULL : rb_set_child_unchecked(node, child, value))

#define rb_set_color_unchecked(node, value) \
    write_block_header_bit((block_t)(node), 2, value)
#define rb_set_color(node, value) \
    ((node) == rbnull ? NULL : rb_set_color_unchecked(node, value))

#define rb_is_black(node) (rb_get_color(node) == RB_BLACK)
#define rb_is_red(node) (rb_get_color(node) == RB_RED)
#define rb_set_black(node) rb_set_color(node, RB_BLACK)
#define rb_set_red(node) rb_set_color(node, RB_RED)

#define rb_which(node, parent) \
    ((rb_which_t)(((node) == rb_child(parent, RB_LEFT)) ? RB_LEFT : RB_RIGHT))

// Store the root pointer in the first 4 bytes
#define rbroot (*(rb_node_t *)mem_heap_lo())
// Store the null node in the following 12 bytes
#define rbnull ((rb_node_t)(sizeof(rb_node_t) + (uint8_t *)mem_heap_lo()))

inline void *rb_init() {
    // The rbnull must also be a block, without payload, it has the minimal length but without footer.
    set_block_length((block_t)rbnull, 12);
    // Set it to "allocated" to have no footer.
    set_block_is_free_flag_only((block_t)rbnull, false);

    init_block_header_type_bit((block_t)rbnull);

    rb_set_color_unchecked(rbnull, RB_BLACK);
    rb_set_parent_unchecked(rbnull, rbnull);
    rb_set_child_unchecked(rbnull, RB_LEFT, rbnull);
    rb_set_child_unchecked(rbnull, RB_RIGHT, rbnull);
    
    rbroot = rbnull;

    return (void *)(12 + (uint8_t *)(rbnull + 12));
}

inline void rb_init_node(rb_node_t node) {
    // A node not in the tree is indicated by having itself as its parent.
    rb_set_parent(node, node);
}

inline void rb_rotate(rb_node_t node, rb_which_t which, rb_node_t old_parent, rb_which_t old_parent_which, rb_node_t old_grandparent) {
    rb_set_parent(node, old_grandparent);
    if (old_grandparent != rbnull) rb_set_child(old_grandparent, old_parent_which, node);

    rb_node_t child = rb_child(node, which ^ 1);
    if (child != rbnull) rb_set_parent(child, old_parent);
    rb_set_child(old_parent, which, child);

    rb_set_parent(old_parent, node);
    rb_set_child(node, which ^ 1, old_parent);

    if (old_grandparent == rbnull) rbroot = node;
}

inline void rb_insert_fixup(rb_node_t node) {
    while (true) {
        if (node == rbroot) {
            rb_set_color(node, RB_BLACK);
            break;
        }

        rb_node_t parent = rb_parent(node);
        if (rb_color(parent) == RB_BLACK) break;

        rb_node_t grandparent = rb_parent(parent);
        rb_which_t parent_which = rb_which(parent, grandparent);
        rb_node_t uncle = rb_child(grandparent, parent_which ^ 1);
        if (rb_color(uncle) == RB_RED) {
            rb_set_color(parent, RB_BLACK);
            rb_set_color(uncle, RB_BLACK);
            rb_set_color(grandparent, RB_RED);
            node = grandparent;
        } else {
            rb_which_t which = rb_which(node, parent);
            rb_node_t tmp;
            if (which != parent_which) {
                rb_rotate(node, which, parent, parent_which, grandparent);
                tmp = node;
            } else {
                tmp = parent;
            }

            rb_set_color(tmp, RB_BLACK);

            rb_node_t tmp_parent = rb_parent(tmp);
            rb_set_color(tmp_parent, RB_RED);
            rb_node_t tmp_grandparent = rb_parent(tmp_parent);
            rb_rotate(tmp, rb_which(tmp, tmp_parent), tmp_parent, rb_which(tmp_parent, tmp_grandparent), tmp_grandparent);
            
            break;
        }
    }
}

inline void rb_insert(rb_node_t node) {
#ifdef DEBUG
    fprintf(stderr, "rb_insert(%p)\n", node);
#endif
    rb_set_child(node, RB_LEFT, rbnull);
    rb_set_child(node, RB_RIGHT, rbnull);

    if (rbroot == rbnull) {
        rbroot = node;
        rb_set_color(node, RB_BLACK);
        rb_set_parent(node, rbnull);
        return;
    }

    size_t value = rb_value(node);

    rb_node_t current = rbroot;
    rb_which_t which_child;
    while (1) {
        which_child = value <= rb_value(current) ? RB_LEFT : RB_RIGHT;

        rb_node_t child = rb_child(current, which_child);
        if (child != rbnull) {
            current = child;
        } else break;
    }

    rb_set_child(current, which_child, node);
    rb_set_parent(node, current);
    rb_set_color(node, RB_RED);

    rb_insert_fixup(node);
}

inline void rb_delete_fixup(rb_node_t node, rb_node_t parent, rb_which_t which) {
    while (parent != rbnull) {
        // assert(rb_parent(node) == parent);
        // assert(rb_which(node, parent) == which);

        rb_node_t brother = rb_child(parent, which ^ 1);
        if (rb_color(brother) == RB_RED) {
            rb_set_color(brother, RB_BLACK);
            rb_set_color(parent, RB_RED);

            rb_node_t grandparent = rb_parent(parent);
            rb_which_t parent_which = rb_which(parent, grandparent);
            rb_rotate(brother, which ^ 1, parent, parent_which, grandparent);

            brother = rb_child(parent, which ^ 1);
        }

        rb_node_t brother_left_child = rb_child(brother, RB_LEFT), brother_right_child = rb_child(brother, RB_RIGHT);
        rb_color_t brother_left_child_color = rb_color(brother_left_child),
                   brother_right_child_color = rb_color(brother_right_child);
        if (brother_left_child_color == RB_BLACK && brother_right_child_color == RB_BLACK) {
            rb_set_color(brother, RB_RED);
            // rb_set_color(parent, RB_BLACK);

            if (rb_color(parent) == RB_RED) {
                rb_set_color(parent, RB_BLACK);
                break;
            }

            rb_node_t grandparent = rb_parent(parent);
            rb_which_t parent_which = rb_which(parent, grandparent);
            node = parent;
            parent = grandparent;
            which = parent_which;
        } else {
            rb_node_t brother_same_side_child = which == RB_LEFT ? brother_left_child : brother_right_child;
            // rb_node_t brother_other_side_child = which == RB_LEFT ? brother_right_child : brother_left_child;
            // rb_color_t brother_same_side_child_color = which == RB_LEFT ? brother_left_child_color : brother_right_child_color;
            rb_color_t brother_other_side_child_color = which == RB_LEFT ? brother_right_child_color : brother_left_child_color;
            if (brother_other_side_child_color == RB_BLACK) {
                rb_set_color(brother, RB_RED);
                rb_set_color(brother_same_side_child, RB_BLACK);

                rb_rotate(brother_same_side_child, which, brother, which ^ 1, parent);

                brother = brother_same_side_child;
            }

            rb_set_color(brother, rb_color(parent));
            rb_set_color(parent, RB_BLACK);
            rb_set_color(rb_child(brother, which ^ 1), RB_BLACK);

            rb_node_t grandparent = rb_parent(parent);
            rb_which_t parent_which = rb_which(parent, grandparent);
            rb_rotate(brother, which ^ 1, parent, parent_which, grandparent);

            rb_set_color(rbroot, RB_BLACK);
            break;
        }
    }
}

inline void rb_delete_with_at_most_one_child(rb_node_t child, rb_node_t parent, rb_which_t which, rb_color_t color) {
    rb_set_parent(child, parent);
    rb_set_child(parent, which, child);

    if (color == RB_RED) {
        return;
    }

    rb_color_t child_color = rb_color(child);
    if (child_color == RB_RED) {
        rb_set_color(child, RB_BLACK);
        return;
    }

    rb_delete_fixup(child, parent, which);
}

inline void rb_delete(rb_node_t node) {
#ifdef DEBUG
    fprintf(stderr, "rb_delete(%p)\n", node);
#endif
    rb_node_t parent = rb_parent(node);
    assert(parent != node);

    rb_node_t left_child = rb_child(node, RB_LEFT);
    rb_node_t right_child = rb_child(node, RB_RIGHT);
    rb_color_t color = rb_color(node);
    if (left_child == rbnull && right_child == rbnull) {
        rb_delete_with_at_most_one_child(rbnull, parent, rb_which(node, parent), color);
        if (node == rbroot) rbroot = rbnull;
    } else if (left_child == rbnull) {
        rb_delete_with_at_most_one_child(right_child, parent, rb_which(node, parent), color);
        if (node == rbroot) rbroot = right_child;
    } else if (right_child == rbnull) {
        rb_delete_with_at_most_one_child(left_child, parent, rb_which(node, parent), color);
        if (node == rbroot) rbroot = left_child;
    } else {
        rb_which_t which = rb_which(node, parent);

        // Find node's successor
        rb_node_t successor = right_child;
        while (true) {
            rb_node_t tmp = rb_child(successor, RB_LEFT);
            if (tmp == rbnull) break;
            successor = tmp;
        }

        // The successor always have no left child
        rb_node_t successor_left_child = rbnull;
        rb_node_t successor_right_child = rb_child(successor, RB_RIGHT);
        rb_color_t successor_color = rb_color(successor);

        // Swap the deleting node with its successor

        if (color != successor_color) {
            rb_set_color(successor, color);
            rb_set_color(node, successor_color);
        }

        if (node == rbroot) rbroot = successor;
        if (successor == right_child) {
            rb_set_child(parent, which, successor);
            rb_set_parent(successor, parent);

            rb_set_child(successor, RB_LEFT, left_child);
            rb_set_parent(left_child, successor);

            rb_set_child(successor, RB_RIGHT, node);
            rb_set_parent(node, successor);

            rb_set_child(node, RB_LEFT, rbnull);

            rb_set_child(node, RB_RIGHT, successor_right_child);
            rb_set_parent(successor_right_child, node);

            rb_delete_with_at_most_one_child(successor_right_child, successor, RB_RIGHT, successor_color);
        } else {
            rb_node_t successor_parent = rb_parent(successor);
            rb_which_t successor_which = rb_which(successor, successor_parent);

            rb_set_child(parent, which, successor);
            rb_set_parent(successor, parent);

            rb_set_child(successor_parent, successor_which, node);
            rb_set_parent(node, successor_parent);

            rb_set_child(successor, RB_LEFT, left_child);
            rb_set_parent(left_child, successor);

            rb_set_child(successor, RB_RIGHT, right_child);
            rb_set_parent(right_child, successor);

            rb_set_child(node, RB_LEFT, successor_left_child);
            rb_set_parent(successor_left_child, node);

            rb_set_child(node, RB_RIGHT, successor_right_child);
            rb_set_parent(successor_right_child, node);

            rb_delete_with_at_most_one_child(successor_right_child, successor_parent, RB_LEFT, successor_color);
        }
    }

    rb_set_parent(node, node);
}

inline rb_node_t rb_lower_bound(size_t value) {
    rb_node_t node = rbroot, result = rbnull;
    while (node != rbnull) {
        if (rb_value(node) >= value) {
            result = node;
            node = rb_child(node, RB_LEFT);
        } else {
            node = rb_child(node, RB_RIGHT);
        }
    }
    return result;
}

#ifdef DEBUG
inline void rb_traversal_indent(size_t depth) {
    for (size_t i = 0; i < depth * 2; i++) putchar(' ');
}

inline size_t rb_traversal(rb_node_t node, size_t depth, bool print, bool validate, size_t **values) {
    if (node == rbnull) return 0;

    rb_color_t color = rb_color(node);

    rb_node_t left_child = rb_child(node, RB_LEFT),
              right_child = rb_child(node, RB_RIGHT);
    rb_color_t left_child_color = rb_color(left_child),
               right_child_color = rb_color(right_child);
    if (color == RB_RED && validate) {
        assert(left_child_color == RB_BLACK);
        assert(right_child_color == RB_BLACK);
    }

    size_t right_black_height = rb_traversal(right_child, depth + 1, print, validate, values) + (right_child_color == RB_BLACK ? 1 : 0);

    if (print) {
        rb_traversal_indent(depth);
        putchar(color == RB_BLACK ? 'B' : 'R');
        putchar(' ');
        printf("%u\n", (unsigned)(rb_value(node) / 4 - 4));
    }
    if (values) *(*values)++ = rb_value(node);

    size_t left_black_height = rb_traversal(left_child, depth + 1, print, validate, values) + (left_child_color == RB_BLACK ? 1 : 0);
    if (validate) assert(left_black_height == right_black_height);

    return left_black_height;
}

inline bool rb_in_tree(rb_node_t node, rb_node_t root) {
    return root != rbnull && (node == root || rb_in_tree(node, rb_child(root, RB_LEFT)) || rb_in_tree(node, rb_child(root, RB_RIGHT)));
}

inline void rb_print() {
    rb_traversal(rbroot, 0, true, false, NULL);
}
#endif

#define INIT_HEAP_AVAILABLE_SIZE \
    (4092 + 16 + 16 + 12 * 3)

#define MIN_EXTEND_SIZE \
    4096

#define HEAP_HEADER_SIZE \
    (4 + 12)

#define block_length_from_payload_size(payload_size) \
    ((((payload_size) + 12) + (4 - 1)) & ~0b11)

#define payload_size_from_block_length(block_length) \
    ((block_length) - 12)

#define get_previous_is_free(block) \
    ((bool)read_block_header_bit(block, 1))

#define set_previous_is_free(block, value) \
    write_block_header_bit(block, 1, value)

#define available_heap_size \
    (mem_heapsize() - HEAP_HEADER_SIZE)

#define is_last_block(block, length) \
    ((void *)((block) + (length) - 1) == mem_heap_hi())

#define virtual_block_to_indicate_if_the_last_block_is_free \
    ((block_t)rbnull)

inline block_t get_previous_block_if_free(block_t block) {
    if (get_previous_is_free(block) == false) return NULL;
    
    size_t length = read_24_transformed(&block[-3]);
    block_t previous_block = block - length;

    assert(get_block_is_free(previous_block));
    return previous_block;
}

inline block_t get_next_block_if_free(block_t block) {
    size_t length = get_block_length(block);
    if (is_last_block(block, length)) return NULL;
    block_t next_block = block + length;
    if (get_block_is_free(next_block) == false) return NULL;
    return next_block;
}

inline block_t get_logical_next_block(block_t block) {
    size_t length = get_block_length(block);
    if (is_last_block(block, length))
        return virtual_block_to_indicate_if_the_last_block_is_free;
    return block + length;
}

inline void set_block_is_free(block_t block, bool is_free) {
    assert(get_block_is_free(block) != is_free);

    size_t second_part_length = get_block_type(block) == BLOCK_8_ALIGNED ? 4 : 8;

    size_t length = get_block_length(block);
    // Free blocks have footer while allocated blocks don't have.
    size_t current_footer_length = is_free ? 0 : 3;
    size_t target_footer_length = is_free ? 3 : 0;
    uint8_t *block_end = block + length;
    memmove(
        block_end - target_footer_length - second_part_length,
        block_end - current_footer_length - second_part_length,
        second_part_length
    );
    set_block_is_free_flag_only(block, is_free);

    block_t next_block = get_logical_next_block(block);
    set_previous_is_free(next_block, is_free);

    if (is_free) set_block_length_footer(block, length);
}

typedef enum {
    SET_TO_FALSE = 0,
    SET_TO_TRUE = 1,
    PRESERVE = 2
} init_block_set_current_block_previous_is_free_action_t;

// init a block whose rb node is not in the tree
// will re-init its rb node
// previous_is_free is given
inline void init_block(
    block_t block,
    size_t length,
    bool is_free,
    init_block_set_current_block_previous_is_free_action_t set_current_block_previous_is_free_action,

    // If the next block has not been initialized, don't attempt to change its previous_is_free
    // since we calculate addresses with the next block's length, which has not been initialized.
    bool set_next_block_previous_is_free
) {
    assert(length >= MINIMAL_BLOCK_SIZE && length < mem_heapsize());

    bool old_previous_is_free_value;
    if (set_current_block_previous_is_free_action == PRESERVE)
        old_previous_is_free_value = get_previous_is_free(block);

    // length depends on "is free"
    // "next block" depends on length

    init_block_header_type_bit(block);

    set_block_is_free_flag_only(block, is_free);
    set_block_length(block, length);

    if (set_current_block_previous_is_free_action == PRESERVE)
        set_previous_is_free(block, old_previous_is_free_value);
    else
        set_previous_is_free(block, set_current_block_previous_is_free_action == SET_TO_TRUE);

    if (set_next_block_previous_is_free)
        set_previous_is_free(get_logical_next_block(block), is_free);

    rb_init_node((rb_node_t)block);
}

inline block_t extend(size_t wanted_block_size) {
    block_t new_space = 1 + (block_t)mem_heap_hi();

    bool old_last_block_is_free = get_previous_is_free(virtual_block_to_indicate_if_the_last_block_is_free);
    if (old_last_block_is_free) {
        size_t last_block_length = read_24_transformed(&new_space[-3]);
        block_t last_block = new_space - last_block_length;
        rb_delete(last_block);

        mem_sbrk(wanted_block_size - last_block_length);
        init_block(last_block, wanted_block_size, true, PRESERVE, true);

        return last_block;
    } else {
        mem_sbrk(wanted_block_size);
        block_t new_block = new_space;
        init_block(new_block, wanted_block_size, true, SET_TO_FALSE, true);

        return new_block;
    }
}

#ifdef DEBUG
void mm_validate_blocks();
#endif

int mm_init(void) {
    rb_init();

    mem_sbrk(HEAP_HEADER_SIZE + INIT_HEAP_AVAILABLE_SIZE);

    uint8_t *heap = (uint8_t *)mem_heap_lo();
    block_t block = (block_t)(heap + HEAP_HEADER_SIZE);
    init_block(block, INIT_HEAP_AVAILABLE_SIZE, true, SET_TO_FALSE, true);
    rb_insert(block);

    return 0;
}

void *mm_malloc(size_t request_size) {
    if (request_size == 0) return NULL;

    size_t length = block_length_from_payload_size(request_size);

    rb_node_t node = rb_lower_bound(length);
    block_t found_block;
    if (node != rbnull) {
        rb_delete(node);
        found_block = (block_t)node;
    } else found_block = extend(length < MIN_EXTEND_SIZE ? MIN_EXTEND_SIZE : length);

    size_t found_block_length = get_block_length(found_block);

    block_t block;
    if (found_block_length < length + MINIMAL_BLOCK_SIZE) block = found_block;
    else {
        block_t block1, block2;
        if (request_size > 64) {
            block1 = found_block + found_block_length - length;
            block2 = found_block;
            init_block(block2, found_block_length - length, true, PRESERVE, false);
            init_block(block1, length, true, SET_TO_TRUE, true);
        } else {
            block1 = found_block;
            block2 = found_block + length;
            init_block(block1, length, true, PRESERVE, false);
            init_block(block2, found_block_length - length, true, SET_TO_TRUE, true);
        }

        rb_insert(block2);
        block = block1;
    }

    set_block_is_free(block, false);
    
    void *payload = get_block_payload(block);

    assert(payload > mem_heap_lo() && payload < mem_heap_hi());

#ifdef DEBUG
    static int i = 0;
    fprintf(stderr, "malloc(%d-th, %p, %u)\n", ++i, block, (unsigned)get_block_length(block));

    mm_validate_blocks();
#endif

    return payload;
}

void mm_free(void *ptr) {
    assert(ptr > mem_heap_lo() && ptr < mem_heap_hi());

    block_t block = get_payload_block(ptr), merged_block = block;
    size_t merged_length = get_block_length(block);
    bool merged = false;

#ifdef DEBUG
    static int i = 0;
    fprintf(stderr, "free(%d-th, %p)\n", ++i, block);
#endif

    block_t previous_block = get_previous_block_if_free(block);
    if (previous_block) {
        rb_delete(previous_block);
        size_t previous_block_length = get_block_length(previous_block);
        merged_length += previous_block_length;
        merged_block = previous_block;
        merged = true;
    }

    block_t next_block = get_next_block_if_free(block);
    if (next_block) {
        rb_delete(next_block);
        size_t next_block_length = get_block_length(next_block);
        merged_length += next_block_length;
        merged = true;
    }

    if (merged) init_block(merged_block, merged_length, true, PRESERVE, true);
    else {
        set_block_is_free(merged_block, true);
    }
    rb_insert((rb_node_t)merged_block);

#ifdef DEBUG
    mm_validate_blocks();
#endif
}

inline void *mm_realloc_by_malloc_free(void *ptr, size_t size) {
    void *old_payload = ptr;
    void *new_payload = mm_malloc(size);
    if (new_payload == NULL) return NULL;

    size_t old_payload_size = payload_size_from_block_length(get_block_length(get_payload_block(ptr)));
    if (size < old_payload_size)
      old_payload_size = size;
    memcpy(new_payload, old_payload, old_payload_size);
    mm_free(old_payload);
    return new_payload;
}

void *mm_realloc(void *ptr, size_t size) {
    if (size == 0) {
        mm_free(ptr);
        return NULL;
    }

    uint8_t *old_payload = (uint8_t *)ptr;

    block_t old_block = get_payload_block(old_payload);
    size_t old_block_length = get_block_length(old_block);
    size_t old_payload_size = payload_size_from_block_length(old_block_length);

    block_t new_block;
    void *new_payload;

    if (size == old_payload_size) {
        new_block = old_block;
        new_payload = old_payload;
    } else if (size < old_payload_size) {
        // Case 0: shrink

        new_block = old_block;
        size_t new_block_length = block_length_from_payload_size(size);
        size_t new_free_block_length = old_block_length - new_block_length;

        block_t next_block = get_next_block_if_free(old_block);
        if (next_block) {
            new_free_block_length += get_block_length(next_block);
            rb_delete(next_block);
        } else if (new_free_block_length < MINIMAL_BLOCK_SIZE) return old_payload;

        init_block(new_block, new_block_length, false, PRESERVE, false);

        block_t new_free_block = new_block + new_block_length;
        init_block(new_free_block, new_free_block_length, true, SET_TO_FALSE, true);
        rb_insert(new_free_block);

        new_payload = old_payload;
    } else {
        size_t min_grow_size = block_length_from_payload_size(size) - old_payload_size;
        if (is_last_block(old_block, get_block_length(old_block))) {
            // Case 1: grow the last block
            mem_sbrk(min_grow_size);
            new_block = old_block;
            init_block(new_block, old_block_length + min_grow_size, false, PRESERVE, true);
            new_payload = old_payload;
        } else {
            block_t next_block = get_next_block_if_free(old_block);
            size_t next_block_length = !next_block ? 0 : get_block_length(next_block);
            if (next_block && is_last_block(next_block, next_block_length) && next_block_length <= min_grow_size) {
                // Case 2: grow the last allocated block, with the next free block's length just enough or not enough for growing
                rb_delete(next_block);
                mem_sbrk(min_grow_size - next_block_length);
                new_block = old_block;
                init_block(new_block, old_block_length + min_grow_size, false, PRESERVE, true);
                new_payload = old_payload;
            } else {
                // Case 3: try merging previous and next block
                block_t previous_block = get_previous_block_if_free(old_block);
                size_t previous_block_length = !previous_block ? 0 : get_block_length(previous_block);

                bool previous_block_enough = old_payload_size + previous_block_length > size,
                     next_block_enough = old_payload_size + next_block_length > size;

                bool merge_previous_block = false,
                     merge_next_block = false;

                if (previous_block_enough && next_block_enough) {
                    if (previous_block_length < next_block_length)
                        merge_previous_block = true;
                    else
                        merge_next_block = true;
                } else if (previous_block_enough)
                    merge_previous_block = true;
                else if (next_block_enough)
                    merge_next_block = true;
                else if (old_payload_size + previous_block_length + next_block_length > size)
                    merge_next_block = merge_previous_block = true;
                
                if (merge_previous_block || merge_next_block) {
                    if (merge_previous_block) rb_delete(previous_block);
                    if (merge_next_block) rb_delete(next_block);

                    new_block = merge_previous_block ? previous_block : old_block;
                    size_t total_block_length = old_block_length
                                            + (merge_previous_block ? previous_block_length : 0)
                                            + (merge_next_block ? next_block_length : 0);

                    size_t new_block_length = block_length_from_payload_size(size);
                    if (total_block_length - new_block_length < MINIMAL_BLOCK_SIZE)
                        new_block_length = total_block_length; 
                    size_t new_free_block_length = total_block_length - new_block_length;

                    // If the new block is on the left side of the old block and has overlap with the old block.
                    // The initialization of the new block may overwrite the old block's payload area.
                    // The possible overwrite range length is 4 or 8 bytes (the second part of header).
                    size_t overlap_length = get_block_type(new_block) == BLOCK_8_ALIGNED ? 4 : 8;
                    uint8_t *overlap_start = new_block + new_block_length - overlap_length;
                    bool overlap = overlap_start + overlap_length > old_payload
                                && overlap_start < old_payload + old_payload_size;
                    uint8_t overlap_range_backup[overlap_length];
                    if (overlap) memcpy(overlap_range_backup, overlap_start, overlap_length);

                    init_block(new_block, new_block_length, false, PRESERVE, new_free_block_length == 0);
                    new_payload = get_block_payload(new_block);
                    if (new_block != old_block) {
                        memmove(new_payload, old_payload, old_payload_size);

                        if (overlap) {
                            size_t overlap_offset = overlap_start - old_payload;
                            memcpy(
                                new_payload + overlap_offset,
                                overlap_range_backup,
                                overlap_length
                            );
                        }
                    }

                    if (new_free_block_length > 0) {
                        block_t new_free_block = new_block + new_block_length;
                        init_block(new_free_block, new_free_block_length, true, SET_TO_FALSE, true);
                        rb_insert(new_free_block);
                    }
                } else {
                    // Case 4: malloc and free
                    return mm_realloc_by_malloc_free(ptr, size);
                }
            }
        }
    }

#ifdef DEBUG
    static int i = 0;
    fprintf(
        stderr, "realloc(%d-th, %p => %p, %u => %u, request %u)\n",
        ++i,
        old_block, new_block,
        old_block_length, (unsigned)get_block_length(new_block),
        (unsigned)size
    );

    mm_validate_blocks();
#endif

    return new_payload;
}

#ifdef DEBUG
inline void mm_validate_blocks() {
    block_t block = (block_t)mem_heap_lo() + HEAP_HEADER_SIZE;
    while (block != (block_t)((uint8_t *)mem_heap_hi() + 1)) {
        assert(get_block_is_free(block) == get_previous_is_free(get_logical_next_block(block)));
        block += get_block_length(block);
    }
}

inline void mm_print_blocks() {
    block_t block = (block_t)mem_heap_lo() + HEAP_HEADER_SIZE;
    size_t i = 0;
    while (block != (block_t)((uint8_t *)mem_heap_hi() + 1)) {
        if (get_block_is_free(block))
            (fprintf)(
                stderr, "block %u, %s, length = %u\n",
                (unsigned)i,
                get_block_is_free(block) ? "free" : "allocated",
                (unsigned)get_block_length(block)
            );
        block += get_block_length(block);
    }
}
#endif
