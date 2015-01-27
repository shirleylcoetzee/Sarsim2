//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "quickload.h"
#include "math.h"
#include "quikgeneral.h"
#include "objectstructdef.h"

//---------------------------------------------------------------------------
TQuickRead::TQuickRead(struct SFileFormat FF)
{
 // Buffer = new unsigned char[BUFFERSIZE];
  BufferOffset  = 0;
  Buffer[0] = NULL;
  // copy file format across
  memcpy(&FFormat, &FF, sizeof(struct SFileFormat));
}
//---------------------------------------------------------------------------
TQuickRead::~TQuickRead()
{
}
//---------------------------------------------------------------------------
bool TQuickRead::OpenFile(AnsiString FileName)
{
  InFilep = fopen(FileName.c_str(), "rb");  // read from file, binary
  if (!InFilep) return FALSE;
  else
    {
      BufferOffset = 0;
      fseek(InFilep,0, SEEK_SET);
      fread(Buffer, 1, BUFFERSIZE, InFilep);
      return TRUE;
    }
}
/*
    AnsiString FileName;
    int FileFormat;   // 0 = SunRasterFile, 1 = Sarsim Sim, 2 = custom
    AnsiString ScriptFile;
    int FileType;    // 0 = Text, 1 = Binary, 2 = IEEE
    unsigned int BytesPerPixel;
    double Offset;
    int Endian;
    int DataType;
    unsigned long HeaderSize;
    unsigned int SizeX;
    unsigned int SizeY;
    double MinValueI, MaxValueI,
           MinValueQ, MaxValueQ,
           MinValueMag, MaxValueMag,
           MinValuePh, MaxValuePh;

struct Value
//---------------------------------------------------------------------------
void __fastcall TQuickRead::ReadValue(unsigned int X, unsigned int Y,
  double *I, double *Q, double *Mag, double *Phase)
{


  unsigned long Pos = FFormat.HeaderSize +
    (FFormat.SizeX * Y * FFormat.BytesPerPixel) + X * FFormat.BytesPerPixel;
  //(AziP * TotalPointsX + SlantP)* 2
  // relative position
  register unsigned long RelPos = Pos - BufferOffset;
  // are all necessary bytes still in buffer ?
  if ((RelPos < 0) || (RelPos > (BUFFERSIZE - FFormat.BytesPerPixel)))
    {
      fseek(InFilep, Pos, SEEK_SET);     // read in new chunk
      fread(Buffer, 1, BUFFERSIZE, InFilep);
      BufferOffset = Pos;
      RelPos = 0;
    }
  if (FFormat.DataType == 0) // magnitude only
    {
      *Mag = Buffer[RelPos] - FFormat.Offset;
      return;
    }
  else if (FFormat.DataType == 1) // complex
    {
      *I = Buffer[RelPos] - FFormat.Offset;
      *Q = Buffer[RelPos+1] - FFormat.Offset;
      *Mag = sqrt(*I * *I + *Q * *Q);
   	  if ((*I != 0) || (*Q != 0))
        *Phase = atan2(*Q,*I);
	  else *Phase = 0;
      if (*Phase < PI) *Phase += (2*PI);
      if (*Phase > PI) *Phase -= (2*PI);
    }
}
*/
//---------------------------------------------------------------------------
void __fastcall TQuickRead::ReadValue(unsigned int X, unsigned int Y,
  struct ValueIQMag *Value)
{
  unsigned long Pos;
  register unsigned long RelPos;
  if (FFormat.FileFormat == 0)  // Sun Raster File
    {


    }
  else if (FFormat.FileFormat == 1)   // Sarsim Simulation
    {


    }
  else   // Custom
    {
      if (FFormat.FileType == 0) // text
        {
        }
      else if (FFormat.FileType == 0)  // binary
        {
          Pos = FFormat.HeaderSize +
            (FFormat.SizeX * Y + X) * FFormat.BytesPerPixel;
          // relative position
          RelPos = Pos - BufferOffset;
          // are all necessary bytes still in buffer ?
          if ((RelPos < 0) || (RelPos > (BUFFERSIZE - FFormat.BytesPerPixel)))
            {
              fseek(InFilep, Pos, SEEK_SET);     // read in new chunk
              fread(Buffer, 1, BUFFERSIZE, InFilep);
              BufferOffset = Pos;
              RelPos = 0;
            }

          if (FFormat.DataType == 0) // complex
            {
              Value->I = Buffer[RelPos] - FFormat.Offset;
              Value->Q = Buffer[RelPos+1] - FFormat.Offset;
            }
          else if (FFormat.DataType == 1) // magnitude only
            {
              Value->Mag = Buffer[RelPos] - FFormat.Offset;
              return;
            }
          else  // power only
            {
              Value->Mag = sqrt(fabs(Buffer[RelPos] - FFormat.Offset));
              return;
            }
        }
      else  // IEEE float
        {


        }
    } // end else custom
}
//---------------------------------------------------------------------------
bool TQuickRead::CloseFile()
{
  fclose(InFilep);
  return TRUE;
}
