// script de testes computacionais para GraphTransitiveClosure e GraphBellmanFord
// [COMPILE] gcc testescomputacionais.c Graph.c GraphTransitiveClosure.c IntegersStack.c instrumentation.c GraphBellmanFordAlg.c SortedList.c -o testescomputacionais

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
#define MARK_ACCESS InstrCount[0]
#define MARK_ASSIGNMENT InstrCount[1]
#define PREDECESSOR_ASSIGNMENT InstrCount[2]
#define DISTANCE_ACCESS InstrCount[3]
#define DISTANCE_ASSIGNMENT InstrCount[4]

static Graph* _fileToGraph(const char* fileName){
    // file must have:
    // is digraph? 1:0
    // is Weighted? 1:0
    // num verticies
    // num edjes
    // edjes ...

    // open file
    FILE* file = fopen(fileName, "r");
    if ( !file ){
        fprintf(stderr, "[ERROR] opening file\n");
        exit(1);
    }

    // read info to create graph
    int info[4];
    for ( int i = 0; i < 4; ++i){
        fscanf(file, "%d", &info[i]);
    }

    // create graph
    Graph* response = GraphCreate(info[2], info[0], info[1]);

    // read each edje
    int v1, v2;
    for ( int i = 0; i < info[3]; ++i ){

        fscanf(file, "%d %d", &v1,&v2);
        GraphAddEdge(response, v1, v2);
    }

    // close file
    fclose(file);

    return response;
}

static void _saveResults(const char* outputFile, const char* results ){
    FILE* file = fopen(outputFile, "a");
    if (file) {
        fprintf(file, "%s", results);
        fclose(file);
    } else {
        fprintf(stderr, "[ERROR] opening output file.\n");
    }
}

static void _testFunction(char* dirName, char* outputFile, int op, char* delimiter){
    // op = 1? bellman: transitive closure
    DIR* dir = opendir(dirName);
    if (!dir) {
        fprintf(stderr, "Error opening the directory: %s\n", dirName);
        return;
    }
    // iterate over the directory
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip '.' and '..' entries
        if (entry->d_name[0] == '.') {
            continue;
        }

        InstrReset();

        // Form the full path for the graph file
        char graphFile[256];
        snprintf(graphFile, sizeof(graphFile), "%s/%s", dirName, entry->d_name);

        // Load the graph
        Graph* g = _fileToGraph(graphFile);

        // save a label with the info about the graph
        char label[512];
        snprintf(label, sizeof(label), "graph-> vertices: %d  edjes: %d\nMARK_ACCESS%sMARK_ASSIGNMENT\
            %sPREDECESSOR_ASSIGNMENT%sDISTANCE_ACCESS%sDISTANCE_ASSIGNMENT\
            ",GraphGetNumVertices(g),GraphGetNumEdges(g),delimiter,delimiter,delimiter,delimiter);
        _saveResults(outputFile, label);

        char result[128];
        if (op == 1) {
            // Run Bellman-Ford Algorithm
            printf("Running Bellman-Ford on graph: %s\n", graphFile);

            for ( unsigned int vStart = 0 ; vStart < GraphGetNumVertices(g); ++vStart){

                InstrReset();

                // Save Bellman-Ford results
                GraphBellmanFordAlgExecute(g, vStart);
                snprintf(result, sizeof(result), "%lu%s%lu%s%lu%s%lu%s%lu\n",
                            MARK_ACCESS,delimiter, MARK_ASSIGNMENT,delimiter, PREDECESSOR_ASSIGNMENT,delimiter,DISTANCE_ACCESS,delimiter, DISTANCE_ASSIGNMENT);
                _saveResults(outputFile, result);
            }
            _saveResults(outputFile, "\n");
        } else {
            // Run Transitive Closure Algorithm
            printf("Running Transitive Closure on graph: %s\n", graphFile);

            // Save Bellman-Ford results
            GraphComputeTransitiveClosure(g);
            snprintf(result, sizeof(result), "%lu%s%lu%s%lu%s%lu%s%lu\n",
                        MARK_ACCESS,delimiter, MARK_ASSIGNMENT,delimiter, PREDECESSOR_ASSIGNMENT,delimiter,DISTANCE_ACCESS,delimiter, DISTANCE_ASSIGNMENT);
            _saveResults(outputFile, result);
            _saveResults(outputFile, "\n");
        }

    }

    // reset value


}

int main(int argc, char** argv){

    if (argc != 4 && argc != 5) {
        fprintf(stderr, "Usage: %s <operation (int)> <directory (string)> <output file (string)> <delimiter <char>>\n", argv[0]);
        return 1; // Exit with error if arguments are not correct
    }

    int op = atoi(argv[1]);  // Convert the first argument to an integer

    // Validate the operation argument (check if it's a valid number)
    if (op != 1 && op != 0) {
        fprintf(stderr, "Error: First argument must be 1 (for Bellman-Ford) or 0 (for Transitive Closure).\n");
        return 1;
    }

    // Get the second argument (directory path for graph files)
    char* dirName = argv[2];

    // Get the third argument (output file path)
    char* outputFile = argv[3];
    char* delimiter = argv[4];

    // Call _testFunction with the parsed arguments
    _testFunction(dirName, outputFile, op, delimiter);

    return 0;
}
