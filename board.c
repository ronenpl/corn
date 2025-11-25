
#include <stdio.h>
#include "defs.h"
#include "board.h"

ChessBoard new_board(void) {
    ChessBoard temp = {0};
    return temp;
}


void initialize_board(ChessBoard *cb) {
    for (int i = 0; i < NrOf_Squares; i++) {
        if ((get_file(i) > 8) || ((get_file(i)) < 1) || (get_rank(i) > 9) || (get_rank(i) < 2)) {
            cb->state[i] = BORDER;
        } else {
            cb->state[i] = EMPTY;
        }
    }
}



void display_board(ChessBoard *cb) {
    const char *display_characters[14] = {

        "P", "R", "K", "B", "Q", "N", /* white: P,R,K,B,Q,N */
        "p", "r", "k", "b", "q", "n", /* black: p,r,k,b,q,n */

        ".",
        "#"

    };

    for (int rank = 9; rank > 1; rank--) {
        for (int file = 1; file < 9; file++) {
            int index = rank * 10 + file;
            printf("%s ", display_characters[cb->state[index]]);
        }
        printf("\n");
    }
    printf("\n");
}



void from_fen(ChessBoard *cb, char *fen) {

    int rank = 9;
    int file = 1;

    while (fen[0] != ' ') {
        int index = rank * 10 + file;
        switch (fen[0]) {

            case 'P': add_piece(cb, WP, index); file++; break;
            case 'R': add_piece(cb, WR, index); file++; break;

            case 'K': add_piece(cb, WK, index); file++; break;
            case 'B': add_piece(cb, WB, index); file++; break;

            case 'Q': add_piece(cb, WQ, index); file++; break;
            case 'N': add_piece(cb, WN, index); file++; break;

            case 'p': add_piece(cb, BP, index); file++; break;
            case 'r': add_piece(cb, BR, index); file++; break;

            case 'k': add_piece(cb, BK, index); file++; break;
            case 'b': add_piece(cb, BB, index); file++; break;

            case 'q': add_piece(cb, BQ, index); file++; break;
            case 'n': add_piece(cb, BN, index); file++; break;

            case '/': rank -= 1; file = 1; break;

            default: file += ((int)(fen[0] - '0')); break;

        }


        fen++;
    }

    fen++;

    if (fen[0] == 'w') {
        cb->side = 1;
    } else {
        cb->side = 0;
    }

    fen += 2;

    while (fen[0] != ' ') {
        switch (fen[0]) {

            case 'K': cb->castle_perms |= WKC; break;
            case 'Q': cb->castle_perms |= WQC; break;

            case 'k': cb->castle_perms |= BKC; break;
            case 'q': cb->castle_perms |= BQC; break;

        }

        fen++;
    }

    fen++;

    while (fen[0] != ' ') {
        switch (fen[0]) {

            case 'a': file = 1; break;
            case 'b': file = 2; break;
            case 'c': file = 3; break;
            case 'd': file = 4; break;
            case 'e': file = 5; break;
            case 'f': file = 6; break;
            case 'g': file = 7; break;
            case 'h': file = 8; break;


            case '1': rank = 2; break;
            case '2': rank = 3; break;
            case '3': rank = 4; break;
            case '4': rank = 5; break;
            case '5': rank = 6; break;
            case '6': rank = 7; break;
            case '7': rank = 8; break;
            case '8': rank = 9; break;

            case '-': file = -1; break; //flag that there is no ep square!

        }

        fen++;
    }

    if (file != -1) {cb->ep_square = (rank * 10 + file);}
    fen++;

    int nr_of_moves = 0;
    while (fen[0] != ' ') {

        if (nr_of_moves > 0) {
            nr_of_moves *= 10;
        }
        int a = fen[0] - '0';
        nr_of_moves += a;

        fen++;

    }

    cb->half_move_clock = nr_of_moves;
    fen++;
    nr_of_moves = 0;

    while (fen[0] != '\0') {

        if (nr_of_moves > 0) {
            nr_of_moves *= 10;
        }
        int a = fen[0] - '0';
        nr_of_moves += a;

        fen++;

    }

    cb->fullmoves = nr_of_moves;



}

void show_diagnostics(ChessBoard *cb) {
    display_board(cb);
    printf("%s to move.\n", cb->side? "white" : "black");

    printf("%s", cb->castle_perms & (WKC)? "K" : "-");
    printf("%s", cb->castle_perms & (WQC)? "Q" : "-");
    printf("%s", cb->castle_perms & (BKC)? "k": "-");
    printf("%s\n", cb->castle_perms & (BQC)? "q" : "-");

    printf("ep: %d\n", cb->ep_square);

    printf("half-moves: %d\n", cb->half_move_clock);

    printf("full-moves: %d\n", cb->fullmoves);

}
