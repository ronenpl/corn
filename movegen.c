#include "movegen.h"
#include "defs.h"
#include "board.h"


U64 ray_attacks[8][64];
U64 knight_attacks[64];
U64 king_attacks[64];


void __attribute((constructor)) InitRayTable(void) {
	const int dir_offsets[8] = {10, 11, 1, -11, -10, -9, -1, 9};

	for (int dir = 0; dir < 8; dir++) {
		for (int sq = 0; sq < 120; sq++) {

			U64 bb = 0ULL;
			int new_sq = sq;
			int sq64 = SQ120_TO_SQ64[sq];

			if (sq64 == -1) {continue;}

			while (1) {
				// step along the given direction and convert to 64 format.
				new_sq += dir_offsets[dir];	
				// On-the-board test
				if (SQ120_TO_SQ64[new_sq] == -1) { break;}
				// add the square to the bitboard
				bb |= (1ULL << SQ120_TO_SQ64[new_sq]);
			}

			ray_attacks[dir][sq64] = bb;
		}
	}
}

void __attribute((constructor)) InitKnightTable(void) {
	const int knight_offsets[8] = {21, 12, -8, -19, -21, -12, 8, 19};

	for (int sq = 21; sq < 99; sq++) {

	    U64 bb = 0;

	    for (int dir = 0; dir < 8; dir++) {

			int new_square = SQ120_TO_SQ64[sq + knight_offsets[dir]];

			if (new_square == -1) {continue;}
			bb |= (1ULL << new_square);
		}
		knight_attacks[SQ120_TO_SQ64[sq]] = bb;
	}
}

void __attribute((constructor)) InitKingTable(void) {
    const int dir_offsets[8] = {10, 11, 1, -9, -11, -10, -1, 9};

	for (int sq = 21; sq < 99; sq++) {

	    U64 bb = 0;

	    for (int dir = 0; dir < 8; dir++) {

			int new_square = SQ120_TO_SQ64[sq + dir_offsets[dir]];
			if (new_square == -1) {continue;}

			bb |= (1ULL << new_square);
		}
		king_attacks[SQ120_TO_SQ64[sq]] = bb;
	}
}


U64 GetPositiveRayAttacks(Drctn dir, int sq120, U64 occupied) {

    int sq64 = SQ120_TO_SQ64[sq120];
	U64 attacks = ray_attacks[dir][sq64];
	U64 blockers = attacks & occupied;

	int sq = BitScanForward(blockers | 0x8000000000000000ULL);

	return attacks ^ ray_attacks[dir][sq];

}

U64 GetNegativeRayAttacks(Drctn dir, int sq120, U64 occupied) {

    int sq64 = SQ120_TO_SQ64[sq120];
	U64 attacks = ray_attacks[dir][sq64];
	U64 blockers = attacks & occupied;
	int sq = BitScanReverse(blockers | 1);

	return attacks ^ ray_attacks[dir][sq];

}

void ProcessU64(U64 bb, Movelist* moves, int offset, int cap, int code) {

	while (bb) {
		int target = PopLSB(&bb);
		moves->list[moves->curr_move_index++] = MoveFrom(target + offset, target, cap, code);
	}
}

void GeneratePseudoLegal(ChessBoard *cb, Movelist *moves) {

    GeneratePawnMoves(cb, moves);
    // TODO states we need to handle: regular pieces (obv), en passant, castling, promotion
}

void GeneratePawnMoves(ChessBoard *cb, Movelist *moves) {
    U64 occupancy = cb->occupancy[BOTH];
    U64 empty = ~occupancy;

    if (cb->side) {
        U64 white_pawns = cb->pieces[WP] & (~(RANK_1 << 48)); // Save promotion for later

        U64 single_push = (white_pawns << 8) & empty;
		ProcessU64(single_push, moves, -8, 0, REGULAR);

        U64 double_push = (white_pawns << 16) & (empty) & (empty << 8) & (RANK_1 << 24);
		ProcessU64(double_push, moves, -16, 0, DPUSH);

        U64 left_caps = (white_pawns << 7) & ((~(FILE_A) << 7)) & cb->occupancy[BLACK];
		ProcessU64(left_caps, moves, -7, 1, REGULAR);

        U64 right_caps = ((white_pawns << 9) & (~FILE_A)) & cb->occupancy[BLACK];
		ProcessU64(right_caps, moves, -9, 1, REGULAR);
    } else {

        U64 black_pawns = cb->pieces[BP] & ~(RANK_1); // Save promotion for later

        U64 single_push = (black_pawns >> 8) & empty;
		ProcessU64(single_push, moves, 8, 0, REGULAR);

        U64 double_push = (black_pawns >> 16) & (empty) & (empty >> 8) & (RANK_1 << 32);
		ProcessU64(double_push, moves, 16, 0, DPUSH);

        U64 left_caps = (black_pawns >> 7) & ((~(FILE_A) << 9)) & cb->occupancy[WHITE];
		ProcessU64(left_caps, moves, 9, 1, REGULAR);

        U64 right_caps = ((black_pawns >> 7) & (~FILE_A)) & cb->occupancy[WHITE];
		ProcessU64(right_caps, moves, 7, 1, REGULAR);
    }
}
