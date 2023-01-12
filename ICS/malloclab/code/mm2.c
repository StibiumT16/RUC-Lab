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
#define CHUNKSIZE (1<<12)
#define MINBLOCKSIZE 24

#define MAX(x, y) ((x) > (y) ? (x) : (y)) 
#define PACK(size, alloc) ((size) | (alloc))
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

#define HDRP(bp) ((char *)(bp) - WSIZE) 
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) 
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

#define PREV_PTR(bp) (*(char **)(bp))
#define NEXT_PTR(bp) (*(char **)(bp + DSIZE))
#define SET_PREV(bp, val) (PREV_PTR(bp) = (val))
#define SET_NEXT(bp, val) (NEXT_PTR(bp) = (val))

static char *heap_listp, *list_head;

static void erase(void *bp){
	if(bp == NULL || GET_ALLOC(HDRP(bp))) return;
	void *prev = PREV_PTR(bp);
	void *next = NEXT_PTR(bp);
	SET_PREV(bp, 0);
	SET_NEXT(bp, 0);
	if(prev == NULL && next ==NULL) list_head = NULL;
	else if(prev ==NULL) SET_PREV(next, 0), list_head = next;
	else if(next == NULL) SET_NEXT(prev, 0);
	else SET_NEXT(prev, next), SET_PREV(next, prev);
} 

static void insert(void *bp){
    if(bp == NULL) return;
    if(list_head == NULL){
        list_head = bp;
        return;
    }
    SET_NEXT(bp, list_head);
    SET_PREV(list_head, bp);
    list_head = bp;
}

static void *coalesce(void *bp){
    void *prev_blk = PREV_BLKP(bp);
    void *next_blk = NEXT_BLKP(bp);
    size_t prev_alloc = GET_ALLOC(FTRP(prev_blk));
    size_t next_alloc = GET_ALLOC(HDRP(next_blk));
    size_t size = GET_SIZE(HDRP(bp));
    if(prev_alloc && next_alloc) {}
    else if(prev_alloc && !next_alloc){
        erase(next_blk);
        size += GET_SIZE(HDRP(next_blk));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    } 
    else if(!prev_alloc && next_alloc){
        erase(prev_blk);
        size += GET_SIZE(HDRP(prev_blk));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(prev_blk), PACK(size, 0));
        bp = prev_blk;
    }
    else{
        erase(prev_blk);
        erase(next_blk);
        size += GET_SIZE(HDRP(prev_blk)) + GET_SIZE(FTRP(next_blk));
        PUT(HDRP(prev_blk), PACK(size, 0));
        PUT(FTRP(next_blk), PACK(size, 0));
        bp = prev_blk;
    }
    insert(bp);
    return bp;
}

static void *extend_heap(size_t words){
    char *bp;
    size_t size;
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if((long)(bp = mem_sbrk(size)) == -1) return NULL;
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    SET_PREV(bp, 0);
    SET_NEXT(bp, 0);
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
    return coalesce(bp);
}

int mm_init(void){
    if((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1) return -1;
    PUT(heap_listp, 0);
    PUT(heap_listp + 1 * WSIZE, PACK(DSIZE, 1));
    PUT(heap_listp + 2 * WSIZE, PACK(DSIZE, 1));
    PUT(heap_listp + 3 * WSIZE, PACK(0, 1));
    heap_listp += DSIZE;
    list_head = NULL;
    if(extend_heap(CHUNKSIZE / WSIZE) == NULL) return -1;
    return 0;
} 

void mm_free(void *bp){
    if(bp == NULL) return;
    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    SET_NEXT(bp, 0);
    SET_PREV(bp, 0);
    coalesce(bp);
}

static void *find_fit(size_t asize){
    for(char *bp = list_head; bp; bp = NEXT_PTR(bp))
        if(GET_SIZE(HDRP(bp)) >= asize)
            return bp;
    return NULL;
} 

static void place(void *bp, size_t asize){
    size_t csize = GET_SIZE(HDRP(bp));
    erase(bp);
    if(csize - asize >= MINBLOCKSIZE){
        PUT(HDRP(bp),PACK(asize, 1));
        PUT(FTRP(bp),PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp),PACK(csize - asize, 0));
        PUT(FTRP(bp),PACK(csize - asize, 0));
        SET_PREV(bp, 0);
        SET_NEXT(bp, 0);
       insert(bp);
    }
    else{
        PUT(HDRP(bp),PACK(csize, 1));
        PUT(FTRP(bp),PACK(csize, 1));
    }
} 

void *mm_malloc(size_t size){
    size_t asize;
    size_t extendsize;
    char *bp;
    if(size == 0) return NULL;
    asize= DSIZE * ((size + DSIZE - 1) / DSIZE + 3);
    if((bp = find_fit(asize)) != NULL){
        place(bp, asize);
        return bp;
    }
    extendsize = MAX(asize, CHUNKSIZE);
    if((bp = extend_heap(extendsize / WSIZE)) == NULL) return NULL;
    place(bp, asize);
    return bp;
}

void *mm_realloc(void *ptr, size_t size){
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL; 
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}


