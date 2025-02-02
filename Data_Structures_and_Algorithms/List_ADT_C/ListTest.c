/****************************************************************************************
 *  ListTest.c
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa1 
 *****************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<assert.h>
#include"List.h"

// Test harness adds from ListClient
int main(int argc, char *argv[]) {
	if(argc != 3){
		printf("Please Provide 3 Arguemnets Program will crash\n"); 
		exit(EXIT_FAILURE);
	}
  List A = newList();
   List B = newList();
   List C = NULL;
   int i;

   for(i=1; i<=20; i++){
      append(A,i);
      prepend(B,i);
   }

   printList(stdout,A); 
   printf("\n");
   printList(stdout,B); 
   printf("\n");

   for(moveFront(A); index(A)>=0; moveNext(A)){
      printf("%d ", get(A));
   }
   printf("\n");
   for(moveBack(B); index(B)>=0; movePrev(B)){
      printf("%d ", get(B));
   }
   printf("\n");

   C = copyList(A);
   printf("%s\n", equals(A,B)?"true":"false");
   printf("%s\n", equals(B,C)?"true":"false");
   printf("%s\n", equals(C,A)?"true":"false");


   moveFront(A);
   for(i=0; i<5; i++) moveNext(A); // at index 5
   insertBefore(A, -1);            // at index 6
   for(i=0; i<9; i++) moveNext(A); // at index 15
   insertAfter(A, -2);
   for(i=0; i<5; i++) movePrev(A); // at index 10
   delete(A);
   printList(stdout,A);
   printf("\n");
   printf("%d\n", length(A));
   clear(A);
   printf("%d\n", length(A));

   freeList(&A);
   freeList(&B);
   freeList(&C);
   List F = newList();
   prepend(F,76);
   prepend(F,4);
   prepend(F,3);
   prepend(F,1);
   moveFront(F);
   insertBefore(F,115);
   printList(stdout,F);
   printf("length = %d\n",length(F));
   
   
   List j = newList();
   prepend(j, 76);
   prepend(j, 4);
   deleteFront(j);
   prepend(j, 3);
   prepend(j, 1);
   moveFront(j);
   insertBefore(j, 115);
   deleteFront(j);
   freeList(&j);
   
   List h = newList();
   List k = newList();
    append(h, 1);
    append(k, 1);
    append(h, 2);
    moveFront(k);
    insertAfter(k, 2);
    assert(equals(h, k));
    append(k, 3);
    moveBack(h);
    insertAfter(h, 3);
    assert( equals(h, k));
 	freeList(&h);
 	freeList(&k);
 	freeList(&F);
 	
 //***********************************************************TESTS***********************************************************
 	List List1 = newList(); 
 	// **************Testing Function Append() length() index()**************
 	append(List1, 60);
 	append(List1, 70);
 	append(List1, 80);
 	append(List1, 90);
 	append(List1, 100);
 	assert( length(List1)==5 );
 	// Length should be 5 
 	assert( index(List1)==-1 );
 	// index should be 0 
 	// **************Testing Functions moveFront() prepend() and get() front()**************
 	moveFront(List1);
 	prepend(List1, 10);
 	prepend(List1, 20);
 	prepend(List1, 30);
 	prepend(List1, 40);
 	prepend(List1, 50);
 	moveFront(List1);
 	assert( get(List1)==front(List1) ); 
 	assert( get(List1)==50 ); 
 	// **************Testing Functions insertBefore() moveBack() back() **************
 	insertBefore(List1,110);
 	moveFront(List1); 
 	assert( get(List1)==110 );
 	moveBack(List1);
 	assert( get(List1)== back(List1) );
 	// **************Testing Function insertAfter() moveFront() moveBack() moveNext() set() **************
 	insertAfter(List1, 8439);
 	//printf("Back = %d ", back(List1));  
 	assert( (back(List1))==8439 );
 	moveFront(List1); 
 	moveNext(List1); 
 	//printList(stdout,List1); 
 	assert( get(List1)==50 ); 
 	movePrev(List1); 
 	assert( get(List1)== 110);
 	moveFront(List1); 
 	set(List1,9999); 
 	// **************Testing Function deleteFront() deleteBack() **************
 	assert( get(List1)==9999 ); 
 	deleteFront(List1); 
 	assert( index(List1)==-1); 
 	printf("dawdad = %d\n",length(List1)); 
 	//printList(stdout, List1); 
 	deleteBack(List1);
 	assert( length(List1)==10); 
 	// **************Testing Function printList() copyList() **************
 	printList(stdout, List1); 
 	moveBack(List1);
 	//printf("Back = %d\n",get(List1));
 	List List2 = copyList(List1);
 	//printList(stdout,List2); 
 	// **************Testing Function Delete in 3 Senarios **************
 	// Senario *******1******** at the front of a list 
 	
 	printf("\n*************BEGINING OF SENARIO 1*************\n");
 	moveFront(List1); 
 	delete(List1); 
 	printList(stdout,List1); 
 	
 	printf("\n*************END OF SENARIO 1*************\n");
 	// Senario *******2******** at the Back of a list 
 	printf("*************BEGINING OF SENARIO 2*************\n");
 	moveBack(List1); 
 	delete(List1); 
 	printList(stdout,List1); 
 	printf("\n*************END OF SENARIO 2*************\n");
 	// Senario *******3******** NOT AT FRONT OR BACK 
 	printf("\n*************BEGINING OF SENARIO 3*************\n");
 	moveFront(List1); 
 	moveNext(List1);
 	moveNext(List1);
 	moveNext(List1);
 	moveNext(List1);
 	delete(List1); 
 	printList(stdout,List1); 
 	printf("\n*************END OF SENARIO 3*************\n");
 	//**********************Check Valgrind for memory leaks****************
 	clear(&List1); 
 	clear(&List2); 
 	// Tested ALL FUNCTIONS
 	printf("*****************CONGRADULATIONS!!!! ALL TESTS PASSED YOU THE GOAT!!!!!*****************\n"); 
 	 return 0; 
}

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
115 1 3 4 76
*/
