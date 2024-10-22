import networkx as nx
import math
import os
import re

num_file = 0

def generar_grafo(num_vertices, radio):
    global num_file

    for _ in range(10000):
        G = nx.random_geometric_graph(num_vertices, radio)
        if nx.is_connected(G):
            nx.write_edgelist(G, "./dades/geometric/original/graph"+ str(num_file) + ".edgelist", data=False)
            num_file += 1
            return
    print()
    print("Incapaç de generar graf connex amb r =", radio, " n =", num_vertices)
    return

def calcular_radio_critico(num_vertices):
    # Fórmula del radio crítico para garantizar conexidad
    return math.sqrt(math.log(num_vertices) / (math.pi * num_vertices))
   
def encontrar_max_numero(directorio):
    patron = re.compile(r'graph(\d+)\.edgelist')
    max_num = -1
    
    for archivo in os.listdir(directorio):
        coincidencia = patron.match(archivo)
        if coincidencia:
            numero = int(coincidencia.group(1))
            if numero > max_num:
                max_num = numero
    
    if max_num != -1:
        return max_num + 1
    else:
        return 0

def read_value(value_type, prompt):
    while (True):
        try:
            return value_type(input(prompt))
        except ValueError:
            print("Entrada no válida")

def main():
    if not os.path.exists("./dades"):
        os.makedirs("./dades")
    
    if not os.path.exists("./dades/geometric/original"):
        os.makedirs("./dades/geometric/original")
    ini = encontrar_max_numero("./dades/geometric/original")         

    radi = read_value(float, "Parametra Radi: ")

    print("Generant grafs amb nodes: ", end='', flush=True)
    for nv in [5, 10, 20, 30, 40, 50, 75, 100, 200, 300, 400, 500, 750, 1000]:
        print(nv, end=' ', flush=True)
        generar_grafo(nv, radi)

if __name__ == "__main__":
    main()

