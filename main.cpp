#include "Sudoku.h"

int main()
{
	Sudoku sudoku = Sudoku();
	sudoku.init();
	while (!sudoku.isCompleted())
	{
		sudoku.printField();
		sudoku.makeMove();
	}

	return 0;
}
