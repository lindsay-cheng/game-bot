#include <stdbool.h>

#include "reversi.h"
#include "reversi_board.h"

#include <sys/time.h>
#include <sys/resource.h>

static const int positionValues[8][8] = {
    {100, -20,  10,   5,   5,  10, -20, 100},
    {-20, -50,  -2,  -2,  -2,  -2, -50, -20},
    { 10,  -2,   5,   1,   1,   5,  -2,  10},
    {  5,  -2,   1,   0,   0,   1,  -2,   5},
    {  5,  -2,   1,   0,   0,   1,  -2,   5},
    { 10,  -2,   5,   1,   1,   5,  -2,  10},
    {-20, -50,  -2,  -2,  -2,  -2, -50, -20},
    {100, -20,  10,   5,   5,  10, -20, 100}
};

static int evaluateBoardState(const char board[][26], int n, char color);
static void copyBoard(const char board[][26], char boardCopy[][26], int n);
static void makeMoveOnCopy(char board[][26], int n, int row, int col, char color);
static int minimax(char board[][26], int n, char player, char turn, int depth, int alpha, int beta);

static int evaluateBoardState(const char board[][26], int n, char color) {
    // 1. calculate score sum based on piece count and position
    // 2. calculate mobility score based on number of moves available
    // 3. calculate score

    int score = 0;
    char opponent = (color == WHITE) ? BLACK : WHITE;

    // sum score for current board state, adds points for pieces of curr player's color, subtracts for opponent
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            if (board[i][j] == color) {
                if (n == 8) {
                    score += positionValues[i][j];
                } else {
                    score++;
                }
            } else if (board[i][j] == opponent) {
                if (n == 8) {
                    score -= positionValues[i][j];
                } else {
                    score--;
                }
            }
        }
    }

    // calculate mobility (number of moves available for curr player) by counting number of legal moves for both players
    int currPlayerMoves = 0, opponentMoves = 0;

    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            if (board[i][j] == EMPTY && isValidMove(board, n, i, j, color)) {
                currPlayerMoves++;
            } else if (board[i][j] == EMPTY && isValidMove(board, n, i, j, opponent)) {
                opponentMoves++;
            }
        }
    }
    score += (currPlayerMoves - opponentMoves) * 5;
    return score;
}

static void copyBoard(const char board[][26], char boardCopy[][26], int n) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            boardCopy[i][j] = board[i][j];
        }
    }
}

// make a move on the board copy and update it
static void makeMoveOnCopy(char board[][26], int n, int row, int col, char color) {
    // Place the piece
    board[row][col] = color;

    // flip pieces in all directions
    for (int deltaRow=-1; deltaRow<2; deltaRow++) {
        for (int deltaCol=-1; deltaCol<2; deltaCol++) {
            if (deltaRow == 0 && deltaCol == 0) {
                continue;
            }
            // check if this direction would cause a flip
            if (checkLegalInDirection(board, n, row, col, color, deltaRow, deltaCol)) {
                // flip pieces in this direction
                int r = row + deltaRow;
                int c = col + deltaCol;
                while (r >= 0 && r < n && c >= 0 && c < n && board[r][c] != EMPTY && board[r][c] != color) {
                    board[r][c] = color;
                    r += deltaRow;
                    c += deltaCol;
                }
            }
        }
    }
}

// minimax function for finding the best move
static int minimax(char board[][26], int n, char player, char turn, int depth, int alpha, int beta) {
    // base case: reached maximum depth or game over
    if (depth == 0) {
        return evaluateBoardState(board, n, player);
    }

    char opponent = (turn == BLACK) ? WHITE : BLACK;
    bool hasValidMove = false;

    // maximizing player (if turn is the player were optimizing for)
    if (turn == player) {
        int maxScore = -1000000;

        // try each possible move
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 'U' && isValidMove(board, n, i, j, player)) {
                    hasValidMove = true;

                    // make copy of board then make the move on it
                    char boardCopy[26][26];
                    copyBoard(board, boardCopy, n);
                    makeMoveOnCopy(boardCopy, n, i, j, turn);

                    // recursively find best opponent response
                    int score = minimax(boardCopy, n, player, opponent, depth - 1, alpha, beta);

                    // update best score
                    maxScore = (score > maxScore) ? score : maxScore;

                    // alpha beta pruning
                    alpha = (alpha > score) ? alpha : score;
                    if (beta <= alpha) {
                        break; // beta cutoff
                    }
                }
            }
            // additional break for beta cutoff
            if (beta <= alpha) {
                break;
            }
        }

        // if no valid move exists pass turn to opponent
        if (!hasValidMove) {
            // if neither player can move, evaluate the board
            if (!movesAvailable(board, n, opponent)) {
                return evaluateBoardState(board, n, player);
            }
            // otherwise, pass the turn and decrease the depth
            return minimax(board, n, player, opponent, depth - 1, alpha, beta);
        }

        return maxScore;
    }
    // minimizing player (opponents turn)
    else {
        int minScore = 1000000;

        // try each possible move
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 'U' && isValidMove(board, n, i, j, turn)) {
                    hasValidMove = true;

                    // make a copy of board then make move on it
                    char boardCopy[26][26];
                    copyBoard(board, boardCopy, n);
                    makeMoveOnCopy(boardCopy, n, i, j, turn);

                    // recursively find best response by decreasing depth each iteration
                    int score = minimax(boardCopy, n, player, opponent, depth - 1, alpha, beta);

                    // update best score
                    minScore = (score < minScore) ? score : minScore;

                    // alpha beta pruning
                    beta = (beta < score) ? beta : score;
                    if (beta <= alpha) {
                        break; // alpha cutoff
                    }
                }
            }
            // additional break for alpha cutoff
            if (beta <= alpha) {
                break;
            }
        }

        // if no valid move pass turn to player
        if (!hasValidMove) {
            // If neither can move => game over
            if (!movesAvailable(board, n, opponent)) {
                return evaluateBoardState(board, n, player);
            }
            // pass turn
            return minimax(board, n, player, opponent, depth - 1, alpha, beta);
        }

        return minScore;
    }
}

int makeMove(const char board[][26], int n, char turn, int *row, int *col) {
    struct rusage usage;
    struct timeval start, end;

    // get start time
    getrusage(RUSAGE_SELF, &usage);
    start = usage.ru_utime;
    double startTime = start.tv_sec + start.tv_usec / 1000000.0;

    // initialize best move
    int bestRow = -1, bestCol = -1;
    int bestScore = -1000000;
    char opponent = (turn == 'B') ? 'W' : 'B';

    // find any valid move first as a fallback
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 'U' && isValidMove(board, n, i, j, turn)) {
                if (bestRow == -1) { // first valid move found
                    bestRow = i;
                    bestCol = j;

                    // if this is a corner take it immediately
                    if ((i == 0 || i == n-1) && (j == 0 || j == n-1)) {
                        *row = i;
                        *col = j;
                        return 1;
                    }
                }
            }
        }
    }

    // iterative deepening by trying increasing depths until time runs out
    for (int depth = 1; depth <= 5; depth++) {
        int currentBestRow = -1, currentBestCol = -1;
        int currentBestScore = -1000000;

        // check each possible move with current depth
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 'U' && isValidMove(board, n, i, j, turn)) {
                    // make copy of board then make move on it
                    char boardCopy[26][26];
                    copyBoard(board, boardCopy, n);
                    makeMoveOnCopy(boardCopy, n, i, j, turn);

                    // use minimax to evaluate this move
                    int score = minimax(boardCopy, n, turn, opponent, depth, -1000000, 1000000);

                    // update current best move for this depth
                    if (score > currentBestScore) {
                        currentBestScore = score;
                        currentBestRow = i;
                        currentBestCol = j;
                    }
                    // if scores are equal prefer moves with lower row then lower column
                    else if (score == currentBestScore) {
                        if (i < currentBestRow || (i == currentBestRow && j < currentBestCol)) {
                            currentBestRow = i;
                            currentBestCol = j;
                        }
                    }

                    // check if time is running out
                    getrusage(RUSAGE_SELF, &usage);
                    end = usage.ru_utime;
                    double currentTime = end.tv_sec + end.tv_usec / 1000000.0;
                    if (currentTime - startTime > 0.8) {
                        // time limit approaching use the best move found so far
                        goto timeOut;
                    }
                }
            }
        }

        // update overall best move with results from curr depth
        if (currentBestRow != -1) {
            bestRow = currentBestRow;
            bestCol = currentBestCol;
            bestScore = currentBestScore;
        }
    }

timeOut:
    // set the chosen move
    if (bestRow != -1) {
        *row = bestRow;
        *col = bestCol;
        return 1; // valid move found
    }

    // if no valid move found, end
    return 0;
}
