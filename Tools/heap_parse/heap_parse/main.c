//
//  main.c
//  heap_parse
//
//  Created by noone on 8/1/20.
//  Copyright © 2020 noone. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

void hexdump(uint8_t *address, uint32_t base, size_t length);

typedef struct {
    uint8_t flags : 2;
    uint32_t prev_size : 30;
    uint32_t cur_size;
} __attribute__((packed)) heap_header_t;

int main(int argc, const char * argv[]) {
    if (argc != 5) {
        printf("usage: %s <file> <startaddr> <size> <blocksize>\n", argv[0]);
        printf("ALL VALUES MUST BE IN HEX\n");
        return -1;
    }
    
    const char *file = argv[1];
    
    int fd = open(file, O_RDONLY);
    if (fd < 0) {
        printf("failed to open file\n");
        return -1;
    }
    
    unsigned long length = lseek(fd, 0, SEEK_END);
    
    void *buffer = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
    
    unsigned long start_addr = strtoul(argv[2], NULL, 16);
    unsigned long size = strtoul(argv[3], NULL, 16);
    unsigned long blocksize = strtoul(argv[4], NULL, 16);
    unsigned long base_addr = start_addr & 0xFFF00000;
    
    if (length < start_addr + size - base_addr) {
        printf("file is too small\n");
        return -1;
    }
    
    unsigned long initial_offset = start_addr - base_addr;
    
    for (unsigned long offset = initial_offset, i = 0; offset < size + initial_offset; i++) {
        heap_header_t *heap_object = buffer + offset;
        
        void *addr = (void *)(base_addr + offset + blocksize);
        unsigned long cur_size = heap_object->cur_size * blocksize;
        unsigned long cur_size_buf = cur_size - blocksize;
        
        printf("%lu: hdr: 0x%lx, addr: %p, size: 0x%lx, flags: 0x%x\n", i, (void*)heap_object - buffer + base_addr, addr, cur_size_buf, heap_object->flags);
        
        printf("header:\n");
        hexdump((uint8_t *)heap_object, (uint32_t)(addr - blocksize), blocksize);
        
        if (cur_size_buf) {
            printf("buffer:\n");
            hexdump((uint8_t *)heap_object + blocksize, (uint32_t)addr, cur_size_buf);
        }
        
        printf("\n");
        
        offset += cur_size;
    }
    
    munmap(buffer, length);
    
    return 0;
}
