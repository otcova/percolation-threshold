#pragma once
#include <string>
#include <vector>
using namespace std;

class Graph {
public:
  Graph() {}
  Graph(int nodes);

  int number_of_nodes() const;
  void add_edge(int node_a, int node_b);
  void print_graph() const;

  int count_connected_components() const;
  bool is_connex() const;
  const vector<int> &adjacent_nodes(int node) const;
  void export_graph(const string &path) const;

private:
  // Graph represented with an adjacency list.
  // connections[i] are the connected nodes to the node i
  vector<vector<int>> connections;
};

Graph read_graph(const string &path);

Graph new_triangular_graph(int alt);
Graph new_grid_graph(int width);
