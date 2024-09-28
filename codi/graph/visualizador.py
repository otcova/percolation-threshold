import networkx as nx
import matplotlib.pyplot as plt

	    
def main():
    while True:
        user_input = input("Introduce el path de la lista de adyacencias a representar:\n")
        
        try:
                fh = open(user_input, "rb")
                G = nx.read_edgelist(fh, nodetype=int)
                fh.close()
                nx.draw(G, with_labels=True, node_color="skyblue", node_size=700, font_size=10, font_color="black", font_weight="bold")
                plt.show()
        except ValueError:
                print("Entrada no válida. Usa el formato: r/rg num_vertices probabilidad/radio")
                continue        
            
        break
            
if __name__ == "__main__":
    main()
