all:
	gcc -Werror -Wall -pedantic -o corn -g -O0 corn.c board.c bitboard_utils.c defs.c moves.c
