/****************************************************************************************
 *  List.c
 *  Implementation ADT 
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa4
 *****************************************************************************************/
#define MAX_LEN 300 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "List.h"
#include "Matrix.h"
// Structs --------------------------------------------------------------------

typedef struct EntryOBJ* Entry;

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

Node create_node(void *data){
	Node newNode = calloc(1,sizeof(NodeOBJ));  // will be ptr to EntryOBJ  
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
	/*
    if(!(*pL)) { 
   	 free(*pL); 
   	 return;
   }
    Node current = (*pL)->front;
    Node nextNode;
        while (current != NULL) {
            nextNode = current->next;
            free((Entry)current->data);
            free(current);
            current = nextNode;
        }
        
    free(*pL);
    */
    
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
void* front(List L){
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
void* back(List L){
		if( length(L) > 0 ){return L->back->data;}
	else{
		printf("List Error: List has a length of 0\n");
		exit(EXIT_FAILURE);
	}
} 

// Returns cursor element of L. Pre: length()>0, index()>=0
void* get(List L){
	if( ((length(L)<0) && (index(L)<0)) || (index(L)>=length(L))){return NULL;} // can insert error msg if neeeded
	return L->cursor->data;
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
void set(List L, void* x){
	isNull(L); 
	L->cursor->data = &x;  
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
		return; 
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
void prepend(List L, void* x){
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
void append(List L, void* x){
	isNull(L); 
	if( length(L)==0 ){
		Node newNode = create_node(x);
		L->front = L->back = newNode; 
	}
	else{
		Node newNode = create_node(x);
		assert( newNode!=NULL );
		L->back->next = newNode; 
		newNode->prev = L->back; 
		L->back = newNode; 
	}
	L->length++; 
} 

// Insert new element before cursor.
void insertBefore(List L, void *x){
	isNull(L); 
	//assert( ( L->index!=-1 )  ); // if cursor is defined 
	if( L->length==0 || L->index==0 || L->index==-1 ){ // if list is empty 
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
void insertAfter(List L, void* x){ // need to check if back val is passed
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
		//exit(EXIT_FAILURE);
	
	}
	
	else{
	Node current = L->front;
	L->front = current->next; 
	current->next = NULL; 
	current->data = NULL; 
	if(L->length==1){L->index=-1;}
	else{L->index--;}
	L->length--; 
	free(current); 
	}
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
	free(current); 
	}
} 

// Delete cursor element, making cursor undefined.
// Pre: length()>0, index()>=0
void delete(List L){
	if( length(L)<0 || index(L)<0 ) {
		printf("List Error: passed undefined cursor or unintialized List to function delete()\n");
		//exit(EXIT_FAILURE);
	
	}
	else{
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
	}
	L->index = -1; 
	L->length--; 
} 

// Other operations -----------------------------------------------------------

// Checks if List passed is NULL
void isNull(List L){
	if( L==NULL ){ 
		printf("List Error: Calling freeList() on a NULL List Reference.\n");
		//exit(EXIT_FAILURE); 
	}
} 

