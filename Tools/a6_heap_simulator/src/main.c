#include <stdio.h>
#include <string.h>
#include <heap_scenario.h>

void usage(const char *program) {
	printf("%s <VERB> [args]\n", program);
	printf("VERBs:\n");
	printf("\tgenerate\tgenerates scenario files\n");
	printf("\tplay\t\tplays scenario files from args\n");
}

int main(int argc, const char *argv[]) {
	if (argc == 2 && strcmp(argv[1], "generate") == 0) {
		return generate_scenarios(&memz_init, ROOT_EVENT_ENTERS);
	} else if (argc > 2 && strcmp(argv[1], "play") == 0) {
		if (update_result_folder() != 0)
			return -1;

		for (int i = 2; i < argc; i++)
			if (play_scenario(argv[i], HEAP_SIZE) != 0)
				return -1;

		return 0;
	} else {
		usage(argv[0]);
		return -1;
	}
}
