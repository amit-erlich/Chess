#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Definition.h"
#include "chessPosArray.h"
#include "chessPosList.h"
#include "pathTree.h"
#include "check_utils.h"

/* ---- InitList -----
* Get: *lst
*
* Sets a list to null.
--------------------*/
void InitList(chessPosList* lst)
{
	lst->head = NULL;
	lst->tail = NULL;
}

/* -------- insertDataToEndList ---------
* Get: *lst, pos
*
* Insert new cell to the end of the list.
* If list is empty, sets the cell to be
* the head and tail.
---------------------------------------*/
void insertDataToEndList(chessPosList* lst, chessPos pos)
{
	chessPosCell* cellToAdd = allocateListCell(pos);

	if (lst->head == NULL)
		AddToEmptyList(lst, cellToAdd);
	else
		AddToEndOfList(lst, cellToAdd);
}

/* -------- insertDataToBeginningList ---------
* Get: *lst, pos
*
* Insert new cell to the beginning of the list.
* If list is empty, sets the cell to be
* the head and tail.
---------------------------------------------*/
void insertDataToBeginningList(chessPosList* lst, chessPos pos)
{
	chessPosCell* cellToAdd = allocateListCell(pos);

	if (lst->head == NULL)
		AddToEmptyList(lst, cellToAdd);
	else
		AddToBeginningOfList(lst, cellToAdd);
}

/* --------- AddToEmptyList ---------
* Get: *lst, *cellToAdd
*
* Insert new cell into an empty list.
-----------------------------------*/
void AddToEmptyList(chessPosList* lst, chessPosCell* cellToAdd)
{
	lst->head = lst->tail = cellToAdd;
}

/* --------- AddToBeginningOfList ---------
* Get: *lst, *cellToAdd
*
* Insert new node to the beginning of list.
-----------------------------------------*/
void AddToBeginningOfList(chessPosList* lst, chessPosCell* cellToAdd)
{
	cellToAdd->next = lst->head;
	lst->head = cellToAdd;
}

/* --------- AddToEndOfList ---------
* Get: *lst, *cellToAdd
*
* Insert new cell to the end of list.
-----------------------------------*/
void AddToEndOfList(chessPosList* lst, chessPosCell* cellToAdd)
{
	lst->tail->next = cellToAdd;
	lst->tail = cellToAdd;
}

/* -------- DeleteCurrCell ---------
* Get: *lst, *prev
*
* Deletes the current cell from list
----------------------------------*/
void DeleteCurrCell(chessPosList* lst, chessPosCell* prev)
{
	if (prev->next == lst->tail)
		DeleteFromEndOfList(lst, prev);
	else
		DeleteFromInnerPlaceInList(prev);
}

/* --- DeleteFromInnerPlaceInList ---
* Get: *prev
*
* Deletes an inner cell from the list
-----------------------------------*/
void DeleteFromInnerPlaceInList(chessPosCell* prev)
{
	chessPosCell* delCell = prev->next;
	prev->next = delCell->next;
	free(delCell);
}

/* --- DeleteFromEndOfList ---
* Get: *lst, *prev
*
* Deletes the tail of the list
----------------------------*/
void DeleteFromEndOfList(chessPosList* lst, chessPosCell* prev)
{
	free(prev->next);
	lst->tail = prev;
	lst->tail->next = NULL;
}

/* ------------------- insertNumAsBitToChar --------------------
* Get: *currC, num, *tempBits, *tempHasValue, *currBit
* 
* Write to currC num's bits, without corrupting currC value.
* Write tempBits's bits to currC as well, if tempBits has value.
--------------------------------------------------------------*/
void insertNumAsBitToChar(char* currC, char num, char* tempBits, int* tempHasValue, int* currBit)
{
	if (*currBit == 2)
	{
		/* insert to tempBits the bits that are going to be "thrown away" from num (1 bit) */
		*tempBits = num;
		(*tempBits) <<= 7;
		*tempHasValue = 1; /* mark that tempBits has value */
		num >>= 1; /* move num's bits 1 time to the right */
		*currBit = 7; /* next time, beggin to write from the 7th bit */
	}
	else if (*currBit == 1)
	{
		/* insert to tempBits the bits that are going to be "thrown away" from num (2 bits) */
		*tempBits = num;
		(*tempBits) <<= 6;
		*tempHasValue = 1; /* mark that tempBits has value */
		num >>= 2; /* move num's bits 2 times to the right */
		*currBit = 6; /* next time, beggin to write from the 6th bit */
	}
	else
	{
		num <<= ((*currBit) - 3); /* move num's bits 3 times to the left */
		*currBit = (*currBit) - 3; /* decrease currBit by 3, because 3 bits are going to be written into currC */
	}

	if (*tempHasValue)
	{
		if (*tempHasValue == 1) /* if tempHasValue = 1, there is no available space to write to currC */
			*tempHasValue = 2;
		else /* if tempHasValue = 2, there is available space to write tempBits to currC */
		{
			num |= (*tempBits); /* write tempBits bits with num bits to num */
			*tempBits = 0;
			*tempHasValue = 0;
		}
	}

	/* write num bits with currC bits to currC */
	*currC |= num;
}

/* ------------------------------- insertCharAsBitToNum -------------------------------
* Get: *pos, currC, *tempBits, *tempHasValue, *currBit, flagColTurn
*
* Write to pos in the flagColTurn place ( = pos[flagColTurn]) 3 bits from currC.
* If there are not enough bits (less then 3 to read), then save those bits in tempBits.
* If tempBits already has value, then complete the number of bits missing in tempBits,
* and also save 3 bits as noted above.
-------------------------------------------------------------------------------------*/
void insertCharAsBitToNum(chessPos* pos, unsigned char currC, char* tempBits, int* tempHasValue, int* currBit, int flagColTurn)
{
	if (*currBit == 2)
	{
		/* insert to tempBits the two bits left (those are part of 3 bits of a number) */
		currC <<= (8 - *currBit);
		currC >>= 5;
		*tempBits = currC;
		*tempHasValue = 1; /* mark that tempBits has value */
		*currBit = 7; /* next time, beggin to read from the 7th bit */
	}
	else if (*currBit == 1)
	{
		/* insert to tempBits the one bit left (its part of 3 bits of a number) */
		currC <<= (8 - *currBit);
		currC >>= 5;
		*tempBits = currC;
		*tempHasValue = 1; /* mark that tempBits has value */
		*currBit = 6; /* next time, beggin to read from the 6th bit */
	}
	else
	{
		if (*tempHasValue == 1) /* if ((*currBit == 7) || (*currBit == 6)) */
		{
			/* complete tempBits to 3 bits */
			(*tempBits) |= (currC >> *currBit);
			*tempHasValue = 2; /* mark that tempBits has 3 bits */
		}
		/* write 3 bits to (*pos)[flagColTurn] from currC */
		currC <<= (8 - *currBit);
		currC >>= 5;
		(*pos)[flagColTurn] = currC;

		if (!flagColTurn)
			(*pos)[flagColTurn] += 'A';
		else
			(*pos)[flagColTurn] += '1';

		*currBit = (*currBit) - 3; /* decrease currBit by 3, because 3 bits written into (*pos)[flagColTurn] */
	}
}

/* ---------------------- writeBinPosToFile -----------------------
* Get: *fileName, *savePosInBits, size, numOfChars
* 
* Open a file named fileName and write to it the number of 
* positions (size), and all the positions written in savePosInBits.
-----------------------------------------------------------------*/
void writeBinPosToFile(char* fileName, char* savePosInBits, unsigned short size, int numOfChars)
{
	FILE* fp;
	int i;

	fp = fopen(fileName, "wb");
	checkFileOpen(fp, fileName);

	/* write size to file */
	fwrite(&size, sizeof(unsigned short), 1, fp);
	/* write all positions (as bits) to file */
	fwrite(savePosInBits, sizeof(char), numOfChars, fp);

	fclose(fp);
}

/* ----------------- createChessPosListFromFile -----------------
* Get: *fileName, *isFileExist
* Return: *list
* 
* Check if file exist - if not - return false in isFileExist.
* Else, read from the file the number of positions, and then
* convert all positions written in the file into list of chessPos
* and return it.
---------------------------------------------------------------*/
chessPosList* createChessPosListFromFile(char* fileName, bool* isFileExist)
{
	FILE* file;
	chessPosList* list = NULL;
	chessPosCell* preCell = NULL;
	chessPos currPos;
	short numOfPos;
	int listIndx = 0, currBit = 8, tempHasValue = 0, flagColTurn = 0;
	unsigned char currC, tempBits = 0, rowOrCol;

	/* open files to read mode */
	file = fopen(fileName, "rb");
	if (!file) /* file not exist */
		*isFileExist = false;
	else
	{
		/* allocate list of chessPos */
		list = (chessPosList*)malloc(sizeof(chessPosList));
		checkAllocation(list, "list in createChessPosListFromFile.");
		InitList(list);

		/* read from file the number of positions written in it */
		fread(&numOfPos, sizeof(short), 1, file);

		if ((!feof(file)) && (numOfPos != 0)) /* file not ended and there is at least one position */
		{
			/* read the first char of position */
			fread(&currC, sizeof(char), 1, file);
			currPos[0] = currPos[1] = 0;

			while ((!feof(file)) && (listIndx < numOfPos))
			{
				/* if all bits in the current char already been read, then read another char */
				if ((currBit == 6) || (currBit == 7) || (currBit == 0))
				{
					fread(&currC, sizeof(char), 1, file);
					if (currBit == 0)
						currBit = 8;
				}

				/* insert to currPos[flagColTurn] the row or col as char (only if there are enough bits to read) */
				insertCharAsBitToNum(&currPos, currC, &tempBits, &tempHasValue, &currBit, flagColTurn);

				/* switch between row and col */
				if (flagColTurn == 0)
				{
					if (tempHasValue == 2)
					{
						/* temp has 3 bits, need to insert it to the col of the cell before */
						preCell->position[1] = tempBits + '1'; 
						tempHasValue = 0;
					}
					flagColTurn = 1; /* switch to col */
				}
				else
				{
					if (tempHasValue == 2)
					{
						/* temp has 3 bits, need to insert it to row */
						currPos[0] = tempBits + 'A';
						tempHasValue = 0;
					}
					flagColTurn = 0; /* switch to row */

					insertDataToEndList(list, currPos);
					listIndx++;
					/* save the previous cell in preCell */
					if (preCell == NULL)
						preCell = list->head;
					else
						preCell = preCell->next;
					currPos[0] = currPos[1] = 0;
				}
			}
			fclose(file);
		}
	}
	return list;
}

/* -------------- saveListToBinFile ---------------
* Get: *fileName, *pos_list
* 
* Save all positions in list as binary in fileName.
* (row and col are written in 3 bits each).
-------------------------------------------------*/
void saveListToBinFile(char* fileName, chessPosList* pos_list)
{
	unsigned short countPos = 0;
	int physical_size = 1, logical_size = 0, currBit = 8, tempHasValue = 0, flagRowTurn = 1;
	char* savePosInBits, tempBits = 0, rowOrCol;
	chessPosCell* currPos = pos_list->head;

	/* allocate array to start size (1) */
	savePosInBits = (char*)malloc(physical_size * sizeof(char));
	checkAllocation(savePosInBits, "savePosInBits - first allocation in saveListToBinFile.");
	savePosInBits[logical_size] = 0;

	/* while pos_list is not empty */
	while (currPos != NULL)
	{
		/* if flagRowTurn = 1, read to rowOrCol row position, else - read col position */
		if (flagRowTurn)
			rowOrCol = (char)((int)(currPos->position[0] - 'A'));
		else
			rowOrCol = (char)((int)(currPos->position[1] - '1'));

		/* if tempBits has value or savePosInBits[logical_size] is full, then increase logical_size.
		   (resize savePosInBits if needed)*/
		if ((currBit == 6) || (currBit == 7) || (currBit == 0))
		{
			logical_size++;
			if ((logical_size) == physical_size) /* resize array if needed */
			{
				physical_size = physical_size * 2;
				savePosInBits = (char*)realloc(savePosInBits, physical_size * sizeof(char));
				checkAllocation(savePosInBits, "savePosInBits - resizing array in saveListToBinFile.");
			}
			savePosInBits[logical_size] = 0;

			if (currBit == 0) /* if currBit is 0, set to 8 */
				currBit = 8;
		}
		/* insert rowOrCol value as bit to savePosInBits[logical_size] */
		insertNumAsBitToChar(&(savePosInBits[logical_size]), rowOrCol, &tempBits, &tempHasValue, &currBit);

		/* switch between row and col */
		if (flagRowTurn)
			flagRowTurn = 0;
		else
		{
			flagRowTurn = 1;
			countPos++;
			currPos = currPos->next;
		}
	}
	logical_size++;
	/* resize savePosInBits to the final size of the array */
	savePosInBits = (char*)realloc(savePosInBits, (logical_size) * sizeof(char));

	/* write to file all positions (in bits) from savePosInBits */
	writeBinPosToFile(fileName, savePosInBits, countPos, logical_size);

	free(savePosInBits);
}

/* ----------------------------- checkAndDisplayPathFromFile ------------------------------
* Get: *file_name
* Return: FILE_NOT_EXIST / PATH_NOT_LEGAL / PATH_COVER_ALL_BOARD / PATH_NOT_COVER_ALL_BOARD
* 
* Check if file exist - if not, return FILE_NOT_EXIST.
* Else, create a list with all the positions from the file.
* Check if path is legal - if not, return PATH_NOT_LEGAL.
* Else, display the list on the screen.
* Check if path covers all board - if it does, return PATH_COVER_ALL_BOARD.
* Else, return PATH_NOT_COVER_ALL_BOARD.
-----------------------------------------------------------------------------------------*/
int checkAndDisplayPathFromFile(char* file_name)
{
	bool isFileExist = true;
	chessPosList* list = createChessPosListFromFile(file_name, &isFileExist);

	if (!isFileExist)
		return FILE_NOT_EXIST;
	
	if (!isLegalPath(list))
	{
		free(list);
		return PATH_NOT_LEGAL;
	}
	display(list);

	if (isPathCoveringAllBoard(list))
	{
		free(list);
		return PATH_COVER_ALL_BOARD;
	}
	free(list);
	return PATH_NOT_COVER_ALL_BOARD;
}

/* ------------------------ isLegalPath ------------------------
* Get: *list
* Return: isLegal
* 
* Check if list contains legal path - return true, else - false.
--------------------------------------------------------------*/
bool isLegalPath(chessPosList* list)
{
	bool isLegal = true;
	chessPosArray*** posArray;
	if (list->head)
	{
		posArray = validKnightMoves();
		isLegal = isLegalPathRec(list->head, &posArray);
		freeValidMovesArr(posArray);
	}
	return isLegal;
}

/* ---------------- isLegalPathRec -----------------
* Get: *cell, ****posArray
* Return: true / false
* 
* Check if next cell contains a legal knight move.
* If not, return false.
* Else, call this function again with the next cell.
* If there is no next cell - return true.
--------------------------------------------------*/
bool isLegalPathRec(chessPosCell* cell, chessPosArray**** posArray)
{
	int i, row, col;
	row = (int)(cell->position[0] - 'A');
	col = (int)(cell->position[1] - '1');

	if (cell->next == NULL)
		return true;

	for (i = 0; i < (*posArray)[row][col]->size; i++)
		if ((cell->next->position[0] == ((*posArray)[row][col]->positions[i])[0]) && (cell->next->position[1] == ((*posArray)[row][col]->positions[i])[1]))
			return isLegalPathRec(cell->next, posArray);
	return false;
}

/* ------ isPathCoveringAllBoard -------
* Get: *list
* Return: true / false
* 
* Check if the list contains a path that
* covers all board.
--------------------------------------*/
bool isPathCoveringAllBoard(chessPosList* list)
{
	int savePosInList[ROWS][COLS] = { 0 };
	chessPosCell* curr = list->head;
	chessPos currPos;
	int curRow, curCol;
	int isAllTaken = 0;

	while (curr != NULL)
	{
		curRow = curr->position[0] - 'A';
		curCol = curr->position[1] - '1';
		savePosInList[curRow][curCol] = 1;  /* mark the position in savePosInList */
		curr = curr->next;
	}

	isAllTaken = allTaken(savePosInList); /* check if all the position in list are marked in savePosInList */

	if (isAllTaken == 0)
		return false;
	return true;
}

/* -------------------- display --------------------
* Get: * lst
*
* Deletes cells that already appeared in list and
* calls to function printBoardWithKnightPos to print
* the board with the knight positions
--------------------------------------------------*/
void display(chessPosList* lst)
{
	int count = 1, posArr[ROWS][COLS] = { 0 }, row, col;
	chessPosCell* curr = lst->head, * tempPrev = NULL;

	while (curr)
	{
		row = (int)(curr->position[0] - 'A');
		col = (int)(curr->position[1] - '1');

		if (posArr[row][col] == 0)
		{
			posArr[row][col] = count++;
			tempPrev = curr;
			curr = curr->next;
		}
		else /* when a cell is deleted, the cell is NOT the head */
		{
			DeleteCurrCell(lst, tempPrev);
			curr = tempPrev->next;
		}
	}
	printBoardWithKnightPos(posArr, ROWS);
}

/* ----- printBoardWithKnightPos ------
* Get: ** arr , size
*
* Prints a board with knight positions
* (letter from A-H, number between 1-8)
-------------------------------------*/
void printBoardWithKnightPos(int arr[][COLS], int size)
{
	int i, j;

	printALine(); /* print "_" line at the head of the board */

	for (i = 0; i <= ROWS; i++)
	{
		for (j = 0; j <= COLS; j++)
		{
			putchar('|');
			if (i == 0) /* print the first line with the col number */
			{
				if (j == 0)
					printf("    ");
				else
					printf("  %d ", j);
			}
			else if (j == 0) /* print the first column with the row number */
			{
				printf("  %c ", (char)('A' + i - 1));
			}
			else
			{
				if (arr[i - 1][j - 1] != 0)
					printf(" %2d ", arr[i - 1][j - 1]);
				else
					printf("    ");
			}
		}
		printf("|\n");
		printALine();
	}
}

/* --------- printALine ---------
* Gets: col
*
* Prints "_" line under the board
-------------------------------*/
void printALine()
{
	int i;

	for (i = 0; i <= COLS; i++)
		printf("+----");
	printf("+\n");
}

/* ----- freeList ------
* Get: *lst
*
* Free all cells in list
----------------------*/
void freeList(chessPosList* lst)
{
	chessPosCell* curr = lst->head, * tempNext;

	while (curr)
	{
		tempNext = curr->next;
		free(curr);
		curr = tempNext;
	}
	lst->tail = NULL;
}

/* -------- allocateListCell ---------
* Get: pos
* Return: res
*
* Create a ListNode with the data (pos)
* and a next pointer.
------------------------------------*/
chessPosCell* allocateListCell(chessPos pos)
{
	chessPosCell* res = (chessPosCell*)malloc(sizeof(chessPosCell));
	checkAllocation(res, "res in allocateListCell");

	copyArr(res->position, pos);
	res->next = NULL;

	return res;
}

/* --------- allocateList ----------
* Return: res
*
* Create a List and initial the list
----------------------------------*/
chessPosList* allocateList()
{
	chessPosList *res = (chessPosList*)malloc(sizeof(chessPosList));
	checkAllocation(res, "res in allocateList");

	InitList(res);

	return res;
}