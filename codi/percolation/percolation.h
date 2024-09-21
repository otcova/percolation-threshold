#pragma once
#include "../graph/graph.h"

// Creates a new graph with some deleted nodes.
// q: probability of a node to be conserved
Graph node_percolation(Graph &graph, float q);

// Creates a new graph with some deleted edges.
// q: probability of an edge to be conserved
Graph edge_percolation(Graph &graph, float q);

// Returns the probability for the graph to be connex with a percolation of q
float node_percolation_probability(Graph &graph, float q);

// Returns the probability for a graph to be connex with a percolation of q
float edge_percolation_probability(Graph &graph, float q);
