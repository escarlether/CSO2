#include <stdlib.h>
#include <stdalign.h>
#include <string.h>
#include "mlpt.h"
#include "config.h"

size_t ptbr = 0;
int vpn_bit_size = POBITS - 3;
size_t pagetable_address;
int page_size = 1 << POBITS;
//This calculates the mask which would normally use vpn_bit_size to shift
int vpn_mask = (1 << (POBITS - 3)) - 1;

/*Tranlate uses LEVELS number of VPNs to index into LEVELS number of pagetables to extract the PPN. If any entry is not valid or if ptbr does not exist, 
then it returns a value of all 1's. Finally, it returns the physical address by concatenating the offset with the PPN*/
size_t translate(size_t va){
    //Checks if ptbr has been instantiated
    if (ptbr == 0) return -1;
    pagetable_address = ptbr;
    int offset = va & ((1 << POBITS) - 1);
    //Loops through every level, extracting the vpn, getting the next page table and repeating until it gets the final PPN
    for (int i = 1; i <= LEVELS; ++i){ 
        /*Calculates the VPN by shifting va to the right by POBITS plus the inverse of whatever 
        level we are on scaled to the size of the VPN. Then it extracts the VPN with a mask*/
        size_t vpn = (va >> (POBITS + ((LEVELS - i) * vpn_bit_size))) & vpn_mask;
        //Checks to see if the page has a valid bit of 1 set
        if ((((size_t *) pagetable_address)[vpn] & 1) == 0) return -1;
        //assigns the next pagetabe address to the entry of the current one at index vpn without the valid bit
        pagetable_address = ((size_t *) pagetable_address)[vpn] - 1;
    }
    return pagetable_address + offset;
}

/*page_allocate will allcocate LEVELS number of pagetables if the PT is invalid, including the ptbr.*/
void page_allocate(size_t va){
    size_t *pointer_to_page; //This is the pointer that will store the pointer to the next pagetable
    if (ptbr == 0){
        posix_memalign((void**) &pointer_to_page, page_size, page_size);
        memset(pointer_to_page, 0, page_size);
        ptbr = (size_t) pointer_to_page;
    }
    pagetable_address = ptbr;
    for (int i = 1; i <= LEVELS; ++i){ //This loops through all of the LEVELS of the pagetable
        size_t vpn = (va >> (POBITS + ((LEVELS - i) * vpn_bit_size))) & vpn_mask;
        if ((((size_t *) pagetable_address)[vpn] & 1) == 0){ //If the pagetable doesn't exist, make it
            posix_memalign((void**) &pointer_to_page, page_size, page_size);
            memset(pointer_to_page, 0, page_size); //Makes sure that everything is a zero
            ((size_t *) pagetable_address)[vpn] = (size_t) pointer_to_page + 1;
        }
        //Sets the next pagetable address to be the entry of the current one
        pagetable_address = ((size_t *) pagetable_address)[vpn] - 1;
    }
}
//This function deallocates the most specific page using the given va
void deallocate(size_t va){
    size_t last_pagetable;
    for(int i = 1; i < LEVELS; ++i){
        size_t vpn = (va >> (POBITS + ((LEVELS - i) * vpn_bit_size))) & vpn_mask;
        pagetable_address = ((size_t *) pagetable_address)[vpn] - 1;

    }
    size_t last_vpn = (va >> POBITS) & vpn_mask;
    ((size_t *)pagetable_address)[last_vpn] -= 1;
    free((void *)((size_t *)pagetable_address)[last_vpn]);
}
int main(){
    return 0;
}