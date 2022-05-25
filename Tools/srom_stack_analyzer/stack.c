#include <stack.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include <utils.h>

int stack_analyze(const platform_t *platform, const platform_stack_t *stack, uint32_t sp, uint32_t fp, void *buf) {

#define BUF_OFFSET_TO_MEM(off)  (sp + off)
#define MEM_OFFSET_TO_BUF(off)  (off - sp)

    const uint32_t stack_size = stack->offset - (sp - platform->sram_base);
    uint32_t sp_off = 0;
    uint32_t fp_off = MEM_OFFSET_TO_BUF(fp);
    bool last_one = false;
    const function_t *prev_function = NULL;
    uint32_t counter = 0;

    printf("this is \"%s\" stack (0x%x - 0x%x)\n", stack->name,
                                                   stack->offset + platform->sram_base,
                                                   stack->offset + platform->sram_base - stack->length);
    
    while (1) {
        uint32_t f_size = fp_off - sp_off;

        if (f_size + sp_off + sizeof(stack_footer_t) == stack_size) {
            last_one = true;
        }

        if (f_size + sp_off > stack_size) {
            printf("bad FP - 0x%x\n", BUF_OFFSET_TO_MEM(fp_off));
            return EINVAL;
        }

        stack_footer_t *footer = buf + fp_off;

        printf("#%d stack frame @ 0x%x (size 0x%x)", counter, BUF_OFFSET_TO_MEM(sp_off), f_size);

        if (prev_function) {
            printf(" - %s()\n", prev_function->name);
        } else {
            printf("\n");
        }
        
        uint32_t normalized_lr = platform_normalize_lr(footer->lr);
        prev_function = platform_get_function_by_lr(platform, normalized_lr);

        hexdump(buf + sp_off, f_size, BUF_OFFSET_TO_MEM(sp_off));

        printf("\n");

        if (last_one) {
            return 0;
        }

        sp_off += f_size + sizeof(stack_footer_t);
        fp_off = MEM_OFFSET_TO_BUF(footer->fp);

        counter++;
    }

}
