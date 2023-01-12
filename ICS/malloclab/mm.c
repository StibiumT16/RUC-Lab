/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "114514",
    /* First member's full name */
    "StibiumT",
    /* First member's email address */
    "2020201018@ruc.edu,cn",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1<<11)

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define PUT(p, val) (*(unsigned int *)(p) = (val))
#define PACK(size, alloc) ((size) | (alloc))
#define GET(p) (*(unsigned int *)(p))
#define SIZE(p) (GET(p) & ~0x7)
#define ALLOC(p) (GET(p) & 0x1)
#define PREV_ALLOC(p) (GET(p) & 0x2)
#define PREV_FREE(p) (GET(p) & 0x4)

#define HDRP(bp) ((char *)(bp) - WSIZE) 
#define FTRP(bp) ((char *)(bp) + SIZE(HDRP(bp)) - DSIZE) 
#define NEXT_BLKP(bp) ((char *)(bp) + SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((GET_PREV_FREE(bp)) ? ((char *)(bp) - DSIZE) :((char *)(bp) - SIZE((char *)(bp) - DSIZE)))

#define GET_SIZE(p) (GET(HDRP(p)) & ~0x7)
#define GET_ALLOC(p) (GET(HDRP(p)) & 0x1)
#define GET_PREV_ALLOC(p) (GET(HDRP(p)) & 0x2)
#define GET_PREV_FREE(p) (GET(HDRP(p)) & 0x4)
#define SET_PREV_ALLOC(p) (GET(HDRP(p)) |= 0x2)
#define SET_PREV_FREE(p) (GET(HDRP(p)) |= 0x4)
#define RESET_PREV_ALLOC(p) (GET(HDRP(p)) &= ~0x2)
#define RESET_PREV_FREE(p) (GET(HDRP(p)) &= ~0x4)


static char *heap_listp;
unsigned int *rt, *list_head, *list;
#define ptr2int(ptr) (unsigned int)((char *)(ptr) - (char *)mem_heap_lo())

unsigned int* GET_FA(void *bp){
    return *(unsigned int*)(bp) == 0 ? NULL : (void*)((char *)mem_heap_lo() + *(unsigned int*)(bp));
}

unsigned int* GET_LS(void *bp){
    return *((unsigned int*)(bp) + 1) == 0 ? NULL : (void*)((char *)mem_heap_lo() + *((unsigned int*)(bp) + 1));
}

unsigned int* GET_RS(void *bp){
    return *((unsigned int*)(bp) + 2) == 0 ? NULL : (void*)((char *)mem_heap_lo() + *((unsigned int*)(bp) + 2));
} 

void SET_FA(void *bp, void *ptr){
    *(unsigned int *)(bp) = (ptr == NULL ? (unsigned int) 0 :  ptr2int(ptr));
}

void SET_LS(void *bp, void* ptr){
    *((unsigned int*)(bp) + 1) = (ptr == NULL ?  (unsigned int) 0 : ptr2int(ptr));  
}

void SET_RS(void *bp, void* ptr){
    *((unsigned int*)(bp) + 2) = (ptr == NULL ?  (unsigned int) 0 : ptr2int(ptr));  
}

static int getpos(void *bp){
    return (char *)(bp) == (char *)GET_RS(GET_FA(bp));
}

static void rotate(unsigned int *x){
    unsigned int *y = GET_FA(x);
    unsigned int *z = GET_FA(y);
    int chk = getpos(x);
    if(chk){
        unsigned int *tmp = GET_LS(x);
        SET_RS(y, tmp);
        if(tmp != NULL) SET_FA(tmp, y);
        SET_LS(x, y);
    }
    else{
        unsigned int *tmp = GET_RS(x);
        SET_LS(y, tmp);
        if(tmp != NULL) SET_FA(tmp, y);
        SET_RS(x, y);
    }
    SET_FA(y, x);
    SET_FA(x, z);
    if(z != NULL){
        if((unsigned int *)y == (unsigned int *)GET_RS(z)) SET_RS(z, x);
        else SET_LS(z, x);
    }
}

static void Splay(unsigned int *x){
    for(unsigned int *f; (f = GET_FA(x)) != NULL; rotate(x)){
        if(GET_FA(f) != NULL)
            rotate(getpos(f) == getpos(x) ? f : x);
    }
    rt = x;
}

static void erase(void *bp){
    if(bp == NULL || GET_ALLOC(bp)) return;
    SET_PREV_ALLOC(NEXT_BLKP(bp));
    size_t asize = GET_SIZE(bp);
    if(asize == DSIZE){
        RESET_PREV_FREE(NEXT_BLKP(bp));
        if(list_head == bp){
            list_head = GET_FA(list_head);
            return;
        }
        unsigned int * x = list_head;
        while(x != NULL){
            if((void *) GET_FA(x) ==(void *)bp) break;
            x = GET_FA(x);
        }
        SET_FA(x, GET_FA(bp));
        return;
    }
    else if(asize == 2 *DSIZE){
        void *prev = GET_FA(bp);
        void *next = GET_LS(bp);
        if(prev == NULL && next ==NULL) list = NULL;
	    else if(prev == NULL) SET_FA(next, NULL), list = next;
	    else if(next == NULL) SET_LS(prev, NULL);
	    else SET_LS(prev, next), SET_FA(next, prev);
        return;
    }
    Splay(bp);
    unsigned int* ls = GET_LS(rt), *rs = GET_RS(rt);
    if(ls == NULL && rs == NULL){
        rt = NULL;
    }
    else if(ls == NULL){
        rt = rs;
        SET_FA(rt, NULL);
    }
    else if(rs == NULL){
        rt = ls;
        SET_FA(rt, NULL);
    }
    else{
        unsigned int *x = rt, *y = ls;
        while(GET_RS(y) != NULL) y = GET_RS(y);
        Splay(y);
        SET_FA(GET_RS(x), y);
        SET_RS(y, GET_RS(x));
    }
} 

static void insert(void *bp, size_t asize){
    if(bp == NULL) return;
    RESET_PREV_ALLOC(NEXT_BLKP(bp));
    if(asize == DSIZE){
        SET_PREV_FREE(NEXT_BLKP(bp));
        SET_FA(bp, list_head);
        list_head = bp;
        return;
    }
    else if(asize == 2 * DSIZE){
        SET_FA(bp, NULL);
        SET_LS(bp, list);
        if(list != NULL) SET_FA(list, bp);
        list = bp;
        return;
    }
    SET_FA(bp, NULL);
    SET_LS(bp, NULL);
    SET_RS(bp ,NULL);
    if(rt == NULL){
        rt = (unsigned int *)bp;
        return;
    }
    unsigned int *x = rt, *f = NULL;
    while(x != NULL) f = x, x = GET_SIZE(x) <= asize ? GET_RS(x) : GET_LS(x);
    SET_FA(bp, f);
    if(GET_SIZE(f) <= asize) SET_RS(f, bp);
    else SET_LS(f, bp);
    Splay(bp);
}

static char *find_fit(size_t asize){
    if(asize == DSIZE && list_head != NULL) return (char *)list_head; 
    else if(asize <= 2* DSIZE && list != NULL) return (char *) list;
    unsigned int* x = rt, *res =NULL;
    while(x != NULL){
        if(GET_SIZE(x) >= asize) res = x, x = GET_LS(x);
        else x = GET_RS(x);
    }
    if(res != NULL) Splay(res);
    return (char *)res;
}

static void *place(void *bp, size_t asize){
    size_t csize = GET_SIZE(bp), tmp = csize - asize;
    erase(bp);
    if(tmp < DSIZE) PUT(HDRP(bp),PACK(csize, 1 | GET_PREV_ALLOC(bp) | GET_PREV_FREE(bp)));
    else if(asize >= 96){ 
        size_t tag = GET_PREV_ALLOC(bp) | GET_PREV_FREE(bp);
        PUT(HDRP(bp),PACK(tmp, tag));
        PUT(FTRP(bp),PACK(tmp, tag));
        void* nxt  = NEXT_BLKP(bp);
        PUT(HDRP(nxt),PACK(asize, 1));
        insert(bp, tmp);
        bp =nxt;
    }
    else{
        PUT(HDRP(bp),PACK(asize, 1 | GET_PREV_ALLOC(bp) | GET_PREV_FREE(bp)));
        void *nxt = NEXT_BLKP(bp);
        PUT(HDRP(nxt),PACK(tmp, 2));
        PUT(FTRP(nxt),PACK(tmp, 2));
        insert(nxt, tmp); 
    }
    return bp;
} 

static void *coalesce(void *bp){
    size_t prev_alloc = GET_PREV_ALLOC(bp);
    size_t next_alloc = GET_ALLOC(NEXT_BLKP(bp));
    size_t size = GET_SIZE(bp);
    if(prev_alloc && next_alloc) {}
    else if(prev_alloc && !next_alloc){
        void *next_blk = NEXT_BLKP(bp);
        size += GET_SIZE(next_blk);
        erase(next_blk);
        size_t tag = GET_PREV_ALLOC(bp) | GET_PREV_FREE(bp);
        PUT(HDRP(bp), PACK(size, tag));
        PUT(FTRP(bp), PACK(size, tag));
    } 
    else if(!prev_alloc && next_alloc){
        void *prev_blk = PREV_BLKP(bp);
        size += GET_PREV_FREE(bp) ? DSIZE : GET_SIZE(prev_blk);
        size_t tag = GET_PREV_ALLOC(prev_blk) | GET_PREV_FREE(prev_blk);
        erase(prev_blk);
        PUT(FTRP(bp), PACK(size, tag));
        PUT(HDRP(prev_blk), PACK(size, tag));
        bp = prev_blk;
    }
    else{
        void *prev_blk = PREV_BLKP(bp);
        void *next_blk = NEXT_BLKP(bp);
        size += (GET_PREV_FREE(bp) ? DSIZE : GET_SIZE(prev_blk)) + GET_SIZE(next_blk);
        size_t tag = GET_PREV_ALLOC(prev_blk) | GET_PREV_FREE(prev_blk);
        erase(prev_blk);
        erase(next_blk);
        PUT(HDRP(prev_blk), PACK(size, tag));
        PUT(FTRP(prev_blk), PACK(size, tag));
        bp = prev_blk;
    }
    insert(bp, size);
    return bp;
}

static void *extend_heap(size_t size){
    char *bp;
    if((long)(bp = mem_sbrk(size)) == -1) return NULL;
    size_t tag = GET_PREV_ALLOC(bp) | GET_PREV_FREE(bp);
    PUT(HDRP(bp), PACK(size, tag));
    PUT(FTRP(bp), PACK(size, tag));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
    return coalesce(bp);
}

int mm_init(void){ 
    rt = NULL;
    list_head = NULL;
    list = NULL;
    if((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1) return -1;
    PUT(heap_listp, 0);
    PUT(heap_listp + 1 * WSIZE, PACK(DSIZE, 1));
    PUT(heap_listp + 2 * WSIZE, PACK(DSIZE, 1));
    PUT(heap_listp + 3 * WSIZE, PACK(0, 3));
    heap_listp += DSIZE;
    if(extend_heap(CHUNKSIZE / WSIZE) == NULL) return -1;
    return 0;
} 

void mm_free(void *bp){
    if(bp == NULL) return;
    size_t size = GET_SIZE(bp);
    size_t tag = GET_PREV_ALLOC(bp) | GET_PREV_FREE(bp);
    PUT(HDRP(bp), PACK(size, tag));
    PUT(FTRP(bp), PACK(size, tag));
    coalesce(bp);
}

void *mm_malloc(size_t size){
    size_t asize, extendsize;
    char *bp = NULL;
    if(size == 0) return NULL;
    asize = DSIZE * ((size + WSIZE + DSIZE - 1) / DSIZE);
    if((bp = find_fit(asize)) == NULL){
        extendsize = MAX(asize, CHUNKSIZE);
        if((bp = extend_heap(extendsize)) == NULL) return NULL;
    }
    return place(bp, asize);
}

void *mm_realloc(void *ptr, size_t size){
    if(ptr == NULL)  return mm_malloc(size);
    else if(size == 0){ 
        mm_free(ptr);
        return NULL;
    }
    size_t asize, cur_size = GET_SIZE(ptr);
    asize = DSIZE * ((size + WSIZE + DSIZE - 1) / DSIZE);
    char *newptr;
    if(cur_size == asize) return ptr;
    
    char *next = NEXT_BLKP(ptr);
    size_t next_alloc =  GET_ALLOC(next);
    size_t next_size = GET_SIZE(next);
    size_t total_size = cur_size;

    if(!next_alloc && (cur_size + next_size >= asize)){ 
        total_size += next_size;
       	erase(next);
        PUT(HDRP(ptr), PACK(total_size, 1 | GET_PREV_ALLOC(ptr) | GET_PREV_FREE(ptr)));
        place(ptr, total_size);
    }
    else if(!next_size && asize >= cur_size){
        size_t extend_size = asize - cur_size;
        if((long)(mem_sbrk(extend_size)) == -1) return NULL; 
        PUT(HDRP(ptr), PACK(total_size + extend_size, 1));
        PUT(HDRP(NEXT_BLKP(ptr)), PACK(0, 1)); 
        place(ptr, asize);
    }
    else{   
        newptr = mm_malloc(asize);
        if(newptr == NULL) return NULL;
        memcpy(newptr, ptr, MIN(cur_size, size));
        mm_free(ptr);
        return newptr;
    }
    return ptr;
}
