#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    const char *name;
    uint32_t offset;
} function_t;

typedef function_t platform_map_t[];

typedef struct {
    const char *name;
    uint32_t offset;    //relative to SRAM base
    uint32_t length;
} platform_stack_t;

typedef struct {
    uint32_t fp;
    uint32_t lr;
} stack_footer_t;


#define NUM_OF_STACKS   3

typedef struct {
    const char *name;
    uint32_t sram_base;
    uint32_t sram_len;
    const function_t *map;
    size_t map_size;
    platform_stack_t stacks[NUM_OF_STACKS];
} platform_t;

uint32_t platform_normalize_lr(uint32_t lr);
const function_t *platform_get_function_by_lr(const platform_t *platform, uint32_t lr);
const platform_stack_t *platform_get_stack_for_sp_fp(const platform_t *platform, uint32_t sp, uint32_t fp);

#endif
