#include "Sudoku.h"

void Sudoku::init()
{
	genEmptySudoku();
}
void Sudoku::printField()
{
	/*
	If a number is already in the field, it is displayed as that number
	Empty field is displayed as " "
	*/
	printf("  1 2 3  4 5 6  7 8 9");
	int rowNum = 1;
	for(int i = 0; i < FIELDSIZE; i++)
	{
		if ( i % 27 == 0 && i > 9 ) printf("\n --------------------"); // To show 3x3 box grid
		if( i % 9 == 0 ) 
		{ 
			printf("\n%d ", rowNum);  // Numbering to input rows easier while playing
			rowNum++;
		}
		if ( i % 3 == 0 && i % 9 != 0 ) printf("|"); // To show 3x3 box grid
		if( mField[i] == -1 ) printf(" ");
		else
		{
			if(mInitialisedField[i] != -1)
			{ // For linux terminals, set immutable numbers to red
				printf("\033[31m%d\033[0m", mField[i]);
			}
			else
			{
				printf("%d", mField[i]);
			}
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
		int row = getSudokuNum("Choose row: ") - 1;
		int col = getSudokuNum("Choose column: ") - 1;
		removeNumFromField(row, col);
	}
	else if ( ch == 'a' )
	{
		int num = getSudokuNum("Choose number to add: ");
		int row = getSudokuNum("Choose row: ") - 1;
		int col = getSudokuNum("Choose column: ") - 1;
		addNumToField(row, col, num);
	}
}
bool Sudoku::isCompleted()
{
	// Function assumes no illegal move was made, ever.

	for(int i = 0; i < FIELDSIZE; i++)
	{
		if (mField[i] == -1) return false;
	}
	return true;
}

void Sudoku::genEmptySudoku()
{
	mField = std::make_unique<int[]>(FIELDSIZE);
	mInitialisedField = std::make_unique<int[]>(FIELDSIZE);
	for(int i = 0; i < FIELDSIZE; i++)
	{
		mField[i] = -1;
		mInitialisedField[i] = -1;
	}
	int n = getBigNum("How many random numbers to add to the field?\n");
	addRandomNumbers(n);
}
void Sudoku::addRandomNumbers(const int n)
{
	std::srand(time(NULL));

	if(n > 30) // Very unlikely to get solvable field at aroud 30...40 filled numbers
	{
		// Fake generation by adding 30 random numbers, solving and removing randomly until at the desired number.

		addRandomNumbers(30);
		solveSudoku(0);
		while(howManyFilledSlots() > n)
		{
			int index = rand() % 81;
			// if (mInitialisedField[i] != -1) continue; // If solving algorithm has a pattern, remove comment at start of line.
			mField[index] = -1;
		}
		for(int i = 0; i < FIELDSIZE; i++)
		{
			mInitialisedField[i] = mField[i];
		}
		return;
	}

	int count = 0; // Count the added numbers manually since random choice can hit the same index multiple times
	while(true)
	{
		while (count < n)
		{	
			int index = rand() % 81;
			int num = rand() % 9;

			if(!numCanBeAtIndex(num, index))
			{
				continue;
			}
			mInitialisedField[index] = num;
			mField[index] = num;
			count++;
		}
		// After generating n random numbers, check for validity and reset if needed

		if(isSolvable()) return;

		for(int i = 0; i < FIELDSIZE; i++)
		{
				mInitialisedField[i] = -1;
				mField[i] = -1;
				count = 0;
		}
	}
}
bool Sudoku::checkIfNumInColumn(const int col, const int num)
{
	if(col < 0 || col > 8 || num < 1 || num > 9) {return true;}

	for (int i = col; i < FIELDSIZE-7+col; i+=9) // Move through the column - helps to look at field with filled indexes
	{
		if (mField[i] == num) return true;
	}
	return false;
}
bool Sudoku::checkIfNumInRow(const int row, const int num)
{
	if(row < 0 || row > 8 || num < 1 || num > 9) {return true;}

	for (int i = row*9; i < 9*(row+1); i++) // Move through the row - helps to look at field with filled indexes
	{
		if (mField[i] == num) return true;
	}
	return false;
}
bool Sudoku::checkIfNumInBox(const int row, const int col, const int num)
{
	// Set row and col to top left of this box
	/* 
	Every 4th row is a multiple of 27 (floor division by 27 gets 3x3 box's row index)
	Every row is a multiple of 9
	Every field index % 9 gets the column index - then floor division by 3 to get box column index
	After seeing all that, they can be combined to build an index for the box.
	Then it can be made into the first index of the box by "scaling it back up"

	Helps to look at field with filled indexes while looking at this.
	*/

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
int Sudoku::getSudokuNum(const char* prompt)
{
	// In the context of a classical sudoku field, only suitable numbers are between 1 and 9 (including both)
	// The caller of this function probably wants to -= 1 from the value to get indexing from 0
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
int Sudoku::getBigNum(const char* prompt)
{
	// Used to generate random numbers to field. 0...81 (both including) are allowed
	int num = -1;
	while(num > 81 || num < 0)
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
	char ch = '-'; // Just a random default character!
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
	if(mInitialisedField[getIndexFromRowCol(row, col)] != -1)
	{
		printf("\n* Can't overwrite that field!\n\n");
		return;
	}
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
	if(mInitialisedField[getIndexFromRowCol(row, col)] != -1)
	{
		printf("\n* Can't remove that field!\n\n");
		return;
	}
	mField[getIndexFromRowCol(row, col)] = -1;
}
bool Sudoku::isSolvable()
{
	// Solve and reset, extracting the value.

	bool solvable = solveSudoku(0);
	for(int i = 0; i < FIELDSIZE; i++)
	{
		mField[i] = mInitialisedField[i];
	}
	return solvable;
}
bool Sudoku::solveSudoku(const int index)
{
	// Recursively try 1...9 at every index until solved

	if(index == FIELDSIZE) return true;
	if(mInitialisedField[index] != -1) return solveSudoku(index + 1);

	for(int i = 1; i < 10; i++)
	{
		if (numCanBeAtIndex(i, index)) 
		{
			mField[index] = i;
			// Only return if it's solved, next iteration of loop might get solved field!
			if(solveSudoku(index+1)) return true;
			mField[index] = -1;
		}
	}
	return false;
}
bool Sudoku::numCanBeAtIndex(const int num, const int index)
{
	const int row = index / 9;
	const int col = index % 9;

	if ( mInitialisedField[index] != -1 ) return false;
	if ( checkIfNumInRow(row, num) ) return false;
	if ( checkIfNumInColumn(col, num) ) return false;
	if ( checkIfNumInBox(row, col, num) ) return false;
	return true;
}
int Sudoku::howManyFilledSlots()
{
	int counter = 0;
	for(int i = 0; i < FIELDSIZE; i++)
	{
		if(mField[i] != -1) counter++;
	}
	return counter;
}