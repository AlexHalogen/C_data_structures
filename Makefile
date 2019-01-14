CC = gcc
CFLAGS = -std=c99  -c -g -Wall -Wextra 

LD = gcc
LDFLAGS = -std=c99 -g -Wall -pthread

callbacks.o: callbacks.c
	$(CC) $(CFLAGS) $^ -o $@


auto_array.o: auto_array.c
	$(CC) $(CFLAGS) $^ -o $@

array_test: array_test.c auto_array.o callbacks.o
	$(CC) $(CFLAGS) $< -o $@.o
	$(LD) $(LDFLAGS)  $^ -o $@

queue.o: queue.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o *.a