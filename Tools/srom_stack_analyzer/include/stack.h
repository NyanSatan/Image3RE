#ifndef STACK_H
#define STACK_H

#include <platform.h>

int stack_analyze(const platform_t *platform, const platform_stack_t *stack, uint32_t sp, uint32_t fp, void *buf);

#endif
