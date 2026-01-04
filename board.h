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

/* Move Encoding Scheme 
	BITS 0-5: The FROM square.

	BITS 6-11: The TO square.

	BIT 12: Capture Flag (1 for capture, 0 otherwise)

	BITS 13-15: Move Codes
		0: Regular
		1: Castling 
		2: En passant
		3: Double push
		4: Promote to rook
		4: Promote to knight
		6: Promote to bishop
		7: Promote to queen
 */

enum MoveCodes {REGULAR, CASTLE, EP, DPUSH, PR, PN, PB, PQ};

// Move List Struct
typedef struct {
    Move list[MAXMOVES];
    int curr_move_index;
} Movelist;

/* BOARD METHODS*/
ChessBoard NewBoard(void);
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
