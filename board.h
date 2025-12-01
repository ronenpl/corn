#ifndef BOARD_H
#define BOARD_H

#include "defs.h"
#include <stdint.h>

typedef unsigned short Move;

/* UNDO STRUCT */
typedef struct {
	Move move;

	char castle_perms;
	char ep_square;

	char captured_piece;
	char moved_piece;

	char half_move_clock;
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
// TODO: Fix move encoding to pack into a short.
// Should be possible with a capture flag and 3 bits for other move flags:
// 0: regular
// 1: promote to queen
// 2: promote to knight
// 3: promote to rook
// 4: promote to bishop
// 5: en passant
// 6: castling
// 7: double push
// Total: 4 bits
// Other 12 twelve bits are for from and to square (each square is 6 bits)

/* A move is defined as:
  3 + 3  +  4 + 4 + 4  +  7   +  7   = 32 bits (int)
 000 000 0000 0000 0000 0000000 0000000
         +--+ +--+ +--+ +-----+ +-----+
         piece flags prom  to   from

    from: the source square; where the piece is coming from.
    to: where the piece is moving to.

    prom: the id of the promoting piece (EMPTY if not promotion)

    flags:  0 <- castle -- 0 <- en passant -- 0 <- double push -- 0 <- capture
*/
enum MoveCodes {REGULAR, CASTLE, EP, DPUSH, PR, PN, PB, PQ};

// Move List Struct
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

Move MoveFrom(char from, char to, char capture, char move_code);

void display_move(Move move);

/* MOVE MACROS */
#define From(m) m & 0x3F
#define To(m) (m & 0xFC0) >> 6

#define CAPTURE_FLAG 1 << 12

#define MoveCode(m) (m & 0xE000) >> 13


#endif
