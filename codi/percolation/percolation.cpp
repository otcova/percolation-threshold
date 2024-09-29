#include "percolation.h"
#include <random>

Graph node_percolation(Graph &graph, float q) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist100(0, 100);

  cout << "old graph" << endl;
  //graph.print_graph();
  
  int old_n_nodes = graph.number_of_nodes();
  vector<int> surviving_nodes;
  for (int u = 0; u < old_n_nodes; ++u) {
    if (dist100(rng) / float(100) <= q)
      surviving_nodes.push_back(u);
  }

  int new_n_nodes = surviving_nodes.size();
  map<int, int> diff_accum;
  int last_diff_accum;
  if (new_n_nodes > 0) {
    last_diff_accum = surviving_nodes[0];
    diff_accum[surviving_nodes[0]] = last_diff_accum;
  }

  for (int u = 1; u < new_n_nodes; ++u) {
    int diff_aux =
        surviving_nodes[u] - surviving_nodes[u - 1] - 1 + last_diff_accum;
    last_diff_accum = diff_aux;
    diff_accum[surviving_nodes[u]] = diff_aux;
  }

  Graph new_graph(new_n_nodes);
  for (int u = 0; u < new_n_nodes; ++u) {
    vector<int> adjacent = graph.adjacent_nodes(
        surviving_nodes[u]); // adjacent nodes to surviving node in old graph
    for (int v : adjacent) {
      auto it =
          diff_accum.find(v); // iterator points to diff_accum of v, or
                              // diff_accum.end() if it's not a surviving node
      if (it != diff_accum.end()) // v is a surviving node
      {
        int new_v = v - diff_accum[v];
        if (new_v > u)
          new_graph.add_edge(u, new_v); // add edge with updated node "id"
      }
    }
  }
  cout << "surviving nodes: " << new_n_nodes << "/" << old_n_nodes << endl;
  for (int u : surviving_nodes)
    cout << u << " ";
  cout << endl;
  cout << "old graph" << endl;
  // graph.print_graph();
  cout << "new graph" << endl;
  // new_graph.print_graph();
  return new_graph;
}

Graph edge_percolation(Graph &graph, float q) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist100(0, 100);

  std::cout << dist100(rng) << std::endl;

  int n_nodes = graph.number_of_nodes();
  Graph new_graph(n_nodes);

  for (int u = 0; u < n_nodes; ++u) {
    vector<int> u_adjacent = graph.adjacent_nodes(u);
    for (int v : u_adjacent) {
      if (v > u && dist100(rng) / float(100) <= q)
        new_graph.add_edge(u, v);
    }
  }

  return new_graph;
}

float node_percolation_probability(Graph &graph, float q) {
  // TODO
  return 0;
}

float edge_percolation_probability(Graph &graph, float q) {
  // TODO
  return 0;
}
