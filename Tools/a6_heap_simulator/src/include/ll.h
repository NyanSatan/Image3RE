#include <sys/types.h>

struct ll {
	struct ll *next;
	struct ll *prev;
};

struct ll *ll_add_node(struct ll *start_node, size_t struct_size);
void ll_remove_node(struct ll *node);

#define ll_iterate(node, code) \
	struct ll *iter = node; \
	while ((iter = iter->next)) { \
		code \
	}
