#ifndef _myfile_h
#define _myfile_h

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

struct malloc_info
{
    uint32_t alloc_size; /// velikost
    int st_odsekov; // kolko odsekov imamo
    void* prvi_prost; /// kje je prvi prost
};

struct odsek
{
    void* zac_strani; /// kje se zacne stran
    uint32_t odsek_velikost; /// kako velik je odsek
};


void *mymalloc(size_t); /// ustvari
void myfree(void*); /// sprosti 

///void function(); /// izpis avtorja

#endif
