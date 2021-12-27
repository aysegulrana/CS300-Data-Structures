#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

template <class Comparable>
class BinarySearchTree;

template <class Comparable>
class BinaryNode
{
public:
	Comparable element;
	BinaryNode  *left;
	BinaryNode  *right;
	int height;
	BinaryNode( const Comparable & theElement, 
		BinaryNode *lt, BinaryNode *rt, int h = 0 )
		: element( theElement ), left( lt ), right( rt ), height( h ) { }

	friend class BinarySearchTree<Comparable>;
};

template <class Comparable>
class BinarySearchTree
{
public:
	explicit BinarySearchTree( const Comparable & notFound );
	BinarySearchTree( const BinarySearchTree & rhs );
	~BinarySearchTree( );

	const Comparable & findMin( ) const;
	const Comparable & findMax( ) const;
	const Comparable & find( const Comparable & x, vector<Comparable> &vec ) const;
	bool isEmpty( ) const;
	void printTree( ) const;

	void makeEmpty( );
	bool insert( const Comparable & x );
	void remove( const Comparable & x );
	BinaryNode<Comparable> *root;
	const BinarySearchTree & operator=( const BinarySearchTree & rhs );

	bool isBalanced(BinaryNode<Comparable> *root);
	void printInorderBST(BinaryNode<Comparable> *node, ofstream & file);
	void printPreorderBST(BinaryNode<Comparable> *node, ofstream & file) ;
	int maxDepth(BinaryNode<Comparable> *root) ;
	int minDepth(BinaryNode<Comparable> *root);

	int height( BinaryNode<Comparable> *t ) const;

	friend class BinaryNode<Comparable>;
private:

	const Comparable ITEM_NOT_FOUND;

	const Comparable & elementAt( BinaryNode<Comparable> *t ) const;

	bool insert( const Comparable & x, BinaryNode<Comparable> * & t ) const;
	void remove( const Comparable & x, BinaryNode<Comparable> * & t ) const;
	BinaryNode<Comparable> * findMin( BinaryNode<Comparable> *t ) const;
	BinaryNode<Comparable> * findMax( BinaryNode<Comparable> *t ) const;
	BinaryNode<Comparable> * find( const Comparable & x, 
		BinaryNode<Comparable> *t, vector<Comparable> &vec ) const;
	void makeEmpty( BinaryNode<Comparable> * & t ) const;
	void printTree( BinaryNode<Comparable> *t ) const;
	BinaryNode<Comparable> * clone( BinaryNode<Comparable> *t ) const;


};

template <class Comparable>
BinarySearchTree<Comparable>::
	BinarySearchTree(const Comparable & notFound ) :
	ITEM_NOT_FOUND( notFound ), root( NULL )
{
}

template <class Comparable>
BinarySearchTree<Comparable>::~BinarySearchTree( ){
	makeEmpty();
}
template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::
	elementAt( BinaryNode<Comparable> *t ) const
{
	return t == NULL ? ITEM_NOT_FOUND : t->element;
}

template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::
	find( const Comparable & x, vector<Comparable> &vec ) const
{
	return elementAt( find( x, root, vec ) );
}

template <class Comparable>
BinaryNode<Comparable> *
	BinarySearchTree<Comparable>::
	find( const Comparable & x, BinaryNode<Comparable> * t, vector<Comparable> &vec ) const
{
	if ( t == NULL )
		return NULL;
	else if( x < t->element )
		return find( x, t->left, vec );
	else if( t->element < x )
		return find( x, t->right, vec );
	else if (t->element == x) {
		//cout<<t->element;
		vec.push_back(t->element);
		find( x, t->left, vec );
		find( x, t->right, vec );	}
}

template <class Comparable>
int BinarySearchTree<Comparable>::height( BinaryNode<Comparable> *t ) const
{
	if (t == NULL)
		return -1;

	return t->height;
}

template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::findMin( ) const
{
	return elementAt( findMin( root ) );
}

template <class Comparable>
BinaryNode<Comparable> *
	BinarySearchTree<Comparable>::findMin( BinaryNode<Comparable> *t ) 
	const
{
	if( t == NULL )
		return NULL;
	if( t->left == NULL )
		return t;
	return findMin( t->left );
}

template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::findMax( ) const
{
	return elementAt( findMax( root ) );
}

template <class Comparable>
BinaryNode<Comparable> *
	BinarySearchTree<Comparable>::findMax( BinaryNode<Comparable> *t ) 
	const
{
	if( t != NULL )
		while( t->right != NULL )
			t = t->right;
	return t;
}

template <class Comparable>
bool BinarySearchTree<Comparable>::
	insert( const Comparable & x, BinaryNode<Comparable> * & t ) const
{
	if( t == NULL ){ //  create a new node at the right place
		t = new BinaryNode<Comparable>( x, NULL, NULL );
		return true;}
	else if( x < t->element )
		insert( x, t->left );  // insert at the left or 
	else if( t->element < x )
		insert( x, t->right );  // right subtree
	else 
		return false;  // Duplicate
	t->height = max( height( t->left ), height( t->right ) ) + 1;
}

template <class Comparable>
bool BinarySearchTree<Comparable>::insert( const Comparable & x )
{
	return insert( x, root );
}

template <class Comparable>
void BinarySearchTree<Comparable>::
	remove( const Comparable & x, BinaryNode<Comparable> * & t ) 
	const
{
	if( t == NULL )
		cout<<"Item not found";
	return;   // Item not found; do nothing
	if( x < t->element )
		remove( x, t->left );
	else if( t->element < x )
		remove( x, t->right );
	else if( t->left != NULL && t->right != NULL ) // Two children
	{
		t->element = findMin( t->right )->element;
		remove( t->element, t->right );
	}
	else // one or no children
	{
		BinaryNode<Comparable> *oldNode = t;
		t = ( t->left != NULL ) ? t->left : t->right;
		delete oldNode;
	}
}            

template <class Comparable>
void BinarySearchTree<Comparable>::remove
	( const Comparable & x )
{
	remove( x, root );
}

template <class Comparable>
void BinarySearchTree<Comparable>::
	makeEmpty( BinaryNode<Comparable> * & t ) const
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
void BinarySearchTree<Comparable>::makeEmpty( )
{
	makeEmpty( root );
}

template <class Comparable>
void BinarySearchTree<Comparable>::
	printTree( BinaryNode<Comparable> * t ) const
{
	if ( t != NULL )
	{
		printTree( t->left );
		cout << t->element << endl;
		printTree( t->right );
	}
}

template <class Comparable>
void BinarySearchTree<Comparable>::printTree( ) const
{
	if( isEmpty( ) )
		cout << "Empty tree" << endl;
	else
		printTree( root );
}

template <class Comparable>
BinarySearchTree<Comparable>::
	BinarySearchTree( const BinarySearchTree<Comparable> & rhs ) :
	root( NULL ), ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND )
{ 
	*this = rhs;
}

template <class Comparable>
BinaryNode<Comparable> *
	BinarySearchTree<Comparable>::
	clone( BinaryNode<Comparable> * t ) const
{
	if ( t == NULL )
		return NULL;
	else
		return new BinaryNode<Comparable> 
		( t->element, 
		clone( t->left ), 
		clone( t->right ) );
}

template <class Comparable>
int BinarySearchTree<Comparable>::maxDepth(BinaryNode<Comparable> *root) 
{
	if (root == NULL) {
		return 0;
	}
	return 1 + 
		std::max(maxDepth(root->left), maxDepth(root->right));
}

template <class Comparable>
int BinarySearchTree<Comparable>::minDepth(BinaryNode<Comparable> *root) 
{
	if (root == NULL) {
		return 0;
	}
	return 1 + 
		std::min(minDepth(root->left), minDepth(root->right));
}

template <class Comparable>
bool BinarySearchTree<Comparable>:: isBalanced(BinaryNode<Comparable> *root) 
{
	return (maxDepth(root) - minDepth(root) <= 1);
}

template <class Comparable>
void BinarySearchTree<Comparable>:: printInorderBST(BinaryNode<Comparable> *node, ofstream & file) 
{ 
	if (node == NULL) 
		return; 

	printInorderBST(node->left, file); 

	file<<node->element;

	printInorderBST(node->right, file); 
} 

template <class Comparable>
void BinarySearchTree<Comparable>:: printPreorderBST(BinaryNode<Comparable> *node, ofstream & file) 
{ 
	if (node == NULL) 
		return; 

	file<<node->element;

	printPreorderBST(node->left, file);  

	printPreorderBST(node->right, file); 
} 

