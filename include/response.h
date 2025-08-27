#ifndef TRIPLE_H

typedef struct {
  const char *query_word;
  const char *title_author_verse;
  unsigned char count_qword;
} Response;

const char *create_query(const char *word);

#endif /* ifndef TRIPLE_H */
