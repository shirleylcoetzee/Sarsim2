//---------------------------------------------------------------------------
#ifndef loadsavedataH
#define loadsavedataH
#include "hdf5.h"
#include "objectstructdef.h"
//---------------------------------------------------------------------------
bool LoadDataFile(char FileName[], struct SDataDefinition *DataDef);

//void OpenFile(char *FileName);
#ifdef COMPILE_FOR_WINDOWS32
void SaveScriptFile(char *FileName);
#endif

int AddAttribute(hid_t file_id, char *path, char *attrname, char *attrvalue);
int CreateDataset(hid_t file_id, hid_t group_id, char *path);

int OpenScriptFile(char *FileName, struct LinkedLists *FirstObjectP);

//void WriteDataDef(FILE *OutFilep, struct SDataDefinition *DataDef);

bool WriteHDF5File(double * pData, int nx, int ny, const char * pszHDF5Filename);

#endif




