/* othello.c --- オセロをするプログラム */
/* ~s1411396/othello/othello.c */
/* Created: 2016/07/04 ver 1.0 */

// please all region indent.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_board();
int  human_put(char color);
int  com_put(char color);
int  can_put(char color, long row, long line, char pos[][]);
int  sub_can_put(char color, long row, long line, int row_vec, int line_vec);
void put(char color, long row, long line);
void reverce(char color, long row, long line, int row_vec, int line_vec);
int  victory(char com_color, char pos[][]);
int  minimax(char com_color, int depth, char pos[][]);

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

	human_color = argv[1][0];
	
	if (human_color == 'w') {
		printf("Your piece is [＊].\n");
		com_color = 'b';
		print_board();
		printf("If you want to pass, please type \"pass\".\n");
		count += human_put(human_color);
		print_board();
	} else if (human_color == 'b') {
		printf("Your piece is [＠].\n");		
		com_color = 'w';
		print_board();
		printf("If you want to pass, please type \"pass\".\n");		
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

	if (victory(human_color, board) > 0) {
		printf("Victory!\n");
	} else if (victory(human_color, board) == 0) {
	  printf("Draw");
	} else {
		printf("Defeat...\n");
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
can_put(char color, long row, long line, char pos[][])
{
	int flag = 0;
	
	if ((row < 1 || 8 < row) ||
	    (line < 1 || 8 < line)) {
		printf("You can't put there.\n");
		return 0;
	}
	if (pos[row - 1][line - 1] != 'n') {
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

	if ((row_ < 1 || 8 < row_) ||
	    (line_ < 1 || 8 < line_)) {
		return 0;
	}
	if (pos[row_ - 1][line_ - 1] == color) {
		return 0;
	}
	if (pos[row_ - 1][line_ - 1] == 'n') {
		return 0;
	}

	while (1) {
		row_ += row_vec;
		line_ += line_vec;

		if ((row_ < 1 || 8 < row_) ||
		    (line_ < 1 || 8 < line_)) {
			return 0;
		}
		if (pos[row_ - 1][line_ - 1] == 'n') {
			return 0;
		}
		if (pos[row_ - 1][line_ - 1] == color) {
			break;
		}
	}

	return 1;
}

/* 指定された位置に駒を置く関数 */
void
put(char color, long row, long line, char pos[][])
{
	int i;
	
	pos[row - 1][line - 1] = color;

	if (direct[0]) {
		reverce(color, row, line, 0, 1);
	}
	if (direct[1]) {
		reverce(color, row, line, 1, 1);
	}
	if (direct[2]) {
		reverce(color, row, line, 1, 0);
	}
	if (direct[3]) {
		reverce(color, row, line, 1, -1);
	}
	if (direct[4]) {
		reverce(color, row, line, 0, -1);
	}
	if (direct[5]) {
		reverce(color, row, line, -1, -1);
	}
	if (direct[6]) {
		reverce(color, row, line, -1, 0);
	}
	if (direct[7]) {
		reverce(color, row, line, -1, 1);
	}

	for (i = 0; i < 8; i++) {
		direct[i] = 0;
	}
}

/* vec で指定された方向にある駒をひっくり返す関数 */
void
reverce(char color, long row, long line, int row_vec, int line_vec)
{
	long row_ = row + row_vec, line_ = line + line_vec;

	while(board[row_ - 1][line_ - 1] != color) {
		board[row_ - 1][line_ - 1] = color;
		row_ += row_vec;
		line_ += line_vec;
	}
}

/* please comment here. */
int
victory(char color, char pos[][]) {
  int player1 = 0, player2 = 0;
  int i, j;

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if (pos[i][j] == color) {
	player1++;
      } else if (pos[i][j] != 'n') {
	player2++;
      }
    }
  }

  return player1 - player2;
}

/* please comment here. */
int
minimax(char color, int depth, char pos[][]) {
  int  i, j;
  // please comment here.
  int  point[8][8] = {{0, 0, 0, 0, 0, 0, 0, 0},
		      {0, 0, 0, 0, 0, 0, 0, 0},
		      {0, 0, 0, 0, 0, 0, 0, 0},
		      {0, 0, 0, 0, 0, 0, 0, 0},
		      {0, 0, 0, 0, 0, 0, 0, 0},
		      {0, 0, 0, 0, 0, 0, 0, 0},
		      {0, 0, 0, 0, 0, 0, 0, 0},
		      {0, 0, 0, 0, 0, 0, 0, 0},}
  char next_pos[8][8];

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      next_pos[i][j] = pos[i][j];
    }
  }

  if (depth == 0) {
    return victory(color);
  }

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if (can_put(color, i, j)) {
	point[i][j] = minimax(color, depth--, );
      }
    }
  }
}
