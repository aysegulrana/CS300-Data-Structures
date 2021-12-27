//CS300 Homework1 Aysegul Rana Erdemli aysegulrana@ 26744
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Stack.h"
#include "randgen.h"
#include "strutils.h"
using namespace std;

struct Cell {
	int x;
	int y;
	bool up, down, left, right, visited; //true for wall false for empty in directions
	Cell(){ up=true; down=true; right=true; left=true; visited=false; };
};

void inputs(int &m, int &n, int &k){ //first take the inputs
	cout<<"Enter the number of mazes: ";
	cin>>k;

	cout<<"Enter the number of rows and columns (M and N): ";
	cin>>m>>n;
}

void movingto (Stack<Cell> &st, Cell &current, vector<vector<Cell>> &cellvec, int rnew, int cnew, string dir, int &remcell){

	if (dir=="d"){
		cellvec[current.y][current.x].down=0; //breaking down the same wall in matrix
		cellvec[rnew][cnew].up=0;} //we break down the wall between the new cell and the previous
	else if (dir=="u"){
		cellvec[current.y][current.x].up=0;
		cellvec[rnew][cnew].down=0;}
	else if (dir=="l"){
		cellvec[current.y][current.x].left=0;
		cellvec[rnew][cnew].right=0;}
	else if (dir=="r"){
		cellvec[current.y][current.x].right=0;
		cellvec[rnew][cnew].left=0;
	}
	current=cellvec[rnew][cnew]; //now we have the current cell as the new one
	cellvec[rnew][cnew].visited=true; 	remcell--;
	current.visited=true;  //we're on it so it is visited
	st.push(current);
}

void randomMaze (int y, int x, int k, vector<vector<Cell>> &cellvec) {

	Stack<Cell> st;		int remcell=(x+1)*(y+1); //count of remaining cells
	cellvec[0][0].visited=true;
	st.push(cellvec[0][0]);
	bool popped=false;
	remcell--; //we decrease remcell each time we mark a place as visited
	//1 up, 2 right, 3 down, 4 left in random selection
	while (remcell!=0){
		Cell current;
		bool available=false;	RandGen r;	current=st.top();

		if (current.x==0 && current.y==0){  //random b/w up and right	
			while (!available){
				int random=r.RandInt(1, 2);
				//up
				if (random==1 && !cellvec[1][0].visited ){ available=true;
				movingto (st, current, cellvec, 1, 0, "u", remcell);
				}
				//right
				else if ( !cellvec[0][1].visited){ available=true;
				movingto (st, current, cellvec, 0, 1, "r", remcell);
				}
				else {
					st.pop(); 
					available=true;
				}
			}
		}
		else if (current.x==x && current.y==y){ //random b/w left and down
			int r3=3, r4=4;
			while (!available){
				int random=r.RandInt(3, 4);
				if (random==3) r3=0; else if (random==4) r4=0; 
				//down
				if (random==3 && !cellvec[current.y-1][current.x].visited ){ available=true; 
				movingto (st, current, cellvec, current.y-1, current.x, "d", remcell);
				}
				//left
				else if (random==4 && !cellvec[current.y][current.x-1].visited){ available=true; 
				movingto (st, current, cellvec, current.y, current.x-1, "l", remcell);
				}
				else if (r3==0 && r4==0) {
					st.pop(); 
					available=true;}
			}
		}
		else if (current.x==0 && current.y==y){ //random b/w right and down
			int r2=2, r3=3;
			while (!available){
				int random=r.RandInt(2, 3);
				if (random==2) r2=0; else if (random==3) r3=0; //we check if all random choices are tested, 
				//down												//if they are we understand that there's no where to go.
				if (random==3 && !cellvec[current.y-1][current.x].visited ){ available=true; 
				movingto (st, current, cellvec, current.y-1, current.x, "d", remcell);
				}
				//right
				else if (random==2 && !cellvec[current.y][current.x+1].visited){ available=true;
				movingto (st, current, cellvec, current.y, current.x+1, "r", remcell);
				}
				else if (r2==0 && r3==0) {
					st.pop(); 
					available=true;}
			}
		}
		else if (current.x==x && current.y==0){ //random b/w left and up
			int r1=1, r4=4;
			while (!available){
				int random=r.RandInt(1, 4);
				if (random==1) r1=0; else if (random==4) r4=0;

				//up
				if (random==1 && !cellvec[current.y+1][current.x].visited ){ available=true; 
				movingto (st, current, cellvec, current.y+1, current.x, "u", remcell);
				}
				//left
				else if (random==4 && !cellvec[current.y][current.x-1].visited){ available=true; 
				movingto (st, current, cellvec, current.y, current.x-1, "l", remcell);
				}
				else if (r1==0 && r4==0) {
					st.pop(); //no remaining available cells around so we will look at the cell under the top
					available=true;} //we get out of the loop by making available true
			}
		}
		else if (current.x==0) { //random b/w up down right 
			int r1=1, r2=2, r3=3;

			while (!available){
				int random=r.RandInt(1, 3);
				if (random==2) r2=0; else if (random==3) r3=0; else if (random==1) r1=0; 
				//up
				if (random==1 && !cellvec[current.y+1][current.x].visited ){ available=true; 
				movingto (st, current, cellvec, current.y+1, current.x, "u", remcell);
				}
				//right
				else if (random==2 && !cellvec[current.y][current.x+1].visited){ available=true;
				movingto (st, current, cellvec, current.y, current.x+1, "r", remcell);
				}
				//down
				else if (random==3 && !cellvec[current.y-1][current.x].visited){ available=true; //going down
				movingto (st, current, cellvec, current.y-1, current.x,"d", remcell);
				}
				else if (r1==0 && r2==0 && r3==0) {
					st.pop(); 
					available=true;}
			}
		}
		else if (current.y==0) { //random b/w up left right
			int r1=1, r2=2, r4=4;

			while (!available){
				int random=r.RandInt(1, 4);
				if (random==2) r2=0;  else if (random==1) r1=0; else if (random==4) r4=0;

				if (random!=3){ //we don't want down
					//up
					if (random==1 && !cellvec[current.y+1][current.x].visited ){ available=true; 
					movingto (st, current, cellvec, current.y+1, current.x, "u", remcell);
					}
					//right
					else if (random==2 && !cellvec[current.y][current.x+1].visited){ available=true;
					movingto (st, current, cellvec, current.y, current.x+1, "r", remcell);
					}
					//left
					else if (random==4 && !cellvec[current.y][current.x-1].visited){ available=true; 
					movingto (st, current, cellvec, current.y, current.x-1,"l", remcell);
					}
					else if (r1==0 && r2==0 && r4==0) {
						st.pop(); 
						available=true;}
				}
			}
		}
		else if (current.x==x) { //random b/w up left down
			int r1=1, r3=3, r4=4;
			while (!available){
				int random=r.RandInt(1, 4);
				if (random==3) r3=0;  else if (random==1) r1=0; else if (random==4) r4=0;

				if (random!=2){ //we don't want right
					//up
					if (random==1 && !cellvec[current.y+1][current.x].visited ){ available=true; 
					movingto (st, current, cellvec, current.y+1, current.x,"u", remcell);
					}
					//down
					else if (random==3 && !cellvec[current.y-1][current.x].visited){ available=true;
					movingto (st, current, cellvec, current.y-1, current.x,"d", remcell);
					}
					//left
					else if (random==4 && !cellvec[current.y][current.x-1].visited){ available=true; 
					movingto (st, current, cellvec, current.y, current.x-1,"l", remcell);
					}
					else if (r1==0 && r3==0 && r4==0) {
						st.pop(); 
						available=true;}
				}
			}
		}
		else if (current.y==y) { //random b/w down right left
			int r1=1, r2=2, r3=3;

			while (!available){
				int random=r.RandInt(2, 4);
				if (random==2) r1=0; else if (random==3) r2=0; else r3=0;

				//right
				if (random==2 && !cellvec[current.y][current.x+1].visited ){ available=true; 
				movingto (st, current, cellvec, current.y, current.x+1,"r", remcell);
				}
				//down
				else if (random==3 && !cellvec[current.y-1][current.x].visited){ available=true;
				movingto (st, current, cellvec, current.y-1, current.x,"d", remcell);
				}
				//left
				else if (random==4 && !cellvec[current.y][current.x-1].visited){ available=true; 
				movingto (st, current, cellvec, current.y, current.x-1,"l", remcell);
				}
				else if (r1==0 && r2==0 && r3==0) {
					st.pop(); 
					available=true;}
			}
		}
		else if (current.x!=0 && current.x!=x && current.y!=y && current.y!=0){ //if we are in a middle cell
			int r1=1, r2=2, r3=3, r4=4;
			while (!available){
				int random=r.RandInt(1, 4);
				if (random==2) r2=0; else if (random==3) r3=0; else if (random==1) r1=0; else r4=0;
				//right
				if (random==2 && !cellvec[current.y][current.x+1].visited ){ available=true; 
				movingto (st, current, cellvec, current.y, current.x+1,"r", remcell);
				}
				//down
				else if (random==3 && !cellvec[current.y-1][current.x].visited){ available=true;
				movingto (st, current, cellvec, current.y-1, current.x,"d", remcell);
				}
				//left
				else if (random==4 && !cellvec[current.y][current.x-1].visited){ available=true; 
				movingto (st, current, cellvec, current.y, current.x-1,"l", remcell);
				}
				//up
				else if (random==1 && !cellvec[current.y+1][current.x].visited ){ available=true; 
				movingto (st, current, cellvec, current.y+1, current.x,"u", remcell);
				}
				else if ((cellvec[current.y][current.x+1].visited && cellvec[current.y-1][current.x].visited &&
					cellvec[current.y][current.x-1].visited && cellvec[current.y+1][current.x].visited) 
					|| (r1==0 && r2==0 && r3==0 && r4==0)){
						st.pop(); 
						available=true;
				}
			}
		}
	}
}

void inputs2(int &startx, int &starty, int &endx, int &endy, int &mazeID, int k){ 
	cout<<"Enter a maze ID between 1 to "<<k<<" inclusive to find a path: ";
	cin>>mazeID;
	cout<<"Enter x and y coordinates of the entry points (x,y) or (column,row): ";
	cin>>startx>>starty;
	cout<<"Enter x and y coordinates of the exit points (x,y) or (column,row): ";
	cin>>endx>>endy;
}

void WritingPath (int mazeID, int starty, int startx, int endx, int endy, Stack<Cell> path){

	ofstream mazefile;	Stack<Cell>temp; 

	mazefile.open ("maze_"+itoa(mazeID)+"_path"+itoa(startx)+"_"+itoa(starty)+"_"+itoa(endx)+"_"+itoa(endy)+".txt");

	while (!path.isEmpty()){ //we need to reverse the stack, the path is reverse of what we want now.
		Cell t; 
		t=path.top();
		temp.push(t);
		path.pop();
	}		
	while (!temp.isEmpty()){ //reversed stack is temp, we pop it's elements so that we see our path
		Cell now; 
		now=temp.top();
		mazefile<<now.x<<" "<<now.y<<endl;
		temp.pop();
	}
	mazefile.close();
}

void pathGenerator(int startx, int starty, int endx, int endy, vector<vector<Cell>> &maze, int mazeID){

	Stack<Cell> path;
	Cell first=maze[starty][startx]; first.visited=true; maze[starty][startx].visited=true;
	path.push(first);
	Cell current=path.top();

	while (current.y!=endy || current.x!=endx) { 
		//we check if there are places to go around or not.
		if (!current.up && !maze[current.y+1][current.x].visited){
			Cell newcell=maze[current.y+1][current.x]; //we have a new cell to push
			maze[newcell.y][newcell.x].visited=true; newcell.visited=true;	//we mark the visited cell on matrix
			path.push(newcell);
		}
		else if (!current.right && !maze[current.y][current.x+1].visited){
			Cell newcell=maze[current.y][current.x+1];
			maze[newcell.y][newcell.x].visited=true; newcell.visited=true;
			path.push(newcell);
		}
		else if (!current.down && !maze[current.y-1][current.x].visited){
			Cell newcell=maze[current.y-1][current.x];
			maze[newcell.y][newcell.x].visited=true; newcell.visited=true;
			path.push(newcell);
		}
		else if (!current.left && !maze[current.y][current.x-1].visited){
			Cell newcell=maze[current.y][current.x-1];
			maze[newcell.y][newcell.x].visited=true; newcell.visited=true;
			path.push(newcell);
		}
		else {
			path.pop();
		}
		current=path.top();
	}
	WritingPath (mazeID, starty, startx, endx, endy, path);
}

int main (){

	int x, y, k;
	inputs (y, x, k);
	y--; x--;
	Cell init;

	vector<vector<vector<Cell>>> matrixvec;
	vector<vector<Cell>> cellvec (y+1, vector<Cell>(x+1, init));
	for (int i=0; i<y+1; i++){
		for (int j=0; j<x+1; j++){
			cellvec[i][j].x=j;
			cellvec[i][j].y=i;
		}
	}

	for (int i=0; i<k; i++){
		vector<vector<Cell>> newcellvec = cellvec;
		randomMaze (y, x, k, newcellvec);

		for (int r=0; r<y+1; r++){
			for (int c=0; c<x+1; c++){
				newcellvec[r][c].visited=false; //we need to empty the cells to make paths
			}
		}
		matrixvec.push_back(newcellvec); //3d vector to put mazes

		ofstream mazefile;
		mazefile.open ("maze_"+itoa(i+1)+".txt");
		mazefile<<y+1<<" "<<x+1<<endl; //m and n

		for (int p=0; p<y+1; p++){
			for (int s=0; s<x+1; s++){
				mazefile<<"x="+itoa(s)+" y="+itoa(p)+" l="+itoa(newcellvec[p][s].left)+
					" r="+itoa(newcellvec[p][s].right)+" u="+itoa(newcellvec[p][s].up)+
					" d="+itoa(newcellvec[p][s].down)<<endl;
			}
		}
		mazefile.close();	
	}
	cout<<"All mazes are generated."<<endl<<endl;

	int startx, starty,endx,endy,mazeID;
	inputs2(startx, starty, endx, endy, mazeID, k);

	pathGenerator(startx, starty, endx, endy, matrixvec[mazeID-1], mazeID);
	return 0;
}