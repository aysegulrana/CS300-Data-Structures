#include <iostream>
#include <string>
using namespace std;

template <class Object>
class Stack
{
public:
	Stack( );
	Stack( const Stack<Object> & rhs ); //changed
	~Stack( );

	bool isEmpty( ) const;
	bool isFull( ) const;
	void makeEmpty( );

	void pop( );
	void push( const Object & x );
	Object topAndPop( );
	const Object & top( ) const;

	const Stack & operator=( const Stack<Object> & rhs ); //changed

private:
	struct ListNode
	{
		Object   element;
		ListNode *next;

		ListNode( const Object & theElement, ListNode * n = NULL )
			: element( theElement ), next( n ) { }
	};

	ListNode *topOfStack;  // list itself is the stack
};
template <class Object>
Stack<Object>::Stack( const Stack<Object> & rhs )
{
	topOfStack = NULL;
	*this = rhs; // deep copy 
}

template <class Object>
Stack<Object>::~Stack( )
{
	makeEmpty( );
}

template <class Object>
Stack<Object>::Stack( )
{
	topOfStack = NULL;
}

template <class Object>
bool Stack<Object>::isFull( ) const
{
	return false;
}

template <class Object>
bool Stack<Object>::isEmpty( ) const
{
	return topOfStack == NULL;
}

template <class Object>
const Object & Stack<Object>::top( ) const
{
	if ( !isEmpty( ) )
		//throw Underflow( );
			return topOfStack->element;
	
}

template <class Object>
void Stack<Object>::pop( ) //const
{
	if (!isEmpty( )){
		//throw Underflow( );
		ListNode *oldTop = topOfStack;
		topOfStack = topOfStack->next;
		delete oldTop;
	}
}

template <class Object>
void Stack<Object>::push( const Object & x )
{
	topOfStack = new ListNode( x, topOfStack );
}

template <class Object>
Object Stack<Object>::topAndPop( )
{
	Object topItem = top( );
	pop( );
	return topItem;
}

template <class Object>
void Stack<Object>::makeEmpty( )
{
	while ( ! isEmpty( ) )
		pop( );
}

template <class Object>
const Stack<Object> & Stack<Object>::
	operator=( const Stack<Object> & rhs )
{
	if ( this != &rhs )
	{
		makeEmpty( );
		if ( rhs.isEmpty( ) )
			return *this;

		ListNode *rptr = rhs.topOfStack;
		ListNode *ptr  = new ListNode( rptr->element );
		topOfStack = ptr;

		for ( rptr = rptr->next; rptr != NULL; rptr = rptr->next )
			ptr = ptr->next = new ListNode( rptr->element );
	}
	return *this;
}

