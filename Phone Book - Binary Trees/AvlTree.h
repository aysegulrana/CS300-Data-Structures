#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

template <class Comparable>
class AvlTree;

template <class Comparable>
class AvlNode
{
public:
	Comparable 	element;
	AvlNode   		*left;
	AvlNode   		*right;
	int        		height;

	AvlNode( const Comparable & theElement, 
		AvlNode *lt, AvlNode *rt, int h = 0 )
		: element( theElement ), left( lt ), right( rt ), height( h ) { }

	friend class AvlTree<Comparable>;
};

template <class Comparable>
class AvlTree
{
public:
	explicit AvlTree( const Comparable & notFound );
	AvlTree( const AvlTree & rhs );
	~AvlTree( );

	const Comparable & findMin( ) const;
	const Comparable & findMax( ) const;
	const Comparable & find( const Comparable & x, vector<Comparable> &vec ) const;
	bool isEmpty( ) const;
	void printTree( ) const;
	void printTree( BinaryNode<Comparable> * &t ) const;

	void printPreorderAVL(AvlNode<Comparable> *node, ofstream &file);
	void printInorderAVL(AvlNode<Comparable> *node, ofstream & file);
	bool isBalanced(AvlNode<Comparable> *root);
	int minDepth(AvlNode<Comparable> *root) ;
	int maxDepth(AvlNode<Comparable> *root) ;

	void makeEmpty( );
	bool insert( const Comparable & x );
	void remove( const Comparable & x );

	static const int ALLOWED_IMBALANCE = 1;
	void balance( AvlNode<Comparable> * & t)
	{
		if( t == nullptr )
			return;

		if( height( t->left ) - height( t->right ) > ALLOWED_IMBALANCE )
			if( height( t->left->left ) >= height( t->left->right ) )
				rotateWithLeftChild( t );
			else
				doubleWithLeftChild( t );
		else
			if( height( t->right ) - height( t->left ) > ALLOWED_IMBALANCE )
				if( height( t->right->right ) >= height( t->right->left ) )
					rotateWithRightChild( t );
				else
					doubleWithRightChild( t );

		t->height = max( height( t->left ), height( t->right ) ) + 1;
	}

	const AvlTree & operator=( const AvlTree & rhs );
	AvlNode<Comparable> *root;
	const Comparable ITEM_NOT_FOUND;

	const Comparable & elementAt( AvlNode<Comparable> *t ) const;

	bool insert( const Comparable & x, AvlNode<Comparable> * & t ) const;
	void remove(const Comparable & x, AvlNode<Comparable> * & t );

	AvlNode<Comparable> * findMin( AvlNode<Comparable> *t ) const;
	AvlNode<Comparable> * findMax( AvlNode<Comparable> *t ) const;
	AvlNode<Comparable> * find( const Comparable & x, AvlNode<Comparable> *t, vector<Comparable> &vec ) 
		const;
	void makeEmpty( AvlNode<Comparable> * & t ) const;
	void printTree( AvlNode<Comparable> *t ) const;
	AvlNode<Comparable> * clone( AvlNode<Comparable> *t ) const;

	// Avl manipulations
	int height( AvlNode<Comparable> *t ) const;
	int max( int lhs, int rhs ) const;
	void rotateWithLeftChild( AvlNode<Comparable> * & k2 ) const;
	void rotateWithRightChild( AvlNode<Comparable> * & k1 ) const;
	void doubleWithLeftChild( AvlNode<Comparable> * & k3 ) const;
	void doubleWithRightChild( AvlNode<Comparable> * & k1 ) const;
};

template <class Comparable>
AvlTree<Comparable>::
	AvlTree(const Comparable & notFound ) :
	ITEM_NOT_FOUND( notFound ), root( NULL )
{
}

template <class Comparable>
AvlTree<Comparable>::~AvlTree( ){
	makeEmpty();
}

template <class Comparable>
void AvlTree<Comparable>::
	makeEmpty( AvlNode<Comparable> * & t ) const
{
	if( t != NULL )
	{
		makeEmpty( t->left );
		makeEmpty( t->right );
		delete t;
	}
	t = NULL;
}

template <class Comparable>
void AvlTree<Comparable>::makeEmpty( )
{
	makeEmpty( root );
}


template <class Comparable>
int AvlTree<Comparable>::height( AvlNode<Comparable> *t ) const
{
	if (t == NULL)
		return -1;

	return t->height;
}

template <class Comparable>
int AvlTree<Comparable>::max( int lhs, int rhs ) const
{
	if (lhs > rhs)
		return lhs;

	return rhs;            
}

template <class Comparable>
bool AvlTree<Comparable>::insert
	( const Comparable & x, AvlNode<Comparable> * & t ) const
{
	if ( t == NULL ) {
		t = new AvlNode<Comparable>( x, NULL, NULL );	return true;
	}
	else if ( x < t->element ) {
		// X should be inserted to the left tree!
		insert( x, t->left );
		// Check if the left tree is out of balance (left subtree grew in height!)
		if ( height( t->left ) - height( t->right ) == 2 )
			if ( x < t->left->element )  // X was inserted to the left-left subtree!
				rotateWithLeftChild( t );
			else			     // X was inserted to the left-right subtree!
				doubleWithLeftChild( t );
	}
	else if( t->element < x )
	{    // Otherwise X is inserted to the right subtree
		insert( x, t->right );
		if ( height( t->right ) - height( t->left ) == 2 )
			// height of the right subtree increased
				if ( t->right->element < x )
					// X was inserted to right-right subtree
						rotateWithRightChild( t );
				else // X was inserted to right-left subtree
					doubleWithRightChild( t );
	}
	else
		return false;  // Duplicate; do nothing

	// update the height the node
	t->height = max( height( t->left ), height( t->right ) ) + 1;
}

template <class Comparable>
bool AvlTree<Comparable>::insert( const Comparable & x )
{
	return insert( x, root );
}


template <class Comparable>
void AvlTree<Comparable>::rotateWithLeftChild( 
	AvlNode<Comparable> * & k2 ) const
{
	AvlNode<Comparable> *k1 = k2->left;
	k2->left = k1->right;
	k1->right = k2;
	k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
	k1->height = max( height( k1->left ), k2->height ) + 1;
	k2 = k1;
}
template <class Comparable>
void AvlTree<Comparable>::rotateWithRightChild
	( AvlNode<Comparable> * & k1 ) const
{
	AvlNode<Comparable> *k2 = k1->right;
	k1->right = k2->left;
	k2->left = k1;
	k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
	k2->height = max( height( k2->right ), k1->height ) + 1;
	k1 = k2;
}
template <class Comparable>
void AvlTree<Comparable>::doubleWithLeftChild
	( AvlNode<Comparable> * & k3 ) const
{
	rotateWithRightChild( k3->left );
	rotateWithLeftChild( k3 );
}
template <class Comparable>
void AvlTree<Comparable>::doubleWithRightChild
	( AvlNode<Comparable> * & k1 ) const
{
	rotateWithLeftChild( k1->right );
	rotateWithRightChild( k1 );
}


template <class Comparable>
void AvlTree<Comparable>::remove(const Comparable & x, AvlNode<Comparable> * & t )
{
	if( t == nullptr ) return; // Item not found; do nothing

	if( x < t->element )
		remove( x, t->left );
	else if( t->element < x )
		remove( x, t->right );
	else if( t->left != nullptr && t->right != nullptr ) // Two children
	{
		t->element = findMin( t->right )->element;
		remove( t->element, t->right );
	}
	else
	{
		AvlNode<Comparable> *oldNode = t;
		t = ( t->left != nullptr ) ? t->left : t->right;
		delete oldNode;
	}

	balance( t );
}

template <class Comparable>
void AvlTree<Comparable>::remove
	( const Comparable & x )
{
	remove( x, root );
}

template <class Comparable>
void AvlTree<Comparable>::
	printTree( BinaryNode<Comparable> * &t ) const
{
	if ( t != NULL )
	{
		printTree( t->left );
		cout << t->element << endl;
		printTree( t->right );
	}
}

template <class Comparable>
void AvlTree<Comparable>::printTree( ) const
{
	if( isEmpty( ) )
		cout << "Empty tree" << endl;
	else
		printTree( root );
}

template <class Comparable>
void AvlTree<Comparable>:: printInorderAVL(AvlNode<Comparable> *node, ofstream & file) 
{ 
	if (node == NULL) 
		return; 


	printInorderAVL(node->left, file); 

	file<<node->element;

	printInorderAVL(node->right, file); 
} 

template <class Comparable>
void AvlTree<Comparable>:: printPreorderAVL(AvlNode<Comparable> *node, ofstream &file) 
{ 
	if (node == NULL) 
		return; 

	file<<node->element;

	printPreorderAVL(node->left, file);  

	printPreorderAVL(node->right, file); 
} 

template <class Comparable>
const Comparable & AvlTree<Comparable>::
	elementAt( AvlNode<Comparable> *t ) const
{
	return t == NULL ? ITEM_NOT_FOUND : t->element;
}

template <class Comparable>
const Comparable & AvlTree<Comparable>::
	find( const Comparable & x, vector<Comparable> &vec ) const
{
	return elementAt( find( x, root, vec ) );
}

template <class Comparable>
AvlNode<Comparable> *
	AvlTree<Comparable>::
	find( const Comparable & x, AvlNode<Comparable> * t, vector<Comparable> &vec ) const
{
	if ( t == NULL )
		return NULL;
	else if( x < t->element )
		return find( x, t->left, vec );
	else if( t->element < x )
		return find( x, t->right, vec );
	else if (t->element == x){ //*
		//cout<<t->element;
		vec.push_back(t->element);
		find( x, t->left, vec );
		find( x, t->right, vec );	}
}

template <class Comparable>
int AvlTree<Comparable>::maxDepth(AvlNode<Comparable> *root) 
{
	if (root == NULL) {
		return 0;
	}
	return 1 + 
		std::max(maxDepth(root->left), maxDepth(root->right));
}

template <class Comparable>
int AvlTree<Comparable>::minDepth(AvlNode<Comparable> *root) 
{
	if (root == NULL) {
		return 0;
	}
	return 1 + 
		std::min(minDepth(root->left), minDepth(root->right));
}

template <class Comparable>
bool AvlTree<Comparable>:: isBalanced(AvlNode<Comparable> *root) 
{
	return (maxDepth(root) - minDepth(root) <= 1);
}

template <class Comparable>
const Comparable & AvlTree<Comparable>::findMin( ) const
{
	return elementAt( findMin( root ) );
}

template <class Comparable>
AvlNode<Comparable> *
	AvlTree<Comparable>::findMin( AvlNode<Comparable> *t ) 
	const
{
	if( t == NULL )
		return NULL;
	if( t->left == NULL )
		return t;
	return findMin( t->left );
}

template <class Comparable>
const Comparable & AvlTree<Comparable>::findMax( ) const
{
	return elementAt( findMax( root ) );
}

template <class Comparable>
AvlNode<Comparable> *
	AvlTree<Comparable>::findMax( AvlNode<Comparable> *t ) 
	const
{
	if( t != NULL )
		while( t->right != NULL )
			t = t->right;
	return t;
}

template <class Comparable>
AvlTree<Comparable>::
	AvlTree( const AvlTree<Comparable> & rhs ) :
	root( NULL ), ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND )
{ 
	*this = rhs;
}

template <class Comparable>
AvlNode<Comparable> *
	AvlTree<Comparable>::
	clone( AvlNode<Comparable> * t ) const
{
	if ( t == NULL )
		return NULL;
	else
		return new AvlNode<Comparable> 
		( t->element, 
		clone( t->left ), 
		clone( t->right ) );
}

