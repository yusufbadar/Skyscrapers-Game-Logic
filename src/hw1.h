#ifndef HW1_H
#define HW1_H

#include <stdbool.h>

#define MAX_LENGTH 8

extern char board[MAX_LENGTH][MAX_LENGTH];
extern int top_key[MAX_LENGTH];
extern int bottom_key[MAX_LENGTH];
extern int left_key[MAX_LENGTH];
extern int right_key[MAX_LENGTH];

int initialize_board(const char *initial_state, const char *keys, int size);
void printBoard(int size);
int solve(const char *initial_state, const char *keys, int size);

bool checkRowDuplicates(int size);
bool checkColDuplicates(int size);
bool checkFilledRow(int size);
bool checkFilledCol(int size);
bool isCellEmpty(int row, int col);

#endif
