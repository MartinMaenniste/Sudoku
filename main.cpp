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
	printf("\n***********\n* Solved! *\n***********\n\n");
	sudoku.printField();

	return 0;
}
