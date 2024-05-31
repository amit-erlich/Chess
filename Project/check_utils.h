#ifndef ___CHECK_UTILS_H___
#define ___CHECK_UTILS_H___

#include <stdio.h> /* for the FILE typedef */

/* --------------------------- Function Declaration --------------------------- */
void checkAllocation( void *ptr, char *msg);
void checkFileOpen( FILE *fp, char* msg);
void checkCommandLineArgs(int argc);

#endif