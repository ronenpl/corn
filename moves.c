#include "board.h"
#include "defs.h"
// The header for this file is contained within board.h


Move move_from(int from, int to, int prom, char ep, char double_push, char castle, int piece, int captured) {

    Move move = 0;

    // move basics

    move |= from;

    move |= to << 7;

    move |= prom << 14;




    // flags

	move |= ep << 18;
	move |= double_push << 19;
    move |= castle << 20;

    // piece stuff

    move |= piece << 21;

    move |= captured << 25;

    return move;
}


void display_move(Move move) {

    int cursor = 29;

    while (move != 0) {

        printf("%d", (move & (1 << cursor)) != 0);

        move &= ~(1 << cursor);

        cursor--;

    }
    printf("\n");


}
void add_piece(ChessBoard *cb, int piece_id, int sq) {

    cb->state[sq] = piece_id;
    cb->pieces[piece_id] |= (1ULL << i120_to_i64[sq]);

}

void remove_piece(ChessBoard *cb, int sq) {
    int piece_id = cb->state[sq];
    cb->state[sq] = EMPTY;
    cb->pieces[piece_id] &= (~(1ULL << i120_to_i64[sq]));
}

void move_piece(ChessBoard *cb, int piece_type, int from, int to) {

	add_piece(cb, piece_type, to);
	remove_piece(cb, from);

}


void make_move(ChessBoard *cb, Move move) {

    int from = From(move);
    int to = To(move);

    // We extract piece data.

    int piece = cb->state[from];
    int piece_color = (is_white(piece));

	// Handle Regular Captures (We do this before making the move to update bitboards):

	if (Capture(move) != EMPTY) {

		remove_piece(cb, to);
	}

    // We make the move. Pretty simple:

	move_piece(cb, piece, from, to);

    // Handle promotions:

    if (Prom(move) != EMPTY) {
		remove_piece(cb, to);
    	add_piece(cb, Prom(move), to);
    }

	// Handle Castling:

	if (move & CASTLE_FLAG) {
		if (to == C1) {
			move_piece(cb, WR, A1, D1);
		}

		else if (to == G1) {
			move_piece(cb, WR, H1, F1);
		}

		else if (to == C8) {
			move_piece(cb, BR, A8, D8);
		}

		else if (to == G8) {
			move_piece(cb, BR, H8, F8);
		}
	}

	// Handle En Passant Captures:

	if (move & EP_FLAG) {
		if (piece_color == WHITE) {

			remove_piece(cb, to - 10);

		} else {

			remove_piece(cb, to + 10);
		}
	}

	// Handle Setting En Passant Square:

	cb->ep_square = 0;

	if (move & DPUSH_FLAG) {
		if (piece_color == WHITE) {

			cb->ep_square = to - 10;

		} else {

			cb->ep_square = to + 10;
		}
	}

	// Handle Fifty Move Clock and Fullmove counter:

	if ((piece == WP) || (piece == BP) || (Capture(move) != EMPTY)) {

		cb->half_move_clock = 0;

	} else {

		cb->half_move_clock++;
	}

	if (cb->side == BLACK) {

	    cb->fullmoves++;

	}

	//TODO Update Castling Rights


    // Toggle side:

    cb->side ^= 1; // Switch side
}
