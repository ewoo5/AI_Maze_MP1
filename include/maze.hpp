#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <unordered_map>
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
				int man_dist, curr_dist;				
				char element;
				box * from;
				int visited;
				box(){
					x = 0; y = 0;
					curr_dist = 1000; man_dist = 1000;
					element = ' ';
					from = NULL;
					visited = false;
				};
		};

		class couple{
			public:
			box *a, *b;

			bool operator==(const couple& argument);
		};

		box ** map;
		//std::unordered_map<couple, stack<box*>> paths_map;

		void parseText(std::fstream& mazeText); //for reading and saving a maze text file.
		//bool operator()(box * lhs, box * rhs);

		void printMaze();
		void bfs();
		void dfs();
		void astar();
		void greedy();
	
		class cmp_man_dist{
			public:
				box * dest;
				cmp_man_dist(){
					dest = NULL;
				}
				cmp_man_dist(box * in_dest){
					dest = in_dest;
				}
				bool operator()(box * lhs, box * rhs);
				int man_dist(box * first, box * second);
		};
	
		class astar_man_dist{
			public:
				int  dest_x, dest_y;
				box ** map_copy;
				astar_man_dist(){
					dest_x = 0; dest_y = 0; map_copy = NULL;
				}
				astar_man_dist(int x, int y, box ** in_map){
					dest_x = x; dest_y = y; this->map_copy = in_map;
				}
				bool operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs);
				//bool operator==(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs);
		};
	
	private:
		int check_right(box * current);
		int check_left(box * current);
		int check_up(box * current);
		int check_down(box * current);

		bool greedy_helper(std::stack<box*>& b_lifo);

};
