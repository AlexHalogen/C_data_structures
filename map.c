#include <stdlib.h>
#include <pthread.h>
#include "auto_array.h"
#include "map.h"
#include "bitmap.h"
#include "utils/list.h"

#define LOCK_MAP(map) pthread_mutex_lock(&(map->mtx))
#define UNLOCK_MAP(map) pthread_mutex_unlock(&(map->mtx))

const float LOAD_FACTOR  = 0.7;

typedef struct {
	void *first;
	void *second;
} pair;

typedef struct sidechain_node {
	list_t *list;

	void *padding; // to pad the struct to the same size of a pair struct, not necessary
} chain_node;


struct map {
	pthread_mutex_t mtx;
	auto_array *table;
	size_t size;
	size_t capacity;
	hash_function_type hash_function;
	compare_type comp;
	copy_constructor_type key_copy_constructor;
	destructor_type key_destructor;
	copy_constructor_type value_copy_constructor;
	destructor_type value_destructor;
	bitmap_t *is_chained;
	bitmap_t *is_occupied;
};




void* pair_copy_constructor( void *p ) {
	pair *ret = malloc(sizeof(pair));
	ret->first = ((pair*)p)->first;
	ret->second = ((pair*)p)->second;
	return ret;
}
void pair_copy_constructor2(void* dest, void* p) {
	((pair*)dest)->first = ((pair*)p)->first;
	((pair*)dest)->second = ((pair*)p)->second;
}

pair *array_find(auto_array *h, void *key, compare_type comp) {
	size_t len = array_size(h);
	for (size_t i=0; i<len; i++) {
		pair *p = array_at(h, i);
		if (comp(key, p->first) == 0 ) {
			return p;
		}
	}
	return NULL;
}

// Cannot use list's find operations because there's no way to let list use
// key comparator used in map. // TODO: Find a more elegant solution!
list_node *list_find(list_t *list, void *key, compare_type comp) {
	list_node *current = list_head(list);
	while (current) {
		pair *p = (pair*)current->data;
		if (comp(p->first, key) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;

}

static size_t find_prime(size_t num);

int shouldResize(map* m) {
	double factor = (((float)(m->size)) / (m->capacity));
	return factor > LOAD_FACTOR;
}

void internal_map_set(map *m, char *key, void *value);

void resize_table(map* m);


map *map_create(hash_function_type hash_function, compare_type comp,

                              copy_constructor_type key_copy_constructor,
                              destructor_type key_destructor,

                              copy_constructor_type value_copy_constructor,
                              destructor_type value_destructor)
{
	map *m = malloc(sizeof(map));
	m->table = array_create(pair_copy_constructor2, nop_destructor, nop_default_constructor, sizeof(pair));
	m->size = 0;
	m->capacity = find_prime(0);
//	array_reserve(m->table, m->capacity);
    array_resize(m->table, m->capacity);
	m->hash_function = hash_function;
	m->comp = comp;
	m->key_copy_constructor = key_copy_constructor;
	m->key_destructor = key_destructor;

	m->value_copy_constructor = value_copy_constructor;
	m->value_destructor = value_destructor;
	m->is_chained = bitmap_create(m->capacity);
	m->is_occupied = bitmap_create(m->capacity);
	pthread_mutex_init(&m->mtx, NULL);
	return m;
}

void map_destroy(map *m) {
	pthread_mutex_destroy(&m->mtx);
	size_t size = m->capacity;
	bitmap_t *chained = m->is_chained;
	bitmap_t *occupied = m->is_occupied;

	for (size_t i=0; i<size; i++) {
		if (bitmap_get(occupied, i) == 1) {
			if (bitmap_get(chained, i) == 1) {
				chain_node *node = array_at(m->table, i);

				list_t *list = node->list;

				list_node *head = list_head(list);
				while (head) {
					pair *p = (pair*)head->data;
					m->key_destructor(p->first);
					m->value_destructor(p->second);
					head = head->next;
				}

				list_destroy(list);
			} else {

				// TODO: What to do if destructors really do something?
				// Current thinking: if occupied, then pair has meaningful stuff, so call des on pair elems
				// Don't call destructors otherwise
				pair *p = array_at(m->table, i);
				m->key_destructor(p->first);
				m->value_destructor(p->second);
			}

		}
	}
	array_destroy(m->table);
	bitmap_destroy(m->is_chained);
	bitmap_destroy(m->is_occupied);
	free(m);
}
int map_empty(map *m) {
	return (m->size == 0);
}
size_t map_size(map *m) {
	return (m->size);
}
int map_contains(map *m, void *key) {
	LOCK_MAP(m);
	int exists = 0;
	size_t hash = m->hash_function(key) % m->capacity;
	if (bitmap_get(m->is_occupied, hash) == 1) {
		if (bitmap_get(m->is_chained, hash) == 1) {
			list_t *list = ((chain_node*)array_at(m->table, hash))->list;

//			list_node *node = list_find_node(list, key);
			list_node *node = list_find(list, key, m->comp);
//			pair *node = array_find(list, key, m->comp);
			if (node) {
				exists = 1;
			}
		}
		else {
			pair *elem = array_at(m->table, hash);
			if (m->comp(elem->first, key) == 0) {
				exists = 1;	
			}
		}
	}

	return exists;


}
void* map_get(map *m, void *key) {
	LOCK_MAP(m);
	void * ret = NULL;
	size_t hash = m->hash_function(key) % m->capacity;

	if (bitmap_get(m->is_occupied, hash) == 1) {
		if (bitmap_get(m->is_chained, hash) == 1) {
			 list_t *list = ((chain_node*)array_at(m->table, hash))->list;
//			 list_node *node = list_find_node(list, key);
			list_node *node = list_find(list, key, m->comp);
			if (node) {
				 ret = ((pair*)node->data)->second;

			} else {
				ret = NULL;
			}
		}
		else {
			ret = ((pair*)array_at(m->table, hash))->second;
		}
	}

	UNLOCK_MAP(m);
	return ret;
}
// pair* map_get(map *m, void *key);
void map_set(map *m, void *key, void *value) {
	LOCK_MAP(m);
	internal_map_set(m, key, value);
	UNLOCK_MAP(m);
}
void internal_map_set(map *m, char *key, void *value) {
	

	if (shouldResize(m)) {
		resize_table(m);
	}
	size_t hash = m->hash_function(key) % m->capacity;
	if ( bitmap_get(m->is_occupied, hash) == 1 ) { // possible collision
		
		// dangerous operation, considering replacing it
		// pair *pair = (pair*)array_at(m->table, hash);
		

		if (bitmap_get(m->is_chained, hash) == 0) {
			// not chained
			pair *elem = (pair*)array_at(m->table, hash);
			if (m->comp(elem->first, key) == 0) { // not a collision, overwrite
				m->value_destructor(elem->second);
				elem->second = m->value_copy_constructor(value);
			} else { // create a linked list and insert original and current pair
				pair orig; orig.first = elem->first; orig.second = elem->second;
				
				chain_node *chain = (chain_node*)array_at(m->table, hash);
				list_t *list = chain->list = list_create(pair_copy_constructor, free, m->comp);


				list_insert_tail(list, &orig);

				bitmap_mark(m->is_chained, hash);

				pair insert;
				insert.first = m->key_copy_constructor(key);
				insert.second = m->value_copy_constructor(value);
				list_insert_tail(list, &insert);
				++m->size;
			}

		}
		else { // chained
			list_t *list = ((chain_node*)array_at(m->table, hash))->list;

//			list_node *node = list_find_node(list, key);
			list_node *node = list_find(list, key, m->comp);
			if (node) {// key exists, not a collision
				pair *elem = node->data;
				m->value_destructor(elem->second);
				elem->second = m->value_copy_constructor(value);
			}
			else {
				pair ins;
				ins.first = m->key_copy_constructor(key);
				ins.second = m->value_copy_constructor(value);
				list_insert_tail(list, &ins);

				++m->size;
			}
			
		}
		
	}
	else {
		bitmap_mark(m->is_occupied, hash);
		pair p;
		p.first = m->key_copy_constructor(key);
		p.second = m->value_copy_constructor(value);
		array_set(m->table, hash, &p);
		++m->size;
	}

}
void map_remove(map *m, void *key) {
	LOCK_MAP(m);
	size_t hash = m->hash_function(key) % m->capacity;
	if (bitmap_get(m->is_occupied, hash) == 1) {

		if (bitmap_get(m->is_chained, hash) == 1) {
			// Search the list for key
			list_t *list = ((chain_node*)array_at(m->table, hash))->list;
//			list_node *node = list_find_node(list, key);
			list_node *node = list_find(list, key, m->comp);
			if (node) {
				list_unlink_node(list, node);
				--m->size;
			}
			// Does not unset branched bit even if the list becomes 
			// empty after removing

		} else {
			// unbranched node
			pair *p = (pair*)array_at(m->table, hash);
			m->key_destructor(p->first);
			m->value_destructor(p->second);
			bitmap_unmark(m->is_occupied, hash);
		}

	}
	UNLOCK_MAP(m);
}
	
void map_clear(map *m);
auto_array *map_keys(map *m);
auto_array *map_values(map *m);


void resize_table(map *m) {
	size_t newsize = find_prime(m->capacity*2);
	size_t capacity = m->capacity;

	auto_array *kvpairs = array_create(pair_copy_constructor2, nop_destructor, nop_default_constructor, sizeof(pair));
	array_reserve(kvpairs, m->size);
	bitmap_t *occupied = m->is_occupied;
	bitmap_t *chained = m->is_chained;
	auto_array *table = m->table;
	
    copy_constructor_type kc = m->key_copy_constructor;
    copy_constructor_type vc = m->value_copy_constructor;
    destructor_type  kd = m->key_destructor;
    destructor_type  vd = m->value_destructor;

    m->key_copy_constructor = m->value_copy_constructor = pointer_copy_constructor;
    m->key_destructor = m->value_destructor = nop_destructor;

	// push all kv pairs to a temp array and destroy all lists
	for (size_t i=0; i<capacity; ++i) {
		if (bitmap_get(occupied, i) == 1) {
			if (bitmap_get(chained, i) == 1) {
				// a sidechain
				list_t *l = ((chain_node*)array_at(table, i))->list;

				list_node *head = list_head(l);

				while (head) {
					array_push_back(kvpairs, head->data);
					head = head->next;
				}
				// destroy the list 
				list_destroy(l);

			} else {
				pair *p = (pair*)array_at(table, i);
				array_push_back(kvpairs, p);
			}

		}
	}

	bitmap_clear(occupied);
	bitmap_clear(chained);

	bitmap_resize(occupied, newsize);
	bitmap_resize(chained, newsize);

	size_t size = m->size;
	m->size = 0;
	m->capacity = newsize;
	array_resize(m->table, newsize);
	for (size_t i=0; i<size; ++i) {
		pair *p = (pair*)array_at(kvpairs, i); 
		internal_map_set(m, p->first, p->second);
	}

	m->key_copy_constructor = kc;
	m->value_copy_constructor = vc;
	m->key_destructor = kd;
	m->value_destructor = vd;
	array_destroy(kvpairs);

}



static size_t find_prime(size_t num) {
	const size_t primes[]
	    = {17ul,         29ul,         37ul,        53ul,        67ul,
	       79ul,         97ul,         131ul,       193ul,       257ul,
	       389ul,        521ul,        769ul,       1031ul,      1543ul,
	       2053ul,       3079ul,       6151ul,      12289ul,     24593ul,
	       49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
	       1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
	       50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
	       1610612741ul, 3221225473ul, 4294967291ul};
    size_t len = sizeof(primes) / sizeof(size_t);
    
    for (unsigned i=0; i<len; ++i) {
    	if (primes[i] >= num) {
    		return primes[i];
    	}
    }
    return 0;
}