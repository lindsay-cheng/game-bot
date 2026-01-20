// reversi public api
#ifndef REVERSI_H
#define REVERSI_H

#include <stdbool.h>

// Function Declarations for Part 1
void printBoard(char board[][26], int n);
bool positionInBounds(int n, int row, int col);
bool checkLegalInDirection(char board[][26], int n, int row, 
                           int col, char colour, int deltaRow, int deltaCol);

// Function Declaration for Part 2
int makeMove(const char board[][26], int n, char turn, int *row, int *col);

#endif
