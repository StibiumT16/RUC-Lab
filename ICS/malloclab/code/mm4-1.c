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
#define MINBLOCKSIZE 24

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define PUT(p, val) (*(unsigned int *)(p) = (val))
#define PACK(size, alloc) ((size) | (alloc))
#define GET(p) (*(unsigned int *)(p))
#define SIZE(p) (GET(p) & ~0x7)
#define ALLOC(p) (GET(p) & 0x1)

#define HDRP(bp) ((char *)(bp) - WSIZE) 
#define FTRP(bp) ((char *)(bp) + SIZE(HDRP(bp)) - DSIZE) 
#define NEXT_BLKP(bp) ((char *)(bp) + SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - SIZE(((char *)(bp) - DSIZE)))

#define GET_SIZE(p) (GET(HDRP(p)) & ~0x7)
#define GET_ALLOC(p) (GET(HDRP(p)) & 0x1)


static char *heap_listp;
unsigned int *rt;
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

static char* find_fit(size_t val){
    unsigned int* x = rt, *res =NULL;
    while(x != NULL){
        if(GET_SIZE(x) >= val) res = x, x = GET_LS(x);
        else x = GET_RS(x);
    }
    if(res != NULL) Splay(res);
    return (char *)res;
}

static void *coalesce(void *bp){
    void *prev_blk = PREV_BLKP(bp);
    void *next_blk = NEXT_BLKP(bp);
    size_t prev_alloc = GET_ALLOC(prev_blk);
    size_t next_alloc = GET_ALLOC(next_blk);
    size_t size = GET_SIZE(bp);
    if(prev_alloc && next_alloc) {}
    else if(prev_alloc && !next_alloc){
        erase(next_blk);
        size += GET_SIZE(next_blk);
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    } 
    else if(!prev_alloc && next_alloc){
        erase(prev_blk);
        size += GET_SIZE(prev_blk);
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(prev_blk), PACK(size, 0));
        bp = prev_blk;
    }
    else{
        erase(prev_blk);
        erase(next_blk);
        size += GET_SIZE(prev_blk) + GET_SIZE(next_blk);
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
    return coalesce(bp);
}

int mm_init(void){ 
    rt = NULL;
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
    size_t size = GET_SIZE(bp);
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}

static void* place(void *bp, size_t asize){
    size_t csize = GET_SIZE(bp), tmp = csize - asize;
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
    if((bp = find_fit(asize)) == NULL){
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
    size_t asize, cur_size = GET_SIZE(ptr);
    asize = DSIZE * ((size + DSIZE + DSIZE - 1) / DSIZE);
    char *newptr;
    if(cur_size == asize) return ptr;
    
    char *next = NEXT_BLKP(ptr);
    size_t next_alloc =  GET_ALLOC(next);
    size_t next_size = GET_SIZE(next);
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

