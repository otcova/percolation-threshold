import networkx as nx
import matplotlib.pyplot as plt

import os


directorio = "../ej_grafos"

if not os.path.exists(directorio):
    os.makedirs(directorio)

def generar_grafo(tipus, num_vertices, prob_radio, i):
    if tipus == "r":  #random
        G = nx.erdos_renyi_graph(num_vertices, prob_radio)
    if tipus == "rg":  #random geometric
        G = nx.random_geometric_graph(num_vertices, prob_radio)
    
    nx.draw(G, with_labels=True, node_color="skyblue", node_size=700, font_size=10, font_color="black", font_weight="bold")
    plt.show()
    nx.write_edgelist(G, os.path.join(directorio, f"random_graph.edgelist{i}"), data=False)
	    
def main():
    while True:
        user_input = input("Introduce el tipo de generación(r, rg) y los parámetros(num_V, p/r)[0, 1]:")
        
        try:
                tipus, num_vertices, prob_radio = user_input.split()    
                num_vertices = int(num_vertices)
                prob_radio = float(prob_radio)    
        except ValueError:
                print("Entrada no válida. Usa el formato: r/rg num_vertices probabilidad/radio")
                continue        
        for i in range(1, 1001):    
            generar_grafo(tipus, num_vertices, prob_radio, i)
            
if __name__ == "__main__":
    main()

