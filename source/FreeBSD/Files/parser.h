#include "dest.h"
//---------------------------------------------------------------------------
#ifndef parserH
#define parserH

#include <stdio.h>

//---------------------------------------------------------------------------
// ParseOneItem() function constants
#define _ITEM_NONE          0
#define _ITEM_EOF           1
#define _ITEM_EOL           2
#define _ITEM_FUNCTION      4
#define _ITEM_PARAMETER     5

// Error meassages
#define ERROR_UNKNOWN_FUNC  0
#define ERROR_TOO_MANY_PARM 1
#define ERROR_TOO_LONG_PARM 2
#define ERROR_ILLEGAL_NUMBER_PARA_TARGET 3
#define ERROR_ILLEGAL_NUMBER_PARA_ARRAY 4
#define ERROR_TOO_MANY_NUMBER_PARA_ARRAY 5
#define ERROR_ILLEGAL_NUMBER_PARA_RADAR 6
#define ERROR_ILLEGAL_NUMBER_PARA_PLATFORM 7
#define ERROR_ARRAY_NOT_FOUND_PLATFORM 8
#define ERROR_ILLEGAL_NUMBER_PARA_SIM 9
#define ERROR_PLATFORM_NOT_FOUND 10
#define ERROR_RADAR_NOT_FOUND 11
#define E_NOT_ENOUGH_PARA 20
#define E_ILLEGAL_BRANCH 21
#define E_NO_LINE_FILE 22
#define E_TOO_MANY_POINTS 23
#define E_MORE_POINTS_EXP 24
#define E_INTERPOLATION_EXP 25
#define E_OUT_OF_LIMITS 26

// Maximum length of each parameter
#define MAX_LENGTH_OF_PARA_STRING 64

int ParseOneItem(FILE *ScriptFile, char *DataString, int LineCount);
void ErrorM(const unsigned int Error,int LineCount, long Multi, char* String);
int ExecuteCommand(int LastFunction,
				    char ParameterList[][MAX_LENGTH_OF_PARA_STRING],
				    int ParameterCount, int LineCount, struct LinkedLists *FirstObjectP);
bool RunStateMachine(struct SStateM *StateM,
						 char ParameterList[][MAX_LENGTH_OF_PARA_STRING],
						 int ParameterCount, int LineCount);


#endif
