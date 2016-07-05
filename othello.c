/* othello.c --- オセロをするプログラム */
/* ~s1411396/othello/othello.c */
/* Created: 2016/07/04 ver 1.0 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_board();
void put(char color, long row, long line);
int  can_put(char color, long row, long line);
int  com_put(char color);
int  human_put(char color);
int  sub_can_put(char color, long row, long line, int row_vec, int line_vec);

/* オセロ板（初期状態） */
char board[8][8] = {{'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'w', 'b', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'b', 'w', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},
		    {'n', 'n', 'n', 'n', 'n', 'n', 'n', 'n'},};

// 駒を置ける方向を格納する配列
int  direct[8] = {0, 0, 0, 0, 0, 0, 0, 0};

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
		printf("Your piece is [＊].\n");
		com_color = 'b';
		count += human_put(human_color);
		print_board();
	} else if (human_color == 'b') {
		printf("Your piece is [＠].\n");		
		com_color = 'w';
	} else {
		printf("Usage: %s your_color[w/b]\n", argv[0]);
		return 0;
	}

	while (count < 64) {
		count += com_put(com_color);
		print_board();
		if (count >= 64) {
			break;
		}
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
	char tmp[8] = {'\0'};
	char *row_, *line_;
	long row = 0, line = 0;

	printf("If you want to pass, please type \"pass\".\n");
	
	while (1) {
		if (fgets(tmp, 8, stdin) == NULL) {
			printf("error human_put()\n");
			exit(-1);
		}

		// 本当は以下の文で比較したいけど上手く行かない（segmentation fault）
		/* if (strcmp(tmp, "pass") == 0) break; */
		if (tmp[0] == 'p' &&
		    tmp[1] == 'a' &&
		    tmp[2] == 's' &&
		    tmp[3] == 's') return 0;

		row_ = strtok(tmp, " ");
		row = strtol(row_, NULL, 10);
		line_ = strtok(NULL, " ");
		if (line_ != NULL) {
			line = line_[0] - 'A' + 1;
		}

		if (can_put(color, row, line)) {
			break;
		}
	}
	
	put(color, row, line);
	return 1;
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
can_put(char color, long row, long line)
{
	int flag = 0;
	
	if ((row < 1 || 8 < row) ||
	    (line < 1 || 8 < line)) {
		printf("You can't put there.\n");
		return 0;
	}
	if (board[row - 1][line - 1] != 'n') {
		printf("You can't put there.\n");
		return 0;
		
	}

	if (sub_can_put(color, row, line, 0, 1)) {
		direct[0] = 1;
		flag = 1;
	}
	if (sub_can_put(color, row, line, 1, 1)) {
		direct[1] = 1;
		flag = 1;
	}
	if (sub_can_put(color, row, line, 1, 0)) {
		direct[2] = 1;
		flag = 1;
	}
	if (sub_can_put(color, row, line, 1, -1)) {
		direct[3] = 1;
		flag = 1;
	}
	if (sub_can_put(color, row, line, 0, -1)) {
		direct[4] = 1;
		flag = 1;
	}
	if (sub_can_put(color, row, line, -1, -1)) {
		direct[5] = 1;
		flag = 1;
	}
	if (sub_can_put(color, row, line, -1, 0)) {
		direct[6] = 1;
		flag = 1;
	}
	if (sub_can_put(color, row, line, -1, 1)) {
		direct[7] = 1;
		flag = 1;
	}

	if (flag) {
		return 1;
	}
	
	printf("You can't put there.\n");
	return 0;
}

/* can_put() のサブ関数 */
/* vec で指定された方向にひっくり返せる駒があるかどうかを判定する */
int
sub_can_put(char color, long row, long line, int row_vec, int line_vec)
{
	long row_ = row + row_vec, line_ = line + line_vec;

	if (board[row_ - 1][line_ - 1] == color) {
		return 0;
	}
	if (board[row_ - 1][line_ - 1] == 'n') {
		return 0;
	}

	while (1) {
		row_ += row_vec;
		line_ += line_vec;

		if ((row < 1 || 8 < row) ||
		    (line < 1 || 8 < line)) {
			return 0;
		}
		if (board[row_ - 1][line_ - 1] == 'n') {
			return 0;
		}
		if (board[row_ - 1][line_ - 1] == color) {
			break;
		}
	}

	return 1;
}

void
put(char color, long row, long line)
{
	// 取りあえず指定箇所に指定された色を置く
	board[row - 1][line - 1] = color;
}
