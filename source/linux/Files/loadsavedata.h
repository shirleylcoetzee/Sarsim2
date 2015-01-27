#include "dest.h"
//---------------------------------------------------------------------------
#ifndef loadsavedataH
#define loadsavedataH
#include "objectstructdef.h"
//---------------------------------------------------------------------------
bool LoadDataFile(char FileName[], struct SDataDefinition *DataDef);

//void OpenFile(char *FileName);
#ifdef COMPILE_FOR_WINDOWS32
void SaveScriptFile(char *FileName);
#endif
void OpenScriptFile(char *FileName, struct LinkedLists *FirstObjectP);

//void WriteDataDef(FILE *OutFilep, struct SDataDefinition *DataDef);

#endif
