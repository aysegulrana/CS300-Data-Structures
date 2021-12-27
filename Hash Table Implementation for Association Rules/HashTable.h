#include <iostream>
#include <string>
#include <list>
#include <array>
#include <vector>
#include <exception>
using namespace std;

template <class Object>
class List; // Incomplete declaration.

template <class Object>
class ListItr; // Incomplete declaration.

template <class Object>
class ListNode
{
	ListNode( const Object & theElement = Object( ), ListNode * n = NULL )

		: element( theElement ), next( n ) { } // Constructor
	Object element;

	ListNode *next;
public: 	
	Object getElement();
	ListNode<Object>* getNext();
	friend class List<Object>;
	friend class ListItr<Object>;
};
template <class Object>
Object ListNode<Object>::getElement(){
	return element;
}

template <class Object>
ListNode<Object>* ListNode<Object>::getNext(){
	return next;
}

template <class Object>
class ListItr
{
public:
	ListItr( ) : current( NULL ) { }
	bool isPastEnd( ) const
	{   return current == NULL;   }
	void advance( )
	{   if ( ! isPastEnd( ) )   current = current->next;   }
	Object & retrieve( ) const
	{  
		if ( isPastEnd( ) )  {  
			Object i;
			return i; }
		return current->element;    }

private:
	// Current position
	ListNode<Object> *current; 
	ListItr( ListNode<Object> *theNode )
		: current( theNode ) { }

	friend class List<Object>;    // Grant access to constructor
};

template <class Object>
class List
{
public:
	List( );
	List( const List & rhs );
	~List( );
	bool isEmpty( ) const;
	void makeEmpty( );
	ListItr<Object> zeroth( ) const;
	ListItr<Object> first( ) const;
	void insert( const Object & x, const ListItr<Object> & p );
	ListItr<Object> find( const Object & x ) const;
	ListItr<Object> findPrevious( const Object & x ) const;
	void remove( const Object & x );

	const List & operator=( const List & rhs );
	ListNode<Object>* getHeader();

private:
	ListNode<Object> *header;
};

template <class Object>
List<Object>::List( )
{
	header = new ListNode<Object>;
}

template <class Object>
bool List<Object>::isEmpty( ) const
{    
	// see if the header points to NULL 
	return this->header->next== NULL;
}

template <class Object>
ListNode<Object>* List<Object>::getHeader(){
	return header;
}

template <class Object>
void List<Object>::insert( const Object & x, const ListItr<Object> & p )
{
	if ( p.current != NULL )
		p.current->next = new ListNode<Object>( x, p.current->next );
}

template <class Object>
ListItr<Object> List<Object>::find( const Object & x ) const
{
	ListNode<Object> *itr = header->next; // Initialize

	while ( itr != NULL && itr->element != x )
		itr = itr->next;

	return ListItr<Object>( itr );
}

template <class Object>
ListItr<Object> List<Object>::findPrevious( const Object & x ) const
{
	ListNode<Object> *itr = header;

	while ( itr->next != NULL && itr->next->element != x )
		itr = itr->next;

	return ListItr<Object>( itr );
}

template <class Object>
void List<Object>::remove( const Object & x )
{
	ListItr<Object> p = findPrevious( x ); // Locate previous of x, if any

	if ( p.current->next != NULL )
	{
		ListNode<Object> *oldNode = p.current->next;
		p.current->next = p.current->next->next;  // Bypass deleted node
		delete oldNode;
	}
}

template <class Object>
ListItr<Object> List<Object>::zeroth( ) const
{
	return ListItr<Object>( header );
}

template <class Object>
ListItr<Object> List<Object>::first( ) const
{
	return ListItr<Object>( header->next );
}

template <class Object>
void List<Object>::makeEmpty( )
{
	while ( ! isEmpty( ) )
		remove( first( ).retrieve( ) );
}

template <class Object>
const List<Object> & List<Object>::operator=( const List<Object> & rhs )
{
	if ( this != &rhs )
	{
		makeEmpty( );

		ListItr<Object> ritr = rhs.first( );
		ListItr<Object> itr = zeroth( );
		for (  ;  ! ritr.isPastEnd( ); ritr.advance( ), itr.advance( ) )
			insert( ritr.retrieve( ), itr );
	}
	return *this;
}

template <class Object>
List<Object>::List( const List<Object> & rhs )
{
	header = new ListNode<Object>;
	*this = rhs;  // Deep Copy
}

template <class Object>
List<Object>::~List( )
{
	makeEmpty( );  // Get rid of all list nodes
	delete header;  // then get rid of the header
}

template <class HashedObj>
class HashTable
{
public:
	HashTable( const HashedObj & notFound, 
		int size = 101 );
	HashTable( const HashTable & rhs )
		: ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ), 
		theLists( rhs.theLists ) { }
	~HashTable();
	const HashedObj & find( const HashedObj & x ) const;

	void makeEmpty( );
	void insert( const HashedObj & x );
	void remove( const HashedObj & x );



	bool contains( const HashedObj & x ) const;
	int findPos( const HashedObj & x ) const;

	void traverseTable(vector<HashedObj> & vec) const;

	const HashTable & operator=( const HashTable & rhs);
	enum EntryType { ACTIVE, EMPTY, DELETED };
	vector<List<HashedObj>> theLists; 
private:
	int currentSize;
	const HashedObj ITEM_NOT_FOUND;

	bool isActive( int currentPos ) const;
	void rehash( );

	bool isPrime( int n )
	{
		if ( n == 2 || n == 3 )
			return true;

		if ( n == 1 || n % 2 == 0 )
			return false;

		for ( int i = 3; i * i <= n; i += 2 )
			if ( n % i == 0 )
				return false;

		return true;
	}
	int nextPrime( int n )
	{
		if ( n % 2 == 0 )
			n++;
		for ( ; ! isPrime( n ); n += 2 )
			;
		return n;
	}
};

template <class HashedObj>
HashTable<HashedObj>::HashTable( 
	const HashedObj & notFound, int size )
	: ITEM_NOT_FOUND( notFound ), theLists( nextPrime( size ) )
{
}

template <class HashedObj>
HashTable<HashedObj>::~HashTable( )
{
	makeEmpty();
}

template <class HashedObj>
void HashTable<HashedObj>::makeEmpty( )
{
	for( auto & thisList : theLists )
		thisList.makeEmpty();
	//destroy the lists but not the vector!
}

template <class HashedObj>
void HashTable<HashedObj>::insert( const HashedObj & x )
{
	HashedObj access=x;
	// hash the given object and locate the list it should be on 
	List<HashedObj> & whichList = theLists[ access.hash(x, theLists.size( ) ) ];
	// locate the object in the list (using List’s find)
	ListItr<HashedObj> itr = whichList.find( x );
	// insert the new item at the head of the list if not found!
	if ( itr.isPastEnd( ) )
		whichList.insert( x, whichList.zeroth( ) );
	else
		itr.retrieve()=x; //updated element is replaced with the old
}

template <class HashedObj>
bool HashTable<HashedObj>::contains( const HashedObj & x ) const
{
	HashedObj access=x;
	auto & whichList = theLists[ access.hash(x, currentSize)];
	return find( begin( whichList ), end( whichList ), x ) != end( whichList );
}

template <class HashedObj>
void HashTable<HashedObj>::remove( const HashedObj & x )
{
	HashedObj access=x;
	// remove from the appropriate list
	theLists[access.hash(x, theLists.size( ) ) ].remove( x );
}

template <class HashedObj>
const HashedObj & HashTable<HashedObj>::
	find( const HashedObj & x ) const
{
	HashedObj access=x;
	ListItr<HashedObj> itr;
	// locate the approriate list and search there
	itr = theLists[ access.hash(x, theLists.size( )) ].find( x );
	// retrieve from the located position
	if (itr.isPastEnd())
		return ITEM_NOT_FOUND;

	return itr.retrieve( );
}

template <class HashedObj>
void HashTable<HashedObj>::rehash( )
{
	vector<list<HashedObj>> oldLists = theLists;

	// Create new double-sized, empty table
	theLists.resize( nextPrime( 2 * theLists.size( ) ) );
	for( auto & thisList : theLists )
		thisList.clear( );

	// Copy table over
	currentSize = 0;
	for( auto & thisList : oldLists )
		for( auto & x : thisList )
			insert( std::move( x ) );
}

template <class HashedObj>
bool HashTable<HashedObj>::isActive( int currentPos ) const
{
	return array[ currentPos ].info == ACTIVE;
}

template <class HashedObj>
int HashTable<HashedObj>::findPos( const HashedObj & x ) const
{
	HashedObj access=x;
	int collisionNum = 0;
	int currentPos = access.hash(x, array.size( ) );

	while ( array[currentPos].info != EMPTY && 
		array[ currentPos ].element != x )
	{
		currentPos += 2 * ++collisionNum - 1;  // add the difference
		if ( currentPos >= array.size( ) )             // perform the mod
			currentPos –= array.size( );               // if necessary
	}
	return currentPos;
}

template <class HashedObj>
void HashTable<HashedObj>::traverseTable(vector<HashedObj> & vec) const
{

	List<HashedObj> whichList;
	ListNode<HashedObj> *ptr=NULL;
	HashedObj currentobj;
	for (int i=0; i<theLists.size(); i++){
		if(!theLists[i].isEmpty()){
			whichList = theLists[i];
			ptr=(whichList.getHeader()->getNext());
			while (ptr!=NULL)
			{	
				vec.push_back(ptr->getElement());
				ptr=ptr->getNext();
			}
		}
	}
}

