#include "percolation.h"
#include <random>

Graph node_percolation(Graph &graph, float q) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist100(0,100); 

  int old_n_nodes = graph.number_of_nodes();
  map<int,int> diff_accum;
  int deleted_counter = 0;
  for (int u = 0; u < old_n_nodes; ++u)
  {
    if (dist100(rng)/float(100) <= q) diff_accum[u] = deleted_counter;
    else ++deleted_counter;
  } 
  int new_n_nodes = diff_accum.size();
  Graph new_graph(new_n_nodes);
  for (auto it_u = diff_accum.begin(); it_u != diff_accum.end(); ++it_u)
  {
    int new_u = it_u->first - it_u->second;
    vector<int> adjacent = graph.adjacent_nodes(it_u->first); //adjacent nodes to surviving node in old graph
    for (int v : adjacent)
    {
      auto it_v = diff_accum.find(v); //iterator points to diff_accum of v, or diff_accum.end() if it's not a surviving node
      if (it_v != diff_accum.end()) // v is a surviving node
      {
        int new_v = v - it_v->second;
        if (new_v > new_u)  new_graph.add_edge(new_u, new_v); //add edge with updated node "id"
      }
    }  
  }
  /*
  cout << "surviving nodes: " << endl;
  for (auto it = diff_accum.begin(); it != diff_accum.end(); ++it) cout << it->first << " ";
  cout << endl;
  cout << "old graph" << endl;
  graph.print_graph();
  cout << "new graph" << endl;
  new_graph.print_graph();*/
  
  return new_graph;
}

Graph edge_percolation(Graph &graph, float q) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist100(0,100); 

  std::cout << dist100(rng) << std::endl;

  int n_nodes = graph.number_of_nodes();
  Graph new_graph(n_nodes);
  
  for (int u = 0; u < n_nodes; ++u)
  {
    vector<int> u_adjacent = graph.adjacent_nodes(u);
    for (int v : u_adjacent)
    {
      if (v > u && dist100(rng)/float(100) <= q) new_graph.add_edge(u,v);
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
