#include "export/csv.h"
#include "graph/graph.h"
#include "percolation/percolation.h"
#include "utils/chrono.h"
#include "utils/cli_utils.h"
#include <algorithm>
#include <atomic>
#include <filesystem>
#include <iostream>
#include <math.h>
#include <thread>
#include <vector>

using namespace std;

enum PercolationType {
  Node,
  Edge,
};

struct Analisis {
  vector<thread> threads;
  atomic_int rows_todo;

  const vector<Graph> &graphs;
  const string &graph_type;

  vector<float> probability_table;

  int q_samples;
  int samples;

  PercolationType percolation_type;

  Chrono chrono;
  atomic<float> last_report_time;
  atomic<int> last_progress_report;

  void run_analisis() {
    unsigned long total_nodes = 0;
    for (const Graph &graph : graphs)
      total_nodes += graph.number_of_nodes() * samples * q_samples;
    cout << "Percolant " << total_nodes << " nodes..." << endl;

    probability_table.resize(graphs.size() * q_samples);

    rows_todo = q_samples;
    threads.resize(thread::hardware_concurrency());

    last_report_time = 0;
    last_progress_report = 0;
    chrono = Chrono();

    for (int i = 0; i < threads.size(); ++i)
      threads[i] = thread(&Analisis::thread_runtime, this);

    thread_runtime();

    for (int i = 0; i < threads.size(); ++i)
      threads[i].join();

    float total_time = round(100 * chrono.seconds()) / 100;
    cout << "Analisis done in " << total_time << "s" << endl;
  }

  float &probability_cell(int row, int graph) {
    const int columns = graphs.size();
    return probability_table[row * columns + graph];
  }

private:
  void calculate_cell(int row, int graph_index) {
    float q = float(row) / float(q_samples - 1);
    float p;
    if (percolation_type == Node)
      p = node_percolation_probability(graphs[graph_index], q, samples);
    else
      p = edge_percolation_probability(graphs[graph_index], q, samples);

    probability_cell(row, graph_index) = p;
  }

  void calculate_row(int row) {
    for (int graph = 0; graph < graphs.size(); ++graph)
      calculate_cell(row, graph);
  }

  void thread_runtime() {
    int row = --rows_todo;
    while (row >= 0) {
      report_progress(row);

      calculate_row(row);
      row = --rows_todo;
    }
    report_progress(0);
  }

  void report_progress(int row) {
    int now = chrono.seconds();
    if (now - last_report_time < 5.)
      return;

    last_report_time = now;

    float progress = float(q_samples - row) / float(q_samples);
    progress = sqrt(progress); // Progress estimation
    int progress_report = min(93, int(100 * progress));
    if (progress_report > last_progress_report) {
      last_progress_report = progress_report;
      cout << "Progress: " << progress_report << "%" << endl;
    }
  }
};

void analisis(const vector<Graph> &graphs, const string &graph_type) {
  /// Analisis Prompt ///
  if (graphs.empty()) {
    cout << "No hi han graphs carregats" << endl;
    cout << "Operacio cancelada" << endl;
    return;
  }

  for (const Graph &g : graphs)
    if (not g.is_connex())
      cout << "Hi ha grafs no connexos havans de percolar" << endl;

  filesystem::create_directories("./dades/percolat/");

  string percolation_type =
      choose_option("Tipus de percolacio", {"nodes", "arestes"});

  string file_path =
      "./dades/percolat/" + graph_type + "_" + percolation_type + ".csv";

  if (filesystem::exists(file_path)) {
    if (!confirm_action("Aquesta operacio sobrescriura:\n  " + file_path +
                        "\nVols continuar?")) {
      cout << "Operacio cancelada" << endl;
      return;
    }
  }

  int q_samples =
      read_value<int>("Quantitat de valors de q a analitzar (51)", 51);
  int samples = read_value<int>("Percolacions per cada graph i q (200)", 200);

  /// Run the analisis ///
  Analisis analisis = {
      .graphs = graphs,
      .graph_type = graph_type,
      .q_samples = q_samples,
      .samples = samples,
      .percolation_type = percolation_type == "nodes" ? Node : Edge,
  };
  analisis.run_analisis();

  /// Find the phase transition ///
  vector<pair<float, float>> phase_transition(graphs.size(), {0, 0});
  float phase_transition_min_start = 1.;
  float phase_transition_max_end = 0.;

  bool mean_the_phase_transition = graphs.size() > 1;
  for (const Graph graph : graphs)
    if (graph.number_of_nodes() != graphs[0].number_of_nodes())
      mean_the_phase_transition = false;

  for (int graph_index = 0; graph_index < graphs.size(); ++graph_index) {
    float start_q = 0.;
    float end_q = 1.;

    for (int row = 0; row < q_samples; ++row) {
      float q = float(row) / float(q_samples - 1);
      float p = analisis.probability_cell(row, graph_index);
      if (p == 0)
        start_q = q;
    }

    for (int row = q_samples - 1; row >= 0; --row) {
      float q = float(row) / float(q_samples - 1);
      float p = analisis.probability_cell(row, graph_index);
      if (p == 1)
        end_q = q;
      else
        break;
    }

    phase_transition_min_start = min(phase_transition_min_start, start_q);
    phase_transition_max_end = max(phase_transition_max_end, end_q);

    phase_transition[graph_index].first = start_q;
    phase_transition[graph_index].second = end_q;
  }

  /// Output: Write to csv file ///
  vector<string> columns = {"q"};
  for (int i = 0; i < graphs.size(); ++i) {
    string nodes = to_string(graphs[i].number_of_nodes());
    columns.push_back("p" + to_string(i) + "_n" + nodes);
  }
  columns.push_back("p_mean");

  columns.push_back(""); // Table separation
  columns.push_back("graph");
  columns.push_back("nodes");
  columns.push_back("start_q");
  columns.push_back("mean_q");
  columns.push_back("end_q");

  TableFile table(file_path, columns);

  for (int row = 0; row < q_samples; ++row) {
    float q = float(row) / float(q_samples - 1);
    table << q;

    float p_sum = 0.;

    for (int graph_index = 0; graph_index < graphs.size(); ++graph_index) {
      float p = analisis.probability_cell(row, graph_index);
      table << p;
      p_sum += p;
    }
    table << (p_sum / graphs.size());

    table << ""; // Table separation
    if (row < graphs.size()) {
      table << row << graphs[row].number_of_nodes();

      float start = phase_transition[row].first;
      float end = phase_transition[row].second;
      float mean = (start + end) / 2;
      table << start << mean << end;
    } else if (row == graphs.size() && mean_the_phase_transition) {
      float start = phase_transition_min_start;
      float end = phase_transition_max_end;
      float mean = (start + end) / 2;
      table << "All" << "-" << start << mean << end;
    } else {
      table << "" << "" << "" << "" << "";
    }
  }
}
