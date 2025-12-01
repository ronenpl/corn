#ifndef DEFS_H
#define DEFS_H


/* General Purpose Constants and Definitions */


#include <stdio.h>
#include <stdlib.h>

#define MAXGAMEMOVES 2048
#define MAXMOVES 218

typedef unsigned long long U64;
typedef unsigned short U16;
typedef signed char int8;

#define NAME "Corn 1.0"
#define NrOf_Squares 64
#define NrOf_Pieces 32

/* ENUMS */

typedef enum {
	N, NE, E, SE, S, SW, W, NW
} Drctn;

enum Pieces {WP, WR, WK, WB, WQ, WN, BP, BR, BK, BB, BQ, BN, EMPTY, BORDER};

enum Sides { BLACK, WHITE, BOTH};

enum Castling { WKC = 1, WQC = 2, BKC = 4, BQC = 8};

enum Squares {
  A1, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8
};


#define RANK_1 0xFFULL

#define FILE_A 0x0101010101010101ULL


// General Utils (utils.c)

static inline int GetRank(int index) {
    return index / 8;
}

static inline int GetFile(int index) {
    return index % 8;
}

static inline int IsWhite(int piece_id) {
    return piece_id < BP;
}

// Bitboard Utils (bitboard_utils.c)

static inline int BitScanForward(U64 bb) {
    return __builtin_ctzll(bb);
}

static inline int BitScanReverse(U64 bb) {
    return 63 - __builtin_clzll(bb);
}

static inline int PopLSB(U64 *bb) {
    int index = BitScanForward(*bb);
    *bb &= *bb - 1;
    return index;
}

void PrintBitboard(U64 bb);

/* Tables */

extern int8_t SQ120_TO_SQ64[120];
extern int8_t SQ64_TO_SQ120[64];
// Assert Macro

#define RED   "\x1b[31m"
#define RESET "\x1b[0m"

#define ASSERT(expr)                                                       \
    do {                                                                      \
        if (!(expr)) {                                                        \
            fprintf(stderr, RED "Assertion failed: %s\n"                      \
                            "  File: %s\n"                                    \
                            "  Line: %d\n" RESET,                             \
                    #expr, __FILE__, __LINE__);                               \
            abort();                                                          \
        }                                                                     \
    } while (0)



#endif
