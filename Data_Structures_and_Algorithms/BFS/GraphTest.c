/****************************************************************************************
 *  Graph.c
 *  Graph ADT 
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa2 
 *****************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"
#include "List.h"
#define NIL 0 
#define INF -1
int main(int argc, char* argv[]){
// List Client Test 1-----------------------------------------------------
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
   
// END of List Client Test 1-----------------------------------------------------

// Testing Functions 2 Pre conditions for functions -----------------------------
//    should terminate program 

/*
	List L = newList()
	Graph G = newGraph(100); 
	getPath(L,G,NIL);
	getParent(G,10000);
	getDist(G,10000);
	
*/ 

// Testing 3 Functionality -------------------------------------------------

	List m = newList();

//	newGraph() n = 50 

	Graph J = newGraph(50);
	
//	addEdge() 

	for(int i = 1; i < 50; i++){
		 addEdge(J, i, i+1);
	}
	
//      printGraph() to stdout 

	printGraph(stdout,J); 

/* Expected OutPut: 
1: 2 
2: 1 3 
3: 2 4 
4: 3 5 
5: 4 6 
6: 5 7 
7: 6 8 
8: 7 9 
9: 8 10 
10: 9 11 
11: 10 12 
12: 11 13 
13: 12 14 
14: 13 15 
15: 14 16 
16: 15 17 
17: 16 18 
18: 17 19 
19: 18 20 
20: 19 21 
21: 20 22 
22: 21 23 
23: 22 24 
24: 23 25 
25: 24 26 
26: 25 27 
27: 26 28 
28: 27 29 
29: 28 30 
30: 29 31 
31: 30 32 
32: 31 33 
33: 32 34 
34: 33 35 
35: 34 36 
36: 35 37 
37: 36 38 
38: 37 39 
39: 38 40 
40: 39 41 
41: 40 42 
42: 41 43 
43: 42 44 
44: 43 45 
45: 44 46 
46: 45 47 
47: 46 48 
48: 47 49 
49: 48 50 
50: 49
*/

//  getOrder()  

	if( getOrder(J)!= 50){
		printf("Failed getOrder Test\n");
	}

//  getSize()

	if( getSize(J)!= 50){
		printf("%d\n",getSize(J));
		printf("Failed getSize Test\n");
	}

 
//  BFS()

	BFS(J,32);
	printGraph(stdout, J); //Adjacency List

// getPath() 

	getPath(m,J, 49);
	printList(stdout, m); // print path
	
// getDist()
	getDist(J,45);
	
// getParent()
	getParent(J,2);

// freeGraph()
	freeGraph(&J);	
	
//      
// Correctly Formatted Graph OutPut ----------------------------------------
    Graph A = newGraph(100);
    List L = newList(); 
    addEdge(A, 64, 4);
    addEdge(A, 64, 3);
    addEdge(A, 42, 2);
    addEdge(A, 2, 64);
    addEdge(A, 4, 2);
    addEdge(A, 3, 42);
    BFS(A, 3);
    getPath(L, A, 64);
    append(C, 3);
    append(C, 64);
    moveFront(L);
    BFS(A, 2);
    getPath(L, A, 2);
    append(C, 2);
    getPath(L, A, 99);
    if (equals(L, C))
      printf("FAILED TEST 3\n");
    clear(L);
    clear(C);
    append(C, NIL);
    BFS(A, 99);
    getPath(L, A, 2);
    if (!equals(C, L))
      printf("FAILED TEST 4\n");
      
      
     
    return 0;
  
}
