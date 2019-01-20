#include <stdlib.h>
#include "list.h"


list_t *list_create(copy_constructor_type copy_constructor,
					destructor_type destructor,
					compare_type compare) {
	list_t *list = malloc(sizeof(list_t));
	list->head = NULL;
	(list->tail = NULL);
	list->copy_constructor = copy_constructor;
	list->destructor = destructor;
	list->compare = compare;
	return list;
}

void list_create2(void *dest, copy_constructor_type copy_constructor,
					destructor_type destructor,
					compare_type compare) {
	list_t *list = (list_t*)dest;
	list->head = NULL;
	(list->tail = NULL);
	list->copy_constructor = copy_constructor;
	list->destructor = destructor;
	list->compare = compare;
}


list_node* list_insert_tail(list_t *list, void *data) {
	list_node *ins = calloc(1, sizeof(list_node));
	ins->data = list->copy_constructor(data);
	if (list->tail == NULL) {
		list->head = list->tail = ins;
		ins->prev = ins->next = NULL;
	}
	else {
		list->tail->next = ins;
		ins->prev = list->tail;
		list->tail = ins;
	}
	list->length++;
	return ins;
}

list_node* list_insert_head(list_t *list, void *data) {
	list_node *ins = calloc(1, sizeof(list_node));
	ins->data = list->copy_constructor(data);
	if (list->head == NULL) {
		list->head = list->tail = ins;
		ins->prev = ins->next = NULL;
	}
	else {
		list->head->prev = ins;
		ins->next = list->head;
		list->head = ins;
	}
	list->length++;
	return ins;
}


list_node *list_at(list_t *list, size_t pos) {
	if (list->head == NULL) return NULL;

	size_t idx = 0;
	list_node *current = list->head;
	while (idx < pos && current) {
		current = current->next;
		idx++;
	}
	if (idx != pos) return NULL;
	return current;
}

list_node *list_insert_at(list_t *list, size_t pos, void *data) {
	if (pos == 0) {
		return list_insert_head(list, data);
	}
	if (pos == list->length) {
		return list_insert_tail(list, data);
	};

	list_node *next = list_at(list, pos);
	if (!next) return NULL;
	list_node *ins = malloc(sizeof(list_node));
	ins->data = list->copy_constructor(data);
	next->prev->next = ins;
	ins->prev = next->prev;
	next->prev = ins;
	ins->next = next;
	list->length++;
	return ins;
}


char list_remove_head(list_t *list) {
	if (list->head == NULL) return -1;
	list_node *head = list->head;
	head->next->prev = NULL;
	list->head = head->next;
	list->destructor(head->data);
	list->length--;
	free(head);
	return 0;
}

char list_remove_tail(list_t *list) {
	if (list->tail == NULL) return -1;
	list_node *tail = list->tail;
	tail->prev->next = NULL;
	list->tail = tail->prev;
	list->destructor(tail->data);
	free(tail);
	list->length--;
	return 0;
}

char list_remove_at(list_t *list, size_t pos) {
	if (pos >= list->length) return -1;
	if (pos == 0) return list_remove_head(list);
	if (pos == list->length-1) return list_remove_tail(list);

	list_node *remove = list_at(list, pos);
	remove->prev->next = remove->next;
	remove->next->prev = remove->prev;
	list->destructor(remove->data);
	free(remove);
	list->length--;
	return 0;
}

char list_unlink_node(list_t *list, list_node *node) {
	if (node->prev) {
		node->prev->next = node->next;
	} else {
		list->head = node->next;
	}

	if (node->next) {
		node->next->prev = node->prev;
	} else {
		list->tail = node->prev;
	}
	list->length--;
	return 0;
}


list_node *list_find_node(list_t *list, void *data) {
	list_node *head = list->head;
	if (!head) return NULL;
	list_node *current = head;
	while (current) {
		if (list->compare(current->data, data) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void list_destroy(list_t *list) {
	list_node *head = list->head;

	while (head) {
		list_node *temp = head;
		head = head->next;
        list->destructor(temp->data);
		free(temp);
	}

	free(list);

}
list_node *list_head(list_t *list) {
	return list->head;
}
