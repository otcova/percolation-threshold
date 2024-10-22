import networkx as nx
import matplotlib.pyplot as plt
import math
import os
import re

num_file = 0

def generar_grafo(num_vertices, radio):
    global num_file

    for _ in range(100):
        G = nx.random_geometric_graph(num_vertices, radio)
        if nx.is_connected(G):
            nx.write_edgelist(G, "./dades/geometric/original/graph"+ str(num_file) + ".edgelist", data=False)
            num_file += 1
            return
    print("error", num_vertices, radio)
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
    
    # num_vertices = read_value(int, "Numero vertices: ")

    # print("(", float(math.sqrt(math.log(num_vertices) / (math.pi * num_vertices))), ")")

    # radio = read_value(float, "Radio: ")
    # n_gen = read_value(int, "Introduce el nombre a generar: ")

    if not os.path.exists("./dades"):
        os.makedirs("./dades")
    
    if not os.path.exists("./dades/geometric/original"):
        os.makedirs("./dades/geometric/original")
    ini = encontrar_max_numero("./dades/geometric/original")         
    # radio_critico = calcular_radio_critico(num_vertices)
    # if radio < radio_critico:
    #     radio = radio_critico
    #     print("[WARN] Cambiando radio", radio, "por", radio_critico)
   # for i in range(ini, ini+n_gen):
    #     generar_grafo(num_vertices, radio, i)
    for nv in [500, 1000]:
        rc = [0.07]   
        print(nv)
        for r in rc:
            generar_grafo(nv, r)

if __name__ == "__main__":
    main()

