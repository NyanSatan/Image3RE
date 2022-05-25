typedef enum {
	DUMMY_HEAP_MALLOC = 0,
	DUMMY_HEAP_CALLOC,
	DUMMY_HEAP_FREE
} dummy_heap_operation_t;

void dummy_heap_init(unsigned int size);
void dummy_heap_free();
void dummy_heap_dump_to_files(const char *heap_file, const char *bins_file);
void dummy_heap_operation(dummy_heap_operation_t op, void *arg, void **res);
