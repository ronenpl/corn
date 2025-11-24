// #include <stdio.h>
#include "defs.h"
#include "board.h"


int main(void) {

	ChessBoard cb = new_board();

	initialize_board(&cb);
	from_fen(&cb, "1q6/2PPPP2/4K3/8/8/4k3/2pppp2/8 w - - 0 1");

	make_move(&cb, move_from(C7, B8, WQ, 0, 0, 0, WP, BQ));
	make_move(&cb, move_from(C2, C1, BQ, 0, 0, 0, BP, EMPTY));
	display_board(&cb);

	unmake_move(&cb);
	display_board(&cb);
	return 0;

}
