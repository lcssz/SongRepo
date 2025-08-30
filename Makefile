CC=gcc
CFLAGS=-Iinclude -Wall
DEPS = include/repository.h include/structures.h
OBJ = main.o repository.o structures.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

song_repo: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) song_repo
