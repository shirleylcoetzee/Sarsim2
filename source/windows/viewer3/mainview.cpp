//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "mainview.h"
#include "loaddata.h"
#include "general.h"
#include "math.h"
#include "quickload.h"
#include "selectpal.h"

// minimum change of range values such that an update occurs
#define MIN_CHANGE 0.01


//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMainViewForm *MainViewForm;
//---------------------------------------------------------------------------
__fastcall TMainViewForm::TMainViewForm(TComponent* Owner)
  : TForm(Owner)
{
  Timer1->Enabled = false;
  // nothing initialized yet
  InitVar = false;
  // show side graphs
  SideGraphs = true;
  // width and height of side-graphs
  WidthLeftGraph = 100;  // must be divisible by 4
  HeightBottomGraph = 100; // must be divisible by 4
  FileLoaded = FALSE;

  GD.Display = 0;    // display real part
  GD.XMin = 0;
  GD.XMax = 10000;
  GD.YMin = 0;
  GD.YMax = 10000;

  // create palette (Cpal)
  ColorPal = RED_BLUE_SCALE;
  MakePalette(ColorPal,127,127,127);
  int temp;
  temp = (Bevel2->Width - 4)& ~3;
  PalBM = new class TBitM(Handle, temp, Bevel2->Height - 3);    // 200 18
  ThumbBM = new class TBitM(Handle, 200, 200);
  GraphBM = new class TBitM(Handle, 4, 4);

}
//---------------------------------------------------------------------------
void TMainViewForm::SetupGraphSize()
{
  // left and right MUST be a multiple of 4
  GD.Border.Left = Panel1->Width + 44;  // = 204+44=248 /4= divisible by 4
  GD.Border.Right = BAUp->Width+14;     // = 22+14=36 /4=9 OK
  GD.Border.Top = Panel2->Height + BRRight->Height + 18;
  GD.Border.Bottom = 26;

  GD.MaxWindowSize.Left = 572;
  GD.MaxWindowSize.Top = 438;

  if (SideGraphs)
    {
      GD.Border.Left += WidthLeftGraph + 12;
      GD.Border.Bottom += HeightBottomGraph + 12;
      GD.MaxWindowSize.Left += WidthLeftGraph;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::RestrictSize(TMessage& Msg)
{
  // set the limits of the graph windows size
  ((POINT far *)Msg.LParam)[3].x = GD.MaxWindowSize.Left;
  ((POINT far *)Msg.LParam)[3].y = GD.MaxWindowSize.Top;
  TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::Open1Click(TObject *Sender)
{
  if (FormLoadSimFile->ShowModal() == mrOk)
    {
      // copy file format parameters back to local FF structure
      CopyFileFormat(&FF,&(FormLoadSimFile->FileFormat));
      double XStart, XEnd, YStart, YEnd;
      XStart = 0; XEnd = FF.SizeX -1;
      YStart = 0; YEnd = FF.SizeY -1;
      GD.XMin = 0;
      GD.XMax = FF.SizeX -1;
      GD.YMin = 0;
      GD.YMax = FF.SizeY -1;

      SetNewGraphRange(&(XStart),&(XEnd),&(YStart),&(YEnd));
      UpdateRange();

      UpdateDialogsGivenFileFormat();
      FileLoaded = TRUE;
      UpdateContents = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormShow(TObject *Sender)
{
  InitVar = false; // all initialized

  SetupGraphSize(); // setup position and size of graph window
  // setup graph window
  GD.XStart = 0;
  GD.XEnd = 10000;
  GD.YStart = 0;
  GD.YEnd = 10000;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),&(GD.YStart),&(GD.YEnd));

  // create buffer for values
  Data1 = DMatrix(0,0,0,0);
  Data2 = DMatrix(0,0,0,0);
  FileLoaded = FALSE;
  SetNewGraphSize();

  SetupPaletteBitmap();
  InitVar = true; // all initialized
  UpdateRange();
  Timer1->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormResize(TObject *Sender)
{
//  Busy = true;
  // only allow widths a multiple of 4 such that bitmap width will be a multiple of 4
  ClientWidth = (ClientWidth + 3) & ~3;

  // setup new button positions etc.
  int CenterX = GD.Border.Left + (ClientWidth - GD.Border.Right -
        GD.Border.Left)*0.5,
      CenterY = GD.Border.Top + (ClientHeight - GD.Border.Bottom -
        GD.Border.Top)*0.5;

  BRLeft->Left = CenterX - 56;
  ZoomInX->Left = CenterX - 26;
  ZoomOutX->Left = CenterX + 4;
  BRRight->Left = CenterX + 34;
  BAUp->Top = CenterY - 56;
  ZoomInY->Top = CenterY - 26;
  ZoomOutY->Top = CenterY + 4;
  BADown->Top = CenterY + 34;
  BAUp->Left = ClientWidth - GD.Border.Right + 10;
  ZoomInY->Left = BAUp->Left;
  ZoomOutY->Left = BAUp->Left;
  BADown->Left = BAUp->Left;
  Panel2->Left = CenterX - Panel2->Width * 0.5 - 4;

  if (Visible && InitVar)
    {
      SetNewGraphSize();
      UpdateContents = true;
      Invalidate();
    }
}
//---------------------------------------------------------------------------
void TMainViewForm::DrawFrame(TCanvas *GraphCanvas, TRect GraphArea, double XStart,
                double XEnd, double YStart, double YEnd)
{
  // number of ticks on graph
  #define MIN_XTICK 7
  #define MIN_YTICK 7

  double Mark, Unit, CValue; // current value
  int Ticks,StartTick, TextSpace,i,TextWidth,TextHeight;
  char str[80];
  double XRange = XEnd - XStart, YRange = YEnd - YStart;
  TRect Frame,TempRect; // all-purpose rect

  // note that FillRect and FrameRect draws excluding lower, right corner,
  // so we add one to either side
  Frame = GraphArea;
  Frame.Right++;
  Frame.Bottom++;

  // set drawing mode to normal
  GraphCanvas->Pen->Mode = pmCopy;
  // gray frame
  GraphCanvas->Brush->Color = clWhite;
  GraphCanvas->FrameRect(Frame);
  // draw shadow
  GraphCanvas->Brush->Color = clBlack;
  TempRect.Left = GraphArea.Left + 2;
  TempRect.Right = GraphArea.Right + 2;
  TempRect.Top = GraphArea.Top - 2;
  TempRect.Bottom = GraphArea.Top;
  GraphCanvas->FillRect(TempRect);
  TempRect.Left = GraphArea.Right + 1;
  TempRect.Right = GraphArea.Right + 3;
  TempRect.Top = GraphArea.Top - 2;
  TempRect.Bottom = GraphArea.Bottom - 3;
  GraphCanvas->FillRect(TempRect);

  // set font
  GraphCanvas->Font->Size = 8;
  GraphCanvas->Font->Name = "Arial";

  // draw X - labels
	// how much space is available for text ?
	TextSpace = GD.PixelX;
  DrawLabels(TextSpace, XRange, int(MIN_XTICK), &Unit, &Ticks);
  if (ceil(XStart/Unit)*Unit == XStart) StartTick = 1;
  else StartTick = 0;
  for (i=StartTick;i<Ticks+1;i++)
    {
      CValue = i*Unit+ceil(XStart/Unit)*Unit;
      Mark = GD.PixelX*((CValue-XStart)/XRange);
      if (Mark < GD.PixelX)
        {
          GraphCanvas->Pen->Width = 1;
		      GraphCanvas->Pen->Color = clBlack;
          GraphCanvas->MoveTo(GraphArea.Left + Mark + 1, GraphArea.Bottom+1);
		      GraphCanvas->LineTo(GraphArea.Left + Mark + 1, GraphArea.Bottom+4);
          sprintf(str,"%-.15G",CValue);
		      GraphCanvas->Font->Color = clWhite;
          GraphCanvas->Brush->Color = clGray;
		      TextWidth = GraphCanvas->TextWidth(str);
		      if (TextSpace >= (TextWidth+5))
		        {
		          GraphCanvas->TextOut(GraphArea.Left + Mark-TextWidth*0.5 + 2,
                                   GraphArea.Bottom+5,str);
		          TextSpace = (Unit / XRange)*GD.PixelX;
		        }
		      else
		        TextSpace += (Unit / XRange)*GD.PixelX;
		    } // end if
	  }  // end for i

  // draw Y - labels
  // how much space is available for text ?
  TextSpace = GD.PixelY;
  DrawLabels(TextSpace, YRange, int(MIN_YTICK), &Unit, &Ticks);
  if (ceil(YStart/Unit)*Unit == YStart) StartTick = 1;
	else StartTick = 0;
	for (i=StartTick;i<Ticks+1;i++)
    {
      CValue = i*Unit+ceil(YStart/Unit)*Unit;
      Mark = GD.PixelY*((CValue-YStart)/YRange);
      if (Mark < GD.PixelY)
        {
          GraphCanvas->Pen->Width = 1;
          GraphCanvas->Pen->Color = clBlack;
          GraphCanvas->MoveTo(GraphArea.Left - 2, GraphArea.Bottom - Mark );
          GraphCanvas->LineTo(GraphArea.Left - 5, GraphArea.Bottom - Mark );
          GraphCanvas->MoveTo(GraphArea.Left - 2, GraphArea.Bottom - Mark - 1);
          GraphCanvas->LineTo(GraphArea.Left - 5, GraphArea.Bottom - Mark - 1);
          sprintf(str,"%-.15G",CValue);
          GraphCanvas->Font->Color = clWhite;
          GraphCanvas->Brush->Color = clGray;
          TextWidth = GraphCanvas->TextWidth(str);
          TextHeight = GraphCanvas->TextHeight(str);
          if (TextSpace >= (TextHeight+2))
		        {
		          GraphCanvas->TextOut(GraphArea.Left - TextWidth - 6,
                  GraphArea.Bottom - Mark - TextHeight * 0.5,str);
		          TextSpace = (Unit / YRange)*GD.PixelY;
		        }
		      else
		        TextSpace += (Unit / YRange)*GD.PixelY;
		    }  // end if
	  }  // end for i

}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormPaint(TObject *Sender)
{

  // palette
  PalBM->Draw(Canvas,Handle,1,Panel1->Height, Cpal); //346

  char str[128];
  if (InitVar)
    {
      // draw frame and units
      DrawFrame(Canvas, GD.GraphArea, GD.XStart, GD.XEnd, GD.YStart, GD.YEnd);
      // display caption
      if (FileLoaded)
        sprintf(str,"SAR Image Viewer : %s",FF.FileName.c_str());
      else
        sprintf(str,"SAR Image Viewer");
      Caption = str;
      // thumb bitmap
      ThumbBM->Draw(Canvas,Handle,(Bevel1->Width - ThumbBM->DIBWidth)*0.5-1,
        Panel1->Height+Bevel2->Height+Panel3->Height+(Bevel1->Height-ThumbBM->DIBHeight)*0.5-1, Cpal); //392
      GraphBM->Draw(Canvas, Handle, GD.Border.Left + 1, GD.Border.Top + 1, Cpal);

    }
}
//---------------------------------------------------------------------------
void TMainViewForm::SetNewGraphSize(void)
{
  // set new size of DIB
  GD.PixelX = ClientWidth - GD.Border.Right - GD.Border.Left;
  GD.PixelY = ClientHeight - GD.Border.Bottom - GD.Border.Top;

  GraphBM->Resize(Handle, GD.PixelX, GD.PixelY);

  // free previous array
  Free_DMatrix(Data1,0,0);
  Free_DMatrix(Data2,0,0);
  // create new array
  Data1 = DMatrix(0,GD.PixelX-1,0,GD.PixelY-1);
  Data2 = DMatrix(0,GD.PixelX-1,0,GD.PixelY-1);

  // set GD.GraphArea to new values
  SetRect(&GD.GraphArea,GD.Border.Left, GD.Border.Top, GD.Border.Left + GD.PixelX + 1,
              GD.Border.Top + GD.PixelY + 1);
}
// read given range into array
// D1, D2  = actual data (size = PixelX x PixelY)
// reads data from XStart to XStart + XRange
//---------------------------------------------------------------------------
void TMainViewForm::LoadDataGivenRange(double XStart, double XRange, long PixelX,
 double YStart, double YRange, long PixelY, double **D1, double **D2)
{
  struct ValueIQMag Value, OldValue;
  double x,y, xStep, yStep;
  long PixX,PixY, OldPixX, OldPixY;
  class TQuickRead QuickRead(FF);
  char str[150];

  if (!FileLoaded) return;
  if (!QuickRead.OpenFile(FF.FileName.c_str()))
    {
      FileLoaded = FALSE;
      sprintf(str,"Cannot open file '%s'",FF.FileName.c_str());
      Application->MessageBox(str, "Error", MB_OK | MB_ICONEXCLAMATION);
      return;
    }
/*
  for (PixY=0;PixY<PixelY;PixY++)
    {
      y = YStart + ((double)PixY / (double)PixelY)*YRange;
      for (PixX=0;PixX<PixelX;PixX++)
        {
          x = XStart + ((double)PixX / (double)PixelX)*XRange;
          QuickRead.ReadValue((long)x,(long)y,&Value);
          if (FF.DataType == 0) // complex
            {
              D1[PixX][PixY] = Value.I;
              D2[PixX][PixY] = Value.Q;
            }
          else // magnitude or power
            {
              D1[PixX][PixY] = Value.Mag;
            }
        }
    }
*/

  double **Da1, **Da2;
  long SamplesX, SamplesY,lx,ly;
  SamplesX = ceil(XStart + XRange) - floor(XStart) + 1;
  SamplesY = ceil(YStart + YRange) - floor(YStart) + 1;

  Da1 = DMatrix(0,SamplesX-1,0,SamplesY-1);
  Da2 = DMatrix(0,SamplesX-1,0,SamplesY-1);

  for(ly=0;ly<SamplesY;ly++)
    {
      for(lx=0;lx<SamplesX;lx++)
        {
          if ((double(lx)+XStart >= 0) && (double(lx)+XStart < FF.SizeX)
             && (double(ly)+YStart >= 0) && (double(ly)+YStart < FF.SizeY))
            {
              QuickRead.ReadValue(lx+(long)XStart,ly+(long)YStart,&Value);
              if (FF.DataType == 0) // complex
                {
                  Da1[lx][ly] = Value.I;
                  Da2[lx][ly] = Value.Q;
                }
              else // magnitude or power
                {
                  Da1[lx][ly] = Value.Mag;
                }
            }
          else
            {
              Da1[lx][ly] = 0;
              Da2[lx][ly] = 0;
            }
        }
    }

  InterpolateArray(Da1, Da2, SamplesX, SamplesY, D1, D2, PixelX, PixelY);
  Free_DMatrix(Da1,0,0);
  Free_DMatrix(Da2,0,0);
  QuickRead.CloseFile();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  Timer1->Enabled = false;

  // delete various memory allocated
  Free_DMatrix(Data1,0,0);
  Free_DMatrix(Data2,0,0);
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::Timer1Timer(TObject *Sender)
{
  if (UpdateContents && FileLoaded)
    {
      double XRange, YRange;
      UpdateContents = false;
      XRange = GD.XEnd - GD.XStart;
      YRange = GD.YEnd - GD.YStart;

      LoadDataGivenRange(GD.XStart, XRange,GD.PixelX,GD.YStart,
        YRange, GD.PixelY, Data1, Data2);

      DrawArray(GraphBM, Data1, Data2, GD.PixelX, GD.PixelY, 0, SBContrast->Position,
              SBBright->Position, SBSat->Position,ColPal);
      UpdateRange();

      Invalidate();
    }

/*
  if ((!Busy) && FileLoaded)
  {
  // update size
  if (UpdateSize)
    {
      UpdateSize = false;
      SetNewGraphSize();
      UpdateContents = true;
    }
  // update contents
  if (UpdateContents)
    {
      double XRange, YRange;
      UpdateContents = false;
      XRange = GD.XEnd - GD.XStart;
      YRange = GD.YEnd - GD.YStart;

      LoadDataGivenRange(GD.XStart, XRange,GD.PixelX,GD.YStart,
        YRange, GD.PixelY, Data1, Data2);
      Invalidate();
    }
  }
 */
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::EnterKeyPress(TObject *Sender, char &Key)
{
 if (Key == (unsigned char)13)
    {
      if (EXStart->Focused())
        EXStartExit(Sender);
      else if (EXEnd->Focused())
        EXEndExit(Sender);
      else if (EYStart->Focused())
        EYStartExit(Sender);
      else if (EYEnd->Focused())
        EYEndExit(Sender);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::EXStartExit(TObject *Sender)
{
  if ( fabs(atof(EXStart->Text.c_str()) - GD.XStart) > MIN_CHANGE)
    {
      GD.XStart =  atof(EXStart->Text.c_str());
      SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
      // show new values in edit boxes
      UpdateRange();
      UpdateContents = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::EYStartExit(TObject *Sender)
{
  if ( fabs(atof(EYStart->Text.c_str()) - GD.YStart) > MIN_CHANGE)
    {
      GD.YStart =  atof(EYStart->Text.c_str());
      SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
      // show new values in edit boxes
      UpdateRange();
      UpdateContents = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::EXEndExit(TObject *Sender)
{
  if ( fabs(atof(EXEnd->Text.c_str()) - GD.XEnd) > MIN_CHANGE)
    {
      GD.XEnd =  atof(EXEnd->Text.c_str());
      SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
      // show new values in edit boxes
      UpdateRange();
      UpdateContents = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::EYEndExit(TObject *Sender)
{
  if ( fabs(atof(EYEnd->Text.c_str()) - GD.YEnd) > MIN_CHANGE)
    {
      GD.YEnd =  atof(EYEnd->Text.c_str());
      SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
      // show new values in edit boxes
      UpdateRange();
      UpdateContents = true;
   }
}
//---------------------------------------------------------------------------
void TMainViewForm::SetNewGraphRange(double *XStart, double *XEnd,
                                       double *YStart, double *YEnd)
{
  // do some range checking
  if (*XStart <  GD.XMin) *XStart = GD.XMin;
  if (*YStart <  GD.YMin) *YStart = GD.YMin;
  if (*XEnd >  GD.XMax) *XEnd = GD.XMax;
  if (*YEnd >  GD.YMax) *YEnd = GD.YMax;
  if (*XStart > *XEnd) Swap(XStart,XEnd);
  if (*YStart > *YEnd) Swap(YStart,YEnd);
  if (*XStart == *XEnd) *XEnd = *XStart + 1;
  if (*YStart == *YEnd) *YEnd = *YStart + 1;
  GD.XStart = *XStart;
  GD.XEnd = *XEnd;
  GD.YStart = *YStart;
  GD.YEnd = *YEnd;
  GD.XRange = GD.XEnd - GD.XStart;
  GD.YRange = GD.YEnd - GD.YStart;
}
//---------------------------------------------------------------------------
void TMainViewForm::UpdateRange(void)
{
  char str[80];
  // setup window range values
  sprintf(str,"%.*f",2,GD.XStart);
  EXStart->Text = str;
  sprintf(str,"%.*f",2,GD.XEnd);
  EXEnd->Text = str;
  sprintf(str,"%.*f",2,GD.YStart);
  EYStart->Text = str;
  sprintf(str,"%.*f",2,GD.YEnd);
  EYEnd->Text = str;
}

//---------------------------------------------------------------------------
void TMainViewForm::UpdateDialogsGivenFileFormat()
{
  // depending on whether I,Q or Mag is available setup radio group
  if (FF.DataType == 0) // I and Q available
    {
      RGReal->Enabled = true;
      RGImag->Enabled = true;
      RGMag->Enabled = true;
      RGPhase->Enabled = true;
      RGPower->Enabled = true;
    }
  else if ((FF.DataType == 1) ||
          (FF.DataType == 2))  // magnitude or power only
    {
      RGReal->Enabled = false;
      RGImag->Enabled = false;
      RGMag->Enabled = true;
      RGPhase->Enabled = false;
      RGPower->Enabled = true;
    }
  // let's select magnitude by default
  RGMag->Checked = true;
  //DataType = 2;
  // now setup info textbox
  char str[100];
  Memo1->Lines->Clear();
  sprintf(str,"File    : %s",FF.FileName.c_str());
  Memo1->Lines->Add(str);
  sprintf(str,"Size    : %d x %d",FF.SizeX,
          FF.SizeY);
  Memo1->Lines->Add(str);
  if (FF.FileFormat == 0) // Sun-Raster File
    sprintf(str,"Sun-Raster-File");
  else if (FF.FileFormat == 1) // Sarsim simu
    sprintf(str,"Sarsim file : %s",FF.ScriptFile.c_str());
  else if (FF.FileFormat == 2) // custom
    sprintf(str,"Custom file");
  Memo1->Lines->Add(str);
//  sprintf(str,"%d Bytes per Value",FF.BytesPerPixel);
//  Memo1->Lines->Add(str);
//  sprintf(str,"Header  : %d",FF.HeaderSize);
//  Memo1->Lines->Add(str);
//  if (FF.DataType == 0) // mag
//    sprintf(str,"Data : Mag only, ");
//  else
//    sprintf(str,"Data : IQ, ");
//  if (FF.Endian == 0) // 8086 endian
//    strcat(str,"little-endian");
//  else
//        strcat(str,"big-endian");
//      Memo1->Lines->Add(str);



  // scroll up to show important stuff
  Memo1->SelStart = 0;
  Memo1->SelLength = 1;
  Memo1->Perform( EM_SCROLLCARET, 0, 0);

}
//---------------------------------------------------------------------------
void TMainViewForm::MakePalette(unsigned char Type,int Bright,
                            int Contrast, int Sat)
{
  // create default lookup table
  CreateRGBPal(ColPal, Bright, Contrast, Sat, Type);
  // current logical palette for bitmap
  LOGPALETTE* BMPal;
  if (!CreateLogPal(&BMPal, ColPal))
    Application->MessageBox("Cannot create palette", "Error", MB_OK | MB_ICONEXCLAMATION);
  Cpal = CreatePalette(BMPal);
}
//---------------------------------------------------------------------------
void TMainViewForm::SetupPaletteBitmap()
{
  // setup palette
  unsigned char Col;
  int i,j;
  for (i=0;i<PalBM->DIBHeight;i++)
    for (j=0;j<PalBM->DIBWidth;j++)
      {
        Col = double(j)/double(PalBM->DIBWidth) * double(NCOLOR);
        PalBM->pDIBBase[i*3*PalBM->DIBWidth + j*3] = ColPal[Col][RED];
        PalBM->pDIBBase[i*3*PalBM->DIBWidth + j*3+1] = ColPal[Col][GREEN];
        PalBM->pDIBBase[i*3*PalBM->DIBWidth + j*3+2] = ColPal[Col][BLUE];
      }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormDestroy(TObject *Sender)
{
  delete ThumbBM;
  delete PalBM;
  delete GraphBM;
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::SBBrightScroll(TObject *Sender,
  TScrollCode ScrollCode, int &ScrollPos)
{
  MakePalette(ColorPal,SBBright->Position,SBContrast->Position,
              SBSat->Position);
  SetupPaletteBitmap();
  DrawArray(GraphBM, Data1, Data2, GD.PixelX, GD.PixelY, 0, SBContrast->Position,
           SBBright->Position, SBSat->Position,ColPal);
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::SBContrastScroll(TObject *Sender,
  TScrollCode ScrollCode, int &ScrollPos)
{
  MakePalette(ColorPal,SBBright->Position,SBContrast->Position,
              SBSat->Position);
  SetupPaletteBitmap();
  DrawArray(GraphBM, Data1, Data2, GD.PixelX, GD.PixelY, 0, SBContrast->Position,
            SBBright->Position, SBSat->Position,ColPal);
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::SBSatScroll(TObject *Sender,
  TScrollCode ScrollCode, int &ScrollPos)
{
   MakePalette(ColorPal,SBBright->Position,SBContrast->Position,
              SBSat->Position);
   SetupPaletteBitmap();
   DrawArray(GraphBM, Data1, Data2, GD.PixelX, GD.PixelY, 0, SBContrast->Position,
              SBBright->Position, SBSat->Position,ColPal);
   Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::BPaletteClick(TObject *Sender)
{
  LoadPalForm->ShowModal();
  ColorPal = LoadPalForm->Pal;
  MakePalette(ColorPal,127,127,127);
  SetupPaletteBitmap();
  Invalidate();
}
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------//
// BM         -  If data is drawn as pixels it will be in this structure
// Data       -  actual data to be drawn
// PulseSendTime -  Times when pulses are sent
// PulseNo       -  Number of pulses in array
// SamplePoints  -  Size of array in x-direction
// PixelY        -  Size of array in y-direction (not necessarily equal to PulseNo)
// CurrentSim    -  CurrentSim structure data for range etc.
// SBContrast, SBBright, SBSat  -  Display variables
// ColPal        -  RGB Color palette
// ShowdB        -  normal or dB scale
void TMainViewForm::DrawArray(class TBitM *BM, double **Data1, double **Data2,
         unsigned long PixelX, unsigned long PixelY, int DisplayType, int SBContrast, int SBBright,
               int SBSat,unsigned char ColPal[NCOLOR][3])
{
  unsigned long SX,SY, Offset;
  double PMax, PMin , temp, Range, Scale;
  unsigned char Val;

  // find min and max values
  PMax = MIN_DOUBLE;
  PMin = MAX_DOUBLE;
  for (SY=0;SY<PixelY;SY++)
    {
      for (SX=0; SX < PixelX; SX++)
		    {
          temp = GetValue(DisplayType,Data1[SX][SY],Data2[SX][SY]);
          if (temp > PMax) PMax = temp;
          if (temp < PMin) PMin = temp;
        }
    }
  Range = PMax - PMin;
  if (Range <= 0) Range = MIN_FLOAT;  // let's make sure

  // calculate scaling factor
  switch (DisplayType)
    {
      case REAL:
      case IMAGINARY:
      case MAGNITUDE:
           Scale = (NCOLOR - 1)/Range;
           break;
      case PHASE:
           Scale = (NCOLOR - 1)/(2*PI); // let's use full 2 Pi for phase
           break;
    }

  // Clear the DIB
  memset(BM->pDIBBase, 0, 3*BM->DIBWidth*BM->DIBHeight);

  for (SY=0;SY<PixelY;SY++)
    {
	    for (SX=0; SX<PixelX; SX++)
		    {
              Val = (GetValue(DisplayType,Data1[SX][SY],
                          Data2[SX][SY])-PMin)*Scale;
              Offset = (SX + SY * BM->DIBWidth)*3;
              BM->pDIBBase[Offset] = ColPal[Val][RED];
              BM->pDIBBase[Offset+1] = ColPal[Val][GREEN];
              BM->pDIBBase[Offset+2] = ColPal[Val][BLUE];
		    }  // end for SX
	  }  // end for SY

}

/*





  unsigned long PNo;                   // loop variables for pulses
  unsigned long Sample;                // loop variable for samples (in azimuth)
  double PMax,PMin,temp,Range,Scale,D;
  unsigned long AziPix,Offset;
  unsigned char Val;
  int PulseStep;
  unsigned long CSlant;
  double ScaleGraph;

  // find min and max values
  PMax = MIN_DOUBLE;
  PMin = MAX_DOUBLE;
  for (PNo=0;PNo<PulseNo;PNo++)
    {
      for (Sample=0; Sample<SamplePoints; Sample++)
		    {
          temp = GetValue(CurrentSim->Display,Data[PNo][2*Sample],Data[PNo][2*Sample+1]);
          if (temp > PMax) PMax = temp;
          if (temp < PMin) PMin = temp;
        }
    }


  Range = PMax - PMin;
  if (Range <= 0) Range = MIN_FLOAT;  // let's make sure
  if (ShowdB) // dB scale
    {
      Range = -DYNAMIC_RANGE; // let's show a dynamic range of x dB
    }
  // calculate scaling factor
  switch (CurrentSim->Display)
    {
      case REAL:
      case IMAGINARY:
      case MAGNITUDE:
           Scale = (NCOLOR - 1)/Range;
           break;
      case PHASE:
           Scale = (NCOLOR - 1)/(2*PI); // let's use full 2 Pi for phase
           break;
    }
  if ((DrawAs == 0) || ((DrawAs == 3) && ((double(PixelY)/double(PulseNo))<(double)PixStep))) // colour pixels
    {
      // Clear the DIB
      memset(BM->pDIBBase, 0, 3*BM->DIBWidth*BM->DIBHeight);
      double PixPerPulse = double(PixelY) / (CurrentSim->AzimuthEnd-CurrentSim->AzimuthStart);

      for (PNo=0;PNo<PulseNo;PNo++)
        {
		      // find equivalent screen pixel value in y direction for Time
		      AziPix = (unsigned long) round( (PulseSendTime[PNo] - CurrentSim->AzimuthStart)*
                          PixPerPulse );
          if ((AziPix >= 0) && (AziPix < (unsigned long)PixelY))
  		    for (Sample=0; Sample<SamplePoints; Sample++)
		        {
  	 		      // Val = actual value at that instance
              if (!ShowdB) //normal scale
                Val = (GetValue(CurrentSim->Display,Data[PNo][2*Sample],
                          Data[PNo][2*Sample+1])-PMin)*Scale;
              else                // dB Scale
                {
                  D = FindValuedB(GetValue(CurrentSim->Display,Data[PNo][2*Sample],
                      Data[PNo][2*Sample+1]), PMax);
                  Val = (NCOLOR - 1) - D * Scale;
                }
              Offset = (Sample + AziPix * BM->DIBWidth)*3;
              BM->pDIBBase[Offset] = ColPal[Val][RED];
              BM->pDIBBase[Offset+1] = ColPal[Val][GREEN];
              BM->pDIBBase[Offset+2] = ColPal[Val][BLUE];
		        }  // end for sample
	      }  // end for PNo
    }
    }
}
*/
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormMouseDown(TObject *Sender,
  TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if ((Button == mbLeft) && (!LeftButtonDown))
	{
      // clicked in graph area ?
	  if (PointInRect(GD.GraphArea,X,Y))
		{
          LeftButtonDown = TRUE;
		  // capture mouse
		  SetCapture(Handle);    // Direct all subsequent mouse input to this window
          // store click point
		  ClickPoint.x = X;
		  ClickPoint.y = Y;
          SetRect(&ZoomRect,X, Y, X, Y);
		  // set drawing mode to inverted
          Canvas->Pen->Mode = pmNot;
		  // draw rectangle
  		  Canvas->DrawFocusRect(ZoomRect);
		}  // end if
	}  // end if
  if ((Button == mbRight) && (!LeftButtonDown) && (!RightButtonDown))  // zoom out by a factor of 2
	{
      RightButtonDown = TRUE;
      // clicked in graph area ?
	  if (PointInRect(GD.GraphArea,X,Y))
        {
		  double Xtemp,Ytemp;
	      // convert window (pixel) coordinates to actual values
		  WindowToReal(X, Y, &Xtemp, &Ytemp);
		  GD.XStart = Xtemp - GD.XRange;
		  GD.YStart = Ytemp - GD.YRange;
		  GD.XEnd = Xtemp + GD.XRange;
		  GD.YEnd = Ytemp + GD.YRange;
          SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                           &(GD.YStart),&(GD.YEnd));
          UpdateContents = true;
         // UpdateGraphBitmap();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormMouseMove(TObject *Sender, TShiftState Shift,
  int X, int Y)
{
  if (!LeftButtonDown)
    {
      // cursor on pixel area ?
      if (PointInRect(GD.GraphArea, X, Y))
        {
          if (!CursorCross)
            {
              Screen->Cursor = crCross;
              CursorCross = TRUE;
            }
          double Xtemp,Ytemp;
          char str[80];
          long PixX = X-GD.GraphArea.Left-1,PixY = GD.GraphArea.Bottom-Y-1;
          // range checking (actually should not be necessary)
          if (PixX < 0) PixX = 0;
          if (PixX >= GD.PixelX) PixX = GD.PixelX - 1;
          if (PixY < 0) PixY = 0;
          if (PixY >= GD.PixelY) PixY = GD.PixelY - 1;

          WindowToReal(X, Y, &Xtemp, &Ytemp);
          sprintf(str,"%0.2f",Xtemp);
          LX->Caption = str;
          sprintf(str,"%0.2f",Ytemp);
          LY->Caption = str;
          if (!UpdateContents)// && (RGDrawData->ItemIndex == 0))
            {
              char str[50];
              if (FormLoadSimFile->FileFormat.DataType == 1) // mag only
                {
                  sprintf(str,"%-.15G",Data1[PixX][PixY]);
                  LMag->Caption = str;
                }
              else // IQ
                {
                  sprintf(str,"%-.15G",Data1[PixX][PixY]);
                  str[12] = '\0';
                  LIn->Caption = str;
                  sprintf(str,"%-.15G",Data2[PixX][PixY]);
                  str[12] = '\0';
                  LQuad->Caption = str;
                  sprintf(str,"%-.15G",GetValue(MAGNITUDE, Data1[PixX]
                    [PixY],Data2[PixX][PixY]));
                  str[12] = '\0';
                  LMag->Caption = str;
                  sprintf(str,"%-.15G",GetValue(PHASE, Data1[PixX]
                    [PixY],Data2[PixX][PixY])*RadToDeg);
                  str[12] = '\0';
                  LPh->Caption = str;
                }

            }
          else
            {
              LIn->Caption = "";
              LQuad->Caption = "";
              LMag->Caption = "";
              LPh->Caption = "";
            }
        }
      else
        {
          if (CursorCross)
            {
              Screen->Cursor = crDefault;
              CursorCross = FALSE;
              LX->Caption = "";
              LY->Caption = "";
              LIn->Caption = "";
              LQuad->Caption = "";
              LMag->Caption = "";
              LPh->Caption = "";
            }
        }
    }

  if (LeftButtonDown)
	{
      // set drawing mode to inverted
      Canvas->Pen->Mode = pmNot;
	  // delete old rectangle
 	  Canvas->DrawFocusRect(ZoomRect);
	  // clipping
	  if (X <= GD.GraphArea.Left) X = GD.GraphArea.Left + 1;
	  if (X > GD.GraphArea.Right) X = GD.GraphArea.Right;
	  if (Y <= GD.GraphArea.Top) Y = GD.GraphArea.Top + 1;
	  if (Y > GD.GraphArea.Bottom) Y = GD.GraphArea.Bottom;
	  // set coords of new focus rect
	  if (X < ClickPoint.x)
		{ ZoomRect.Right = ClickPoint.x; ZoomRect.Left = X;  }
	  else
	    { ZoomRect.Right = X; ZoomRect.Left = ClickPoint.x;}
	  if (Y < ClickPoint.y)
	    { ZoomRect.Bottom = ClickPoint.y; ZoomRect.Top = Y; }
	  else
	    { ZoomRect.Bottom = Y; ZoomRect.Top = ClickPoint.y;}
	  // and draw it
  	  Canvas->DrawFocusRect(ZoomRect);
	 }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormMouseUp(TObject *Sender, TMouseButton Button,
  TShiftState Shift, int X, int Y)
{
  if ((Button == mbLeft) && LeftButtonDown)
	{
	  // set drawing mode to inverted
      Canvas->Pen->Mode = pmNot;
      // erase rect
  	  Canvas->DrawFocusRect(ZoomRect);
	  // release mouse
      ReleaseCapture();
	  LeftButtonDown = FALSE;
	  // convert window (pixel) coordinates to actual values
      double XStemp,YStemp,XEtemp,YEtemp;
	  WindowToReal(ZoomRect.Left, ZoomRect.Bottom, &XStemp,
        &YStemp);
	  WindowToReal(ZoomRect.Right, ZoomRect.Top, &XEtemp,
        &YEtemp);
      if ( (XStemp != XEtemp) && (YStemp != YEtemp) )
        {
          // initialize graph with new window size parameters
          SetNewGraphRange(&XStemp,&XEtemp,&YStemp,&YEtemp);
          UpdateContents = true;
         // UpdateGraphBitmap();
        }
	}
  if ((Button == mbRight) && RightButtonDown)
    RightButtonDown = FALSE;
}
//---------------------------------------------------------------------------
void TMainViewForm::WindowToReal(int PixX, int PixY, double *X, double *Y)
{
	*X = GD.XStart + double(PixX - GD.GraphArea.Left)*
		((GD.XEnd - GD.XStart) / double(GD.PixelX));
	*Y = GD.YEnd - double(PixY - GD.GraphArea.Top)*
		((GD.YEnd - GD.YStart) / double(GD.PixelY));
}
//---------------------------------------------------------------------------
void TMainViewForm::RealToWindow(double X, double Y, int *PixX, int *PixY)
{
  *PixX = (X - GD.XStart)*(double(GD.PixelX)/(GD.XEnd - GD.XStart));
  *PixY = (GD.YEnd - Y)*(double(GD.PixelY)/(GD.YEnd - GD.YStart));
}
//---------------------------------------------------------------------------
// A1 = source
// A2 = dest
void TMainViewForm::InterpolateArray(double **A1D1, double **A1D2,
   long A1SizeX, long A1SizeY, double **A2D1, double **A2D2,
   long A2SizeX, long A2SizeY)
{
  long lx,ly, lx2;
  double X;
  double *A1Xx, *A1Xy;

  A1Xx = DVector(0,A1SizeX-1);
  A1Xy = DVector(0,A1SizeX-1);

  // initialize x values
  for (lx=0;lx<A1SizeX;lx++)
    A1Xx[lx] = lx;

  for (ly=0;ly<A1SizeY;ly++)
    {
      for (lx=0;lx<A1SizeX;lx++)
        {
          // initialize y values
          A1Xy[lx]=A1D1[lx][ly];
        }
      if (ly<A2SizeY)
      {
      for (lx2=0;lx2<A2SizeX;lx2++)
        {
          X = double(lx2)*(double(A1SizeX)/double(A2SizeX));

          A2D1[lx2][ly] = Interpolate2(A1SizeX, A1Xx, A1Xy, X, 1);
       //   A2D2[lx2][ly] = Interpolate2(A1SizeX, A1Xx, A1Xy, X, 1);
        }
      }
    }


  Free_DVector(A1Xx,0);
  Free_DVector(A1Xy,0);
/*

  for (y=0;y<A1SizeY;y++)
    {
      for (x=0;x<A1SizeX;x++)
        {
          if (x<A2SizeX && y<A2SizeY)
            {
              A2D1[x][y] = A1D1[x][y];
              A2D1[x][y] = A1D1[x][y];
             }
        }
    }
*/
/*
//double Interpolate2(long n, double *x, double *y, double X,int method)
  for (PixY=0;PixY<PixelY;PixY++)
    {
      y = YStart + ((double)PixY / (double)PixelY)*YRange;
      for (PixX=0;PixX<PixelX;PixX++)
        {
          x = XStart + ((double)PixX / (double)PixelX)*XRange;
          QuickRead.ReadValue((long)x,(long)y,&Value);
          if (FF.DataType == 0) // complex
            {
              D1[PixX][PixY] = Value.I;
              D2[PixX][PixY] = Value.Q;
            }
          else // magnitude or power
            {
              D1[PixX][PixY] = Value.Mag;
            }
        }
    }
*/
}


