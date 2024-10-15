#include "cli_utils.h"
#include "export/csv.h"
#include "graph/graph.h"
#include "percolation/percolation.h"
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <math.h>
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
    for (int n : {1, 5, 10, 20, 40, 50, 100, 200}) {
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

void analisis() {

  //// Analisis Prompt ////

  if (conj_graph_global.empty()) {
    cout << "No hi han graphs carregats" << endl;
    cout << "Operacio cancelada" << endl;
    return;
  }
  for (const Graph &g : conj_graph_global)
    if (not g.is_connex())
      cout << "graf a percolat no es connex" << endl;
  if (filesystem::create_directories("./dades/percolat/"))
    perror("create_directories");

  string percolation_type =
      choose_option("Tipus de percolacio", {"nodes", "arestes"});
  bool node_percolation = percolation_type == "nodes";

  string file_path =
      "./dades/percolat/" + tipus_conj_global + "_" + percolation_type + ".csv";

  if (filesystem::exists(file_path)) {
    if (!confirm_action("Aquesta operacio sobrescriura " + file_path +
                        ". Vols continuar?")) {
      cout << "Operacio cancelada" << endl;
      return;
    }
  }

  int q_samples =
      read_value<int>("Quantitat de valors de q a analitzar (50)", 50);
  int samples = read_value<int>("Percolacions per cada graph i q (200)", 200);

  int total_samples = samples * q_samples * conj_graph_global.size();
  cout << "Calculant " << total_samples << " graphs percolats" << endl;

  //// Setup Analisis Output File ////

  vector<string> columns_titles = {"q"};
  for (int i = 0; i < conj_graph_global.size(); ++i) {
    int nodes = conj_graph_global[i].number_of_nodes();
    columns_titles.push_back("p" + to_string(i) + "_n" + to_string(nodes));
  }
  columns_titles.push_back("p_mean");
  TableFile file(file_path, columns_titles);

  vector<float> phase_transition_start_q(conj_graph_global.size(), 0.);
  vector<float> phase_transition_end_q(conj_graph_global.size(), 1.);

  //// Generate Percolations ////

  int count = 1;
  for (int q_sample_index = 0; q_sample_index < q_samples; ++q_sample_index) {
    float q = float(q_sample_index) / float(q_samples - 1);
    file << q;
    float p_sum = 0.;
    if (q * 100.f > 10 * count) {
      cout << "Percentage calculat: " << q * 100.f << "%" << endl;
      ++count;
    }

    // Graph transition phase probability
    for (int i = 0; i < conj_graph_global.size(); ++i) {
      const Graph &graph = conj_graph_global[i];
      float p;
      if (node_percolation)
        p = node_percolation_probability(graph, q, samples);
      else
        p = edge_percolation_probability(graph, q, samples);
      p_sum += p;
      file << p;

      // Calculate phase transition q
      if (p == 0.)
        phase_transition_start_q[i] = q;
      else if (p < 1.)
        phase_transition_end_q[i] = q;
    }

    // Mean transition phase probability
    file << p_sum / float(conj_graph_global.size());
  }

  //// Output Percolation transition phase ////
  string transition_file_path = "./dades/percolat/" + tipus_conj_global + "_" +
                                percolation_type + "_transition.csv";

  TableFile transition_file(transition_file_path,
                            {"graph", "nodes", "start q", "mean q", "end q"});

  { // Take into acount all graphs
    float start_q = *std::min_element(phase_transition_start_q.begin(),
                                      phase_transition_start_q.end());
    float end_q = *std::max_element(phase_transition_end_q.begin(),
                                    phase_transition_end_q.end());
    transition_file << "All" << "-" << start_q << (start_q + end_q) / 2
                    << end_q;
  }

  for (int i = 0; i < conj_graph_global.size(); ++i) {
    int nodes = conj_graph_global[i].number_of_nodes();
    float start_q = phase_transition_start_q[i];
    float end_q = phase_transition_end_q[i];
    transition_file << i << nodes << start_q << (start_q + end_q) / 2 << end_q;
  }

  cout << "done" << endl;
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
      analisis();
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
