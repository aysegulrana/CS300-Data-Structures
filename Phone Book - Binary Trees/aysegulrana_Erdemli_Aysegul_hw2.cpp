#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <vector>
#include "BinarySearchTree.h"
#include "AvlTree.h"

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
bool operator>(const Contact &lhs, const Contact& rhs) { return lhs.fullName>rhs.fullName.substr(0, lhs.fullName.size()); }
bool operator ==(const Contact &lhs, string rhs){ return lhs.fullName.substr(0, rhs.size())==rhs;}
bool operator !=(const Contact &lhs, string rhs){ return lhs.fullName.substr(0, rhs.size())!=rhs;}
ostream &operator << (ostream& os, Contact &rhs){ //when we want to print a contact, it'll automatically print the objects of the struct
	os<< rhs.fullName <<" "<< rhs.phoneNumber <<" "<<rhs.city<<endl;
	return os;
}

void readingFromFile (string file, BinarySearchTree <Contact> &bstree, AvlTree <Contact> &avltree){
	string line, firstName, lastName;
	ifstream infile(file);

	std::chrono::time_point<std::chrono::system_clock> startTime1;
	std::chrono::time_point<std::chrono::system_clock> endTime1;
	std::chrono::time_point<std::chrono::system_clock> startTime2;
	std::chrono::time_point<std::chrono::system_clock> endTime2;

	int bst_time=0, avl_time=0;

	cout<<"Loading the phonebook into a BST."<<endl;

	while (getline(infile, line))
	{	
		Contact record;
		istringstream iss(line);
		string name, surname, number, city;
		iss>>name>>surname>>record.phoneNumber>>record.city;	

		for (int i=0; i<name.size(); i++)
			name[i]=toupper(name[i]);
		record.firstName=name;
		for (int i=0; i<surname.size(); i++)
			surname[i]=toupper(surname[i]);
		record.lastName=surname;

		record.fullName=record.firstName+" "+record.lastName;

		startTime1=std::chrono::system_clock::now();
		bstree.insert(record);
		endTime1=std::chrono::system_clock::now();
		bst_time+=std::chrono::duration_cast<std::chrono::milliseconds>(endTime1-startTime1).count(); 
		//we add up the times of each insertion
		startTime2=std::chrono::system_clock::now();
		avltree.insert(record);
		endTime2=std::chrono::system_clock::now();
		avl_time+=std::chrono::duration_cast<std::chrono::milliseconds>(endTime2-startTime2).count();
	}

	cout<<"Phonebook creation in BST took "<<bst_time<<" milliseconds. . ."<<endl;

	if (bstree.isBalanced(bstree.root)){
		cout<<"The tree is balanced"<<endl;
	}

	else {
		cout<<"The tree is not balanced"<<endl;
	}

	cout<<"The heights of BST are for left: "<<bstree.maxDepth(bstree.root->left)<<" and right: "<<bstree.maxDepth(bstree.root->right)<<endl;

	cout<<"loading the phonebook into an AVL"<<endl;
	cout<<"Phonebook creation in AVL took "<<avl_time<<" milliseconds. . ."<<endl;

	if (avltree.isBalanced(avltree.root)){
		cout<<"The tree is balanced"<<endl;
	}
	else {
		cout<<"The tree is not balanced"<<endl;
	}
	cout<<"The heights of AVL are for left: "<<avltree.maxDepth(avltree.root->left)<<" and right: "<<avltree.maxDepth(avltree.root->right)<<endl<<endl;
}

//1
void SearchAContact(BinarySearchTree<Contact> &bst, AvlTree<Contact> &avlt){

	std::chrono::time_point<std::chrono::system_clock> startTime1;
	std::chrono::time_point<std::chrono::system_clock> endTime1;
	std::chrono::time_point<std::chrono::system_clock> startTime2;
	std::chrono::time_point<std::chrono::system_clock> endTime2;

	Contact search; 
	string name;
	vector<Contact> vecbst, vecavl;

	cout<<"Search for a contact: ";
	std::getline(std::cin >> std::ws, name); //we take the entered string

	for (int i=0; i<name.size(); i++)
		name[i]=toupper(name[i]);

	cout<<endl<<"Searching an item in the phonebook (BST) . . ."<<endl;
	cout<<endl<<"Phonebook: Searching for: ("<<name<<")"<<endl;
	cout<<"===================================="<<endl;
	search.fullName=name;

	startTime1=std::chrono::system_clock::now();
	bst.find(search, vecbst);
	endTime1=std::chrono::system_clock::now();

	for (int i=0; i<vecbst.size(); i++)
		cout<<vecbst[i];

	cout<<endl<<"Searching an item in the phonebook (AVL) . . ."<<endl;
	cout<<endl<<"Phonebook: Section: ("<<name<<")"<<endl;
	cout<<"===================================="<<endl;
	startTime2=std::chrono::system_clock::now();
	avlt.find(search, vecavl);
	endTime2=std::chrono::system_clock::now();

	for (int i=0; i<vecavl.size(); i++)
		cout<<vecavl[i];

	cout<<endl<<"The search in BST took "<< 
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime1 - startTime1).count()<<" nanoseconds. . ."<<endl;
	cout<<"The search in AVL took "<< 
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime2 - startTime2).count()<<" nanoseconds. . ."<<endl;
}
//2
void AddContact(BinarySearchTree<Contact> &bst, AvlTree<Contact> &avlt){

	std::chrono::time_point<std::chrono::system_clock> startTime1;
	std::chrono::time_point<std::chrono::system_clock> endTime1;
	std::chrono::time_point<std::chrono::system_clock> startTime2;
	std::chrono::time_point<std::chrono::system_clock> endTime2;

	Contact newRecord;	bool avlbool, bstbool;

	cout<<"Enter the information of the contact to be added: "<<endl;
	cout<<"Name: ";
	cin>>newRecord.firstName>>newRecord.lastName;
	cout<<endl<<"Tel: ";
	cin>>newRecord.phoneNumber;
	cout<<endl<<"City: ";
	cin>>newRecord.city;

	for (int i=0; i<newRecord.firstName.size(); i++)
		newRecord.firstName[i]=toupper(newRecord.firstName[i]);

	for (int i=0; i<newRecord.lastName.size(); i++)
		newRecord.lastName[i]=toupper(newRecord.lastName[i]);

	newRecord.fullName=newRecord.firstName+" "+newRecord.lastName;

	cout<<endl<<"Adding an item to the phonebook BST . . ."<<endl;
	cout<<"===================================="<<endl;
	startTime1=std::chrono::system_clock::now();
	bstbool=bst.insert(newRecord);
	endTime1=std::chrono::system_clock::now();

	if (bstbool)
		cout<<"Contact has been added successfully to the BST"<<endl;

	else{ cout<<"The given contact full name already exists in the database."<<endl; return; } 
	//we don't need to insert to avl if it already exists
	cout<<endl<<"Adding an item to the phonebook (AVL) . . ."<<endl;
	cout<<"===================================="<<endl;
	startTime2=std::chrono::system_clock::now();
	avlbool=avlt.insert(newRecord);
	endTime2=std::chrono::system_clock::now();

	cout<<"Contact has been added successfully to the AVL Tree"<<endl;

	cout<<endl<<"Adding a contact to the Binary Tree took "<< 
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime1 - startTime1).count()<<" nanoseconds. . ."<<endl;
	cout<<"Adding a contact to the AVL Tree took "<< 
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime2 - startTime2).count()<<" nanoseconds. . ."<<endl;
}
//3
void DeleteContact(BinarySearchTree<Contact> &bst, AvlTree<Contact> &avlt){

	std::chrono::time_point<std::chrono::system_clock> startTime1;
	std::chrono::time_point<std::chrono::system_clock> endTime1;
	std::chrono::time_point<std::chrono::system_clock> startTime2;
	std::chrono::time_point<std::chrono::system_clock> endTime2;

	string deleteName, deleteSurname;
	cout<<"Deleting an item from the phonebook . . ."<<endl;
	cout<<"Enter the fullname of the contact to be deleted: ";
	cin>>deleteName>>deleteSurname;

	Contact dname;
	dname.fullName=deleteName+" "+deleteSurname;

	for (int i=0; i<dname.fullName.size(); i++)
		dname.fullName[i]=toupper(dname.fullName[i]);

	startTime1=std::chrono::system_clock::now();
	bst.remove(dname);
	endTime1=std::chrono::system_clock::now();

	startTime2=std::chrono::system_clock::now();
	avlt.remove(dname);
	endTime2=std::chrono::system_clock::now();

	cout<<endl<<"Deleted succcessfully. . ."<<endl<<endl;

	cout<<"Deletion from the Binary Tree took "<< 
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime1 - startTime1).count()<<" nanoseconds. . ."<<endl;
	cout<<"Deletion from AVL Tree took "<< 
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime2 - startTime2).count()<<" nanoseconds. . ."<<endl;
}
//4
void PreOrderPrintToFile(BinarySearchTree<Contact> &bst, AvlTree<Contact> &avlt){

	std::chrono::time_point<std::chrono::system_clock> startTime1;
	std::chrono::time_point<std::chrono::system_clock> endTime1;
	std::chrono::time_point<std::chrono::system_clock> startTime2;
	std::chrono::time_point<std::chrono::system_clock> endTime2;

	ofstream bstfile;
	bstfile.open("phonebookPreOrderBST.txt");
	startTime1=std::chrono::system_clock::now();
	bst.printPreorderBST(bst.root, bstfile);
	endTime1=std::chrono::system_clock::now();
	bstfile.close();

	ofstream avlfile;	
	avlfile.open ("phonebookPreOrderAVL.txt");
	startTime2=std::chrono::system_clock::now();
	avlt.printPreorderAVL(avlt.root, avlfile);
	endTime2=std::chrono::system_clock::now();
	avlfile.close();

	cout<<"Printing pre order to file from the Binary Tree took "<< 
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime1 - startTime1).count()<<" nanoseconds. . ."<<endl;
	cout<<"Printing pre order to file from AVL Tree took "<< 
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime2 - startTime2).count()<<" nanoseconds. . ."<<endl;
}

void InOrderPrintToFile(BinarySearchTree<Contact> &bst, AvlTree<Contact> &avlt){
	std::chrono::time_point<std::chrono::system_clock> startTime1;
	std::chrono::time_point<std::chrono::system_clock> endTime1;
	std::chrono::time_point<std::chrono::system_clock> startTime2;
	std::chrono::time_point<std::chrono::system_clock> endTime2;

	ofstream bstfile;
	bstfile.open("phonebookInOrderBST.txt");
	startTime1=std::chrono::system_clock::now();
	bst.printInorderBST(bst.root, bstfile);
	endTime1=std::chrono::system_clock::now();
	bstfile.close();

	ofstream avlfile;	
	avlfile.open ("phonebookInOrderAVL.txt");
	startTime2=std::chrono::system_clock::now();
	avlt.printInorderAVL(avlt.root, avlfile);
	endTime2=std::chrono::system_clock::now();
	avlfile.close();

	cout<<"Printing in order to file from the Binary Tree took "<< 
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime1 - startTime1).count()<<" nanoseconds. . ."<<endl;
	cout<<"Printing in order to file from AVL Tree took "<< 
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime2 - startTime2).count()<<" nanoseconds. . ."<<endl;
}

void drawBST(BinaryNode<Contact> *node, string line, string blank, BinarySearchTree<Contact> &bst, int depth, ofstream & file) {
	string space="  |";

	if (node == NULL) 
		return; 

	if (node==bst.root)
		file << "|__" << node->element.fullName<<endl;

	else{
		for (int i=0; i<(depth); i++){
			blank+=space;	
		}
		file << blank << line << node->element.fullName<<endl;	}

	blank=""; //we reset the blanks
	drawBST(node->left, "--", blank, bst, depth+1, file);  //if it's a left node subtree two line will be added
	drawBST(node->right, "__", blank, bst, depth+1, file); //if it's a right node two underlines
}

void drawAVL(AvlNode<Contact> *node, string line, string blank, AvlTree<Contact> &avlt, int depth, ofstream &file) {
	string space="  |";

	if (node == NULL) 
		return; 

	if (node==avlt.root)
		file << "|__" << node->element.fullName<<endl;

	else {
		for (int i=0; i<(depth); i++){
			blank+=space;	}
		file << blank << line << node->element.fullName<<endl;	}

	blank="";
	drawAVL(node->left, "--", blank, avlt, depth+1, file);  
	drawAVL(node->right, "__", blank, avlt, depth+1, file); 
}

//5
void DrawTreeToFile(BinarySearchTree<Contact> &bst, AvlTree<Contact> &avlt){
	std::chrono::time_point<std::chrono::system_clock> startTime1;
	std::chrono::time_point<std::chrono::system_clock> endTime1;
	std::chrono::time_point<std::chrono::system_clock> startTime2;
	std::chrono::time_point<std::chrono::system_clock> endTime2;

	string blank="";
	string line; int depth=0;

	ofstream bstfile;
	bstfile.open("phonebookTreeBST.txt");
	startTime1=std::chrono::system_clock::now();
	drawBST (bst.root, line, blank, bst, depth, bstfile);
	endTime1=std::chrono::system_clock::now();
	bstfile.close();

	ofstream avlfile;
	avlfile.open("phonebookTreeAVL.txt");	
	startTime2=std::chrono::system_clock::now();
	drawAVL(avlt.root, line, blank, avlt, depth, avlfile);
	endTime2=std::chrono::system_clock::now();
	avlfile.close();

	cout<<"Drawing tree to file from the Binary Tree took "<< 
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime1 - startTime1).count()<<" nanoseconds. . ."<<endl;
	cout<<"Drawing tree to file from AVL Tree took "<< 
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime2 - startTime2).count()<<" nanoseconds. . ."<<endl;
}

void Menu(BinarySearchTree<Contact> &bst, AvlTree<Contact> &avlt){
	bool repeat=true;
	while (repeat){
		int option;
		cout << endl;
		cout << "Choose which action to perform from 1 to 6: ";
		cout<< endl<<"1 - Search a phonebook contact" << endl
			<< "2 - Adding a phonebook contact" << endl
			<< "3 - Deleting a phonebook contact" << endl
			<< "4 - Print the phonebook to a file(inorder)" << endl
			<< " Print the phoenbook to a file(preorder)" << endl
			<< "5 - Draw the phonebook as a Tree to a file" << endl
			<< "6 - Press 6 to exit" << endl;
		cin >> option;
		cout<<endl;
		switch (option)
		{
		case 1:
			SearchAContact(bst, avlt);
			break;
		case 2:
			AddContact(bst, avlt);
			break;
		case 3:
			DeleteContact(bst, avlt);
			break;
		case 4:
			InOrderPrintToFile( bst, avlt);
			PreOrderPrintToFile(bst, avlt);
			break;
		case 5:
			DrawTreeToFile(bst, avlt);
			break;
		case 6:
			repeat=false;
		}
	}
}

int main (){

	string file;
	cout<<"Please enter the contact file name: ";
	cin>>file;

	Contact first;
	BinarySearchTree<Contact> bstree (first);
	AvlTree<Contact> avltree (first);

	readingFromFile(file, bstree, avltree);

	Menu(bstree, avltree);

	return 0;
}