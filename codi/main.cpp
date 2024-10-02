#include "./graph/graph.h"
#include "./percolation/percolation.h"
#include "export/csv.h"
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <math.h>
#include <fstream>
using namespace std;

string tipus_conj_global;
vector<Graph> conj_graph_global;
const vector<string> vtipus = {"graelles", "geometric", "triangular", "random"};
const vector<string> v_no_cargar = {"graelles", "triangular"};

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
  cout << endl;
  cout << text << endl;
}

void clear_data() {
  cout << "clearing data..." << endl;
  conj_graph_global.clear();
  cout << "done" << endl;
}

void short_cut(string &tipus) {
  if (tipus == "gra")
    tipus = "graelles";
  else if (tipus == "geo")
    tipus = "geometric";
  else if (tipus == "tri")
    tipus = "triangular";
  else if (tipus == "rand")
    tipus = "random";
}

int count_file(const string &path) {
  int fileCount = 0;
  if (filesystem::exists(path)) {
    for (const auto &entry : filesystem::directory_iterator(path)) {
      if (entry.is_regular_file())
        ++fileCount;
    }
  }
  return fileCount;
}

bool ask(const string &msg, const string &tipus) {
  if (tipus_conj_global.empty() || tipus == tipus_conj_global)
    return true;
  cout << "Conjunt cargat amb graphs " << tipus_conj_global << endl;
  cout << msg << " [yes/no]: ";
  string ok;
  cin >> ok;
  return (ok == "yes");
}

bool is_connex(const string &path) {
  return read_graph(path).count_connected_components() == 1;
}

void cargar_tipus(const string &tipus, const Graph &graph) {
  if (tipus != tipus_conj_global && !tipus_conj_global.empty())
    clear_data();
  tipus_conj_global = tipus;
  conj_graph_global.push_back(graph);
}

void cargar_graph(const string &tipus) {
  if (find(vtipus.begin(), vtipus.end(), tipus) != vtipus.end()) {
    cout << "cargant graphs..." << endl;
    if (ask("Estas segur de sustituir amb graph de tipus " + tipus, tipus)) {
      string directoryPath = "./dades/" + tipus + "/original";
      int fileCount = count_file(directoryPath);
      if (fileCount == 0)
        print_error("No hay ficheros");
      else {
        cout << "cargant sempre 1 graf pero hi han " << fileCount
             << " graphs..." << endl; // sempra carga un graf
        int start_i, end_i;
        if (tipus == "geometric") { // Els grafs geometrics estan guardats als fitxers amb cardinalitat 0..999
          start_i = 0;
          end_i = 1000;
        }
        else { // Els grafs geometrics estan guardats als fitxers amb cardinalitat 1000..1999
          start_i = 1000;
          end_i = 2000;
        } 
        while (start_i < end_i)  {
          std::ifstream archivo("./dades/" + tipus + "/graph" + std::to_string(start_i) +".edgelist");
          if (archivo.good()) {  
            Graph graph = read_graph("./dades/" + tipus + "/original/graph" + std::to_string(start_i) +".edgelist"); // cargar el graf0
            cargar_tipus(tipus, graph);                         // carga un graf
          }
          else {
            if (start_i == 0 || start_i == 1000) cout << "ended with file_num == -1" << endl;
            else cout << "ended with file_num == " << start_i-1 << endl;
            break;
          }
          ++start_i;
        }
        cout << "done" << endl;
      }
    }
  } else
    print_error("error format cargar");
  // modificar per conj graphs
}

void cargar_graph() {
  cout << "Introduce tipus de graf[geo, rand] a cargar: ";
  string tipus;
  cin >> tipus;
  short_cut(tipus);
  if (find(v_no_cargar.begin(), v_no_cargar.end(), tipus) != v_no_cargar.end())
    print_error("No es pot cargar aquest tipus");
  else
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
  if (ask("Estas segur de generar i cargar graph de graelles", "graelles")) {
    cout << "Introduce n para un grafo graelles de n*n nodos: ";
    int n;
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
    cout << "done" << endl;
    cout << "cargant " << 1 << " graphs..." << endl;
    cargar_tipus("graelles", graph);
    cout << "done" << endl;
  } else
    print_error("cancelat");
}

int get_height(int n) { return (1 + sqrt(1 + 8 * n)) / 2; }

void genera_triangular_graph() {
  if (ask("Estas segur de generar i cargar graph de triangulars",
          "triangular")) {
    // generar graelles graph n * n
    cout << "Introduce altura[1..] para un grafo triangular de h altura: ";
    int alt;
    cin >> alt;

    int n = alt * (alt + 1) / 2;
    cout << "generant graphs de " << n << " nodes..." << endl;
    Graph graph(n);
    // graf tiangular
    // conectar amb dos fills
    for (int i = 0; i < n - alt; ++i) {
      int h = get_height(i);
      graph.add_edge(i, i + h);
      graph.add_edge(i, i + h + 1);
    }
    // conectar gemans
    for (int i = 1; i < n - 1; ++i) {
      i += (get_height(i) != get_height(i + 1));
      graph.add_edge(i, i + 1);
    }

    cout << "done" << endl;
    cout << "cargant " << 1 << " graphs..." << endl;
    cargar_tipus("triangular", graph);
    cout << "done" << endl;
  } else
    print_error("cancelat");
}

void generar_graphs() {
  cout << "Introduce tipus de graf[gra, geo, tri, rand] a generar: ";
  string tipus;
  cin >> tipus;

  if (tipus == "gra")
    genera_graelles_graph();
  else if (tipus == "geo")
    generar_geometric_graphs();
  else if (tipus == "tri")
    genera_triangular_graph();
  else if (tipus == "rand")
    generar_geometric_graphs();
  else
    print_error("error format generar");
}

void analisis() {
  if (conj_graph_global.empty()) {
    print_error("error conjunt buit");
    return;
  }
  if (ask("Estas segur de analitzar", "")) {
    string directoryPath = "./dades/" + tipus_conj_global;
    cout << "analitzant " << conj_graph_global.size() << " graphs...: " << endl;
    float q = 0.7;
    Graph new_graph = node_percolation(conj_graph_global[0], q);

    filesystem::create_directory(directoryPath);
    filesystem::create_directory(directoryPath + "/percolat");
    new_graph.export_graph(directoryPath + "/percolat/graph.edgelist");

    // resultats
    cout << "result with q = " << q << ":" << endl;
    for (int i = 0; i < conj_graph_global.size(); ++i) {
      int c1 = conj_graph_global[0].count_connected_components();
      int c2 = new_graph.count_connected_components(); // cambiar a conjunt
      cout << i << ": " << c1 << " -> " << c2 << endl;
    }

    cout << "done" << endl;
  } else
    print_error("cancelat");

  // modificar per conj graphs i afegir percolacio
  // xavier
  // Graph new_graph = edge_percolation(conj_graph[0],0.7);
  // cout << "old comp conn ant: " <<
  // conj_graph[0].count_connected_components()
  // << endl; cout << "new comp conn ant: " <<
  // new_graph.count_connected_components() << endl;
}

void prova_exportar_dades() {
  TableFile file("sine_wave", {"x", "sin(x)"});
  for (float x = 0; x < 30; x += 0.05)
    file << x << sin(x);
}

void print_conj_graph() {
  cout << "Printeant el nombre de nodes de " << conj_graph_global.size()
       << " graphs " << tipus_conj_global << " cargats..." << endl;
  for (const Graph &graph : conj_graph_global) {
    cout << graph.number_of_nodes() << " ";
    if (graph.number_of_nodes() < 10)
      graph.print_graph();
  }

  cout << (conj_graph_global.empty() ? "nothing here" : "") << endl;
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
