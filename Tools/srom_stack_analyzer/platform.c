#include <platform.h>

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

const function_t *platform_get_function_by_lr(const platform_t *platform, uint32_t lr) {
    const function_t *curr_map = platform->map;
    size_t curr_map_size = platform->map_size;

    while (1) {
        curr_map_size = curr_map_size / 2;

        const function_t *current = &curr_map[curr_map_size];
        const function_t *next = NULL;

        if (current != platform->map + platform->map_size - 1) {
            next = current + 1;
        }
        
        if (lr >= current->offset) {
            if (next) {
                if (lr <= next->offset) {
                    return current;
                }
            } else {
                return current;
            }

            curr_map = next;
        }
    }
}

uint32_t platform_normalize_lr(uint32_t lr) {
    if (lr & 1) {
        return lr - 1;
    } else {
        return lr;
    }
}

const platform_stack_t *platform_get_stack_for_sp_fp(const platform_t *platform, uint32_t sp, uint32_t fp) {
    sp -= platform->sram_base;
    fp -= platform->sram_base;

    for (size_t i = 0; i < NUM_OF_STACKS; i++) {
        const platform_stack_t *stack = &platform->stacks[i];

        if (sp <= stack->offset && sp > stack->offset - stack->length) {
            if (fp <= stack->offset && fp >= sp) {
                return stack;
            } else {
                printf("FP outside of stack bounds?!\n");
                return NULL;
            }
        }
    }
    
    return NULL;
}