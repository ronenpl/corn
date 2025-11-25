#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "defs.h"

extern U64 ray_attacks[8][64];
extern U64 knight_attacks[64];
extern U64 king_attacks[64];

void init_attack_tables(void);

U64 get_positive_ray_attacks(Drctn dir, int sq120, U64 occupied);
U64 get_negative_ray_attacks(Drctn dir, int sq120, U64 occupied);







#endif
