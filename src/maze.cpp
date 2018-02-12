#include "maze.hpp"
#include <queue>
//#include <stack>
#include <map>

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
				this->f_x.push_back(j);
				this->f_y.push_back(i);
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

	queue<box*> b_fifo;
	int start_x = this->p_x.front();
	int start_y = this->p_y.front();
	int end_x = this->f_x.front();
	int end_y = this->f_y.front();
	maze::box* current;
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
	
	current = &(this->map[this->f_y.front()][this->f_x.front()]);
	stack<box*> path_stack;
	while(current->from != NULL){
		path_stack.push(current);
		current = current->from;
	}
	
	couple new_pair;
	new_pair.a = &(this->map[this->p_y.front()][this->p_x.front()]);
	new_pair.b = &(this->map[this->f_y.front()][this->f_x.front()]);
	std::pair<couple, stack<box*>> new_path(new_pair, path_stack);
	//this->paths_map.insert(new_path);

}

void maze::dfs(){

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

	for(int a = 0; a < width; a++){
		for(int b = 0; b < length; b++){
			if(map[a][b].from != NULL){
				map[a][b].element = '#';
			}
		}
	}
	
	current = &(this->map[this->f_y.front()][this->f_x.front()]);
	while(current->from != NULL){
		current->element = '.';
		current = current->from;
	}

}

void maze::astar(){

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
	
	for(int a = 0; a < width; a++){
		for(int b = 0; b < length; b++){
			if(map[a][b].from != NULL){
				map[a][b].element = '#';
			}
		}
	}
	
	current = &(this->map[this->f_y.front()][this->f_x.front()]);
	while(current->from != NULL){
		current->element = '.';
		current = current->from;
	}

}

void maze::greedy(){

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
	
	current = &(this->map[this->f_y.front()][this->f_x.front()]);
	while(current->from != NULL){
		current->element = '.';
		current = current->from;
	}

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
bool maze::astar_man_dist::operator== (const std::pair<int, int>& lhs, const std::pair<int, int>& rhs){

	return (lhs.first == rhs.first) && (lhs.second == rhs.second);

}
*/
