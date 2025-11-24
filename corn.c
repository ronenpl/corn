// #include <stdio.h>
#include "defs.h"
#include "board.h"


int main(void) {

	ChessBoard cb = new_board();

	initialize_board(&cb);
	from_fen(&cb, "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");

	make_move(&cb, move_from(A1, B1,EMPTY, 0, 0, 1, WR, EMPTY));
	make_move(&cb, move_from(A8, B8,EMPTY, 0, 0, 1, BR, EMPTY));
	make_move(&cb, move_from(B1, A1,EMPTY, 0, 0, 1, WR, EMPTY));
	make_move(&cb, move_from(B8, A8,EMPTY, 0, 0, 1, BR, EMPTY));

	show_diagnostics(&cb);
	unmake_move(&cb);
	show_diagnostics(&cb);
	unmake_move(&cb);
	show_diagnostics(&cb);
	unmake_move(&cb);
	show_diagnostics(&cb);

	return 0;

}
