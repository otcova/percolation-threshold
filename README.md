# Fitxers del projecte
El projecte està organitzat en les següents carpetes:
- `codi/` - Tot el codi c++
- `build/` - Carpeta generada pel `Makefile`. Conte els fitxers de compilació (*.exe, *.o, ...)
- `dades/` - Carpeta generada per l'app.



# Makefile
El `Makefile` és automàtic. **No** fa falta modificar-lo quan es fa un `#include`,
o quan es creen/eliminen fitxers.

### Makefile Commands
- `make`: Compila el projecte.
- `make run`: Compila i executa el projecte.
- `make clean`: Esborra la carpeta `build/` (on hi han tots els fitxers resultants de compilació).



# Organització del codi

### `main.cpp`
Codi del menú inicial de l'app.

### `graph/`
Codi relacionat amb la classe `Graph`
- Mètodes de anàlisis: count_connected_components, ...
- Funcions de generació: random geometrics graph, grid graph, ...

### `percolation/`
- Codi de `node_percolation` i `edge_percolation` que crea un graph a partir de un graph i una `q`.
- Codi per a calcular la probabilitat que un graph aleatori i q sigui connex.
- Codi necesari per a calcular/trobar les transicions.

### `export/`
Codi per a exportar dades en fitxers.

