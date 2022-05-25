#include <stdint.h>
#include <dummy_heap.h>

struct scenario_event {
	int id;
	const struct scenario_event *first;
	const struct scenario_event *second;
	dummy_heap_operation_t op;
	unsigned size; 
	int free_id; //depending on op - this or the previous
	char desc[64];
};

struct scenario_step {
	uint32_t id;
	dummy_heap_operation_t op;
	uint32_t size;
	uint32_t free_id;
	char desc[64];
} __attribute__((packed));

int generate_scenarios(const struct scenario_event *root_event, int max_root_event_enters);

int update_result_folder();
int play_scenario(const char *path, size_t heap_size);
