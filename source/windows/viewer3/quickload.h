//---------------------------------------------------------------------------
#ifndef quickloadH
#define quickloadH
//---------------------------------------------------------------------------
#include "io.h"
#include "loaddata.h"
#include "objectstructdef.h"
#define BUFFERSIZE 8192  //2048
class TQuickRead {
  private :
    // part of file is hold in memory
    unsigned char Buffer[BUFFERSIZE];
    // offset where buffer starts (relative to file begin)
    unsigned long BufferOffset;
    // pointer to file to be opened
    FILE *InFilep;
    // file format e.g. Width, Height etc..
    struct SFileFormat FFormat;
  public :
    TQuickRead(struct SFileFormat FF);
    ~TQuickRead();
    // what to return, i.e. mag phase etc
    bool OpenFile(AnsiString FileName);
    void __fastcall ReadValue(unsigned int X, unsigned int Y,
      struct ValueIQMag *Value);
//    void __fastcall ReadValue(unsigned int X, unsigned int Y,
//       double *I, double *Q, double *Mag, double *Phase);
    bool CloseFile();
};

#endif
