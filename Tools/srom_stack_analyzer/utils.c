/*
 * TODO: rewrite this all
 */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

bool is_symbol(char symbol) {
	if (symbol >= 0x20 && symbol <= 0x7e) 
		return true;
	else 
		return false;
}

/*
 * Hacky hexdump - do not use for non-4-aligned sizes
 */

void hexdump(uint8_t *address, size_t length, uint32_t base) {
	for (size_t i = 0; i < length;) {
		printf("0x%08lx:", base + i);

		size_t left = 0;

		for (size_t z = 0; (z < 4) && (i < length); z++) {
            printf(" ");
            printf("%08x", *(uint32_t *)(address + i));
            i += 4;
            left += 4;
		}

		i -= left;

		for (size_t f = 0; f < 4 - left / 4 ; f++) {
			printf("         ");
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
