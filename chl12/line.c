/* Challenge 12 Text processor
 * For a text processor, can you use a doubly linked list to store text?
 * The idea is to represent a ``blob'' of text through a struct that
 * contains a string (for the text) and pointers to preceding and
 * following blobs.
 *
 * Can you build a function that splits a text blob in two at a given
 * point?
 *
 * One that joins two consecutive text blobs?
 *
 * One that runs through the entire text and puts it in the form of 
 * one blob per line?
 *
 * Can you create a function that prints the entire text or prints until
 * the text is cut off due to the screen size?
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "line.h"

line line_get(char const str[static 1], line* prevp, line* nextp) {
  line ret = { 
    .text = { 0 },
    .prevp = prevp,
    .nextp = nextp,
  };
  strncpy(ret.text, str, LINE_MAX-1);
  ret.text[LINE_MAX-1] = '\0';
  return ret;
}

line* line_init(line* lp, char const str[static 1], line* prevp, line* nextp) {
  if (lp)
    *lp = line_get(str, prevp, nextp);
  return lp;
}

void line_destroy(line* lp) {
  if (lp) *lp = (line){ 0 };
}


line* line_split_at(line* lp, line* nlp, size_t n) {
  assert(n < LINE_MAX);
  if (lp && nlp) {
    line_init(nlp, lp->text+n, lp, lp->nextp);
    lp->text[n] = '\0';
    lp->nextp = nlp;
  }
  return lp;
}

line* line_join_next(line* lp) {
  if (lp && lp->nextp) {
    size_t n = strlen(lp->text);
    assert(n + strlen(lp->nextp->text) <= LINE_MAX); 
    strncat(lp->text, lp->nextp->text, LINE_MAX - n);
    line* tmp = lp->nextp;
    lp->nextp = lp->nextp->nextp;
    line_destroy(tmp);
    if (lp->nextp) lp->nextp->prevp = lp;
  }
  return lp;
}

line* line_append(line* first, line* second) {
  if (first && second && !first->nextp && !second->prevp && !second->nextp) {
    first->nextp = second;
    second->prevp = first;
  }
  return second;
}

int str_newline(char str[static 1]) {
  for (unsigned i = 0; str[i]; ++i) {
    if (str[i] == '\n') return i;
  }
  return -1;
}

line* line_normalize(line* head) {
  if (!head) return head;
  while (head) {
    while (head->nextp && str_newline(head->text) < 0) {
      line_join_next(head);
    }
    head = head->nextp;
  }
  return head;
}

int line_nprint(line* head, size_t rows, size_t cols) {
  char output[LINE_MAX] = { 0 };
  while (head && rows > 0) {
    strncpy(output, head->text, cols);
    fputs(output, stdout);
    --rows;
    head = head->nextp;
  }
  return EXIT_SUCCESS;
}

enum { buf_max = 64, };

int main(int argc, char* argv[argc+1]) {
  // Test line_get
  line first = line_get("Hello world!", 0, 0);
  assert(!strcmp(first.text, "Hello world!"));
  assert(!first.prevp);
  assert(!first.nextp);

  // Test line_init
  line* testp = &(line){ .text = { 0 }, .prevp = 0, .nextp = 0 };
  line_init(testp, "line_init test", &first, 0);
  assert(!strcmp(testp->text, "line_init test"));
  assert(testp->prevp == &first);
  assert(!testp->nextp);
  assert(!strcmp(first.text, testp->prevp->text));

  // Test line_split_at
  line* testp2 = &(line){ 0 };
  line* testp3 = &(line){ 0 };
  line_init(testp, "Test Split Line", 0, 0);

  line_split_at(testp, testp2, 4);
  assert(testp);
  assert(testp2);
  assert(!strcmp(testp->text, "Test"));
  assert(testp->nextp);
  assert(!strcmp(testp->nextp->text, " Split Line"));
  assert(testp->nextp->prevp == testp);
  assert(testp->nextp == testp2);
  assert(testp->nextp->nextp == testp2->nextp);
  assert(testp2->prevp == testp);

  line_split_at(testp2, testp3, 6);
  assert(!strcmp(testp->nextp->text, " Split"));
  assert(testp->nextp->nextp);
  assert(!strcmp(testp->nextp->nextp->text, " Line"));
  assert(!strcmp(testp3->text, " Line"));
  assert(testp->nextp->nextp->prevp == testp->nextp);

  // Test line_join_next
  line_join_next(testp);
  assert(!strcmp(testp->text, "Test Split"));
  assert(testp->nextp == testp3);

  line_join_next(testp);
  assert(!strcmp(testp->text, "Test Split Line"));
  assert(!testp->nextp);

  // Test line_normalize
  line* p1 = &(line){ .text = "First ", .nextp = 0, .prevp = 0 };
  line* p2 = &(line){ .text = "Line\n", .nextp = 0, .prevp = 0 };
  line* p3 = &(line){ .text = "Second Line\n", .nextp = 0, .prevp = 0 };
  line_append(p1, p2);
  line_append(p2, p3);

  line_normalize(p3);
  assert(!strcmp(p3->text, "Second Line\n"));
  assert(!p3->nextp);
  assert(p3->prevp == p2);

  line_normalize(p1);
  assert(!strcmp(p1->text, "First Line\n"));
  assert(p1->nextp == p3);
  assert(p3->prevp == p1);

  puts("Everything works!");
  return EXIT_SUCCESS;
}
