/* Challenge 8 Shortest Path
 * Extend the idea of an adjacency matrix of a graph G to a distance
 * matrix D that holds the distance when going from point i to point j.
 * Mark the absence of a direct arc with a very large value, such as SIZE_MAX.
 *
 * Can you find the shortest path between two nodes x and y given as an input?
 */

#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

void print_path(const unsigned a, const unsigned b, const unsigned n,
    const unsigned prev[static n], const unsigned dist[static n]) {
  unsigned cur = b;

  while (cur <= n && cur != a) {
    printf("%u <- ", cur);
    cur = prev[cur];
  }
  printf("%u\n", cur);
  printf("Total distance: %u\n", dist[b]);
}

int shortest_path(const unsigned a, const unsigned b,
    const unsigned n, const unsigned D[static n][n]) {
  unsigned nodes = 0;
  bool visited[n];
  unsigned dist[n];
  unsigned prev[n];

  for (unsigned i = 0; i < n; i++) {
    visited[i] = false;
    prev[i] = UINT_MAX;
    dist[i] = UINT_MAX;
  }

  dist[a] = 0;

  while (nodes < n) {
    unsigned cur = UINT_MAX;
    for (unsigned i = 0; i < n; i++) {
      if (!visited[i]) {
        if (cur >= n) cur = i;
        else cur = dist[i] < dist[cur] ? i : cur;
      }
    }
    if (cur >= n || dist[cur] == UINT_MAX) {
      printf("No path detected\n");
      return -1;
    } else if (cur == b) {
      print_path(a, b, n, prev, dist);
      return 0;
    }
    visited[cur] = true;
    nodes++;
    for (unsigned i = 0; i < n; i++) {
      if (D[cur][i]) {
        unsigned alt = D[cur][i] + dist[cur];
        if (alt < dist[i]) {
          dist[i] = alt;
          prev[i] = cur;
        }
      }
    }
  } 
  printf("No path detected\n");
  return -1;
}

int main() {
  unsigned graph[9][9] = {
    { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
    { 4, 0, 8, 0, 0, 0, 0, 11, 0 },
    { 0, 8, 0, 7, 0, 4, 0, 0, 2 },
    { 0, 0, 7, 0, 9, 14, 0, 0, 0 },
    { 0, 0, 0, 9, 0, 10, 0, 0, 0 },
    { 0, 0, 4, 14, 10, 0, 2, 0, 0 },
    { 0, 0, 0, 0, 0, 2, 0, 1, 6 },
    { 8, 11, 0, 0, 0, 0, 1, 0, 7 },
    { 0, 0, 2, 0, 0, 0, 6, 7, 0 },
  };

  unsigned disc_graph[7][7] = {
    { 0, 1, 1, 0, 0, 0, 0 },
    { 1, 0, 1, 0, 0, 0, 0 },
    { 1, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1 },
    { 0, 0, 0, 0, 0, 0, 1 },
    { 0, 0, 0, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0 },
  };

  shortest_path(4, 0, 9, graph);
  shortest_path(0, 5, 7, disc_graph);

  return 0;
}
