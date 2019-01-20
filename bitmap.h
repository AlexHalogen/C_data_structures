#pragma once
typedef struct struct_bitmap bitmap_t;


bitmap_t* bitmap_create(size_t size);

void bitmap_destroy(bitmap_t *bitmap);

char bitmap_get(bitmap_t *bitmap, size_t index);
void bitmap_mark(bitmap_t *bitmap, size_t index);
void bitmap_unmark(bitmap_t *bitmap, size_t index);

void bitmap_toggle(bitmap_t *bitmap, size_t index);

char bitmap_resize(bitmap_t *bitmap, size_t newsize);

void bitmap_clear(bitmap_t *bitmap);