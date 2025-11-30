// #include <stdio.h>
#include "defs.h"
#include "board.h"
#include "movegen.h"

char *STARTPOS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main(void) {

	ChessBoard cb = NewBoard();

	InitBoard(&cb);
	FromFen(&cb, STARTPOS);

	Movelist moves = {.list = {0}, .curr_move_index = 0};

	GeneratePawnMoves(&cb, &moves);
	// PrintBoard(&cb);
	// for (int i = 0; i < moves.curr_move_index; i++) {

	// display_move(moves.list[i]);

	// }

	return 0;

}
