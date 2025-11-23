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
#define NrOf_Squares 120
#define NrOf_Pieces 32

/* ENUMS */

typedef enum {
    NORTH,
    SOUTH,
    EAST,
    WEST
} Drctn;

enum {WP, WR, WK, WB, WQ, WN, BP, BR, BK, BB, BQ, BN, EMPTY, BORDER};

enum { BLACK, WHITE, BOTH};

enum { WKC = 1, WQC = 2, BKC = 4, BQC = 8};

enum {
  A1 = 21, B1, C1, D1, E1, F1, G1, H1,
  A2 = 31, B2, C2, D2, E2, F2, G2, H2,
  A3 = 41, B3, C3, D3, E3, F3, G3, H3,
  A4 = 51, B4, C4, D4, E4, F4, G4, H4,
  A5 = 61, B5, C5, D5, E5, F5, G5, H5,
  A6 = 71, B6, C6, D6, E6, F6, G6, H6,
  A7 = 81, B7, C7, D7, E7, F7, G7, H7,
  A8 = 91, B8, C8, D8, E8, F8, G8, H8
};




// General Utils (utils.c)

static inline int get_rank(int index) {
    return index / 10;
}

static inline int get_file(int index) {
    return index % 10;
}

static inline int is_white(int piece_id) {
    return piece_id < BP;
}

// Bitboard Utils (bitboard_utils.c)

void print_bitboard(U64 bb);

/* Tables */

extern int8_t i120_to_i64[120];


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
