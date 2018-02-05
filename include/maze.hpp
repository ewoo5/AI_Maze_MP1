#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class maze{

	public:
		maze();
		maze(std::fstream& mazeText);
		~maze();
		int width, length; //width is delta y, and length is delta x.
		vector<int> p_x, p_y, f_x, f_y;

		class box{
			public:
				int x, y;				
				char element;
				box * from;
				int visited;
				box(){
					x = 0; y = 0;
					element = ' ';
					from = NULL;
					visited = false;
				};
		};

		box ** map;

		void parseText(std::fstream& mazeText); //for reading and saving a maze text file.
		void printMaze();
		void bfs();
		void dfs();
		void astar();
		void greedy();
	
	private:
		int check_right(box * current);
		int check_left(box * current);
		int check_up(box * current);
		int check_down(box * current);

};
