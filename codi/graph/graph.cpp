#include "graph.h"
#include <fstream>
#include <iostream>
#include <stack>
using namespace std;

Graph::Graph(int nodes) : connections(nodes) {}

int Graph::number_of_nodes() { return connections.size(); }

void Graph::add_edge(int node_a, int node_b) {
  int node_max = max(node_a, node_b);
  while (number_of_nodes() <= node_max)
    connections.push_back(vector<int>());

  connections[node_a].push_back(node_b);
  connections[node_b].push_back(node_a);
}

int Graph::count_connected_components() {
  vector<int> visited(number_of_nodes(), false);
  int connected_components = 0;

  stack<int> s;
  for (int node_visited = 0; node_visited < number_of_nodes(); ++node_visited) {
    if (visited[node_visited])
      continue;
    s.push(node_visited);
    while (not s.empty()) {
      int node_top = s.top();
      s.pop();
      for (int node : connections[node_top]) {
        if (not visited[node]) {
          visited[node] = true;
          s.push(node);
        }
      }
    }
    ++connected_components;
  }

  return connected_components;
}

Graph read_graph(const string &path) {
  Graph graph;
  ifstream file(path);
  int a, b;
  while (file >> a >> b)
    graph.add_edge(a, b);
  

  return graph;
}