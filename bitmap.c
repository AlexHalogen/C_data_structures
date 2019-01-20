#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "bitmap.h"

#define bitmap_shift 3
#define bitmap_mask 0x07

struct struct_bitmap {
	char *array;
	size_t capacity_bytes;
	size_t last_bit_pos;
};

bitmap_t* bitmap_create(size_t size) {
	bitmap_t *ret = malloc(sizeof(bitmap_t));
	size_t size_bytes = ceil((double)size / 8);
	ret->array = calloc(size_bytes, 1);
	ret->last_bit_pos = 0;
	ret->capacity_bytes = size_bytes;
	return ret;
}

void bitmap_destroy(bitmap_t *bitmap) {
	free(bitmap->array);
	free(bitmap);
}

char bitmap_get(bitmap_t *bitmap, size_t index) {
    if (index > bitmap->last_bit_pos) {
        return 0;
    }

    size_t byte_offset = index >> bitmap_shift;


	unsigned bit_offset = index & bitmap_mask;
	char byte = (bitmap->array)[byte_offset];
	return ( (byte >> bit_offset) & 1U );
}
void bitmap_mark(bitmap_t *bitmap, size_t index) {
	size_t byte_offset = index >> bitmap_shift;
	if (byte_offset >= bitmap->capacity_bytes) {
        bitmap_resize(bitmap, index+1);
	}
	unsigned bit_offset = index & bitmap_mask;
	(bitmap->array)[byte_offset] |= (1U << bit_offset);  
	if (index > bitmap->last_bit_pos) {
		bitmap->last_bit_pos = index;
	}
}
void bitmap_unmark(bitmap_t *bitmap, size_t index) {
	size_t byte_offset = index >> bitmap_shift;
	if (byte_offset >= bitmap->capacity_bytes) {
        bitmap_resize(bitmap, index+1);
	}

	unsigned bit_offset = index & bitmap_mask;
	(bitmap->array)[byte_offset] &= ~(1U << bit_offset);
	if (index > bitmap->last_bit_pos) {
		bitmap->last_bit_pos = index;
	}
}


void bitmap_toggle(bitmap_t *bitmap, size_t index) {
	size_t byte_offset = index >> bitmap_shift;
	if (index >= bitmap->last_bit_pos) {
        bitmap->last_bit_pos = index;
        bitmap_resize(bitmap, index+1);
	}

	unsigned bit_offset = index & bitmap_mask;
	(bitmap->array)[byte_offset] ^= (1U << bit_offset);
}
char bitmap_resize(bitmap_t *bitmap, size_t newsize) {
	size_t old_capacity = bitmap->capacity_bytes;
	size_t target_capacity = ceil((double)newsize / 8);
	size_t new_capacity = 1;

	while (new_capacity < target_capacity) {
        new_capacity *= 2;
	}

	bitmap->array = realloc(bitmap->array, new_capacity);
	if (!bitmap->array) {
	    return 1; // realloc failed
	}
    bitmap->capacity_bytes = new_capacity;
	if (newsize > old_capacity) {
		memset(bitmap->array+old_capacity, 0, new_capacity-old_capacity);
	} 
	else {
		bitmap->last_bit_pos = newsize-1;
		unsigned char mask = 0xff;
		size_t byte_offset = newsize >> bitmap_shift;
		mask = mask >> (8*sizeof(char) - (newsize & bitmap_mask));
//		(bitmap->array)[target_capacity-1] &= mask;
		(bitmap->array)[byte_offset] &= mask;
	}
	return 0;
}

void bitmap_clear(bitmap_t *bitmap) {
	memset(bitmap->array, 0, bitmap->capacity_bytes);
}