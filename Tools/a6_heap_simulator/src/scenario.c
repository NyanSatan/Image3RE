#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <bsd/stdlib.h>

#include <scenario.h>
#include <ll.h>

/*====================================
 * Scenario generation
 *====================================*/

#define SCENARIOS_FOLDER 	"scenarios"
#define SCENARIO_BASE_NAME	"scenario"
#define SCENARIO_INITIAL_ID 0
#define SCENARIO_NEW		-1
#define MAX_SCENARIOS		64

int add_event_to_file(int fd, const struct scenario_event *event, int scenario_id) {
	struct scenario_step step;

	step.id = event->id;
	step.op = event->op;

	switch (step.op) {
		case DUMMY_HEAP_MALLOC:
		case DUMMY_HEAP_CALLOC:
			step.size = event->size;
			step.free_id = -1;
			break;

		case DUMMY_HEAP_FREE:
			step.size = -1;
			step.free_id = event->free_id;
			break;

		default:
			printf("unknown heap op\n");
			return -1;
	}

	strncpy((char *)&step.desc, (char *)&event->desc, sizeof(step.desc));

	if (write(fd, &step, sizeof(step)) != sizeof(step)) {
		printf("failed to write\n");
		return -1;
	}

	printf("written to scenario %d, event: %s\n", scenario_id, event->desc);

	return 0;
}

int new_file(int prev_id,
			 int id) {
	if (id == MAX_SCENARIOS) {
		printf("scenario count exceeded\n");
		return -1;
	}

	bool need_file_copy = id != SCENARIO_INITIAL_ID;

	char *filename;

	if (asprintf(&filename, "%s/%s-%d", SCENARIOS_FOLDER, SCENARIO_BASE_NAME, id) == -1) {
		printf("out of memory!");
		return -1;
	}
	
	printf("creating: %s\n", filename);

	if (need_file_copy) {
		char *old_filename;

		if (asprintf(&old_filename, "%s/%s-%d", SCENARIOS_FOLDER, SCENARIO_BASE_NAME, prev_id) == -1) {
			printf("out of memory!");
			return -1;
		}

		char *copy_cmd;
		if (asprintf(&copy_cmd, "/bin/cp %s %s", old_filename, filename) == -1) {
			printf("out of memory!");
			return -1;
		}

		if (system(copy_cmd) != 0) {
			printf("failed to clone scenario\n");
			free(filename);
			free(old_filename);
			return -1;
		}

		free(old_filename);
	}

	int fd;

	if (need_file_copy)
		fd = open(filename, O_WRONLY);
	else
		fd = open(filename, O_WRONLY | O_CREAT, 0644);

	free(filename);

	lseek(fd, 0, SEEK_END);

	return fd;
}

int recursive_generation(const struct scenario_event *root_event,
						 int max_root_event_entries,
						 const struct scenario_event *current_event,
						 int root_event_entries,
						 int *global_id,
						 int fd,
						 int id) {

	if (fd < 0) {
		printf("failed to create/open file\n");
		return -1;
	}

	int ret = 0;

	if (current_event == root_event) {
		root_event_entries++;
	}

	if (root_event_entries > max_root_event_entries) {
		printf("ending scenario\n");
		goto out;
	}

	if (add_event_to_file(fd, current_event, id) != 0) {
		ret = -1;
		goto out;
	}

	int new_fd;
	int new_id;

	if (current_event->second) {
		new_id = ++(*global_id);
		new_fd = new_file(id, new_id);
	}


	if (current_event->first) {
		if (recursive_generation(
			root_event,
			max_root_event_entries,
			current_event->first,
			root_event_entries,
			global_id,
			fd,
			id
			) != 0) {

			printf("failure\n");
			ret = -1;
		}
	}

	if (current_event->second) {
		if (recursive_generation(
			root_event,
			max_root_event_entries,
			current_event->second,
			root_event_entries,
			global_id,
			new_fd,
			new_id
			) != 0) {

			printf("failure\n");
			ret = -1;
		}	
	}

out:
	close(fd);

	return ret;
}

int generate_scenarios(const struct scenario_event *root_event, int max_root_event_entries) {
	if (system("rm -rf " SCENARIOS_FOLDER) != 0) {
		printf("failed to remove current scenarios folder\n");
		return -1;
	}

	if (mkdir(SCENARIOS_FOLDER, 0755) != 0) {
		printf("failed to create scenarios directory\n");
		return -1;
	}

	int initial_id = SCENARIO_INITIAL_ID;

	int fd = new_file(SCENARIO_NEW, initial_id);

	return recursive_generation(
		root_event,
		max_root_event_entries,
		root_event,
		0,
		&initial_id,
		fd,
		initial_id
	);
}

/*====================================
 * Scenario playing
 *====================================*/

#define SCENARIO_FINAL_RESULTS_FOLDER	"scenario_results"

struct allocation {
	struct ll list;
	int id;
	void *ptr;
};

int update_result_folder() {
	if (system("rm -rf " SCENARIO_FINAL_RESULTS_FOLDER) != 0) {
		printf("failed to remove current scenario results folder\n");
		return -1;
	}

	if (mkdir(SCENARIO_FINAL_RESULTS_FOLDER, 0755) != 0) {
		printf("failed to create scenario results directory\n");
		return -1;
	}

	return 0;
}

int play_scenario(const char *path, size_t heap_size) {
	const char *filename = NULL;
	size_t full_path_len = strlen(path);

	for (int i = full_path_len - 1; i >= 0; i--)
		if (path[i] == '/')
			filename = &path[i] + 1;

	if (!filename)
		filename = path;
	
	printf("playing scenario: %s\n", filename);

	int fd = open(path, O_RDONLY);
	if (fd < 0) {
		printf("failed to open scenario\n");
		return -1;
	}

	size_t len = lseek(fd, 0, SEEK_END);
	if (len % sizeof(struct scenario_step)) {
		printf("invalid file size\n");
		close(fd);
		return -1;
	}

	void *buf = malloc(len);
	if (!buf) {
		printf("out of memory!\n");
		close(fd);
		return -1;
	}

	if (pread(fd, buf, len, 0) != len) {
		printf("failed to read\n");
		free(buf);
		close(fd);
		return -1;		
	}

	close(fd);

	int count = len / sizeof(struct scenario_step);
	printf("step count: %d\n", count);

	dummy_heap_init(heap_size);

	struct ll allocations = {
		.next = NULL,
		.prev = NULL
	};

	for (int i = 0; i < count; i++) {
		struct scenario_step *step = (struct scenario_step *)buf + i;
		printf("\taction: %s\n", step->desc);

		if (step->op == DUMMY_HEAP_FREE) {
			struct allocation *to_free = NULL;

			ll_iterate(&allocations, {
				if (((struct allocation *)iter)->id == step->free_id) {
					to_free = (struct allocation *)iter;
					break;
				}
			});

			if (!to_free) {
				printf("failed to find allocation to be free'd\n");
				return -1;
			}

			dummy_heap_operation(step->op, to_free->ptr, NULL);

			ll_remove_node((struct ll *)to_free);
		} else {
			void *res;
			unsigned int size = step->size;

			dummy_heap_operation(step->op, &size, &res);

			struct allocation *new = (struct allocation *)ll_add_node(&allocations, sizeof(struct allocation));
			new->id = step->id;
			new->ptr = res;

                        arc4random_buf(res, size);
		}
	}

	char *bins_filename, *heap_filename;

	if (asprintf(&bins_filename, SCENARIO_FINAL_RESULTS_FOLDER "/bins_%s", filename) == -1 ||
		asprintf(&heap_filename, SCENARIO_FINAL_RESULTS_FOLDER "/heap_%s", filename) == -1)
	{
		printf("out of memory!");
		return -1;
	}

	dummy_heap_dump_to_files(heap_filename, bins_filename);

	free(bins_filename);
	free(heap_filename);

	dummy_heap_free();

	free(buf);

	return 0;
}
