#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/**
 * # Example
 *
 * TableFile file("sine_wave.csv", {"x", "sin(x)"});
 * for (float x = 0; x < 30; x += 0.05) {
 *   file << x << sin(x);
 */
class TableFile {
public:
  TableFile(const filesystem::path &file_path, const vector<string> &titles);

  // Add a value to the table
  template <typename T> TableFile &operator<<(T value);

private:
  // The file
  ofstream file;

  // Number of columns
  int columns;

  // Number of columns that needs to be filled with
  // data to complete the row.
  int columns_left;
};

template <typename T> TableFile &TableFile::operator<<(T value) {
  file << value;
  --columns_left;

  if (columns_left > 0) {
    file << ';';
  } else {
    file << endl;
    columns_left = columns;
  }

  return *this;
}
