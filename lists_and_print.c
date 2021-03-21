#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_struct_functions.h"
#include "other_functions.h"

void insert_at_start(Listptr *alist, char i, char j) {
	Listptr templist;
	templist = *alist;
	*alist = malloc(sizeof(Listptr));
	(*alist)->i = i;
	(*alist)->j = j;
	(*alist)->next = templist;
}

int delete_first(Listptr *alist, char** board) {
	if (*alist == NULL) {
		return 1;
	}
	else {
		Listptr templist;
		templist = *alist;
		board[templist->j][templist->i] = 'n'; // FIX THIS
		*alist = (*alist)->next;
		free(templist);
		return 0;
	}
}

int moves_array_modify(int n, char **board, short *moves_array, int move_number, char color) {
	static int i=-1;
	if (color == 'n') {
		board[moves_array[i] / n][moves_array[i] % n] = 'n';
		i--;
		return 0;
	}
	else {
		i++;
		moves_array[i] = move_number;
		board[move_number / n][move_number % n] = color;
		return 0;
	}
}

void printboard(int n, char** board) {
	printf("    ");
	for (int i = 4; i <= n; i++) {
		printf("  ");
	}
	printf("W H I T E\n ");
	for (int i = 0; i < n; ++i) {
		printf("   %c", 65+i);
	}
	printf("\n ");
	for (int i = 0; i < n; ++i) {
		printf("   _");
	}
	printf("\n   ");
	for (int i = 0; i < n; ++i) {
		printf("/ \\_");
	}
	printf("\n");
	for (int i = 0; i < n; ++i) {
		printf("%d ", i+1);
		for (int j = 0; j <= n; ++j) {
			printf("|");
			if (j != n) {
				if (board[i][j] != 'n') printf(" %c ",board[i][j]);
				else printf("   ");
			}
		}
		printf(" %d\n  ", i+1);
		for (int k = 0; k < i; ++k) {
			printf("  ");
		}
		for (int j = 0; j < n; ++j) {
			printf(" \\_/");
		}
		if (i != n-1) {
			printf(" \\\n  ");
			for (int k = 0; k < i; ++k) {
				printf("  ");
			}
		}
	}
	printf("\n");
}