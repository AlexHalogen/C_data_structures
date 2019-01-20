#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bitmap.h"



int main() {
    bitmap_t *b = bitmap_create(1);

    for (int i=0; i<9; i++) {
        bitmap_mark(b, i);
    }

    for (int i=0; i<9; i++) {
        char r = bitmap_get(b, i);
        (void)r;
    }

    bitmap_mark(b, 19);
    bitmap_resize(b,99);
    assert(bitmap_get(b, 19));
    bitmap_resize(b,1);
    for (int i=1; i<100; i++) {
        unsigned char k = bitmap_get(b,i);
        assert( k == 0);
    }

    for (int i=0; i<100; i+=2) {
        bitmap_mark(b, i);
    }

    for (int i=0; i<100; i+=2) {
        char k = bitmap_get(b, i);
        assert(k);
    }



    bitmap_destroy(b);
    return 0;
}