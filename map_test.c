#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "map.h"


int int_compare(const void *a, const void *b) {

	return (*(int*)a != *(int*)b );
}

int strcmp_wrapper(const void *a, const void* b) {
	return strcmp((const char*)a, (const char*)b);
}


void wc_test() {
	map *m = map_create(string_hash_function, strcmp_wrapper, string_copy_constructor, string_destructor, size4_copy_constructor, size4_destructor);

	FILE *file = fopen("alice.txt", "r");
	char *word = malloc(25565);
	size_t wc = 0;
	size_t are_count = 0;
	while (fscanf(file, "%s", word) == 1 ) {

		wc++;
		int newval = 1;
		void *res;
		if (strcmp(word, "are") == 0) {
			// for determining to go into
			int j=0; j++;
		}
		res = map_get(m, word);
		if (res) {
			newval = (*(int*)res) + 1;
		}
		if (strcmp(word, "are") == 0) {
			are_count++;
		}


		map_set(m, word, &newval);
	}
	printf("total word count %ld\n", wc);
	int *result = map_get(m, "are");
	printf("are: %d/%zu\n", *result, are_count);

	map_destroy(m);
	free(word);
	fclose(file);
}

int main() {
	map *m = map_create(size4_hash_function, int_compare, size4_copy_constructor, size4_destructor, size4_copy_constructor, size4_destructor);
	int key = 1;
	int v = 9;
	map_set(m, &key, &v);
	int *result = map_get(m, &key);
	printf("%d\n", *result);
	v = 255;
	map_set(m, &key, &v);
	result = map_get(m, &key);
	printf("%d\n", *result);

	wc_test();
	map_destroy(m);

	return 0;
}