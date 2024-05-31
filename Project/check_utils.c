#include <stdio.h>
#include <stdlib.h>
#include "check_utils.h"

#define MEMORY_ALLOCATION_ERROR -9
#define FILE_OPENING_ERROR -7
#define ERROR_IN_USAGE -20

/* ---------- checkAllocation ----------
* Get: *ptr, *msg
*
* Check if allocation failed, and if so,
* prints a message and exits the program
--------------------------------------*/
void checkAllocation(void* ptr, char* msg)
{
	if (!ptr)  /* the same as (ptr == NULL) */
	{
		fprintf(stderr, "Allocation failure: %s\n", msg);
		exit(MEMORY_ALLOCATION_ERROR);
	}
}

/* ----------- checkFileOpen -----------
* Get: *fp, *msg
*
* Check if file open failed, and if so,
* prints a message and exits the program
--------------------------------------*/
void checkFileOpen(FILE* fp, char* msg)
{
	if (!fp)
	{
		fprintf(stderr, "Error opening file: %s\n", msg);
		exit(FILE_OPENING_ERROR);
	}
}
/* ----------------- checkCommandLineArgs -----------------
* Get: argc
*
* Check if the arguments sent to the program are less
* then 3, and if so, prints a message and exits the program
---------------------------------------------------------*/
void checkCommandLineArgs(int argc)
{
	if (argc < 3) /* Wrong number of args */
	{
		fprintf(stderr, "Usage: progname <filename1> <filename2>\n");
		exit(ERROR_IN_USAGE);
	}
}