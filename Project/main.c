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

void main()
{
	chessGame(); /* start game */
	/* do after the game ends: */
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n                                    Bye bye!\n\n\n\n\n\n\n\n\n\n\n\n");
	system("pause");
	system("cls");
}

/* ---- chessGame ----
* The function runs the chess game.
* Prints the game's manu and activates the choosen option.
*/
void chessGame()
{
	char userChoice = 0;
	bool chPosHasValue = false;
	chessPos chPos;
	chessPosList* list = NULL;
	pathTree tree;
	tree.root = NULL;

	while (userChoice != '6') /* as long as user does not want to exit game */
	{
		userChoice = displayMenu(); /* prints the menu and get the user's choice */

		switch (userChoice)
		{
		case '1': /* user choose:  "Enter a knight's starting position" */
			getStartPosition(&chPos, &chPosHasValue);
			break;
		case '2': /* user choose: "Create all possible knight paths" */
			buildTree(&tree, &chPos, chPosHasValue);
			break;
		case '3': /* user choose: "Find a knight path covering all board" */
			findPathCoveringAllBoard(&list, &tree, &chPos, chPosHasValue);
			break;
		case '4': /* user choose: "Save knight path covering all board to file" */
			createFlieWithPathCoveringAllBoardPositions(&list, &tree, &chPos, chPosHasValue);
			break;
		case '5': /* user choose: "Load and display path from file" */
			checkPathFromFile();
			break;
		case '6': /* user choose: "Exit" */
			break;
		default: /* user entered invalid input */
			printf("\n\nThe char you entered is not valid.\nPlease enter a number between 1-6\n\n\n\n\n\n\n");
			system("pause");
			break;
		}
		system("cls");
	}

	printf("\n\n\n\n\n\n\n\n\n\n\n\n                           Freeing all allocations...\n\n\n\n\n\n\n\n\n\n\n\n");

	if (list) /* if the list was allocate, free all list */
		freeList(list);
	if (tree.root) /* if the tree's root was allocate, free all tree */
		freeTree(&tree);
}

/* ------- displayMenu --------
* Return: userChoice
* 
* Prints the game's manu
* and returns the user's choice
-----------------------------*/
char displayMenu()
{
	char userChoice;
	printf("             _____________________________________________________\n");
	printf("            |                                                     |\n");
	printf("            |              > WELCOME TO CHESS GAME <              |\n");
	printf("            |                                                     |\n");
	printf("            | Please choose one of the options:                   |\n");
	printf("            | (1) Enter a knight's starting position              |\n");
	printf("            | (2) Create all possible knight paths                |\n");
	printf("            | (3) Find a knight path covering all board           |\n");
	printf("            | (4) Save knight path covering all board to file     |\n");
	printf("            | (5) Load and display path from file                 |\n");
	printf("            | (6) Exit                                            |\n");
	printf("            |_____________________________________________________|\n");
	printf("\n                             Enter your choice: ");
	scanf(" %c", &userChoice);
	while (getchar() != '\n'); /* ignores every char entered after the first char (except '\n') */
	return userChoice;
}

/* ----------------- getStartPosition -----------------
* Get: *chPos, *chPosHasValue
* 
* Get a position from user, and save it in chPos.
* If position entered is not valid, displays a message.
-----------------------------------------------------*/
void getStartPosition(chessPos* chPos, bool* chPosHasValue)
{
	char row, col;

	printf("\n\nPlease enter <row> between 'A'-'H': ");
	scanf(" %c", &row);
	while (getchar() != '\n'); /* ignores every char entered after the first char (except '\n') */
	printf("Please enter <col> between '1'-'8': ");
	scanf(" %c", &col);
	while (getchar() != '\n'); /* ignores every char entered after the first char (except '\n') */
	
	if ((isLetter(row)) && (isDigit(col))) /* if position valid */
	{
		*chPosHasValue = true; /* mark that position has value */
		(*chPos)[0] = row;
		(*chPos)[1] = col;
	}
	else
	{
		printf("\n\nPosition entered is not valid.\nYou need to enter <row> between 'A'-'H' and <col> between '1'-'8'\n\n\n");
		system("pause");
	}
}

/* -------------------------- buildTree ---------------------------
* Get: *tree, *chPos, chPosHasValue
* 
* Builds a tree with root position of chPos and returns it in tree.
* If no starting position was entered, displays a message.
-----------------------------------------------------------------*/
void buildTree(pathTree* tree, chessPos* chPos, bool chPosHasValue)
{
	if (!chPosHasValue) /* if no starting position was entered */
	{
		printf("\n\nNo starting position was entered.\nPlease enter a position before choosing this option.\n\n\n\n\n\n\n");
		system("pause");
	}
	else
	{
		if (tree->root == NULL) /* if no other tree was build, build a tree */
			*tree = findAllPossibleKnightPaths(chPos);
		else if ((tree->root) && ((tree->root->position[0] != (*chPos)[0]) || (tree->root->position[1] != (*chPos)[1])))
		{
			/* if position has changed, free the current tree and build a new one with the current position */
			freeTree(tree);
			*tree = findAllPossibleKnightPaths(chPos);
		}
	}
}

/* ----------------------------------- findPathCoveringAllBoard -----------------------------------
* Get: **list, *tree, *chPos, chPosHasValue
*
* If there is a starting position, calls findknightPathCoveringAllBoard, else - displays a message.
* If a path covering all board was not found, displays a message.
-------------------------------------------------------------------------------------------------*/
void findPathCoveringAllBoard(chessPosList** list, pathTree* tree, chessPos* chPos, bool chPosHasValue)
{
	/* if position has changed, creates a new tree calling function buildTree */
	if ((tree->root == NULL) || (tree->root->position[0] != (*chPos)[0]) || (tree->root->position[1] != (*chPos)[1]))
		buildTree(tree, chPos, chPosHasValue);
	if (tree->root)
	{
		if (*list == NULL) /* if no other list was build, build a list */
			*list = findknightPathCoveringAllBoard(tree);
		else if (*list && (((*list)->head->position[0] != (*chPos)[0]) || ((*list)->head->position[1] != (*chPos)[1])))
		{
			/* if position has changed, free the current list and build a new one with the current position */
			freeList(*list);
			*list = findknightPathCoveringAllBoard(tree);
		}

		if (*list == NULL)
		{
			printf("\n\nNo path covering all board from the starting position was found.\n\n\n\n\n\n\n\n");
			system("pause");
		}
	}
}

/* ---------------- createFlieWithPathCoveringAllBoardPositions ----------------
* Get: **list, *tree, *chPos, chPosHasValue
*
* Receives a file name from user and checks if file name is valid.
* If file name is not valid, displays an error message.
* Else, save to the file a path covering all board from starting position chPos.
------------------------------------------------------------------------------*/
void createFlieWithPathCoveringAllBoardPositions(chessPosList** list, pathTree* tree, chessPos* chPos, bool chPosHasValue)
{
	char* fileName;
	/* if position has changed, creates a new list calling function findPathCoveringAllBoard */
	if ((*list == NULL) || ((*list)->head->position[0] != (*chPos)[0]) || ((*list)->head->position[1] != (*chPos)[1]))
		findPathCoveringAllBoard(list, tree, chPos, chPosHasValue);

	if (*list)
	{
		printf("\n\nPlease enter a file name with '.bin' ending.\n\nFile name: ");
		fileName = getString();

		if (!checkFileNameValidtion(fileName)) /* check if file name is valid */
		{
			printf("\n\nFile name is not valid.\n\n\n");
			system("pause");
		}
		else
			saveListToBinFile(fileName, *list);
		free(fileName);
	}
}

/* --------------------- checkPathFromFile -----------------------
* Receives a file name from user and checks if file name is valid.
* If file name is not valid, displays an error message.
* Else, calling function checkAndDisplayPathFromFile and
* displays a message according to the content of the file.
----------------------------------------------------------------*/
void checkPathFromFile()
{
	char* fileName;
	printf("\n\nPlease enter a file name with '.bin' ending.\n\nFile name: ");
	fileName = getString();

	if (!checkFileNameValidtion(fileName)) /* check if file name is valid */
	{
		printf("\n\nFile name is not valid.\n\n\n");
		system("pause");
	}
	else
	{
		switch (checkAndDisplayPathFromFile(fileName))
		{
		case FILE_NOT_EXIST:
			printf("\n\nfile does not exist.\n\n\n");
			break;
		case PATH_NOT_LEGAL:
			printf("\n\nilegal path.\n\n\n");
			break;
		case PATH_COVER_ALL_BOARD:
			printf("\ncover all board.\n");
			break;
		case PATH_NOT_COVER_ALL_BOARD:
			printf("\nnot cover all board.\n");
			break;
		default:
			printf("\nsomething went wrong...\n");
			break;
		}
		system("pause");
	}
	free(fileName);
}

/* ----------- getString -----------
* Return: *str
*
* Creates a string of unknown length
* as an input and returns it
----------------------------------*/
char* getString()
{
	int physical_size = 1, logical_size = 0;
	char* str, currChar;

	str = (char*)malloc(physical_size * sizeof(char));
	checkAllocation(str, "str - first allocation in getString.");

	currChar = getchar();
	while ((currChar != '\n') && (currChar != EOF))
	{
		if (logical_size == physical_size) /* resize str if needed */
		{
			physical_size = physical_size * 2 + 1;
			str = (char*)realloc(str, physical_size * sizeof(char));
			checkAllocation(str, "str - resizing array in getString");
		}
		str[logical_size] = currChar;
		logical_size++;
		currChar = getchar();
	}
	/* resize str to the final size of the string */
	str = (char*)realloc(str, (logical_size + 1) * sizeof(char));
	str[logical_size] = '\0';
	return str;
}


/* ----- checkFileNameValidtion -----
* Get: *fileName
* Return: true / false
*
* Returns true if file name is valid,
* else returns false
-----------------------------------*/
bool checkFileNameValidtion(char* fileName)
{
	int len = strlen(fileName);
	if (len < 5) /* File name must be at least 5 characters (at least one character and .bin ending) */
		return false;
	if (isDigit(fileName[0])) /* File name cant start with a digit */
		return false;
	if (strcmp((fileName + len - 4), ".bin")) /* File name must include .bin ending */
		return false;
	return true;
}