#include "csv.h"
#include <filesystem>
using namespace std;

TableFile::TableFile(const string &file_name, const vector<string> &titles) {
  filesystem::create_directory("./dades");
  file.open("./dades/" + file_name + ".csv");
  columns = titles.size();
  columns_left = titles.size();

  for (string title : titles)
    *this << title;
}
