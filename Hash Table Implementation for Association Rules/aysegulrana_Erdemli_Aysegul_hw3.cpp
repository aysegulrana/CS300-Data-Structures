#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "HashTable.h"
using namespace std;

class Item {
public:
	bool operator==(const Item &rhs) const
	{  
		if(rhs.name.size()==0 || name.size()==0)
			return false;
		else if((rhs.name.size()==2 && name.size()==1) || (rhs.name.size()==1 && name.size()==2))
			return false;
		else if (rhs.name.size()==1 && name.size()==1)
			return (name==rhs.name);
		else if (rhs.name.size()==2 && name.size()==2)
			return (name == rhs.name || (name[0]==name[1] && name[1]==name[0])); //the order of pair doesn't matter
	}
	bool operator!=(const Item &rhs) const
	{  		if(rhs.name.size()==0 || name.size()==0)
	return true;
	else if((rhs.name.size()==2 && name.size()==1) || (rhs.name.size()==1 && name.size()==2))
		return true;
	else if (rhs.name.size()==1 && name.size()==1)
		return !(name==rhs.name);
	else if (rhs.name.size()==2 && name.size()==2)
		return !(name == rhs.name || (name[0]==name[1] && name[1]==name[0])); 
	}
	vector<string> name;
	double support;
	int count;
	int hash(string & key, int tableSize )
	{
		int hashVal = 0;
		for( char ch : key )
			hashVal += ch;
		return hashVal % tableSize;
	}
	int hash(Item item, int tableSize)
	{   
		return(item.hash(item.name[0], tableSize)); 
	}
};
const Item ITEM_NOT_FOUND;
struct Rule {
	Item first_item;
	Item sec_item;
	double confidence;
	double total_support;
	int total_count;
};
//7 writing the rules to file
void writeRules(vector<Rule> &unknowns, double tconfidence, vector<Rule> r){
	int rule_count=0;
	ofstream rfile;
	for (int h=0; h<r.size(); h++){
		if (r[h].confidence>=tconfidence){
			if(rule_count==0){
				rfile.open("results.txt");
			}
			rule_count++;
			if(r[h].first_item.name.size()==2)
				rfile<<r[h].first_item.name[0]<<","<<r[h].first_item.name[1]<<";";
			else{
				rfile<<r[h].first_item.name[0]<<";";	}
			if(r[h].sec_item.name.size()==2)
				rfile<<r[h].sec_item.name[0]<<","<<r[h].sec_item.name[1]<<";";
			else{
				rfile<<r[h].sec_item.name[0]<<";";	}
			rfile << std::fixed;
			rfile << std::setprecision(2);
			rfile << r[h].confidence<<endl;
		}
	}
	for (int j=0; j<unknowns.size(); j++){
		if (unknowns[j].confidence>=tconfidence){
			if(rule_count==0){
				rfile.open("results.txt");
			}
			rule_count++;
			if(unknowns[j].first_item.name.size()==2)
				rfile<<unknowns[j].first_item.name[0]<<","<<unknowns[j].first_item.name[1]<<";";
			else{
				rfile<<unknowns[j].first_item.name[0]<<";";	}
			if(unknowns[j].sec_item.name.size()==2)
				rfile<<unknowns[j].sec_item.name[0]<<","<<unknowns[j].sec_item.name[1]<<";";
			else{
				rfile<<unknowns[j].sec_item.name[0]<<";";	}
			rfile << std::fixed;
			rfile << std::setprecision(2);
			rfile << unknowns[j].confidence<<endl;
		}
	}
	if(rule_count==0){
		cout<<"There is no rule for the given support and confidence values."<<endl;
	}
	else {
		rfile.close();
		cout<<rule_count<<" rules are written to results.txt"<<endl;}
}
//6 finding the confidence values of the rules' by reading the file again. 
//if the rule is like A->B we don't need to look at the file. we already have the pair and single support. 
//we need to read the file for the 3 and 4 ones.
void findConfidence(HashTable<Item>&table, double tconfidence, double tsupport, int linecount, string file, vector<Rule> &r, HashTable<Item> &pairs_removed){
	vector<Rule> unknowns; vector<string> support; vector<Item>test;
	table.traverseTable(test);
	for (int i=0; i<r.size(); i++){
		if(r[i].first_item.name.size()==1 && r[i].sec_item.name.size()==1) { //if it is like A->B
			if (r[i].first_item.name[0]>r[i].sec_item.name[0]){
				support.push_back(r[i].first_item.name[0]);
				support.push_back(r[i].sec_item.name[0]);
			}
			else{
				support.push_back(r[i].sec_item.name[0]);
				support.push_back(r[i].first_item.name[0]);
			}
			Item pairitem; pairitem.name=support;  //this vector will help us find the support val of pair in the table

			if(table.find(pairitem)==pairitem) //*i don't have the pair in hash table bc it is under threshold
				r[i].confidence=table.find(pairitem).support/table.find(r[i].first_item).support; 

			else {
				r[i].confidence=pairs_removed.find(pairitem).support/table.find(r[i].first_item).support;
			}
			support.clear(); //we need this later
		}
		else { //if there's a pair in the rule
			unknowns.push_back(r[i]); 
		}
	}
	for (int f=0; f<unknowns.size(); f++)
		unknowns[f].total_count=0;
	string line;
	ifstream infile(file);
	vector<string> transaction;
	while (getline(infile, line))
	{	
		string name;	
		istringstream iss(line);

		while (iss>>name){
			transaction.push_back(name);
		}
		int rule; //this will hold the number of items match the rule.
		for (int l=0; l<unknowns.size(); l++){ //each rule
			rule=0; 
			if(unknowns[l].first_item.name.size()==unknowns[l].sec_item.name.size()){ //both 2 items
				for(int k=0; k<transaction.size(); k++){ //each item
					if (transaction[k]==unknowns[l].first_item.name[0] || transaction[k]==unknowns[l].first_item.name[1] 
					|| transaction[k]==unknowns[l].sec_item.name[0] || transaction[k]==unknowns[l].sec_item.name[1]){
						rule++; //if the item is in rule
					}
				}
				if(rule==4){ //if the transaction has the rule
					unknowns[l].total_support=((double)unknowns[l].total_count+1)/linecount; //support(A,B,C,D)
					unknowns[l].total_count++;
					unknowns[l].confidence=unknowns[l].total_support/table.find(unknowns[l].first_item).support;
				}
			}
			else if(unknowns[l].first_item.name.size()==2){ //it's like AB->C
				for(int k=0; k<transaction.size(); k++){ //each item
					if (transaction[k]==unknowns[l].first_item.name[0] || transaction[k]==unknowns[l].first_item.name[1] 
					|| transaction[k]==unknowns[l].sec_item.name[0]){
						rule++; 
					}
				}
				if(rule==3){ //if the transaction has the rule
					unknowns[l].total_support=((double)unknowns[l].total_count+1)/linecount; //support(A,B,C)
					unknowns[l].total_count++;
					unknowns[l].confidence=unknowns[l].total_support/table.find(unknowns[l].first_item).support;
				}

			}
			else if(unknowns[l].sec_item.name.size()==2){ //it's like A->CB
				for(int k=0; k<transaction.size(); k++){ //each item
					if (transaction[k]==unknowns[l].first_item.name[0] || transaction[k]==unknowns[l].sec_item.name[0] 
					|| transaction[k]==unknowns[l].sec_item.name[1]){
						rule++; 
					}
				}
				if(rule==3){ //if the transaction has the rule
					unknowns[l].total_support=((double)unknowns[l].total_count+1)/(double)linecount; //support(A,B,C)
					unknowns[l].total_count++;
					unknowns[l].confidence=unknowns[l].total_support/table.find(unknowns[l].first_item).support;
				}
			}
		}
		transaction.clear(); //we clear it for the next line
	}
	writeRules(unknowns, tconfidence, r);
}
//5
vector<Rule> findRules(HashTable<Item> & table){
	vector<Rule> r;	vector<Item> vec; bool invalid;
	//traversing the hash table, find all the "pairs" of items including paired items and single ones.
	table.traverseTable(vec);
	for (int i=0; i<vec.size(); i++){
		for (int j=0; j<vec.size(); j++){
			if (vec[i].name!=vec[j].name){ 
				invalid=false;
				for(int k=0;k<vec[j].name.size(); k++){ //we check if we are allowed to have transaction in between
					for (int m=0; m<vec[i].name.size();m++){
						if(vec[j].name[k]==vec[i].name[m]){
							invalid=true;
							break;
						}
					}
				}
				if(!invalid){ 
					Rule ourule; 
					ourule.first_item=vec[i];
					ourule.sec_item=vec[j];
					r.push_back(ourule);
				}
			}
		}
	}
	return r;
}

//4
void findPairs (HashTable<Item> & table, string filename, vector<Item> pairs, int transaction, double threshold, HashTable<Item> &pairs_removed) {
	ifstream infile(filename);	string line;	vector<string> vecline; 

	while (getline(infile, line))
	{	
		string name;	
		istringstream iss(line);
		while (iss>>name){
			Item item; 
			item.name.push_back(name);
			if (table.find(item)!=item){ //if it doesn't exists in table, skip
				continue; } 
			else {
				vecline.push_back(name); //add the items in the line to the vector to see which pairs we have in the line
			}
		} //now we will increase the count of pairs we found on possible pairs vector.
		bool exists=false;
		for (int i=0; i<vecline.size(); i++){
			for (int j=0; j<vecline.size(); j++){
				if (vecline[i]!=vecline[j]){ 
					Item pair; 
					if(vecline[i]>vecline[j]){
						pair.name.push_back(vecline[i]);
						pair.name.push_back(vecline[j]); 
					}
					else{
						pair.name.push_back(vecline[j]); 
						pair.name.push_back(vecline[i]);
					}
					if(!exists){
						for (int n=0; n<pairs.size(); n++){
							if(pairs[n].name==pair.name) { //we check if the pair on the line exists in possible pairs
								pairs[n].count++;
								pairs[n].support=(double)pairs[n].count/(double)transaction;
								break;
							}
						}
					}
					exists=false;
				}
			}
		}
		vecline.clear(); //we're going to be ready for next line
	}
	for (int h=0; h<pairs.size(); h++){
		Item pairadd=pairs[h];
		pairadd.support=pairadd.support/(double)2;
		pairadd.count=pairadd.count/2;

		if(pairs[h].count!=0 && pairs[h].support/(double)2>threshold){
			table.insert(pairadd);
		}
		else{ //if we did not add them to our main hash table
			pairs_removed.insert(pairadd);
		}
	}
}
//3 this function will return a vector that have all the possible pairs in the remainings of the hash table
vector<Item> possiblePairs(HashTable<Item> & table){
	vector<Item>hashvec;	vector<Item> pairvec;	bool exists=false;
	table.traverseTable(hashvec); //now vector has all the items in hash table

	for (int i=0;i<hashvec.size();i++){
		for (int j=0; j<hashvec.size();j++){

			if (hashvec[i].name!=hashvec[j].name){		

				for (int k=0; k<pairvec.size(); k++){
					if ((pairvec[k].name[0]==hashvec[j].name[0] &&	pairvec[k].name[1]==hashvec[i].name[0]) 
						|| (pairvec[k].name[1]==hashvec[j].name[0] &&	pairvec[k].name[1]==hashvec[i].name[0])
						|| (pairvec[k].name[0]==hashvec[j].name[0] &&	pairvec[k].name[0]==hashvec[i].name[0])
						|| (pairvec[k].name[1]==hashvec[j].name[0] &&	pairvec[k].name[0]==hashvec[i].name[0])){
							exists=true;
					}
				}
				if(!exists){
					Item pair;	
					if(hashvec[i].name[0]>hashvec[j].name[0]){
						pair.name.push_back(hashvec[i].name[0]);
						pair.name.push_back(hashvec[j].name[0]);
					}
					else {
						pair.name.push_back(hashvec[j].name[0]);
						pair.name.push_back(hashvec[i].name[0]);
					}
					pair.count=0;
					pairvec.push_back(pair);
				}
				exists=false;
			}
		}
	}
	return pairvec;
}
//2
void thresholdCheck(HashTable<Item> & table, double tr, int linecount){
	vector<Item> vec; 	
	table.traverseTable(vec);
	double support;
	for (int i=0;i<vec.size();i++){
		support=(double)vec[i].count/(double)linecount;
		if (support<tr)
			table.remove(table.find(vec[i]));
		else {
			Item it=vec[i];
			it.support=support;
			table.insert(it);
		}
	}
}
//1
string readFile(HashTable<Item> & table, int &linecount){
	string line, file; 
	cout<<"Please enter the file name: ";
	cin>>file;
	ifstream infile(file);

	while (getline(infile, line))
	{	
		linecount++;
		string name;	
		istringstream iss(line);

		while (iss>>name){
			Item item;
			item.name.push_back(name);
			if (table.find(item)!=item) { //if we cannot find item in hash table
				item.count=1;
				table.insert(item);
			}
			else { //if it was added before
				item.count=table.find(item).count+1;
				table.insert(item);
			}
		}
	}
	return file;
}

int main () {
	Item i; int size=101; double support, confidence; int linecount=0;
	HashTable<Item> table(i, size);		vector<Item> pospairs;
	string filename=readFile(table, linecount);
	cout<<"Please enter support and confidence values between 0 and 1: ";
	cin>>support>>confidence;
	Item x;
	HashTable<Item> pairs_removed(x, 101);
	//i need them while finding confidences like A->B
	thresholdCheck(table, support, linecount);
	pospairs=possiblePairs(table);
	findPairs (table, filename, pospairs, linecount, support, pairs_removed);
	vector<Rule> rules=findRules(table);
	findConfidence(table, confidence, support, linecount, filename, rules, pairs_removed);

	return 0;
}