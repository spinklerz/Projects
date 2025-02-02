/****************************************************************************************
 *  List.c
 *  Implementation ADT 
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa2
 *****************************************************************************************/
#define MAX_LEN 300 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "List.h"
#define NIL 0 
#define INF
// Structs --------------------------------------------------------------------

typedef struct NodeOBJ* Node; //private node type 
// private NodeOBJ type 
typedef struct NodeOBJ{
	ListElement data; 
	Node next; 
	Node prev; 
}NodeOBJ;

// private ListOBJ type 
typedef struct ListOBJ{
	Node front; 
	Node back; 
	Node cursor; 
	int length; 
	int index;  
}ListOBJ; 
// Constructors-Destructors ---------------------------------------------------

Node create_node(int data){
	Node newNode = calloc(1,sizeof(NodeOBJ)); 
	newNode->data = data; 
	newNode->next = NULL;
	newNode->prev = NULL;
	return newNode;
}
// Creates and returns a new empty List.
List newList(void){
	List L;
	L = malloc(sizeof(ListOBJ)); 
	assert( L!=NULL );
	L->front = L->back = NULL;
	L->length = 0; 
	L->index = -1; 
	L->cursor = NULL; 
	return L; 
} 

// Frees all heap memory associated with *pL, and sets
// *pL to NULL.
void freeList(List* pL) {
    isNull(*pL);
    Node current = (*pL)->front;
    Node nextNode;
        while (current != NULL) {
            nextNode = current->next;
            free(current);
            current = nextNode;
            
        }
    free(*pL);
}

// Access functions -----------------------------------------------------------

// Returns the number of elements in L.
int length(List L){
	return L->length; 
}

// Returns index of cursor element if defined, -1 otherwise.
// The List->index will represent our cursor 
int index(List L){
	if( L->index==-1 ){return -1;}
	return L->index; 
} 

// Returns front element of L. Pre: length()>0
int front(List L){
	Node current = L->front; 
	if(length(L) > 0){return L->front->data;}
	else{
		printf("List Error: List has a length of 0\n");
		exit(EXIT_FAILURE);
	}
	while(current->prev){current = current->prev;}
	return current->data; 
}

// Returns back element of L. Pre: length()>0
int back(List L){
		if( length(L) > 0 ){return L->back->data;}
	else{
		printf("List Error: List has a length of 0\n");
		exit(EXIT_FAILURE);
	}
} 

// Returns cursor element of L. Pre: length()>0, index()>=0
int get(List L){
	if( ((length(L)<0) && (index(L)<0)) || (index(L)>=length(L))){return -1;} // can insert error msg if neeeded
	return L->cursor->data;
}

// Returns true iff Lists A and B are in same
// state, and returns false otherwise.
bool equals(List P, List G){
	if( ( length(P)!=length(G) ) || ( P==NULL ) || ( G==NULL ) ){
		//printf("List Error: passing NULL Lists or passing Lists of unequal lengths");
		return 0; 
	}
	Node Pptr = P->front; 
	Node Gptr = G->front; 
	while(Pptr && Gptr){
		if( Pptr->data !=Gptr->data ){
			return 0; 
		}
		Gptr = Gptr->next; 
		Pptr = Pptr->next;
	}
	return 1; 
} 

//returns 1 if value is found in list otherwise returns 0
bool findList(List L, int x){
	Node current = L->front; 
	while(current && L->index!=-1 ){
		if( current->data==x ){return 1; }
		current = current->next; 
	}
	return 0; 
}

// Manipulation procedures -----------------------------------------------------

// Resets L to its original empty state.
void clear(List L){
	isNull(L); 
	Node Lptr = L->front;
	while(Lptr){
		Lptr->data = 0;
		Lptr = Lptr->next; 
	}
	L->length = 0;
	L->index = -1; 
} 

// Overwrites the cursor element’s data with x.
void set(List L, int x){
	isNull(L); 
	L->cursor->data = x;  
}

// Pre: length()>0, index()>=0
// If L is non-empty, sets cursor under the front element,
// otherwise does nothing.
void moveFront(List L){
	if( L->length>0 && L!=NULL ){
		L->cursor = L->front; 
		L->index = 0; 
	}
	else{
		//printf("Manipulation Error: passed invalid value to moveFront()\n");
		//exit(EXIT_FAILURE);
	}
}

// If L is non-empty, sets cursor under the back element,
// otherwise does nothing.
void moveBack(List L){
	if( L!=NULL ){
		L->cursor = L->back; 
		L->index = L->length-1;  
	}
	else{
		printf("List Error: In Function moveBack() Passing a List that is NULL\n");
		exit(EXIT_FAILURE);
	} 
}

// If cursor is defined and not at front, move cursor one
// step toward the front of L; if cursor is defined and at
// front, cursor becomes undefined; if cursor is undefined
// do nothing
void movePrev(List L){
	if( ( L->index==0 ) || ( L->index < 0 ) ){
		L->index = -1;
	}
	else if( L->index>0 ){
		L->cursor = L->cursor->prev;  
		L->index -= 1;
		}
	// if undefined nothing happens
} 

// If cursor is defined and not at back, move cursor one
// step toward the back of L; if cursor is defined and at
// back, cursor becomes undefined; if cursor is undefined
// do nothing
void moveNext(List L){
	if( ( L->index >= 0 ) && ( !( L->length-1<=L->index ) ) ){ 
		L->cursor = L->cursor->next; 
		L->index += 1; 
	} 
	else{ L->index=-1; }
	// if undefined nothing happens
} 

// Insert new element into L. If L is non-empty,
// insertion takes place before front element.
void prepend(List L, int x){
	Node newNode = create_node(x);
	assert( newNode!=NULL ); 
	if( (L->length)==0 ){
		L->front = newNode; 
		L->back = newNode; 
	}
	else{
		if( L->index!=-1 ){
			L->index++; 
		}
		newNode->next = L->front; 
		L->front->prev = newNode; 
		L->front = newNode; 
	}
	L->length++; 
} 
// Insert new element into L. If L is non-empty,
// insertion takes place after back element.
//Graph.c implementatoin, will also check to keep adjacent value hiearchy decending to ascending 
// calls insertion sort 
void append(List L, int x){
	isNull(L); 
	if( length(L)==0 ){
		Node newNode = create_node(x);
		L->front = L->back = newNode; 
	}
	else{
		Node newNode = create_node(x);
		assert( newNode!=NULL );
	//	if(call insertion sort); 
		L->back->next = newNode; 
		newNode->prev = L->back; 
		L->back = newNode; 
		
	}
	L->length++; 
} 

// Insert new element before cursor.
void insertBefore(List L, int x){
	isNull(L); 
	assert( ( L->index!=-1 )  ); // if cursor is defined 
	if( L->length==0 || L->index==0 ){ // if list is empty 
		prepend(L,x);
	}
	else{
	Node newNode = create_node(x);
	Node after = L->cursor;
	Node before = L->cursor->prev; 
	newNode->next = after; 
	after->prev = newNode; 
	before->next = newNode; 
	newNode->prev = before; 
	L->length++; 
	L->index++; 
	}
	
}

// Pre: length()>0, index()>=0
// Insert new element after cursor.
void insertAfter(List L, int x){ // need to check if back val is passed
	assert( L->index>=0 ); // while not undefined 
	if( L->length==0 ){  //if length is 0 
		append(L,x);
	}
	else if ( L->length-1==L->index ){
			append(L,x);
		}
	else{

		Node after = L->cursor->next; 
		Node before = L->cursor; 
		Node newNode = create_node(x);
		newNode->prev = before; 
		newNode->next = after; 
		after->prev = newNode;
		before->next = newNode; 
		L->length++; 
	}

} 

// Pre: length()>0, index()>=0
// Delete the front element. Pre: length()>0
void deleteFront(List L){
	if( length(L)<0 ) {
		printf("List Error: passed undefined cursor or unintialized List to function deleteFront()\n");
		exit(EXIT_FAILURE);
	
	}
	Node current = L->front;
	L->front = current->next; 
	current->next = NULL; 
	current->data = 0; 
	L->length--; 
	L->index--;
	free(current); 
}

// Delete the back element. Pre: length()>0
void deleteBack(List L){
	if( length(L)<0 ) {
		printf("List Error: passed unintialized List to function deleteBack()\n");
		exit(EXIT_FAILURE);
	
	}
	if(L->index==(L->length-1) ){ L->index=-1; }
	else{
	Node current = L->back;
	L->back = current->prev; 
	current->prev = NULL; 
	current->next = NULL; 
	//L->back->next = NULL; 
	current->data = 0; 
	L->length--;  
	//free(current); 
	}
} 

// Delete cursor element, making cursor undefined.
// Pre: length()>0, index()>=0
void delete(List L){
	if( length(L)<0 || index(L)<0 ) {
		printf("List Error: passed undefined cursor or unintialized List to function delete()\n");
		exit(EXIT_FAILURE);
	
	}
	
	if( L->cursor==L->front){
		L->front = L->front->next; 
		free(L->cursor); 
	}
	else if( L->cursor==L->back){
		L->back = L->back->prev;
		L->back->next = NULL; 
		free(L->cursor);
	}
	else{
		Node after = L->cursor->next;
		Node before = L->cursor->prev;
		free(L->cursor);
		after->prev = before;
		before->next = after; 
	}
	
	L->index = -1; 
	L->length--; 
} 

// Other operations -----------------------------------------------------------

// Prints to the file pointed to by out, a
// string representation of L consisting
// of a space separated sequence of integers,
// with front on left.
void printList(FILE* out, List L){
	Node current = L->front; 
	while(current && current->data){
		fprintf(out,"%d ",current->data);
		current = current->next; 
	}
}

// Returns a new List representing the same integer
// sequence as L. The cursor in the new list is undefined,
// regardless of the sta
List copyList(List L){
	List newL = newList();
	Node current = L->front; 
	for(int i = 0; i < L->length; i++){
		append(newL,current->data); 
		current = current->next; 
	}
	newL->back = current; 
	return newL; 
}

// Checks if List passed is NULL
void isNull(List L){
	if( L==NULL ){ 
		printf("List Error: Calling freeList() on a NULL List Reference.\n");
		exit(EXIT_FAILURE); 
	}
} 

/*
int main(int argc, char* argv[]){
// Initialize Values ----------------------------------------------------------------
   FILE *in; 
   FILE *out; 
   char line[MAX_LEN];
   char Buffer[MAX_LEN]; 
   char *token; 
   char **array = (char**)malloc(MAX_LEN);
   size_t size = 0;  // rows
   List lst = newList(); 
   int temp1; 
   int temp2; 
   int MAX_ITERATION; 
// Check Correct Arguements -------------------------------------------------------------
   if(argc!=3){
   	printf("Arguement Error: The number of arguements provided doesnt not match the intended.\n");
   	exit(EXIT_FAILURE);
   }
   in = fopen(argv[1],"r"); 
   assert ( in!=NULL ); 
// Get row from file, copy the row token is pointing to into array[i] --------------------
   for(int i = 0; fgets(line, MAX_LEN, in)!=NULL;i++){
  	token = strtok(line,"\n"); 
  	array[i] = (char*)malloc(strlen(token)+1); 
  	strcpy(array[i],token); // didnt knwo we can actually strcpy a pointer to a string to something 
  	append(lst,i); 
	size++;
	//printf("%s\n",array[i]);
   }
   fclose(in);
// Sort List -----------------------------------------------------------------------------
   moveFront(lst);
   MAX_ITERATION = size*10; 
   for(int j = 0; j < MAX_ITERATION; j++){
   	temp1 = get(lst);
   	moveNext(lst); 
   	temp2 = get(lst); 
   	while(strcmp(array[temp1],array[temp2]) > 0) {
   		movePrev(lst); 
   		set(lst,temp2); 
   		moveNext(lst); 
   		set(lst,temp1); 
   		movePrev(lst);
   		movePrev(lst); 
   		if( index(lst)==-1 ){
   			lst->index = 0; 
   			
   		}
   		temp1 = get(lst);
   		moveNext(lst); 
   		temp2 = get(lst);
   		}
   }
   out = fopen(argv[2],"a");
   printList(out,lst);
   fclose(out);
   out = fopen(argv[2],"r");
   while(fgets(Buffer,MAX_LEN,out)!=NULL ){
   	printf("Buffer: %s ",Buffer); 
   }
   fclose(out); 
   out = fopen(argv[2],"w");
   token = strtok(Buffer," "); 
    for(int h = 0; h < size*2 ; h++){
   	if(token==NULL){continue;}
   	fprintf(out,"%s\n", array[atoi(token)]);
   	//printf("%s\n",array[atoi(token)]);
   	token = strtok(NULL," ");
   }
   fclose(out); 
   //free array 
   
   for(int j = 0; j < size; j++){
   	free(array[j]);
   	
   }
   free(array);
   freeList(&lst);
   
   return(0);
}
*/
/*
Output of this program:
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
false
false
true
1 2 3 4 5 -1 6 7 8 9 11 12 13 14 15 -2 16 17 18 19 20
21
0
*/
	

