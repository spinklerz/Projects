/****************************************************************************************
 *  List.cpp
 *  Implementation ADT 
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa5
 *****************************************************************************************/
#include<string>
#include<iostream>
#include<stdexcept>
#include "List.h"
// Class Construct and Destructors --------------------------------------------
//Node Constructor
List::Node::Node(ListElement x){
	data = x; 
	prev = nullptr; 
	next = nullptr; 
}

List::List(){
	frontDummy = new Node(0); 
	backDummy = new Node(0);
	beforeCursor = frontDummy; 
	afterCursor = backDummy; 
	pos_cursor = 0; 
	num_elements = 0;
}

// Copy constructor.
List::List(const List& L) {
    frontDummy = new Node(0);
    backDummy = new Node(0);
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = L.num_elements;
    if (num_elements == 0) { return; }
    Node* current = L.frontDummy->next;
    while (current != L.backDummy) {
        this->insertBefore(current->data); 
        current = current->next;
    }
    
}
   
List::~List(){
	if (num_elements==0){
		delete frontDummy;
		delete backDummy; 
		frontDummy = nullptr;
		backDummy = nullptr; 
		return; 
	}
	
	Node *current = frontDummy->next;
	Node* del;
	delete frontDummy;
	delete backDummy;
	frontDummy = nullptr;
	backDummy = nullptr;  
	while( current->next ){
		del = current;
		current = current->next;
		delete del;
	}
	delete current;  
}

// Access Functions ---------------------------------------------------

//Returns length of list 
int List::length()const{ return num_elements; }

//Returns front element in list when length > 0 	
ListElement List::front()const{ 
	if(num_elements == 0){ 
		std::cout << "List: front(): empty list\n\tcontinuing without interruption" << std::endl; 
		return -1;
	}
	else { return 0; }
}		
	
//Returns back element in list when length > 0 	
ListElement List::back()const{
	if(num_elements == 0){ 
		std::cout << "List: back(): empty list\n\tcontinuing without interruption" << std::endl;
		return -1; 
	}
	else{ return 0; }	
}		
	
//Returns position in list if Cursor is defined	
int List::position()const{
	if ( pos_cursor!=-1 ){ return pos_cursor; }
	else{ return -1; }
}		

//Returns element of AfterCursor
ListElement List::peekNext()const{
	if( pos_cursor==num_elements ) 
	{
		std::cout << "List: peekNext(): cursor at Back\n\tcontinuing without interruption" << std::endl;
		return -1;  
	}
	return afterCursor->data; 
}

//Returns element of BeforeCursor
ListElement List::peekPrev()const{
	if( pos_cursor==0 ) 
	{
		std::cout << "List: peekPrev(): cursor at Front\n\tcontinuing without interruption" << std::endl; 
		return -1;
	}
	return beforeCursor->data; 
}

// Manipulation Procedures --------------------------------------------

//Sets list to a empty state
void List::clear(){
	Node* current = frontDummy->next; 
	Node* del = frontDummy->next; 
	if (num_elements==0){ return; }
	while( current!=backDummy ){
		current = current->next;
		delete del;  
		del = current; 
	}
	frontDummy->next = backDummy;
	backDummy->prev = frontDummy;
	moveFront(); 
	num_elements = 0; 
	pos_cursor = 0; 
}

//Move cursor to front of list
void List::moveFront(){
	afterCursor = frontDummy->next; 
	beforeCursor = frontDummy; 
	pos_cursor = 0; 
}	

//Move cursor to back of list 
void List::moveBack(){
	afterCursor = backDummy;
	beforeCursor = backDummy->prev; 
	pos_cursor = num_elements; 
}

//Moves the Cursor to the next available node		
ListElement List::moveNext(){
	if(  pos_cursor==num_elements ){ 
		pos_cursor = -1; 
		std::cout << "List: moveNext(): cursor at back\n\tcontinuing without interruption" << std::endl; 
		return -1; 
	}
	afterCursor = afterCursor->next;
	beforeCursor = beforeCursor->next;
	pos_cursor++; 
	return beforeCursor->data; 
}	

//Moves the Cursor to the prev available node		
ListElement List::movePrev(){
	if( pos_cursor==-1  || pos_cursor==0 ){ 
		std::cout << "List: movePrev(): cursor at front\n\tcontinuing without interruption" << std::endl;
		pos_cursor = -1;  
		return -1; 
	}
	afterCursor = afterCursor->prev;
	beforeCursor = beforeCursor->prev;
	pos_cursor--; 
	return afterCursor->data; 
}	

//Inserts x after cursor 
void List::insertAfter(ListElement x){//BEAUTIFUL 
		Node* newNode = new Node(x); 
		beforeCursor->next = newNode; 
		newNode->prev = beforeCursor; 
		afterCursor->prev = newNode;
		newNode->next = afterCursor; 
		afterCursor = newNode; 
		num_elements++; 
}

//Insert x beforeCursor (**Before Cursor updates with each call**)
void List::insertBefore(ListElement x){
	Node* newNode = new Node(x); 
	beforeCursor->next = newNode; 
	newNode->prev = beforeCursor; 
	afterCursor->prev = newNode;
	newNode->next = afterCursor; 
	beforeCursor = newNode; 
	pos_cursor++; 
	num_elements++; 
}

//Sets afterCursor to new element x 
void List::setAfter(ListElement x){
	if( pos_cursor>=num_elements || afterCursor==backDummy ){ 
		std::cout << "List: setAfter(): cursor at back\n\tcontinuing without interruption" << std::endl;
		pos_cursor = -1; 
		return;
	}
	afterCursor->data = x; 
}

//Sets beforeCursor to new element x
void List::setBefore(ListElement x){
	if( pos_cursor==0 || pos_cursor==-1 ){
		std::cout << "List: setBefore(): cursor at Front\n\tcontinuing without interruption" << std::endl; 
		return;
	}
	beforeCursor->data = x; 
}

//Erases After Cursor
void List::eraseAfter(){
	if( pos_cursor>=num_elements || afterCursor==backDummy ){ 
		std::cout << "List: eraseAfter(): cursor at back\n\tcontinuing without interruption" << std::endl; 
		return; 
	}
	Node *temp = afterCursor; 
	afterCursor = afterCursor->next; 
	temp->next = nullptr; 
	temp->prev = nullptr; 
	delete(temp); 
	beforeCursor->next = afterCursor; 
	afterCursor->prev = beforeCursor; 
	num_elements--; 
}

//Erases Before Cursor 
void List::eraseBefore(){
	if( pos_cursor==0 || pos_cursor==-1 || num_elements==0 ){
		std::cout << "List: eraseBefore(): cursor at front\n\tcontinuing without interruption" << std::endl; 
		return; 
	 }
	Node *temp = beforeCursor; 
	beforeCursor = beforeCursor->prev; 
	temp->next = nullptr;
	temp->prev = nullptr; 
	delete(temp);
	beforeCursor->next = afterCursor; 
	afterCursor->prev = beforeCursor; 
	pos_cursor--;
	num_elements--;
}

//Other Functions ---------------------------------------------------------

//Finds value in List x traversing front -> back 
int List::findNext(ListElement x) {
	while( pos_cursor<num_elements ){
		moveNext(); 
		if( afterCursor->data==x ){
			moveNext();
			return pos_cursor;
		}
	}
	moveBack();
	return -1; 
}

//Finds value in List x traversing from back -> front
int List::findPrev(ListElement x){ 
	while( pos_cursor>=0 ){
		movePrev(); 
		if (beforeCursor->data== x){
			movePrev(); 
			return pos_cursor; 
		}
		else if (afterCursor->data==x){
			return pos_cursor; 
		}
	}
	moveFront();
	return -1; 
}

//Wows
void List::cleanup(){
	Node *current = frontDummy; 
	Node *anchor = frontDummy; 
	Node *before;
	Node *after; 
	int temp,j;
	Node *del;  
// Initial clean not including beforeCurosr or afterCursor values --------------------------------
	for(int i = 0; i<num_elements; i++){
		j = 0;
		anchor = anchor->next;   
		temp = anchor->data;
		current = frontDummy->next;  
		while( current!=backDummy ){
			if( (current->data==beforeCursor->data || current->data==afterCursor->data ) && (!(current==beforeCursor || current==afterCursor)) && current!=anchor ){
				del = current;
				//delete -----------------------------------------------------------
				before = del->prev; 
				after = del->next; 
				before->next = after; 
				after->prev = before; 
				del->next = nullptr;
				del->prev = nullptr;
				num_elements--;
				delete(del); 
				current = after; 
				if( j<pos_cursor ){ pos_cursor--; }
			}
			else if( current==beforeCursor || current==afterCursor ){ 
				current = current->next;
				j++;
				continue; 
			}
			else if( temp==current->data && current!=anchor ){
				del = current;
				before = del->prev; 
				after = del->next; 
				before->next = after; 
				after->prev = before; 
				del->next = nullptr;
				del->prev = nullptr;
				num_elements--;
				delete(del); 
				current = after; 
				if( j<pos_cursor ){ pos_cursor--; }
			}
			else{ 
				current = current->next; 
				j++; 
			} 
		}
	
	} 
}

List List::concat(const List& L) const {
	List J;
   	Node* N = this->frontDummy->next;
   	Node* M = L.frontDummy->next;
	while ( N!=backDummy ) {
		J.insertBefore(N->data);
		N = N->next;
		
    }
    	while( M!=L.backDummy ){
    		J.insertBefore(M->data);
    		M = M->next; 
    	}
    J.num_elements = num_elements + L.num_elements; 
    J.moveFront(); 
    return J; // return the concatenated list
}


std::string List::to_string() const {
	std::string s = "";
	Node *current = frontDummy->next; 
  	while( current!=backDummy ){
      		s += std::to_string(current->data)+" ";
      		current = current->next; 
   }
   return s;
}

bool List::equals(const List& R)const{

	if (R.num_elements != num_elements){ return 0; }
	Node *current1 = frontDummy->next; 
	Node *current2 = (R.frontDummy)->next; 
	while(current1){
		if(current1->data != current2->data){ return 0; }
		current1 = current1->next;
		current2 = current2->next;
	}
	return 1; 
}


   // Overriden Operators -----------------------------------------------------
   
   // operator<<()
   // Inserts string representation of L into stream.
std::ostream& operator<<( std::ostream& stream, const List& L ){
	return stream << L.to_string(); 
}

   // operator==()
   // Returns true if and only if A is the same integer sequence as B. The 
   // cursors in both Lists are unchanged.
bool operator==( const List& A, const List& B ){
	return B.equals(A);
}

   // operator=()
   // Overwrites the state of this List with state of L.
List& List::operator=( const List& L ){
	//std::cout << "efwg";
	if( this != &L && L.num_elements!=0 ){
		pos_cursor = L.pos_cursor; 
		Node* current = (L.frontDummy)->next; 
		while( current!=(L.backDummy )){
			insertBefore(current->data); 
			current = current->next; 
		}
		}
		
    return *this; }
  


