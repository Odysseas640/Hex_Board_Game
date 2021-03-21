typedef struct listnode *Listptr;
struct listnode {
	char i;
	char j;
	Listptr next;
};

void insert_at_start(Listptr *alist, char i, char j);
int delete_first(Listptr *alist, char** board);
int moves_array_modify(int n, char **board, short *moves_array, int move_number, char color);