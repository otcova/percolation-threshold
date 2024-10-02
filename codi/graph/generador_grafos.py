import networkx as nx
import matplotlib.pyplot as plt
import os
import re

if not os.path.exists("./dades"):
    os.makedirs("./dades")

def generar_grafo(tipus, num_vertices, prob_radio, i):
    if tipus == "r":  #random
        if not os.path.exists("./dades/random"):
            os.makedirs("./dades/random")
        G = nx.erdos_renyi_graph(num_vertices, prob_radio)
        nx.write_edgelist(G, "./dades/random/graph"+ str(i)+".edgelist", data=False)
    if tipus == "rg":  #random geometric
        if not os.path.exists("./dades/geometric"):
            os.makedirs("./dades/geometric")
        G = nx.random_geometric_graph(num_vertices, prob_radio)
        nx.write_edgelist(G, "./dades/geometric/graph"+ str(i)+".edgelist", data=False)
    
    #nx.draw(G, with_labels=True, node_color="skyblue", node_size=700, font_size=10, font_color="black", font_weight="bold")
    #plt.show()
   
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
    
    user_input = input("Introduce el tipo de generación(r, rg) y los parámetros(num_V, p/r)[0, 1]:")
        
    try:
            tipus, num_vertices, prob_radio = user_input.split()    
            num_vertices = int(num_vertices)
            prob_radio = float(prob_radio)    
    except ValueError:
            print("Entrada no válida. Usa el formato: r/rg num_vertices probabilidad/radio")
    if tipus == "rg":
        ini = encontrar_max_numero("./dades/geometric", tipus)         
        for i in range(ini, ini+10):    
            generar_grafo(tipus, num_vertices, prob_radio, i)
    else:
        ini = encontrar_max_numero("./dades/random", tipus)  
        for i in range(ini, ini+1010):    
            generar_grafo(tipus, num_vertices, prob_radio, i)
            
if __name__ == "__main__":
    main()

