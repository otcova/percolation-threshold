import networkx as nx
import matplotlib.pyplot as plt
import math
import os
import re

def generar_grafo(tipus, num_vertices, prob_radio, i):
    if tipus == "r":  #random
        if not os.path.exists("./dades/random/original"):
            os.makedirs("./dades/random/original")
        G = nx.erdos_renyi_graph(num_vertices, prob_radio)
        nx.write_edgelist(G, "./dades/random/original/graph"+ str(i)+".edgelist", data=False)
    if tipus == "rg":  #random geometric
        
        while True:  
            G = nx.random_geometric_graph(num_vertices, prob_radio)
            if nx.is_connected(G):  
                break  
        nx.write_edgelist(G, "./dades/geometric/original/graph"+ str(i)+".edgelist", data=False)
    
    #nx.draw(G, with_labels=True, node_color="skyblue", node_size=700, font_size=10, font_color="black", font_weight="bold")
    #plt.show()
   
def calcular_radio_critico(num_vertices):
    # Fórmula del radio crítico para garantizar conexidad
    return math.sqrt(math.log(num_vertices) / (math.pi * num_vertices))
   
def encontrar_max_numero(directorio, tipus):
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
        if tipus == "rg":
            return 0
        else:
            return 1000
	    
def main():
    
    while (True):
        try:
            user_input = input("Introduce el tipo de generación(r, rg) y los parámetros(num_V, p/r[0..1]): ")
            tipus, num_vertices, prob_radio = user_input.split()    
            num_vertices = int(num_vertices)
            prob_radio = float(prob_radio)
            break    
        except ValueError:
            print("Entrada no válida. Usa el formato: r/rg num_vertices probabilidad/radio")
    
    while (True):
        try:
            n_gen = input("Introduce el nombre a generar: ")
            n_gen = int(n_gen)
            break
        except ValueError:
            print("Entrada no válida. Formato entero")

    if not os.path.exists("./dades"):
        os.makedirs("./dades")
    
    if tipus == "rg":
        if not os.path.exists("./dades/geometric/original"):
            os.makedirs("./dades/geometric/original")
        ini = encontrar_max_numero("./dades/geometric/original", tipus)         
        radio_critico = calcular_radio_critico(num_vertices)
        if prob_radio < radio_critico:
            prob_radio = radio_critico
        for i in range(ini, ini+n_gen):    
            generar_grafo(tipus, num_vertices, prob_radio, i)
    else:
        if not os.path.exists("./dades/random/original"):
            os.makedirs("./dades/random/original")
        ini = encontrar_max_numero("./dades/random/original", tipus)  
        for i in range(ini, ini+n_gen):    
            generar_grafo(tipus, num_vertices, prob_radio, i)
            
if __name__ == "__main__":
    main()

