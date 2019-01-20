#pragma once
#include "utils/callbacks.h"
#include "auto_array.h"

typedef struct map map;

map *map_create(hash_function_type hash_function, compare_type comp,

                              copy_constructor_type key_copy_constructor,
                              destructor_type key_destructor,

                              copy_constructor_type value_copy_constructor,
                              destructor_type value_destructor);
void map_destroy(map *this);
int map_empty(map *this);
size_t map_size(map *this);
int map_contains(map *this, void *key);
void *map_get(map *this, void *key);
// key_value_pair map_at(map *this, void *key);
void map_set(map *this, void *key, void *value);
void map_remove(map *this, void *key);
void map_clear(map *this);
auto_array *map_keys(map *this);
auto_array *map_values(map *this);