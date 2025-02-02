/****************************************************************************************
 *  Graph.c
 *  Graph ADT 
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa3
 *****************************************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "List.h"
#include "Graph.h"
#include <string.h>
#define MAX_LEN 4096

int main(int argc,char* argv[]){
	if( argc!=3 ){
		printf("3 Arguments Where not given, Program Terminating\n");
		exit(EXIT_FAILURE);
	}
//Initializations -----------------------------------------------------------------
	FILE *in;
   	FILE *out;
   	char line[MAX_LEN]; 
	int n, s, u, count, line_count; 
	int Component = count = 0;
	int j = 0;
	List values = newList();
//Get First Line in File ---------------------------------------------------------
	in = fopen( argv[1], "r" ); 
	fgets(line, MAX_LEN, in); 
	n = atoi(line); 
	Graph G = newGraph(n); 
//Traverses First Part of FILE -----------------------------------------------------
	while( fgets(line, MAX_LEN, in) != NULL)  {
		line_count++;
      		sscanf(line, "%d %d",&s,&u); 
      		if( !s && !u ){ break; }
      		addArc(G,s,u); 
      		printf("%d %d\n",s,u);
      		append(values,s);
}
// -----------------------------------------------------------------------------------
	out = fopen(argv[2],"a");
	fprintf(out,"Adjacency list representation of G:\n");
	printGraph(out,G); 
	Graph T = transpose(G);
	printGraph(stdout, T);  
	DFS(G,values); 
	//List new1 = copyList(S);
	printList(stdout, values); 
	printf("\n");
	DFS(T,values);
	printList(stdout, values); 
	for(moveFront(values); index(values)!=-1; moveNext(values)){
		if(getParent(T,get(values)) <= 0){
			count++; 
		}
	}
	printf("%d",count);
	fprintf(out,"\nG contains %d strongly connected components:\n", count); 
	int h; 
	List S = newList();
	S = copyList(values); 
	List S2 = newList();
	for(moveFront(S); index(S)!=-1 ; moveNext(S)){
		prepend(S2,get(S));
	}
	printf("\n");
	printList(stdout,S2);
	for(moveFront(S2); index(S2)!=-1; ){
		if( getParent(T,get(S2))==0 ){
			Component++; 
			fprintf(out, "Component %d:",Component); 
			h = index(S2); 
			for(; index(S2)!=-1; movePrev(S2)){ 
				fprintf(out," %d", get(S2));  
				
			}
			moveFront(S2);
			for(int i = 0; i <= h; i++ ){ 
				delete(S2);
				moveFront(S2); 
			}
		 	
			fprintf(out,"\n");
		}
		else{
			moveNext(S2);
		
		}
	
	}
	/*
	for(moveFront(S); index(S)!=-1;){
		if(getParent(T,get(S))==0 ){
			Component++; 
			fprintf(out, "Component %d:",Component); 
			j = index(S)+1; 
			for(moveFront(S); index(S)!=j; moveNext(S)){fprintf(out," %d", get(S)); }
			moveBack(S); 
			while( j!=(length(S)+1) ){ 
				delete(S);
				moveFront(S); 
			}
		 
			fprintf(out,"\n");
		}
		else{
			moveNext(S);
		
		}
	}
	*/
	return 0; 

}



