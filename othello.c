/* othello.c --- オセロをするプログラム */
/* ~s1411396/othello/othello.c */
/* Created: 2016/07/04 ver 1.0 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_board();
void put(char color, int row, int line, int direct[]);
int  can_put(char color, int row, int line, int direct[]);
int   com_put(char color);
int   human_put(char color);

/* オセロ板（初期状態） */
char board[8][8] = {{'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'w', 'b', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'b', 'w', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},};

int
main(int argc, char **argv)
{
	char human_color, com_color;
	int  count = 4;
	
	if (argc != 2) {
		printf("Usage: %s your_color[w/b]\n", argv[0]);
		return 0;
	}

	print_board();
	human_color = argv[1][0];
	
	if (human_color == 'w') {
		com_color = 'b';
		count += human_put(human_color);
		print_board();
	} else if (human_color == 'b') {
		com_color = 'w';
	} else {
		printf("Usage: %s your_color[w/b]\n", argv[0]);
		return 0;
	}

	// 上手くループしない
	while (count < 64) {
		count += com_put(com_color);
		print_board();
		if (count < 64) break;
		count += human_put(human_color);
		print_board();
	}
}

/* オセロ板を表示する関数 */
void
print_board()
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

/* コマンドラインからの入力を受けて駒を置く関数 */
int
human_put(char color)
{
	// 駒を置ける方向を格納する配列
	int  direct[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	char tmp[8] = {'\0'};
	char *row_, *line_;
	long row, line;

	printf("If you want to pass, please type \"pass\".\n");
	
	while (1) {
		if (fgets(tmp, 8, stdin) == NULL) {
			printf("error human_put()\n");
			exit(-1);
		}

		// ここの比較が成功しない（segmentation fault）
		if (strcmp(tmp, "pass") == 0) break;

		row_ = strtok(tmp, " ");
		row = strtol(row_, NULL, 10);
		line_ = strtok(NULL, " ");
		line = line_[0] - 'A' + 1;
		
		if (row < 1 || 8 < row)
			printf("Usage: row[0~8] line[A~H]\n");
		else if (line < 1 || 8 < line)
			printf("Usage: row[0~8] line[A~H]\n");
		
		else {
			if (can_put(color, row, line, direct)) {
				put(color, row, line, direct);
				return 1;
			} else {
				printf("You can't put here.\n");
			}
		}
	}
	return 0;
}

/* コンピュータが駒を置く関数 */
int
com_put(char color)
{
	// 取りあえず人間 vs 人間を実装
	return human_put(color);
}

/* 指定された位置が置ける場所かどうか判定する関数 */
int
can_put(char color, int row, int line, int direct[])
{
	// 取りあえず常に1
	return 1;
}

void
put(char color, int row, int line, int direct[])
{
	// 取りあえず指定箇所に指定された色を置く
	board[row - 1][line - 1] = color;
}
