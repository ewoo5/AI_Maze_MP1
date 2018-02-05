#include "maze.hpp"
#include <queue>

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
			this->map[current->y][current->x + 1].element = 'x';
			b_fifo.push(&(this->map[current->y][current->x + 1]));
		}
		if(check_left(current)){
			this->map[current->y][current->x - 1].from = current;
			this->map[current->y][current->x - 1].visited = true;
			this->map[current->y][current->x - 1].element = 'x';
			b_fifo.push(&(this->map[current->y][current->x - 1]));
		}
		if(check_down(current)){
			this->map[current->y + 1][current->x].from = current;
			this->map[current->y + 1][current->x].visited = true;
			this->map[current->y + 1][current->x].element = 'x';
			b_fifo.push(&(this->map[current->y + 1][current->x]));
		}
		if(check_up(current)){
			this->map[current->y - 1][current->x].from = current;
			this->map[current->y - 1][current->x].visited = true;
			this->map[current->y - 1][current->x].element = 'x';
			b_fifo.push(&(this->map[current->y - 1][current->x]));
		}
	}
	
	current = &(this->map[this->f_y.front()][this->f_x.front()]);
	while(current->from != NULL){
		current = current->from;
		current->element = '.';
	}

}

void maze::dfs(){



}

void maze::astar(){



}

void maze::greedy(){



}

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
