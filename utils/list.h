#pragma once
#include "callbacks.h"

struct list_node {
	struct list_node *prev;
	struct list_node *next;
	void *data;
};

typedef struct list_node list_node;

typedef struct list_t {
	list_node *head;
	list_node *tail;
	size_t length;
	copy_constructor_type copy_constructor;
	destructor_type destructor;
	compare_type compare;
} list_t;


list_t *list_create(copy_constructor_type copy_constructor,
					destructor_type destructor, 
					compare_type compare);

void list_create2(void *dest, copy_constructor_type copy_constructor,
					destructor_type destructor, 
					compare_type compare);

list_node* list_insert_tail(list_t *list, void *data);

list_node* list_insert_head(list_t *list, void *data);

list_node *list_at(list_t *list, size_t pos);

list_node *list_insert_at(list_t *list, size_t pos, void *data);

char list_remove_head(list_t *list);

char list_remove_tail(list_t *list);

char list_remove_at(list_t *list, size_t pos);

char list_unlink_node(list_t *list, list_node *node);

list_node *list_find_node(list_t *list, void *data);

list_node *list_head(list_t *list);


void list_destroy(list_t *list);