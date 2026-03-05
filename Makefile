
all: run
	./run

run: main.o Sudoku.o
	g++ main.o Sudoku.o -o run

main.o: main.cpp
	g++ -c main.cpp

Sudoku.o: Sudoku.cpp
	g++ -c Sudoku.cpp
