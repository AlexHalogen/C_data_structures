CC = clang
CFLAGS = -std=c99  -c -g -Wall -Wextra -fPIC

LD = clang
LDFLAGS = -std=c99 -g -Wall -pthread

all: map_test array_test queue.o


utils/callbacks.o: utils/callbacks.c
	$(CC) $(CFLAGS) $^ -o $@

utils/list.o: utils/list.c
	$(CC) $(CFLAGS) $^ -o $@


auto_array.o: auto_array.c
	$(CC) $(CFLAGS) $^ -o $@

queue.o: queue.c
	$(CC) $(CFLAGS) $^ -o $@

map.o: map.c
	#utils/callbacks.o utils/list.o auto_array.o bitmap.o
	$(CC) $(CFLAGS) $< -o $@

bitmap.o: bitmap.c
	$(CC) $(CFLAGS) $< -o $@


array_test: array_test.c auto_array.o utils/callbacks.o
	$(CC) $(CFLAGS) $< -o $@.o
	$(LD) $(LDFLAGS)  $^ -o $@

map_test: map_test.c auto_array.o utils/callbacks.o utils/list.o map.o bitmap.o
	$(CC) $(CFLAGS) $< -o $@.o
	$(LD) $(LDFLAGS) -lm $^ -o $@

clean:
	rm -f *.o *.a
	rm -f utils/*.o utils/*.a
