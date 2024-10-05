#pragma once
#include "../graph/graph.h"
#include <iostream>
#include <map>

// Returns the probability for the graph to be connex with a percolation of q
float node_percolation_probability(const Graph &graph, float q, int samples);

// Returns the probability for a graph to be connex with a percolation of q
float edge_percolation_probability(const Graph &graph, float q, int samples);
