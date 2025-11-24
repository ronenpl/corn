#ifndef BOARD_H
#define BOARD_H

#include "defs.h"
#include <stdint.h>


/* UNDO STRUCT */


typedef struct {

	int move;

	char castle_perms;
	U16 ep_square;

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
  3 + 4  +  4 + 3 + 4  +  7   +  7   = 32 bits (int)
 000 0000 0000 000 0000 0000000 0000000
     +--+ +--+ +-+ +--+ +-----+ +-----+
     cap piece flags prom  to   from


    from: the source square; where the piece is coming from.

    to: where the piece is moving to.

    prom: the id of the promoting piece (EMPTY if not promotion)

    flags:  0 <- castle -- 0 <- en passant -- 0 <- double push

    piece: the id of the moving piece.

    capture: the id of the captured piece.
*/




/* BOARD METHODS*/

ChessBoard new_board(void);
void initialize_board(ChessBoard *cb);
void display_board(ChessBoard *cb);


void from_fen(ChessBoard *cb, char *fen);
void show_diagnostics(ChessBoard *cb);

/* MOVE METHODS */

void add_piece(ChessBoard *cb, int piece_id, int sq);
void remove_piece(ChessBoard *cb, int sq);
void move_piece(ChessBoard *cb, int from, int to);

void make_move(ChessBoard *cb, Move move);
void unmake_move(ChessBoard *cb);
char update_castling_rights(int from, int to, int piece_color, int piece_id, int move);

Move move_from(

    int from, int to, int prom,
    char ep, char double_push, char castle,
    int piece, int captured
);

void display_move(Move move);

/* MOVE MACROS */

#define From(m) m & 0x7F

#define To(m) (m & 0x3F80) >> 7

#define Prom(m) (m & 0x3C000) >> 14

#define EP_FLAG 1 << 18

#define DPUSH_FLAG 1 << 19

#define CASTLE_FLAG 1 << 20

#define Piece(m) (m & 0xF00000) >> 21
#define Capture(m) (m & 0x1E000000) >> 25


#endif
