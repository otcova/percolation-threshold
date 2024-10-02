import networkx as nx
import matplotlib.pyplot as plt
import os

if not os.path.exists("./dades"):
    os.makedirs("./dades")

def generar_grafo(tipus, num_vertices, prob_radio, i):
    if tipus == "r":  #random
        if not os.path.exists("./dades/random"):
            os.makedirs("./dades/random")
        G = nx.erdos_renyi_graph(num_vertices, prob_radio)
        nx.write_edgelist(G, "./dades/random/random_graph"+ str(i)+".edgelist", data=False)
    if tipus == "rg":  #random geometric
        if not os.path.exists("./dades/geometric"):
            os.makedirs("./dades/geometric")
        G = nx.random_geometric_graph(num_vertices, prob_radio)
        nx.write_edgelist(G, "./dades/geometric/random_graph"+ str(i)+".edgelist", data=False)
    
    #nx.draw(G, with_labels=True, node_color="skyblue", node_size=700, font_size=10, font_color="black", font_weight="bold")
    #plt.show()
    
	    
def main():
    
    user_input = input("Introduce el tipo de generación(r, rg) y los parámetros(num_V, p/r)[0, 1]:")
        
    try:
            tipus, num_vertices, prob_radio = user_input.split()    
            num_vertices = int(num_vertices)
            prob_radio = float(prob_radio)    
    except ValueError:
            print("Entrada no válida. Usa el formato: r/rg num_vertices probabilidad/radio")
                   
    for i in range(0, 10):    
        generar_grafo(tipus, num_vertices, prob_radio, i)
            
if __name__ == "__main__":
    main()

