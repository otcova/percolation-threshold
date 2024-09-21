#include "graph.h"

Graph new_random_geometric_grap(int nodes, float r) {
  Graph graph(nodes);
  // TODO
  return graph;
}

Graph new_triangular_graph(int width) {
  Graph graph(width * (width - 1) / 2);
  // TODO
  return graph;
}

Graph new_3d_grid_graph(int width) {
  Graph graph(width * width * width);
  // TODO
  return graph;
}
