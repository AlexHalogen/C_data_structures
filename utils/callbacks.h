#pragma once

typedef void *(*copy_constructor_type)(void *elem);
typedef void (*destructor_type)(void *elem);
typedef void *(*default_constructor_type)(void);


typedef void (*copy_constructor2_type)(void *dest, void *elem);
typedef void (*default_constructor2_type)(void *dest);

typedef size_t (*hash_function_type)(void *key);

typedef int (*compare_type)(const void *A, const void *B);


void nop_destructor(void *ptr);

void nop_default_constructor(void *dest);


void pointer_destructor(void *ptr);

void *pointer_copy_constructor(void *ptr );
void *pointer_default_constructor();
void *pointer_copy_constructor(void *ptr );
void pointer_copy_constructor2(void *dest, void *ptr );
void pointer_default_constructor2(void *dest);
size_t pointer_hash_function(void *ptr);

void char_destructor(void *ptr);
void *char_copy_constructor(void *ptr );
void *char_default_constructor();
void char_copy_constructor2(void *dest, void *ptr );
void char_default_constructor2(void *dest);
size_t char_hash_function(void *ptr);

void size4_destructor(void *ptr);
void *size4_copy_constructor(void *ptr );
void *size4_default_constructor();
void size4_copy_constructor2(void *dest, void *ptr );
void size4_default_constructor2(void *dest);
size_t size4_hash_function(void *ptr);

void size8_destructor(void *ptr);
void *size8_copy_constructor(void *ptr );
void *size8_default_constructor();
void size8_copy_constructor2(void *dest, void *ptr );
void size8_default_constructor2(void *dest);
size_t size8_hash_function(void *ptr);

void string_copy_constructor2(void *dest, void *elem);
void *string_copy_constructor(void *ptr );
void *string_default_constructor();
void string_destructor(void *elem);
void string_default_constructor2(void *dest);
size_t string_hash_function(void *elem) ;