#include <stdbool.h>
#include <stdio.h>

#include "reversi.h"
#include "reversi_board.h"

int main(void) {
    char board[26][26];
    char computerColor;
    int n;
    char rowChar, colChar;

    printf("Enter the board dimension: ");
    scanf("%d", &n);

    printf("Computer plays (B/W) : ");
    scanf(" %c", &computerColor);

    initializeBoard(board, n);
    printBoard(board, n);

    char currentPlayer = BLACK;
    char opponent = WHITE;

    bool gameOver = false;

    int numberOfMoves = 4; // keep track of number of pieces placed + 4 center pieces

    // start game loop; don't forget to add a space before scanf lines if there is a previous scanf with a newline character
    while (!gameOver) {
        bool moveAvailable = movesAvailable(board, n, currentPlayer);
        int bestRow, bestCol;

        // move available (both will be for B)
        if (moveAvailable) {

            // computer move
            if (computerColor == currentPlayer) {
                // makeComputerMove(board, n, currentPlayer);

                if (makeMove(board, n, currentPlayer, &bestRow, &bestCol)) {
                    executeMove(board, n, currentPlayer, bestRow, bestCol);
                    printf("Computer places %c at %c%c.\n", currentPlayer, bestRow + 'a', bestCol + 'a');
                }

                printBoard(board, n);
                changeTurnAndIncrement(&numberOfMoves, &currentPlayer, &opponent);

            // player move
            } else {
                bool validMove = false;
                while (!validMove) {
                    printf("Enter move for %c (row col): ", currentPlayer);
                    scanf(" %c%c", &rowChar, &colChar);

                    int row = rowChar - 'a';
                    int col = colChar - 'a';

                    if (positionInBounds(n, row, col) && isValidMove(board, n, row, col, currentPlayer)) {
                        executeMove(board, n, currentPlayer, row, col);
                        printf("Player places %c at %c%c.\n", currentPlayer, rowChar, colChar);
                        validMove = true;
                    } else {
                        printf("Invalid move.\n");
                    }
                }

                printBoard(board, n);
                changeTurnAndIncrement(&numberOfMoves, &currentPlayer, &opponent);
            }

        // if move not available
        } else if (!moveAvailable) {

            // if board is not full (pieces placed < n^2)
            if (numberOfMoves < n*n) {

                // if other player has moves, skip current player's turn
                if (movesAvailable(board, n, opponent)) {
                    printf("%c player has no valid move.\n", currentPlayer);
                    // switch turns without incrementing piece count
                    currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK;
                    opponent = (opponent == WHITE) ? BLACK : WHITE;

                // if other player has no moves either, draw
                } else if (!movesAvailable(board, n, opponent)) {
                    printf("Draw!\n");
                    gameOver = true;
                }

            // if board is full
            } else if (numberOfMoves == n*n) {

                // if #B pieces == #W pieces, draw
                if (countPieces(board, n, currentPlayer) == countPieces(board, n, opponent)) {
                    printf("Draw!\n");
                    gameOver = true;

                // if #B pieces != #W pieces, print win statement for player with more pieces
                } else if (countPieces(board, n, currentPlayer) > countPieces(board, n, opponent)) {
                    printf("%c player wins.\n", currentPlayer);
                    gameOver = true;

                } else if (countPieces(board, n, currentPlayer) < countPieces(board, n, opponent)) {
                    printf("%c player wins.\n", opponent);
                    gameOver = true;
                }
            }
        }
    }

    return 0;
}
