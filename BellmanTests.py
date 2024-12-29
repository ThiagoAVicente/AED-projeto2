import random
import sys
import subprocess
dir = "grafos"
outdir = "out"
NUMTESTS = 5

def generate_random_graph(is_digraph, numVertices, filename):
    """Cria o grafo aleatório."""
    maxVertices=100
    is_digraph = is_digraph if is_digraph!=-1 else random.randint(0, 1)
    NVertices = numVertices if numVertices!=-1 else random.randint(0, maxVertices)
    maxEdges = NVertices*(NVertices-1) if is_digraph else int(NVertices*(NVertices-1)/2)
    NEdges = random.randint(0, maxEdges)
    graph = {
        "is_digraph" : is_digraph,
        "is_weighted" : 0,
        "numVertices" : NVertices,
        "numEdges" : NEdges,
        "edges" : [ [] for _ in range(NVertices)]
    }
    for _ in range(NEdges):
        while True:
            u = random.randint(0, NVertices-1)
            v=random.randint(0, NVertices-1)
            if not graph["is_digraph"]:
                if u in graph["edges"][v] or v in graph["edges"][u]:
                    continue
            if v!=u and v not in graph["edges"][u]:
                break
        graph["edges"][u].append(v)

    with open(filename, 'w') as file:
        file.write(f"{graph["is_digraph"]}\n{0}\n{NVertices}\n{NEdges}\n")
        for u in range(NVertices):
            for v in graph["edges"][u]:
                file.write(f"{u} {v}\n")  # Aumentando 1 para tornar os índices 1-based
    return graph

def generate_and_save_graphs(n, num_graphs,is_directed,case , filename_template=dir+"/graph_{}.txt"):
    """Gera n grafos e os salva em arquivos .txt"""
    for i in range(1, num_graphs + 1):
        if is_directed == -1:
            graph = generate_random_graph(n,case, random.choice([True,False]))
        else:
            graph = generate_random_graph(n,case, is_directed)
        filename = filename_template.format(i)

def call_func(input, output):
    try:
        args = [
            "./bellmantests",  # Caminho para o programa C
            input,       # Argumento 2: directory (string)
            outdir + "/" + output,     # Argumento 3: output file (string)
            " && "        # Argumento 4: delimiter (char)
        ]

        # Executar o programa com os argumentos fornecidos
        result = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        if "=-6" in str(result):
            exit(0)
        print(result)

    except FileNotFoundError:
        print("The program 'testescomputacionais' was not found. Make sure it is compiled and in the same directory.")
        exit(1)


def main():

    if len(sys.argv) < 2:
        print(f"[USAGE] {sys.argv[0]} <num-graphs>")
        exit(0)

    n = int(sys.argv[1])

    # bellman test
    for i in range(3, n):
        for j in range(3):
            graph_file = "grafos/graph{:d}_{:03d}.txt".format(j+1, i)
            generate_random_graph(-1, i, graph_file)
            call_func(graph_file, "bellman_average_{:d}_{:03d}.txt".format(j+1, i))
if __name__ == "__main__":
    main()
