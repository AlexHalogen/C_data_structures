void pointer_destructor(void *ptr) ;
void pointer_copy_constructor2(void *dest, void *ptr ) ;
void pointer_default_constructor2(void *dest) ;
size_t pointer_hash_function(void *ptr) ;

void char_destructor(void *ptr) ;
void char_copy_constructor2(void *dest, void *ptr ) ;
void char_default_constructor2(void *dest) ;
size_t char_hash_function(void *ptr) ;

void size4_destructor(void *ptr) ;
void size4_copy_constructor2(void *dest, void *ptr ) ;
void size4_default_constructor2(void *dest) ;
size_t size4_hash_function(void *ptr) ;

void size8_destructor(void *ptr) ;
void size8_copy_constructor2(void *dest, void *ptr ) ;
void size8_default_constructor2(void *dest) ;
size_t size8_hash_function(void *ptr) ;

void string_copy_constructor2(void *dest, void *elem) ;
void string_destructor2(void *elem) ;
void string_default_constructor2(void *dest) ;
size_t string_hash_function(void *elem) ;