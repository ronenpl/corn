// #include <stdio.h>
#include "defs.h"
#include "board.h"
#include "movegen.h"

char *STARTPOS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main(void) {

	ChessBoard cb = NewBoard();

	InitBoard(&cb);
	FromFen(&cb, STARTPOS);

	MakeMove(&cb, MoveFrom(E2, E4, 0, DPUSH));
	ShowDiagnostics(&cb);
	MakeMove(&cb, MoveFrom(D7, D5, 0, DPUSH));
	ShowDiagnostics(&cb);
	MakeMove(&cb, MoveFrom(E4, E5, 0, DPUSH));
	ShowDiagnostics(&cb);
	MakeMove(&cb, MoveFrom(F7, F5, 0, DPUSH));
	ShowDiagnostics(&cb);
	MakeMove(&cb, MoveFrom(E5, F6, 0, EP));

	ShowDiagnostics(&cb);

	UnmakeMove(&cb);

	ShowDiagnostics(&cb);

	return 0;

}
