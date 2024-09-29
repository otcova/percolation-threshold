#include "./graph/graph.h"
#include "./percolation/percolation.h"
#include "export/csv.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <math.h>
using namespace std;

vector<Graph> conj_graph;
const vector<string> vtipus = {"graelles", "geometric", "random"};

void menu() {
  cout << u8"\033[2J\033[1;1H";
  cout << "-- Menu --" << endl;
  cout << "-1. Exit" << endl;
  cout << " 0. Clear screen" << endl;
  cout << " 1. Genera i carga [tipus] graphs" << endl;
  cout << " 2. Cargar [tipus] graphs" << endl;
  cout << " 3. Analisis [tipus] graphs" << endl;
  cout << " 4. Print graphs cargats" << endl;
  cout << " 5. Clear data" << endl;
  cout << "... Clear screen" << endl;
}

void print_error(const string &text) {
  menu();
  cout << text << endl;
}

void short_cut(string& tipus)
{
  if (tipus == "gra")
    tipus = "graelles";
  else if (tipus == "geo")
    tipus = "geometric";
  else if (tipus == "rand")
    tipus = "random";
}

void cargar_graph(const string &tipus) {
  if (find(vtipus.begin(), vtipus.end(), tipus) != vtipus.end()) {
    cout << "cargant graphs..." << endl;
    conj_graph.push_back(read_graph("./dades/" + tipus + "/graph.edgelist"));
    cout << "done" << endl;
  } else
    print_error("error al cargar");
  // modificar per conj graphs
}

void cargar_graph() {
  cout << "Introduce tipus de graf[gra, geo, rand] a cargar: ";
  string tipus;
  cin >> tipus;
  short_cut(tipus);
  cargar_graph(tipus);
  // modificar per conj graphs
}

void generar_geometric_graphs() {
  cout << "generant graphs..." << endl;
  system("python3 ./codi/graph/generador_grafos.py ");
  cout << "done" << endl;
  cargar_graph("geometric");
}

void genera_graelles_graph() {
  // generar graelles graph n * n
  cout << "Introduce n [0..5000] para un grafo graelles de n*n nodos: ";

  int n = 50;
  cin >> n;

  cout << "generant graphs de " << n * n << " nodes..." << endl;
  Graph graph(n * n);
  // conectar horitzaontal
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n - 1; ++j)
      graph.add_edge(n * i + j, n * i + (j + 1));
  }
  // conectar vertical
  for (int j = 0; j < n; ++j) {
    for (int i = 0; i < n - 1; ++i)
      graph.add_edge(n * i + j, n * (i + 1) + j);
  }
  graph.export_graph("./dades/graelles/graph.edgelist");
  cout << "done" << endl;
  cargar_graph("graelles");
}

void generar_graphs() {
  cout << "Introduce tipus de graf[gra, geo, rand] a generar: ";
  string tipus;
  cin >> tipus;

  if (tipus == "gra")
    genera_graelles_graph();
  else if (tipus == "geo")
    generar_geometric_graphs();
  else if (tipus == "rand")
    generar_geometric_graphs();
  else
    print_error("error al generar");
}

void analisis() {
  cout << "Introduce tipus de graf[gra, geo, rand] a analitzar: ";
  string tipus;
  cin >> tipus;
  short_cut(tipus);
  if (find(vtipus.begin(), vtipus.end(), tipus) != vtipus.end()) {
    cout << "analitzant...: " << endl;
    Graph new_graph = node_percolation(conj_graph[0], 0.7);
    new_graph.export_graph("./dades/" + tipus + "/graph_percolat.edgelist");
    cout << "done" << endl;
  } else
    print_error("error al analitzar");
  // modificar per conj graphs i afegir percolacio
  // xavier
  // Graph new_graph = edge_percolation(conj_graph[0],0.7);
  // cout << "old comp conn ant: " << conj_graph[0].count_connected_components()
  // << endl; cout << "new comp conn ant: " <<
  // new_graph.count_connected_components() << endl;
}

void clear_data() {
  cout << "clearing data..." << endl;
  conj_graph.clear();
  cout << "done" << endl;
}

void prova_exportar_dades() {
  TableFile file("sine_wave", {"x", "sin(x)"});
  for (float x = 0; x < 30; x += 0.05)
    file << x << sin(x);
}

void print_conj_graph() {
  cout << "Printeant nombre de nodes graphs cargats..." << endl;
  for (const Graph &graph : conj_graph)
    cout << graph.number_of_nodes() << " ";
  cout << (conj_graph.empty() ? "nothing here" : "") << endl;
  cout << "done" << endl;
}

int main() {
  menu();
  cout << "> ";

  int option;
  while (cin >> option) {

    if (option == -1)
      break;
    else if (option == 0)
      menu();
    else if (option == 1)
      generar_graphs();
    else if (option == 2)
      cargar_graph();
    else if (option == 3)
      analisis();
    else if (option == 4)
      print_conj_graph();
    else if (option == 5)
      clear_data();
    else
      menu();
    cout << "> ";
  }
}
