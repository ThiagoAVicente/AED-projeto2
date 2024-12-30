import random
import sys
import subprocess
import matplotlib.pyplot as plt
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
    save_graph(graph, filename, NVertices, NEdges)

def generate_best_graph(numVertices, filename):
    """Cria o grafo aleatório."""
    maxVertices=100
    is_digraph = 0
    NVertices = numVertices if numVertices!=-1 else random.randint(0, maxVertices)
    NEdges = 0
    graph = {
        "is_digraph" : is_digraph,
        "is_weighted" : 0,
        "numVertices" : NVertices,
        "numEdges" : NEdges,
        "edges" : [ [] for _ in range(NVertices)]
    }
    save_graph(graph, filename, NVertices, NEdges)
    return 4*numVertices+3

def generate_worst_graph(numVertices, filename):
    """Cria o grafo aleatório."""
    maxVertices=100
    is_digraph = 1
    NVertices = numVertices if numVertices!=-1 else random.randint(0, maxVertices)
    NEdges = 0
    graph = {
        "is_digraph" : is_digraph,
        "is_weighted" : 0,
        "numVertices" : NVertices,
        "numEdges" : NEdges,
        "edges" : [ [] for _ in range(NVertices)]
    }
    graph["edges"][0] = [1, 2]
    graph["edges"][1] = [0, 2, 3]
    NEdges += 5
    for u in range(2, numVertices-1):
        graph["edges"][u].append(u+1)
        NEdges+=1
        for v in range(u):
            graph["edges"][u].append(v)
            NEdges+=1
    graph["edges"][-1] = [i for i in range(numVertices-1)]
    NEdges+=NVertices-1
    graph["numEdges"] = NEdges
    save_graph(graph, filename, NVertices, NEdges)
    return 0.5*(3*numVertices**2+11*numVertices)+1

def save_graph(graph, filename, NVertices, NEdges):
    """Gera n grafos e os salva em arquivos .txt"""
    with open(filename, 'w') as file:
        file.write(f"{graph["is_digraph"]}\n{0}\n{NVertices}\n{NEdges}\n")
        for u in range(NVertices):
            for v in graph["edges"][u]:
                file.write(f"{u} {v}\n")  # Aumentando 1 para tornar os índices 1-based
    return graph


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
            print(result)
            exit(0)

    except FileNotFoundError:
        print("The program 'testescomputacionais' was not found. Make sure it is compiled and in the same directory.")
        exit(1)


def call_cases(input, output, bestAcess, bestAts, bestTotals, bestTime):
    output=outdir + "/" + output
    try:
        args = [
            "./bellmantestcases",  # Caminho para o programa C
            input,       # Argumento 2: directory (string)
            output,     # Argumento 3: output file (string)
            " && "        # Argumento 4: delimiter (char)
        ]

        # Executar o programa com os argumentos fornecidos
        result = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        if "=-6" in str(result):
            print(result)
            exit(0)

    except FileNotFoundError:
        print("led and in the same directory.")
        exit(1)
    
    with open(output, "r") as file:
        for line in file:
            l=line.split(" && ")
            if not l[0].strip().isdigit():
                continue
            acs = int(l[0]) +int(l[1]) + int(l[3])
            bestAcess.append(acs)
            at = int(l[2])
            bestAts.append(at)
            bestTotals.append(at+acs)
            bestTime.append(float(l[4]))


def getData(output, acess, ats, totals, teos, num, it):
    with open(output, "r") as file:
        for line in file:
            l=line.split(" && ")
            if not l[0].strip().isdigit():
                continue
            num.append(it)
            acs = int(l[0]) +int(l[1]) + int(l[3])
            acess.append(acs)
            at = int(l[2])
            ats.append(at)
            totals.append(at+acs)
            teos.append(int(l[4]))
            



def main():

    if len(sys.argv) < 2:
        print(f"[USAGE] {sys.argv[0]} <num-graphs>")
        exit(0)

    n = int(sys.argv[1])
    acess = []
    ats = []
    totals = []
    teos = []
    num = []
    bestTeo = []
    bestAcess=[] 
    bestAts=[]
    bestTotals=[]
    bestTime=[]
    worstTeo = []
    worstAcess=[] 
    worstAts=[]
    worstTotals=[]
    worstTime=[]
    # bellman test
    for i in range(4, n):
        for j in range(3):
            graph_file = "grafos/graph{:d}_{:03d}.txt".format(j+1, i)
            generate_random_graph(-1, i, graph_file)
            output="bellman_average_{:d}_{:03d}.txt".format(j+1, i)
            call_func(graph_file, output)
            getData("out/" + output, acess, ats, totals, teos, num, i)
        graph_file = "grafos/best_{:03d}.txt".format(i)
        teo=generate_best_graph(i, graph_file)
        bestTeo.append(teo)
        output="bellman_best_{:03d}.txt".format(i)
        call_cases(graph_file, output, bestAcess, bestAts, bestTotals, bestTime)

        graph_file = "grafos/worst_{:03d}.txt".format(i)
        teo=generate_worst_graph(i, graph_file)
        worstTeo.append(teo)
        output="bellman_worst_{:03d}.txt".format(i)
        call_cases(graph_file, output, worstAcess, worstAts, worstTotals, worstTime)
    mean=[]
    startid=0

    for i in range(4, n):
        size=i*3
        mean.append(sum(totals[startid: startid+size])/size)
        startid+=size
    x=[i for i in range(4, n)]
    plt.plot(x, bestTeo, label="Melhor caso Valor Teórico")
    plt.plot(x, worstTeo, label="Pior caso Valor Teórico")
    plt.plot(x, bestTotals, label="Melhor caso Valor Prático")
    plt.plot(x, worstTotals, label="Pior caso Valor Prático")
    plt.plot(x, mean, label="Caso Médio Valor Prático")
    plt.scatter(num, totals, label="Valor prático de grafos aleatórios")
    plt.scatter(num, teos, label="Valor teórico de grafos aleatórios")
    plt.legend()
    plt.title("Análise computacional do número de acessos e atribuições")
    plt.xlabel("Número de vértices")
    plt.ylabel("Número de acesso e atribuições")
    plt.show()

    plt.plot(x, bestTime, label="Melhor caso")
    plt.plot(x, worstTime, label="Pior caso")
    plt.legend()
    plt.title("Tempo de execução para vários grafos")
    plt.xlabel("Número de vértices")
    plt.ylabel("Tempo de execução (s)")
    plt.show()

if __name__ == "__main__":
    main()
