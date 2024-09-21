#pragma once
#include <vector>
using namespace std;

class Graph {
public:
  Graph(int nodes);

  int number_of_nodes();
  void add_edge(int node_a, int node_b);

  int count_connected_components();

private:
  // Graph represented with an adjacency list.
  // connections[i] are the connected nodes to the node i
  vector<vector<int>> connections;
};

Graph new_random_geometric_grap(int nodes, float r);
Graph new_triangular_graph(int width);
Graph new_3d_grid_graph(int width);
