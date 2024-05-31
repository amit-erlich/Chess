#pragma once
#include <stdbool.h>

#define ROWS 5
#define COLS 5

#define maxPossiblePosSize 8

#define FILE_NOT_EXIST -1
#define PATH_NOT_LEGAL 1
#define PATH_COVER_ALL_BOARD 2
#define PATH_NOT_COVER_ALL_BOARD 3

#define isLetter(c)  ( ((c) >= 'A') && ((c) <= 'H') ) ? (true) : (false)
#define isDigit(c)	( ((c) >= '1') && ((c) <= '8') ) ? (true) : (false)

#include "chessPosArray.h"
#include "pathTree.h"


/* --------------------------------------- Function Declaration ---------------------------------------- */
void chessGame();
char displayMenu();
void getStartPosition(chessPos* chPos, bool* chPosHasValue);
void buildTree(pathTree* tree, chessPos* chPos, bool chPosHasValue);
void findPathCoveringAllBoard(chessPosList** list, pathTree* tree, chessPos* chPos, bool chPosHasValue);
void createFlieWithPathCoveringAllBoardPositions(chessPosList** list, pathTree* tree, chessPos* chPos, bool chPosHasValue);
void checkPathFromFile();


/* Get and Set Funtions */
char* getString();

/* Check Functions */
bool checkFileNameValidtion(char* fileName);