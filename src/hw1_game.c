#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "hw1.h"

int main(int argc, char **argv) {
    assert(argc == 4); //we want a size, board state, and a list of keys
	initialize_board(argv[2], argv[3], (int)strtoul(argv[1], NULL, 10));	

    int size = (int)strtoul(argv[1], NULL, 10);
    if (!initialize_board(argv[2], argv[3], size)) {
        printf("Invalid initial board state.\n");
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
                printf("Invalid choice. Choose a piece (1 - %d) or q to quit: ", size);
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
            printBoard(size);
            printf("Congratulations, you have filled the board!\n");
            break;
        }
    }
    return 0;
}