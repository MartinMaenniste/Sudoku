#include "Sudoku.h"
#include <iostream>

void Sudoku::init()
{
	genEmptySudoku();
}
void Sudoku::printField()
{
	/*
	If a number is already in the field, it is displayed as that number
	Empty field is displayed as ' '
	*/
	printf("  1 2 3  4 5 6  7 8 9");
	int rowNum = 1;
	for(int i = 0; i < FIELDSIZE; i++)
	{
		if ( i % 27 == 0 && i > 9 ) printf("\n --------------------");
		if( i % 9 == 0 ) 
		{ 
			printf("\n%d ", rowNum); 
			rowNum++;
		}
		if ( i % 3 == 0 && i % 9 != 0 ) printf("|");
		if( mField[i] == -1 ) printf(" ");
		else
		{
			printf("%d", mField[i]);
		}
		// Add space after every number to read better
		printf(" ");
	}
	printf("\n");
}
void Sudoku::makeMove()
{
	char ch = getMoveType("Do you want to add or remove a number from the field?\n(a/r): ");
	if ( ch == 'r' )
	{
		int row = getNum("Choose row: ") - 1;
		int col = getNum("Choose column: ") - 1;
		removeNumFromField(row, col);
	}
	else if ( ch == 'a' )
	{
		int num = getNum("Choose number to add: ");
		int row = getNum("Choose row: ") - 1;
		int col = getNum("Choose column: ") - 1;
		addNumToField(row, col, num);
	}
}
bool Sudoku::isCompleted()
{
	for(int i = 0; i < FIELDSIZE; i++)
	{
		if (mField[i] == -1) return false;
	}
	return true;
}

void Sudoku::genEmptySudoku()
{
	mField = std::make_unique<int[]>(FIELDSIZE);
	for(int i = 0; i < FIELDSIZE; i++)
	{
		mField[i] = -1;
	}
}
bool Sudoku::checkIfNumInColumn(const int col, const int num)
{
	// Both the column and the number can only be between 0 and 9 (including)
	if(col < 0 || col > 9 || num < 0 || num > 9) {return true;}

	for (int i = col; i < FIELDSIZE-7+col; i+=9)
	{
		if (mField[i] == num) {return true;}
	}
	return false;
}
bool Sudoku::checkIfNumInRow(const int row, const int num)
{
	// Both the column and the number can only be between 0 and 9 (including)
	if(row < 0 || row > 9 || num < 0 || num > 9) {return true;}

	for (int i = row*9; i < 9*(row+1); i++)
	{
		if (mField[i] == num) {return true;}
	}
	return false;
}
bool Sudoku::checkIfNumInBox(const int row, const int col, const int num)
{
	// Set row and col to top left of this box

	int selectedAsIndex = getIndexFromRowCol(row, col);
	int boxIndex = ( selectedAsIndex%9 ) / 3 + 3*( selectedAsIndex/27 );
	int firstIndexInBox = 27 * ( boxIndex/3 ) + 3* ( boxIndex%3 );
	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			if ( mField[firstIndexInBox + i + 9*j] == num ) { return true; } 
		}
	}
	return false;
}
int Sudoku::getNum(const char* prompt)
{
	// In the context of a classical sudoku field, only suitable numbers are between 1 and 9 (including both)
	int num = -1;
	while(num > 9 || num < 1)
	{
		printf(prompt);
		std::scanf("%d", &num);

		int c;
		while( (c = std::getchar()) != '\n' && c != EOF );
	}
	return num;
}
char Sudoku::getMoveType(const char* prompt)
{
	char ch = '-';
	while(ch != 'r' && ch != 'a')
	{
		printf(prompt);
		std::scanf(" %c", &ch);
		
		int c;
		while( (c = std::getchar()) != '\n' && c != EOF );
	}
	return ch;
}
int Sudoku::getIndexFromRowCol(const int row, const int col)
{
	return row*9 + col;
}
void Sudoku::addNumToField(const int row, const int col, const int num)
{
	if ( checkIfNumInRow(row, num) )
	{
		printf("\n* Number already in the same row!\n\n");
		return;
	}
	if ( checkIfNumInColumn(col, num) )
	{
		printf("\n* Number already in the same column!\n\n");
		return;
	}
	if ( checkIfNumInBox(row, col, num) )
	{
		printf("\n* Number already in that 3x3 box!\n\n");
		return;
	}
	mField[getIndexFromRowCol(row, col)] = num;
}
void Sudoku::removeNumFromField(const int row, const int col)
{
	mField[getIndexFromRowCol(row, col)] = -1;
}
