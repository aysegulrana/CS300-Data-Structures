#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include "AvlTree.h"
#include "BinarySearchTree.h"

using namespace std;
struct Contact {
	string fullName;
	string firstName;
	string lastName;
	string phoneNumber;
	string city; //operator overload
	const bool& operator ==(const Contact &rhs){ return this->fullName.substr(0, rhs.fullName.size())==rhs.fullName;}
};
//in comparisons we will compare fullnames. and if it's a partial string in ==, we will look at the other one partially too.
bool operator<(const Contact &lhs, const Contact& rhs) { return lhs.fullName<rhs.fullName.substr(0, lhs.fullName.size()); }
bool operator<=(const Contact &lhs, const Contact& rhs) { return lhs.fullName<=rhs.fullName.substr(0, lhs.fullName.size()); }
bool operator>=(const Contact &lhs, const Contact& rhs) { return lhs.fullName>=rhs.fullName.substr(0, lhs.fullName.size()); }
bool operator>(const Contact &lhs, const Contact& rhs) { return lhs.fullName>rhs.fullName.substr(0, lhs.fullName.size()); }
bool operator ==(const Contact &lhs, string rhs){ return lhs.fullName.substr(0, rhs.size())==rhs;}
bool operator !=(const Contact &lhs, string rhs){ return lhs.fullName.substr(0, rhs.size())!=rhs;}
ostream &operator << (ostream& os, Contact &rhs){ //when we want to print a contact, it'll automatically print the objects of the struct
	os<< rhs.fullName <<" "<< rhs.phoneNumber <<" "<<rhs.city<<endl;
	return os;
}

template <typename Comparable>
vector<int> binarySearch(vector<Comparable> &arr, int l, int r, Comparable &x, vector<int> &found) 
{ 
	if (r >= l) { 
		int mid = l + (r - l) / 2; 
		if (arr[mid] == x) {
			found.push_back(mid); //we need to search around with for loops
			for (int i=1; arr[mid+i] == x && i<arr.size(); i++)
				found.push_back(mid+i);
			for (int j=1; arr[mid-j] == x && mid-j>0; j++)
				found.push_back(mid-j);
			return found;} 
		else if (arr[mid] > x) 
			return binarySearch(arr, l, mid - 1, x, found); 

		else return binarySearch(arr, mid + 1, r, x, found); 
	} 
	return found; 
} 

template <typename Comparable>
inline void swap( Comparable & obj1, 
				 Comparable & obj2 )
{
	Comparable tmp = obj1;
	obj1 = obj2;
	obj2 = tmp;
}

template <typename Comparable>
void insertionSort (vector <Comparable> & a)
{
	int j;
	// loop over the passes
	for (int p = 1;  p < a.size(); p++)
	{
		Comparable tmp = a[p];
		// loop over the elements
		for (j = p; j > 0 &&  tmp < a[j -1]; j--)
			a[j] = a[j -1];
		a[j] = tmp;
	}
}

inline int leftChild( int i )
{
	return 2*i+1;
}

template <typename Comparable>  // for deleteMax
// a is the array, i is the position to percolate down from
// n is the logical size of the array
void percDown( vector<Comparable> & a, int i, int n )
{
	int child;
	Comparable tmp;
	for (tmp=a[i] ; leftChild(i) < n; i = child )
	{
		child = leftChild(i);
		if ( child != n-1 && a[ child  ] < a[ child+1 ] )
			child++;
		if ( a[child ] > tmp )
			a[i] = a[ child ];
		else
			break;
	} 
	a[ i ] = tmp;
}

template <typename Comparable>
void heapsort(vector<Comparable> & a)
{
	// buildHeap
	for (int i = a.size()/2; i >=0; i--)
		percDown(a, i, a.size());

	// sort
	for (int j = a.size() -1; j >0; j--)
	{
		std::swap(a[0], a[j]); // swap max to the last pos.
		percDown(a, 0, j); // re-form the heap
	}
}

template <typename Comparable>
void merge(vector<Comparable> & arr, int start, int mid, int end) 
{ 
	int start2 = mid + 1; 
	if (arr[mid] <= arr[start2]) { 
		return; 
	} 
	while (start <= mid && start2 <= end) { 
		if (arr[start] <= arr[start2]) { 
			start++; 
		} 
		else { 
			Comparable value = arr[start2]; 
			int index = start2; 
			while (index != start) { 
				arr[index] = arr[index - 1]; 
				index--; 
			} 
			arr[start] = value; 
			// Update all the pointers 
			start++; 
			mid++; 
			start2++; 
		} 
	} 
} 
template <typename Comparable>
void mergeSort(vector<Comparable> & arr, int l, int r) 
{ 
	if (l < r) { 
		int m = l + (r - l) / 2; 
		mergeSort(arr, l, m); 
		mergeSort(arr, m + 1, r); 
		merge(arr, l, m, r); 
	} 
} 

template <typename Comparable>
void insertionSort( vector<Comparable> & a, int left, int right )
{
	for ( int p = left + 1; p <= right; p++ )
	{
		Comparable tmp = a[ p ];
		int j;

		for ( j = p; j > left && tmp < a[ j - 1 ]; j-- )
			a[ j ] = a[ j - 1 ];
		a[ j ] = tmp;
	}
}

template <typename Comparable>
const Comparable & median( vector<Comparable> & a, 
						  int left, int right )
{  
	Comparable tmp;
	int center = ( left + right ) / 2;
	if ( a[ center ] < a[ left ] ){
		std::swap(a[center], a[left]); }
	if ( a[ right ] < a[ left ] ){
		std::swap(a[right], a[left]); }
	if ( a[ right ] < a[ center ] ){
		std::swap(a[right], a[center]); }

	// Place pivot at position right - 1
	std::swap(a[center], a[right-1]);
	return a[ right - 1 ];
}

template <typename Comparable>
void quicksort( vector<Comparable> & a, int left, int right )
{
	if ( left + 10 <= right )
	{
		Comparable pivot = median( a, left, right );
		// Begin partitioning
		int i = left, j = right - 1;
		for ( ; ; )
		{
			while ( a[ ++i ] < pivot ) { }
			while ( pivot < a[ --j ] ) { }
			if ( i < j ){
				std::swap(a[i], a[j]); }
			else
				break;
		}
		std::swap(a[i], a[right-1]);
		// Restore pivot

		quicksort( a, left, i - 1 );       // Sort small elements
		quicksort( a, i + 1, right );    // Sort large elements
	}
	else  // Do an insertion sort on the subarray
		insertionSort( a, left, right );
}

template <typename Comparable>
void quicksort( vector<Comparable>  &a )
{
	quicksort( a, 0, a.size( ) - 1 );
}
void readingFromFile (string file, BinarySearchTree <Contact> &bstree, AvlTree <Contact> &avltree, 
					  vector<Contact> &quickvec, vector<Contact> &mergevec, 
					  vector<Contact> &heapvec,vector<Contact> &insertvec){
						  string line, firstName, lastName;
						  ifstream infile(file);

						  int bst_time=0, avl_time=0;

						  while (getline(infile, line))
						  {	
							  Contact record;
							  istringstream iss(line);
							  string name, surname, number, city;
							  iss>>name>>surname>>record.phoneNumber>>record.city;	

							  record.firstName=name;
							  record.lastName=surname;

							  record.fullName=record.firstName+" "+record.lastName;

							  bstree.insert(record);
							  avltree.insert(record);
							  quickvec.push_back(record);
							  mergevec.push_back(record);
							  insertvec.push_back(record);
							  heapvec.push_back(record);
						  }
}
template <typename T>
void sortingVectors(vector<Contact> &quickvec, vector<Contact> &mergevec, vector<Contact> &heapvec,
					vector<Contact> &insertvec, T &qtime, T &mtime, T &htime, T &itime){

						int N=100;
						std::chrono::time_point<std::chrono::high_resolution_clock> startTime1;
						std::chrono::time_point<std::chrono::high_resolution_clock> endTime1;
						std::chrono::time_point<std::chrono::high_resolution_clock> startTime2;
						std::chrono::time_point<std::chrono::high_resolution_clock> endTime2;
						std::chrono::time_point<std::chrono::high_resolution_clock> startTime3;
						std::chrono::time_point<std::chrono::high_resolution_clock> endTime3;
						std::chrono::time_point<std::chrono::high_resolution_clock> startTime4;
						std::chrono::time_point<std::chrono::high_resolution_clock> endTime4;

						cout<<endl<<"Sorting the vector copies" <<endl<<
							"======================================"<<endl;

						startTime1=std::chrono::high_resolution_clock::now();
						quicksort(quickvec);
						endTime1=std::chrono::high_resolution_clock::now();
						auto qutime=(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime1 - startTime1).count());
						qtime=qutime;

						startTime2=std::chrono::high_resolution_clock::now();
						mergeSort(mergevec, 0, mergevec.size()-1);
						endTime2=std::chrono::high_resolution_clock::now();
						auto metime=(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime2 - startTime2).count());
						mtime=metime;

						startTime3=std::chrono::high_resolution_clock::now();
						heapsort(heapvec);
						endTime3=std::chrono::high_resolution_clock::now();
						auto hetime=(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime3 - startTime3).count());
						htime=hetime;

						startTime4=std::chrono::high_resolution_clock::now();
						insertionSort(insertvec);
						endTime4=std::chrono::high_resolution_clock::now();
						auto intime=(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime4 - startTime4).count());
						itime=intime;

						cout<<"Quick Sort Time: "<<qtime<<" Nanoseconds"<<endl
							<<"Insertion Sort Time: "<<itime<<" Nanoseconds"<<endl
							<<"Merge Sort Time: "<<mtime<<" Nanoseconds"<<endl
							<<"Heap Sort Time: "<<htime<<" Nanoseconds"<<endl;
}

void searchingName(string word, vector<Contact> &sortedArray, BinarySearchTree <Contact> &bstree, AvlTree <Contact> &avltree,
				  long long int &btime, long long int &atime, long long int &vtime ){
					   Contact search; search.fullName=word;
					   vector<Contact> vecbst, vecavl;
					   int N=100;
					   std::chrono::time_point<std::chrono::high_resolution_clock> startTime1;
					   std::chrono::time_point<std::chrono::high_resolution_clock> endTime1;
					   std::chrono::time_point<std::chrono::high_resolution_clock> startTime2;
					   std::chrono::time_point<std::chrono::high_resolution_clock> endTime2;
					   std::chrono::time_point<std::chrono::high_resolution_clock> startTime3;
					   std::chrono::time_point<std::chrono::high_resolution_clock> endTime3;

					   
						cout<<endl<<"Searching for " <<search.fullName.substr(0, search.fullName.find(" "))<<endl<<
							"======================================";

					   startTime1=std::chrono::high_resolution_clock::now();
					   for (int i=0; i<N; i++)
						   bstree.find(search, vecbst);
					   endTime1=std::chrono::high_resolution_clock::now();
					   auto bstime=(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime1 - startTime1).count())/N;
					   btime=bstime;
					   startTime2=std::chrono::high_resolution_clock::now();
					   for (int i=0; i<N; i++)
						   avltree.find(search, vecavl);
					   endTime2=std::chrono::high_resolution_clock::now();
					   auto avltime=(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime2 - startTime2).count())/N;
					   atime=avltime;

					   vector<int> j;
					   startTime3=std::chrono::high_resolution_clock::now();
					   for (int i=0; i<N; i++)
						   binarySearch(sortedArray, 0, sortedArray.size()-1, search, j);
					   endTime3=std::chrono::high_resolution_clock::now();
					   auto vectime=(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime3 - startTime3).count())/N;
					   vtime=vectime;
					   cout<<endl<<"The search in BST took "<< 
						   bstime<<" Nanoseconds"<<endl;
					   cout<<"The search in AVL took "<< 
						   avltime<<" Nanoseconds"<<endl;
					   cout<<"Binary Search Time: "<<
						   vectime<<" Nanoseconds"<<endl;
}

void speedUpSearching (long long int &bstime, long long int &avltime, long long int &binarytime){
	cout<<endl<<"SpeedUps in search"<<endl<<"======================================"<<endl;
	cout<<"(BST / AVL) SpeedUp  = "<<(double)bstime/(double)avltime<<endl;
	cout<<"(Binary Search / AVL) SpeedUp  = "<<(double)binarytime/(double)avltime<<endl;
	cout<<"(Binary Search / BST) SpeedUp  = "<<(double)binarytime/(double)bstime<<endl;
}

void speedUpSorting (long long int &qtime, long long int &mtime, long long int &htime, long long int& itime){
	cout<<endl<<"SpeedUps between Sorting Algorithms"<<endl<<"======================================"<<endl;
	cout<<"(Insertion Sort/ Quick Sort) SpeedUp = "<<(double)itime/(double)qtime<<endl;
	cout<<"(Merge Sort / Quick Sort) SpeedUp = "<<(double)mtime/(double)qtime<<endl;
	cout<<"(Heap Sort / Quick Sort) SpeedUp = "<<(double)htime/(double)qtime<<endl;
}

int main (){

	string file, line, name;
	cout<<"Please enter the contact file name: ";
	cin>>file;
	cout<<"Please enter the word to be queried: ";
	cin>>name;

	Contact empty, empty2;
	vector<Contact> insertionvec, quickvec, mergevec, heapvec;
	BinarySearchTree <Contact> bstree(empty);
	AvlTree <Contact> avltree(empty2);
	long long int itime, qtime, mtime, htime, bstime, avltime, binarysearch;

	readingFromFile (file, bstree, avltree, quickvec, mergevec, heapvec, insertionvec);
	sortingVectors(quickvec, mergevec, heapvec, insertionvec, qtime, mtime, htime, itime);
	searchingName(name, quickvec, bstree, avltree, bstime, avltime, binarysearch);
	speedUpSearching(bstime, avltime, binarysearch);
	speedUpSorting(qtime, mtime, htime, itime);

	return 0;
}
