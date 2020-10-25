CFLAGS=-g -O0 -Wall -Wextra

all: driver_skinny

driver_skinny: driver_skinny.c skinny.o
	$(CC) $(CFLAGS) -o $@ $^

skinny.o: skinny.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f driver_skinny skinny.o
