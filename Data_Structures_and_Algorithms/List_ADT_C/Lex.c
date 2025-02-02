/****************************************************************************************
 *  Lex.c
 *  Client Side 
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa1 
 *****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "List.h"
#define MAX_LEN 4096
// Insertion Sort Functions --------------------------------------------------------

// Swaps Two Nodes
void swap(List L1, int data1, int data2){
	movePrev(L1); // make sure L1 is ahead of L2
	set(L1,data2);
	moveNext(L1); 
	set(L1,data1);
	movePrev(L1);
}

// Finds a value and moves cursor to it ****not used*****
void find(List L1,int value){
	moveFront(L1); 
	while(get(L1)!=value){moveNext(L1);}
}

// Part 2 of insertion sort, sorts towards the front of the list 
void sortPrev(List lst, int data1, char** array){
	int frontList = front(lst);
	int data2 = 0;  
	movePrev(lst);
	data2 = get(lst); 
	for(int i = 0; (strcmp(array[data1],array[data2]) < 0) ; i++){
			frontList = front(lst);
			moveNext(lst);
			swap(lst,data2,data1); 
			movePrev(lst); 
			data2 = get(lst); 
			if(data2==frontList){
				if((strcmp(array[data1],array[data2]) < 0)){
					moveNext(lst);
					swap(lst,data2,data1); 
				}
				moveFront(lst); 
				break;
			}
	}
}

// Part 1 of insertion sort, sorts towards the end of list 
void sortNext(List lst, int data1, int data2, int size, char** array){
	int j = 0; 
	//add index
	int pos; 
		for( j = 0; j < size && index(lst)!=(length(lst)-1) ; j++){ 
			data1 = get(lst); 
   			moveNext(lst); 
   			data2 = get(lst);
   			if((strcmp(array[data1],array[data2]) > 0 && j==0 )){
   				swap(lst,data1,data2);
   				}	
   				else if ((strcmp(array[data1],array[data2]) > 0)){
   					swap(lst,data1,data2);
   					pos = index(lst); 
   					sortPrev(lst,data2,array);
   					while(pos!=index(lst)){
   						moveNext(lst); 
   					}
   				}
   				if(index(lst)==-1){moveBack(lst);}
   			
   				
   		
}

}
int main(int argc, char* argv[]){
// Initialize Values ----------------------------------------------------------------
   FILE *in; 
   FILE *out; 
   char line[MAX_LEN];
   char Buffer[MAX_LEN]; 
   char *token; 
   int numTokens = 0;;
   char **array = (char**)malloc(MAX_LEN*sizeof(char*));
   size_t size = 0;  // rows
// Check Correct Arguements -------------------------------------------------------------
   if(argc!=3){
   	printf("Arguement Error: The number of arguements provided doesnt not match the intended.\n");
   	exit(EXIT_FAILURE);
   }
   in = fopen(argv[1],"r"); 
   assert ( in!=NULL );
   List lst = newList();  
// Get row from file, copy the row token is pointing to into array[i] --------------------
   for(int i = 0; fgets(line, MAX_LEN, in)!=NULL;i++){
  	token = strtok(line,"\n"); 
  	array[i] = malloc(strlen(token)+1); 
  	strcpy(array[i],token); // didnt knwo we can actually strcpy a pointer to a string to something 
  	append(lst,i); 
	size++;
	//printf("%d\n",i);
	//printf("String = %s Current Line = %d\n ",array[i],i);
   }
 // printf("Array size = %ld\n", size);
  fclose(in);
// Sort List -----------------------------------------------------------------------------
   moveFront(lst);
   sortNext(lst,0,0,size*2,array); 
   out = fopen(argv[2],"a");
   printList(out,lst);
   fclose(out);
// Read list indexes from file store into buffer -----------------------------------------
   out = fopen(argv[2],"r");
   int bytesRead = fread(Buffer, 1, MAX_LEN - 1, out);
   Buffer[bytesRead] = '\0';
  // printf("Buffer: %s\n", Buffer);
  // printf("Bytes Read = %d\n",bytesRead); 
   fclose(out);
// Write to file output ------------------------------------------------------------------
	out = fopen(argv[2], "w");
	token = strtok(Buffer, " "); 
	for (int h = 0; h < size; h++) {
   	 if (token == NULL) {
        	break;
    	}
    	numTokens++;
    	fprintf(out, "%s\n", array[atoi(token)]);
    // printf("%s\n", array[atoi(token)]);
    	token = strtok(NULL, " ");
	}
	//printf("num tokens = %d\n",numTokens); 
	

   fclose(out); 
   //free array 
// FREE EVERYTHIN ------------------------------------------------------------------------
   for(int j = 0; j < size; j++){
   	free(array[j]);
   	
   }
   free(array);
   freeList(&lst);
   return(0);
 
}
