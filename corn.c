// #include <stdio.h>
#include "defs.h"
#include "board.h"
#include "movegen.h"

char *STARTPOS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main(void) {

	ChessBoard cb = NewBoard();

	InitBoard(&cb);

	FromFen(&cb, STARTPOS);

	// U64 occupancy = 0;

	// for (int i = 0; i < EMPTY; i++) {
	//     occupancy |= cb.pieces[i];

	// }

	U64 bb = knight_attacks[SQ120_TO_SQ64[E5]];

	PrintBitboard(bb);

	printf("%d\n",PopLSB(&bb));

	PrintBitboard(bb);


	ShowDiagnostics(&cb);

	return 0;

}
