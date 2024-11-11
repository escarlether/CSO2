#define _GNU_SOURCE
#include "util.h"
#include <stdio.h>      // for printf
#include <stdlib.h>     // for atoi (and malloc() which you'll likely use)
#include <sys/mman.h>   // for mmap() which you'll likely use
#include <stdalign.h>

alignas(4096) volatile char global_array[4096 * 32];

void labStuff(int which) {
    if (which == 0) {
        /* do nothing */
    } else if (which == 1) {
        //one page
        global_array[0]='a';
        //2 page
        global_array[4096]='a';
    } else if (which == 2) {
        //this makes resident set size be 4096 which means that one page is being allocatedd
        char * alloc = malloc(1000000);
        alloc[0]='a';
    } else if (which == 3) {
        //i need to allocate 32 pages so i will access that in a for loop
        char *ptr;
        ptr = mmap(NULL, /* hint address */
                1048576/* length */,
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS,
                -1, /* file descriptor (-1 for "none") */
                0
            );

        
        int c =0;
        for(int i=0;i<32;i++){
            global_array[c]='a';
            c+=4096;
        }
    } else if (which == 4) {
            char *ptr;
            ptr = mmap((void *)0x5555555bc000 + 0x200000,
                    4096,
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE,
                    -1,
                        0
                );
            ptr[0] = 'a';
    }else if (which ==5){
        char *ptr;
            ptr = mmap((void *)0x5555555bb000 + 0x10000000000,
                    4096,
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE,
                    -1,
                        0
                );
            ptr[0] = 'a';
    }
}

int main(int argc, char **argv) {
    int which = 0;
    if (argc > 1) {
        which = atoi(argv[1]);
    } else {
        fprintf(stderr, "Usage: %s NUMBER\n", argv[0]);
        return 1;
    }
    printf("Memory layout:\n");
    print_maps(stdout);
    printf("\n");
    printf("Initial state:\n");
    force_load();
    struct memory_record r1, r2;
    record_memory_record(&r1);
    print_memory_record(stdout, NULL, &r1);
    printf("---\n");

    printf("Running labStuff(%d)...\n", which);

    labStuff(which);

    printf("---\n");
    printf("Afterwards:\n");
    record_memory_record(&r2);
    print_memory_record(stdout, &r1, &r2);
    print_maps(stdout);
    return 0;
}
