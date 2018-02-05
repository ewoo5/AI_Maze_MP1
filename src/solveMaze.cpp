#include <cstdlib>
#include <iostream>
#include <fstream>
#include "maze.hpp"
using namespace std;

int main(int argc, char* argv[])
{
	
	if(argc != 3){
		cout<<"Needs 2 arguments in the form: >./solveMaze <maze.txt> <search algo>\n";
		return 0;
	}

	fstream mazeText, solvedMaze;
	mazeText.open(argv[1], ios::in);
	solvedMaze.open("solvedMaze.txt", ios::out);
	if(!(mazeText.is_open())){
		cout<<"Failed to open maze text file.\n";
		return 0;
	}
	if(!(solvedMaze.is_open())){
		cout<<"Failed to open solved maze text file.\n";
		mazeText.close();
		return 0;
	}

	maze inputMaze(mazeText);

	inputMaze.printMaze();

	mazeText.close();
	solvedMaze.close();

	return 0;
}
