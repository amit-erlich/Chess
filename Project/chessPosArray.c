#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Definition.h"
#include "chessPosArray.h"
#include "check_utils.h"

/* ----------- getPositions -------------
* Get: *array, row, col
*
* Sets *array with all valid movements of
* knight that is in position (row, col),
* and save the array size.
---------------------------------------*/
void getPositions(chessPosArray* array, int row, int col)
{
	int indx = 0, i, indxArr[8] = { 1, 1, -1, -1, 1, -1, 1, -1 };
	chessPos temp;

	/* goes over all possible movements of the knight, and if possible -> insert position into array */
	for (i = 0; i < 4; i++)
	{
		twoUpDownOneRightLeft(temp, row, col, indxArr[i], indxArr[i + 4]);
		setPos((array->positions)[indx], &indx, temp);
		twoRightLeftOneUpDown(temp, row, col, indxArr[i], indxArr[i + 4]);
		setPos((array->positions)[indx], &indx, temp);
	}
	/* if array size is smaller than 8, resize the array */
	if (indx < maxPossiblePosSize)
	{
		array->positions = (chessPos*)realloc(array->positions, indx * sizeof(chessPos));
		checkAllocation(array->positions, "array in getPositions.");
	}
	array->size = indx;
}

/* -------------- setPos ---------------
* Get: cPos, *index, temp
*
* Copys temp values to cPos (if temp[0]
* is not 'x'), and increases index value
--------------------------------------*/
void setPos(chessPos cPos, int* index, chessPos temp)
{
	if (temp[0] != 'x') /* if temp contains real position */
	{
		cPos[0] = temp[0];
		cPos[1] = temp[1] + 1;
		(*index)++;
	}
}

/* -------------- validKnightMoves --------------
* Return: ***validMovesArr
*
* Sets an array of pointers to an array of
* valid knight moves, so that every cell contains
* valid positions that the knight can move to.
-----------------------------------------------*/
chessPosArray*** validKnightMoves()
{
	int i, j;
	chessPosArray*** validMovesArr;

	validMovesArr = (chessPosArray***)malloc(ROWS * sizeof(chessPosArray**));
	checkAllocation(validMovesArr, "validMovesArr in validKnightMoves.");

	for (i = 0; i < ROWS; i++)
	{
		validMovesArr[i] = (chessPosArray**)malloc(COLS * sizeof(chessPosArray*));
		checkAllocation(validMovesArr[i], "validMovesArr[i] in validKnightMoves.");

		for (j = 0; j < COLS; j++)
		{
			validMovesArr[i][j] = (chessPosArray*)malloc(sizeof(chessPosArray));
			checkAllocation(validMovesArr[i][j], "validMovesArr[i][j] in validKnightMoves.");

			validMovesArr[i][j]->positions = (chessPos*)malloc(maxPossiblePosSize * sizeof(chessPos));
			checkAllocation(validMovesArr[i][j]->positions, "positions in validKnightMoves.");

			/* get valid positions array that the knight can move to from this position (i, j) */
			getPositions(validMovesArr[i][j], i, j);
		}
	}
	return validMovesArr;
}

/* --------------------- twoUpDownOneRightLeft ---------------------
* Get: cPos, row, col ,rightOrLeft, upOrDown
*
* Checks if 2 steps down or up and 1 step right or left is a
* possible move, and if so updates cPos arr to the possible position
------------------------------------------------------------------*/
void twoUpDownOneRightLeft(chessPos cPos, int row, int col, int upOrDown, int rightOrLeft)
{
	cPos[0] = cPos[1] = 'x';
	if (row + 2 * upOrDown >= 0 && row + 2 * upOrDown < ROWS)
		if (col + rightOrLeft >= 0 && col + rightOrLeft < COLS)
		{
			cPos[0] = (char)(row + 2 * upOrDown + 'A');
			cPos[1] = (char)(col + rightOrLeft + '0');
		}
}

/* --------------------- twoRightLeftOneUpDown ---------------------
* Get: cPos, row, col ,rightOrLeft, upOrDown
*
* Checks if 2 steps right or left and 1 step up or down is a
* possible move, and if so updates cPos arr to the possible position
------------------------------------------------------------------*/
void twoRightLeftOneUpDown(chessPos cPos, int row, int col, int rightOrLeft, int upOrDown)
{
	cPos[0] = cPos[1] = 'x';
	if (row + upOrDown >= 0 && row + upOrDown < ROWS)
		if (col + 2 * rightOrLeft >= 0 && col + 2 * rightOrLeft < COLS)
		{
			cPos[0] = (char)(row + upOrDown + 'A');
			cPos[1] = (char)(col + 2 * rightOrLeft + '0');
		}
}

/* ------------ copyArr -------------
* Get: dest, src
*
* function copies all the values from
* source array to destination array
-----------------------------------*/
void copyArr(chessPos dest, chessPos src)
{
	dest[0] = src[0];
	dest[1] = src[1];
}

/* -------- printArray ---------
* Get: * arr, size
*
* Prints arr cells as (row, col)
------------------------------*/
void printArray(chessPos* arr, unsigned int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf("position %d: (%c,%c)\n", i, arr[i][0], (arr[i][1] + 1));
}

/* -- freeValidMovesArr ---
* Get: *** validMovesArr
*
* Free all positions in arr
-------------------------*/
void freeValidMovesArr(chessPosArray*** validMovesArr)
{
	int i, j;
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLS; j++)
		{
			free(validMovesArr[i][j]->positions);
			free(validMovesArr[i][j]);
		}
		free(validMovesArr[i]);
	}
	free(validMovesArr);
}