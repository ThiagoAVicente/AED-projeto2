// script de testes computacionais para GraphTransitiveClosure e GraphBellmanFord
// [COMPILE] gcc bellmantests.c Graph.c GraphTransitiveClosure.c IntegersStack.c instrumentation.c GraphBellmanFordAlg.c SortedList.c -o bellmantests

#include "GraphBellmanFordAlg.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>

#include "Graph.h"
#include "GraphTransitiveClosure.h"
#include "IntegersStack.h"
#include "instrumentation.h"

// COUNTERS
#define MARK_ATRIBUTION InstrCount[1]
#define PREDECESSOR_ATRIBUTION InstrCount[2]
#define DISTANCE_ACCESS InstrCount[3]
#define DISTANCE_ATRIBUTION InstrCount[4]

static void _saveResults(const char* outputFile, const char* results ){
    FILE* file = fopen(outputFile, "a");
    if (file) {
        fprintf(file, "%s", results);
        fclose(file);
    } else {
        fprintf(stderr, "[ERROR] opening output file.\n");
    }
}

static void _testFunction(char* inputFile, char* outputFile, char* delimiter){

    InstrReset();

    FILE* file = fopen(inputFile, "r");
    if ( !file ){
        fprintf(stderr, "[ERROR] opening file\n");
        exit(1);
    }
    // Load the graph
    Graph* g = GraphFromFile(file);
    int V = GraphGetNumVertices(g);

    // save a label with the info about the graph
    char label[512];
    snprintf(label, sizeof(label), "graph-> vertices: %d  edjes: %d directed: %d\nMARK_ATRIBUTION%sPREDECESSOR_ATRIBUTION\
%sDISTANCE_ACCESS%sDISTANCE_ATRIBUTION%sTotal_Teo\n\
",V,GraphGetNumEdges(g),GraphIsDigraph(g),delimiter,delimiter,delimiter, delimiter);
    _saveResults(outputFile, label);

    char result[128];
    for ( unsigned int vStart = 0 ; vStart < V; ++vStart){

        InstrReset();

        // Save Bellman-Ford results
        GraphBellmanFordAlg* gr = GraphBellmanFordAlgExecute(g, vStart);
        int teo = V*3 + 3;
        int Va = 0;
        int I = 0;
        int Ea = 0;
        if (GraphIsDigraph(g)){ Ea+=GraphGetVertexOutDegree(g,vStart);
            }else{Ea+=GraphGetVertexDegree(g,vStart);}
            printf("%d\n", Ea);
        for (int i=0; i<V; i++){
            if (vStart == i) continue;
            if (GraphBellmanFordAlgReached(gr, i)==0) continue;
            Va++;
            int dist = GraphBellmanFordAlgDistance(gr, i);
            if (I<dist){
                I =dist;
            }
            if (dist==V-1){
                I--;
                continue;
            }
            if (GraphIsDigraph(g)){ Ea+=GraphGetVertexOutDegree(g,i);
            }else{Ea+=GraphGetVertexDegree(g,i);}
            printf("%d\n", Ea);
        }
        I++;
        teo += 3*Va+I*V+Ea;
        snprintf(result, sizeof(result), "%lu%s%lu%s%lu%s%lu%s%d\n",
                    MARK_ATRIBUTION,delimiter, PREDECESSOR_ATRIBUTION,delimiter, DISTANCE_ACCESS,delimiter,DISTANCE_ATRIBUTION,delimiter, teo);

        _saveResults(outputFile, result);

        GraphBellmanFordAlgDestroy(&gr);
    }
    _saveResults(outputFile, "\n");


    // reset value


}

int main(int argc, char** argv){

    if (argc != 3 && argc != 4) {
        fprintf(stderr, "Usage: %s <operation (int)> <directory (string)> <output file (string)> <delimiter <char>>\n", argv[0]);
        return 1; // Exit with error if arguments are not correct
    }

    // Get the second argument (directory path for graph files)
    char* dirName = argv[1];

    // Get the third argument (output file path)
    char* outputFile = argv[2];
    char* delimiter = argv[3];
    
    // Call _testFunction with the parsed arguments
    _testFunction(dirName, outputFile, delimiter);

    return 0;
}
