#pragma once

#include <chrono>
using namespace std;

class Chrono {
  chrono::steady_clock::time_point start_timepoint;

public:
  Chrono() : start_timepoint(chrono::steady_clock::now()) {}

  inline float seconds() {
    using namespace chrono;
    duration<float> elapsed(steady_clock::now() - start_timepoint);
    return elapsed.count();
  }
};
