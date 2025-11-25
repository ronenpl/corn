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

void move_piece(ChessBoard *cb, int from, int to) {

	add_piece(cb, cb->state[from], to);
	remove_piece(cb, from);

}


void make_move(ChessBoard *cb, Move move) {


	// Before we do anything, we store previous state data to unmake the move later.
	cb->history[cb->undo_top++] = (Undo) {

			.move = move,
		   	.castle_perms = cb->castle_perms,
		   	.ep_square = cb->ep_square,
		   	.half_move_clock = cb->half_move_clock
	};



    int from = From(move);
    int to = To(move);
    int captured = Capture(move);
    // We extract piece data.

    int piece = cb->state[from];
    int piece_color = (is_white(piece));

	// Handle Regular Captures (We do this before making the move to update bitboards):

	if (captured != EMPTY) {

		remove_piece(cb, to);
	}

    // We make the move. Pretty simple:

	move_piece(cb, from, to);

    // Handle promotions:

    if (Prom(move) != EMPTY) {
		remove_piece(cb, to);
    	add_piece(cb, Prom(move), to);
    }

	// Handle Castling:

	if (move & CASTLE_FLAG) {
		if (to == C1) {
			move_piece(cb, A1, D1);
		}

		else if (to == G1) {
			move_piece(cb, H1, F1);
		}

		else if (to == C8) {
			move_piece(cb, A8, D8);
		}

		else if (to == G8) {
			move_piece(cb, H8, F8);
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

	if ((piece == WP) || (piece == BP) || (captured != EMPTY)) {

		cb->half_move_clock = 0;

	} else {

		cb->half_move_clock++;
	}

	if (cb->side == BLACK) {

	    cb->fullmoves++;

	}

  cb->castle_perms &= update_castling_rights(from, to, piece_color, piece, captured);

    // Toggle side:

  cb->side ^= 1; // Switch side
}

char update_castling_rights(int from, int to, int piece_color, int piece_id, int captured) {

	char castleperm = 0xF;

	if (piece_id == WK) {
		castleperm &= ~(WKC | WQC);
	}

	if (piece_id == BK) {
		castleperm &= ~(BKC | BQC);
	}

	if ((piece_id == WR) || (piece_id == BR)) {
		if (from == A1) castleperm &= (~WQC);

		if (from == H1) {castleperm &= (~WKC);}

		if (from == A8) castleperm &= (~BQC);

		if (from == H8) castleperm &= (~BKC);
	}

    if (captured == WR) {
        if (to == A1) castleperm &= ~WQC;
        if (to == H1) castleperm &= ~WKC;
    }
    if (captured == BR) {
        if (to == A8) castleperm &= ~BQC;
        if (to == H8) castleperm &= ~BKC;
    }

	return castleperm;
}

void unmake_move(ChessBoard *cb) {

	// Restore the side to what it was before the move was made

	cb->side ^= 1;

	// Extract the previous move and data:

	cb->undo_top--;

	Undo prev_pos = cb->history[cb->undo_top];
	int prev_move = prev_pos.move;

	int from = From(prev_move);
	int to = To(prev_move);

	char piece = Piece(prev_move);
	char piece_color = is_white(piece);

	char captured = Capture(prev_move);

	// Unmake the move:

	move_piece(cb, to, from);

	// Handle Captures:

	if (captured != EMPTY) {
		add_piece(cb, captured, to);

	}

	// Handle Castling:

	if (prev_move & CASTLE_FLAG) {
		if (to == C1) {
			move_piece(cb, D1, A1);
		}

		else if (to == G1) {
			move_piece(cb, F1, H1);
		}

		else if (to == C8) {
			move_piece(cb, D8, A8);
		}

		else if (to == G8) {
			move_piece(cb, F8, H8);
		}
	}

	// Handle En Passant Captures:

	if (prev_move & EP_FLAG) {
		if (piece_color == WHITE) {

			add_piece(cb, BP, to - 10);

		} else {

			add_piece(cb, WP, to + 10);
		}
	}
	// Handle Promotions:

	if (Prom(prev_move) != EMPTY) {
	    remove_piece(cb, from);
	    add_piece(cb, piece, from);
	}

	//Restore previous board state stuff

	cb->ep_square = prev_pos.ep_square;
	cb->castle_perms = prev_pos.castle_perms;
	cb->half_move_clock = prev_pos.half_move_clock;

	// Handle Fullmoves

	if (cb->side == BLACK) {

		cb->fullmoves--;
	}
}
