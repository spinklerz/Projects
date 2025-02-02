/****************************************************************************************
 *  Graph.c
 *  Graph ADT 
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa3 
 *****************************************************************************************/

//-----------------------------------------------------------------------------
// GraphClient.c
// Test client for the Graph ADT
//-----------------------------------------------------------------------------
#include <stdint.h>

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "List.h"
#include "Graph.h"
#include <string.h>


int main(int argc, char* argv[]){
   int i, n=8;
   List S = newList();
   Graph G = newGraph(n);
   Graph T=NULL, C=NULL;

   for(i=1; i<=n; i++) append(S, i);

   addArc(G, 1,2);
   addArc(G, 1,5);
   addArc(G, 2,5);
   addArc(G, 2,6);
   addArc(G, 3,2);
   addArc(G, 3,4);
   addArc(G, 3,6);
   addArc(G, 3,7);
   addArc(G, 3,8);
   addArc(G, 6,5);
   addArc(G, 6,7);
   addArc(G, 8,4);
   addArc(G, 8,7);
   printGraph(stdout, G);

   DFS(G, S);
   fprintf(stdout, "\n");
   fprintf(stdout, "x:  d  f  p\n");
   for(i=1; i<=n; i++){
      fprintf(stdout, "%d: %2d %2d %2d\n", i, getDiscover(G, i), getFinish(G, i), getParent(G, i));
   }
   fprintf(stdout, "\n");
   printList(stdout, S);
   fprintf(stdout, "\n");

   T = transpose(G);
   C = copyGraph(G);
   fprintf(stdout, "\n");
   printGraph(stdout, C);
   fprintf(stdout, "\n");
   printGraph(stdout, T);
   fprintf(stdout, "\n");

   DFS(T, S);
   fprintf(stdout, "\n");
   fprintf(stdout, "x:  d  f  p\n");
   for(i=1; i<=n; i++){
      fprintf(stdout, "%d: %2d %2d %2d\n", i, getDiscover(T, i), getFinish(T, i), getParent(T, i));
   }
   fprintf(stdout, "\n");
   printList(stdout, S);
   fprintf(stdout, "\n");

   freeList(&S);
   freeGraph(&G);
   freeGraph(&T);
   freeGraph(&C);
   Graph A = newGraph(100);
   List L = newList();
   /*   for (uint8_t i = 1; i <= 100; i++)
      if (getDiscover(A, i) != UNDEF)
        return 1;
    addArc(A, 64, 4);
    addArc(A, 64, 3);
    addArc(A, 42, 2);
    addArc(A, 2, 64);
    addArc(A, 4, 2);
    addArc(A, 3, 42);
    for (uint8_t i = 1; i <= 100; i++) {
      prepend(L, i);
    }
    DFS(A, L);
    printGraph(stdout,A); 
    if (getDiscover(A, 4) != 80)
      return 4;
    DFS(A, L);
    if (getDiscover(A, 4) != 126)
      return 5;
    if (getDiscover(A, 63) != 117)
      return 6;
    DFS(A, L);
    if (getDiscover(A, 65) != 71)
      return 7;
    if (getDiscover(A, 1) != 199)
      return 8;
    for (uint8_t i = 1; i <= 100; i++)
      if (getParent(A, i) != NIL)
        return 1;
      */  
    addArc(A, 64, 4);
    addArc(A, 64, 3);
    addArc(A, 42, 2);
    addArc(A, 2, 64);
    addArc(A, 4, 2);
    addArc(A, 3, 42);
    for (uint8_t i = 1; i <= 100; i++) {
      append(L, i);
    }
    DFS(A, L);
    if (getParent(A, 100) != NIL)
      return 2;
    //if (getParent(A, 2) != NIL)
      //return 3;
    if (getParent(A, 42) != 3)
      return 4;
    if (getParent(A, 3) != 64)
      return 5;
    if (getParent(A, 4) != 64)
      return 6;
  
  
  
   return(0);
}




