#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "game.h"



struct piece *newPiece(char name)
{
    struct piece *res =  malloc(sizeof(struct piece));
    if(name == 'p')
        res->value = 100;
    else if (name == 'b')
        res->value = 300;
    else if (name == 'n')
        res->value = 300;
    else if (name == 'r')
        res->value = 500;
    else if (name == 'q')
        res->value = 900;
    else if (name == 'k')
        res->value = 10000;
    else
    {
        free(res);
        res = NULL;
        printf("NewPiece: not a valid name\n");
        fflush(stdout);
    }
    if(res)
    {
        res->hasMoved = 0;
        res->isWhite = 0;
        res->nbMoves = 0;
        res->name = name;
        res->possibleMoves = malloc(0);
    }
    return res;
}

void freePiece(struct piece *p)
{
    free(p->possibleMoves);
    free(p);
}

void freeBoard(struct piece **board)
{
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            struct piece *p = board[i * 8 + j];
            if (p != NULL)
            {
                freePiece(p);
            }   
        }   
    }
    free(board);
}

struct piece **FenToBoard(char *fen)
{
    struct piece **board = malloc(64 * sizeof(struct piece *));
    size_t iBoard = 0;
    size_t i = 0;
    while(fen[i] != 0)
    {
        char letter = fen[i];
        if(letter - '0' >= 0 && letter - '0' <= 9)
        {
            for (size_t j = 0; j < (size_t)letter - '0'; j++)
            {
                board[iBoard] = NULL;
                iBoard++;
            }
        }
        else
        {
            char c = letter;
            int isWhite = 0;
            if(isupper(c))
            {
                isWhite = 1;
                c = tolower(c);
            }
            switch (c)
            {
            case 'r':
                board[iBoard] = newPiece('r');
                break;
            case 'k':
                board[iBoard] = newPiece('k');
                break;
            case 'q':
                board[iBoard] = newPiece('q');
                break;
            case 'n':
                board[iBoard] = newPiece('n');
                break;
            case 'b':
                board[iBoard] = newPiece('b');
                break;
            case 'p':
                board[iBoard] = newPiece('p');
                break;
            case '/':
                i++;
                continue;
            default:
                printf("erreur notation fen : %c at i = %ld\n", c, i);
                break;
            }
            board[iBoard]->pos = iBoard;
            board[iBoard]->pos = isWhite;
            iBoard++;
        }

        i++;
    }
    return board;
}



/*

int placePiece(struct piece **board, char* name, int pos)
{
    int res = 0;
    if(pos < 0 || pos > 63)
    {
        printf("place_piece: Error in position\n");
        res = -1;
    }
    else if(board[pos] != NULL)
    {
        printf("place_piece: A piece is already there!\n");
        res = 1;
    }
    else
    {
        board[pos] = newPiece(name);
        board[pos]->pos = pos;
    }
    return res;
}

//TODO
int movePiece(struct piece **board, int pos, int dest)
{
    return __movePiece(board, pos, dest, 0);
}

//used to be able to not filter moves. else use movePiece
int __movePiece(struct piece **board, int pos, int dest, int filterMoves)
{
    struct piece *piece = board[pos];
    int res = 0;
    if(piece != NULL)
    {
        int isPossible = 0;
        int i =0;
        while(i<piece->nbMoves && !isPossible)
        {
            isPossible = piece->possibleMoves[i] == dest;
            i++;
        }
        if(isPossible)
        {
            if (!strcmp(piece->name, "king"))
            {
                if (piece->pos == dest-2 || piece->pos == dest+2)
                {
                    size_t k = 0;
                    size_t k_dst = 0;
                    if (dest < piece->pos)
                    {
                        k = piece->pos-4;
                        k_dst = piece->pos-1;
                    }
                    else
                    {
                        k = piece->pos+3;
                        k_dst = piece->pos+1;
                    }
                    board[k_dst] = board[k];
                    board[k] = NULL;
                    board[k_dst]->hasMoved = 1;
                    board[k_dst]->pos = k_dst;
                }
                
            }
            
            if(board[dest] != NULL)
            {
                //eats the piece on the position
                freePiece(board[dest]);
            }
            
            board[dest] = board[pos];
            board[pos] = NULL;
            piece->hasMoved = 1;
            piece->pos = dest;
            if (!strcmp(piece->name, "pawn"))
            {
                if (piece->isWhite && piece->pos / 8 == 7)
                {
                    struct piece *queen =  malloc(sizeof(struct piece));
                    queen->name = malloc(6);
                    queen->possibleMoves = malloc(0);
                    queen->isWhite = piece->isWhite;
                    queen->nbMoves = 0;
                    queen->pos = piece->pos;
                    queen->hasMoved = 1;
                    memcpy(queen->name, "queen", 6);
                    freePiece(piece);
                    board[dest] = queen;
                }
                else
                {
                    if (!piece->isWhite && piece->pos / 8 == 0)
                    {
                        struct piece *queen =  malloc(sizeof(struct piece));
                        queen->name = malloc(6);
                        queen->possibleMoves = malloc(0);
                        queen->isWhite = piece->isWhite;
                        queen->nbMoves = 0;
                        queen->pos = piece->pos;
                        queen->hasMoved = 1;
                        memcpy(queen->name, "queen", 6);
                        freePiece(piece);
                        board[dest] = queen;
                    }
                }
            }
            
        }
        CalculateColorMoves(board,!(piece->isWhite), filterMoves);
        res = isPossible;
    }
    return res;
}

int movePieceNoFree(struct piece **board, int pos, int dest)
{
    struct piece *piece = board[pos];
    int res = 0;
    if(piece != NULL)
    {
        int isPossible = 0;
        int i =0;
        while(i<piece->nbMoves && !isPossible)
        {
            isPossible = piece->possibleMoves[i] == dest;
            i++;
        }
        if(isPossible)
        {
            if (!strcmp(piece->name, "king"))
            {
                if (piece->pos == dest-2 || piece->pos == dest+2)
                {
                    size_t k = 0;
                    size_t k_dst = 0;
                    if (dest < piece->pos)
                    {
                        k = piece->pos-4;
                        k_dst = piece->pos-1;
                    }
                    else
                    {
                        k = piece->pos+3;
                        k_dst = piece->pos+1;
                    }

                    board[k_dst] = board[k];
                    board[k] = NULL;
                    board[k_dst]->hasMoved = 1;
                    board[k_dst]->pos = k_dst;
                }
                
            }
            board[dest] = board[pos];
            board[pos] = NULL;
            piece->hasMoved = 1;
            piece->pos = dest;
            if (!strcmp(piece->name, "pawn"))
            {
                if (piece->isWhite && piece->pos / 8 == 7)
                {
                    struct piece *queen =  malloc(sizeof(struct piece));
                    queen->name = malloc(6);
                    queen->possibleMoves = malloc(0);
                    queen->isWhite = piece->isWhite;
                    queen->nbMoves = 0;
                    queen->pos = piece->pos;
                    queen->hasMoved = 1;
                    memcpy(queen->name, "queen", 6);
                    freePiece(piece);
                    board[dest] = queen;
                }
                else
                {
                    if (!piece->isWhite && piece->pos / 8 == 0)
                    {
                        struct piece *queen =  malloc(sizeof(struct piece));
                        queen->name = malloc(6);
                        queen->possibleMoves = malloc(0);
                        queen->isWhite = piece->isWhite;
                        queen->nbMoves = 0;
                        queen->pos = piece->pos;
                        queen->hasMoved = 1;
                        memcpy(queen->name, "queen", 6);
                        freePiece(piece);
                        board[dest] = queen;
                    }
                }
            }
            
        }
        res = isPossible;
        CalculateColorMoves(board,!(piece->isWhite), 0);
    }
    else{
        printf("piece is null\n");
        exit(EXIT_FAILURE);
    }
    return res;
}

void turn(struct piece **board, int isWhiteTurn)
{
    while(1)
    {
        print_chess(board);
        printf("Enter piece position then destinaton:\n");
        char* piecePos = calloc(3, 1);
        char* dest = calloc(3, 1);
        fgets(piecePos, 3, stdin);
        getchar();
        //si pas de piece, redemander.
        fgets(dest, 3, stdin);
        getchar();
        CalculateColorMoves(board, isWhiteTurn, 1);


        if(piecePos[0] < 'a' || piecePos[0] > 'h' || 
            piecePos[1] < '1' || piecePos[1] > '8' )
        {
            printf("incorrect character. ");
            continue;
        }
        int p1 = (piecePos[1] - '1')*8 + piecePos[0] - 'a';
        int p2 = (dest[1] - '1')*8 + dest[0] - 'a';

        printf("pos = %d to %d\n", p1,p2);

        if(!board[p1] || !(board[p1]->isWhite == isWhiteTurn))
        {
            printf("No piece on this position. ");
            continue;
        }
        if(movePiece(board, p1,p2))
        {
            break;
        }
        printf("Position not accessible. ");
    }

    printf("line 183\n");
    int nextMoves = CalculateColorMoves(board, !isWhiteTurn, 1);
    printf("line 185\n");
    if(nextMoves == 0)
    {
        if(isWhiteTurn)
            printf("White Won!\n");
        else
            printf("Black Won!\n");
    }
    else
    {
        //calls again for next turn from other player
        turn(board, !isWhiteTurn);
    }
}

int CalculateColorMoves(struct piece** board, int isWhite, int filterMoves)
{
    int res = 0;
    for(size_t i = 0; i<64; i++)
    {
        struct piece *p = board[i];
        if(p != NULL && p->isWhite == isWhite)
        {
            //printf("doing %s at pos %d\n", p->name, i);
            //fflush(stdout);
            size_t len = strlen(p->name);
            
            if(strncmp(p->name, "pawn", len) == 0)
                CreatePossibleMovePawn(board, p);            

            else if (strncmp(p->name, "knight", len) == 0)
                CreatePossibleMoveKnight(board, p);
            else if (strncmp(p->name, "bishop", len) == 0)
                CreatePossibleMoveBishop(board, p, 0);

            else if (strncmp(p->name, "rook", len) == 0)
                CreatePossibleMoveTower(board, p);

            else if (strncmp(p->name, "queen", len) == 0)
                CreatePossibleMoveQueen(board, p);
        
            else if(strncmp(p->name, "king", len) == 0)
                CreatePossibleMoveKing(board, p);

            if(filterMoves)
                FilterMoves(board, p);
            res+= p->nbMoves;
        }
    }
    return res;
}

void FilterMoves(struct piece **board, struct piece *p)
{
    int removed = 0;
    for (int i = 0; i<p->nbMoves; i++)
    {
    fflush(stdout);
        if(TestCheckmate(board,p,p->possibleMoves[i]))
        {
            p->possibleMoves[i] = -1;
            removed++;
        }
    }
    int* newmoves = malloc(sizeof(int) * (p->nbMoves-removed));
    int j = 0;
    for (int i = 0; i<p->nbMoves; i++)
    {
        if(p->possibleMoves[i] >= 0)
        {
            newmoves[j] = p->possibleMoves[i];
            j++;
        }
    }
    p->nbMoves -= removed;
    free(p->possibleMoves);
    p->possibleMoves = newmoves;
}

int __TestCheckmate(struct piece **board, int isWhite)
{
    for(size_t i = 0; i<63; i++)
    {
        struct piece *p = board[i];
        if(p != NULL && p->isWhite != isWhite)
        {
            size_t len = strlen(p->name);
            
            if(strncmp(p->name, "pawn", len) == 0)
                CreatePossibleMovePawn(board, p);            

            else if (strncmp(p->name, "knight", len) == 0)
                CreatePossibleMoveKnight(board, p);
            else if (strncmp(p->name, "bishop", len) == 0)
                CreatePossibleMoveBishop(board, p, 0);

            else if (strncmp(p->name, "rook", len) == 0)
                CreatePossibleMoveTower(board, p);

            else if (strncmp(p->name, "queen", len) == 0)
                CreatePossibleMoveQueen(board, p);
        
            else if(strncmp(p->name, "king", len) == 0)
                CreatePossibleMoveKing(board, p);
            
            for(int k = 0; k<p->nbMoves;k++)
            {
                struct piece *p2 = board[p->possibleMoves[k]];
                if(p2 && strncmp(p2->name, "king", strlen(p2->name)) == 0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int TestCheckmate(struct piece **board ,struct piece *piece, int dest)
{
    struct move *m = newMove(board, piece->pos, dest);
    board[m->start] = pieceCopy(piece);
    movePieceNoFree(board, piece->pos, dest);
    int res = __TestCheckmate(board, piece->isWhite);
    undoMove(board, m);
    free(m);
    return res;
}

int isCheck(struct piece **board, int isWhite)
{
    CalculateColorMoves(board, !isWhite, 0);
    for(size_t i = 0; i<63; i++)
    {
        struct piece *p = board[i];
        if(p!=NULL && p->isWhite != isWhite)
        {
            for(int k = 0; k<p->nbMoves; k++)
            {
                struct piece *p2 = board[p->possibleMoves[k]];
                if(p2 != NULL && strncmp(p2->name,"king", strlen(p2->name))==0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}*/