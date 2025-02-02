/****************************************************************************************
 *  Graph.h
 *  Header file for Graph ADT 
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa2 
 *****************************************************************************************/

#ifndef GRAPH_H_INCLUDE__
#define GRAPH_H_INCLUDE__ 
#include "List.h"
#define NIL 0 
#define INF -1
// Constructors-Destructors -----------------------------------------------------------------
typedef struct ListOBJ* List; 
typedef struct GraphOBJ* Graph; 

//Creates a new Graph as a Queue
Graph newGraph(int n);

//Frees the Graph Queue
void freeGraph(Graph* pG);

// Acesss Functions -------------------------------------------------------------------------

//Get vertices in current Graph
int getOrder(Graph G);

//Get edges in current Graph
int getSize(Graph G);

//Get Source Node 
int getSource(Graph G);

//Get Parent Node of current Node
int getParent(Graph G, int u);

//Get distance from source node to u 
int getDist(Graph G, int u);

//Describe the path to obtain int u 
void getPath(List L, Graph G, int u);

// Manipulation Function -------------------------------------------------------------------

//Sets the current Graph to NULL
void makeNull(Graph G);

//Adds Edge to Graph where u is source and v is terminus
void addEdge(Graph G, int u, int v);

//Adds vertice to Graph where u is source and v is terminus
void addArc(Graph G, int u, int v);

//Conducts BFS on Graph G given source s 
void BFS(Graph G, int s);
// Other Operations --------------------------------------------------------------------------

// Prints Graph to a file 
void printGraph(FILE* out, Graph G);

#endif
