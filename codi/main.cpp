#include "analisis.h"
#include "export/csv.h"
#include "graph/graph.h"
#include "utils/cli_utils.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <math.h>
using namespace std::chrono;
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
  cout << " 1. Genera i carga grafs" << endl;
  cout << " 2. Analisis de percolacions" << endl;
  cout << " 3. Print grafs cargats" << endl;
  cout << " 4. Clear data" << endl;
  cout << " 5. Clear file geometric" << endl;
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
  return confirm_action(msg);
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

void cargar_graph_geometric() {
  string tipus = "geometric";

  cout << "cargant graphs..." << endl;
  if (ask("Estas segur de sustituir amb graph de tipus " + tipus, tipus)) {
    string directoryPath = "./dades/" + tipus + "/original";
    int fileCount = count_file(directoryPath);
    if (fileCount == 0)
      print_error("No hay ficheros");
    else {
      cout << "cargant " << fileCount << " graphs..."
           << endl; // sempra carga un graf

      int start_i = 0;
      int end_i = 1000;
      while (start_i < end_i) {
        ifstream archivo("./dades/" + tipus + "/original/graph" +
                         to_string(start_i) + ".edgelist");
        if (archivo.good()) {
          Graph graph =
              read_graph("./dades/" + tipus + "/original/graph" +
                         to_string(start_i) + ".edgelist"); // cargar el graf0
          cargar_tipus(tipus, graph);                       // carga un graf
        } else {
          if (start_i == 0 || start_i == 1000)
            cout << "ended with file_num == -1" << endl;
          else
            cout << "ended with file_num == " << start_i - 1 << endl;
          break;
        }
        ++start_i;
      }
      cout << "done" << endl;
    }
  }
}

void generar_geometric_graphs() {
  bool graf_files_found =
      filesystem::exists("./dades/geometric/original/graph0.edgelist");
  bool generate_new_grafs = true;

  if (graf_files_found) {
    generate_new_grafs = not confirm_action(
        "Vols carregar els grafs geometric guardats en fitxers enlloc "
        "de generar de nous?");
  }

  if (generate_new_grafs) {
    system("python3 ./codi/graph/generador_grafos.py ");
    cout << "done" << endl;
  }

  cargar_graph_geometric();
}

void genera_graelles_graph() {
  // generar graelles graph n * n
  if (ask("Estas segur de generar i cargar graph de graelles", "graelles")) {
    for (int n : {1,  2,  3,  4,   5,   6,   7,   8,   9,   10,
                  20, 40, 50, 100, 200, 400, 500, 600, 800, 1000}) {
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
    }
  } else
    print_error("cancelat");
}

int get_height(int n) {
  return (1 + sqrt(1 + 8 * n)) / 2;
}

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
  cout << "Introduce tipus de graf a generar";
  cout << "(GRaella, GEometric, Triangular): ";

  string tipus;
  cin >> tipus;
  tipus = to_lowercase(tipus);

  if (starts_with(tipus, "gr"))
    genera_graelles_graph();
  else if (starts_with(tipus, "ge"))
    generar_geometric_graphs();
  else if (starts_with(tipus, "t"))
    genera_triangular_graph();
  else
    print_error("error format generar");
}

void prova_exportar_dades() {
  TableFile file("./dades/sine_wave.csv", {"x", "sin(x)"});
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
      analisis(conj_graph_global, tipus_conj_global);
    else if (option == 3)
      print_conj_graph();
    else if (option == 4)
      clear_data();
    else if (option == 5) {
      if (filesystem::remove_all("./dades/geometric/original"))
        perror("remove_all");
      if (filesystem::create_directories("./dades/geometric/original"))
        perror("create_directories");
    } else
      menu();
    cout << "> ";
  }
}
