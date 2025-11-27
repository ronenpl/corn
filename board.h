#ifndef BOARD_H
#define BOARD_H

#include "defs.h"
#include <stdint.h>

/* UNDO STRUCT */


typedef struct {

	int move;

	char castle_perms;
	U16 ep_square;
	char captured_piece;

	int half_move_clock;


} Undo;



/* BOARD STRUCT */


typedef struct {
    // Board Representation:

    U16 state[NrOf_Squares];

	// Bitboards:

    U64 pieces[12]; // Pieces Bitboard
    U64 occupancy[3]; // Black, White, Both

    // General Data:

    int side;
    int castle_perms;
    int ep_square;

    // Draw-Detection Related Data:

    int half_move_clock;
    int fullmoves;

	// Board History:

	Undo history[MAXGAMEMOVES];
	int undo_top;

} ChessBoard;


/* Move Encoding Scheme */

typedef uint32_t Move;

/* A move is defined as:
  3 + 3  +  4 + 4 + 4  +  7   +  7   = 32 bits (int)
 000 000 0000 0000 0000 0000000 0000000
         +--+ +--+ +--+ +-----+ +-----+
         piece flags prom  to   from


    from: the source square; where the piece is coming from.

    to: where the piece is moving to.

    prom: the id of the promoting piece (EMPTY if not promotion)

    flags:  0 <- castle -- 0 <- en passant -- 0 <- double push -- 0 <- capture

    piece: the id of the moving piece.


*/

/*
-----------+
Moves List |
-----------+
*/

typedef struct {

    Move list[MAXMOVES];
    int curr_move_index;

} Movelist;



/* BOARD METHODS*/

ChessBoard NewBoard(void);
void InitBoard(ChessBoard *cb);
void PrintBoard(ChessBoard *cb);


void FromFen(ChessBoard *cb, char *fen);
void ShowDiagnostics(ChessBoard *cb);

/* MOVE METHODS */

void AddPiece(ChessBoard *cb, int piece_id, int sq);
void RemovePiece(ChessBoard *cb, int sq);
void MovePiece(ChessBoard *cb, int from, int to);

void MakeMove(ChessBoard *cb, Move move);
void UnmakeMove(ChessBoard *cb);
char UpdateCastlingRights(int from, int to, int piece_color, int piece_id, int move);

Move MoveFrom(

    int from, int to, int prom,
    char ep, char double_push, char castle, char is_capture,
    int piece
);

void display_move(Move move);

/* MOVE MACROS */

#define From(m) m & 0x7F

#define To(m) (m & 0x3F80) >> 7

#define Prom(m) (m & 0x3C000) >> 14

#define EP_FLAG 1 << 18

#define DPUSH_FLAG 1 << 19

#define CASTLE_FLAG 1 << 20
#define CAPTURE_FLAG 1 << 21

#define Piece(m) (m & 0x3C00000) >> 22


#endif
