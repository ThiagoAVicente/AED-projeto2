//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphBellmanFord - Bellman-Ford Algorithm
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

// -----DONE-----
/*** COMPLETE THE GraphBellmanFordAlgExecute FUNCTION ***/

#include "GraphBellmanFordAlg.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "Graph.h"
#include "IntegersStack.h"
#include "instrumentation.h"


struct _GraphBellmanFordAlg {
  unsigned int* marked;  // To mark vertices when reached for the first time
  int* distance;  // The number of edges on the path from the start vertex
                  // distance[i]=-1, if no path found from the start vertex to i
  int* predecessor;  // The predecessor vertex in the shortest path
                     // predecessor[i]=-1, if no predecessor exists
  Graph* graph;
  unsigned int startVertex;  // The root of the shortest-paths tree
};

static void _check(int condition, char mssg[] ){

    if (!condition){
        printf("[ERROR] %s\n",mssg);
        exit(1);
    }

}
static void _helperBellmanFord(GraphBellmanFordAlg* g, unsigned int start, unsigned int numVertices){
    // finds the shortest paths to all vertices starting from "start"

    // mark start vertex
    g->distance[start] = 0;
    g->predecessor[start] = -1;

    // check for the others vertices using bellman ford alg
    // flag to mark if the iteration did change something
    int changed = 1;

    // maximum amount ou iterations is |V| - 1
    int iter = numVertices - 1;

    while(iter > 0 && changed){

        // reset flag
        changed = 0;

        // iterate over each vertex that has a known path
        for (unsigned int vertex_i = 0; vertex_i < numVertices; ++vertex_i){

            if ( g->distance[vertex_i] == INT_MAX || g->marked[vertex_i]) continue;

            g->marked[vertex_i] = 1;

            // get the list of adjacent vertices
            unsigned int* next = GraphGetAdjacentsTo(g->graph,vertex_i);
            unsigned int numAdjacents = next[0]; // check graph.c for more info

            for ( unsigned int j = 1 ; j < numAdjacents+1; ++j){

                unsigned int adjacentVertex = next[j];

                // check for minimum value of the distance
                if (g->distance[adjacentVertex] == INT_MAX){

                  // calculate distance
                  int dist = g->distance[vertex_i] + 1;

                  // update variables
                  g->distance[adjacentVertex] = dist;
                  g->predecessor[adjacentVertex] = vertex_i;

                  // set flag to true
                  changed = 1;
                }
            }

            free(next);
        }
        // decrement iter
        iter--;
    }
}

GraphBellmanFordAlg* GraphBellmanFordAlgExecute(Graph* g,
                                                unsigned int startVertex) {
  assert(g != NULL);
  assert(startVertex < GraphGetNumVertices(g));
  assert(GraphIsWeighted(g) == 0);

  GraphBellmanFordAlg* result =
      (GraphBellmanFordAlg*)malloc(sizeof(struct _GraphBellmanFordAlg));
  assert(result != NULL);

  // Given graph and start vertex for the shortest-paths
  result->graph = g;
  result->startVertex = startVertex;

  unsigned int numVertices = GraphGetNumVertices(g);

  // CREATE AND INITIALIZE
  // result->marked
  result->marked = (unsigned int*) malloc(numVertices*sizeof(unsigned int*));
  _check(result->marked != NULL,"GraphBellmanFordAlgExecute--marked");

  // result->distance
  result->distance = (int*) malloc(numVertices*sizeof(int*));
  _check(result->distance!=NULL, "GraphBellmanFordAlgExecute--distance");

  // result->predecessor
  result->predecessor = (int*) malloc(numVertices*sizeof(int*));
  _check(result->predecessor!=NULL, "GraphBellmanFordAlgExecute--predessor");

  for ( unsigned int i = 0; i < numVertices; ++i){
    // Mark all vertices as not yet visited, i.e., ZERO
    result->marked[i] = 0;
    // No vertex has (yet) a (valid) predecessor
    result->predecessor[i] = -1;
    // No vertex has (yet) a (valid) distance to the start vertex
    result->distance[i] = INT_MAX;
  }

  // THE ALGORITHM TO BUILD THE SHORTEST-PATHS TREE
  _helperBellmanFord(result,startVertex,numVertices);

  return result;
}

void GraphBellmanFordAlgDestroy(GraphBellmanFordAlg** p) {
  assert(*p != NULL);

  GraphBellmanFordAlg* aux = *p;

  free(aux->marked);
  free(aux->predecessor);
  free(aux->distance);

  free(*p);
  *p = NULL;
}

// Getting the paths information

int GraphBellmanFordAlgReached(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->marked[v];
}

int GraphBellmanFordAlgDistance(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->distance[v];
}
Stack* GraphBellmanFordAlgPathTo(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = StackCreate(GraphGetNumVertices(p->graph));

  if (p->marked[v] == 0) {
    return s;
  }

  // Store the path
  for (unsigned int current = v; current != p->startVertex;
       current = p->predecessor[current]) {
    StackPush(s, current);
  }

  StackPush(s, p->startVertex);

  return s;
}

// DISPLAYING on the console

void GraphBellmanFordAlgShowPath(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = GraphBellmanFordAlgPathTo(p, v);

  while (StackIsEmpty(s) == 0) {
    printf("%d ", StackPop(s));
  }

  StackDestroy(&s);
}

// Display the Shortest-Paths Tree in DOT format
void GraphBellmanFordAlgDisplayDOT(const GraphBellmanFordAlg* p) {
  assert(p != NULL);

  Graph* original_graph = p->graph;
  unsigned int num_vertices = GraphGetNumVertices(original_graph);

  // The paths tree is a digraph, with no edge weights
  Graph* paths_tree = GraphCreate(num_vertices, 1, 0);

  // Use the predecessors array to add the tree edges
  for (unsigned int w = 0; w < num_vertices; w++) {
    // Vertex w has a predecessor vertex v?
    int v = p->predecessor[w];
    if (v != -1) {
      GraphAddEdge(paths_tree, (unsigned int)v, w);
    }
  }

  // Display the tree in the DOT format
  GraphDisplayDOT(paths_tree);

  // Housekeeping
  GraphDestroy(&paths_tree);
}
