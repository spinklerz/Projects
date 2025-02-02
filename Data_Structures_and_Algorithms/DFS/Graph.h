/****************************************************************************************
 *  Graph.h
 *  Header file for Graph ADT 
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa3 
 *****************************************************************************************/

#ifndef GRAPH_H_INCLUDE__
#define GRAPH_H_INCLUDE__ 
#include "List.h"
#define NIL 0
#define UNDEF -1
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

//Get Parent Node of current Node
int getParent(Graph G, int u);

//get finish time of Graph G 
int getFinish(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */

//get discover time of Graph G
int getDiscover(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */

// Manipulation Function -------------------------------------------------------------------


//Adds Edge to Graph where u is source and v is terminus
void addEdge(Graph G, int u, int v); /* Pre: 1<=u<=n, 1<=v<=n */

//Adds vertice to Graph where u is source and v is terminus
void addArc(Graph G, int u, int v); /* Pre: 1<=u<=n, 1<=v<=n */

// Conducts DFS on graph G
void DFS(Graph G, List S); /* Pre: length(S)==getOrder(G) */

// Works along side DFS
void Visit(Graph G, List S, int *time, int x);

// Other Operations --------------------------------------------------------------------------

// Transposes other graph 
Graph transpose(Graph G);

//Clears Graph
void makeNull(Graph G);

// Returns copied Graph
Graph copyGraph(Graph G);

// Prints Graph to a file 
void printGraph(FILE* out, Graph G);

#endif





