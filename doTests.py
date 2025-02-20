import networkx as nx
import random
import sys
import subprocess
import os
dir = "grafos"
outdir = "out"
NUMTESTS = 500
MINVERTICES = 4

def genereate_grafo_do_ze(n):
    assert n >= 4

    # criar grafo sem arrestas
    G = nx.erdos_renyi_graph(n, 0, directed=True)

    # adicionar arrestas inicias
    G.add_edge(0,1)
    G.add_edge(0,2)
    used = set()
    for i in range(2,n):
        list = [j for j in range(0,i+2)]

        for v in range(i-1,i+1):
            if v in used:continue
            used.add(v)
            for item in list:
                if item != v and item<n:
                    G.add_edge(v,item)


    return G

def generate_random_graph(n, case, is_directed=False):
    """Gera um grafo aleatório com n vértices e arestas direcionais ou não,
    garantindo que o último vértice não tenha caminho para o vértice inicial."""
    # Grafo Erdős-Rényi com probabilidade de arestas
    if case == 0:
        p = random.uniform(0, 1)  # Probabilidade de aresta variada
    elif case == 1:
        p = 0  # Sem conexões (grafo vazio)
    elif case == -1:
        p = 1  # Totalmente conectado
    else:
        raise ValueError("Invalid case. Use 0 (varied), 1 (no connections), or -1 (fully connected).")

    # Criar o grafo
    if is_directed:
        G = nx.erdos_renyi_graph(n, p, directed=True)
    else:
        G = nx.erdos_renyi_graph(n, p)


    # Garantir que o último vértice (n-1) não tenha caminho para o vértice inicial (0)
    inds = [i for i in range(n)]
    if is_directed:
        for i in range(0,n):
                v = inds[i-1]
                w = inds[i]
                if G.has_edge(w, v):
                            G.remove_edge( w,v)


    return G

def clear_directory(directory_path):
    # Loop through all items in the directory
    for filename in os.listdir(directory_path):
        file_path = os.path.join(directory_path, filename)

        # Check if it's a file or directory and remove it accordingly
        if not os.path.isdir(file_path):

            os.remove(file_path)  # Remove file



def save_graph_to_txt(graph, filename):
    """Salva o grafo em um arquivo no formato especificado."""
    is_directed = 1 if graph.is_directed() else 0
    num_vertices = len(graph.nodes)
    num_edges = len(graph.edges)

    with open(filename, 'w') as file:
        file.write(f"{is_directed}\n{0}\n")
        file.write(f"{num_vertices}\n")
        file.write(f"{num_edges}\n")

        for edge in graph.edges():
            u, v = edge
            file.write(f"{u} {v}\n")  # Aumentando 1 para tornar os índices 1-based

        # Escrever os nós isolados (sem arestas)
        isolated_nodes = set(range(0, num_vertices)) - set([node for edge in graph.edges() for node in edge])
        for node in sorted(isolated_nodes):
            file.write(f"{node}\n")

def generate_and_save_graphs(n, num_graphs,is_directed,case , filename_template=dir+"/graph_{}.txt"):
    """Gera n grafos e os salva em arquivos .txt"""
    for i in range(1, num_graphs + 1):
        if is_directed == -1:
            graph = generate_random_graph(n,case, random.choice([True,False]))
        else:
            graph = generate_random_graph(n,case, is_directed)
        filename = filename_template.format(i)
        save_graph_to_txt(graph, filename)

def call_func(op,name):
    try:
        args = [
            "./testescomputacionais",  # Caminho para o programa C
            str(op),  # Argumento 1: operation (int)
            dir,       # Argumento 2: directory (string)
            outdir + "/" +name,     # Argumento 3: output file (string)
            ";"        # Argumento 4: delimiter (char)
        ]

        # Executar o programa com os argumentos fornecidos
        result = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

        #print(result)

    except FileNotFoundError:
        print("The program 'testescomputacionais' was not found. Make sure it is compiled and in the same directory.")
        exit(1)


def main():

    if len(sys.argv) < 3:
        print(f"[USAGE] {sys.argv[0]} <mode> <num-graphs>")
        print("mode = 1 means bellmanford test and mode = 0 means transitive clossure")
        exit(0)

    op = int(sys.argv[1])
    n = int(sys.argv[2])

    if op == 1:
        # bellman test
        for case in [-1]:
            for i in range(MINVERTICES,NUMTESTS+MINVERTICES):
                generate_and_save_graphs(i,n,1,case)
                #G = genereate_grafo_do_ze(i)
                #save_graph_to_txt(G,dir+"/grafo_do_ze.txt")
                call_func(1,f"bellman_{["worst","average","best"][case+1]}_{i}")

    else:
        # transitive closure test
        for case in [-1,0,1]:
            file = f"{["worst","average","best"][case+1]}"
            clear_directory(dir)
            for i in range(MINVERTICES,NUMTESTS+MINVERTICES):
                a = n
                if abs(case) == 1: a = 1
                generate_and_save_graphs(i,a,1,case)
                #G = genereate_grafo_do_ze(i)
                #save_graph_to_txt(G,dir+"/grafo_do_ze.txt")
                #call_func(0,f"tClosure_{["worst","average","best"][case+1]}_{i}")
                call_func(0,file)
                with open(outdir+"/"+file,"at")as fout:
                    fout.write("\n")



if __name__ == "__main__":
    main()
