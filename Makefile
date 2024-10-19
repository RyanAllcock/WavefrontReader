CXX := g++
SRC := source/
BIN := temp/
OBJ := $(BIN)filereader.o $(BIN)objreader.o $(BIN)mtlreader.o

main: main.cpp $(OBJ)
	$(CXX) -o main.exe $(OBJ) main.cpp

test: test.cpp $(BIN)filereader.o
	$(CXX) -o test.exe $(BIN)filereader.o test.cpp

$(BIN)filereader.o: $(SRC)filereader/filereader.cpp $(SRC)filereader/filereader.hpp
	$(CXX) -c -o $(BIN)filereader.o $(SRC)filereader/filereader.cpp

$(BIN)objreader.o: $(SRC)objreader.cpp $(SRC)wavefrontreader.hpp $(BIN)filereader.o
	$(CXX) -c -o $(BIN)objreader.o $(SRC)objreader.cpp

$(BIN)mtlreader.o: $(SRC)mtlreader.cpp $(SRC)wavefrontreader.hpp $(BIN)filereader.o
	$(CXX) -c -o $(BIN)mtlreader.o $(SRC)mtlreader.cpp