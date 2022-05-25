//
//  utils.c
//  heap_parse
//
//  Created by noone on 8/1/20.
//  Copyright © 2020 noone. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

bool is_symbol(char symbol) {
    if (symbol >= 0x20 && symbol <= 0x7e)
        return true;
    else
        return false;
}

void hexdump(uint8_t *address, uint32_t base, size_t length) {
    for (size_t i = 0; i < length;) {
        printf("0x%08lx:", base + i);

        size_t left = 0;

        for (size_t z = 0; (z < 16) && (i < length); z++) {
            printf(" ");
            printf("%02x", *(address + i++));
            left++;
        }

        i -= left;

        for (size_t f = 0; f < 16 - left; f++) {
            printf("   ");
        }

        printf("  ");

        for (size_t d = 0; d < left; d++) {
            char symbol = *(address + i++);

            if (is_symbol(symbol))
                printf("%c", symbol);
            else
                printf(".");
        }

        printf("\n");
    }
}
