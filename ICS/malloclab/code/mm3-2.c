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
#define MINBLOCKSIZE 16
#define NUM_OF_LIST 16

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define PACK(size, alloc) ((size) | (alloc))
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

#define HDRP(bp) ((char *)(bp) - WSIZE) 
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) 
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

static char *heap_listp;
unsigned int *list_head;

#define int2ptr(x) (void*)((char *)mem_heap_lo() + (x))
#define ptr2int(ptr) (unsigned int)((char *)(ptr) - (char *)mem_heap_lo())

inline void* PREV_PTR(void *bp){
    return *(unsigned int*)(bp) == 0 ? NULL : (void*)((char *)mem_heap_lo() + *(unsigned int*)(bp));
}

inline void* NEXT_PTR(void *bp){
    return *((unsigned int*)(bp) + 1) == 0 ? NULL : (void*)((char *)mem_heap_lo() + *((unsigned int*)(bp) + 1));
} 

inline void SET_PREV(void *bp, void *ptr){
    *(unsigned int *)(bp) = ptr == NULL ? (unsigned int) 0 :  ptr2int(ptr);
}

inline void SET_NEXT(void *bp, void* ptr){
    *((unsigned int*)(bp) + 1) = ptr == NULL ?  (unsigned int) 0 : ptr2int(ptr);  
}

inline void* GET_LIST(int i){
    return *(unsigned int *)(list_head + i) == 0 ?  NULL : (int2ptr(*(list_head + i)));
}

inline void SET_LIST(int i, void *bp){
    *(unsigned int *)(list_head + i) = bp == NULL ?  (unsigned int) 0 : ptr2int(bp);
}



static void erase(void *bp){
    if(bp == NULL || GET_ALLOC(HDRP(bp))) return;
    int i = 0;
    size_t size = GET_SIZE(HDRP(bp));
    for(; i < NUM_OF_LIST - 1 && size > 1; size >>= 1, ++i);
	void *pre = PREV_PTR(bp);
	void *nxt = NEXT_PTR(bp);
	if(pre == NULL && nxt == NULL) SET_LIST(i, NULL);
	else if(pre == NULL && nxt != NULL) SET_PREV(nxt, NULL);
	else if(pre != NULL && nxt == NULL) SET_NEXT(pre, NULL), SET_LIST(i, pre);
	else SET_NEXT(pre, nxt), SET_PREV(nxt, pre);
} 

static void insert(void *bp, size_t size){
    if(bp == NULL) return;
    int i = 0;
    for(;i < NUM_OF_LIST - 1 && size > 1; size >>= 1, ++i); 
    void *pre = GET_LIST(i) , *nxt = NULL;
    for(; pre != NULL && size > GET_SIZE(HDRP(pre)); nxt = pre, pre =  PREV_PTR(pre));
    if(pre == NULL && nxt == NULL){
        SET_PREV(bp, NULL);
        SET_NEXT(bp, NULL);
        SET_LIST(i, bp);
    }
    else if(pre == NULL && nxt != NULL){
        SET_PREV(bp, NULL);
        SET_NEXT(bp, nxt);
        SET_PREV(nxt, bp);
    }
    else if(pre != NULL && nxt == NULL){
        SET_NEXT(pre, bp);
        SET_PREV(bp, pre);
        SET_NEXT(bp, NULL);
        SET_LIST(i, bp);
    }
    else{
        SET_NEXT(pre, bp);
        SET_PREV(bp, pre);
        SET_PREV(nxt, bp);
        SET_NEXT(bp, nxt);
    }
}

static void *coalesce(void *bp){
    void *prev_blk = PREV_BLKP(bp);
    void *next_blk = NEXT_BLKP(bp);
    size_t prev_alloc = GET_ALLOC(FTRP(prev_blk));
    size_t next_alloc = GET_ALLOC(HDRP(next_blk));
    size_t size = GET_SIZE(HDRP(bp));
    if(prev_alloc && next_alloc) return bp;
    else if(prev_alloc && !next_alloc){
        erase(bp);
        erase(next_blk);
        size += GET_SIZE(HDRP(next_blk));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    } 
    else if(!prev_alloc && next_alloc){
       	erase(bp);
        erase(prev_blk);
        size += GET_SIZE(HDRP(prev_blk));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(prev_blk), PACK(size, 0));
        bp = prev_blk;
    }
    else{
    	erase(bp);
        erase(prev_blk);
        erase(next_blk);
        size += GET_SIZE(HDRP(prev_blk)) + GET_SIZE(HDRP(next_blk));
        PUT(HDRP(prev_blk), PACK(size, 0));
        PUT(FTRP(next_blk), PACK(size, 0));
        bp = prev_blk;
    }
    insert(bp, size);
    return bp;
}

static void *extend_heap(size_t words){
    char *bp;
    size_t size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if((long)(bp = mem_sbrk(size)) == -1) return NULL;
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
    insert(bp, size);
    return coalesce(bp);
}

int mm_init(void){ 
    if((list_head = mem_sbrk(NUM_OF_LIST * sizeof(WSIZE))) == (void *)-1) return -1;
    for(int i = 0; i < NUM_OF_LIST; ++i) SET_LIST(i, NULL);
    if((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1) return -1;
    PUT(heap_listp, 0);
    PUT(heap_listp + 1 * WSIZE, PACK(DSIZE, 1));
    PUT(heap_listp + 2 * WSIZE, PACK(DSIZE, 1));
    PUT(heap_listp + 3 * WSIZE, PACK(0, 1));
    heap_listp += DSIZE;
    if(extend_heap(CHUNKSIZE / WSIZE) == NULL) return -1;
    return 0;
} 

void mm_free(void *bp){
    if(bp == NULL) return;
    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    insert(bp, size);
    coalesce(bp);
}

static void* place(void *bp, size_t asize){
    size_t csize = GET_SIZE(HDRP(bp)), tmp = csize - asize;
    erase(bp);
    if(tmp <= MINBLOCKSIZE){
        PUT(HDRP(bp),PACK(csize, 1));
        PUT(FTRP(bp),PACK(csize, 1));
    }
    else if(asize >= 96){
        PUT(HDRP(bp),PACK(tmp, 0));
        PUT(FTRP(bp),PACK(tmp, 0));
        insert(bp, tmp);
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp),PACK(asize, 1));
        PUT(FTRP(bp),PACK(asize, 1));
    }
    else{
        PUT(HDRP(bp),PACK(asize, 1));
        PUT(FTRP(bp),PACK(asize, 1));
        void *nxt = NEXT_BLKP(bp);
        PUT(HDRP(nxt),PACK(tmp, 0));
        PUT(FTRP(nxt),PACK(tmp, 0));
        insert(nxt, tmp);
    }
    return bp;
} 

void *mm_malloc(size_t size){
    size_t asize, extendsize;
    char *bp = NULL;
    if(size == 0) return NULL;
    asize = DSIZE * ((size + DSIZE + DSIZE - 1) / DSIZE);
    int i = 0;
    size_t tmp = asize;
    for(; i < NUM_OF_LIST; tmp >>= 1, ++i){
        if(i == NUM_OF_LIST - 1 || (tmp <= 1 && GET_LIST(i) != NULL)){
            for(bp = GET_LIST(i); bp != NULL && asize > GET_SIZE(HDRP(bp)); bp = PREV_PTR(bp));
            if(bp != NULL) break;
        }
        
    }
    if(bp == NULL){
        extendsize = MAX(asize, CHUNKSIZE);
        if((bp = extend_heap(extendsize / WSIZE)) == NULL) return NULL;
    }
    return place(bp, asize);
}

void *mm_realloc(void *ptr, size_t size){
    if(ptr == NULL)  return mm_malloc(size);
    else if(size == 0){ 
        mm_free(ptr);
        return NULL;
    }
    size_t asize, cur_size = GET_SIZE(HDRP(ptr));
    asize = DSIZE * ((size + DSIZE + DSIZE - 1) / DSIZE);
    char *newptr;
    if(cur_size == asize) return ptr;
    
    char *next = NEXT_BLKP(ptr);
    size_t next_alloc =  GET_ALLOC(HDRP(next));
    size_t next_size = GET_SIZE(HDRP(next));
    size_t total_size = cur_size;

    if(!next_alloc && (cur_size + next_size >= asize)){ 
        total_size += next_size;
       	erase(next);
        PUT(HDRP(ptr), PACK(total_size, 1));
        PUT(FTRP(ptr), PACK(total_size, 1));
        place(ptr, total_size);
    }
    else if(!next_size && asize >= cur_size){
        size_t extend_size = asize - cur_size;
        if((long)(mem_sbrk(extend_size)) == -1) return NULL; 
        PUT(HDRP(ptr), PACK(total_size + extend_size, 1));
        PUT(FTRP(ptr), PACK(total_size + extend_size, 1));
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

