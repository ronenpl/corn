// #include <stdio.h>
#include "defs.h"
#include "board.h"
#include "movegen.h"

char *STARTPOS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1";

int main(void) {

	ChessBoard cb = NewBoard();

	FromFen(&cb, STARTPOS);

	MakeMove(&cb, MoveFrom(A2, A3, 0, REGULAR));	
	PrintBoard(&cb);

	Movelist moves;
	
	GeneratePawnMoves(&cb, &moves);
	for (int i = 0; i < moves.curr_move_index; i++) {
		MakeMove(&cb, moves.list[i]);
		PrintBoard(&cb);
		UnmakeMove(&cb);
	}
	
	return 0;

}
