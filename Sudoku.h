#include<cstdio>
#include<cstdlib>
#include<memory>
#include<time.h>

#define FIELDSIZE 81

class Sudoku {
public:
    Sudoku() = default;
    ~Sudoku() = default;

    void init();

    void printField();
    void makeMove();
    bool isCompleted();

private:
    std::unique_ptr<int[]> mField;
    std::unique_ptr<int[]> mInitialisedField;

    void genEmptySudoku();
    void addRandomNumbers(const int n);

    bool checkIfNumInColumn(const int col, const int num);
    bool checkIfNumInRow(const int row, const int num);
    bool checkIfNumInBox(const int row, const int col, const int num);

    int getNum(const char* prompt);
    char getMoveType(const char* prompt);
    int getIndexFromRowCol(const int row, const int col);

    void addNumToField(const int row, const int col, const int num); // Function internally gives reason if can't put number on that field
    void removeNumFromField(const int row, const int col);
};