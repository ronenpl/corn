all:
	gcc -Werror -Wall -pedantic -o corn -Ofast corn.c board.c bitboard_utils.c defs.c moves.c movegen.c
