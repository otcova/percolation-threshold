#include "csv.h"
#include <filesystem>
using namespace std;

TableFile::TableFile(const filesystem::path &file_path, const vector<string> &titles) {
  filesystem::create_directory(file_path.parent_path());
  file.open(file_path);
  columns = titles.size();
  columns_left = titles.size();

  for (string title : titles)
    *this << title;
}
