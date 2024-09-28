import networkx as nx
import matplotlib.pyplot as plt
import matplotlib

def generar_grafo():
    num_vertices = 50
    probabilidad = 0.2  # no cambiar

    G = nx.erdos_renyi_graph(num_vertices, probabilidad)
    nx.draw(G, with_labels=True, node_color="skyblue", node_size=700, font_size=10, font_color="black", font_weight="bold")
    plt.title("Grafo Aleatorio - Modelo Erdős-Rényi")
    #plt.show()
    # modificar per conjunt
    nx.write_edgelist(G, "./dades/random_graph.edgelist", data=False)
	    
def main():
    #while True:
    generar_grafo()
        # user_input = input("Presionar la tecla 'a' para generar un grafo nuevo")
        # if user_input.lower() != 'a':
        #     break

if __name__ == "__main__":
    main()

