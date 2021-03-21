#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_struct_functions.h"
#include "other_functions.h"
#define max(A,B) ((A) > (B) ? (A) : (B))
#define min(A,B) ((A) < (B) ? (A) : (B))

// I had a hard time grasping the concept, so I started with (copied, really) pseudocode from Wikipedia.
int minimax(int n, char** board, char** checked, char color_for_max, int depth, Listptr alist, int a, int b, char max_player, int difficulty_ext, short *moves_array) {
	int value, temp=0, empty_spots=0, previous_value;
	char max_i, max_j, prune=0;
	char max_i_2 = max_i, max_j_2 = max_j;
	if (depth == 0) { // If we're at the bottom level
		return evaluate_board(n, board, checked, color_for_max); // (color_for_max score) MINUS (other color score)
	}
	else if (max_player == 1) { // Pick the maximum
		value = -1987654321;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				if (board[i][j] == 'n') {
					moves_array_modify(n, board, moves_array, i * n + j, color_for_max);
					previous_value = value;
					value = max(value, minimax(n, board, checked, color_for_max, depth - 1, alist, a, b, 0, difficulty_ext, moves_array));
					if (value > previous_value) {
						max_i = i;
						max_j = j;
					}
					moves_array_modify(n, board, moves_array, -1, 'n');
					a = max(a, value);
					if (a >= b) {
						prune = 1;
						break;
					}
				}
			}
			if (prune == 1) {
				prune = 0;
				break;
			}
		}
		if (depth != difficulty_ext) return value;
	}
	else if (max_player == 0) {
		value = 1987654321;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				if (board[i][j] == 'n') {
					if (color_for_max == 'b') // Put other color on the spot
						moves_array_modify(n, board, moves_array, i * n + j, 'w');
					else
						moves_array_modify(n, board, moves_array, i * n + j, 'b');
					previous_value = value;
					value = min(value, minimax(n, board, checked, color_for_max, depth - 1, alist, a, b, 1, difficulty_ext, moves_array));
					if (value < previous_value) {
						max_i = i;
						max_j = j;
					}
					moves_array_modify(n, board, moves_array, -1, 'n');
					b = min(b, value);
					if (a >= b) {
						prune = 1;
						break;
					}
				}
			}
			if (prune == 1) {
				prune = 0;
				break;
			}
		}
		if (depth != difficulty_ext) return value;
	}
	return max_i * n + max_j;
}

int evaluate_board(int n, char** board, char** checked, char positive_color) { // (color_for_max score) MINUS (other color score)
	int value, tiles_visited, total_value = 0;
	char mini, maxi, minj, maxj;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			checked[i][j] = 0;
		}
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (board[i][j] != 'n' && checked[i][j] == 0) {
				mini=i;
				maxi=i;
				minj=j;
				maxj=j;
				tiles_visited=0;
				evaluate_adjacent_moves(n, board, i, j, &mini, &maxi, &minj, &maxj, &tiles_visited, checked, board[i][j]); // Last argument is so the function knows what color to look for
				if (board[i][j] == 'w') { // Formula for value of set of moves
					if (mini == 0 && maxi == n-1)
						value = 987654321 - ((maxi - mini) * (maxi - mini + 7) + 5 * (maxj - minj) - 2 * tiles_visited);
					else
						value = (maxi - mini) * (maxi - mini + 7) + 5 * (maxj - minj) - 2 * tiles_visited;
				}
				else if (board[i][j] == 'b') {
					if (minj == 0 && maxj == n-1)
						value = 987654321 - ((maxj - minj) * (maxj - minj + 7) + 5 * (maxi - mini) - 2 * tiles_visited);
					else
						value = (maxj - minj) * (maxj - minj + 7) + 5 * (maxi - mini) - 2 * tiles_visited;
				}
				if (board[i][j] == positive_color)
					total_value += value;
				else
					total_value -= value;
			}
		}
	}
	return total_value;
}

void evaluate_adjacent_moves(int n, char** board, char i, char j, char* mini, char* maxi, char* minj, char* maxj, int* tiles_visited, char** checked, char color) {
	checked[i][j] = 1; // This function visits every spot adjacent to the one we're examining, to calculate the parameters in the formula
	(*tiles_visited)++;
	if ((*mini) > i) (*mini) = i;
	if ((*maxi) < i) (*maxi) = i;
	if ((*minj) > j) (*minj) = j;
	if ((*maxj) < j) (*maxj) = j;
	if (i < n-1 && checked[i+1][j] == 0 && board[i+1][j] == color) // Down
		evaluate_adjacent_moves(n, board, i+1, j, mini, maxi, minj, maxj, tiles_visited, checked, color);
	if (i > 0 && checked[i-1][j] == 0 && board[i-1][j] == color) // Up
		evaluate_adjacent_moves(n, board, i-1, j, mini, maxi, minj, maxj, tiles_visited, checked, color);
	if (i > 0 && j < n-1 && checked[i-1][j+1] == 0 && board[i-1][j+1] == color) // Up Right
		evaluate_adjacent_moves(n, board, i-1, j+1, mini, maxi, minj, maxj, tiles_visited, checked, color);
	if (j < n-1 && checked[i][j+1] == 0 && board[i][j+1] == color) // Right
		evaluate_adjacent_moves(n, board, i, j+1, mini, maxi, minj, maxj, tiles_visited, checked, color);
	if (j > 0 && checked[i][j-1] == 0 && board[i][j-1] == color) // Left
		evaluate_adjacent_moves(n, board, i, j-1, mini, maxi, minj, maxj, tiles_visited, checked, color);
	if (j > 0 && i < n-1 && checked[i+1][j-1] == 0 && board[i+1][j-1] == color) // Down Left
		evaluate_adjacent_moves(n, board, i+1, j-1, mini, maxi, minj, maxj, tiles_visited, checked, color);
}

int has_anyone_won(int n, char** board, char** checked) { // Check whether anyone has won
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			checked[i][j] = 0; // Reset array that shows whether we have visited each spot on the board
		}
	}
	for (int j = 0; j < n; ++j)
		if (board[0][j] != 'n')
			if (has_white_won(n, board, 0, j, checked) == 1) {
				return 'w';
			}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			checked[i][j] = 0;
		}
	}
	for (int i = 0; i < n; ++i)
		if (board[i][0] != 'n')
			if (has_black_won(n, board, i, 0, checked) == 1) {
				return 'b';
			}
}

int has_white_won(int n, char** board, char i, char j, char** checked) {
	int temp = 0, end = 0;
	if (board[i][j] == 'w') {
		checked[i][j] = 1;
		if (i == n-1) end = 1; // End recursion, the 2 edges are connected
		else {
			if (i > 0 && checked[i-1][j] == 0) {
				temp = has_white_won(n, board, i-1, j, checked);
				if (temp == 1) end = 1;
			}
			if (i < n-1 && checked[i+1][j] == 0) {
				temp = has_white_won(n, board, i+1, j, checked);
				if (temp == 1) end = 1;
			}
			if (j > 0 && checked[i][j-1] == 0) {
				temp = has_white_won(n, board, i, j-1, checked);
				if (temp == 1) end = 1;
			}
			if (j < n-1 && checked[i][j+1] == 0) {
				temp = has_white_won(n, board, i, j+1, checked);
				if (temp == 1) end = 1;
			}
			if (i > 0 && j < n-1 && checked[i-1][j+1] == 0) {
				temp = has_white_won(n, board, i-1, j+1, checked);
				if (temp == 1) end = 1;
			}
			if (j > 0 && i < n-1 && checked[i+1][j-1] == 0) {
				temp = has_white_won(n, board, i+1, j-1, checked);
				if (temp == 1) end = 1;
			}
		}
	}
	return end;
}

int has_black_won(int n, char** board, char i, char j, char** checked) {
	int temp = 0, end = 0;
	if (board[i][j] == 'b') {
		checked[i][j] = 1;
		if (j == n-1) end = 1; // End recursion, the 2 edges are connected
		else {
			if (i > 0 && checked[i-1][j] == 0) {
				temp = has_black_won(n, board, i-1, j, checked);
				if (temp == 1) end = 1;
			}
			if (i < n-1 && checked[i+1][j] == 0) {
				temp = has_black_won(n, board, i+1, j, checked);
				if (temp == 1) end = 1;
			}
			if (j > 0 && checked[i][j-1] == 0) {
				temp = has_black_won(n, board, i, j-1, checked);
				if (temp == 1) end = 1;
			}
			if (j < n-1 && checked[i][j+1] == 0) {
				temp = has_black_won(n, board, i, j+1, checked);
				if (temp == 1) end = 1;
			}
			if (i > 0 && j < n-1 && checked[i-1][j+1] == 0) {
				temp = has_black_won(n, board, i-1, j+1, checked);
				if (temp == 1) end = 1;
			}
			if (j > 0 && i < n-1 && checked[i+1][j-1] == 0) {
				temp = has_black_won(n, board, i+1, j-1, checked);
				if (temp == 1) end = 1;
			}
		}
	}
	return end;
}