/* Challenge 7 Adjacency matrix
 * The adjacency matrix of a graph G is a matrix A that holds a value true or
 * false in element A[i][j] if there is an arc from node i to node j.
 * 
 * At this point, can you use an adjacency matrix to conduct a breadth-first
 * search in a graph G? Can you find the connected components? Can you find
 * a spanning tree?
 */

#include <stdio.h>
#include <stdbool.h>

bool test1[6][6] = {
  { false, false, true, false, true, true },
  { false, false, false, true, true, false },
  { true, false, false, false, true, true },
  { false, true, false, false, false, true },
  { true, true, true, false, false, false },
  { true, false, true, true, false, false },
};

bool test2[7][7] = {
  { false, true, true, false, false, false, false },
  { true, false, true, false, false, false, false },
  { true, true, false, false, false, false, false },
  { false, false, false, false, false, true, true },
  { false, false, false, false, false, false, true },
  { false, false, false, true, false, false, false },
  { false, false, false, true, true, false, false },
};

bool all_visited(unsigned n, bool v[static n]) {
  for (unsigned i = 0; i < n; i++) {
    if (!v[i]) return false;
  }
  return true;
}

void bfs(unsigned node, unsigned n, bool A[static n][n]) {
  bool visited[n];
  unsigned queue[n];

  for (unsigned i = 0; i < n; i++) {
    visited[i] = false;
  }

  queue[0] = node;
  unsigned first = 0;
  unsigned last = 0;
  visited[node] = true;

  while (first <= last && first < n) {
    unsigned v = queue[first];

    printf("Visited node %c\n", v + 'a');

    for (unsigned i = 0; i < n; i++) {
      if (A[v][i] && !visited[i]) {
        last++;
        queue[last] = i;
        visited[i] = true;
      }
    }
    first++;
  }
}

void test_bfs() {
  printf("Breadth-first search of graph test1 starting at node a (0)\n");
  bfs(0, 6, test1);

  printf("Breadth-first search of graph test1 starting at node e (4)\n");
  bfs(4, 6, test1); 

  printf("Breadth-first search of graph test2 starting at node a (0)\n");
  bfs(0, 7, test2);

  printf("Breadth-first search of graph test2 starting at node g (6)\n");
  bfs(6, 7, test2);
}

void dfs(unsigned node, unsigned n,
    bool visited[static n], bool A[static n][n]) {
  if (visited[node]) return;
  visited[node] = true;

  for (unsigned i = 0; i < n; i++) {
    if (A[node][i] && !visited[i]) {
      dfs(i, n, visited, A);
    }
  }
  printf("Node %c\n", node + 'a');
}

void connected(unsigned n, bool A[static n][n]) {
  bool visited[n];

  for (unsigned i = 0; i < n; i++) {
    visited[i] = false;
  }

  for (unsigned i = 0; i < n; i++) {
    if (!visited[i]) {
      printf("The following nodes are connected:\n");
      dfs(i, n, visited, A);
    }
  }
}

void test_connected() {
  printf("Connected components of graph test1\n");
  connected(6, test1);

  printf("Connected components of graph test2\n");
  connected(7, test2);
}

void dfs_span(unsigned node, unsigned n, bool visited[n], 
    bool A[static n][n], bool T[static n][n]) {
  if (visited[node]) return;
  
  visited[node] = true;

  for (unsigned i = 0; i < n; i++) {
    if (A[node][i] && !visited[i]) {
      dfs_span(i, n, visited, A, T);
      T[node][i] = true;
      T[i][node] = true;
    }
  }

}

void spanning_tree(unsigned n, bool A[static n][n], bool T[static n][n]) {
  bool visited[n];

  for (unsigned i = 0; i < n; i++) {
    visited[i] = false;
  }

  dfs_span(0, 6, visited, A, T);
}

void test_spanning_tree() {
  bool T[6][6];

  for (unsigned i = 0; i < 6; i++) {
    for (unsigned j = 0; j < 6; j++) {
      T[i][j] = false;
    }
  }

  spanning_tree(6, test1, T);
  
  printf("Adjacency matrix for spanning tree of test1:\n");
  for (unsigned i = 0; i < 6; i++) {
    for (unsigned j = 0; j < 6; j++) {
      printf(j == 5 ? " %u\n" : " %u ", T[i][j]);
    }
  }

  printf("Spanning tree of test1 should be connected\n");
  connected(6, T);
}

int main(void) {
  test_bfs();
  test_connected();
  test_spanning_tree();

  return 0;
}
