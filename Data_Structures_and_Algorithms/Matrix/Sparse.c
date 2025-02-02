#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h> 
#include "Matrix.h"
#include "List.h"
#define MAX_LEN 1024

int main(int argc, char *argv[]){
	if( argc!=3 ) {return 0;}
	FILE* in; 
	FILE* out; 
	char Buffer[MAX_LEN];
	int NNZA;
	int NNZB; 
	int n,i,j; 
	double x; 
	in = fopen(argv[1],"r"); 
	
	fgets(Buffer, sizeof(Buffer), in);
	sscanf(Buffer, "%d %d %lf",&i,&j,&x); 
	n = i; 
	NNZA = j;
	NNZB = (int)x; 
	fgets(Buffer, sizeof(Buffer), in); 
	Matrix A = newMatrix(n);
	Matrix B = newMatrix(n);
		while( fgets(Buffer, MAX_LEN, in) != NULL)  {
      			sscanf(Buffer, "%d %d %lf",&i,&j,&x); 
      			if( !strcmp(Buffer,"\n") ){ break; }
 			changeEntry(A, i, j, x); 
}
	while( fgets(Buffer, MAX_LEN, in) != NULL)  {
      			sscanf(Buffer, "%d %d %lf",&i,&j,&x); 
      			if( !strcmp(Buffer,"\n") ){ break; }
 			changeEntry(B, i, j, x); 
}
	fclose(in);
	out = fopen(argv[2],"a"); 
	fprintf(out,"A has %d non-zero entries:\n", NNZA); 
	printMatrix(out, A);
	fprintf(out,"B has %d non-zero entries:\n", NNZB); 
	printMatrix(out, B);
	fprintf(out,"(1.5)*A =\n"); 
	printMatrix(out, scalarMult(1.5,A)); 
	fprintf(out, "A+B =\n");
	printMatrix(out,sum(A,B));
	fprintf(out, "A+A =\n"); 
	printMatrix(out,sum(A,A));
	fprintf(out, "B-A =\n"); 
	printMatrix(out,diff(B,A));
	fprintf(out, "A-A =\n"); 
	printMatrix(out,diff(A,A));
	fprintf(out, "Transpose(A) =\n"); 
	printMatrix(out,transpose(A));
	fprintf(out, "A*B =\n"); 
	printMatrix(out,product(A,B));
	fprintf(out, "B*B =\n"); 
	printMatrix(out,product(B,B));
	return 0;
}
