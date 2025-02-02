/****************************************************************************************
 *  Shuffle.c
 *  Implementation ADT 
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa5
 *****************************************************************************************/

#include <iostream>
#include <string>
#include  "List.h"
using namespace std; 


void shuffle(List& D){
	int lengthA; 
	List B,C;
	lengthA = D.length()/2;
	if(D.length()==1){ return; }
	for(D.moveFront(); !(D.position()==D.length()) ; D.moveNext() ){
		if(D.position() < lengthA){ C.insertBefore(D.peekNext()); }
		else{ B.insertBefore(D.peekNext()); }
	}
	D.clear();
	B.moveFront(); 
	C.moveFront();
	for(int p = 1; (B.position()!=B.length()) && !(C.length()==C.position()) ; p++){
		B.moveNext();
		B.insertBefore(C.peekNext());
		C.moveNext();
	}
	D = B; 
}

int main(int argc, char *argv[]){
   int n;
   int count = 0; 
   size_t begin, end, len;
   n = atoi(argv[1]); 
   if( argc != 2 ){
      cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
      return(EXIT_FAILURE);
   }
	cout << "deck size\tshuffle count" << endl;	
      	cout << "------------------------------" << endl; 
   List A,D; 
   for(int i = 1; i<=n; i++){
   	D.clear();
   	A.clear();
   	count = 0; 
   	for(int j = 1; j<=i; j++){
   		A.insertBefore(j); 
   	}
   	D = A;  
   	//cout << A << endl; 
   	do{
   		shuffle(D);
   		//cout << D << endl; 
   		count++; 
   	}while( !A.equals(D) );
   	cout << " " << i << "\t\t" << count<<endl;
   }

   return(EXIT_SUCCESS);
}
