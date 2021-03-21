void printboard(int n, char** board);
int has_anyone_won(int n, char** board, char** checked); // This function calls the 2 others below
int has_white_won(int n, char** board, char i, char j, char** checked);
int has_black_won(int n, char** board, char i, char j, char** checked);
int minimax(int n, char** board, char** checked, char color_for_max, int depth, Listptr alist, int a, int b, char max_player, int difficulty_ext, short *moves_array);
int evaluate_board(int n, char** board, char** checked, char color_for_max);
void evaluate_adjacent_moves(int n, char** board, char i, char j, char* mini, char* maxi, char* minj, char* maxj, int* tiles_visited, char** checked, char color_for_max);
void insert_at_start(Listptr *alist, char i, char j);