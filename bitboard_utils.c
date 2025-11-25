#include "defs.h"
#include <stdio.h>



void print_bitboard(U64 bb) {

    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file ++) {
            int index = rank * 8 + file;
            (bb & (1ULL << index))? printf("1 ") : printf(". ");

        }

        printf("\n");
    }
    printf("\n");
}
