#include <iostream>

#include "objreader.hpp"

int main(int argc, char *argv[]){
	
	// access arguments
	if(argc != 2){
		std::cout << "Testing error: invalid number of arguments\n";
		return -1;
	}
	const char *fileName = argv[1];
	
	// open file
	OBJData data;
	if(!FileReader::read(fileName, data)){
		std::cout << "Testing error: file " << fileName << " didn't return data\n";
		return -1;
	}
	
	// display data
	DataHandler::display(data);
	
	return 0;
}