import networkx as nx
import matplotlib.pyplot as plt

def generar_grafo():
    num_vertices = 50
    probabilidad = 0.2

    G = nx.erdos_renyi_graph(num_vertices, probabilidad)
    nx.draw(G, with_labels=True, node_color="skyblue", node_size=700, font_size=10, font_color="black", font_weight="bold")
    plt.title("Grafo Aleatorio - Modelo Erdős-Rényi")
    plt.show()
    nx.write_edgelist(G, "random_graph.edgelist", data=False)
    adj_matrix = nx.to_numpy_array(G, dtype=int)

    with open("grafos.txt", "a") as f:
        f.write("Matriz de adyacencia del grafo:\n")
        for row in adj_matrix:
            f.write(" ".join(map(str, row)) + "\n")
        f.write("--------------------\n")
        f.write("\n")
        f.write("\n")
	    
def main():
    while True:
        generar_grafo()
        user_input = input("Presionar la tecla 'a' para generar un grafo nuevo")
        if user_input.lower() != 'a':
            break

if __name__ == "__main__":
    main()

