#pragma once
#include "Definition.h"
#include "chessPosList.h"

typedef struct _treeNodeListCell treeNodeListCell;

typedef struct _treeNode
{
	chessPos position;
	treeNodeListCell* next_possible_positions;
}treeNode;

typedef struct _treeNodeListCell
{
	treeNode* node;
	struct _treeNodeListCell* next;
}treeNodeListCell;

typedef struct _pathTree
{
	treeNode* root;
}pathTree;

/* --------------------------------------- Function Declaration ---------------------------------------- */
/* Get, Set and Delete Functions */
void InitTreeNodeListCell(treeNodeListCell* lst);
void setTreeNode(treeNode* node, chessPos pos);
void insertDataToEndTreeNodeListCell(treeNodeListCell* head, chessPos pos);
void AddToEmptyTreeNodeListCell(treeNodeListCell* head, treeNode* cellToAdd);
void AddToEndOfTreeNodeListCell(treeNodeListCell* tail, treeNode* cellToAdd);

pathTree findAllPossibleKnightPaths(chessPos* startingPosition);
void findAllPossibleKnightPathsRec(treeNode* root, int takenPosition[][COLS], chessPosArray**** posArray);
void buildList(treeNode* root, chessPosArray**** posArray);
void arrayToList(treeNodeListCell* possiblePosition, chessPosArray* posArr);
treeNodeListCell* findTailOfTreeNodeListCell(treeNodeListCell* head);

chessPosList* findknightPathCoveringAllBoard(pathTree *path_tree);
chessPosList* findknightPathCoveringAllBoardRec(treeNode* root, int takenPosition[][COLS]);
int allTaken(int takenPosition[][COLS]);

/* Print Functions */
void printTreeNodeListCell(treeNodeListCell* head);

/* Free and Reallocation Functions */
void freeTree(pathTree* tree);
void freeTreeNodeRec(treeNode* root);
treeNodeListCell* allocateTreeNodeListCell();
treeNode* allocateTreeNode(chessPos pos);