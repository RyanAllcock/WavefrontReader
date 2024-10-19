#ifndef HEADER_FILEREADER
#define HEADER_FILEREADER

#include <stdio.h>
#include <string>
#include <vector> // expression argument listings
#include <algorithm> // expression argument searching/matching

#define READ_BUFFER_SIZE 256
#define MAX_STRING_SIZE 260

class FileReader{
	
	// internal
	FILE *fp = NULL;
	bool isEndOfFile;
	char buffer[READ_BUFFER_SIZE]; // null-terminated file transmission buffer
	
	// buffer
	void shift(int count);
	void fill(int count);
	
public:
	
	// setup
	FileReader(const char *fileName);
	~FileReader();
	
	// seeking
	void reset();
	bool isFinished();
	void advance(int count);
	
	// peeking
	bool check(int offset, std::string name);
	std::string seek(int &offset, std::vector<char> const &stops);
	std::string seek(int &offset, char stop);
	std::string peek(int &offset, std::vector<char> const &characters);
	std::string peek(int &offset, char character);
	
	// parsing
	bool race(int &offset, std::vector<char> const &delimiters, std::vector<char> const &stops);
	std::string peekDigits(int &offset);
	std::string peekFloat(int &offset);
	std::vector<std::string> peekFloats(int &offset, std::vector<char> const &delimiters, std::vector<char> const &stops);
};

#endif