/* othello.c --- オセロをするプログラム */
/* ~s1411396/othello/othello.c */
/* Created: 2016/07/04 ver 1.0 */

#include <stdio.h>
#include <stdlib.h>

void print_board();
void human_put(int *count);
void com_put(int *count);

/* オセロ板（初期状態） */
char board[8][8] = {{'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'w', 'b', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'b', 'w', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},};

int main(int argc, char **argv)
{
	char human_color, com_color;
	int count = 4;
	
	if (argc != 2) {
		printf("Usage: %s your_color[w/b]\n", argv[0]);
		return 0;
	}

	print_board();
	/* human_color = argv[1]argc[0]; */
	/* if (human_color = 'w') { */
	/* 	com_color = 'b'; */
	/* 	human_put(&count); */
	/* } else if (human_color = 'b') { */
	/* 	com_color = 'w'; */
	/* } else { */
	/* 	printf("Usage: %s your_color[w/b]\n", argv[0]); */
	/* 	return; */
	/* } */

	/* while (count < 64) { */
	/* 	com_put(&count); */
	/* 	if (count < 64) break; */
	/* 	human_put(&count); */
	/* } */
}

void print_board()
{
	int i, j;
	
	printf("　ＡＢＣＤＥＦＧＨ\n");
	for (i = 0; i < 8; i++) {
		printf(" %d", i + 1);
		for (j = 0; j < 8; j++) {
			if (board[i][j] == 'n')
				printf("・");
			else if (board[i][j] == 'w')
				printf("＊");
			else if (board[i][j] == 'b')
				printf("＠");
			else {
				printf("error print_board()\n");
				exit(-1);
			}
		}
		printf("\n");
	}
}

void human_put(int *count)
{
	// write here
}

void com_put(int *count)
{
	// write here
}
