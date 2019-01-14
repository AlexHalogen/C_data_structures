/**
* Vector Lab
* CS 241 - Fall 2018
*/

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "auto_array.h"
#include "callbacks.h"



typedef void (print_func)(void *elem);
typedef void (vprint_func)(int elem);

void array_stat(auto_array *vec, print_func *print);
void array_stat2(auto_array *vec, vprint_func *print);
void print_elem(print_func *print, void *elem);

void char_print(void* c);
void int_print(void* i); 
void int_noptr_print(int i);

int* int_array_get(auto_array *array, size_t position) {
	int *arr = array_front(array);
	return &arr[position];
}

int int_noptr_array_get(auto_array *array, size_t position) {
	int *arr = array_front(array);
	return arr[position];
}

int main(int argc, char *argv[]) {

	(void)argc;
	(void)argv;
	// simple creation
	printf("==============\n");
	printf("Simple creation\n");
	auto_array *int_vector = array_create(
		size4_copy_constructor2, size4_destructor , size4_default_constructor2, 4);
	array_reserve(int_vector, 10);
	array_stat(int_vector, NULL);
	printf("Vector empty: %i\n", array_empty(int_vector));

	int arr[10] = {1,2,3,4,5,6,7,8,9,0};
	int *rand = malloc(sizeof(int));
	*rand = random() % 10+1;

	// simple push
	printf("==============\n");
	printf("Simple push\n");
	array_push_back(int_vector, arr);
	array_push_back(int_vector, arr+3);
	
	array_stat(int_vector, &int_print);

	// push beyond capacity
	printf("==============\n");
	printf("Push over capacity\n");
	for (int i=0; i<20; i++) {
		array_push_back(int_vector, rand);
		rand[0] = random() % 10+1;
	}
	array_stat(int_vector, &int_print);

	// resize
	printf("==============\n");
	printf("Resize\n");
	array_resize(int_vector, 7);
	array_stat(int_vector, &int_print);
	array_resize(int_vector, 20);
	array_stat(int_vector, &int_print);
	// array_resize(int_vector, 400);
	// array_stat(int_vector, &int_print);
	
	array_resize(int_vector, 9);
	array_stat(int_vector, &int_print);

	
	// array_at, set, get
	printf("==============\n");
	printf("At get set\n");
	rand[0] = random() % 10+1;
	array_set(int_vector, 8, rand);
	// print_elem(&int_print,*(array_at(int_vector, 8)));
	print_elem(&int_print,int_array_get(int_vector, 8));
	rand[0] = random() % 10+1;
	array_set(int_vector, 8, rand);
	// print_elem(&int_print,*(array_at(int_vector, 8)));

	print_elem(&int_print,int_array_get(int_vector, 8));
	array_stat(int_vector, &int_print);

	array_resize(int_vector, 2);
	array_set(int_vector, 1, rand);
	print_elem(&int_print,int_array_get(int_vector, 1));
	array_stat(int_vector, &int_print);

	printf("Front: ");
	print_elem(&int_print,(int*)array_front(int_vector));

	// push
	for (int i=0; i<7;i++) {
		int i = random() % 10 +1;
		array_push_back(int_vector, &i);
	}
	array_resize(int_vector, 9);
	array_resize(int_vector, 9);
	array_stat(int_vector, &int_print);

	// pop push pop push
	printf("==============\n");
	printf("Pop push pop push\n");
	array_pop_back(int_vector);
	array_stat(int_vector, &int_print);
	rand[0] = random() % 10 +1;
	array_push_back(int_vector, rand);
	array_stat(int_vector, &int_print);
	array_pop_back(int_vector);
	array_stat(int_vector, &int_print);
	rand[0] = random() % 10 +1;
	array_push_back(int_vector, rand);
	array_stat(int_vector, &int_print);

	// Insert, erase
	printf("==============\n");
	printf("Insert, erase\n");
	for (int i=0; i<3; i++) {
		int *temp = malloc(sizeof(int));
		*temp = i;
		array_insert(int_vector, 0, temp);
		free(temp);
	}
	array_stat(int_vector, &int_print);
	/*for (int i=0; i<3; i++) {
		int *temp = malloc(sizeof(int));
		*temp = i;
		array_insert(int_vector, array_size(int_vector)-1, temp);
		free(temp);
	}
	array_stat(int_vector, &int_print);*/
	int *temp = malloc(sizeof(int));
	*temp = 99;
	array_insert(int_vector, 1, temp);
	array_insert(int_vector, 1, temp);
	array_stat(int_vector, &int_print);
	array_erase(int_vector, 1);
	array_erase(int_vector, 1);
	array_stat(int_vector, &int_print);

	array_insert(int_vector, array_size(int_vector), temp);
	array_insert(int_vector, array_size(int_vector), temp);
	array_stat(int_vector, &int_print);
	array_erase(int_vector, array_size(int_vector)-1);
	array_erase(int_vector, array_size(int_vector)-1);
	array_stat(int_vector, &int_print);

	free(temp);


	// clear, destory
	printf("==============\n");
	printf("clear and destroy\n");
	array_clear(int_vector);
	array_stat(int_vector, NULL);
	array_destroy(int_vector);
	free(rand);


	// More memory
	auto_array *a_vector = array_create(
		char_copy_constructor2, char_destructor , char_default_constructor2, 1);

	array_resize(a_vector, 100);
	array_stat(a_vector,NULL);
	array_resize(a_vector, 1);
	array_resize(a_vector, 100);
	array_resize(a_vector, 0);
	array_stat(a_vector,NULL);
	array_destroy(a_vector);

	// Shallow vector
	auto_array *s_vector = array_create(pointer_copy_constructor2, pointer_destructor, pointer_default_constructor2, sizeof(void*));

	printf("==============\n");
	printf("Shallow copy\n");
	int* source1 = malloc(sizeof(int));
	int* source2 = malloc(sizeof(int));
	*source1 = 22;
	*source2 = 33;
	array_push_back(s_vector, source1);
	array_push_back(s_vector, source1);
	array_push_back(s_vector, source1);
	array_push_back(s_vector, source1);
	array_push_back(s_vector, source1);
	array_push_back(s_vector, source1);
	array_push_back(s_vector, source2);
	array_push_back(s_vector, source2);
	array_push_back(s_vector, source2);
	array_push_back(s_vector, source1);
	array_push_back(s_vector, source1);
	array_push_back(s_vector, source2);
	array_stat2(s_vector, &int_noptr_print);
	*source1 = 2233;
	array_stat2(s_vector, &int_noptr_print);
	
	// array_stat(s_vector, NULL);
	array_destroy(s_vector);

	free(source1);
	free(source2);

	return 0;
}

void array_stat(auto_array *vec, print_func *print) {
	assert(vec);
	printf("array_size: %zu\n", array_size(vec));
	printf("array_capacity: %zu\n", array_capacity(vec));
	if (!print) return;
	printf("[ ");
	size_t size = array_size(vec);
	for (size_t i=0; i<size-1; i++) {
		(*print)(int_array_get(vec, i));
		printf("%s", ", ");
	}
	(*print)(int_array_get(vec,array_size(vec)-1));
	printf(" ]\n");
}


void array_stat2(auto_array *vec, vprint_func *print) {
	assert(vec);
	printf("array_size: %zu\n", array_size(vec));
	printf("array_capacity: %zu\n", array_capacity(vec));
	if (!print) return;

	int **arr = array_front(vec);
	printf("[ ");
	size_t size = array_size(vec);
	for (size_t i=0; i<size-1; i++) {
		printf("%i", *(arr[i]));
		printf("%s", ", ");
	}

	printf("%i", *(arr[array_size(vec)-1]));
	printf(" ]\n");
}

void int_print(void* i) {
	printf("%i ", *(int*)i);
}

void int_noptr_print(int i) {
	printf("%i ", i);
}

void char_print(void* c) {
	printf("%c ", *(char*)c);
}

void print_elem(print_func *print, void *elem) {
	(*print)(elem);
	printf("\n");
}
