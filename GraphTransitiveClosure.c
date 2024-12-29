//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphTransitiveClosure - Transitive Closure of a directed graph
//

// Student Name : José Bagagem
// Student Number : 120141
// Student Name : Thiago Vicente
// Student Number : 121497

/*** COMPLETE THE GraphComputeTransitiveClosure FUNCTION ***/

#include "GraphTransitiveClosure.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "instrumentation.h"

// COUNTERS
#define MARK_ACCESS InstrCount[0]

// Compute the transitive closure of a directed graph
// Return the computed transitive closure as a directed graph
// Use the Bellman-Ford algorithm
Graph* GraphComputeTransitiveClosure(Graph* g) {
    assert(g != NULL);
    assert(GraphIsDigraph(g));
    assert(GraphIsWeighted(g) == 0);

    // get the num of vertices in the graph
    unsigned int numVertices = GraphGetNumVertices(g);

    // create response graph
    Graph* response = GraphCreate(numVertices, 1, 0);

    // return if there is no edjes
    //if ( GraphGetNumEdges(g) == 0) return response;

    // iterate over each vertex
    for ( unsigned int vertex1 = 0 ; vertex1 < numVertices; ++vertex1  ){

        // compute the bellman fort paths
        GraphBellmanFordAlg* bf = GraphBellmanFordAlgExecute(g, vertex1);

        // iterate over all vertices
        for ( unsigned int vertex2 = 0; vertex2 < numVertices; ++vertex2){

            //skip if vertex1 is equal to vertex2
            if (vertex1==vertex2) continue;

            // check if it is possible to reach vertex2 starting from vertex1
            // the bellman module already provides this feature

            MARK_ACCESS++;
            if ( GraphBellmanFordAlgReached(bf, vertex2)){

                // create an edje in the graph between vertex1 and vertex2
                GraphAddEdge(response, vertex1, vertex2);
            }
        }

        // free allocated space
        GraphBellmanFordAlgDestroy(&bf);
    }

    //  return
    return response;
}
