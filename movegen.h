#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "defs.h"

extern U64 ray_attacks[8][64];
extern U64 knight_attacks[64];
extern U64 king_attacks[64];


U64 GetPositiveRayAttacks(Drctn dir, int sq120, U64 occupied);
U64 GetNegativeRayAttacks(Drctn dir, int sq120, U64 occupied);
#endif
