// #include <stdio.h>
#include "defs.h"
#include "board.h"


int main(void) {

	ChessBoard cb = new_board();

	initialize_board(&cb);
	from_fen(&cb, "rnbqkbnr/ppp1pppp/8/8/2PpP3/5N2/PP1P1PPP/RNBQKB1R b KQkq c3 0 3");

	Move move = move_from(D4, C3, EMPTY, 1, 0, 0, EMPTY, WP);

	ASSERT(cb.side == 0);
	
	//Move move = move_from(E1, G1, EMPTY, 0, 0, 1, EMPTY, EMPTY);
	//Move move = move_from(H5, H7, EMPTY, 1, 1, 1, EMPTY, EMPTY);
	//Move move = move_from(H5, H3, EMPTY, 0, 0, 1, EMPTY, EMPTY);
	make_move(&cb, move);
	display_board(&cb);

	print_bitboard(cb.pieces[BP]);
	ASSERT(cb.castle_perms == 0xF);













	return 0;

}
