#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "hw1.h"

int top_key[MAX_LENGTH] = {0};
int bottom_key[MAX_LENGTH] = {0};
int left_key[MAX_LENGTH] = {0};
int right_key[MAX_LENGTH] = {0};


bool checkRowDuplicates(int size);
bool checkColDuplicates(int size);
bool checkFilledRow(int size);
bool checkFilledCol(int size);

char board[MAX_LENGTH][MAX_LENGTH] = {0};

int length = 5;


int initialize_board(const char *initial_state, const char *keys, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j] = initial_state[i * size + j];
		}
	}

	for (int i = 0; i < size; i++) {
		top_key[i] = keys[i] - '0';
	}
	for (int i = 0; i < size; i++) {
		bottom_key[i] = keys[size + i] - '0';
	}
	for (int i = 0; i < size; i++) {
		left_key[i] = keys[2 * size + i] - '0';
	}
	for (int i = 0; i < size; i++) {
		right_key[i] = keys[3 * size + i] - '0';
	}

	if (!checkRowDuplicates(size) || !checkColDuplicates(size)) {
		return 0;
	}

	if (!checkFilledRow(size) || !checkFilledCol(size)) {
		return 0;
	}

	return 1;
}

bool checkRowDuplicates(int size) {
	for (int i = 0; i < size; i++) {
		int found[10] = {0};
		for (int j = 0; j < size; j++) {
			char ch = board[i][j];
			if (ch != '-') {
				int num = ch - '0';
				if (found[num]) {
					return false;
				}
				found[num] = 1;
			}
		}
	}
	return true;
}

bool checkColDuplicates(int size) {
	for (int j = 0; j < size; j++) {
		int found[10] = {0};
		for (int i = 0; i < size; i++) {
			char ch = board[i][j];
			if (ch != '-') {
				int num = ch - '0';
				if (found[num]){
					return false;
				}
				found[num] = 1;
			}
		}
	}
	return true;
}

bool checkFilledRow(int size) {
	for (int i = 0; i < size; i++) {
		bool rowFilled = true;
		for (int j = 0; j < size; j++) {
			if (board[i][j] == '-') {
				rowFilled = false;
				break;
			}
		}
		if (rowFilled) {
			int positionsLeft = 0, maxLeft = 0;
			for (int j = 0; j < size; j++) {
				int height = board[i][j];
				if (height > maxLeft) {
					positionsLeft++;
					maxLeft = height;
				}
			}
			if (left_key[i] != 0 && positionsLeft != left_key[i]) {
				return false;
			}
			int positionsRight = 0, maxRight = 0;
			for (int j = size - 1; j >= 0; j--) {
				int height = board[i][j] - '0';
				if (height > maxRight) {
					positionsRight++;
					maxRight = height;
				}
			}
			if (right_key[i] != 0 && positionsRight != right_key[i]) {
				return false;
			}
		}
	}
	return true;
}

bool checkFilledCol(int size) {
	for (int j = 0; j < size; j++) {
		bool columnFilled = true;
		for (int i = 0; i < size; i++) {
			if (board[i][j] == '-') {
				columnFilled = false;
				break;
			}
		}
		if (columnFilled) {
			int countTop = 0, maxTop = 0;
			for (int i = 0; i < size; i++) {
				int height = board[i][j];
				if (height > maxTop) {
					countTop++;
					maxTop = height;
				}
			}
			if (top_key[j] != 0 && countTop != top_key[j]) {
				return false;
			}
			int countBottom = 0, maxBottom = 0;
			for (int i = size - 1; i >= 0; i--) {
				int height = board[i][j] - '0';
				if (height > maxBottom) {
					countBottom++;
					maxBottom = height;
				}
			}
			if (bottom_key[j] != 0 && countBottom != bottom_key[j]) {
				return false;
			}
		}
	}
	return true;
}

bool isCellEmpty(int row, int col) {
    return board[row][col] == '-';
}

void printBoard(int size) {
	printf("    ");
	for (int i = 0; i < size; i++) {
		printf("%d ", top_key[i]);
	}
	printf("\n");

	printf("    ");
	for (int i = 0; i < size; i++) {
		printf("v ");
	}
	printf("\n");

	for (int i = 0; i < size; i++) {
		printf("%d > ", left_key[i]);
		for (int j = 0; j < size; j++){
			printf("%c ", board[i][j]);
		}
		printf("< %d", right_key[i]);
		printf("\n");
	}

	printf("    ");
	for (int i = 0; i < size; i++) {
		printf("^ ");
	}
	printf("\n");

	printf("    ");
	for (int i = 0; i < size; i++) {
		printf("%d ", bottom_key[i]);
	}
	printf("\n");
}

int solve(const char *initial_state, const char *keys, int size){
	(void) initial_state;
	(void) keys;
	(void) size;
	
	return 1;
	
}