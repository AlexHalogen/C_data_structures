#include <stdlib.h>

/*
 * Simplistic array wrapping that allows automatic growing of the array
 * Array accepts any type of data, and data is guaranteed to be contigious in 
 * memory, 
 * e.g &(array_get(array, 0)) == &(array_get(array,1)) + sizeof(array_element)
 *
 * User of the array need to supply a destructor, a default constructor and 
 * copy constructor.
 * Destructor behaves like those in C++, while copy constructor and default 
 * constructor constructs the object at designated address(Assuming abundant 
 * space is allocated)
 * 
 * The array provides thread-safe push_back and pop_back operations, and 
 * pop_back () does NOT block when the array is empty.
 * 
 * All other operations are NOT thread-safe.
 */


typedef struct auto_array auto_array;


typedef void (*copy_constructor2_type)(void *dest, void *elem);

typedef void (*destructor_type)(void *elem);

typedef void (*default_constructor2_type)(void *dest);


// initial capacity for array
extern const int INITIAL_CAPACITY;
// factor used to expand the array when the array is full
extern const int GROWTH_FACTOR;


auto_array *array_create(copy_constructor2_type copy_constructor,
					destructor_type destructor,
					default_constructor2_type default_constructor,
					int elem_size);
void array_destroy(auto_array *array) ;
int array_push_back(auto_array* array, void* elem) ;
void array_pop_back(auto_array *array);
int array_reserve(auto_array *array, size_t new_size) ;
void array_insert(auto_array *array, size_t position, void *element) ;
void array_erase(auto_array *array, size_t position) ;
void *array_front(auto_array *array) ;
void *array_back(auto_array *array) ;
void array_set(auto_array *array, size_t position, void *element);
void *array_at(auto_array *array, size_t position) ;
void array_resize(auto_array *array, size_t n);
void array_clear(auto_array *array);

size_t array_size(auto_array *array);
size_t array_capacity(auto_array *array);
int array_empty(auto_array *array);