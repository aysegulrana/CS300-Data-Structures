#include <iostream>
#include <vector>
#include <string>
#include <exception>

using namespace std;

template <class Comparable>
class BinaryHeap
{
public:
	BinaryHeap( int capacity = 100){ currentSize=0; array.resize(10000);}; 

	void buildHeap(vector<Comparable> Input);
	bool isEmpty() const;
	bool isFull() const;
	const Comparable & findTop( ) const;

	virtual void insert( const Comparable & x ){};
	void deleteTop( ); //i changed it as deleteTop because it'll be inherited for both max and min
	void makeEmpty( );

protected:
	int currentSize; //Number of elements in heap
	vector<Comparable> array; //The heap array

	void buildHeap();
	virtual void percolateDown(int hole){};
};

template <class Comparable>
bool BinaryHeap<Comparable>::isFull( )const
{
	if(currentSize==array.capacity())
		return true;
	return false;
}

template <class Comparable>
bool BinaryHeap<Comparable>::isEmpty( )const
{
	if(currentSize==0)
		return true;
	return false;
}

template <class Comparable>
const Comparable & BinaryHeap<Comparable>::findTop( ) const{ 
	return array[1];
}

template <class Comparable>
void BinaryHeap<Comparable>::deleteTop()
{
	if ( isEmpty( ) )
		cout<<"Underflow error."<<endl;

	array[1] = array[currentSize--];
	percolateDown(1);
}

template <class Comparable>
void BinaryHeap<Comparable>::buildHeap( 
									   vector<Comparable> Input)
{
	array = Input; //copy input array to private array;
	currentSize = Input.size();
	for ( int i = currentSize / 2; i > 0; i–– )
		percolateDown( i );
}

template <class Comparable>
class minHeap: public BinaryHeap<Comparable> {
	//all operations are already for min
public: 
	void insert( const Comparable & x);
private:
	void percolateDown( int hole );
};

template <class Comparable>
void minHeap<Comparable>::insert( const Comparable & x )
{
	if ( isFull( ) )
		cout<<"Overflow error."<<endl;
	//Percolate up
	//Note that instead of swapping we move the hole up

	currentSize++;
	int hole = currentSize;
	for (  ; hole > 1 && x < array[ hole / 2 ]; hole /= 2 )
		array[ hole ] = array[ hole / 2 ];
	array[ hole ] = x;
}

template <class Comparable>
void minHeap<Comparable>::percolateDown( int hole )
{
	int child;
	Comparable tmp = array[ hole ];  //tmp is the item that will 
	//be percolated down
	for( ; hole * 2 <= currentSize; hole = child )
	{
		child = hole * 2;
		if ( child != currentSize && array[ child + 1 ] < array[ child ] )
			child++;
		if ( array[ child ] < tmp )
			array[ hole ] = array[ child ];
		else
			break;
	}
	array[hole] = tmp;
}

template <class Comparable>
class maxHeap: public BinaryHeap<Comparable> {
public:
	void insert( const Comparable & x );

private:
	void percolateDown( int hole ); 
};

template <class Comparable>
void maxHeap<Comparable>::percolateDown( int hole )
{
	int child;
	Comparable tmp = array[hole];  //tmp is the item that will 
	//be percolated down
	for( ; hole* 2 <= currentSize; hole = child )
	{
		child = hole * 2;
		if ( child != currentSize && array[ child + 1 ] > array[ child ] )
			child++;
		if ( array[ child ] > tmp )
			array[ hole ] = array[ child ];
		else
			break;
	}
	array[hole] = tmp;
}

template <class Comparable>
void maxHeap<Comparable>::insert( const Comparable & x )
{
	if ( isFull( ) )
		cout<<"Overflow error."<<endl;
	//Percolate up
	//Note that instead of swapping we move the hole up
	int hole = ++currentSize;
	for (  ; hole > 1 && x > array[ hole / 2 ]; hole /= 2 )
		array[ hole ] = array[ hole / 2 ];
	array[ hole ] = x;
}