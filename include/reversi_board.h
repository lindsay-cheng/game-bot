#ifndef REVERSI_BOARD_H
#define REVERSI_BOARD_H

#include <stdbool.h>

#define EMPTY 'U'
#define WHITE 'W'
#define BLACK 'B'

void initializeBoard(char board[][26], int n);
void printBoard(char board[][26], int n);
bool positionInBounds(int n, int row, int col);
bool checkLegalInDirection(char board[][26], int n, int row, int col, char color, int deltaRow, int deltaCol);
bool isValidMove(char board[][26], int n, int row, int col, char color);
bool movesAvailable(char board[][26], int n, char color);
void executeMove(char board[][26], int n, char color, int row, int col);
int countPieces(char board[][26], int n, char color);
void changeTurnAndIncrement(int* numberOfMoves, char* currentPlayer, char* opponent);

#endif
