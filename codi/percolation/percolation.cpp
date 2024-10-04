#include "percolation.h"
#include <random>

Graph node_percolation(Graph &graph, float q) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist100(0, 100);

  int old_n_nodes = graph.number_of_nodes();
  
  vector<int> survivors;
  vector<int> new_index(old_n_nodes,-1); 
  //if new_index[u] == -1, it's an eliminated node
  //otherwise, it's the new_index of the node with old_index u
  int deleted_counter = 0;
  for (int u = 0; u < old_n_nodes; ++u) {
    if (dist100(rng) / float(100) <= q)
    {
      new_index[u] = u - deleted_counter;
      survivors.push_back(u);
    }
    else ++deleted_counter;
  }
  int new_n_nodes = survivors.size();
  Graph new_graph(new_n_nodes);
  for (int u = 0; u < new_n_nodes; ++u) {
    vector<int> adjacent = graph.adjacent_nodes(survivors[u]); // adjacent nodes to surviving node in old graph
    for (int v : adjacent) {
      if (new_index[v] != -1) // v is a surviving node
      {
        int new_v = new_index[v];
        if (new_v > u)
          new_graph.add_edge(u, new_v); // add edge with updated node index
      }
    }
  }

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
