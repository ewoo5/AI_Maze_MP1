maze::maze(){
	map = NULL;
	width = 0;
	length = 0;
}

maze::maze(fstream mazeText){

	parseText(mazeText);

}

void maze::parseText(fstream mazeText){

	char buf;
	int i, j;
	buf = mazeText.get();
	while(buf != \n){
		buf = mazeText.get();
	}
	this->length = mazeText.tellg() + 1;
	mazeText.seekg(0, mazeText.end);
	this->width = (mazeText.tellg() + 1)/this->length;
	mazeText.seekg(0, mazeText.beg);

	this->map = new char*[this->width];
	for(i = 0; i < this->width; i++){
		this->map[i] = new char[this->length];
	}

	for(i = 0; i < this->width; i++){
		for(j = 0; j < this->length; j++){
			map[i][j] = mazeText.get();
		}
	}

}

void maze::printMaze(){

	int i, j;
	for(i = 0; i < this->width; i++){
		for(j = 0; j < this->length; j++){
			cout<<this->map[i][j];
			if(j == this->length + 1){
				cout<<"\n";
			}
		}
	}

}

maze::~maze(){

	int i;
	for(i = 0; i< this->length; j++){
		delete[] this->map[i];
	}
	delete[] this->map;

}