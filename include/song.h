#ifndef SONG_H

typedef struct {
  const char *tittle;
  const char *author;
  const char *lyrics;
} Song;

Song create_new_song(const char *tittle, const char *autor, const char *lyrics);

#endif /* ifndef SONG_H */
