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
			int ds = sq;
			int sq64 = SQ120_TO_SQ64[sq];

			if (sq64 == -1) {continue;}

			while (1) {

				ds += dir_offsets[dir];

				if (sq < 0 ||sq >=120) { break; }
				if (SQ120_TO_SQ64[ds] == -1) { break;}

				bb |= (1ULL << SQ120_TO_SQ64[ds]);

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

void GeneratePseudoLegal(ChessBoard *cb, Movelist *moves) {

    GeneratePawnMoves(cb, moves);
    // TODO states we need to handle: regular pieces (obv), en passant, castling, promotion
}

void GeneratePawnMoves(ChessBoard *cb, Movelist *moves) {
    U64 occupancy = cb->occupancy[BOTH];
    U64 empty = ~occupancy;

    if (cb->side) {

        U64 white_pawns = cb->pieces[WP] & (~(RANK_1 << 56)); // Save promotion for later

        U64 single_push = (white_pawns << 8) & empty;

        while (single_push) {
            int target = SQ64_TO_SQ120[PopLSB(&single_push)];
            moves->list[moves->curr_move_index++] = MoveFrom(target - 10, target, EMPTY, 0, 0, 0, 0, EMPTY);
        }

        U64 double_push = (white_pawns << 16) & (empty) & (empty << 8) & (RANK_1 << 24);

        while (double_push) {
            int target = SQ64_TO_SQ120[PopLSB(&double_push)];
            moves->list[moves->curr_move_index++] = MoveFrom(target - 20, target, EMPTY, 0, 1, 0, 0, EMPTY);
        }

        U64 left_caps = (white_pawns << 7) & ((~(FILE_A)) << 7) & cb->occupancy[BLACK];

        while (left_caps) {
            int target = SQ64_TO_SQ120[PopLSB(&left_caps)];
            moves->list[moves->curr_move_index++] = MoveFrom(target - 9, target, EMPTY, 0, 0, 0, 1, EMPTY);
        }

        U64 right_caps = ((white_pawns << 9) & (~FILE_A)) & cb->occupancy[BLACK];

        while (right_caps) {
            int target = SQ64_TO_SQ120[PopLSB(&right_caps)];
            moves->list[moves->curr_move_index++] = MoveFrom(target - 9, target, EMPTY, 0, 0, 0, 1, EMPTY);
        }
    }
}
