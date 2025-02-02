#include"Dictionary.h"
#include<iostream>
#include<string>
/****************************************************************************************
 *  Dictionary.cpp
 *  
 *  Name: David Shonack
 *  CruzID: dshonack
 *  Assignment: pa7
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
}

// Copy constructor.
Dictionary::Dictionary(const Dictionary& D) {
    // Pre order walk func ig
    nil = new Node("*",0);
    root = nil;
    current = nil;
    num_pairs = 0;
    preOrderCopy(D.root, D.nil);
    num_pairs = D.num_pairs;
}

// Destructor
Dictionary::~Dictionary() {
    postOrderDelete(root,nil);
    delete nil;
}

// Helper Functions (Optional) ---------------------------------------------

//Used For Delete It Transplants
void Dictionary::Transplant(Node *u, Node *v){
	if( u->parent==nil ){ root = v; }
	else if(u == u->parent->left){ u->parent->left = v; }
	else{ u->parent->right = v; }
	if( v!=nil ){ v->parent = u->parent; }
}
     
 // inOrderString()
   // Appends a string representation of the tree rooted at R to string s. The
   // string appended consists of: "key : value \n" for each key-value pair in
   // tree R, arranged in order by keys.
void Dictionary::inOrderString(std::string& s, Node* R) const{
   	if( R!=nil ){
		inOrderString(s,R->left);
		s += R->key + " : " + std::to_string(R->val) + "\n";
		inOrderString(s,R->right); 
	}
}

   // preOrderString()
   // Appends a string representation of the tree rooted at R to s. The appended
   // string consists of keys only, separated by "\n", with the order determined
   // by a pre-order tree walk.
void Dictionary::preOrderString(std::string& s, Node* R) const{
	if( R!=nil ){
		s += R->key + "\n";
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
void Dictionary::postOrderDelete(Node* R, Node* N){
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
Dictionary::Node* Dictionary::findNext(Node* N){
	if( N == findMax(root) ){ return nil; }
	else if( N->right==nil && N->left==nil){ return N->parent; } //case 1 Left and Right are both nil 
	else if( N->left==nil ){ return N->right; } // case 2 only left is nil 
	else{ return N->left; }// case 3 both true
}

// findPrev()
// If N does not point to the leftmost Node, returns a pointer to the
// Node before N in an in-order tree walk.  If N points to the leftmost 
// Node, or is nil, returns nil.
Dictionary::Node* Dictionary::findPrev(Node* N){
	if( N == findMin(root) ){ return nil; }
	else if( N->right==nil && N->left==nil){ return N->parent; } //case 1 Left and Right are both nil 
	else if( N->left==nil ){ return N->parent; } // case 2 only left is nil 
	else{ return N->left; }// case 3 both true 
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
	if ( result ){ return result->val; }
	return result->val; 
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
	postOrderDelete(root,nil); 
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
	if( root==nil ){
		Node* newNode = new Node(k,v);
		newNode->parent = nil; 
		newNode->left = nil;
		newNode->right = nil; 
		root = newNode;
		num_pairs++; 
		return;  
	}
	Node *c = root; 
	while( c!=nil){
		if ( c->key == k ){
			c->val = v; 
			break;
		}
		else if( c->key > k ){ 
			if( c->left==nil ){
				Node* newNode = new Node(k,v);
				newNode->parent = c; 
				newNode->left = nil; 
				newNode->right = nil; 
				c->left = newNode; 
				num_pairs++;
				break;			
			}
			c = c->left; 
			
		}
		else if( c->key < k ){ 
			if( c->right==nil ){
				Node* newNode = new Node(k,v);
				newNode->parent = c; 
				c->right = newNode; 
				newNode->right = nil; 
				newNode->left = nil; 
				num_pairs++;
				break;			
			}
			c = c->right; 
			
		}
	}
	
}


// remove()
// Deletes the pair for which key==k. If that pair is current, then current
// becomes undefined.
// Pre: contains(k).
void Dictionary::remove(keyType k){ 
	Node *z = search(root,k); 
	if( z->left == nil){
		Transplant(z,z->right);
	}
	else if( z->right==nil ){
		Transplant(z,z->left);
	}
	else{
		Node *y = findMin(z->right);
		if( y->parent != z){
			Transplant(z,y); 
			y->right = z->right; 
			y->right->parent = y; 
		}
		Transplant(z,y);
		y->left = z->left; 
		y->left->parent = y; 
	}
	num_pairs--; 
	if( z==current ){ current = nil; }
	delete z; 

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

