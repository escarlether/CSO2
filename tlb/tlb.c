#include <stdlib.h>
#include <stdalign.h>
#include <assert.h>
#include <stdio.h>
#include "tlb.h"

int SET = 16;
int WAY = 4;
int INDEX_MASK = 15;
int INDEX_BITS = 4;
int OFFSET_MASK = (1 << POBITS) - 1;
typedef struct{
    size_t ppn;
    int LRU;
    size_t tag;
    int valid;
}entry;

entry cache[16][4];

/** stub for the purpose of testing tlb_* functions */
size_t translate(size_t va) { 
    if (va < 0x1234000){
        return va + 0x20000;
    }
    else if (va > 0x2000000 && va < 0x2345000){
        return va + 0x100000;
    }
    else{
        return -1;
    }
}

/** invalidate all cache lines in the TLB */
void tlb_clear(){
    for(int i = 0; i < SET; i++){
        for(int j = 0; j < WAY; j++){
            cache[i][j].valid = 0;
            cache[i][j].LRU = 1;
        }
    }
}

/**
 * return 0 if this virtual address does not have a valid
 * mapping in the TLB. Otherwise, return its LRU status: 1
 * if it is the most-recently used, 2 if the next-to-most,
 * etc.
 */
int tlb_peek(size_t va){
    size_t index = (va >> POBITS) & INDEX_MASK;
    size_t tag = va >> (POBITS + INDEX_BITS); 
    for(int i = 0; i < WAY; i++){
        if(cache[index][i].tag == tag){
            if(cache[index][i].valid == 1){
                return (5 - cache[index][i].LRU); 
            }
        }
    }
    return 0;
}

/**
 * If this virtual address is in the TLB, return its
 * corresponding physical address. If not, use
 * `translate(va)` to find that address, store the result
 * in the TLB, and return it. In either case, make its
 * cache line the most-recently used in its set.
 *
 * As an exception, if translate(va) returns -1, do not
 * update the TLB: just return -1.
 */
size_t tlb_translate(size_t va){
    size_t transl;
    size_t index = (va >> POBITS) & INDEX_MASK;
    size_t tag = va >> (POBITS + INDEX_BITS); 
    size_t LRU_of_hit;
    size_t index_of_oldest;
    int hit = 0;
    //calculates if its a hit
    for(int i = 0; i < WAY; i++){
        if(cache[index][i].tag == tag){
            if(cache[index][i].valid == 1){
                transl = cache[index][i].ppn;
                LRU_of_hit = cache[index][i].LRU;
                hit = 1;
                index_of_oldest = i;
            }
        }
        //this could be wrong
        if(cache[index][i].LRU == 1 && !hit){
            index_of_oldest = i;
        }
    }
    if(hit == 0){
        transl = translate(va & (0xefffffff << POBITS));
        if(transl == -1){
            return -1;
        }
        cache[index][index_of_oldest].ppn = transl;
        cache[index][index_of_oldest].tag = tag;
        cache[index][index_of_oldest].valid = 1;
        LRU_of_hit = 1;
    }
    for(int i = 0; i < WAY; i++){
        if(cache[index][i].LRU > LRU_of_hit){
            cache[index][i].LRU--;
        }
    }
    cache[index][index_of_oldest].LRU = 4;
    return transl + (va & OFFSET_MASK);
}
int main(){
    // tlb_clear();
    // assert(tlb_peek(0) == 0);
    // assert(tlb_translate(0) == 0x0020000);
    // assert(tlb_peek(0) == 1);

    // assert(tlb_translate(0x200) == 0x20200);
    // assert(tlb_translate(0x400) == 0x20400);
    // assert(tlb_peek(0) == 1);
    // assert(tlb_peek(0x200) == 1);
    // assert(tlb_translate(0x2001200) == 0x2101200);
    // assert(tlb_translate(0x0005200) == 0x0025200);
    // assert(tlb_translate(0x0008200) == 0x0028200);
    // assert(tlb_translate(0x0002200) == 0x0022200);
    // assert(tlb_peek(0x2001000) == 1);
    // assert(tlb_peek(0x0001000) == 0);
    // assert(tlb_peek(0x0004000) == 0);
    // assert(tlb_peek(0x0005000) == 1);
    // assert(tlb_peek(0x0008000) == 1);
    // assert(tlb_peek(0x0002000) == 1);
    // assert(tlb_peek(0x0000000) == 1);
    // tlb_clear();
    // assert(tlb_peek(0x2001000) == 0);
    // assert(tlb_peek(0x0005000) == 0);
    // assert(tlb_peek(0x0008000) == 0);
    // assert(tlb_peek(0x0002000) == 0);
    // assert(tlb_peek(0x0000000) == 0);
    // assert(tlb_translate(0) == 0x20000);
    // assert(tlb_peek(0) == 1);

    tlb_clear();
    assert(tlb_translate(0x0001200) == 0x0021200);
    assert(tlb_translate(0x2101200) == 0x2201200);
    assert(tlb_translate(0x0801200) == 0x0821200);
    assert(tlb_translate(0x2301200) == 0x2401200);
    assert(tlb_translate(0x0501200) == 0x0521200);
    assert(tlb_translate(0x0A01200) == 0x0A21200);
    assert(tlb_peek(0x0001200) == 0);
    assert(tlb_peek(0x2101200) == 0);
    assert(tlb_peek(0x2301200) == 3);
    assert(tlb_peek(0x0501200) == 2);
    assert(tlb_peek(0x0801200) == 4);
    assert(tlb_peek(0x0A01200) == 1);
    assert(tlb_translate(0x2301800) == 0x2401800);
    assert(tlb_peek(0x0001000) == 0);
    assert(tlb_peek(0x2101000) == 0);

    size_t f = tlb_peek(0x2301000);
    printf("%zu /n", f );

    assert(tlb_peek(0x2301000) == 1);

    assert(tlb_peek(0x0501000) == 3);
    assert(tlb_peek(0x0801000) == 4);
    assert(tlb_peek(0x0A01000) == 2);
    assert(tlb_translate(0x404000) == 0x424000);
    tlb_clear();
    assert(tlb_peek(0x301000) == 0);
    assert(tlb_peek(0x501000) == 0);
    assert(tlb_peek(0x801000) == 0);
    assert(tlb_peek(0xA01000) == 0);
    assert(tlb_translate(0xA01200) == 0xA21200);
    return 1;
}