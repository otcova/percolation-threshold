#include "percolation.h"
#include <random>

// Creates a new graph with some deleted nodes.
// q: probability of a node to be conserved
Graph node_percolation(const Graph &graph, float q) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<> dist100(0., 1.);

  int old_n_nodes = graph.number_of_nodes();

  vector<int> survivors;
  vector<int> new_index(old_n_nodes, -1);
  // if new_index[u] == -1, it's an eliminated node
  // otherwise, it's the new_index of the node with old_index u
  int deleted_counter = 0;
  for (int u = 0; u < old_n_nodes; ++u) {
    if (dist100(rng) <= q) {
      new_index[u] = u - deleted_counter;
      survivors.push_back(u);
    } else
      ++deleted_counter;
  }
  int new_n_nodes = survivors.size();
  Graph new_graph(new_n_nodes);
  for (int u = 0; u < new_n_nodes; ++u) {
    vector<int> adjacent = graph.adjacent_nodes(
        survivors[u]); // adjacent nodes to surviving node in old graph
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

// Creates a new graph with some deleted edges.
// q: probability of an edge to be conserved
Graph edge_percolation(const Graph &graph, float q) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist100(0, 100);

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

// Equivalent to do: `node_percolation_probability(graph)`
// But this function is some orders of magnitude faster because
// it merges multiple algorithms preventing a lot of memory allocations and
// other stuff.
float fast_node_percolation(const Graph &graph, float q, int samples) {
  random_device device;
  mt19937 rng(device());
  std::uniform_real_distribution<> dist(0., 1.);

  int connex_count = 0;

  vector<int> visited(graph.number_of_nodes(), 0);
  vector<int> nodes_to_visit;

  for (int sample_index = 1; sample_index <= samples; ++sample_index) {
    int connected_components = 0;

    int deleted_nodes = 0;
    int visited_count = 1;
    visited[0] = sample_index;
    nodes_to_visit.push_back(0);

    while (not nodes_to_visit.empty()) {
      int node = nodes_to_visit.back();
      nodes_to_visit.pop_back();

      for (int node : graph.adjacent_nodes(node)) {
        if (visited[node] != sample_index) {
          visited[node] = sample_index;
          if (dist(rng) <= q) { // Check if node should exist
            ++visited_count;
            nodes_to_visit.push_back(node);
          } else {
            ++deleted_nodes;
          }
        }
      }
    }

    if (visited_count + deleted_nodes == graph.number_of_nodes())
      ++connex_count;
  }

  return float(connex_count) / float(samples);
}

// Equivalent to do: `edge_percolation_probability(graph)`
// But this function is some orders of magnitude faster because
// it merges multiple algorithms preventing a lot of memory allocations and
// other stuff.
float fast_edge_percolation(const Graph &graph, float q, int samples) {
  random_device device;
  mt19937 rng(device());
  std::uniform_real_distribution<> dist(0., 1.);

  int connex_count = 0;

  vector<int> visited(graph.number_of_nodes(), 0);
  vector<int> nodes_to_visit;

  for (int sample_index = 1; sample_index <= samples; ++sample_index) {
    int connected_components = 0;

    int visited_count = 1;
    visited[0] = sample_index;
    nodes_to_visit.push_back(0);

    while (not nodes_to_visit.empty()) {
      int node = nodes_to_visit.back();
      nodes_to_visit.pop_back();

      for (int node : graph.adjacent_nodes(node)) {
        if (visited[node] != sample_index) {
          if (dist(rng) <= q) { // Check if edge should exist
            ++visited_count;
            visited[node] = sample_index;
            nodes_to_visit.push_back(node);
          }
        }
      }
    }

    if (visited_count == graph.number_of_nodes())
      ++connex_count;
  }

  return float(connex_count) / float(samples);
}

float node_percolation_probability(const Graph &graph, float q, int samples) {
  /*
  float count = 0;
  for (int i = 0; i < samples; ++i) {
    if (node_percolation(graph, q).is_connex())
      ++count;
  }
  return count / float(samples);
  */
  return fast_node_percolation(graph, q, samples);
}

float edge_percolation_probability(const Graph &graph, float q, int samples) {
  /*
  float count = 0;
  for (int i = 0; i < samples; ++i) {
    if (edge_percolation(graph, q).is_connex())
      ++count;
  }
  return count / float(samples);
  */
  return fast_edge_percolation(graph, q, samples);
}
