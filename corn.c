// #include <stdio.h>
#include "defs.h"
#include "board.h"
#include "movegen.h"

int main(void) {

	ChessBoard cb = new_board();

	initialize_board(&cb);

	from_fen(&cb, "r3k2r/3p1ppp/p1n1pn2/1p2Pb2/4P3/2N2N2/PP3PPP/R2Q1RK1 w kq b6 0 12");

	// U64 occupancy = 0;

	// for (int i = 0; i < EMPTY; i++) {
	//     occupancy |= cb.pieces[i];

	// }

	display_board(&cb);
	print_bitboard(knight_attacks[28]);
	return 0;

}
