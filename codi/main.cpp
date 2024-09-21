#include "export/csv.h"
#include <iostream>
#include <math.h>
using namespace std;

void generar_mostres() {
  // TODO
}

void contar_components_connexes() {
  // TODO
}

void prova_exportar_dades() {
  TableFile file("sine_wave", {"x", "sin(x)"});
  for (float x = 0; x < 30; x += 0.05)
    file << x << sin(x);
}

int main() {
  cout << "-- Menu --" << endl;
  cout << "1. Generar mostres" << endl;
  cout << "2. Contar components connexes" << endl;
  cout << "3. Prova: exportar dades" << endl;
  cout << "> ";

  int option;
  cin >> option;

  if (option == 0)
    generar_mostres();
  else if (option == 1)
    contar_components_connexes();
  else if (option == 2)
    prova_exportar_dades();
}
