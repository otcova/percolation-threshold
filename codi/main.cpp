#include "./graph/graph.h"
#include "export/csv.h"
#include <iostream>
#include <math.h>
#include <cstdlib>
using namespace std;

vector<Graph> conj_graph;

void cargar_graph_geometric()
{
  cout << "cargar graph..." << endl; 
  // modificar per conj graphs
  conj_graph.push_back(read_graph("./dades/random_graph.edgelist"));
  cout << "done" << endl;
}

void generar_graphs() {
  cout << "generant graphs..." << endl;
  system("python3 ./codi/graph/generador_grafos.py ");
  cout << "done" << endl;
  cargar_graph_geometric();
}


void genera_carga_graph_graelles()
{
  // modificar generar i cargar
  // xiangfeng
}

void analisis() {
  cout << "analitzant...: ";
  // modificar per conj graphs i afegir percolacio
  // xavier
  cout << conj_graph[0].count_connected_components() << endl;
}

void prova_exportar_dades() {
  TableFile file("sine_wave", {"x", "sin(x)"});
  for (float x = 0; x < 30; x += 0.05)
    file << x << sin(x);
}

void menu() {
  cout << u8"\033[2J\033[1;1H";
  cout << "-- Menu --" << endl;
  cout << "0. Exit" << endl;
  cout << "1. Generar graphs" << endl;
  cout << "2. Cargar graphs geometrics" << endl;
  cout << "3. Genera i carga graphs graelles" << endl;
  cout << "4. Analisis" << endl;
  cout << "5. Prova: exportar dades" << endl;
  cout << ".. Clear" << endl;
}

int main() {
  menu();
  cout << "> ";

  int option;
  while (cin >> option) {

    if (option == 0)
      break;
    else if (option == 1)
      generar_graphs();
    else if (option == 2)
      cargar_graph_geometric();
    else if (option == 3)
      genera_carga_graph_graelles();
    else if (option == 4)
      analisis();
    else if (option == 5)
      prova_exportar_dades();
    else 
      menu();
    cout << "> ";
  }
}
