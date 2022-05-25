#include <ll.h>
#include <stdio.h>
#include <stdlib.h>

struct ll *ll_create_node(size_t struct_size) {
	return calloc(struct_size, 1);
}

struct ll *ll_last_node(struct ll *node) {
	struct ll *next;
	while ((next = node->next))
		node = next;

	return node;
}

struct ll *ll_add_node(struct ll *start_node, size_t struct_size) {
	struct ll *new = ll_create_node(struct_size);

	if (!new) {
		printf("ll: out of memory!\n");
		return NULL;
	}

	struct ll *last = ll_last_node(start_node);

	last->next = new;
	new->prev = last;

	return new;
}

void ll_remove_node(struct ll *node) {
	if (node->prev)
		node->prev->next = node->next;

	if (node->next)
		node->next->prev = node->prev;

	free(node);
}
