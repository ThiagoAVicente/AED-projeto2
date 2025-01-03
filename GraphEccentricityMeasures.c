//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphEccentricityMeasures
//

// Student Name : José Bagagem
// Student Number : 120141
// Student Name : Thiago Vicente
// Student Number : 121497

/*** COMPLETE THE GraphEccentricityMeasuresCompute FUNCTION ***/
/*** COMPLETE THE GraphGetCentralVertices FUNCTION ***/
/*** COMPLETE THE GraphEccentricityMeasuresPrint FUNCTION ***/

#include "GraphEccentricityMeasures.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphAllPairsShortestDistances.h"

struct _GraphEccentricityMeasures {
  unsigned int*
      centralVertices;  // centralVertices[0] = number of central vertices
                        // array size is (number of central vertices + 1)
  int* eccentricity;    // the eccentricity value of each vertex
  Graph* graph;         // the graph
  int graphRadius;      // the graph radius
  int graphDiameter;    // the graph diameter
};

// Allocate memory
// Compute the vertex eccentricity values
// Compute graph radius and graph diameter
// Compute the set of central vertices
GraphEccentricityMeasures* GraphEccentricityMeasuresCompute(Graph* g) {
  assert(g != NULL);

  // COMPLETE THE CODE
  // CREATE AUXILIARY (static) FUNCTIONS, IF USEFUL
  // Graph radius --- the smallest vertex eccentricity value
  // Graph diameter --- the largest vertex eccentricity value
  // Do not forget that -1 represents an IDEFINITE value

  //Creates the GraphEccentricityMeasures
  GraphEccentricityMeasures* measures = (GraphEccentricityMeasures*)malloc(sizeof(GraphEccentricityMeasures)); 
  measures->graph=g;

  unsigned int numEdges = GraphGetNumEdges(g);
  unsigned int numVertices = GraphGetNumVertices(g);

  // Just returns if there are no edges or vertices
  if (numEdges == 0 || numVertices == 0) {
      measures->eccentricity = NULL;
      measures->graphDiameter = -1;
      measures->graphRadius = -1;
      measures->centralVertices = NULL;
      return measures;
  }

  //Allocates memory fo the array with the eccentricity values
  measures->eccentricity = (int*) malloc(sizeof(int)*numVertices);

  //Calculates the distance of every posssible pair of vertices 
  GraphAllPairsShortestDistances* pairsDistances = GraphAllPairsShortestDistancesExecute(g);

  //The diameter and radius are initialized with the respective minimum and maximum possible values
  int diameter = -1;
  int radius = numEdges;

  //The Number of Central Vertices is initialized
  unsigned int NCentralVertices=0;
  for (unsigned int i = 0; i<numVertices; i++){
    //Iterates through each vertex and initializes it eccentricity measure has the minimun possible value  
    int eccent = -1;
    for (unsigned int j = 0; j<numVertices; j++){
      //Compares the distance with every other vertix and stores the maximum value
      int distance=GraphGetDistanceVW(pairsDistances, i, j);
      if (distance>eccent){
        eccent=distance;
      }
    }
    
    //Stores the eccentricity value
    measures->eccentricity[i]= eccent;

    //Check if it is greater than the current diameter, if it is, it becomes the new diameter
    if (diameter<eccent){
      diameter=eccent;
    }
    //Check if it is smaller than the current radius, if it is, it becomes the new radius and resets the
    //number of central vertices
    if (radius>eccent){
      radius=eccent;
      NCentralVertices=1;
    }
    // If the eccentricity measure is equal to the radius, than it upadates the number of central vertices
    else if (radius==eccent)
    {
      NCentralVertices++;
    }
    
    
  }

  //Stores the radius and diameter values
  measures->graphDiameter=diameter;
  measures->graphRadius=radius;

  //Destroys the pairDistance as it is no more needed
  GraphAllPairsShortestDistancesDestroy(&pairsDistances);

  // Computing the set of central vertices
  // Allocate the central vertices array : number of central vertices + 1
  // Fill in the central vertices array

  //Allocates memory for the array of central vertices
  measures->centralVertices = (unsigned int*)malloc(sizeof(unsigned int)*(NCentralVertices+1));
  measures->centralVertices[0] = NCentralVertices;

  //Stores all central vertices in the respective array
  unsigned int centralId = 1;
  for (unsigned int i = 0; i<numVertices; i++){
    if (measures->eccentricity[i]==radius){
      measures->centralVertices[centralId]=i;
      centralId++;
    }
  } 
  

  return measures;
}

void GraphEccentricityMeasuresDestroy(GraphEccentricityMeasures** p) {
  assert(*p != NULL);

  GraphEccentricityMeasures* aux = *p;

  free(aux->centralVertices);
  free(aux->eccentricity);

  free(*p);
  *p = NULL;
}

// Getting the computed measures

int GraphGetRadius(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphRadius;
}

int GraphGetDiameter(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphDiameter;
}

int GraphGetVertexEccentricity(const GraphEccentricityMeasures* p,
                               unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(p->eccentricity != NULL);

  return p->eccentricity[v];
}

// Getting a copy of the set of central vertices
// centralVertices[0] = number of central vertices in the set
unsigned int* GraphGetCentralVertices(const GraphEccentricityMeasures* p) {
  assert(p != NULL);
  assert(p->centralVertices != NULL);

  //Gets the original set of central vertices and the number of central vertices
  unsigned int* centralVertices = p->centralVertices;
  unsigned int NCentralVertices = centralVertices[0];

  //Allocates the memory needed to store the copy of the set of central vertices
  unsigned int* centralVerticesCpy = (unsigned int*) malloc((NCentralVertices+1)*sizeof(unsigned int));
  
  //Stores all values of the original set in the copy
  centralVerticesCpy[0]=NCentralVertices;
  for (unsigned int i=1; i<=NCentralVertices; i++){
    centralVerticesCpy[i]=centralVertices[i];
  }

  return centralVerticesCpy;
}

// Print the graph radius and diameter
// Print the vertex eccentricity values
// Print the set of central vertices
void GraphEccentricityMeasuresPrint(const GraphEccentricityMeasures* p) {
  // COMPLETE THE CODE

  //Prints the radius an the diameter
  printf("Radius: %d\n", p->graphRadius);
  printf("Diameter: %d\n", p->graphDiameter);

  //Iterates through every vertex and prints the vertex with the respective eccentricity value
  printf("Eccentricity Values:\n");
  unsigned int NumVertices = GraphGetNumVertices(p->graph);
  for(unsigned int i=0; i<NumVertices; i++){
    printf("Vertex %d: %d\n", i, p->eccentricity[i]);
  }

  //Gets the number of central vertices
  unsigned int NCentralVertices = p->centralVertices[0];

  //Prints every central vertices, the last one is printed without a comma in front
  printf("Central Vertices: ");
  unsigned int i=1;
  for(; i<NCentralVertices; i++){
    printf("%d, ", p->centralVertices[i]);
  }
  printf("%d\n", p->centralVertices[i]);
}
