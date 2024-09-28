import networkx as nx
import matplotlib.pyplot as plt
import matplotlib

def generar_grafo(tipus, num_vertices, prob_radio):
    if tipus == "r":  #random
        G = nx.erdos_renyi_graph(num_vertices, prob_radio)
    if tipus == "rg":  #random geometric
        G = nx.random_geometric_graph(num_vertices, prob_radio)
    
    nx.draw(G, with_labels=True, node_color="skyblue", node_size=700, font_size=10, font_color="black", font_weight="bold")
    #plt.show()
    nx.write_edgelist(G, "./dades/random_graph.edgelist", data=False)
	    
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
            
        generar_grafo(tipus, num_vertices, prob_radio)
        break
            
if __name__ == "__main__":
    main()

