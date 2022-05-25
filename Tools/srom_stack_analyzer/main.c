#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <platform.h>
#include <stack.h>
#include <configs/h5p.h>

const platform_t *valid_platforms[] = {
    &h5p_platform
};

static void print_usage(const char *program_name) {
    printf("usage: %s -i <input> -p <platform> -s <sp> -f <fp>\n", program_name);
    printf("available platforms:");

    for (size_t i = 0; i < sizeof(valid_platforms) / sizeof(*valid_platforms); i++) {
        printf(" %s", valid_platforms[i]->name);
    }
    
    printf("\n");
}

const char *args = "i:p:s:f:";

int parse_numeric_arg(const char *arg, int base, uint64_t *val, uint64_t max_val) {
    char *stop;
    uint64_t result = strtoull(arg, &stop, base);
    if (*stop || result > max_val) {
        return -1;
    }
    
    *val = result;
    
    return 0;
}

int main(int argc, const char *argv[]) {
    /* parsing args */
    const char *input = NULL;
    const platform_t *platform = NULL;
    uint32_t sp = 0;
    uint32_t fp = 0;

    char c;
    while ((c = getopt(argc, (char *const *)argv, args)) != -1) {
        switch (c) {
            case 'i':
                input = optarg;
                break;
            
            case 'p': {
                /* finding platform descriptor */
                const char *platform_name = optarg;

                for (size_t i = 0; i < sizeof(valid_platforms) / sizeof(*valid_platforms); i++) {
                    if (strcmp(platform_name, valid_platforms[i]->name) == 0) {
                        platform = valid_platforms[i];
                        break;
                    }
                }

                if (!platform) {
                    printf("unknown platform\n");
                    return EINVAL;
                }

                break;
            }

            case 's': {
                uint64_t _sp;
                if (parse_numeric_arg(optarg, 16, &_sp, UINT32_MAX) != 0) {
                    printf("invalid SP\n");
                    return EINVAL;
                }

                if (_sp % 4) {
                    printf("unaligned SP\n");
                    return EINVAL;
                }

                sp = (uint32_t)_sp;

                break;
            }

            case 'f': {
                uint64_t _fp;
                if (parse_numeric_arg(optarg, 16, &_fp, UINT32_MAX) != 0) {
                    printf("invalid FP\n");
                    return EINVAL;
                }

                if (_fp % 4) {
                    printf("unaligned FP\n");
                    return EINVAL;
                }
                
                fp = (uint32_t)_fp;

                break;
            }

            case '?':
                print_usage(argv[0]);
                return EINVAL;

            default:
                abort();
                break;
        }
    }

    if (!input || !platform || !sp || !fp) {
        printf("not enough args\n");
        print_usage(argv[0]);
        return EINVAL;
    }

    int fd = open(input, O_RDONLY);
    if (fd < 0) {
        printf("failed to open input file\n");
        return errno;
    }
    
    off_t length = lseek(fd, 0, SEEK_END);
    if (length != platform->sram_len) {
        printf("bad SRAM dump - length doesn't match (want - 0x%x, got - 0x%llx)\n", platform->sram_len, length);
        close(fd);
        return EINVAL;
    }
    
    const platform_stack_t *stack = platform_get_stack_for_sp_fp(platform, sp, fp);
    if (!stack) {
        printf("SP/FP do not belong to any known stack\n");
        close(fd);
        return EINVAL;
    }
    
    uint32_t result_offset = sp - platform->sram_base;
    uint32_t result_length = stack->offset - result_offset;

    uint8_t buf[result_length];

    ssize_t amount_read = pread(fd, buf, result_length, result_offset);
    close(fd);
    if (amount_read != result_length) {
        printf("failed to read from input file\n");
        return errno;
    }

    return stack_analyze(platform, stack, sp, fp, buf);
}
