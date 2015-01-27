//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "WaitForThread.h"
#include "SaveProgress.h"
#include "SaveSimData.h"
extern int CalcProgress2;
extern bool UserAbort;
//---------------------------------------------------------------------------
__fastcall TWaitThread::TWaitThread(void) : TThread(TRUE)
{
}
//---------------------------------------------------------------------------
void __fastcall TWaitThread::DoVisualUpdate()
{
  SaveProgressForm->ProgressBar1->Position = (unsigned char)CalcProgress2;
}
//---------------------------------------------------------------------------
void __fastcall TWaitThread::Execute()
{
  UserAbort = FALSE;
  SaveSimForm->SaveData(&MaxMagnitude,&TotalSampleNo,&ClippedSamples,
   &ZeroSamples);
}
//---------------------------------------------------------------------------
void TWaitThread::UpdateProgress()
{
  Synchronize(&DoVisualUpdate);
}

