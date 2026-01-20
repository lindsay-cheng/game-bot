#include <stdbool.h>
#include <stdio.h>

#include "reversi_board.h"

void initializeBoard(char board[][26], int n) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {

            if ((i == (n / 2) - 1 && j == (n / 2) - 1) || ((i == (n / 2) && j == (n / 2)))) {
                board[i][j] = WHITE;
            } else if ((i == (n / 2) - 1 && j == (n / 2)) || (i == (n / 2) && j == (n / 2) - 1)) {
                board[i][j] = BLACK;
            } else {
                board[i][j] = EMPTY;
            }
        }
    }
}

void printBoard(char board[][26], int n) {

    // print first line: "  abcd..."
    printf("  ");

    for (int i=0; i<n; i++) {
        printf("%c", 'a' + i); // ascii value for 'a' = 97, so increment it n times
    }

    printf("\n");

    // print the rows of board
    for (int i=0; i<n; i++) {
        printf("%c ", 'a' + i);
        for (int j=0; j<n; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

bool positionInBounds(int n, int row, int col) {
    if (row < 0 || row >= n || col < 0 || col >= n) return false;
    return true;
}

bool checkLegalInDirection(char board[][26], int n, int row, int col, char currentPlayer, int deltaRow, int deltaCol) {
    // the purpose of this function is to check from an EMPTY cell the legality of a move on that empty cell

    // early return cases
    // position must be unoccupied
    if (board[row][col] != EMPTY) {
        return false;
    }

    // both deltas cant be zero
    if (deltaRow == 0 && deltaCol == 0) {
        return false;
    }

    // set opponents color
    char opponent = (currentPlayer == WHITE) ? BLACK : WHITE;

    // take first step in direction
    row += deltaRow;
    col += deltaCol;

    // must have at least one opponents piece adjacent
    if (!positionInBounds(n, row, col) || board[row][col] != opponent) {
        return false;
    }

    while (positionInBounds(n, row, col)) {

        row += deltaRow;
        col += deltaCol;

        if (!positionInBounds(n, row, col)) {
            return false;
        }

        if (board[row][col] == EMPTY) {
            return false;
        }

        if (board[row][col] == currentPlayer) {
            return true;
        }
    }
    return false;
}

bool isValidMove(char board[][26], int n, int row, int col, char color) {
    // make sure current cell is unoccupied and in range
    if (board[row][col] != EMPTY || !positionInBounds(n, row, col)) {
        return false;
    }

    // checks 8 directions
    for (int deltaRow=-1; deltaRow<2; deltaRow++) {
        for (int deltaCol=-1; deltaCol<2; deltaCol++) {
            if (checkLegalInDirection(board, n, row, col, color, deltaRow, deltaCol)) {
                return true;
            }
        }
    }
    // if nothing is returned at the end, return false
    return false;
}

bool movesAvailable(char board[][26], int n, char color) {
    // calls isValidMove for each cell, returns true if current color has any available moves
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            if (isValidMove(board, n, i, j, color)) {
                return true;
            }
        }
    }
    return false;
}

void executeMove(char board[][26], int n, char color, int row, int col) {

    // this function assumes the move entered is valid
    // validity of the move is checked in main

    char opponent = (color == WHITE) ? BLACK : WHITE;

    // flip all sandwiched opponent pieces, as well as any other valid 
    // first check in which directions pieces should be flipped
    for (int deltaRow=-1; deltaRow<2; deltaRow++) {
        for (int deltaCol=-1; deltaCol<2; deltaCol++) {

            if (deltaRow == 0 && deltaCol == 0) {
                continue; // skips if both directions are zero
            }

            // if legal in curr direction, flip all sandwiched cells
            // checkLegalInDirection checks legality of the curr EMPTY cell, so we have to iterate in that direction until we find the same color piece as being played, then flip all opponent pieces

            int curRow = row;
            int curCol = col;

            if (checkLegalInDirection(board, n, row, col, color, deltaRow, deltaCol)) {

                // move to the adjacent cell first
                curRow += deltaRow;
                curCol += deltaCol;

                // runs until we reached our own color
                while (board[curRow][curCol] == opponent) {
                    board[curRow][curCol] = color;
                    curRow += deltaRow;
                    curCol += deltaCol;
                }
            }
        }
    }

    // place piece at the very end so that previous conditional checks are not messed up
    board[row][col] = color;
}

int countPieces(char board[][26], int n, char color) {
    int sum = 0;
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            if (board[i][j] == color) {
                sum++;
            }
        }
    }
    return sum;
}

void changeTurnAndIncrement(int* numberOfMoves, char* currentPlayer, char* opponent) {
    // small function that runs every time a piece is placed by either player
    // made for readability in main
    (*numberOfMoves)++;
    *currentPlayer = (*currentPlayer == BLACK) ? WHITE : BLACK;
    *opponent = (*opponent == WHITE) ? BLACK : WHITE;
}
