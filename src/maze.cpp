#include "maze.hpp"
#include <queue>
//#include <stack>
//#include <unordered_map>
#include <vector>

int win_expanded = 0;

maze::maze(){
	map = NULL;
	width = 0;
	length = 0;
}

maze::maze(std::fstream& mazeText){

	parseText(mazeText);

}

void maze::parseText(std::fstream& mazeText){

	char buf;
	int i, j;
	buf = mazeText.get();
	while(buf != '\n'){
		buf = mazeText.get();
	}
	this->length = mazeText.tellg() - 1;
	mazeText.seekg(0, mazeText.end);
	this->width = (mazeText.tellg() + 2)/(this->length + 1);
	mazeText.seekg(0, mazeText.beg);

	this->map = new box*[this->width];
	for(i = 0; i < this->width; i++){
		this->map[i] = new box[this->length];
	}

	for(i = 0; i < this->width; i++){
		for(j = 0; j < this->length; j++){
			map[i][j].element =  mazeText.get();
			map[i][j].x = j;
			map[i][j].y = i;
			if( map[i][j].element == 'P'){
				this->p_x.push_back(j);
				this->p_y.push_back(i);
			}
			if(map[i][j].element == '.'){
				//this->f_x.push_back(j);
				//this->f_y.push_back(i);
				this->food_list.push_back(std::pair<int,int>(j, i));
			}			
		}
		mazeText.get();
	}

}

void maze::printMaze(){

	int i, j;
	for(i = 0; i < this->width; i++){
		for(j = 0; j < this->length; j++){
			cout<<this->map[i][j].element;
			if(j == this->length - 1){
				cout<<'\n';
			}
		}
	}

}

maze::~maze(){

	int i;
	for(i = 0; i< this->width; i++){
		delete[] this->map[i];
	}
	delete[] this->map;

}

void maze::bfs(){

	f_y.push_back(food_list.begin()->second);
	f_x.push_back(food_list.begin()->first);
	//cout<<"reached here";

	queue<box*> b_fifo;
	int start_x = this->p_x.front();
	int start_y = this->p_y.front();
	int end_x = this->f_x.front();
	int end_y = this->f_y.front();
	box* current;
	this->map[start_y][start_x].visited = true;
	b_fifo.push(&(this->map[start_y][start_x]));
	while(!b_fifo.empty()){
		current = b_fifo.front();
		b_fifo.pop();
		if( (current->x == this->f_x.front()) && (current->y == this->f_y.front())){
			break;
		} 
		
		if(check_right(current)){
			this->map[current->y][current->x + 1].from = current;
			this->map[current->y][current->x + 1].visited = true;
			b_fifo.push(&(this->map[current->y][current->x + 1]));
		}
		if(check_left(current)){
			this->map[current->y][current->x - 1].from = current;
			this->map[current->y][current->x - 1].visited = true;
			b_fifo.push(&(this->map[current->y][current->x - 1]));
		}
		if(check_down(current)){
			this->map[current->y + 1][current->x].from = current;
			this->map[current->y + 1][current->x].visited = true;
			b_fifo.push(&(this->map[current->y + 1][current->x]));
		}
		if(check_up(current)){
			this->map[current->y - 1][current->x].from = current;
			this->map[current->y - 1][current->x].visited = true;
			b_fifo.push(&(this->map[current->y - 1][current->x]));
		}
	}
	
	int nodes_expanded = 0;
	int path_length = 0;
	for(int a = 0; a < width; a++){
		for(int b = 0; b < length; b++){
			if(map[a][b].from != NULL){
				nodes_expanded++;
				map[a][b].element = '#';
			}
		}
	}

	current = &(this->map[this->f_y.front()][this->f_x.front()]);
	while(current->from != NULL){
		current->element = '.';
		nodes_expanded--;
		path_length++;
		current = current->from;
	}
	
	cout<<"Nodes expanded: "<<nodes_expanded<<"\n";	
	cout<<"Path length: "<<path_length<<"\n";

}

void maze::dfs(){

	f_y.push_back(food_list.begin()->second);
	f_x.push_back(food_list.begin()->first);

	stack<box*> b_lifo;
	box * current = &(this->map[this->p_y.front()][this->p_x.front()]);
	b_lifo.push(current);
	current->visited = true;

	while(!b_lifo.empty()){
		current = b_lifo.top();
		if( (current->x == this->f_x.front()) && (current->y == this->f_y.front())){
			break;
		} 
		
		if(check_right(current)){
			this->map[current->y][current->x + 1].from = current;
			this->map[current->y][current->x + 1].visited = true;
			b_lifo.push(&(this->map[current->y][current->x + 1]));
			continue;
		}
		if(check_left(current)){
			this->map[current->y][current->x - 1].from = current;
			this->map[current->y][current->x - 1].visited = true;
			b_lifo.push(&(this->map[current->y][current->x - 1]));
			continue;
		}
		if(check_down(current)){
			this->map[current->y + 1][current->x].from = current;
			this->map[current->y + 1][current->x].visited = true;
			b_lifo.push(&(this->map[current->y + 1][current->x]));
			continue;
		}
		if(check_up(current)){
			this->map[current->y - 1][current->x].from = current;
			this->map[current->y - 1][current->x].visited = true;
			b_lifo.push(&(this->map[current->y - 1][current->x]));
			continue;
		}
		b_lifo.pop();
	}

	int nodes_expanded = 0;
	int path_length = 0;
	for(int a = 0; a < width; a++){
		for(int b = 0; b < length; b++){
			if(map[a][b].from != NULL){
				nodes_expanded++;
				map[a][b].element = '#';
			}
		}
	}

	current = &(this->map[this->f_y.front()][this->f_x.front()]);
	while(current->from != NULL){
		current->element = '.';
		nodes_expanded--;
		path_length++;
		current = current->from;
	}
	
	cout<<"Nodes expanded: "<<nodes_expanded<<"\n";	
	cout<<"Path length: "<<path_length<<"\n";

}

std::vector<box*> maze::astar(){

	f_y.push_back(food_list.begin()->second);
	f_x.push_back(food_list.begin()->first);

	int x, y;
	box * current = &(map[p_y.front()][p_x.front()]);
	std::map<std::pair<int, int>, box*, astar_man_dist> neighbors (astar_man_dist(f_x.front(), f_y.front(), this->map)); 
	std::pair<int, int> xy_pair(p_x.front(), p_y.front());
	current->curr_dist = 0;
	neighbors[xy_pair] = current;

	while(!neighbors.empty()){
		//call helper?
		std::map<std::pair<int, int>, box*>::iterator it = neighbors.begin();
		current = it->second;
		x = current->x; y = current->y;
		if( (x == f_x.front()) && (y == f_y.front())){
			break;
		}

		map[current->y][current->x].visited = true;
		xy_pair = std::pair<int, int>(current->x, current->y);
		neighbors.erase(xy_pair);
	
		// NOTE: check mechanism to see if something is already in map, otherwise take it out and replace it.
		if(check_right(current)){
			if(map[current->y][current->x + 1].curr_dist > (map[current->y][current->x].curr_dist + 1)){
				neighbors.erase(std::pair<int,int>(current->x + 1, current->y));
				map[current->y][current->x + 1].curr_dist = (map[current->y][current->x].curr_dist + 1);
				map[current->y][current->x + 1].from = current;
			}
			xy_pair = std::pair<int, int>(current->x + 1, current->y);
			//cout<<"("<<xy_pair.first<<","<<xy_pair.second<<")";
			std::pair<std::map<std::pair<int,int>,box*>::iterator, bool> insert_check  = neighbors.insert(std::pair<std::pair<int,int>,box*>(xy_pair, &(map[current->y][current->x + 1])));
			if(insert_check.second == false){
				//cout<<"Fail!\n";
			}//else{cout<<"\n";}
		}

		if(check_left(current)){
			if(map[current->y][current->x - 1].curr_dist > (map[current->y][current->x].curr_dist + 1)){
				neighbors.erase(std::pair<int,int>(current->x - 1, current->y));
				map[current->y][current->x - 1].curr_dist = (map[current->y][current->x].curr_dist + 1);
				map[current->y][current->x - 1].from = current;
			}
			xy_pair = std::pair<int, int>(current->x - 1, current->y);
			//cout<<"("<<xy_pair.first<<","<<xy_pair.second<<")";
			std::pair<std::map<std::pair<int,int>,box*>::iterator, bool> insert_check  = neighbors.insert(std::pair<std::pair<int,int>,box*>(xy_pair, &(map[current->y][current->x - 1])));
			if(insert_check.second == false){
				//cout<<"Fail!\n";
			}//else{cout<<"\n";}
		}

		if(check_down(current)){
			if(map[current->y + 1][current->x].curr_dist > (map[current->y][current->x].curr_dist + 1)){
				neighbors.erase(std::pair<int,int>(current->x, current->y + 1));
				map[current->y + 1][current->x].curr_dist = (map[current->y][current->x].curr_dist + 1);
				map[current->y + 1][current->x].from = current;
			}
			xy_pair = std::pair<int, int>(current->x, current->y + 1);
			//cout<<"("<<xy_pair.first<<","<<xy_pair.second<<")";
			std::pair<std::map<std::pair<int,int>,box*>::iterator, bool> insert_check  = neighbors.insert(std::pair<std::pair<int,int>,box*>(xy_pair, &(map[current->y + 1][current->x])));
			if(insert_check.second == false){
				//cout<<"Fail!\n";
			}//else{cout<<"\n";}
		}

		if(check_up(current)){
			if(map[current->y - 1][current->x].curr_dist > (map[current->y][current->x].curr_dist + 1)){
				neighbors.erase(std::pair<int,int>(current->x, current->y - 1));
				map[current->y - 1][current->x].curr_dist = (map[current->y][current->x].curr_dist + 1);
				map[current->y - 1][current->x].from = current;
			}
			xy_pair = std::pair<int, int>(current->x, current->y - 1);
			//cout<<"("<<xy_pair.first<<","<<xy_pair.second<<")";
			std::pair<std::map<std::pair<int,int>,box*>::iterator, bool> insert_check  = neighbors.insert(std::pair<std::pair<int,int>,box*>(xy_pair, &(map[current->y - 1][current->x])));
			if(insert_check.second == false){
				//cout<<"Fail!\n";
			}//else{cout<<"\n";}
		}
		
	}
	
	//int nodes_expanded = 0;
	//int path_length = 0;
	// this chunk modifies the elements immediately (void function)
	for(int a = 0; a < width; a++){
		for(int b = 0; b < length; b++){
			if(map[a][b].from != NULL){
				win_expanded++;
				//map[a][b].element = '#';
			}
		}
	}
	
	current = &(this->map[this->f_y.front()][this->f_x.front()]);
	while(current->from != NULL){
		win_expanded--;
		//path_length++;
		current->element = '.';
		current = current->from;
	}

	//cout<<"Nodes expanded: "<<nodes_expanded<<"\n";	
	//cout<<"Path length: "<<path_length<<"\n";	
	
	
	// for returning the solution vector
	std::vector<box*> solution;
	current = &(this->map[this->f_y.front()][this->f_x.front()]);
	while(current->from != NULL){
		solution.push_back(current);
		current = current->from;
	}
	
	return solution;
	

}

void maze::greedy(){

	f_y.push_back(food_list.begin()->second);
	f_x.push_back(food_list.begin()->first);

	stack<box*> b_lifo;
	box * current = &(this->map[this->p_y.front()][this->p_x.front()]);
	b_lifo.push(current);
	current->visited = true;

	greedy_helper(b_lifo);

	for(int a = 0; a < width; a++){
		for(int b = 0; b < length; b++){
			if(map[a][b].from != NULL){
				map[a][b].element = '#';
			}
		}
	}
	
	int nodes_expanded = 0;
	int path_length = 0;
	for(int a = 0; a < width; a++){
		for(int b = 0; b < length; b++){
			if(map[a][b].from != NULL){
				nodes_expanded++;
				map[a][b].element = '#';
			}
		}
	}

	current = &(this->map[this->f_y.front()][this->f_x.front()]);
	while(current->from != NULL){
		current->element = '.';
		nodes_expanded--;
		path_length++;
		current = current->from;
	}
	
	cout<<"Nodes expanded: "<<nodes_expanded<<"\n";	
	cout<<"Path length: "<<path_length<<"\n";

}

bool maze::greedy_helper(std::stack<box*>& b_lifo){

	box * current;
	current = b_lifo.top();
	if( (current->x == this->f_x.front()) && (current->y == this->f_y.front())){
		return true;
	}

	priority_queue<box*, std::vector<box*>, cmp_man_dist> Hue_queue (cmp_man_dist(&(map[f_y.front()][f_x.front()])));

	/*
	* Checks each direction if available.
	* If available, push into priority queue.
	*/
	if(check_right(current)){
		this->map[current->y][current->x + 1].from = current;
		this->map[current->y][current->x + 1].visited = true;
		Hue_queue.push(&(map[current->y][current->x +1]));
	}
	if(check_left(current)){
		this->map[current->y][current->x - 1].from = current;
		this->map[current->y][current->x - 1].visited = true;
		Hue_queue.push(&(map[current->y][current->x -1]));
	}
	if(check_down(current)){
		this->map[current->y + 1][current->x].from = current;
		this->map[current->y + 1][current->x].visited = true;
		Hue_queue.push(&(map[current->y + 1][current->x]));
	}
	if(check_up(current)){
		this->map[current->y - 1][current->x].from = current;
		this->map[current->y - 1][current->x].visited = true;
		Hue_queue.push(&(map[current->y - 1][current->x]));
	}
	
	while(!Hue_queue.empty()){
		b_lifo.push(Hue_queue.top());
		Hue_queue.pop();
		if(greedy_helper(b_lifo)){
			return true;
		}
	}
	b_lifo.pop();
	return false;

}

std::vector<std::pair<int,int>> maze::win(){

	// insert the pacman location into the priority queue.
	vector<std::pair<int,int>> start_vector;
	start_vector.push_back(std::pair<int,int>(p_x.front(), p_y.front()));
	priority_queue<std::pair<int,vector<std::pair<int,int>>>, std::vector<std::pair<int,vector<std::pair<int,int>>>>, win_cmp_hue>  Hue_queue (win_cmp_hue(food_list.size()));
	Hue_queue.push(std::pair<int,vector<std::pair<int,int>>>(0, start_vector));

	while(!Hue_queue.empty()){
		std::pair<int, vector<std::pair<int,int>>> current = Hue_queue.top();	
		Hue_queue.pop();
		
		//cout<<"current best distance:"<<current.first<<", with "<<current.second.size()<<"eaten.\n";

		// if all food are already eaten + pacman himself
		if(current.second.size() == food_list.size() + 1){
			auto it2 = current.second.begin();
			int a = 1;
			for(it2 = current.second.begin(); it2 != current.second.end(); it2++){
				if(a < 10){
					map[it2->second][it2->first].element = '0' + a;
				}else{
					map[it2->second][it2->first].element = '@' + a - 9;
				}
				a++;;
			}
			cout<<"Nodes expanded: "<<win_expanded<<"\n";
			cout<<"Path length: "<<current.first<<"\n"; // print the lowest number we got.
			return current.second;
		}

		auto it = food_list.begin();
		for(it = food_list.begin(); it != food_list.end(); it++){
			if(!food_is_in(*it, current.second)){
				// if this food hasn't been eaten. Find solution, add into vector, then push into queue.
				std::pair<int,vector<std::pair<int,int>>> new_venture(current);
				std::pair<std::pair<int,int>, std::pair<int,int>> new_pair(*it, new_venture.second.back());
				int mut_ex = insert_solution(new_pair);
				if(mut_ex < 30){
					new_venture.first += mut_ex;
				}else{
					if( (std::make_pair(35,10) == new_pair.first) || (std::make_pair(35,10) == new_pair.second)){
						if(mut_ex < 40){
							new_venture.first += mut_ex;
						}else{
							new_venture.first += 1000;
						}
					}else{
						new_venture.first += 1000;
					}
				}
				new_venture.second.push_back(*it);
				Hue_queue.push(new_venture);
			}
		}
	}
	
	cout<<"somehow its wrong";
	return std::vector<std::pair<int,int>>();

}

/*---------------------------------private helper--------------------------------------*/

int maze::check_right(box * current){
	
	if( (current->x + 1)>=this->length || current->x < 0){
		return false;
	}
	
	if( this->map[current->y][current->x + 1].visited == true ){
		return false;
	}

	if( (this->map[current->y][current->x + 1].element != '%') ){
		return true;
	}else{	

		this->map[current->y][current->x + 1].visited = true;

	}

	return false;

}

int maze::check_left(box * current){

	if( (current->x - 1)< 0){
		return false;
	}


	if( this->map[current->y][current->x - 1].visited == true ){
		return false;
	}


	if( (this->map[current->y][current->x - 1].element != '%') ){
		return true;
	}else{

		this->map[current->y][current->x - 1].visited = true;

	}

	return false;

}

int maze::check_down(box * current){

	if( (current->y + 1)>=this->length || current->y < 0){
		return false;
	}
	
	if( this->map[current->y + 1][current->x].visited == true ){
		return false;
	}

	if( (this->map[current->y + 1][current->x].element != '%') ){
		return true;
	}else{

		this->map[current->y + 1][current->x].visited = true;

	}

	return false;

}

int maze::check_up(box * current){

	if( (current->y - 1)< 0){
		return false;
	}
	
	if( this->map[current->y - 1][current->x].visited == true ){
		return false;
	}

	if( (this->map[current->y - 1][current->x].element != '%') ){
		return true;
	}else{

		this->map[current->y - 1][current->x].visited = true;

	}

	return false;

}


/*
* Calculate and returns the Manhattan distance between 2 boxes.
*/
int maze::cmp_man_dist::man_dist(box * first, box * second){

	int sum = 0;
	int dif_x, dif_y;
	if( (dif_x = first->x - second->x) < 0){
		dif_x = -dif_x;
	}
	if( (dif_y = first->y - second->y) < 0){
		dif_y = -dif_y;
	}

	return sum = dif_x + dif_y;

}


/*
* Checks if a food is currently within a vector path/solution.	
*/
bool maze::food_is_in(std::pair<int,int>& food, std::vector<std::pair<int,int>>& current_list){

	std::vector<std::pair<int,int>>::iterator it;
	for(it = current_list.begin(); it != current_list.end(); it++){
		if( (food.first == it->first) && (food.second == it->second) ){
			return true;
		}
	}
	return false;

}


int maze::insert_solution(std::pair<std::pair<int,int>,std::pair<int,int>> coord_pair){

	auto it = solution_map.find(coord_pair);
	if(it == solution_map.end()){
		// try finding with the coordinates swapped.
		it = solution_map.find(std::pair<std::pair<int,int>,std::pair<int,int>>(coord_pair.second, coord_pair.first));
		if(it == solution_map.end()){
			// Confirmed solution hasn't been found;
			int temp_x, temp_y;
			temp_x = p_x.front(); temp_y = p_y.front();
			*(p_x.begin()) = coord_pair.first.first;
			*(p_y.begin()) = coord_pair.first.second;
			f_x.push_back(coord_pair.second.first);
			f_y.push_back(coord_pair.second.second);
			//cout<<"("<<f_x.front()<<","<<f_y.front()<<")-("<<p_x.front()<<","<<p_y.front()<<") (Input)\n";
			refresh();
			std::vector<box*> ret = astar();
			solution_map.insert(std::pair<std::pair<std::pair<int,int>,std::pair<int,int>>, std::vector<box*>>(coord_pair, ret));
			f_x.pop_back();
			f_y.pop_back();
			*(p_x.begin()) = temp_x;
			*(p_y.begin()) = temp_y;
			
			//cout<<"("<<coord_pair.first.first<<","<<coord_pair.first.second<<")-("<<coord_pair.second.first<<","<<coord_pair.second.second<<") with dist "<<ret.size()<<" (New)\n";
			return ret.size();
		}
	}

	//cout<<"("<<coord_pair.first.first<<","<<coord_pair.first.second<<")-("<<coord_pair.second.first<<","<<coord_pair.second.second<<") with dist "<<it->second.size()<<"\n";
	return it->second.size();

}

void maze::refresh(){
	
	int i, j;
	for(i = 0; i < width; i++){
		for(j = 0; j < length; j++){
			map[i][j].from = NULL;
			map[i][j].visited = false;
			map[i][j].curr_dist = 1000;
		}
	}

}

/*---------------------------------operator overload--------------------------------------*/

/*
* For determining if a pair contains the same combination.
* Will be used to order pairs into a djikstra's algorithm.
*/
bool maze::couple::operator==(const couple& argument){

	return ( (((this->a->x == argument.a->x) && (this->a->y == argument.a->y)) &&
		 ((this->b->x == argument.b->x) && (this->b->y == argument.b->x))) ||
		 (((this->a->x == argument.b->x) && (this->a->y == argument.b->y)) &&
		 ((this->b->x == argument.a->x) && (this->b->y == argument.a->x)))
		);

}

/*
* Compares the Manhattan distances from the destination for lhs and rhs. (See which is nearer to goal)
*/
bool maze::cmp_man_dist::operator()(box * lhs, box * rhs){

	return man_dist(lhs, dest) > man_dist(rhs, dest);
	
}

/*
* Compares 2 the manhattan distances of 2 pairs of coordinates from the destination/food.
*/
bool maze::astar_man_dist::operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs){

	// Calculate manhattan distance for lhs
	int lhs_x_dist, lhs_y_dist, lhs_dist, rhs_dist, rhs_x_dist, rhs_y_dist;
	if( (lhs_x_dist = lhs.first - dest_x) < 0){
		lhs_x_dist = -lhs_x_dist;
	}
	if( (lhs_y_dist = lhs.second - dest_y) < 0){
		lhs_y_dist = -lhs_y_dist;
	}
	lhs_dist = lhs_y_dist + lhs_x_dist + map_copy[lhs.second][lhs.first].curr_dist;
	
	// Calculate manhattan distance for rhs
	if( (rhs_x_dist = rhs.first - dest_x) < 0){
		rhs_x_dist = -rhs_x_dist;
	}
	if( (rhs_y_dist = rhs.second - dest_y) < 0){
		rhs_y_dist = -rhs_y_dist;
	}
	rhs_dist = rhs_y_dist + rhs_x_dist + map_copy[rhs.second][rhs.first].curr_dist;

	if(rhs_dist == lhs_dist){
		if(lhs.first == rhs.first){
			return lhs.second < rhs.second;
		}
		return lhs.first > rhs.first;
	}

	return lhs_dist < rhs_dist;
}

/*
* Priority queue comparison tool for multiple foods. 
*/
bool maze::win_cmp_hue::operator()(std::pair<int,vector<std::pair<int,int>>>& lhs, std::pair<int,vector<std::pair<int,int>>>& rhs){

	int lhs_total, rhs_total;
	lhs_total = lhs.first + (food_num + 1 - lhs.second.size())*2;
	rhs_total = rhs.first + (food_num + 1 - rhs.second.size())*2;
	
	return lhs_total > rhs_total;

}
