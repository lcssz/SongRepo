#include "./include/repository.h"
#include <stdio.h>
#include <string.h>

void insert_song(FILE *file) {
  char *song_name;
  char *author_name;
  // Lê a primeira linha e altera o '\n' para '\0'
  fgets(song_name, 42, file);
  song_name[strcspn(song_name, "\n")] = 0;
  // Lê a segunda linha e altera o '\n' para '\0'
  fgets(author_name, 42, file);
  author_name[strcspn(song_name, "\n")] = 0;
}
