#include "movegen.h"
#include "defs.h"

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
