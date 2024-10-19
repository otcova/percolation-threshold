#include "graph.h"
#include <math.h>

int get_height(int n) {
  return (1 + sqrt(1 + 8 * n)) / 2;
}

Graph new_triangular_graph(int alt) {
  int n = alt * (alt + 1) / 2;
  Graph graph(n);
  // graf tiangular
  // conectar amb dos fills
  for (int i = 0; i < n - alt; ++i) {
    int h = get_height(i);
    graph.add_edge(i, i + h);
    graph.add_edge(i, i + h + 1);
  }
  // conectar gemans
  for (int i = 1; i < n - 1; ++i) {
    i += (get_height(i) != get_height(i + 1));
    graph.add_edge(i, i + 1);
  }
  return graph;
}

Graph new_grid_graph(int width) {
  Graph graph(width * width);

  // conectar horitzaontal
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < width - 1; ++j)
      graph.add_edge(width * i + j, width * i + (j + 1));
  }

  // conectar vertical
  for (int j = 0; j < width; ++j) {
    for (int i = 0; i < width - 1; ++i)
      graph.add_edge(width * i + j, width * (i + 1) + j);
  }

  return graph;
}
