//---------------------------------------------------------------------
// Matrix.c 
// CruzID: dshonack
// pa4 
//---------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h> 
#include "Matrix.h"
#include "List.h"

// Constructor-Destructor --------------------------------------------

typedef struct EntryOBJ{
	int col; 
	EntryElement value; 
}EntryOBJ; 

typedef struct EntryOBJ* Entry; 

typedef struct MatrixOBJ{
	int size; 
	int NNZ; 
	List* rows; 
}MatrixOBJ; 

Entry create_entry(Matrix M, int i, int j, double x){
		Entry newEntry = malloc(sizeof(EntryOBJ)); 
		newEntry->col = j;
		newEntry->value = x; 
		return newEntry;
}
// Returns a reference to a new nXn Matrix object in the zero state.
Matrix newMatrix(int n){
	Matrix newMatrix = calloc( 1,sizeof(MatrixOBJ) ); 
	assert( newMatrix!=NULL ); 
	newMatrix->rows = malloc(sizeof(List)*(n+1));
	assert( newMatrix->rows!=NULL ); 
	for( int i=1; i<=n; i++ ){ 
		newMatrix->rows[i] = newList(); 
	}
	newMatrix->size = n; 
	return newMatrix; 
}

// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix* pM){
	for( int i=1; i<=((*pM)->size); i++){
		for(moveFront((*pM)->rows[i]); index((*pM)->rows[i])!=-1; moveNext((*pM)->rows[i])){
		free((Entry)get((*pM)->rows[i])); 
		free((*pM)->rows[i]);
		}
	}
	free(*pM); 
	return; //tbd 
}

// Return the size of square Matrix M.
int size(Matrix M){
	return M->size; 
}

// Return the number of non-zero elements in M.
int NNZ(Matrix M){
	return M->NNZ; 
}

// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B){ 
	Entry entryA;
	Entry entryB; 
	if( (!(size(A)==size(B))) ){
		return 0; 
	}
	
	for(int i = 1; i<size(A); i++){
		moveFront(B->rows[i]);
		if ( length(A->rows[i])==length(B->rows[i]) ){
		for(moveFront(A->rows[i]); index(A->rows[i])!=-1; ){
			entryA = (Entry)get(A->rows[i]); 
			entryB = (Entry)get(B->rows[i]);
			if(entryA->col==entryB->col){
				moveNext(A->rows[i]); 
				moveNext(B->rows[i]); 
			}
			else {return 0;}
		}
		}
		else {return 0;}
	}
	return 1; 
	
}


// Manipulation procedures ------------------------------------------------

//Matrix->row[ith]->list->node->Entry->value 
// Re-sets M to the zero Matrix state.
void makeZero(Matrix M){
	//Entry current;
	for(int i = 1; i<size(M); i++){
		freeList(&(M->rows[i]));
		moveFront(M->rows[i]);
		M->rows[i] = newList(); 
	}
	M->NNZ = 0;
	//M->size = 0;  
}
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x){
	if( !(((1<=i) && (i<=size(M))) || ((1<=j)&&(j<=size(M)))) ){
		return; 
	}
	List current = M->rows[i];
	Entry block; 
	if( length(M->rows[i])==0 && x!=0 ){
		//check to sort 
		append(M->rows[i], create_entry(M,i,j,x)); 
		M->NNZ++;
		return;
	}
//Checks if value is in List ---------------------------------------------------------------------
	for(moveFront(current); index(current)!=-1 ; ){
		block = (Entry)get(current);
		if( block->col==j ){
			if(x == 0){
				M->NNZ--;
				delete(current);
				break; 
			}
			block->value = x;
			M->NNZ++;
			return; 
		}
		if( block->col>j ){
			insertBefore(current,create_entry(M,i,j,x) );
			M->NNZ++;
			return; 
		}
		else if( block->col<j){ moveNext(current); }
		
		
	}
	if(x==0){
		return;
	}
	else{
	append(M->rows[i], create_entry(M,i,j,x)); 
	M->NNZ++;
	return;
	}
	
	
}

// Matrix Arithmetic operations ------------------------------------------------

// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A){
	Matrix new = newMatrix(size(A));
	new->size = A->size; 
	new->NNZ = A->NNZ; 
	Entry entryA; 
	List currentA; 
	for(int i = 1; i<=size(A); i++){ 
		for(moveFront(A->rows[i]); index(A->rows[i])!=-1; moveNext(A->rows[i])){
			currentA = A->rows[i]; 
			entryA = (Entry)get(currentA); 
			append(new->rows[i], create_entry(new,i,entryA->col,entryA->value));
			//printMatrix(stdout,new); 
		}
	}
	
	return new;
}

// Returns a reference to a new Matrix object representing the transpose
// of A.
Matrix transpose(Matrix A){
	Matrix T = newMatrix(size(A)); 
	Entry entryX;
	for(int i = 1; i<=A->size; i++){
		for(moveFront(A->rows[i]); index(A->rows[i])!=-1; moveNext(A->rows[i])){
			entryX = (Entry)get(A->rows[i]); 
			changeEntry(T,entryX->col,i,entryX->value);
		}
	}
	return T; 
}

// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A){
	List current; 
	Entry entry; 
	Matrix new = copy(A); 
	for(int i = 1; i<=size(new); i++){
		current = new->rows[i];
		for(moveFront(current); index(current)!=-1; moveNext(current)){
			entry = (Entry)get(current); 
			entry->value *= x; 
		}
	}
	return new; 
}

// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
void add(Matrix S, List A, List B, int row){
	moveFront(A);
	moveFront(B);
	Entry entryA;
	Entry entryB;
	for(;index(A)!=-1 && index(B)!=-1; ){
		entryA = (Entry)get(A);
		entryB = (Entry)get(B);  
		if(entryA->col==entryB->col){
			if( (entryB->value + entryA->value != 0) ){
				append(S->rows[row],create_entry(S,row,entryB->col,(entryB->value + entryA->value)));
				S->NNZ++;
			}
			moveNext(A);
			moveNext(B);
		}
		else if(entryA->col > entryB->col){
			append(S->rows[row],create_entry(S,row,entryB->col,(entryB->value)));
			moveNext(B); 
			S->NNZ++;
		}
		else if(entryA->col < entryB->col){
			append(S->rows[row],create_entry(S,row,entryA->col,(entryA->value)));
			moveNext(A);
			S->NNZ++;
		}
	}
	
	while(index(A)!=-1){
		entryA = (Entry)get(A);
		append(S->rows[row],create_entry(S,row,entryA->col,(entryA->value)));
		moveNext(A); 
		S->NNZ++;
	}
	while(index(B)!=-1){
		entryB = (Entry)get(B);
		append(S->rows[row],create_entry(S,row,entryB->col,(entryB->value)));
		moveNext(B); 
		S->NNZ++;
	}
}

Matrix sum(Matrix A, Matrix B){
	if( size(A)!=size(B) ){ return NULL; } 
	Matrix S = newMatrix(size(A)); 
	Matrix C = copy(B);
	for(int i =1; i<=size(A); i++){
		add(S,A->rows[i],C->rows[i],i); 
	}
		//free Matrix B
		return S; 
		
}

void sub(Matrix S, List A, List B, int row){
	moveFront(A);
	moveFront(B);
	Entry entryA;
	Entry entryB;
	for(;index(A)!=-1 && index(B)!=-1; ){
		entryA = (Entry)get(A);
		entryB = (Entry)get(B);  
		if(entryA->col==entryB->col){
			if( (entryB->value - entryA->value != 0) ){
				append(S->rows[row],create_entry(S,row,entryB->col,(entryA->value - entryB->value)));
				S->NNZ++;
			}
			moveNext(A);
			moveNext(B);
		}
		else if(entryA->col > entryB->col){
			append(S->rows[row],create_entry(S,row,entryB->col,-(entryB->value)));
			moveNext(B); 
			S->NNZ++;
		}
		else if(entryA->col < entryB->col){
			append(S->rows[row],create_entry(S,row,entryA->col,(entryA->value)));
			moveNext(A);
			S->NNZ++;
		}
	}
	
	while(index(A)!=-1){
		entryA = (Entry)get(A);
		append(S->rows[row],create_entry(S,row,entryA->col,(entryA->value)));
		moveNext(A); 
		S->NNZ++;
	}
	while(index(B)!=-1){
		entryB = (Entry)get(B);
		append(S->rows[row],create_entry(S,row,entryB->col,-(entryB->value)));
		moveNext(B); 
		S->NNZ++;
	}
}
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B){
	if( size(A)!=size(B) ){ return NULL; } 
	Matrix S = newMatrix(size(A)); 
	Matrix C = copy(B);
	for(int i =1; i<=size(A); i++){
		sub(S,A->rows[i],C->rows[i],i); 
	}
		//remeber to free C
		return S; 

}

double vectorDot(List P, List Q){
	double totalDot=0; 
	Entry entryP; 
	Entry entryQ; 
	moveFront(Q);

	for(moveFront(P);index(P)!=-1 && index(Q)!=-1 ;){
		entryP = (Entry)get(P);
		entryQ = (Entry)get(Q); 
		if(entryP->col==entryQ->col){
			totalDot += entryP->value*entryQ->value; 
			moveNext(P);
			moveNext(Q);
		}
		else if (entryP->col > entryQ->col){
			moveNext(Q);
		}
		else if (entryQ->col > entryP->col){
			moveNext(P);
		}
	}
	return totalDot; 
}

// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B){
	assert( size(A)==size(B) ); 
	Matrix T = transpose(B);
	Matrix new = newMatrix(size(B)); 
	double dot; 
	//printMatrix(stdout,A); 
	for(int i = 1; i<= size(A); i++){
		for(int j = 1; j<=size(B); j++){
		 	dot = vectorDot(A->rows[i],T->rows[j]);
		 	if( dot!=0 ){ 
		 		new->NNZ++; 
		 		append(new->rows[i],create_entry(new,i,j,dot)); 
		 	}
		 }
	}
	//free T 
	return new; 
	 
}

// Prints a string representation of Matrix M to filestream out. Zero rows
// are not printed. Each non-zero row is represented as one line consisting
// of the row number, followed by a colon, a space, then a space separated
// list of pairs "(col, val)" giving the column numbers and non-zero values
// in that row. The double val will be rounded to 1 decimal point.
void printMatrix(FILE* out, Matrix M){
	List current;
	Entry entry; 
	for(int i = 1; i<=size(M); i++){
		current = M->rows[i];
		if( length(current)==0 ){ continue; }
		fprintf(out,"%d:",i); 
		for(moveFront(current); index(current)!=-1 ; moveNext(current)){
			entry = (Entry)get(current);
			fprintf(out," (%d, %.1f)",entry->col,entry->value); 
			//printf("(%d, %.1f)\n",entry->col,entry->value);
		}
		fprintf(out,"\n"); 
	}
	fprintf(out,"\n"); 
		

}


