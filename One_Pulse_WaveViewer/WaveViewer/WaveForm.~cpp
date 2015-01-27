//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "WaveForm.h"
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfWave *fWave;
//---------------------------------------------------------------------------
__fastcall TfWave::TfWave(TComponent* Owner)
        : TForm(Owner)
{
 offbmp = new Graphics::TBitmap;
 iqs.clear();
 iWave->Canvas->Brush->Color = clWhite;
 clicked = false;
}
//---------------------------------------------------------------------------
void __fastcall TfWave::Open1Click(TObject *Sender)
{
 if (odWave->Execute())
 {
  p2c("Opening - " + odWave->FileName);
  if (LoadFile(odWave->FileName.c_str()) == 0)
  {
   //lbIQ->Clear();
   gbData->Enabled = true;
  }
 }
}
//---------------------------------------------------------------------------
int __fastcall TfWave::LoadFile(char * filename)
{
   ifstream infile(filename);

   if (infile)
   {
    if(iqs.size() > 0)
     ClearMem();

    while (!infile.eof())
    {
     char line[50];
     int * iq = new int[2];
     infile.getline(line,50,'\n');
     if (strlen(line) >= 3)
     {
      ParseLine(line,iq[0],iq[1]);
      iqs.push_back(iq);
     }
    }
    p2c(iqs.size());

    p2c("Closing file! - All's OK");
    infile.close();
    return 0;
   }
   p2c("Closing file! - Could not open");
   infile.close();
   return -1;
}
void __fastcall TfWave::p2c(AnsiString s)
{
 mCon->Lines->Add(s);
}

void __fastcall TfWave::ClearMem()
{
 p2c("Deleting buffers!");
 if (iqs.size() > 0)
 {
  for (unsigned int i=0;i<iqs.size();i++)
   delete [] iqs[i];
  iqs.clear();
 }
}

void __fastcall TfWave::ParseLine(char * line, int &iVal, int &qVal)
{
 //p2c("start");
 int spaceplace=0;
 spaceplace = strchr(line,' ')- line;
 if (spaceplace > 0)
 {
  char iStr[10], qStr[10];
  //p2c(line);

  strncpy(iStr,line,spaceplace);
  iStr[spaceplace] = '\0';
  strncpy(qStr,line+(spaceplace+1),strlen(line)-spaceplace);

  iVal = atoi(iStr);
  qVal = atoi(qStr);
 }
 else
 {
  AnsiString l(line);
  p2c("parsing line failed! " + l);
  iVal=0;
  qVal=0;
 }

 //p2c("end");
}

void __fastcall TfWave::DrawWave(char type, int sampPerPix,int startsmp,int samples)
{
 offbmp->Height = 300;
 offbmp->Width = samples;
 offbmp->Canvas->Pen->Color = clWhite;
 offbmp->Canvas->Rectangle(0,0,offbmp->Width,offbmp->Height);
 offbmp->Canvas->Pen->Color = clRed;
 switch (type)
 {
  case 'r' :
  {
   p2c("Plotting Real Waveform");
   offbmp->Canvas->MoveTo(0,(offbmp->Height)/2 - iqs[startsmp][0]);
   //for (int i=1;i<iqs.size();i++)
   for (int i=startsmp+1;i<samples+startsmp;i++)
   {
    offbmp->Canvas->LineTo(i,(offbmp->Height)/2 - iqs[i*sampPerPix][0]);
   }
  }
  break;
  case 'i' :
  {
   p2c("Plotting Imaginary Waveform");
   offbmp->Canvas->MoveTo(0,(offbmp->Height)/2 - iqs[startsmp][1]);
   //for (int i=1;i<iqs.size();i++)
   for (int i=startsmp+1;i<samples+startsmp;i++)
   {
    offbmp->Canvas->LineTo(i,(offbmp->Height)/2 - iqs[i*sampPerPix][1]);
   }
  }
  break;
  case 'm' :
  {
   p2c("Plotting Magnitude Waveform");
   int mag = sqrt(iqs[startsmp][0]*iqs[startsmp][0] + iqs[startsmp][1]*iqs[startsmp][1]);
   offbmp->Canvas->MoveTo(0,(offbmp->Height)/2 - mag);
   //for (int i=1;i<iqs.size();i++)
   for (int i=startsmp+1;i<samples+startsmp;i++)
   {
    mag = sqrt(iqs[i*sampPerPix][0]*iqs[i*sampPerPix][0] + iqs[i*sampPerPix][1]*iqs[i*sampPerPix][1]);
    offbmp->Canvas->LineTo(i,(offbmp->Height)/2 - mag);
   }
  }
  break;
  case 'p' :
  {
   p2c("Plotting Phase Waveform");
  }
  break;
 }
}

void __fastcall TfWave::FormResize(TObject *Sender)
{
 //iWave->Height = ClientHeight;
 //iWave->Width = ClientWidth;
}
//---------------------------------------------------------------------------


void __fastcall TfWave::Exit1Click(TObject *Sender)
{
 fWave->Close();
}
//---------------------------------------------------------------------------

void __fastcall TfWave::FormClose(TObject *Sender, TCloseAction &Action)
{
 delete offbmp;
 ClearMem();
 return;
}
//---------------------------------------------------------------------------


void __fastcall TfWave::rbRealClick(TObject *Sender)
{
 //lbIQ->Clear();
 DrawWave('r',iqs.size()/iWave->Width,0,iWave->Width);
 iWave->Picture->Bitmap = offbmp;
 /*for (int i=0;i<iqs.size();i++)
 {
  lbIQ->AddItem(iqs[i][0],NULL);
  //p2c(iqs[i][0]);
 }      */
}
//---------------------------------------------------------------------------

void __fastcall TfWave::rbImagClick(TObject *Sender)
{
 //lbIQ->Clear();
 DrawWave('i',iqs.size()/iWave->Width,0,iWave->Width);
 iWave->Picture->Bitmap = offbmp;
/* for (int i=0;i<iqs.size();i++)
 {
  lbIQ->AddItem(iqs[i][1],NULL);
  //p2c(iqs[i][1]);
 }  */
}
//---------------------------------------------------------------------------

void __fastcall TfWave::rbMagClick(TObject *Sender)
{
 //lbIQ->Clear();
 DrawWave('m',iqs.size()/iWave->Width,0,iWave->Width);
 iWave->Picture->Bitmap = offbmp;
/* for (int i=0;i<iqs.size();i++)
 {
  double mag = sqrt(iqs[i][0]*iqs[i][0] + iqs[i][1]*iqs[i][1]);
  lbIQ->AddItem(mag,NULL);
  //p2c(mag);
 }    */
}
//---------------------------------------------------------------------------

void __fastcall TfWave::rbPhaseClick(TObject *Sender)
{
 //lbIQ->Clear();
 //DrawWave('p');
 /*for (int i=0;i<iqs.size();i++)
 {
  double phase = atan(iqs[i][1] / iqs[i][0]);
  lbIQ->AddItem(phase,NULL);
  //p2c(phase);
 }   */
}
//---------------------------------------------------------------------------

void __fastcall TfWave::bSaveWaveClick(TObject *Sender)
{
 Graphics::TBitmap *old = new Graphics::TBitmap;
 old->Assign(offbmp);
 if (rbReal->Checked)
  DrawWave('r',1,0,iqs.size());
 else if (rbImag->Checked)
  DrawWave('i',1,0,iqs.size());
 else if (rbMag->Checked)
  DrawWave('m',1,0,iqs.size());
 else if (rbPhase->Checked)
  DrawWave('p',1,0,iqs.size());


 //iWave->Picture->SaveToFile("test.bmp");
 TJPEGImage *temp = new TJPEGImage();

 temp->Assign(offbmp);
 temp->CompressionQuality = 100;
 temp->SaveToFile(eImgFilename->Text);

 delete temp;
 offbmp->Assign(old);
 delete old;
}
//---------------------------------------------------------------------------

void __fastcall TfWave::iWaveMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
 AnsiString pos;
 pos.cat_sprintf("Current (x,y) = (%d,%d)",X,Y);
 lPos->Caption = pos;
 Graphics::TBitmap *temp = new Graphics::TBitmap();

 temp->Width = iWave->Width;
 temp->Height = iWave->Height;
 int myy = iWave->Height/2;
 if (rbImag->Checked || rbReal->Checked || rbMag->Checked)
 {
  temp->Assign(offbmp);
  if (rbReal->Checked)
   myy = iWave->Height/2 - iqs[X*(iqs.size()/iWave->Width)][0];
  else if (rbImag->Checked)
   myy = iWave->Height/2 - iqs[X*(iqs.size()/iWave->Width)][1];
  else if (rbMag->Checked)
   myy = iWave->Height/2 - sqrt(iqs[X*(iqs.size()/iWave->Width)][0]*iqs[X*(iqs.size()/iWave->Width)][0] + iqs[X*(iqs.size()/iWave->Width)][1]*iqs[X*(iqs.size()/iWave->Width)][1]);
 }


 temp->Canvas->Ellipse(X-3,myy-3,X+3,myy+3);

 if (clicked)
 {
  temp->Canvas->DrawFocusRect(Rect(clickLoc,Point(X,Y)));
 }

 iWave->Picture->Bitmap = temp;
 delete temp;
}
//---------------------------------------------------------------------------

void __fastcall TfWave::iWaveMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
       /* clicked = true;
        clickLoc.x = X;
        clickLoc.y = Y;*/
}
//---------------------------------------------------------------------------

void __fastcall TfWave::iWaveMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 /*clicked = false;
 if (rbReal->Checked)
  DrawWave('r',((X-clickLoc.x)*(iqs.size()/iWave->Width))/iWave->Width,clickLoc.x,(X-clickLoc.x)*(iqs.size()/iWave->Width));
 else if (rbImag->Checked)
  DrawWave('i',1,clickLoc.x,iqs.size());
 else if (rbMag->Checked)
  DrawWave('m',1,clickLoc.x,iqs.size());
 else if (rbPhase->Checked)
  DrawWave('p',1,clickLoc.x,iqs.size());
                            */
 //iWave->Picture->Bitmap = offbmp;
}
//---------------------------------------------------------------------------

