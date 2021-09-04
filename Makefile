CC=gcc
CFLAGS= -g -ggdb -I.
DEPS = part_types.h
OBJ = parse_partition_table.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make:	$(OBJ)
	$(CC) -o parse_partition_table $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o parse_partition_table
