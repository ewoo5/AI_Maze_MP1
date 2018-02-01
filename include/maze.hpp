#include <iostream>
#include <fstream>
using namespace std;

class maze{

	public:
		maze();
		maze(fstream mazeText);
		~maze();
		int width, length; //width is delta y, and length is delta x.
		char ** map;

		void parseText(fstream mazeText); //for reading and saving a maze text file.
		void printMaze();

	private:


};