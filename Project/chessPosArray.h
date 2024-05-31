#pragma once

typedef char chessPos[2];

typedef struct _chessPosArray
{
	unsigned int size;
	chessPos* positions;
}chessPosArray;


/* --------------------------------------- Function Declaration ---------------------------------------- */
/* Get, Set and Delete Functions */
void getPositions(chessPosArray* array, int row, int col);
void setPos(chessPos cPos, int* index, chessPos temp);

chessPosArray*** validKnightMoves();
void twoUpDownOneRightLeft(chessPos cPos, int row, int col, int upOrDown, int rightOrLeft);
void twoRightLeftOneUpDown(chessPos cPos, int row, int col, int rightOrLeft, int upOrDown);

void copyArr(chessPos dest, chessPos src);

/* Print Functions */
void printArray(chessPos* arr, unsigned int size);

/* Free and Reallocation Functions */
void freeValidMovesArr(chessPosArray*** validMovesArr);