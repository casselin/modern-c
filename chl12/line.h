#ifndef LINE_H
# define LINE_H 1
# include <stddef.h>

# define LINE_MAX 256

typedef struct line line;

struct line {
  char text[LINE_MAX];
  line* prevp;
  line* nextp;
};

/* Functions that return a value of type line */
line line_get(char const str[static 1], line* prevp, line* nextp);

/* Functions that operate on pointers to line */
void line_destroy(line* lp);
line* line_init(line* lp, char const [static 1], line* prevp, line* nextp);
line* line_split_at(line* lp, line* nlp, size_t n);
line* line_join_next(line* lp);
line* line_normalize(line* head);

/* Functions that handle I/O */
int line_nprint(line* head, size_t rows, size_t cols);

#endif
