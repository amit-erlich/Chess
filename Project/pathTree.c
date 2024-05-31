#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Definition.h"
#include "chessPosArray.h"
#include "chessPosList.h"
#include "pathTree.h"
#include "check_utils.h"

/* ------- InitTreeNodeListCell -------
* Get: *lst
*
* Sets lst's node and next node to NULL
-------------------------------------*/
void InitTreeNodeListCell(treeNodeListCell* lst)
{
	lst->node = NULL;
	lst->next = NULL;
}

/* ----------- setTreeNode ------------
* Get: *node, pos
*
* Insert pos values into node position,
* allocate next_possible_positions and
* sets its attributes to NULL.
-------------------------------------*/
void setTreeNode(treeNode* node, chessPos pos)
{
	copyArr(node->position, pos);

	node->next_possible_positions = (treeNodeListCell*)malloc(sizeof(treeNodeListCell));
	checkAllocation(node->next_possible_positions, "node->next_possible_positions in setTreeNode");

	InitTreeNodeListCell(node->next_possible_positions);
}

/* ------------- insertDataToEndList -------------
* Get: *head, pos
*
* Insert new node to the end of the list (head
* is the head of the list). If list if empty
* (there is no head), sets the node to be the node
* of the head.
------------------------------------------------*/
void insertDataToEndTreeNodeListCell(treeNodeListCell* head, chessPos pos)
{
	treeNode* nodeToAdd = allocateTreeNode(pos);

	if (head->node == NULL)
		AddToEmptyTreeNodeListCell(head, nodeToAdd);
	else
	{
		treeNodeListCell* tail = findTailOfTreeNodeListCell(head); /* get the tail of the list */
		AddToEndOfTreeNodeListCell(tail, nodeToAdd);
	}
}

/* --- AddToEmptyTreeNodeListCell ---
* Get: *head, *nodeToAdd
*
* Insert new node into an empty list.
-----------------------------------*/
void AddToEmptyTreeNodeListCell(treeNodeListCell* head, treeNode* nodeToAdd)
{
	head->node = nodeToAdd;
}

/* --- AddToEndOfTreeNodeListCell ---
* Get: *tail, *nodeToAdd
*
* Insert new node to the end of list.
-----------------------------------*/
void AddToEndOfTreeNodeListCell(treeNodeListCell* tail, treeNode* nodeToAdd)
{
	tail->next = allocateTreeNodeListCell();

	tail->next->node = nodeToAdd;
	tail = tail->next;
}

/* ----------- findAllPossibleKnightPaths ------------
* Get: *startingPosition
* Return: tree
*
* Builds a tree with all the possible moves the knight
* can do and returns it.
----------------------------------------------------*/
pathTree findAllPossibleKnightPaths(chessPos* startingPosition)
{
	pathTree tree;
	tree.root = NULL;
	int takenPosition[ROWS][COLS] = { 0 }; /* this array will save all the places whose list of children has already been inserted into the tree */
	chessPosArray*** posArray;

	if (startingPosition)
	{
		/* allocate the root */
		tree.root = (treeNode*)malloc(sizeof(treeNode));
		checkAllocation(tree.root, "tree.root in findAllPossibleKnightPaths");
		setTreeNode(tree.root, *startingPosition); /* set the root with the given position */

		/* sets posArray to contain valid positions that the knight can move to */
		posArray = validKnightMoves();
		
		/* calls this function to build the tree */
		findAllPossibleKnightPathsRec(tree.root, takenPosition, &posArray);

		freeValidMovesArr(posArray);
	}
	return tree;
}

/* ------------- findAllPossibleKnightPathsRec -------------
* Get: *root, takenPosition[][COLS], ****posArray
*
* Builds root's list of all possible moves a knight can do,
* and marks the root position in takenPosition.
* Calls this function again with every node of the list of
* possible moves.
* If the root position is already marked (in takenPosition),
* the function ends.
----------------------------------------------------------*/
void findAllPossibleKnightPathsRec(treeNode* root, int takenPosition[][COLS], chessPosArray**** posArray)
{
	int row, col;
	treeNodeListCell* rootPossiblePos;
	row = (int)(root->position[0] - 'A');
	col = (int)(root->position[1] - '1');

	if (takenPosition[row][col] != 0) /* if root position is marked */
		return;

	buildList(root, posArray); /* build list of possible next moves */
	takenPosition[row][col] = 1; /* mark the root position */

	for (rootPossiblePos = root->next_possible_positions; rootPossiblePos; rootPossiblePos = rootPossiblePos->next)
		findAllPossibleKnightPathsRec(rootPossiblePos->node, takenPosition, posArray);

	takenPosition[row][col] = 0; /* unmark the root position */
}

/* --------------------- buildList ---------------------
* Get: *root, ****posArray
*
* Build next_possible_positions list with the values in
* posArray in place [i][j].
-----------------------------------------------------*/
void buildList(treeNode* root, chessPosArray**** posArray)
{
	int row, col;
	row = (int)(root->position[0] - 'A');
	col = (int)(root->position[1] - '1');

	arrayToList(root->next_possible_positions, (*posArray)[row][col]);
}

/* ----------- arrayToList ------------
* Get:  *possiblePosition, *posArr
* Return:
*
* Function sets *possiblePosition cells
* to the data in array *posArr
-------------------------------------*/
void arrayToList(treeNodeListCell* possiblePosition, chessPosArray* posArr)
{
	int i;

	for (i = 0; i < posArr->size; i++)
		insertDataToEndTreeNodeListCell(possiblePosition, posArr->positions[i]);
}

/* ---- findTailOfTreeNodeListCell ----
* Get: * head
* Return: curr
*
* Function returns the tail of the list
-------------------------------------*/
treeNodeListCell* findTailOfTreeNodeListCell(treeNodeListCell* head)
{
	treeNodeListCell* curr = head;

	while (curr->next)
		curr = curr->next;

	return curr;
}

/* ------------- findknightPathCoveringAllBoard -------------
* Get: *path_tree
* Return: *list
* 
* Call rec function to try to find a path covering all board.
* If path found - returns the path as a list,
* else - returns list = NULL.
-----------------------------------------------------------*/
chessPosList* findknightPathCoveringAllBoard(pathTree* path_tree)
{
	chessPosList* list = NULL;
	int takenPosition[ROWS][COLS] = { 0 };
	
	if (path_tree->root)
		list = findknightPathCoveringAllBoardRec(path_tree->root, takenPosition);

	return list;
}

/* ------------ findknightPathCoveringAllBoardRec -------------
* Get: *root, takenPosition[][COLS]
* Return: *list
* 
* Try to find a path from root to a leaf that covers all board.
* If path found - returns it, else - return list = NULL.
-------------------------------------------------------------*/
chessPosList* findknightPathCoveringAllBoardRec(treeNode* root, int takenPosition[][COLS])
{
	chessPosList* list = NULL;
	treeNodeListCell* rootPossiblePos;
	int row, col;
	row = (int)(root->position[0] - 'A');
	col = (int)(root->position[1] - '1');

	if (allTaken(takenPosition)) /* if found a path covering all board */
	{
		/* create a list and return it */
		list = allocateList();
		return list;
	}

	if (root->next_possible_positions->node == NULL) /* if current root is a leaf */
		return list;

	takenPosition[row][col]++; /* mark the root position */

	for (rootPossiblePos = root->next_possible_positions; rootPossiblePos && (list == NULL); rootPossiblePos = rootPossiblePos->next)
		list = findknightPathCoveringAllBoardRec(rootPossiblePos->node, takenPosition);

	if (list == NULL) /* if path not found */
		takenPosition[row][col]--; /* unmark the root position */
	else
		insertDataToBeginningList(list, root->position); /* insert curr pos to the beginning of the list */
	return list;
}

/* ------------ allTaken --------------
* Get: **takenPosition
* Return: isAllTaken
* 
* Function checks if each cell in array
* contains 1
* if so, returns 1, else 0
-------------------------------------*/
int allTaken(int takenPosition[][COLS])
{
	int i, j, isAllTaken = 1;
	for (i = 0; (i < ROWS) && isAllTaken; i++)
		for (j = 0; (j < COLS) && isAllTaken; j++)
			if (takenPosition[i][j] == 0)
				isAllTaken = 0;
	return isAllTaken;
}

/* -- printTreeNodeListCell --
* Get: * head
*
* Prints the list cells
----------------------------*/
void printTreeNodeListCell(treeNodeListCell* head)
{
	treeNodeListCell* curr;

	for (curr = head; curr; curr = curr->next)
		printf("(%c,%c) ", curr->node->position[0], curr->node->position[1]);
	putchar('\n');
}

/* --------------- freeTree --------------
* Get: tree
*
* Free tree using function freeTreeNodeRec
----------------------------------------*/
void freeTree(pathTree* tree)
{
	if (tree->root)
	{
		freeTreeNodeRec(tree->root);
		tree->root = NULL;
	}
}

/* -------- freeTreeNodeRec -------------
* Get: root
*
* For each leaf, free list of
* possible positions, then free the root
----------------------------------------*/
void freeTreeNodeRec(treeNode* root)
{
	treeNodeListCell* curr = root->next_possible_positions, * tempNext;

	while (curr)
	{
		if (curr->node)
			freeTreeNodeRec(curr->node);
		tempNext = curr->next;
		free(curr);
		curr = tempNext;
	}
	free(root);
}

/* ----- allocateTreeNodeListCell -------
* Return: res
*
* Create a ListNode with the data
* and a next pointer initialized to NULL.
---------------------------------------*/
treeNodeListCell* allocateTreeNodeListCell()
{
	treeNodeListCell* res = (treeNodeListCell*)malloc(sizeof(treeNodeListCell));
	checkAllocation(res, "res in allocateTreeNodeListCell");

	InitTreeNodeListCell(res);

	return res;
}

/* -------- allocateTreeNode ----------
* Get: pos
* Return: res
*
* Create a ListNode with the data (pos)
* and a pointer to positions list.
------------------------------------*/
treeNode* allocateTreeNode(chessPos pos)
{
	treeNode* res = (treeNode*)malloc(sizeof(treeNode));
	checkAllocation(res, "res in allocateTreeNode");

	setTreeNode(res, pos);

	return res;
}