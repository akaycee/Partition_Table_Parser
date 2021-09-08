CC=gcc
CFLAGS= -g -ggdb -I.
OBJ = guid_hash_gen.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make:	$(OBJ)
	$(CC) -o guid_hash_gen $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o guid_hash_gen
