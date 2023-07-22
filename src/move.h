#include "game.h"
#include <stdlib.h>
#include <stddef.h>

#ifndef MOVE_H
#define MOVE_H

size_t LenPossibleMoveBishop(struct piece **board, struct piece *p);
void CreatePossibleMoveBishop(struct piece **board, struct piece *p, size_t cpt);

size_t LenPossibleMoveKing(struct piece **board, struct piece *p);
void CreatePossibleMoveKing(struct piece **board, struct piece *p);

size_t LenPossibleMoveKnight(struct piece **board, struct piece *p);
void CreatePossibleMoveKnight(struct piece **board, struct piece *p);

size_t LenPossibleMovePawn(struct piece **board, struct piece *p);
void CreatePossibleMovePawn(struct piece **board, struct piece *p);

void CreatePossibleMoveQueen(struct piece **board, struct piece *p);

size_t LenPossibleMoveRock(struct piece **board, struct piece *p);
void CreatePossibleMoveRock(struct piece **board, struct piece *p);

int move(struct piece **board, int start, int end);
void MovePiece(struct piece **board, struct piece *p, int dst);
void createAllMove(struct piece **board, int color);
void createMovePiece(struct piece **board, struct piece *p);

#endif