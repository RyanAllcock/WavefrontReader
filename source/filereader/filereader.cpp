#include "filereader.hpp"

#include <cstring> // constant string length counting

// buffer functions

void FileReader::shift(int count){
	if(count > 0){
		int i = 0, start = count;
		for(; start + i < READ_BUFFER_SIZE - 1; i++)
			buffer[i] = buffer[start + i];
		buffer[i] = '\0';
	}
}

void FileReader::fill(int count){
	shift(count);
	int currentOccupancy;
	do{
		currentOccupancy = strlen(buffer);
		isEndOfFile = fgets(&buffer[currentOccupancy], READ_BUFFER_SIZE - currentOccupancy, fp) == NULL;
		isEndOfFile &= !strlen(buffer);
	} while(currentOccupancy != strlen(buffer) && strlen(buffer) < READ_BUFFER_SIZE - 1 && !isEndOfFile);
}

// setup functions

FileReader::FileReader(const char *fileName){
	fp = fopen(fileName, "r");
	if(fp == NULL) printf("File Reader Error: file [%s] not opened\n", fileName);
	reset();
}

FileReader::~FileReader(){
	fclose(fp);
}

// seeking functions

void FileReader::reset(){
	fseek(fp, 0, SEEK_SET);
	isEndOfFile = false;
	buffer[0] = '\0';
	fill(0);
}

bool FileReader::isFinished(){
	return isEndOfFile;
}

void FileReader::advance(int count){
	int index = 0;
	for(int i = 0; i < count && !isEndOfFile; i++){
		if(buffer[index++] == '\0' || index >= READ_BUFFER_SIZE - 1){
			fill(index);
			index = 0;
		}
	}
	if(index > 0) fill(index);
}

// peeking functions

bool FileReader::check(int offset, std::string name){
	bool isBufferLargeEnough = strlen(&buffer[0]) >= name.size();
	return isBufferLargeEnough && (strncmp(&buffer[offset], name.c_str(), strlen(name.c_str())) == 0);
}

std::string FileReader::seek(int &offset, std::vector<char> const &stops){
	std::string string;
	while(offset < READ_BUFFER_SIZE - 1 && std::find(stops.begin(), stops.end(), buffer[offset]) == stops.end()){
		string += buffer[offset];
		offset++;
	}
	return string;
}

std::string FileReader::seek(int &offset, char stop){
	return seek(offset, std::vector<char>{stop});
}

std::string FileReader::peek(int &offset, std::vector<char> const &characters){
	std::string string;
	while(offset < READ_BUFFER_SIZE - 1 && std::find(characters.begin(), characters.end(), buffer[offset]) != characters.end()){
		string += buffer[offset];
		offset++;
	}
	return string;
}

std::string FileReader::peek(int &offset, char character){
	return peek(offset, std::vector<char>{character});
}

// parsing functions

bool FileReader::race(int &offset, std::vector<char> const &delimiters, std::vector<char> const &stops){
	int delimitOffset = offset, stopOffset = offset;
	peek(delimitOffset, delimiters);
	seek(stopOffset, stops);
	if(delimitOffset < stopOffset && delimitOffset != offset) offset = delimitOffset;
	else{
		offset = stopOffset;
		return false;
	}
	return true;
}

std::string FileReader::peekDigits(int &offset){
	return peek(offset, std::vector<char>{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'});
}

std::string FileReader::peekFloat(int &offset){
	std::string sign = peek(offset, '-');
	std::string integer = peekDigits(offset);
	std::string floating;
	if(check(offset, ".")){
		offset++;
		floating = peekDigits(offset);
		if(floating == "") offset--;
		else floating = '.' + floating;
	}
	return sign + integer + floating;
}

std::vector<std::string> FileReader::peekFloats(int &offset, std::vector<char> const &delimiters, std::vector<char> const &stops){
	std::vector<std::string> numbers;
	while(!isEndOfFile){
		numbers.push_back(peekFloat(offset));
		if(!race(offset, delimiters, stops)) break;
	}
	return numbers;
}