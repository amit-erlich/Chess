#pragma once
#include <stdbool.h>
#include "Definition.h"

#define COLS 5
typedef struct _chessPosCell
{
	chessPos position;
	struct _chessPosCell* next;
}chessPosCell;

typedef struct _chessPosList
{
	chessPosCell* head;
	chessPosCell* tail;
}chessPosList;

/* --------------------------------------- Function Declaration ---------------------------------------- */
/* Get, Set and Delete Functions */
void InitList(chessPosList* lst);
void insertDataToEndList(chessPosList* lst, chessPos pos);
void insertDataToBeginningList(chessPosList* lst, chessPos pos);
void AddToEmptyList(chessPosList* lst, chessPosCell* cellToAdd);
void AddToBeginningOfList(chessPosList* lst, chessPosCell* cellToAdd);
void AddToEndOfList(chessPosList* lst, chessPosCell* cellToAdd);
void DeleteCurrCell(chessPosList* lst, chessPosCell* prev);
void DeleteFromInnerPlaceInList(chessPosCell* prev);
void DeleteFromEndOfList(chessPosList* lst, chessPosCell* prev);

void insertNumAsBitToChar(char* currC, char num, char* tempBits, int* tempHasValue, int* currBit);
void insertCharAsBitToNum(chessPos* pos, unsigned char currC, char* tempBits, int* tempHasValue, int* currBit, int flagColTurn);

void writeBinPosToFile(char* fileName, char* savePosInBits, unsigned short size, int numOfChars);

chessPosList* createChessPosListFromFile(char* fileName, bool* isFileExist);

void saveListToBinFile(char* fileName, chessPosList* pos_list);

int checkAndDisplayPathFromFile(char* file_name);
bool isLegalPath(chessPosList* list);
bool isLegalPathRec(chessPosCell* cell, chessPosArray**** posArray);
bool isPathCoveringAllBoard(chessPosList* list);

void display(chessPosList* lst);

/* Print Functions */
void printBoardWithKnightPos(int arr[][COLS], int size);
void printALine();

/* Free and Reallocation Functions */
void freeList(chessPosList* lst);
chessPosCell* allocateListCell(chessPos pos);
chessPosList* allocateList();