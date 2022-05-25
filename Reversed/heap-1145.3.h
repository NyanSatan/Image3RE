#define HEAP_BLOCKSIZE 0x40UL
#define NUM_OF_BINS 32

struct heap_block {
    unsigned prev_size;
#define FLAG_THIS_FREE (1<<0)
#define FLAG_PREV_FREE (1<<1)
    unsigned cur_size;
    unsigned char pad[HEAP_BLOCKSIZE - 2 * sizeof(unsigned)];
} __attribute__((packed));

struct free_list {
	struct free_block *fwd;
	struct free_list *bkd;
} __attribute__((packed));

struct free_block {
	struct heap_block block;
	struct free_list free_list;
} __attribute__((packed));

typedef struct heap_block heap_block_t;
typedef struct free_list  free_list_t;
typedef struct free_block free_block_t;

unsigned int 	compute_bin(unsigned int size);
void *		 	heap_malloc(unsigned int size);
void 		 	heap_free(void *buffer);
