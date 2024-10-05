#include <algorithm>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <vector>

using namespace std;

inline string to_uppercase(string text) {
  transform(text.begin(), text.end(), text.begin(), ::toupper);
  return text;
}
inline string to_lowercase(string text) {
  transform(text.begin(), text.end(), text.begin(), ::tolower);
  return text;
}

template <typename T> T read_value(const string &prompt, optional<T> default_value = {}) {
  // Ignore all text written before the prompt
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  // Display prompt
  cout << prompt << ": ";

  // return default If inputed a blank new line
  if (default_value.has_value()) {
    if (cin.peek() == '\n')
      return *default_value;
  }

  // Read input
  T value;
  if (cin >> value)
    return value; // Return value if no errors

  // Exit if the input pipe closed
  if (cin.eof())
    exit(-1);

  // invalid input, retry
  return read_value<T>(prompt, default_value);
}

inline string choose_option(const string &prompt, const vector<string> &options) {
  string complete_prompt = prompt + " (" + to_uppercase(options[0]);
  for (int i = 1; i < options.size(); ++i)
    complete_prompt += "/" + to_lowercase(options[i]);
  complete_prompt += ")";

  string input = to_lowercase(read_value<string>(complete_prompt, options[0]));

  // Complete word match
  for (const string &option : options) {
    if (to_lowercase(option) == input)
      return option;
  }

  // First letter match
  for (const string &option : options) {
    if (tolower(option[0]) == input[0])
      return option;
  }

  // invalid input, retry
  return choose_option(prompt, options);
}

inline bool confirm_action(const string &prompt) {
  return choose_option(prompt, {"yes", "no"}) == "yes";
}
