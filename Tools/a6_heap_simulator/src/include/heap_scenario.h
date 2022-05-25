#include <scenario.h>

#define ROOT_EVENT_ENTERS 	4
#define HEAP_SIZE		  	0x16000

const struct scenario_event memz_init;
const struct scenario_event memz_deinit;
const struct scenario_event main_image3_handle_deinit;

const struct scenario_event trampoline_init = {
	.id = 12,
	.first = NULL,
	.second = NULL,
	.op = DUMMY_HEAP_MALLOC,
	.size = 0x40,
	.desc = "Boot trampoline init"
};

const struct scenario_event main_image3_handle_deinit_final = {
	.id = 11,
	.first = &trampoline_init,
	.second = NULL,
	.op = DUMMY_HEAP_FREE,
	.free_id = 3,
	.desc = "Final main Image3 handle struct de-init"
};

const struct scenario_event nonce_deinit = {
	.id = 10,
	.first = &main_image3_handle_deinit_final,
	.second = NULL,
	.op = DUMMY_HEAP_FREE,
	.free_id = 9,
	.desc = "Nonce buffer de-init"
};

const struct scenario_event nonce_init = {
	.id = 9,
	.first = &nonce_deinit,
	.second = NULL,
	.op = DUMMY_HEAP_MALLOC,
	.size = 4000,
	.desc = "Nonce buffer init"
};

const struct scenario_event nested_image3_buffer_init_big = {
	.id = 8,
	.first = &nonce_init,
	.second = &main_image3_handle_deinit,
	.op = DUMMY_HEAP_CALLOC,
	.size = 0x84,
	.desc = "Nested Image3 buffer struct init (big)"
};

const struct scenario_event nested_image3_buffer_init_small = {
	.id = 7,
	.first = &nonce_init,
	.second = &main_image3_handle_deinit,
	.op = DUMMY_HEAP_CALLOC,
	.size = 0x68,
	.desc = "Nested Image3 buffer struct init (small)"
};

const struct scenario_event nested_image3_handle_init = {
	.id = 5,
	.first = &nested_image3_buffer_init_big,
	.second = &nested_image3_buffer_init_small,
	.op = DUMMY_HEAP_CALLOC,
	.size = 0x10,
	.desc = "Nested Image3 handle struct init"
};

const struct scenario_event main_image3_handle_deinit = {
	.id = 4,
	.first = &memz_deinit,
	.second = NULL,
	.op = DUMMY_HEAP_FREE,
	.free_id = 3,
	.desc = "Main Image3 handle struct de-init"
};

const struct scenario_event main_image3_handle_init = {
	.id = 3,
	.first = &nested_image3_handle_init,
	.second = &main_image3_handle_deinit,
	.op = DUMMY_HEAP_CALLOC,
	.size = 0x10,
	.desc = "Main Image3 handle struct init"
};

const struct scenario_event memz_deinit = {
	.id = 2,
	.first = &memz_init,
	.second = NULL,
	.op = DUMMY_HEAP_FREE,
	.free_id = 1,
	.desc = "Memz struct de-init"
};

const struct scenario_event memz_init = {
	.id = 1,
	.first = &main_image3_handle_init,
	.second = &memz_deinit,
	.op = DUMMY_HEAP_MALLOC,
	.size = 0x14,
	.desc = "Memz struct init"
};
