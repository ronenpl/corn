#include "board.h"
#include "defs.h"
// The header for this file is contained within board.h


Move MoveFrom(int from, int to, int prom, char ep, char double_push, char castle, char capture, int piece) {

    Move move = 0;

    // move basics

    move |= from;

    move |= to << 7;

    move |= prom << 14;




    // flags

	move |= ep << 18;
	move |= double_push << 19;
    move |= castle << 20;
    move |= capture << 21;

    move |= piece << 22;

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

void AddPiece(ChessBoard *cb, int piece_id, int sq) {

    cb->state[sq] = piece_id;
    cb->pieces[piece_id] |= (1ULL << SQ120_TO_SQ64[sq]);

}

void RemovePiece(ChessBoard *cb, int sq) {
    int piece_id = cb->state[sq];
    cb->state[sq] = EMPTY;
    cb->pieces[piece_id] &= (~(1ULL << SQ120_TO_SQ64[sq]));
}

void MovePiece(ChessBoard *cb, int from, int to) {

	AddPiece(cb, cb->state[from], to);
	RemovePiece(cb, from);

}


void MakeMove(ChessBoard *cb, Move move) {

    int from = From(move);
    int to = To(move);

    int is_capture_move = ((move & CAPTURE_FLAG) != 0);

	// Before we do anything, we store previous state data to unmake the move later.
	cb->history[cb->undo_top++] = (Undo) {

			.move = move,
		   	.castle_perms = cb->castle_perms,
		   	.ep_square = cb->ep_square,
			.captured_piece = (is_capture_move)? cb->state[to] : EMPTY,
		   	.half_move_clock = cb->half_move_clock
	};




    // We extract piece data.

    int piece = cb->state[from];
    int piece_color = (IsWhite(piece));

	// Handle Regular Captures (We do this before making the move to update bitboards):

	if (move & CAPTURE_FLAG) {

		RemovePiece(cb, to);
	}

    // We make the move. Pretty simple:

	MovePiece(cb, from, to);

    // Handle promotions:

    if (Prom(move) != EMPTY) {
		RemovePiece(cb, to);
    	AddPiece(cb, Prom(move), to);
    }

	// Handle Castling:

	if (move & CASTLE_FLAG) {
		if (to == C1) {
			MovePiece(cb, A1, D1);
		}

		else if (to == G1) {
			MovePiece(cb, H1, F1);
		}

		else if (to == C8) {
			MovePiece(cb, A8, D8);
		}

		else if (to == G8) {
			MovePiece(cb, H8, F8);
		}
	}

	// Handle En Passant Captures:

	if (move & EP_FLAG) {
		if (piece_color == WHITE) {

		RemovePiece(cb, to - 10);

		} else {

		RemovePiece(cb, to + 10);
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

	if ((piece == WP) || (piece == BP) || (is_capture_move)) {

		cb->half_move_clock = 0;

	} else {

		cb->half_move_clock++;
	}

	if (cb->side == BLACK) {

	    cb->fullmoves++;

	}

  cb->castle_perms &= UpdateCastlingRights(from, to, piece_color, piece, is_capture_move);

    // Toggle side:

  cb->side ^= 1; // Switch side
}

char UpdateCastlingRights(int from, int to, int piece_color, int piece_id, int is_capture_move) {

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

    if (is_capture_move) {
        if (to == A1) castleperm &= ~WQC;
        if (to == H1) castleperm &= ~WKC;
        if (to == A8) castleperm &= ~BQC;
        if (to == H8) castleperm &= ~BKC;
    }

	return castleperm;
}

void UnmakeMove(ChessBoard *cb) {

	// Restore the side to what it was before the move was made

	cb->side ^= 1;

	// Extract the previous move and data:

	cb->undo_top--;

	Undo prev_pos = cb->history[cb->undo_top];
	int prev_move = prev_pos.move;

	int from = From(prev_move);
	int to = To(prev_move);

	char piece = Piece(prev_move);
	char piece_color = IsWhite(piece);

	char captured = prev_pos.captured_piece;

	// Unmake the move:

	MovePiece(cb, to, from);

	// Handle Captures:

	if (captured != EMPTY) {
		AddPiece(cb, captured, to);
	}

	// Handle Castling:

	if (prev_move & CASTLE_FLAG) {
		if (to == C1) {
			MovePiece(cb, D1, A1);
		}

		else if (to == G1) {
			MovePiece(cb, F1, H1);
		}

		else if (to == C8) {
			MovePiece(cb, D8, A8);
		}

		else if (to == G8) {
			MovePiece(cb, F8, H8);
		}
	}

	// Handle En Passant Captures:

	if (prev_move & EP_FLAG) {
		if (piece_color == WHITE) {

			AddPiece(cb, BP, to - 10);

		} else {

			AddPiece(cb, WP, to + 10);
		}
	}
	// Handle Promotions:

	if (Prom(prev_move) != EMPTY) {
	 RemovePiece(cb, from);
	    AddPiece(cb, piece, from);
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
