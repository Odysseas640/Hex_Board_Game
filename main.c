#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_struct_functions.h"
#include "other_functions.h"
#define max(A,B) ((A) > (B) ? (A) : (B))
#define min(A,B) ((A) < (B) ? (A) : (B))

int main(int argc, char *argv[]) {
	int n=4, new_n, i, j, move, best_i, best_j, swap_i, swap_j;
	int player_turns = 0, AIturns = 0, difficulty = 8, move_return, difficulty_ext;
	char **board, *buffer, c, **checked, win=0, color, swap=0, player_color = 'w', AIcolor = 'b';
	char swap_used = 0;
	short *moves_array;
	Listptr alist=NULL;

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-n") == 0)
			n = atoi(argv[i]+3);
		if (strcmp(argv[i], "-d") == 0)
			difficulty = atoi(argv[i]+3);
		if (strcmp(argv[i], "-b") == 0) {
			player_color = 'b';
			AIcolor = 'w';
		}
		if (strcmp(argv[i], "-s") == 0)
			swap = 1;
	}
	if (n<4 || n>26) {
		printf("Size must be 4 to 26\n");
		return 0;
	}
	buffer = malloc(80 * sizeof(char)); // This holds the command given at every round
	board = malloc(n * sizeof(char *)); // Game board
	checked = malloc(n * sizeof(char *)); // This shows whether each spot on the board has been looked at in each round, so recursive functions don't run for ever
	moves_array = malloc(n * n * sizeof(short)); // This array holds moves for minimax function.
	if (board == NULL || buffer == NULL || checked == NULL || moves_array == NULL) {
		printf("malloc error\n");
		return 0;
	}
	for (int i = 0; i < n; ++i) {
		board[i] = malloc(n * sizeof(char));
		checked[i] = malloc(n * sizeof(char));
		if (board[i] == NULL || checked[i] == NULL) {
			printf("malloc error\n");
			return 0;
		}
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			board[i][j] = 'n'; // Initialize game board
		}
	}
	printboard(n, board);

	char quit = 0, whoseturn = 0;
	do {
		for (int i = 0; i < 80; ++i) {
			buffer[i] = '\0';
		}
		if (swap_used == 0) { // Display whose turn it is
			if (player_color == 'w' && player_turns == AIturns) {
				printf("White player (human) plays now\n");
				whoseturn = 'h';
			}
			else if (player_color == 'w' && player_turns == AIturns + 1) {
				printf("Black player (computer) plays now\n");
				whoseturn = 'c';
			}
			else if (player_color == 'b' && player_turns + 1 == AIturns) {
				printf("Black player (human) plays now\n");
				whoseturn = 'h';
			}
			else if (player_color == 'b' && player_turns == AIturns) {
				printf("White player (computer) plays now\n");
				whoseturn = 'c';
			}
			else
				printf("Unaccounted for instance. player_turns=%d, AIturns=%d\n", player_turns, AIturns);
		}
		else if (swap_used == 1) {
			if (player_color == 'w' && player_turns + 1 == AIturns) {
				printf("White player (human) plays now\n");
				whoseturn = 'h';
			}
			else if (player_color == 'w' && player_turns == AIturns) {
				printf("Black player (computer) plays now\n");
				whoseturn = 'c';
			}
			else if (player_color == 'b' && player_turns == AIturns) {
				printf("Black player (human) plays now\n");
				whoseturn = 'h';
			}
			else if (player_color == 'b' && player_turns == AIturns + 1) {
				printf("White player (computer) plays now\n");
				whoseturn = 'c';
			}
			else
				printf("swap=1 Unaccounted for instance. player_turns=%d, AIturns=%d\n", player_turns, AIturns);
		}
		// printf("$ "); // Command prompt
		i = -1;
		fflush(stdout);
		while ((c = getchar()) != '\n') { // Copy buffer to buffer(80) so I can look for words in it
			if (i > 79) {
				printf("Buffer overflow\n"); // Correct commands are much less than 80 characters anyway
				break;
			}
			i++;
			buffer[i] = c;
		}
		if (strstr(buffer, "newgame") != NULL) { // This accepts any of the 3 arguments, not necessarily in order, FIX IT
			while (delete_first(&alist, board) == 0); // Delete list of moves. When done, it returns 1
			player_turns = 0; // Reset turn counters
			AIturns = 0;
			win = 0; // Reset whether anybody has won
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					board[i][j] = 'n'; // Reset game board
				}
			}
			if (strstr(buffer, "white") != NULL) { // Human is white
				player_color = 'w';
				AIcolor = 'b';
			}
			else if (strstr(buffer, "black") != NULL) { // Human is black
				player_color = 'b';
				AIcolor = 'w';
			}
			if (strstr(buffer, "swapoff") != NULL) {
				swap = 0;
			}
			else if (strstr(buffer, "swapon") != NULL) {
				swap = 1;
			}
			for (int i = 0; i < 80; ++i) {
				if ((new_n = atoi(buffer + i)) != 0) { // Look for numbers in buffer(80)
					if (new_n < 4 || new_n > 26) {
						printf("Board size must be 4 to 26\n");
						break;
					}
					for (int i = 0; i < n; ++i) { // Free arrays and malloc them again for the new size
						free(board[i]);
						free(checked[i]);
					}
					free(board);
					free(checked);
					free(moves_array);
					n = new_n;
					board = malloc(n * sizeof(char *));
					checked = malloc(n * sizeof(char *));
					moves_array = malloc(n * n * sizeof(short));
					if (board == NULL || checked == NULL) {
						printf("malloc error\n");
						return 0;
					}
					for (int i = 0; i < n; ++i) {
						board[i] = malloc(n * sizeof(char));
						checked[i] = malloc(n * sizeof(char));
						if (board[i] == NULL || checked[i] == NULL) {
							printf("malloc error\n");
							return 0;
						}
					}
					for (int i = 0; i < n; ++i) {
						for (int j = 0; j < n; ++j) {
							board[i][j] = 'n';
						}
					}
					break;
				}
			}
			whoseturn = 0; // This is for loading a file
			swap_used = 0; // This is for calculating whose turn it is. It's different when swap has been used
		}
		else if (strstr(buffer, "play") != NULL) { // If command is "play <spot>"
			if (whoseturn != 'h') {
				printf("Not your turn\n");
				continue;
			}
			i = buffer[5] - 'A';
			j = atoi(buffer + 6) -1;
			if (i >= n || j >= n || i < 0 || j < 0) {
				printf("Indicated spot is outside of board\n");
				continue;
			}
			else if (board[j][i] == 'n') {
				board[j][i] = player_color;
				insert_at_start(&alist, i, j); // Save move in a list, so it can be undone
				player_turns++;
				swap_i = i; // These are only used if the computer's first move is swap
				swap_j = j;
				printf("Move played: %c%d\n", i+65, j+1);
			}
			else printf("Spot is not empty\n");
		}
		else if (strstr(buffer, "cont") != NULL) { // If command is "cont"
			if (whoseturn != 'c') {
				printf("Not the computer's turn\n");
				continue;
			}
			if (swap == 1 && swap_used == 0 && player_turns == 1 && AIturns == 0) { // Computer plays swap for its first move
				delete_first(&alist, board);
				player_turns--;
				board[swap_i][swap_j] = AIcolor;
				AIturns++;
				insert_at_start(&alist, swap_j, swap_i);
				printf("Move played: swap\n");
				swap_used = 1;
			}
			else {

				int empty_spots_minimax = 0;
				for (int i = 0; i < n; ++i) {
					for (int j = 0; j < n; ++j) {
						if (board[i][j] == 'n') {
							empty_spots_minimax++;
						}
					}
				}
				difficulty_ext = min(difficulty, empty_spots_minimax);

				move_return = minimax(n, board, checked, AIcolor, difficulty_ext, alist, -987654321, 987654321, 1, difficulty_ext, moves_array); // Calculate best move
				board[move_return / n][move_return % n] = AIcolor;
				insert_at_start(&alist, move_return % n, move_return / n); // Save move in list
				AIturns++;
				swap_i = move_return / n; // These are only used if player's first move is swap
				swap_j = move_return % n;
				printf("Move played: %c%d\n", move_return % n + 65, move_return / n + 1);
			}
		}
		else if (strstr(buffer, "undo") != NULL) { // If command is "undo"
			if (swap_used == 0) {
				if (player_color == 'w' && player_turns == AIturns && player_turns > 0) {
					delete_first(&alist, board);
					AIturns--;
					delete_first(&alist, board);
					player_turns--;
				}
				else if (player_color == 'w' && player_turns == AIturns + 1 && player_turns > 0) {
					delete_first(&alist, board);
					player_turns--;
					whoseturn = 'h';
				}
				else if (player_color == 'b' && player_turns + 1 == AIturns && player_turns > 0) {
					delete_first(&alist, board);
					AIturns--;
					delete_first(&alist, board);
					player_turns--;
				}
				else if (player_color == 'b' && player_turns == AIturns && player_turns > 0) {
					delete_first(&alist, board);
					player_turns--;
					whoseturn = 'h';
				}
				else
					printf("No moves to undo\n");
			}
			else if (swap_used == 1) { // It's different if swap has been used
				if (player_color == 'w' && player_turns + 1 == AIturns && player_turns > 0) {
					delete_first(&alist, board);
					AIturns--;
					delete_first(&alist, board);
					player_turns--;
				}
				else if (player_color == 'w' && player_turns == AIturns && player_turns > 0) {
					delete_first(&alist, board);
					player_turns--;
					whoseturn = 'h';
				}
				else if (player_color == 'b' && player_turns == AIturns && player_turns > 0) {
					delete_first(&alist, board);
					AIturns--;
					delete_first(&alist, board);
					player_turns--;
				}
				else if (player_color == 'b' && player_turns == AIturns + 1 && player_turns > 0) {
					delete_first(&alist, board);
					player_turns--;
					whoseturn = 'h';
				}
				else
					printf("No moves to undo\n");
			}
		}
		else if (strstr(buffer, "suggest") != NULL) { // If command is "suggest"
			if (whoseturn != 'h')
				printf("It's not your turn.\n");
			else {
				int empty_spots_minimax = 0;
				for (int i = 0; i < n; ++i) {
					for (int j = 0; j < n; ++j) {
						if (board[i][j] == 'n') {
							empty_spots_minimax++;
						}
					}
				}
				difficulty_ext = min(difficulty, empty_spots_minimax); // Difficulty must not exceed empty spots, otherwise it doesn't work properly
				move_return = minimax(n, board, checked, player_color, difficulty, alist, -987654321, 987654321, 1, difficulty_ext, moves_array);
				printf("You may play at %c%d\n", move_return % n + 65, move_return / n + 1);
			}
		}
		else if (strstr(buffer, "level") != NULL) {
			if (atoi(buffer+6) == 0)
				printf("Current difficulty level: %d\n", difficulty);
			else difficulty = atoi(buffer+6);
		}
		else if (strstr(buffer, "swap") != NULL) {
			if (whoseturn != 'h') {
				printf("Not your turn\n");
				continue;
			}
			if (AIturns == 1 && player_turns == 0 && swap_used == 0) {
				delete_first(&alist, board);
				AIturns--;
				board[swap_j][swap_i] = player_color;
				player_turns++;
				insert_at_start(&alist, swap_i, swap_j);
				printf("Move played: swap\n");
				swap_used = 1;
			}
			else if (swap_used == 1)
				printf("You can only use swap in your first move\n");
			else
				printf("Swap is only available in your first move when you play second.\n");
		}
		else if (strstr(buffer, "save") != NULL) {
			FILE *fille = fopen(buffer + 5, "wb");
			if (fille == NULL)
				printf("Error opening file\n");
			fprintf(fille, "%c", n);
			if (swap_used == 0) { // Save whose turn it is to play
				if (player_color == 'w' && player_turns == AIturns)
					fprintf(fille, "%c", 'w');
				else if (player_color == 'w' && player_turns == AIturns + 1)
					fprintf(fille, "%c", 'b');
				else if (player_color == 'b' && player_turns + 1 == AIturns)
					fprintf(fille, "%c", 'b');
				else if (player_color == 'b' && player_turns == AIturns)
					fprintf(fille, "%c", 'w');
				else
					printf("Unaccounted for instance. player_turns=%d, AIturns=%d\n", player_turns, AIturns);
			}
			else if (swap_used == 1) {
				if (player_color == 'w' && player_turns + 1 == AIturns)
					fprintf(fille, "%c", 'w');
				else if (player_color == 'w' && player_turns == AIturns)
					fprintf(fille, "%c", 'b');
				else if (player_color == 'b' && player_turns == AIturns)
					fprintf(fille, "%c", 'b');
				else if (player_color == 'b' && player_turns == AIturns + 1)
					fprintf(fille, "%c", 'w');
				else
					printf("swap=1 Unaccounted for instance. player_turns=%d, AIturns=%d\n", player_turns, AIturns);
			}
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					fprintf(fille, "%c", board[i][j]);
				}
			}
			fclose(fille);
		}
		else if (strstr(buffer, "load") != NULL) {
			while (delete_first(&alist, board) == 0); // Delete list of moves
			FILE *fille = fopen(buffer + 5, "rb");
			if (fille == NULL)
				printf("Error opening file\n");
			new_n = getc(fille);
			for (int i = 0; i < n; ++i) { // Free old board, because new one may have a different size
				free(board[i]);
				free(checked[i]);
			}
			free(board);
			free(checked);
			free(moves_array);
			n = new_n;
			board = malloc(n * sizeof(char *)); // New board
			checked = malloc(n * sizeof(char *));
			moves_array = malloc(n * n * sizeof(short));
			if (board == NULL || checked == NULL)
				printf("malloc error\n");
			for (int i = 0; i < n; ++i) {
				board[i] = malloc(n * sizeof(char));
				checked[i] = malloc(n * sizeof(char));
				if (board[i] == NULL || checked[i] == NULL)
					printf("malloc error\n");
			}
			char whoseturn = getc(fille);
			printf("whoseturn = %c\n", whoseturn);
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					board[i][j] = getc(fille);
				}
			}
			fclose(fille);
			player_turns = 0;
			AIturns = 0;
			if (whoseturn == 'w') { // If file says it's the white player's turn to play
				printf("We don't know who is black and who is white,\n  so we're assuming human is white\n");
				player_color = 'w';
				AIcolor = 'b';
			}
			else {
				printf("We don't know who is black and who is white,\n  so we're assuming human is black\n");
				player_color = 'b';
				AIcolor = 'w';
				AIturns = 1; // This is a quick fix for when the game is loaded and it's the black player's turn
			}
			swap_used = 0;
			swap = 0; // Disable swap
		}
		else if (strstr(buffer, "showstate") != NULL) printboard(n, board);
		else if (strstr(buffer, "quit") != NULL) {
			quit = 1;
			break;
		}
		printboard(n, board);
		win = has_anyone_won(n, board, checked); // Check whether anyone has won
		if (win == 'w') {
			printf("White player won\n");
			quit = 1;
		}
		else if (win == 'b') {
			printf("Black player won\n");
			quit = 1;
		}
	} while (quit == 0);

	for (int i = 0; i < n; ++i) {
		free(board[i]);
		free(checked[i]);
	}
	free(board);
	free(checked);
	free(buffer);
	free(moves_array);
	return 0;
}