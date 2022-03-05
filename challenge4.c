/* 
    Challenge 4 Union-Find
  
  The Union-Find problem deals with the representation of partitions over a base set.
  We will identify the elements of the base set using the numbers 0, 1, ... and will
  represent partitions with a forest data structure where each element has a "parent"
  that is another element inside the same partition. Each set in such a partition is
  identified by a designated element called the root of the set.

  We want to perform two principal operations:
    - A Find operation receives one element of the ground set and returns the root of
	  the corresponding set.
	- A Union operation receives two elements and merges the two sets to which these
	  elements belong into one.

  Can you implement a forest data structure in an index table of base type size_t
  called parent? Here, a value in the table SIZE_MAX would mean a position represents
  a root of one of the trees; another number represents position of the parent of
  the corresponding tree. One of the important features to start the implementation
  is an initialization function that makes parent the singleton partition: that is,
  the partition where each element is the root of its own prviate set.

  With this index table, can you implement a Find function that, for a given index,
  finds the root of its tree?

  Can you implement a FindReplace function that changes all parent entries on a path
  to the root (including) to a specific value?

  Can you implement a FindCompress function that changes all parent entries to the root
  that has been found?

  Can you implement a Union function that, for two given elements, combines their trees
  into one? Use FindCompress for one side and FindReplace for the other.

*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define TABLE_MAX 5

size_t Find(size_t table[static 1], size_t i) {
  if (table[i] == SIZE_MAX) {
    return i;
  }

  return Find(table, table[i]);
}

void FindReplace(size_t table[static 1], size_t i, size_t x) {
  if (table[i] == SIZE_MAX) {
    table[i] = x;
	return;
  }

  FindReplace(table, table[i], x);

  table[i] = x;

  if (table[i] == i) {
    table[i] = SIZE_MAX;
  }
}

void FindCompress(size_t table[static 1], size_t i) {
  size_t root = Find(table, i);

  while (i != root) {
    size_t temp = table[i];
	table[i] = root;
	i = temp;
  }
}

void Union(size_t table[static 1], size_t a, size_t b) {
  FindCompress(table, a);

  if (Find(table, a) == Find(table, b)) {
    return;
  }
  FindReplace(table, b, a);
}

void initialize(size_t table[static 1], size_t size) {
  for (size_t i = 0; i < size; ++i) {
    table[i] = SIZE_MAX;
  }
}

int main(int argc, char* argv[argc+1]) {
  size_t parent[TABLE_MAX];
  
  initialize(parent, TABLE_MAX);

  Union(parent, 0, 1);
  Union(parent, 2, 3);
  Union(parent, 3, 4);
  Union(parent, 0, 4);

  for (int i = 0; i < TABLE_MAX; ++i) {
    printf("i=%d\tparent[i]=%zu\n", i, parent[i]);
  }

  return 0;
}
