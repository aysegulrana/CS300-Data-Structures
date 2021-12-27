#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Heap.h"
#include "strutils.h"

using namespace std;

int main (){
	//generate max and min heaps  and a variable which is median.
	//in while loop get streaming numbers, in while loop first look if number is larger or smaller than median
	//if the heap you're inserting will exceed the +2 then move median to the other heap and make inserted number the new median.
	maxHeap<int> maxHeap;
	minHeap<int> minHeap;
	string number, file;	double median=0; int maxCount=0, minCount=0, exchange, num_count=0, temp, filecount=1;

	cout<<"Please enter the next filename that contains integer stream: ";
	while (getline(cin, file)){
		if(file=="z")
			break;
		ifstream infile(file);
		while(getline(infile, number)){	
			int num=atoi(number); num_count++;
			if(num_count==1){ //if we are on the first number
				median=num;
			}
			else if(num_count==2){ //if we are on the 2nd number we can generate heaps
				temp=median;
				if(num<median){
					maxHeap.insert(num);
					minHeap.insert(temp); maxCount++; minCount++;
				}
				else{
					minHeap.insert(num);
					maxHeap.insert(temp); maxCount++; minCount++;
				}
				median=(num+temp)/2; //now we have minheap's top, median and maxheap. all operations are available
			}
			else {
				if(maxCount==minCount){
					if(median>num){
						maxHeap.insert(num); maxCount++;
						median=maxHeap.findTop(); //top of larger heap
					}
					else {
						minHeap.insert(num); minCount++;
						median=minHeap.findTop();
					}
				}
				else if(maxCount>minCount){
					if (num<median){ //we cannot add to maxheap, we'll change median
						maxHeap.insert(num); maxCount++; //we inserted it now we have inbalance
						exchange=maxHeap.findTop(); //we will send this to min heap
						minHeap.insert(exchange); minCount++;
						maxHeap.deleteTop(); maxCount--;
						median=((double)maxHeap.findTop()+(double)minHeap.findTop())/2;
					}
					else { //num>median
						minHeap.insert(num); minCount++;
						median=((double)maxHeap.findTop()+(double)minHeap.findTop())/2;
					}
				}
				else { //mincount is greater
					if (num>median){ //the difference will be 2 after insertion
						minHeap.insert(num); minCount++; //we inserted it now we have inbalance
						exchange=minHeap.findTop(); //we will send this to max heap
						maxHeap.insert(exchange); maxCount++;
						minHeap.deleteTop(); minCount--;
						median=((double)maxHeap.findTop()+(double)minHeap.findTop())/2; 
					}
					else {//we will insert to max since num<median difference will be 0
						maxHeap.insert(num); maxCount++;
						median=((double)maxHeap.findTop()+(double)minHeap.findTop())/2;
					}
				}
			}
		}
		cout<<"current median: "<<median;
		infile.close();
		cout<<endl<<"Please enter the next filename that contains integer stream: ";
		file="z";
	}
	return 0;
}