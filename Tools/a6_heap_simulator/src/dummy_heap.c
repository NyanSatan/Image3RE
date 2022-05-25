#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <heap-1145.3.h>
#include <dummy_heap.h>


void *gHeap = NULL;
unsigned int gHeap_size = 0;
free_block_t **gHeapBins = NULL;
size_t gHeapBins_size = 0;
unsigned int gHeapAvailable = 0;


void dummy_heap_init(unsigned int size) {
	/*
	 * setting up buffers
	 */

	gHeap_size = size;

	gHeap = malloc(size);
	assert(gHeap != NULL);
	memset(gHeap, 0x0, size);

	gHeapBins_size = sizeof(free_block_t *) * NUM_OF_BINS;
	gHeapBins = malloc(gHeapBins_size);
	assert(gHeapBins != NULL);
	memset(gHeapBins, 0x0, gHeapBins_size);

	/*
	 * setting up top block
	 */

	heap_block_t *top_block = (heap_block_t *)gHeap;
	top_block->prev_size = 0;
	top_block->cur_size = 1;

	/*
	 * setting up bottom block
	 */

	heap_block_t *bottom_block = (heap_block_t *)(gHeap + size - HEAP_BLOCKSIZE);
	bottom_block->cur_size = 1;
	bottom_block->prev_size = (0 << 2) | FLAG_PREV_FREE;

	/*
	 * setting up middle (free) block
	 */

	heap_block_t *free_block = (heap_block_t *)(gHeap + HEAP_BLOCKSIZE);
	free_block->prev_size = (top_block->cur_size << 2) | FLAG_THIS_FREE;
	free_block->cur_size = (size - (top_block->cur_size + bottom_block->cur_size * HEAP_BLOCKSIZE)) / HEAP_BLOCKSIZE; 
	bottom_block->prev_size = (bottom_block->prev_size & (FLAG_PREV_FREE | FLAG_THIS_FREE)) | (free_block->cur_size << 2);

	unsigned int suitable_bin = compute_bin(free_block->cur_size * HEAP_BLOCKSIZE);
	free_block_t **suitable_bin_ptr = gHeapBins + suitable_bin;
	*suitable_bin_ptr = (free_block_t *)free_block;
	((free_block_t *)free_block)->free_list.bkd = (free_list_t *)suitable_bin_ptr;

	gHeapAvailable = free_block->cur_size;

	/*
	 * setting up initial buffers
	 */

	heap_malloc(HEAP_BLOCKSIZE);
	heap_malloc(0x100);
}

void dummy_heap_operation(dummy_heap_operation_t op, void *arg, void **res) {
	switch (op) {
		case DUMMY_HEAP_MALLOC:
			*res = heap_malloc(*((unsigned int *)arg));
			break;

		case DUMMY_HEAP_CALLOC:
			*res = heap_malloc(*((unsigned int *)arg));
			memset(*res, 0, *((unsigned int *)arg));
			break;

		case DUMMY_HEAP_FREE:
			heap_free(arg);
			break;

		default:
			abort();
	}
}

void dummy_heap_free() {
	free((void *)gHeapBins);
	gHeapBins = NULL;

	free((void *)gHeap);
	gHeap = NULL;

	gHeap_size = 0;
	gHeapBins_size = 0;
	gHeapAvailable = 0;
}

void write_file(const char *file, void *buffer, size_t size) {
	int fd = open(file, O_WRONLY | O_CREAT, 0644);
	assert(fd >= 0);
	assert(write(fd, buffer, size) == size);
	close(fd);

	printf("written %p (0x%x) to \"%s\"\n", buffer, size, file);
}

void dummy_heap_dump_to_files(const char *heap_file, const char *bins_file) {
	write_file(heap_file, gHeap, gHeap_size);
	write_file(bins_file, gHeapBins, gHeapBins_size);
}
