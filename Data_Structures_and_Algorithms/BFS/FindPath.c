/****************************************************************************************
 *  Graph.c
 *  Graph ADT 
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa2 
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
	int s,u; 
	int line_count;
   	FILE *out;
   	char line[MAX_LEN];
	List L; 
	int n;
	in = fopen( argv[1], "r" ); 
	fgets(line, MAX_LEN, in); 
	n = atoi(line); 
	Graph G = newGraph(n); 
//Traverses First Part of FILE -----------------------------------------------------
	while( fgets(line, MAX_LEN, in) != NULL)  {
		line_count++;
      		sscanf(line, "%d %d",&s,&u); 
      		if( !s && !u ){ break; }
      		addEdge(G,s,u); 
}

	out = fopen(argv[2],"a");
	printGraph(out,G); 
	//printf("\n");
//Traverses second part of file ------------------------------------------------
	while( fgets(line, MAX_LEN, in) != NULL)  {
		line_count++;
      		sscanf(line, "%d %d",&s,&u); 
      		if( !s && !u ){ break; }
      		BFS(G,s); 
      		L = newList(); 
      		getPath(L,G,u); 
      		if( !front(L) ){
      			fprintf(out,"\n\nThe distance from %d to %d is infinity\n", s, u);
        		fprintf(out,"No %d-%d path exists",s,u);
      		}
      		else{
      			fprintf(out,"\n\nThe distance from %d to %d is %d\n",s,back(L),length(L)-1);
			fprintf(out, "A shortest %d-%d path is: ",s,back(L) );
			
			printList(out,L); 
			//printf("\n");
		} 
      		clear(L); 
}
	// 
	//printGraph(out, G);
	//BFS(G,15); 
	//getPath(L,G,8); 
	//fprintf(out,"\nA distance from %d to %d is %d",getSource(G),back(L),length(L)-1);
	//fprintf(out, "\nA Shortest %d-%d path is: ",getSource(G),back(L) ); 
	fclose(in);
	fclose(out);
	return 0; 
	

}
