#include <stdio.h>
#include <stdlib.h>

int  minimax(char **board, char color, int depth, int flag, int *r, int *l);
int  can_put_(char color, long row, long line, char **board);
int  sub_can_put_(char color, long row, long line, int row_vec, int line_vec, char **board);
void put_(char color, long row, long line, char **board, char **next);
void reverce_(char color, long row, long line, int row_vec, int line_vec, char **board);
int  eval(char color, char **board, int flag);
char reverce_color(char color);

int direct_[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/* minimax法を実現する関数 */
/* flag = 1 のとき color は敵の色を示す */
int
minimax(char **board, char color, int depth, int flag, int *r, int *l)
{
	int  i, j, tmp;
	// 評価値の最大値を格納する変数
	int  point = -65;
	// 評価が最大になる場所を記憶する変数
	int row = -1, line = -1;

	// char ** じゃなくて char *[8] だと gcc に認識されるのは何故？
	char pos[8][8];
	char rev_color;

	// 指定された深さまで探索したら評価値を返す
	if (depth == 0) {
		return eval(color, board, flag);
	}

	// board 全体を調べる
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			// 現在位置に駒が置ける場合
			if (can_put_(color, i, j, board)) {
				// 駒を置いた状態を作る
			  put_(color, i, j, board, pos);
				rev_color = reverce_color(color);
				// その状態で minimax() を再帰（color は反転）
				tmp = minimax(pos, rev_color, depth--, (flag + 1) % 2, r, l);
				// 現在位置に置いたときの評価が良い場合
				if (tmp > point) {
					// point, row, line を更新
					point = tmp;
					row = i;
					line = j;
				}
			}
		}
	}

	*r = row;
	*l = line;
	return point;
}

/* 指定された位置が置ける場所かどうか判定する関数 */
int
can_put_(char color, long row, long line, char **board)
{
	int flag = 0;
	
	if ((row < 1 || 8 < row) ||
	    (line < 1 || 8 < line)) {
		return 0;
	}
	if (board[row - 1][line - 1] != 'n') {
		return 0;
		
	}

	if (sub_can_put_(color, row, line, 0, 1, board)) {
		direct_[0] = 1;
		flag = 1;
	}
	if (sub_can_put_(color, row, line, 1, 1, board)) {
		direct_[1] = 1;
		flag = 1;
	}
	if (sub_can_put_(color, row, line, 1, 0, board)) {
		direct_[2] = 1;
		flag = 1;
	}
	if (sub_can_put_(color, row, line, 1, -1, board)) {
		direct_[3] = 1;
		flag = 1;
	}
	if (sub_can_put_(color, row, line, 0, -1, board)) {
		direct_[4] = 1;
		flag = 1;
	}
	if (sub_can_put_(color, row, line, -1, -1, board)) {
		direct_[5] = 1;
		flag = 1;
	}
	if (sub_can_put_(color, row, line, -1, 0, board)) {
		direct_[6] = 1;
		flag = 1;
	}
	if (sub_can_put_(color, row, line, -1, 1, board)) {
		direct_[7] = 1;
		flag = 1;
	}

	if (flag) {
		return 1;
	}
	
	return 0;
}

/* can_put_() のサブ関数 */
/* vec で指定された方向にひっくり返せる駒があるかどうかを判定する */
int
sub_can_put_(char color, long row, long line, int row_vec, int line_vec, char **board)
{
	long row_ = row + row_vec, line_ = line + line_vec;

	if ((row_ < 1 || 8 < row_) ||
	    (line_ < 1 || 8 < line_)) {
		return 0;
	}
	if (board[row_ - 1][line_ - 1] == color) {
		return 0;
	}
	if (board[row_ - 1][line_ - 1] == 'n') {
		return 0;
	}

	while (1) {
		row_ += row_vec;
		line_ += line_vec;

		if ((row_ < 1 || 8 < row_) ||
		    (line_ < 1 || 8 < line_)) {
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

/* 指定された位置に駒を置く関数 */
void
put_(char color, long row, long line, char **board, char **next)
{
  int i, j;

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      next[i][j] = board[i][j];
    }
  }
	
	next[row - 1][line - 1] = color;

	if (direct_[0]) {
	  reverce_(color, row, line, 0, 1, next);
	}
	if (direct_[1]) {
	  reverce_(color, row, line, 1, 1, next);
	}
	if (direct_[2]) {
	  reverce_(color, row, line, 1, 0, next);
	}
	if (direct_[3]) {
	  reverce_(color, row, line, 1, -1, next);
	}
	if (direct_[4]) {
	  reverce_(color, row, line, 0, -1, next);
	}
	if (direct_[5]) {
	  reverce_(color, row, line, -1, -1, next);
	}
	if (direct_[6]) {
	  reverce_(color, row, line, -1, 0, next);
	}
	if (direct_[7]) {
	  reverce_(color, row, line, -1, 1, next);
	}

	for (i = 0; i < 8; i++) {
		direct_[i] = 0;
	}
}

/* vec で指定された方向にある駒をひっくり返す関数 */
void
reverce_(char color, long row, long line, int row_vec, int line_vec, char **board)
{
	long row_ = row + row_vec, line_ = line + line_vec;

	while(board[row_ - 1][line_ - 1] != color) {
		board[row_ - 1][line_ - 1] = color;
		row_ += row_vec;
		line_ += line_vec;
	}
}

/* 盤面を評価する関数 */
/* flag = 1 のとき color は敵の色を示す */
int
eval(char color, char **board, int flag)
{
	int i, j;
	int p1 = 0, p2 = 0;
	char p1color;

	if (flag == 0) {
	  p1color = color;
	} else {
	  p1color = reverce_color(color);
	}
	
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (board[i][j] == p1color) {
				p1++;
			}
			else if (board[i][j] != 'n') {
				p2++;
			}
		}
	}

	return p1 - p2;
}

/* 色を反転する関数 */
char
reverce_color(char color)
{
  if (color == 'w') {
    return 'b';
  } else if (color == 'b') {
    return 'w';
  }

  return 'n';
}