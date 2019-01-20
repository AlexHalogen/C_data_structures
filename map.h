#pragma once
#include "utils/callbacks.h"
#include "auto_array.h"

typedef struct map map;

map *map_create(hash_function_type hash_function, compare_type comp,

                              copy_constructor_type key_copy_constructor,
                              destructor_type key_destructor,

                              copy_constructor_type value_copy_constructor,
                              destructor_type value_destructor);
void map_destroy(map *m);
int map_empty(map *m);
size_t map_size(map *m);
int map_contains(map *m, void *key);
void *map_get(map *m, void *key);

void map_set(map *m, void *key, void *value);
void map_remove(map *m, void *key);

// not implemented yet
void map_clear(map *m);
auto_array *map_keys(map *m);
auto_array *map_values(map *m);