#include"Dictionary.h"
#include<iostream>
#include<string>
#define BLACK 0 
#define RED 1 
/****************************************************************************************
 *  Dictionary.cpp
 *  
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa8
 *****************************************************************************************/
// Class Constructors & Destructors ----------------------------------------
using namespace std;
// Creates new Dictionary in the empty state.
Dictionary::Node::Node(keyType k, valType v) {
    key = k; 
    val = v;
}

Dictionary::Dictionary() {
    nil = new Node("*",0);
    root = nil;
    current = nil;
    num_pairs = 0;
    nil->color = -1; 
}

// Copy constructor.
Dictionary::Dictionary(const Dictionary& D) {
    // Pre order walk func ig
    nil = new Node("*",0);
    root = nil;
    current = nil;
    num_pairs = 0;
    nil->color = -1; 
    preOrderCopy(D.root, D.nil);
    num_pairs = D.num_pairs;
}

// Destructor
Dictionary::~Dictionary() {
    //postOrderDelete(root,nil);
    delete nil;
}

// RBT Helper Functions (Optional) -----------------------------------------

// LeftRotate()
void Dictionary::LeftRotate(Node* N){
	Node *x = N;
	Node* y = x->right;
	x->right = y->left;
	y->left->parent = x;
	y->parent = x->parent; 
	if (x->parent == nil){ root = y; } 
	else if (x == x->parent->left){ x->parent->left = y; }
	else { x->parent->right = y; }
	y->left = x;
	x->parent = y;
	
}

// RightRotate()
void Dictionary::RightRotate(Node* N){
	Node *x = N; 
	Node* y = x->left;
	x->left = y->right;
	y->right->parent = x;
	y->parent = x->parent;
	if (x->parent == nil){ root = y; } 
	else if (x == x->parent->right){ x->parent->right = y; } 
	else{ x->parent->left = y; } 
	y->right = x;
	x->parent = y;
}


// RB_InsertFixUP()
 void Dictionary::RB_InsertFixUp(Node* N){
	Node* y;
	Node *z = N; 
   while (z->parent->color == RED){
      if (z->parent == z->parent->parent->left){
         y = z->parent->parent->right;
         if (y->color == RED){
            z->parent->color = BLACK;             // case 1
            y->color = BLACK;                     // case 1
            z->parent->parent->color = RED;       // case 1
            z = z->parent->parent;                // case 1   
            }
         
         else{
            if( z == z->parent->right ){
               z = z->parent;                     // case 2
               LeftRotate(z);   		  // case 2
            }             			
            z->parent->color = BLACK;              // case 3
            z->parent->parent->color = RED;        // case 3
            RightRotate(z->parent->parent);
            }
          }    	
      else{ 
         y = z->parent->parent->left;
         if( y->color == RED ){
            z->parent->color = BLACK;           // case 4
            y->color = BLACK;                   // case 4
            z->parent->parent->color = RED;     // case 4
            z = z->parent->parent;		// case 4
          }            
         else{
            if ( z == z->parent->left ){
               z = z->parent;                     // case 5
               RightRotate(z);			  // case 5
            }               			
            z->parent->color = BLACK;              // case 6
            z->parent->parent->color = RED;        // case 6
            LeftRotate(z->parent->parent);         // case 6
            }
      }
	}
   root->color = BLACK;
}

// RB_Transplant()
void Dictionary::RB_Transplant(Node* u, Node* v){
	if( u->parent==nil ){ root = v; }
	else if(u == u->parent->left){ u->parent->left = v; }
	else{ u->parent->right = v; }
	if( v!=nil ){ v->parent = u->parent; }
}

// RB_DeleteFixUp()
void Dictionary::RB_Delete(Node* N){
	Node* y;
	Node *x; 
	Node* z = N; 
	y = z;
	int y_original_color = y->color;
	if (z->left == nil){
		x = z->right;
		RB_Transplant(z, z->right);
	}
	else if (z->right == nil){
		x = z->left;
		RB_Transplant( z, z->left);
	}
	else {
		y = findMin(z->right);
		y_original_color = y->color;
		x = y->right;
		if (y->parent == z){ x->parent = y; }
		else{
			RB_Transplant(y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		RB_Transplant(z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	delete N;
	if (y_original_color == BLACK && x!= nil){ RB_DeleteFixUp(x); } 
}

// RB_Delete()
void Dictionary::RB_DeleteFixUp(Node* N){
	Node *x = N; 
	Node *w; 
   while ((x != root) && (x->color == BLACK) ){
      if (x == x->parent->left ){
         w = x->parent->right;
         if(w->color == RED){
            w->color = BLACK;                        // case 1
            x->parent->color = RED;                  // case 1
            LeftRotate(x->parent);              // case 1
            w = x->parent->right;
         }                    // case 1
         if ((w->left->color == BLACK) && (w->right->color == BLACK)){
            w->color = RED;                         // case 2
            x = x->parent;                          // case 2
         }
         else{
            if (w->right->color == BLACK){
               w->left->color = BLACK;                // case 3
               w->color = RED;                       // case 3
               RightRotate(w);                   // case 3
               w = x->parent->right;               // case 3
        	}
            w->color = x->parent->color;             // case 4
            x->parent->color = BLACK;                 // case 4
            w->right->color = BLACK;                  // case 4
            LeftRotate(x->parent);               // case 4
            x = root ;                            // case 4
      	}
      }
      else{ 
         w = x->parent->left;
         if (w->color == RED){
            w->color = BLACK;                        // case 5
            x->parent->color = RED;                  // case 5
            RightRotate(x->parent);              // case 5
            w = x->parent->left;  
         }                  
         if ((w->right->color == BLACK) && (w->left->color == BLACK)){
            w->color = RED;                          // case 6
            x = x->parent;  
          }                       
         else {
            if (w->left->color == BLACK){
               w->right->color = BLACK;             // case 7
               w->color = RED;                       // case 7
               LeftRotate(w);                    // case 7
               w = x->parent->left; 
            }           
            w->color = x->parent->color;               // case 8
            x->parent->color = BLACK;                // case 8
            w->left->color = BLACK;                   // case 8
            RightRotate(x->parent);              // case 8
            x = root;    
            }                         // case 8
   }
}
   x->color = BLACK;
   
}

// Helper Functions (Optional) ---------------------------------------------

//Used For Delete It Transplants
/*
void Dictionary::Transplant(Node *u, Node *v){
	if( u->parent==nil ){ root = v; }
	else if(u == u->parent->left){ u->parent->left = v; }
	else{ u->parent->right = v; }
	if( v!=nil ){ v->parent = u->parent; }
}
     */
 // inOrderString()
   // Appends a string representation of the tree rooted at R to string s. The
   // string appended consists of: "key : value \n" for each key-value pair in
   // tree R, arranged in order by keys.
void Dictionary::inOrderString(std::string& s, Node* R) const{
   	if( R!=nil ){
		inOrderString(s,R->left);
		if(R!=nil ){
			s += R->key + " : " + std::to_string(R->val) + "\n";
		}
		inOrderString(s,R->right); 
	}
}

   // preOrderString()
   // Appends a string representation of the tree rooted at R to s. The appended
   // string consists of keys only, separated by "\n", with the order determined
   // by a pre-order tree walk.
void Dictionary::preOrderString(std::string& s, Node* R) const{
	if( R!=nil ){
		s += R->key;
		s += R->color == RED ? " (RED)" : ""; 
		s += "\n"; 
		preOrderString(s,R->left);
		preOrderString(s,R->right); 
	}
}

   // preOrderCopy()
   // Recursively inserts a deep copy of the subtree rooted at R into this 
   // Dictionary. Recursion terminates at N.
void Dictionary::preOrderCopy(Node* R, Node* N){
	if ( R == N ){ return; } 
	else{
		setValue(R->key, R->val); 
		preOrderCopy(R->left, N); 
		preOrderCopy(R->right, N);
	}
}
   // postOrderDelete()
   // Deletes all Nodes in the subtree rooted at R.
void Dictionary::postOrderDelete(Node* N){
	/*if ( !(R==N) ){ 
		postOrderDelete(R->left,N); 
		postOrderDelete(R->right,N); 
		cout << "Deleteing: " << R->key << "\n";
		R->val = 0;
		if( !(R->val == 0) ){
			R = nullptr; 
			R->parent = nullptr; 
			R->left = nullptr; 
			R->right = nullptr; 
			R->key = ""; 
			delete R;
		} 
		
	}
	*/
	
}

// search()
// Searches the subtree rooted at R for a Node with key==k. Returns
// the address of the Node if it exists, returns nil otherwise.
Dictionary::Node* Dictionary::search(Node* R, keyType k) const{
		while( R!=nil ){
				if ( R->key==k ){ return R; } 
				else if( R->key < k ){ R = R->right; }
				else if (R->key > k){ R = R->left;  }
		}
		return nil; 
}

// findMin()
// If the subtree rooted at R is not empty, returns a pointer to the 
// leftmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMin(Node* R){
	while (R->left != nil){
      		R = R->left;
      		if(R->left==R){ break; }
      	}
   	return R;
}

// findMax()
// If the subtree rooted at R is not empty, returns a pointer to the 
// rightmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMax(Node* R){
	while (R->right != nil){ R = R->right; }
	return R;

}

// findNext()
// If N does not point to the rightmost Node, returns a pointer to the
// Node after N in an in-order tree walk.  If N points to the rightmost 
// Node, or is nil, returns nil. 

Dictionary::Node* Dictionary::findNext(Node* N) {
	Node* x = N; 
    	
	if (x->right != nil){ return findMin(x->right); }
	Node *y = x->parent;
	while ((y != nil) && (x == y->right)){
		x = y;
		y = y->parent;
	}
	return y;
}



// findPrev()
// If N does not point to the leftmost Node, returns a pointer to the
// Node before N in an in-order tree walk.  If N points to the leftmost 
// Node, or is nil, returns nil.
Dictionary::Node* Dictionary::findPrev(Node* N){
	Node* x = N; 
    
	if (x->left != nil){ return findMax(x->left); }
	Node *y = x->parent;
	while ((y != nil) && (x == y->left)){
		x = y;
		y = y->parent;
	}
	return y;
}


// Access functions --------------------------------------------------------

// size()
// Returns the size of this Dictionary.
int Dictionary::size() const{ return num_pairs; }

// contains()
// Returns true if there exists a pair such that key==k, and returns false
// otherwise.
bool Dictionary::contains(keyType k) const{
	return search(root, k)->key == k ? 1 : 0; 
}

// getValue()
// Returns a reference to the value corresponding to key k.
// Pre: contains(k)
valType& Dictionary::getValue(keyType k) const{
	Node *result = search(root, k);
	if ( result !=nil ){ return result->val; }
	else{ throw std::logic_error("Current value not available."); }
}

// hasCurrent()
// Returns true if the current iterator is defined, and returns false 
// otherwise.
bool Dictionary::hasCurrent() const{ return current!=nil ? 1 : 0; }

// currentKey()
// Returns the current key.
// Pre: hasCurrent() 
keyType Dictionary::currentKey() const{
	if( hasCurrent() ){ return current->key; }
	else { throw std::logic_error("Current key not available."); }
}

// currentVal()
// Returns a reference to the current value.
// Pre: hasCurrent()
valType& Dictionary::currentVal() const{
	if( hasCurrent() ){ return current->val; }
	else { throw std::logic_error("Current key not available."); } 
}


// Manipulation procedures -------------------------------------------------

// clear()
// Resets this Dictionary to the empty state, containing no pairs.
void Dictionary::clear(){
	postOrderDelete(nil); 
	root = nil; 
	root->left = nil;
	root->right = nil; 
	current = nil; 
	num_pairs = 0;
}

// setValue()
// If a pair with key==k exists, overwrites the corresponding value with v, 
// otherwise inserts the new pair (k, v).
void Dictionary::setValue(keyType k, valType v){
	Node *x = search(root, k);
	if( x!= nil){ 
		x->val = v; 
		return; }
	if( root==nil ){
		Node* newNode = new Node(k,v);
		newNode->parent = nil; 
		newNode->left = nil;
		newNode->right = nil; 
		root = newNode;
		num_pairs++; 
		return;  
	}
	Node *newNode; 
	Node *c = root; 
	while( c!=nil){
		if ( c->key == k ){
			c->val = v; 
			break;
		}
		else if( c->key > k ){ 
			if( c->left==nil ){
				newNode = new Node(k,v);
				newNode->parent = c; 
				newNode->left = nil; 
				newNode->right = nil; 
				c->left = newNode; 
				num_pairs++;
				newNode->color = 1; 
				break;			
			}
			c = c->left; 
			
		}
		else if( c->key < k ){ 
			if( c->right==nil ){
				newNode = new Node(k,v);
				newNode->parent = c; 
				c->right = newNode; 
				newNode->right = nil; 
				newNode->left = nil; 
				newNode->color = 1;
				num_pairs++;
				break;			
			}
			c = c->right; 
			
		}
	}
	RB_InsertFixUp(newNode);  
}


// remove()
// Deletes the pair for which key==k. If that pair is current, then current
// becomes undefined.
// Pre: contains(k).
void Dictionary::remove(keyType k){ 
	Node *z = search(root,k); 
	if( z==current ){ current = nil;  }
	if( z==nil ){ throw std::logic_error("Value to Remove not available."); }
	RB_Delete(z); 
	num_pairs--;
	/*if( z->left == nil){
		RB_Transplant(z,z->right);
	}
	else if( z->right==nil ){
		RB_Transplant(z,z->left);
	}
	else{
		Node *y = findMin(z->right);
		if( y->parent != z){
			RB_Transplant(z,y); 
			y->right = z->right; 
			y->right->parent = y; 
		}
		RB_Transplant(z,y);
		y->left = z->left; 
		y->left->parent = y; 
	}
	num_pairs--; 
	if( z==current ){ current = nil; }
	delete z; 
*/
}


// begin()
// If non-empty, places current iterator at the first (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::begin(){
	if (root != nil) {
            current = findMin(root);
        }
}

// end()
// If non-empty, places current iterator at the last (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::end(){
	if (root != nil) {
            current = findMax(root);
        }
}

// next()
// If the current iterator is not at the last pair, advances current 
// to the next pair (as defined by the order operator < on keys). If 
// the current iterator is at the last pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::next(){
 	if (current != nil) {
            current = findNext(current);
        }
        else{ current = nil; }
}

// prev()
// If the current iterator is not at the first pair, moves current to  
// the previous pair (as defined by the order operator < on keys). If 
// the current iterator is at the first pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::prev(){
 	if (current != nil) {
            	current = findPrev(current);
        }
}


// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this Dictionary. Consecutive (key, value)
// pairs are separated by a newline "\n" character, and the items key and value 
// are separated by the sequence space-colon-space " : ". The pairs are arranged 
// in order, as defined by the order operator <.
std::string Dictionary::to_string() const{
	std::string s = ""; 
	inOrderString(s,root); 
	return s;
}

// pre_string()
// Returns a string consisting of all keys in this Dictionary. Consecutive
// keys are separated by newline "\n" characters. The key order is given
// by a pre-order tree walk.
std::string Dictionary::pre_string() const{
	std::string s = ""; 
	preOrderString(s,root); 
	return s; 
}

// equals()
// Returns true if and only if this Dictionary contains the same (key, value)
// pairs as Dictionary D.
bool Dictionary::equals(const Dictionary& D) const{
	if( num_pairs != D.num_pairs){ return 0; }
	std::string O = "";
	std::string t = "";
	D.inOrderString(O,D.root);
	inOrderString(t,root);
	return O==t ? 1 : 0; 	
}


// Overloaded Operators ----------------------------------------------------

// operator<<()
// Inserts string representation of Dictionary D into stream, as defined by
// member function to_string().
std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
	return stream << D.to_string(); 
}

// operator==()
// Returns true if and only if Dictionary A equals Dictionary B, as defined
// by member function equals(). 
bool operator==( const Dictionary& A, const Dictionary& B ){
	return A.equals(B);
}

// operator=()
// Overwrites the state of this Dictionary with state of D, and returns a
// reference to this Dictionary.
Dictionary& Dictionary::operator=( const Dictionary& D ){
	if(this != &D){
		Dictionary temp(D);
		std::swap(this->nil, temp.nil);
		std::swap(this->root, temp.root);
		std::swap(this->current, temp.current);
		std::swap(this->num_pairs, temp.num_pairs);

	}
	return *this;
}

