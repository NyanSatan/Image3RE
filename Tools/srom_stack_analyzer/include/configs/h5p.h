#ifndef H5P_H
#define H5P_H

#include <platform.h>
#include <configs/h5p_map.h>

platform_t h5p_platform = {
    .name = "h5p_c0",
    .sram_base = 0x10000000,
    .sram_len = 0x80000,
    .map = (const function_t *)&h5p_map,
    .map_size = sizeof(h5p_map) / sizeof(function_t),
    .stacks = {
        {
            .name = "svc",
            .offset = 0x7C000,
            .length = 0x2800
        },
        {
            .name = "exc",
            .offset = 0x79800,
            .length = 0x400
        },
        {
            .name = "exc_fiq",
            .offset = 0x79400,
            .length = 0x400
        }
    }
};

#endif
