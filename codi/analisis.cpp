#include "export/csv.h"
#include "percolation/percolation.h"
#include "utils/chrono.h"
#include "utils/cli_utils.h"
#include <algorithm>
#include <atomic>
#include <filesystem>
#include <future>
#include <iostream>
#include <math.h>
#include <vector>

enum PercolationType {
  Node,
  Edge,
};

struct Analisis {
  vector<future<void>> threads;
  static constexpr int rows_per_thread = 4;
  atomic_int completed_cells = 0;

  const vector<Graph> &graphs;
  const string &graph_type;

  vector<float> probability_table;

  int q_samples;
  int samples;

  PercolationType percolation_type;

  Chrono chrono;
  float last_report_time = 0;

  void run_analisis() {
    int total_samples = samples * q_samples * graphs.size();
    cout << "Calculant " << total_samples << " graphs percolats" << endl;

    probability_table.resize(graphs.size() * q_samples);

    completed_cells = 0;
    threads.resize((q_samples / rows_per_thread) + 1);

    last_report_time = 0;
    chrono = Chrono();

    for (int i = 0; i < threads.size(); ++i)
      threads[i] = async(&Analisis::thread_task, this, i);

    while (!threads.empty()) {
      if (threads.back().wait_for(chrono::seconds(3)) == future_status::ready)
        threads.pop_back();

      print_progress();
    }

    float total_time = round(100 * chrono.seconds()) / 100;
    cout << "Analisis done in " << total_time << "s" << endl;

    print_progress();
  }

  float &probability_cell(int q_sample, int graph) {
    const int columns = graphs.size();
    return probability_table[q_sample * columns + graph];
  }

  void calculate_cell(int q_sample_index, int graph_index) {
    float q = float(q_sample_index) / float(q_samples - 1);
    float p;
    if (percolation_type == Node)
      p = node_percolation_probability(graphs[graph_index], q, samples);
    else
      p = edge_percolation_probability(graphs[graph_index], q, samples);

    probability_cell(q_sample_index, graph_index) = p;
    ++completed_cells;
  }

private:
  void calculate_row(int q_sample_index) {
    for (int i = 0; i < graphs.size(); ++i)
      calculate_cell(q_sample_index, i);
  }

  void thread_task(int task_index) {
    if (task_index == q_samples / rows_per_thread) {
      int q_sample = task_index * rows_per_thread;
      for (int i = 0; i < q_samples % rows_per_thread; ++i)
        calculate_row(q_sample + i);
    } else {
      // This wierd execution order is to improve the progress estimation
      for (int i = 0; i < rows_per_thread; ++i)
        calculate_row(task_index + i * q_samples / rows_per_thread);
    }
  }

  void print_progress() {
    if (chrono.seconds() - last_report_time > 2.) {
      last_report_time = chrono.seconds();

      int total_work = probability_table.size();
      int work_done = completed_cells;
      float progress = float(work_done) / float(total_work);

      int expected_time = chrono.seconds() / progress;
      int time = int(chrono.seconds());

      if (time < expected_time) {
        cout << "Progress: " << int(100 * progress) << "%   ";
        cout << time << "s / ~" << expected_time << "s" << endl;
      }
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

  string probability_file_path =
      "./dades/percolat/" + graph_type + "_" + percolation_type + ".csv";
  string transition_file_path = "./dades/percolat/" + graph_type + "_" +
                                percolation_type + "_transition.csv";

  if (filesystem::exists(probability_file_path) ||
      filesystem::exists(transition_file_path)) {
    if (!confirm_action("Aquesta operacio sobrescriura:\n  " +
                        probability_file_path + "\n  " + transition_file_path +
                        "\nVols continuar?")) {
      cout << "Operacio cancelada" << endl;
      return;
    }
  }

  int q_samples =
      read_value<int>("Quantitat de valors de q a analitzar (101)", 101);
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

  /// Output: Write probability to file ///
  vector<string> probability_columns = {"q"};
  for (int i = 0; i < graphs.size(); ++i) {
    string nodes = to_string(graphs[i].number_of_nodes());
    probability_columns.push_back("p" + to_string(i) + "_n" + nodes);
  }
  probability_columns.push_back("p_mean");
  TableFile probability_file(probability_file_path, probability_columns);

  for (int q_sample_index = 0; q_sample_index < q_samples; ++q_sample_index) {
    float q = float(q_sample_index) / float(q_samples - 1);
    probability_file << q;

    float p_sum = 0.;

    for (int graph_index = 0; graph_index < graphs.size(); ++graph_index) {
      float p = analisis.probability_cell(q_sample_index, graph_index);
      probability_file << p;
      p_sum += p;
    }

    probability_file << (p_sum / graphs.size());
  }

  //// Output: Write Percolation transition phase ////
  TableFile transition_file(transition_file_path,
                            {"graph", "nodes", "start q", "mean q", "end q"});
  float min_start_q = 1.;
  float max_end_q = 0.;

  for (int graph_index = 0; graph_index < graphs.size(); ++graph_index) {
    float start_q = 0.;
    float end_q = 1.;

    for (int q_sample_index = 0; q_sample_index < q_samples; ++q_sample_index) {
      float q = float(q_sample_index) / float(q_samples - 1);
      float p = analisis.probability_cell(q_sample_index, graph_index);
      if (p == 0) {
        start_q = q;
      } else if (p == 1) {
        end_q = q;
        break;
      }
    }

    min_start_q = min(min_start_q, start_q);
    max_end_q = max(max_end_q, end_q);
    int nodes = graphs[graph_index].number_of_nodes();
    transition_file << graph_index << nodes;

    transition_file << start_q << (start_q + end_q) / 2 << end_q;
  }

  transition_file << "All" << "-" << min_start_q
                  << (min_start_q + max_end_q) / 2 << max_end_q;
}
