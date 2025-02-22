#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "hw1.h"
#define MAX_LENGTH 8

extern char board[MAX_LENGTH][MAX_LENGTH];
extern int top_key[MAX_LENGTH];
extern int bottom_key[MAX_LENGTH];
extern int left_key[MAX_LENGTH];
extern int right_key[MAX_LENGTH];

int initialize_board(const char *initial_state, const char *keys, int size);
void printBoard(int size);
int solve(const char *initial_state, const char *keys, int size);

bool isCellEmpty(int row, int col);
bool checkRowDuplicates(int size);
bool checkColDuplicates(int size);
bool checkFilledRow(int size);
bool checkFilledCol(int size);

int main(int argc, char **argv) {
    assert(argc == 4); //we want a size, board state, and a list of keys
	initialize_board(argv[2], argv[3], (int)strtoul(argv[1], NULL, 10));	

    int size = (int)strtoul(argv[1], NULL, 10);
    if (!initialize_board(argv[2], argv[3], size)) {
        printf("Invalid initial board state.\n");
        return 0;
    }
    char input [20];
    int piece;
    int row, col;
    
    while(true) {
        printBoard(size);

        int firstPiece = 1;
        while (1) {
            if (firstPiece) {
                printf("Choose a piece (1-%d) or q to quit: ", size);
                firstPiece = 0;
            } else {
                printf("Invalid choice. Choose a piece (1-%d) or q to quit: ", size);
            }
            scanf("%19s", input);
            if (input[0] == 'q' || input[0] == 'Q') {
                return 0;
            }
            piece = atoi(input);
            if (piece >= 1 && piece <= size) {
                break;
            }
        }

        int firstRow = 1;
        while (1) {
            if (firstRow) {
                printf("Choose a row (0-%d): ", size - 1);
                firstRow = 0;
            } else {
                printf("Invalid choice. Choose a row (0-%d): ", size - 1);
            }
            if (scanf("%d", &row) != 1) {
                while (getchar() != '\n');
                continue;
            }
            if (row >= 0 && row < size) {
                break;
            }
        }

        int firstCol = 1;
        while(1) {
            if (firstCol) {
                printf("Choose a column (0-%d): ", size - 1);
                firstCol = 0;
            } else {
                printf("Invalid choice. Choose a column (0-%d): ", size - 1);
            }
            if (scanf("%d", &col) != 1) {
                while (getchar() != '\n');
                continue;
            }
            if (col >= 0 && col < size) {
                break;
            }
        }

        if (!isCellEmpty(row, col)) {
            printf("Invalid choice. That space is already occupied.\n");
            continue;
        }

        char original = board[row][col];
        board[row][col] = piece + '0';

        if (!checkRowDuplicates(size) || !checkColDuplicates(size)) {
            printf("Invalid choice. There is already a building with that height in that row or column.\n");
            board[row][col] = original;
            continue;
        }
        
        if (!checkFilledRow(size) || !checkFilledCol(size)) {
            printf("Invalid choice. You violate one of the key requirements.\n");
            board[row][col] = original;
            continue;
        }

        bool filled = true;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == '-') {
                    filled = false;
                    break;
                }
            }
            if (!filled) break;
        }
        if (filled) {
            printf("Congratulations, you have filled the board!\n");
            printBoard(size);
            break;
        }
    }
    return 0;
}