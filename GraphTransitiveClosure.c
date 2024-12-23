//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphTransitiveClosure - Transitive Closure of a directed graph
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

// [TODO] MAKE THIS SHIT MORE EFFICIENT
/*** COMPLETE THE GraphComputeTransitiveClosure FUNCTION ***/

#include "GraphTransitiveClosure.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "instrumentation.h"

// Compute the transitive closure of a directed graph
// Return the computed transitive closure as a directed graph
// Use the Bellman-Ford algorithm
Graph* GraphComputeTransitiveClosure(Graph* g) {
    assert(g != NULL);
    assert(GraphIsDigraph(g));
    assert(GraphIsWeighted(g) == 0);

    // COMPLETE THE CODE
    unsigned int numVertices = GraphGetNumVertices(g);
    assert(numVertices > 0);

    // get bellman ford struct
    // [TODO] want to use bf just one time

    // create response graph
    Graph* response = GraphCreate(numVertices, 1, 0);

    // iterate over each vertex
    for ( unsigned int vertex1 = 0 ; vertex1 < numVertices;++vertex1  ){

        // compute the bellman fort paths
        GraphBellmanFordAlg* bf = GraphBellmanFordAlgExecute(g, vertex1);

        // iterate over each vertex starting on vertex1+1 for better efficiency
        for ( unsigned int vertex2 = vertex1+1; vertex2 < numVertices; ++vertex2){

            // if there is a connection create an edje between the vertices
            if ( GraphBellmanFordAlgReached(bf, vertex1)){
                GraphAddEdge(response, vertex1, vertex2);
            }
        }
        GraphBellmanFordAlgDestroy(&bf);
    }

    //  return
    return response;
}
