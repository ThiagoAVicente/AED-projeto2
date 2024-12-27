//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphAllPairsShortestDistances
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

// DONE
/*** COMPLETE THE GraphAllPairsShortestDistancesExecute FUNCTION ***/

#include "GraphAllPairsShortestDistances.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"

#define INDEFINITE -1

struct _GraphAllPairsShortestDistances {
  int** distance;  // The 2D matrix storing the all-pairs shortest distances
                   // It is stored as an array of pointers to 1D rows
                   // Idea: an INDEFINITE distance value is stored as -1
  Graph* graph;
};

static void _check( const int condition,const char* mssg){
    // checks an condition. in case of false it stops exexcution
    if ( !condition ){
        printf("[ERROR] %s\n",mssg);
        exit(1);
    }
}

static void _fill(int * vec, unsigned int n, unsigned int val ){
    // fills vec with val
    for ( unsigned int i = 0; i < n;++i){
        vec[i] = val;
    }
}

static int** _allocate2DMatrix(unsigned int n){
    // allocates memory for a 2d, nxn, matrix

    int ** response = (int**)malloc(n*sizeof(int*));
    _check(response != NULL, "malloc in allocate2DMatrix");

    // alocate space for each line
    for ( unsigned int i = 0; i < n; ++i){
        response[i] = (int*)malloc(n*sizeof(int));
        _check(response[i]!=NULL, "malloc in allocate2DMatrix (lines)");
        _fill(response[i],n,INDEFINITE);
    }

    return response;
}

// Allocate memory and initialize the distance matrix
// Compute the distances between vertices by running the Bellman-Ford algorithm
GraphAllPairsShortestDistances* GraphAllPairsShortestDistancesExecute(
    Graph* g) {
  assert(g != NULL);

  // get info about g
  unsigned int numEdjes = GraphGetNumEdges(g);
  unsigned int numVertices = GraphGetNumVertices(g);

  // allocate response space
  GraphAllPairsShortestDistances* response = (GraphAllPairsShortestDistances*)malloc(sizeof(GraphAllPairsShortestDistances));
  response->distance = _allocate2DMatrix(numVertices);
  response->graph = g;

  // just return if there are no edjes or vertices
  if ( numEdjes == 0 || numVertices == 0) return response;

  // iterate over each vertice
  for( unsigned int vertex = 0; vertex < numVertices; ++vertex){
      // calculate bf
      GraphBellmanFordAlg* bf = GraphBellmanFordAlgExecute(g, vertex);

      // no need for all iteractions
      for ( unsigned int vertex2 = 0; vertex2 < numVertices; ++vertex2){

          // check if vertex was reached
          if ( GraphBellmanFordAlgReached(bf, vertex2)){
              response->distance[vertex][vertex2] = GraphBellmanFordAlgDistance(bf, vertex2);
          }
      }

      // free space
      GraphBellmanFordAlgDestroy(&bf);
  }

  return response;
}

void GraphAllPairsShortestDistancesDestroy(GraphAllPairsShortestDistances** p) {
  assert(*p != NULL);

  GraphAllPairsShortestDistances* aux = *p;
  unsigned int numVertices = GraphGetNumVertices(aux->graph);

  for (unsigned int i = 0; i < numVertices; i++) {
    free(aux->distance[i]);
  }

  free(aux->distance);

  free(*p);
  *p = NULL;
}

// Getting the result

int GraphGetDistanceVW(const GraphAllPairsShortestDistances* p, unsigned int v,
                       unsigned int w) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(w < GraphGetNumVertices(p->graph));

  return p->distance[v][w];
}

// DISPLAYING on the console

void GraphAllPairsShortestDistancesPrint(
    const GraphAllPairsShortestDistances* p) {
  assert(p != NULL);

  unsigned int numVertices = GraphGetNumVertices(p->graph);
  printf("Graph distance matrix - %u vertices\n", numVertices);

  for (unsigned int i = 0; i < numVertices; i++) {
    for (unsigned int j = 0; j < numVertices; j++) {
      int distanceIJ = p->distance[i][j];
      if (distanceIJ == -1) {
        // INFINITY - j was not reached from i
        printf(" INF");
      } else {
        printf(" %3d", distanceIJ);
      }
    }
    printf("\n");
  }
}
