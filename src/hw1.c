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

bool edgeClueInitialization(int size, bool possible[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH+1]);
bool constraintPropagation(int size, bool possible[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH+1]);
bool processOfElimination(int size, bool possible[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH+1]);
bool clueElimination(int size, bool possible[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH+1]);

char board[MAX_LENGTH][MAX_LENGTH] = {0};

#define MAX_SIZE 8

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
	if (!initialize_board(initial_state, keys, size)) {
		return 0;
	}
	bool constraintList[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH+1];
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] != '-') {
				int value = board[i][j] - '0';
				for (int k = 0; k <= size; k++) {
					constraintList[i][j][k] = (k == value);
				}
			} else {
				for (int k = 1; k <= size; k++) {
					constraintList[i][j][k] = true;
				}
			}
		}
	}

	bool forward = true;
	int attempts = 0;
	const int maxAttempts = 1000;
	while (forward && attempts < maxAttempts) {
		forward = false;
		attempts++;

		if (edgeClueInitialization(size, constraintList)) {
			forward = true;
		}
		if (constraintPropagation(size, constraintList)) {
			forward = true;
		}
		if (processOfElimination(size, constraintList)) {
			forward = true;
		}
		if (clueElimination(size, constraintList)) {
			forward = true;
		}
	}

	bool solved = true;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int count = 0;
			int canditate = 0;
			for (int k = 1; k <= size; k++) {
				if (constraintList[i][j][k]) {
					count++;
					canditate = k;
				}
			}
			if (count == 1) {
				board[i][j] = canditate + '0';
			} else {
				solved = false;
			}
		}
	}
	if (!solved) {
		return 0;
	}
	printBoard(size);
	return 1;
}

bool edgeClueInitialization(int size, bool constraintList[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH + 1]){
	bool different = false;
	int crossOff;

	for (int col = 0; col < size; col++) {
		int clue = top_key[col];
		if (clue == 1) {
			for (int k = 1; k <= size; k++) {
				bool newValue = (k == size);
				if (constraintList[0][col][k] != newValue) {
					constraintList[0][col][k] = newValue;
					different = true;
				}
			}
		} else if (clue == size) {
			for (int row = 0; row < size; row++) {
				for (int k = 1; k <= size; k++) {
					bool newValue = (k == row +1);
					if (constraintList[row][col][k] != newValue) {
						constraintList[row][col][k] = newValue;
						different = true;
					}
				}
			}
		} else if (clue > 1 && clue < size) {
			for (int row = 0; row < size; row++) {
				int d = row;
				crossOff = size - clue + 2 + d;
				for (int k = crossOff; k <= size; k++){
					if (constraintList[row][col][k]) {
						constraintList[row][col][k] = false;
						different = true;
					}
				}
			}
		}
	}

	for (int col = 0; col < size; col++) {
		int clue = bottom_key[col];
		if (clue == 0) {
			continue;
		}
		if (clue == 1) {
			for (int k = 1; k <= size; k++) {
				bool newValue = (k == size);
				if (constraintList[size-1][col][k] != newValue) {
					constraintList[size - 1][col][k] = newValue;
					different = true;
				}
			}
		} else if (clue == size) {
			for (int row = 0; row < size; row++) {
				int goal = row + 1;
				int r = size - 1 - row;
				for (int k = 1; k <= size; k++) {
					bool newValue = (k == goal);
					if (constraintList[r][col][k] != newValue) {
						constraintList[r][col][k] = newValue;
						different = true;
					}
				}
			}
		} else if (clue > 1 && clue < size) {
			for (int d = 0; d < size; d++) {
				int r = size - 1 - d;
				crossOff = size - clue + 2 + d;
				for (int k = crossOff; k <= size; k++) {
					if (constraintList[r][col][k]) {
						constraintList[r][col][k] = false;
						different = true;
					}
				}
			}
		}
	}

	for (int row = 0; row < size; row++) {
		int clue = left_key[row];
		if (clue == 0) {
			continue;
		} if (clue == 1) {
			for (int k = 1; k <= size; k++) {
				bool newValue = (k == size);
				if (constraintList[row][0][k] != newValue) {
					constraintList[row][0][k] = newValue;
					different = true;
				}
			}
		} else if(clue == size) {
			for (int col = 0; col < size; col++) {
				for (int k = 1; k <= size; k++) {
					bool newValue = (k == col + 1);
					if (constraintList[row][col][k] != newValue) {
						constraintList[row][col][k] = newValue;
						different = true;
					}
				}
			}
		} else if (clue > 1 && clue < size) {
			for (int col = 0; col < size; col++) {
				int d = col;
				crossOff = size - clue + 2 + d;
				for (int k = crossOff; k <= size; k++) {
					if (constraintList[row][col][k]) {
						constraintList[row][col][k] = false;
						different = true;
					}
				}
			}
		}
	}

	for (int row = 0; row < size; row++) {
		int clue = right_key[row];
		if (clue == 0) {
			continue;
		}
		if (clue == 1) {
			for (int k = 1; k <= size; k++) {
				bool newValue = (k == size);
				if (constraintList[row][size-1][k] != newValue) {
					constraintList[row][size-1][k] = newValue;
					different = true;
				}
			}
		} else if (clue == size) {
			for (int col = 0; col < size; col++) {
				int goal = col + 1;
				int c = size - 1 - col;
				for (int k = 1; k <=size; k++) {
					bool newValue = (k == goal);
					if (constraintList[row][c][k] != newValue) {
						constraintList[row][c][k] = newValue;
						different = true;
					}
				}
			}
		} else if (clue > 1 && clue < size) {
			for (int d = 0; d < size; d++) {
				int c = size - 1 - d;
				crossOff = size - clue + 2 + d;
				for (int k = crossOff; k <= size; k++) {
					if (constraintList[row][c][k]) {
						constraintList[row][c][k] = false;
						different = true;
					}
				}
			}
		}
	}
	return different;
}

bool constraintPropagation(int size, bool constraintList[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH+1]) {
	bool different = false;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int count = 0, candidate = 0;
			for (int k = 1; k <= size; k++) {
				if (constraintList[i][j][k]) {
					count++;
					candidate = k;
				}
			}
			if (count == 1) {
				for (int col = 0; col < size; col++) {
					if (col != j && constraintList[i][col][candidate]) {
						constraintList[i][col][candidate] = false;
						different = true;
					}
				}
				for (int row = 0; row < size; row++) {
					if (row != i && constraintList[row][j][candidate]) {
						constraintList[row][j][candidate] = false;
						different = true;
					}
				}
			}
		}
	}
	return different;
}

bool processOfElimination(int size, bool constraintList[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH + 1]) {
	bool different = false;
	for (int i = 0; i < size; i++) {
		for (int num = 1; num <= size; num++) {
			int count = 0;
			int colIndex = -1;
			for (int j = 0; j < size; j++) {
				if (constraintList[i][j][num]) {
					count++;
					colIndex = j;
				}
			}
			if (count == 1) {
				for (int k = 1; k <= size; k++) {
					bool newValue = (k == num);
					if (constraintList[i][colIndex][k] != newValue) {
						constraintList[i][colIndex][k] = newValue;
						different = true;
					}
				}
			}
		}
	}
	for (int j = 0; j < size; j++) {
		for (int num = 1; num <= size; num++) {
			int count = 0;
			int rowIndex = -1;
			for (int i = 0; i < size; i++) {
				if (constraintList[i][j][num]) {
					count++;
					rowIndex = i;
				}
			}
			if (count == 1) {
				for (int k = 1; k <= size; k++) {
					bool newValue = (k == num);
					if (constraintList[rowIndex][j][k] != newValue) {
						constraintList[rowIndex][j][k] = newValue;
						different = true;
					}
				}
			}
		}
	}
	return different;
} 

static int visibleCount(const int *sequence, int size) {
    int visible = 0, maxH = 0;
    for (int i = 0; i < size; i++) {
        if (sequence[i] > maxH) {
            visible++;
            maxH = sequence[i];
        }
    }
    return visible;
}

static int rowPerms[40320][MAX_SIZE];
static int rowPermutations;

void rowSequenceHelper(int row, int size, bool constraintsList[MAX_SIZE][MAX_SIZE][MAX_SIZE+1], int pos, int *seq, bool used[MAX_SIZE + 1]){
	if (pos == size) {
		for (int i = 0; i < size; i++) {
			rowPerms[rowPermutations][i] = seq[i];
		}
		rowPermutations++;
		return;
	}
	for(int value = 1; value <= size; value++) {
		if (!constraintsList[row][pos][value]) {
			continue;
		}
		if(used[value]) {
			continue;
		}
		seq[pos] = value;
		used[value] = true;
		rowSequenceHelper(row, size, constraintsList, pos+1, seq, used);
		used[value] = false;
	}
}

int allRowSequences(int row, int size, bool constraintsList[MAX_SIZE][MAX_SIZE][MAX_SIZE+1]) {
	rowPermutations = 0;
	int seq[MAX_SIZE];
	bool used[MAX_SIZE + 1];
	for (int i = 1; i <= size; i++) {
		used[i] = false;
	}
	rowSequenceHelper(row, size, constraintsList, 0, seq, used);
	return rowPermutations;
}

bool passesLeft(const int *seq, int clue, int size) {
	if (clue == 0) {
		return true;
	}
	return (visibleCount(seq, size) == clue);
}

bool passesRight(const int *seq, int clue, int size) {
	if (clue == 0) {
		return true;
	}
	int reversed[MAX_SIZE];
	for (int i = 0; i< size; i++) {
		reversed[i] = seq[size - 1 - i];
	}
	return (visibleCount(reversed, size) == clue);
}

int filterRowSequences(int row, int size, bool constraintsList[MAX_SIZE][MAX_SIZE][MAX_SIZE+1], int leftClue, int rightClue) {
	int total = allRowSequences(row, size, constraintsList);
	int filteredCount = 0;
	for (int i = 0; i < total; i++) {
		if (passesLeft(rowPerms[i], leftClue, size) && passesRight(rowPerms[i], rightClue, size)){
			if (i != filteredCount) {
				for (int j = 0; j < size; j++) {
					rowPerms[filteredCount][j] = rowPerms[i][j];
				}
			}
			filteredCount++;
		}
	}
	return filteredCount;
}

bool collectRow(int row, int size, int validCount, bool constraintsList[MAX_SIZE][MAX_SIZE][MAX_SIZE+1]) {
	bool different = false;
	for (int col = 0; col < size; col++) {
		bool intersection[MAX_SIZE+1];
		for (int k = 1; k <= size; k++) {
			intersection[k] = true;
		}
		for(int i = 0; i < validCount; i++) {
			int candidate = rowPerms[i][col];
			for (int j = 1; j <= size; j++) {
				if (j!= candidate) {
					intersection[j] = false;
				}
			}
		}
		for (int p = 1; p <= size; p++) {
			if (constraintsList[row][col][p] && !intersection[p]) {
				constraintsList[row][col][p] = false;
				different = true;
			}
		}
	}
	return different;
}

bool clueEliminationRow(int row, int size, bool constraintsList[MAX_SIZE][MAX_SIZE][MAX_SIZE+1]) {
	int left = left_key[row];
	int right = right_key[row];
	if (left == 0 && right == 0) {
		return false;
	}
	int validCount = filterRowSequences(row, size, constraintsList, left, right);
	if (validCount == 0) {
		return false;
	}
	return collectRow(row, size, validCount, constraintsList);
}

static int colPerms[40320][MAX_SIZE];
static int colPermutations;

void colSequenceHelper(int col, int size, bool constraintsList[MAX_SIZE][MAX_SIZE][MAX_SIZE+1], int pos, int *seq, bool used[MAX_SIZE + 1]){
	if (pos == size) {
		for (int i = 0; i < size; i++) {
			colPerms[colPermutations][i] = seq[i];
		}
		colPermutations++;
		return;
	}
	for(int value = 1; value <= size; value++) {
		if (!constraintsList[col][pos][value]) {
			continue;
		}
		if(used[value]) {
			continue;
		}
		seq[pos] = value;
		used[value] = true;
		colSequenceHelper(col, size, constraintsList, pos+1, seq, used);
		used[value] = false;
	}
}

int allColSequences(int col, int size, bool constraintsList[MAX_SIZE][MAX_SIZE][MAX_SIZE+1]) {
	colPermutations = 0;
	int seq[MAX_SIZE];
	bool used[MAX_SIZE + 1];
	for (int i = 1; i <= size; i++) {
		used[i] = false;
	}
	colSequenceHelper(col, size, constraintsList, 0, seq, used);
	return colPermutations;
}

int filterColSequences(int col, int size, bool constraintsList[MAX_SIZE][MAX_SIZE][MAX_SIZE+1], int topClue, int bottomClue) {
	int total = allColSequences(col, size, constraintsList);
	int filteredCount = 0;
	for (int i = 0; i < total; i++) {
		if (passesLeft(colPerms[i], topClue, size)){
			int reversed[MAX_SIZE];
			for (int j = 0; j < size; j++) {
				reversed[j] = colPerms[i][size - 1 - j];
			}
			if (passesLeft(reversed, bottomClue, size)) {
				if (i != filteredCount) {
					for (int k = 0; k < size; k++) {
						colPerms[filteredCount][k] = colPerms[i][k];
					}
				}
				filteredCount++;
			}
		}
	}
	return filteredCount;
}

bool collectCol(int col, int size, int validCount, bool constraintsList[MAX_SIZE][MAX_SIZE][MAX_SIZE+1]) {
	bool different = false;
	for (int row = 0; row < size; row++) {
		bool intersection[MAX_SIZE+1];
		for (int k = 1; k <= size; k++) {
			intersection[k] = true;
		}
		for(int i = 0; i < validCount; i++) {
			int candidate = colPerms[i][row];
			for (int j = 1; j <= size; j++) {
				if (j!= candidate) {
					intersection[j] = false;
				}
			}
		}
		for (int p = 1; p <= size; p++) {
			if (constraintsList[row][col][p] && !intersection[p]) {
				constraintsList[row][col][p] = false;
				different = true;
			}
		}
	}
	return different;
}

bool clueEliminationCol(int col, int size, bool constraintsList[MAX_SIZE][MAX_SIZE][MAX_SIZE+1]) {
	int top = top_key[col];
	int bottom = bottom_key[col];
	if (top == 0 && bottom == 0) {
		return false;
	}
	int validCount = filterColSequences(col, size, constraintsList, top, bottom);
	if (validCount == 0) {
		return false;
	}
	return collectCol(col, size, validCount, constraintsList);
}

bool clueElimination(int size, bool constraintsList[MAX_SIZE][MAX_SIZE][MAX_SIZE+1]) {
	bool overall = false;
	for (int row = 0; row < size; row++) {
		if (clueEliminationRow(row, size, constraintsList)) {
			overall = true;
		}
	}
	for (int col = 0; col < size; col++) {
		if (clueEliminationCol(col, size, constraintsList)) {
			overall = true;
		}
	}
	return overall;
}