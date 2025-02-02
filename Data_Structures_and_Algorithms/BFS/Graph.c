/****************************************************************************************
 *  Graph.c
 *  Graph ADT 
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa2 
 *****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Graph.h"
#include "List.h"
#define NIL 0 
#define INF -1
#include <string.h>
typedef struct ListOBJ* List; 
// Initialize QueueOBJ ------------------------------------------------------------------------------------
typedef struct GraphOBJ{
	int *distance;
	int *color; // Where 1 is White 2 is Gray 3 is Black 
	int *parent; // 0 is nil or undef
	List* al;
	int size; 
	int order; 
	int source; 
} GraphOBJ; 

// Constructor - Destructor -------------------------------------------------------------------------------

//Create a newGraph 
Graph newGraph(int n){
	Graph newGraph = malloc(sizeof(GraphOBJ));
	newGraph->distance = malloc((n+1)*sizeof(int));
	newGraph->parent = malloc((n+1) * sizeof(int));
	newGraph->color = calloc(n+1,sizeof(int));
	newGraph->al = malloc((n+1)*sizeof(List));
	newGraph->order = n; 
	newGraph->size = 0; 
	for(int i = 1; i <= n; i++){
		newGraph->parent[i] = NIL;
		newGraph->distance[i] = INF; 
		newGraph->al[i] = newList(); 
		newGraph->color[i] = 1; 
	 }
	return newGraph; 
}

//Frees all memory allocated in Graph
void freeGraph(Graph* pG){
	free((*pG)->distance);
	free((*pG)->parent); 
	free((*pG)->color);
	for(int i = 1; i <= (*pG)->order ; i++){
		freeList(&((*pG)->al[i])); 
		 
	}
	free(((*pG)->al)); 
	free(*pG); 
}


// Access Functions ---------------------------------------------------------------------------------------

//Get Vertices in Current Graph
int getOrder(Graph G){
	return G->order;
}

//Get edges in Current Graph
int getSize(Graph G){
	return G->size; 
}

//Get Source Node
int getSource(Graph G){
	//search for source node
	return G->source;  
}

//Get parent of current Node
int getParent(Graph G, int u){
	if( u >= 1 && u <= G->order){
		return G->parent[u]; 
	}
	else{
		//printf("Graph Error: getParent() value u is not within range\n");
		//exit(EXIT_FAILURE);
		return INF; 
	}
}

//Get distance from source to u
int getDist(Graph G, int u){
	if( (u >= 1 && u <= G->order)){
			return  G->distance[u]; 
	}
	else{
		//printf("Graph Error: getDist() value u is not within range\n");
		return INF; 
	}
}

//Describe path from source to terminus
void getPath(List L, Graph p, int u){	
	assert( getSource(p)!=NIL ); 
	
   	if (u == getSource(p)){
        	append(L,getSource(p));
        	}
   	else if(u == NIL){
        	clear(L); 
        	append(L,NIL); 
        }
   	else{
        	getPath(L,p, p->parent[u]);
        	moveFront(L); 
        	if( !findList(L,NIL) ){
        		append(L,u);
        	}
        	
        }
}

// Manipulation Functions ----------------------------------------------------------------------------------

//Make Graph Null
void makeNull(Graph G){
	for(int i = 1; i <= G->order ; i++){
		G->distance[i] = INF; 
		G->parent[i] = NIL;
		G->color[i] = 1; 
	 }
}

//Adds edge to Graph if already found in list do nothing
//use cursor to sort list
void addEdge(Graph G, int u, int v){
	if( !findList( G->al[u], v ) ){
		append( G->al[u] , v );
		if( length(G->al[u])!=0 ){
			moveBack(G->al[u]); 
			while( index(G->al[u])!=-1 ){
				movePrev(G->al[u]);
				if( v < get(G->al[u]) ){
					//swap 
					int temp1 = get(G->al[u]); 
					set(G->al[u],v);
					moveNext(G->al[u]);
					set(G->al[u],temp1); 					
				}
			}
			
		}
		
	}
	if( !findList( G->al[v], u ) ){ 
		append( G->al[v] , u );
		if( length(G->al[v])!=0 ){
			moveBack(G->al[v]); 
			while( index(G->al[v])!=-1 ){
				movePrev(G->al[v]);
				if( u < get(G->al[v]) ){
					//swap 
					int temp1 = get(G->al[v]); 
					set(G->al[v],u);
					moveNext(G->al[v]);
					set(G->al[v],temp1); 					
				}
			}
			
		}
		
	}
	 
	 G->size++; 
}

//Adds Directed Vertice
void addArc(Graph G, int u, int v){
	if( !findList( G->al[u], v ) ){ 
		append( G->al[u] , v);
		G->size++;
	}
}

//Will Perform BFS on a Duplicate Graph 
void BFS(Graph G, int s){
//intialization of source node and Lists
	makeNull(G);
	List Queue = newList();
	G->source = s; 
	int x = s; 
	List current;
	append(Queue,x);
	moveFront(Queue);
	G->color[x] = 3;
	G->distance[x] = 0;
	G->parent[x] = 0; 
	while((index(Queue))!=-1){ 
		current = G->al[x]; // list 1
		moveFront(current); 
		while( index(current)!=-1 ){
			if( G->color[get(current)] == 1 ){
				G->parent[get(current)] = x; 
				G->color[get(current)] = 2;
				G->distance[get(current)] = G->distance[x] + 1; 
				append(Queue,get(current)); 
			}
			moveNext(current); 
		}
		G->color[x] = 3;
		moveNext(Queue);
		x = get(Queue); 
	}
	freeList(&Queue);  
}

//Print out Adjancey list 
void printGraph(FILE *out, Graph G){
//output Adjancecy list 
	//fprintf(out, "Vertice| adjList | color | distance | parent\n");
	for(int i = 1; i < G->order; i++){
		fprintf(out,"%d: ", i); 
		printList(out,G->al[i]);
		fprintf(out, "\n");
		//fprintf(out,"|           %d        |          %d        |        %d\n",G->color[i],G->distance[i],G->parent[i]);
	}
	fprintf(out,"%d: ", G->order); 
	printList(out,G->al[G->order]);
}


//-----------------------------------------------------------------------------
//
// GraphClient.c
// 
// This is a fancy test client that calculates the Radius and Diameter of 
// the graph defined on lines 41-49, along with its Central and Peripheral 
// vertices.  The definitions of these terms at:
// 
//    http://en.wikipedia.org/wiki/Distance_(graph_theory)
// 
// Place this file in a directory with copies of your List.c, List.h, Graph.c, 
// Graph.h and an appropriate Makefile, then compile and run. The output 
// is contained in the file GraphClientOut.
// 
// This program does not exercise all functions in your Graph ADT, but it 
// does a pretty good job of testing BFS().  If your output differs from 
// the above, you have a logical problem in either your Graph or List ADT.
// 
// Remember you are required to submit a file named GraphTest.c with pa4 that
// exercises your Graph functions.  Do not submit this file.
//
//-----------------------------------------------------------------------------
/*
int main(int argc, char* argv[]){
   int i, s, max, min, d, n=35;
   List  C = newList(); // central vertices 
   List  P = newList(); // peripheral vertices 
   List  E = newList(); // eccentricities 
   Graph G = NULL;
	
   // Build graph G 
   G = newGraph(n);
   for(i=1; i<n; i++){
      if( i%7!=0 ) addEdge(G, i, i+1);
      if( i<=28  ) addEdge(G, i, i+7);
   }
   addEdge(G, 9, 31);
   addEdge(G, 17, 13);
   addEdge(G, 14, 33);

   // Print adjacency list representation of G
   printGraph(stdout, G);

   // Calculate the eccentricity of each vertex 
   for(s=1; s<=n; s++){
      BFS(G, s);
      max = getDist(G, 1);
      for(i=2; i<=n; i++){
         d = getDist(G, i);
         max = ( max<d ? d : max );
      }
      append(E, max);
   }

   // Determine the Radius and Diameter of G, as well as the Central and 
   // Peripheral vertices.
   append(C, 1);
   append(P, 1);
   min = max = front(E);
   moveFront(E);
   moveNext(E);
   for(i=2; i<=n; i++){
      d = get(E);
      if( d==min ){
         append(C, i);
      }else if( d<min ){
         min = d;
         clear(C);
         append(C, i);
      }
      if( d==max ){
         append(P, i);
      }else if( d>max ){
         max = d;
         clear(P);
         append(P, i);
      }
      moveNext(E);
   }

   // Print results 
   printf("\n");
   printf("Radius = %d\n", min);
   printf("Central vert%s: ", length(C)==1?"ex":"ices");
   printList(stdout, C);
   printf("\n");
   printf("Diameter = %d\n", max);
   printf("Peripheral vert%s: ", length(P)==1?"ex":"ices");
   printList(stdout, P);
   printf("\n");

   // Free objects 
   freeList(&C);
   freeList(&P);
   freeList(&E);
   freeGraph(&G);

   return(0);
}*/
