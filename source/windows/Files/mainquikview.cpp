//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "mainquikview.h"
#include "quikgeneral.h"
#include "quickload.h"
#include "loadfile.h"
#include "math.h"
#include "selectpal.h"
#include "general.h"
#include "objectstructdef.h"

// minimum change of range values such that an update occurs
#define MIN_CHANGE 0.9

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMainViewForm *MainViewForm;

//---------------------------------------------------------------------------
__fastcall TMainViewForm::TMainViewForm(TComponent* Owner)
	: TForm(Owner)
{
  // we are setting up, so let's not draw anything yet
  Busy = true;

  // show side graphs
  SideGraphs = true;
  // width and height of side-graphs
  WidthLeftGraph = 100;
  HeightBottomGraph = 100;

  // setup graph window
  GD.XStart = 0;
  GD.XEnd = 10000;
  GD.YStart = 0;
  GD.YEnd = 10000;
  GD.XMin = 0;
  GD.XMax = 10000;
  GD.YMin = 0;
  GD.YMax = 10000;
  GD.XRange = 10000;
  GD.YRange = 10000;
  // set bitmap size to some arbitary values
  GD.PixelX = 4;
  GD.PixelY = 4;
  SetupGraphSize(); // setup position and size of window
  SetRect(&GD.GraphArea,GD.Border.Top,GD.Border.Left,
          (int)(GD.Border.Left + GD.PixelX + 1),
          (int)(GD.Border.Top + GD.PixelY + 1));

  ColorPal = RED_BLUE_SCALE;    // default palette
  // create palette (Cpal)
  MakePalette(ColorPal,127,127,127);

  // and instantiate the bitmaps
  GraphBM = new class TBitM(Handle, GD.PixelX, GD.PixelY);
  PalBM = new class TBitM(Handle, int((Bevel2->Width - 4)& ~3),
    Bevel2->Height - 3);    // 200 18
  ThumbBM = new class TBitM(Handle, 200, 200);
  // this one used for cross-sections
  LGraphBM = new Graphics::TBitmap();
  // used for side graphs
  LeftGraphBM = new Graphics::TBitmap();
  BottomGraphBM = new Graphics::TBitmap();

  // create palette bitmap
  SetupPaletteBitmap();
}
//---------------------------------------------------------------------------
void TMainViewForm::SetupGraphSize()
{
  // Left - Right MUST be a multiple of 4
  GD.Border.Left = Panel1->Width + 44;
  if (SideGraphs)
    GD.Border.Left += WidthLeftGraph + 12;
  GD.Border.Left = (GD.Border.Left+3)& ~3;
  GD.Border.Right = BAUp->Width+14;
  GD.Border.Top = Panel2->Height + BRRight->Height + 18;
  GD.Border.Bottom = 26;
  if (SideGraphs)
    GD.Border.Bottom += HeightBottomGraph + 12;
  GD.Display = 0;
  GD.MaxWindowSize.Left = 572;
  if (SideGraphs)
    GD.MaxWindowSize.Left += WidthLeftGraph;
  GD.MaxWindowSize.Top = 438;
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormDestroy(TObject *Sender)
{
  // delete bitmaps
  delete GraphBM;
  delete PalBM;
  delete ThumbBM;
  delete LGraphBM;
  delete LeftGraphBM;
  delete BottomGraphBM;
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormPaint(TObject *Sender)
{
  if (!Busy)
    {
      char str[128];
      // display caption
      if (FileLoaded)
        sprintf(str,"SAR Image Viewer : %s",FF.FileName.c_str());
      else
        sprintf(str,"SAR Image Viewer");
      Caption = str;

      // draw frame and units
      DrawFrame(Canvas, GD.GraphArea, GD.XStart, GD.XEnd, GD.YStart, GD.YEnd);
  /*
  // draw graph bitmap
  if (RGDrawData->ItemIndex == 0) // color plot
    {
      GraphBM->Draw(Canvas, Handle, GD.Border.Left + 1, GD.Border.Top + 1, Cpal);
    }
  else // crossections
    {
      LGraphBM->IgnorePalette = TRUE;
      Canvas->Draw(GD.Border.Left + 1,GD.Border.Top + 1,LGraphBM);
    }
  // palette
  PalBM->Draw(Canvas,Handle,1,Panel1->Height, Cpal); //346
  // thumb bitmap
//  ThumbBM->Draw(Canvas,Handle,1,Panel1->Height+Bevel2->Height+Panel3->Height, Cpal); //392
  ThumbBM->Draw(Canvas,Handle,(Bevel1->Width - ThumbBM->DIBWidth)*0.5-1,
    Panel1->Height+Bevel2->Height+Panel3->Height+(Bevel1->Height-ThumbBM->DIBHeight)*0.5-1, Cpal); //392

  if (SideGraphs)
    {
      LeftGraphBM->IgnorePalette = TRUE;
      Canvas->Draw(Panel1->Width + 4 + 1, GD.GraphArea.Top + 1,LeftGraphBM);
      BottomGraphBM->IgnorePalette = TRUE;
      Canvas->Draw(GD.GraphArea.Left + 1,GD.GraphArea.Bottom + 30 + 1,BottomGraphBM);
    }

//  FindMinMaxForWindow(RGShow->ItemIndex);
//  char str[100];
  sprintf(str,"Min:%3.5f  Max:%3.5f", GraphMin, GraphMax);
  Caption = str;

    //      Canvas->Brush->Style = bsSolid;
          Canvas->Brush->Color = clWhite;
          Canvas->Pen->Color = clWhite;
          Canvas->Pen->Mode = pmWhite;//	pmNot;
      	  Canvas->DrawFocusRect(CursorRectH);
      	  Canvas->DrawFocusRect(CursorRectV);
//      SetRect(&CursorRectV, 0,0,0,0);
//      SetRect(&CursorRectH, 0,0,0,0);

*/
  } // end if busy

}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::ChangingSize(TMessage& Msg)
{
  TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::ChangedSize(TMessage& Msg)
{
  TForm::Dispatch(&Msg);
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
void __fastcall TMainViewForm::QueryPal(TMessage& Msg)
{
/*  AnsiString str;
 str = "Query :" + IntToStr((int)Handle) + "  " + IntToStr((int)Msg.WParam);
 Memo2->Lines->Add(str);
 //  if (RealizeMyPalette()) Invalidate();
*/
  TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::PalChanged(TMessage& Msg)
{
/*
//  if (RealizeMyPalette()) Invalidate();

  AnsiString str;
 str = "Change " + IntToStr((int)Handle) + "  " + IntToStr((int)Msg.WParam);
 Memo2->Lines->Add(str);

 if ((HWND)(Msg.WParam) != (HWND)Handle)
  {
   if (RealizeMyPalette(true)) Invalidate();
   str = "Handle diff ";
   Memo2->Lines->Add(str);
  }
*/
  TForm::Dispatch(&Msg);
}
// ForceBackGround should be true on WM_PALETTE_CHANGED
// and false on WM_QUERYNEWPALETTE
//---------------------------------------------------------------------------
int TMainViewForm::RealizeMyPalette(bool ForceBackGround)
{
  // realize current palette
  HDC hdc = GetDC(Handle);
  HPALETTE hOldPal = SelectPalette(hdc, Cpal, ForceBackGround);
  // if some other window fiddeled with palette repaint is necessary
  // (nChanged != 0)
  int nChanged = RealizePalette(hdc);
  SelectPalette(hdc, hOldPal, false);
  ReleaseDC(Handle, hdc);
  return nChanged;
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormShow(TObject *Sender)
{
  // do not redraw while initiating
  Busy = true;
  DataType = 2;  // we want to show magnitude
  // clear info
  Memo1->Lines->Clear();
  Memo1->Lines->Add("No image loaded.");
  MakePalette(ColorPal,SBBright->Position,SBContrast->Position,SBSat->Position);

  // create buffer for values
  Data1 = DMatrix(0,0,0,0);
  Data2 = DMatrix(0,0,0,0);
  DataThumb1 = DMatrix(0,ThumbBM->DIBWidth-1,0,ThumbBM->DIBHeight-1);
  DataThumb2 = DMatrix(0,ThumbBM->DIBWidth-1,0,ThumbBM->DIBHeight-1);
  LeftButtonDown = FALSE;
  RightButtonDown = FALSE;

  // setup graph window
  GD.XStart = 0;
  GD.XEnd = 10000;
  GD.YStart = 0;
  GD.YEnd = 10000;
  GD.XMin = 0;
  GD.XMax = 10000;
  GD.YMin = 0;
  GD.YMax = 10000;
  GD.XRange = 10000;
  GD.YRange = 10000;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),&(GD.YStart),&(GD.YEnd));
  SetNewGraphSize();
  IsUptodate = FALSE;
  FileLoaded = FALSE;

  // enable timer
  Timer1->Enabled = TRUE;
  Busy = false;
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
void __fastcall TMainViewForm::FormResize(TObject *Sender)
{
  // only allow widths a multiple of 3 such that bitmap width will be a multiple of 4
  ClientWidth = (ClientWidth + 3) & ~3;

  // setup new button positions etc.
  int NewPixX = ClientWidth - GD.Border.Right - GD.Border.Left,
      NewPixY = ClientHeight - GD.Border.Bottom - GD.Border.Top;
  int CenterX = GD.Border.Left + NewPixX * 0.5,
      CenterY = GD.Border.Top + NewPixY * 0.5;

  BRLeft->Left = CenterX - 56;
  ZoomInX->Left = CenterX - 26;
  ZoomOutX->Left = CenterX + 4;
  BRRight->Left = CenterX + 34;
  BAUp->Top = CenterY - 56;
  ZoomInY->Top = CenterY - 26;
  ZoomOutY->Top = CenterY + 4;
  BADown->Top = CenterY + 34;
  BAUp->Left = NewPixX + GD.Border.Left + 10;
  ZoomInY->Left = BAUp->Left;
  ZoomOutY->Left = BAUp->Left;
  BADown->Left = BAUp->Left;
  Panel2->Left = CenterX - Panel2->Width * 0.5 - 4;

  // initialize graph with new window size parameters
  if (Visible && !Busy)
    {
//      if (SideGraphs)
//        DrawSideGraph(Canvas, GD.GraphArea, GD.XStart, GD.XEnd, GD.YStart,
//            GD.YEnd, 0,0);
      SetNewGraphSize();
      Timer1->Enabled = FALSE;
      Timer1->Enabled = TRUE;
      IsUptodate = FALSE;
      Invalidate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
/*
  if ((Button == mbLeft) && LeftButtonDown)
	{
	  // set drawing mode to inverted
        // set drawing mode to inverted
      Canvas->Pen->Mode = pmNot;
      Canvas->Brush->Color = clWhite;
      Canvas->Pen->Mode = pmWhite;//	pmNot;
//    Canvas->Pen->Mode = pmNot;
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
          Busy = true;
          // initialize graph with new window size parameters
          SetNewGraphRange(&XStemp,&XEtemp,&YStemp,&YEtemp);
          UpdateGraphBitmap();
          FindMinMaxForWindow(RGShow->ItemIndex);
          Busy = false;
        }
	}
  if ((Button == mbRight) && RightButtonDown)
    RightButtonDown = FALSE;
*/
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
/*
  if (!LeftButtonDown)
    {
    if (PointInRect(GD.GraphArea, X, Y))
        {
          Canvas->Brush->Color = clWhite;
          Canvas->Pen->Color = clWhite;
          Canvas->Pen->Mode = pmWhite;//	pmNot;
      	  Canvas->DrawFocusRect(CursorRectV);
          SetRect(&CursorRectV, X, GD.GraphArea.Top+1, X, GD.GraphArea.Bottom);
      	  Canvas->DrawFocusRect(CursorRectV);
      	  Canvas->DrawFocusRect(CursorRectH);
          SetRect(&CursorRectH, GD.GraphArea.Left+1, Y, GD.GraphArea.Right, Y);
      	  Canvas->DrawFocusRect(CursorRectH);

        //  Canvas->MoveTo(X,GD.GraphArea.Top);
        //  Canvas->LineTo(X,GD.GraphArea.Bottom);
          double Xtemp,Ytemp;
          char str[30];
          WindowToReal(X, Y, &Xtemp, &Ytemp);
          if (SideGraphs)
            DrawSideGraph(Canvas, GD.GraphArea, GD.XStart, GD.XEnd, GD.YStart,
              GD.YEnd, X-1-GD.Border.Left, Y-1-GD.Border.Top);

          sprintf(str,"%d",(long)round(Xtemp));
          if (strlen(str) > 7) str[7]='\0';
          LX->Caption = str;
          sprintf(str,"%d",(long)round(Ytemp));
          if (strlen(str) > 7) str[7]='\0';
          LY->Caption = str;
          if (IsUptodate)// && (RGDrawData->ItemIndex == 0))
            {
              char str[50];
              if (FF.DataType == 0) // mag only
                {
                  sprintf(str,"-.15G",Data1[X-1-GD.Border.Left]
                    [Y-1-GD.Border.Top]);
                  LMag->Caption = str;
                }
              else // IQ
                {
                  sprintf(str,"%-.15G",Data1[X-1-GD.Border.Left]
                    [Y-1-GD.Border.Top]);
                  str[7] = '\0';
                  LIn->Caption = str;
                  sprintf(str,"%-.15G",Data2[X-1-GD.Border.Left]
                    [Y-1-GD.Border.Top]);
                  str[7] = '\0';
                  LQuad->Caption = str;
                  sprintf(str,"%-.15G",GetValue(MAGNITUDE, Data1[X-1-GD.Border.Left]
                    [Y-1-GD.Border.Top],Data2[X-1-GD.Border.Left]
                    [Y-1-GD.Border.Top]));
                  str[7] = '\0';
                  LMag->Caption = str;
                  sprintf(str,"%-.15G",GetValue(PHASE, Data1[X-1-GD.Border.Left]
                    [Y-1-GD.Border.Top],Data2[X-1-GD.Border.Left]
                    [Y-1-GD.Border.Top])*RadToDeg);
                  str[7] = '\0';
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
        }
    }

  if (LeftButtonDown)
	{
      // set drawing mode to inverted
//      Canvas->Pen->Mode = pmNot;
      // set drawing mode to inverted
//      Canvas->Pen->Mode = pmNot;
      Canvas->Brush->Color = clWhite;
      Canvas->Pen->Mode = pmWhite;//	pmNot;

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
*/
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
/*
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
        // set drawing mode to inverted
      Canvas->Pen->Mode = pmNot;
      Canvas->Brush->Color = clWhite;
      Canvas->Pen->Mode = pmWhite;//	pmNot;
//        Canvas->Pen->Mode = pmNot;
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
          UpdateGraphBitmap();
        }
    }
*/
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

/*
  Timer1->Enabled = FALSE;
  Timer1->Enabled = TRUE;
  IsUptodate = FALSE;
*/
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::Open1Click(TObject *Sender)
{
/*
struct SFileFormat
  {
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
  };
*/
  Busy = true;
  if (FormLoadSimFile->ShowModal() == mrOk)
    {
      // copy file format parameters back to local FF structure
      memcpy(&FF,&(FormLoadSimFile->FileFormat),sizeof(struct SFileFormat));
      FileLoaded = TRUE;
      UpdateDialogsGivenFileFormat();
    }
  Busy = false;
//      LoadBitmap(ThumbBM, 0, FF.SizeX - 1,
//        0,FF.SizeY - 1, DataThumb1, DataThumb2);
//      ShowAll();
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
  DataType = 2;
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
void __fastcall TMainViewForm::Timer1Timer(TObject *Sender)
{
/*   if ((!IsUptodate) && FileLoaded && !Busy)
     {
       UpdateGraphBitmap();
       UpdateThumbBM();
     }
*/
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
void TMainViewForm::DrawCut(void)
{
/*
  unsigned long CAzi, CSlant;
  double Val;
  double Scale;

  // Clear the DIB to white
  LGraphBM->Width = GD.PixelX;
  LGraphBM->Height = GD.PixelY;
  TRect ClearArea;
  SetRect(&ClearArea,0, 0, GD.PixelX, GD.PixelY);
  LGraphBM->Canvas->Brush->Color = clWhite;
  LGraphBM->Canvas->FillRect(ClearArea);

  // find scaling factor
  double Range = FF.MaxValue -
                     FF.MinValue;
  if (Range == 0) Range = 1;
  switch (RGShow->ItemIndex)
        {
          case REAL:
          case IMAGINARY:
            Scale = (PixStep - 1)/Range;
            break;
          case MAGNITUDE:
            Scale = (PixStep - 1)/(Range*1.414213563);
            break;
          case PHASE:
            Scale = (PixStep - 1)/(2*PI);
            break;
        }

  if (RGDrawData->ItemIndex == 1) // cuts in range
    {
      double SlantPD;
      // step size in slant (x) range for which a graph will be drawn
      double SlantStep = ceil((GD.XRange * PixStep)/double(GD.PixelX));
      // we don't want to go below pixel level
      if (SlantStep < 1) SlantStep = 1;
      for (SlantPD = round(GD.XStart); SlantPD < GD.XEnd; SlantPD += SlantStep)
        {
          // CSlant = corresponding pixel position
          CSlant = (unsigned long)round(((SlantPD - GD.XStart) / GD.XRange)*
		        double(GD.PixelX));
          if ((CSlant >= 0) && (CSlant < GD.PixelX))
            {
              LGraphBM->Canvas->Pen->Color = clGray;
              LGraphBM->Canvas->MoveTo(CSlant,0);
              LGraphBM->Canvas->LineTo(CSlant,GD.PixelY);
              LGraphBM->Canvas->Pen->Color = clBlue;
              for (CAzi = 0; CAzi < GD.PixelY; CAzi ++)
                {
                  Val = GetValue(RGShow->ItemIndex, Data1[CSlant][CAzi],
                    Data2[CSlant][CAzi])*Scale;
                  if (CAzi == 0)
                    LGraphBM->Canvas->MoveTo(CSlant+Val,CAzi);
                  else
                   LGraphBM->Canvas->LineTo(CSlant+Val,CAzi);
               }
            }
        }  // end for
    } // end if
  else if (RGDrawData->ItemIndex == 2) // cuts in azimuth
    {
      double AziPD;
      // step size in azimuth (y) for which a graph will be drawn
      double AziStep = ceil((GD.YRange * PixStep)/double(GD.PixelY));
      if (AziStep < 1) AziStep = 1;
      for (AziPD = round(GD.YStart); AziPD < GD.YEnd; AziPD += AziStep)
        {
          // CAzi = corresponding pixel position
          CAzi = (unsigned long)round(((AziPD - GD.YStart)/GD.YRange)*
            double(GD.PixelY));
          if ((CAzi >=0) && (CAzi < GD.PixelY))
            {
              LGraphBM->Canvas->Pen->Color = clGray;
              LGraphBM->Canvas->MoveTo(0, GD.PixelY - 1 - CAzi);
              LGraphBM->Canvas->LineTo(GD.PixelX,GD.PixelY - 1 - CAzi);
              LGraphBM->Canvas->Pen->Color = clBlue;
              for (CSlant = 0; CSlant < GD.PixelX; CSlant ++)
                {
                  Val = GetValue(RGShow->ItemIndex, Data1[CSlant][CAzi],
                    Data2[CSlant][CAzi])*Scale;
                  if (CSlant == 0)
                    LGraphBM->Canvas->MoveTo(CSlant,GD.PixelY-1-(CAzi+Val));
                  else
                    LGraphBM->Canvas->LineTo(CSlant,GD.PixelY-1-(CAzi+Val));
                } // end for
            } // end if
        } // end for
    }  // end if
 */   
}
//---------------------------------------------------------------------------
void TMainViewForm::UpdateGraphBitmap(void)
{
  if (FileLoaded)
    {
/*
          Canvas->Brush->Color = clWhite;
          Canvas->Pen->Color = clWhite;
          Canvas->Pen->Mode = pmWhite;//	pmNot;
      	  Canvas->DrawFocusRect(CursorRectV);
      	  Canvas->DrawFocusRect(CursorRectH);
      SetRect(&CursorRectV, 0,0,0,0);
      SetRect(&CursorRectH, 0,0,0,0);
*/


  LoadBitmap(GraphBM, GD.XStart, GD.XRange, GD.YStart, GD.YRange, Data1, Data2);
  DrawCut();
  IsUptodate = TRUE;
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

  double CValue; // current value
  double Mark, Ticks, Unit;
  int StartTick, expo, TextSpace,i,TextWidth,TextHeight;
  char str[80];
  double XRange = XEnd - XStart;
  double YRange = YEnd - YStart;

  TRect Frame;
  // all-purpose rect
  TRect TempRect;

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

        GraphCanvas->Font->Size = 8;
        GraphCanvas->Font->Name = "Arial";

	      // how much space is available for text ?
	      TextSpace = GD.PixelX;
	      // find how how many ticks should be drawn
	      Ticks = SciNotation(XRange,&expo);
          // Unit = difference from one tick to the next
	      Unit = pow10(expo);
	      // adjust number of ticks
          while (Ticks < MIN_XTICK)
            {Ticks *=2; Unit /= 2;}
	      Ticks = floor(Ticks);

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

 	      // how much space is available for text ?
	      TextSpace = GD.PixelY;
	      // find how how many ticks should be drawn
	      Ticks = SciNotation(YRange,&expo);
	      // Unit = difference from one tick to the next
	      Unit = pow10(expo);
	      // adjust number of ticks
          while (Ticks < MIN_YTICK)
            {Ticks *=2; Unit /= 2;}
	      Ticks = floor(Ticks);
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
    // show new values in edit boxes
  UpdateRange();

  Timer1->Enabled = FALSE;
  Timer1->Enabled = TRUE;
  IsUptodate = FALSE;
  if(FileLoaded)
    {
      UpdateThumbBM();
    }

}
//---------------------------------------------------------------------------
void TMainViewForm::UpdateRange(void)
{
  char str[80];
  // setup window range values
  sprintf(str,"%.*f",0,GD.XStart);
  EXStart->Text = str;
  sprintf(str,"%.*f",0,GD.XEnd);
  EXEnd->Text = str;
  sprintf(str,"%.*f",0,GD.YStart);
  EYStart->Text = str;
  sprintf(str,"%.*f",0,GD.YEnd);
  EYEnd->Text = str;
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  Busy = true;
  Timer1->Enabled = FALSE;
  // delete various memory allocated
  Free_DMatrix(Data1,0,0);
  Free_DMatrix(Data2,0,0);
  Free_DMatrix(DataThumb1,0,0);
  Free_DMatrix(DataThumb2,0,0);
}
//---------------------------------------------------------------------------
void TMainViewForm::WindowToReal(int PixX, int PixY, double *X, double *Y)
{
/*	*X = GD.XStart + double(PixX - GD.GraphArea.Left)*
		(GD.XRange / double(GD.PixelX));
	*Y = GD.YEnd - double(PixY - GD.GraphArea.Top)*
		(GD.YRange / double(GD.PixelY));
*/	*X = GD.XStart + double(PixX - GD.GraphArea.Left)*
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
void __fastcall TMainViewForm::ZoomInXClick(TObject *Sender)
{
  GD.XStart = GD.XStart + 0.25 * GD.XRange;
  GD.XEnd = GD.XEnd - 0.25 * GD.XRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::ZoomOutXClick(TObject *Sender)
{
  GD.XStart = GD.XStart - 0.5 * GD.XRange;
  GD.XEnd = GD.XEnd + 0.5 * GD.XRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::ZoomInYClick(TObject *Sender)
{
  GD.YStart = GD.YStart + 0.25 * GD.YRange;
  GD.YEnd = GD.YEnd - 0.25 * GD.YRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::ZoomOutYClick(TObject *Sender)
{
  GD.YStart = GD.YStart - 0.5 * GD.YRange;
  GD.YEnd = GD.YEnd + 0.5 * GD.YRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::BRLeftClick(TObject *Sender)
{
  GD.XStart = GD.XStart - 0.25 * GD.XRange;
  GD.XEnd = GD.XEnd - 0.25 * GD.XRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::BRRightClick(TObject *Sender)
{
  GD.XStart = GD.XStart + 0.25 * GD.XRange;
  GD.XEnd = GD.XEnd + 0.25 * GD.XRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::BAUpClick(TObject *Sender)
{
  GD.YStart = GD.YStart + 0.25 * GD.YRange;
  GD.YEnd = GD.YEnd + 0.25 * GD.YRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::BADownClick(TObject *Sender)
{
  GD.YStart = GD.YStart - 0.25 * GD.YRange;
  GD.YEnd = GD.YEnd - 0.25 * GD.YRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::EXStartExit(TObject *Sender)
{
  if ( fabs(atof(EXStart->Text.c_str()) - GD.XStart) > MIN_CHANGE)
    {
      GD.XStart =  atof(EXStart->Text.c_str());
      SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
      UpdateGraphBitmap();
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
      UpdateGraphBitmap();
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
      UpdateGraphBitmap();
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
      UpdateGraphBitmap();
   }
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
void __fastcall TMainViewForm::BAllClick(TObject *Sender)
{
  ShowAll();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::ShowAll()
{
  if (FileLoaded)
    {
      Busy = true;
      GD.XMax = FF.SizeX - 1;
      GD.YMax = FF.SizeY - 1;
      GD.XStart = 0;
      GD.XEnd = GD.XMax;
      GD.YStart = 0;
      GD.YEnd = GD.YMax;
      // initialize graph with new window size parameters
      SetNewGraphRange(&(GD.XStart),&(GD.XEnd), &(GD.YStart),&(GD.YEnd));
      UpdateGraphBitmap();
      FindMinMaxForWindow();
      Busy = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::RGShowClick(TObject *Sender)
{
  Busy = true;
  if (FileLoaded)
    {
      UpdateGraphBM();
      UpdateThumbBM();
      FindMinMaxForWindow();
    }
  Busy = false;
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::RGDrawDataClick(TObject *Sender)
{
  if (RGDrawData->ItemIndex == 0) // show color, not line
    {
      SBBright->Enabled = true;
      SBContrast->Enabled = true;
      SBSat->Enabled = true;
    }
  else
    {
      SBBright->Enabled = false;
      SBContrast->Enabled = false;
      SBSat->Enabled = false;
    }
  UpdateGraphBM();
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::SBScroll(TObject *Sender,
	TScrollCode ScrollCode, int &ScrollPos)
{
  if ((ScrollCode == scPosition) || (ScrollCode == scLineUp) || (ScrollCode == scPageUp) ||
      (ScrollCode == scLineDown) || (ScrollCode == scPageDown))
    {
      MakePalette(ColorPal,SBBright->Position,SBContrast->Position,
              SBSat->Position);
      SetupPaletteBitmap();
      if (FileLoaded)
        {
          UpdateGraphBM();
          UpdateThumbBM();
        }
      Invalidate();
    }
}
//---------------------------------------------------------------------------
void TMainViewForm::UpdateThumbBM(void)
{
/*

  UpdateBM(ThumbBM, DataThumb1, DataThumb2);
  long CAzi, CSlant;
  unsigned long Offset;
  // as bitmaps are shown inverted, values have to be subtracted from size
  unsigned long pDIBBaseInvert = (ThumbBM->DIBHeight * ThumbBM->DIBWidth * 3)-
    (ThumbBM->DIBWidth * 3);

    // draw rectangle
      TRect ZoomRect;
      ZoomRect.Left = (int)((GD.XStart / FF.SizeX)*double(ThumbBM->DIBWidth));
      ZoomRect.Right = (int)((GD.XEnd / FF.SizeX)*double(ThumbBM->DIBWidth));
      ZoomRect.Bottom = ThumbBM->DIBHeight - 1 - (int)((GD.YStart / FF.SizeY)*double(ThumbBM->DIBHeight));
      ZoomRect.Top = ThumbBM->DIBHeight - 1 - (int)((GD.YEnd / FF.SizeY)*double(ThumbBM->DIBHeight));

      unsigned char ColRGB[2] = {255,254};
      bool Toggle=true;
      CAzi = ZoomRect.Top;
      for (CSlant = ZoomRect.Left; CSlant < ZoomRect.Right; CSlant ++)
        {
          Toggle = !Toggle;
          Offset = pDIBBaseInvert + (CSlant - CAzi * ThumbBM->DIBWidth)*3;
          ThumbBM->pDIBBase[Offset] = ColRGB[Toggle];
          ThumbBM->pDIBBase[Offset+1] = ColRGB[Toggle];
          ThumbBM->pDIBBase[Offset+2] = ColRGB[Toggle];
        }
      CAzi = ZoomRect.Bottom;
      for (CSlant = ZoomRect.Left; CSlant < ZoomRect.Right; CSlant ++)
        {
          Toggle = !Toggle;
          Offset = pDIBBaseInvert + (CSlant - CAzi * ThumbBM->DIBWidth)*3;
          ThumbBM->pDIBBase[Offset] = ColRGB[Toggle];
          ThumbBM->pDIBBase[Offset+1] = ColRGB[Toggle];
          ThumbBM->pDIBBase[Offset+2] = ColRGB[Toggle];
        }
      CSlant = ZoomRect.Left;
      for (CAzi = ZoomRect.Top; CAzi < ZoomRect.Bottom; CAzi++)
        {
          Toggle = !Toggle;
          Offset = pDIBBaseInvert + (CSlant - CAzi * ThumbBM->DIBWidth)*3;
          ThumbBM->pDIBBase[Offset] = ColRGB[Toggle];
          ThumbBM->pDIBBase[Offset+1] = ColRGB[Toggle];
          ThumbBM->pDIBBase[Offset+2] = ColRGB[Toggle];
        }
      CSlant = ZoomRect.Right;
      for (CAzi = ZoomRect.Top; CAzi < ZoomRect.Bottom; CAzi ++)
        {
          Toggle = !Toggle;
          Offset = pDIBBaseInvert + (CSlant - CAzi * ThumbBM->DIBWidth)*3;
          ThumbBM->pDIBBase[Offset] = ColRGB[Toggle];
          ThumbBM->pDIBBase[Offset+1] = ColRGB[Toggle];
          ThumbBM->pDIBBase[Offset+2] = ColRGB[Toggle];
        }
*/
}
//---------------------------------------------------------------------------
void TMainViewForm::UpdateGraphBM(void)
{
  if (FileLoaded)
    {
      if (RGDrawData->ItemIndex == 0) // color plot
        UpdateBM(GraphBM, Data1, Data2);
      else
        DrawCut();
      IsUptodate = TRUE;
    }
}
//---------------------------------------------------------------------------
void TMainViewForm::UpdateBM(class TBitM *BM, double **DI, double **DQ)
{
/*
  unsigned long CAzi, CSlant;
  unsigned long Offset;
  unsigned char Val;
  double Mag, Phase;
      // Clear the DIB
      memset(BM->pDIBBase, 0, 3*BM->DIBWidth*BM->DIBHeight);
      // as bitmaps are shown inverted, values have to be subtracted from size
      unsigned long pDIBBaseInvert = (BM->DIBHeight * BM->DIBWidth * 3) - (BM->DIBWidth * 3);
      // find scaling factor
      double Scale;
      double Range = FF.MaxValue -
                     FF.MinValue;
      switch (RGShow->ItemIndex)
        {
          case REAL:
          case IMAGINARY:
            Scale = (NCOLOR - 1)/Range;
            break;
          case MAGNITUDE:
            Scale = (NCOLOR - 1)/(Range*1.414213563);
            break;
          case PHASE:
            Scale = (NCOLOR - 1)/(2*PI);
            break;
        }
      for (CAzi = 0; CAzi < (unsigned long)BM->DIBHeight; CAzi ++)
        {
          for (CSlant = 0; CSlant < (unsigned long)BM->DIBWidth; CSlant ++)
            {
              Offset = pDIBBaseInvert + (CSlant - CAzi * BM->DIBWidth)*3;
              switch (RGShow->ItemIndex)
                {
                      case REAL:
                        Val = (DI[CSlant][CAzi]+FF.Offset)*Scale;
                        break;
                      case IMAGINARY:
                        Val = (DQ[CSlant][CAzi]+FF.Offset)*Scale;
                        break;
                      case MAGNITUDE:
                        Mag = sqrt(DI[CSlant][CAzi] * DI[CSlant][CAzi] +
                                 DQ[CSlant][CAzi] * DQ[CSlant][CAzi]);
                        Val = (Mag+FF.Offset)*Scale;
                        break;
                      case PHASE:
                        Phase = GetValue(PHASE,DI[CSlant][CAzi],DQ[CSlant][CAzi]);
                      //  if ((DI[CSlant][CAzi] != 0) || (DQ[CSlant][CAzi] != 0))
                      //    Phase = atan2(DQ[CSlant][CAzi],DI[CSlant][CAzi]);
                 	    //  else Phase = 0;
                      //  if (Phase < PI) Phase += (2*PI);
                      //  if (Phase > PI) Phase -= (2*PI);
                        Val = (Phase+PI)*Scale;
                        break;
                }
              BM->pDIBBase[Offset] = ColPal[Val][RED];
              BM->pDIBBase[Offset+1] = ColPal[Val][GREEN];
              BM->pDIBBase[Offset+2] = ColPal[Val][BLUE];
            }
        }
*/
}
// read given range into array
//---------------------------------------------------------------------------
void TMainViewForm::LoadDataGivenRange(long XStart, long PixelX, long YStart,
  long PixelY, double **D1, double **D2)
{
  struct ValueIQMag Value;
  long x,y;
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
  for (y=YStart;y<YStart+PixelY;y++)
    {
      for (x=XStart;x<XStart+PixelX;x++)
        {
          QuickRead.ReadValue(x,y,&Value);
          if (FF.DataType == 0) // complex
            {
              D1[x][y] = Value.I;
              D1[x][y] = Value.Q;
            }
          else // magnitude or power
            {
              D1[x][y] = Value.Mag;
            }
        }
    }
  QuickRead.CloseFile();
}
//---------------------------------------------------------------------------
void TMainViewForm::LoadBitmap(class TBitM *BM, double XStart, double XRange,
  double YStart, double YRange, double **D1, double **D2)
{
/*
  unsigned long AziP, SlantP;
  unsigned long CAzi, CSlant;
  unsigned long TotalPointsX, TotalPointsY;
  char str[150];
  class TQuickRead QuickRead(FF);
  unsigned long Offset;
  unsigned char Val;
  double I, Q, Mag, Phase;

 if (!FileLoaded) return;
  if (!QuickRead.OpenFile(FF.FileName.c_str()))
    {
      FileLoaded = FALSE;
      sprintf(str,"Cannot open file '%s'",FF.FileName.c_str());
      Application->MessageBox(str, "Error", MB_OK | MB_ICONEXCLAMATION);
      return;
    }
  TotalPointsX = FF.SizeX;
  TotalPointsY = FF.SizeY;

      // Clear the DIB
      memset(BM->pDIBBase, 0, 3*BM->DIBWidth*BM->DIBHeight);
      // as bitmaps are shown inverted, values have to be subtracted from size
      unsigned long pDIBBaseInvert = (BM->DIBHeight * BM->DIBWidth * 3) - (BM->DIBWidth * 3);
      // find scaling factor
      double Scale;
      double Range = FF.MaxValue -
                     FF.MinValue;
      switch (RGShow->ItemIndex)
        {
          case REAL:
          case IMAGINARY:
            Scale = (NCOLOR - 1)/Range;
            break;
          case MAGNITUDE:
            Scale = (NCOLOR - 1)/(Range*1.414213563);
            break;
          case PHASE:
            Scale = (NCOLOR - 1)/(2*PI);
            break;
        }
      double PixPerXRange = (XRange / double(BM->DIBWidth));
      double PixPerYRange = (YRange / double(BM->DIBHeight));
      for (CAzi = 0; CAzi < (unsigned long)BM->DIBHeight; CAzi ++)
        {
          sprintf(str,"Loading file : %3.0f %%",double(CAzi)/double(BM->DIBHeight) * 100.0);
          Caption = str;
          AziP = round(YStart+YRange  - double(CAzi)*PixPerYRange);
          if (AziP < TotalPointsY)
          for (CSlant = 0; CSlant < (unsigned long)BM->DIBWidth; CSlant ++)
            {
  	          SlantP = round(XStart + double(CSlant)* PixPerXRange);
              if (SlantP < TotalPointsX)
                {
                  QuickRead.ReadValue(SlantP, AziP, &I, &Q, &Mag, &Phase);
                  if (FF.DataType == 0) // mag only
                    {
                      DI[CSlant][CAzi] = Mag;
                      DQ[CSlant][CAzi] = 0;
                    }
                  else // IQ
                    {
                      DI[CSlant][CAzi] = I;
                      DQ[CSlant][CAzi] = Q;
                    }
                  Offset = pDIBBaseInvert + (CSlant - CAzi * BM->DIBWidth)*3;
                  switch (RGShow->ItemIndex)
                    {
                      case REAL:
                        Val = (I+FF.Offset)*Scale;
                        break;
                      case IMAGINARY:
                        Val = (Q+FF.Offset)*Scale;
                        break;
                      case MAGNITUDE:
                        Val = (Mag+FF.Offset)*Scale;
                        break;
                      case PHASE:
                        Val = (Phase+PI)*Scale;
                        break;
                    }
                  BM->pDIBBase[Offset] = ColPal[Val][RED];
                  BM->pDIBBase[Offset+1] = ColPal[Val][GREEN];
                  BM->pDIBBase[Offset+2] = ColPal[Val][BLUE];
                } // end if
          }
       }
  QuickRead.CloseFile();
*/
//void __fastcall TQuickRead::ReadValue(unsigned int X, unsigned int Y,
//  struct ValueIQMag *Value)


/*
  unsigned long AziP, SlantP;
  unsigned long CAzi, CSlant;
  unsigned long TotalPixelsX, TotalPixelsY;
  char str[150];
  class TQuickRead QuickRead(FF);
  unsigned long Offset;
  unsigned char Val;
  double I, Q, Mag, Phase;

  if (!FileLoaded) return;

  if (!QuickRead.OpenFile(FF.FileName.c_str()))
    {
      FileLoaded = FALSE;
      sprintf(str,"Cannot open file '%s'",FF.FileName.c_str());
      Application->MessageBox(str, "Error", MB_OK | MB_ICONEXCLAMATION);
      return;
    }
  TotalPixelsX = FF.SizeX;
  TotalPixelsY = FF.SizeY;

      double PixPerXRange = (XRange / double(BM->DIBWidth));
      double PixPerYRange = (YRange / double(BM->DIBHeight));
      for (CAzi = 0; CAzi < (unsigned long)BM->DIBHeight; CAzi ++)
        {
          sprintf(str,"Loading file : %3.0f %%",double(CAzi)/double(BM->DIBHeight) * 100.0);
          Caption = str;
          AziP = round(YStart+YRange  - double(CAzi)*PixPerYRange);
          if (AziP < TotalPixelsY)
          for (CSlant = 0; CSlant < (unsigned long)BM->DIBWidth; CSlant ++)
            {
  	          SlantP = round(XStart + double(CSlant)* PixPerXRange);
              if (SlantP < TotalPixelsX)
                {
                  QuickRead.ReadValue(SlantP, AziP, &I, &Q, &Mag, &Phase);
                  if (FF.DataType == 0) // mag only
                    {
                      DI[CSlant][CAzi] = Mag;
                      DQ[CSlant][CAzi] = 0;
                    }
                  else // IQ
                    {
                      DI[CSlant][CAzi] = I;
                      DQ[CSlant][CAzi] = Q;
                    }
                  Offset = pDIBBaseInvert + (CSlant - CAzi * BM->DIBWidth)*3;
                  switch (RGShow->ItemIndex)
                    {
                      case REAL:
                        Val = (I+FF.Offset)*Scale;
                        break;
                      case IMAGINARY:
                        Val = (Q+FF.Offset)*Scale;
                        break;
                      case MAGNITUDE:
                        Val = (Mag+FF.Offset)*Scale;
                        break;
                      case PHASE:
                        Val = (Phase+PI)*Scale;
                        break;
                    }
                  BM->pDIBBase[Offset] = ColPal[Val][RED];
                  BM->pDIBBase[Offset+1] = ColPal[Val][GREEN];
                  BM->pDIBBase[Offset+2] = ColPal[Val][BLUE];
                } // end if
          }
       }
  QuickRead.CloseFile();
*/
}
//---------------------------------------------------------------------------
void TMainViewForm::FindMinMaxForWindow()
{
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
           MinValuePh, MaxValuePh,
           MinValuePower, MaxValuePower;
*/
/*
  unsigned long CAzi, CSlant;
  double temp;

  // set to opposite max
  FF.MinValueI = MAX_DOUBLE;
  FF.MaxValueI = MIN_DOUBLE;
  FF.MinValueQ = MAX_DOUBLE;
  FF.MaxValueQ = MIN_DOUBLE;
  FF.MinValueMag = MAX_DOUBLE;
  FF.MaxValueMag = MIN_DOUBLE;
  FF.MinValuePh = MAX_DOUBLE;
  FF.MaxValuePh = MIN_DOUBLE;
  FF.MinValuePower = MAX_DOUBLE;
  FF.MaxValuePower = MIN_DOUBLE;

  for (CAzi = 0; CAzi < (unsigned long)GraphBM->DIBHeight; CAzi ++)
    {
      for (CSlant = 0; CSlant < (unsigned long)GraphBM->DIBWidth; CSlant ++)
        {
//          if (What != DB_POWER_MAG)
            temp = GetValue(What,Data1[CSlant][CAzi],Data2[CSlant][CAzi]);
//          else
//            temp = GetValue(MAGNITUDE,Data1[CSlant][CAzi],Data2[CSlant][CAzi]);
          if (temp > GraphMax) GraphMax = temp;
          if (temp < GraphMin) GraphMin = temp;
        }
    }
 */
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::Exit1Click(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void TMainViewForm::DrawSideGraph(TCanvas *GraphCanvas, TRect GraphArea, double XStart,
                double XEnd, double YStart, double YEnd, int X, int Y)
{
/*
  double XRange = XEnd - XStart;
  double YRange = YEnd - YStart;
  unsigned long CAzi, CSlant;
  double Val;
  double Scale;
  TRect ClearArea;
  TRect FrameL,FrameB;
  // all-purpose rect
  TRect TempRect;

   // note that FillRect and FrameRect draws excluding lower, right corner,
   // so we add one to either side
   FrameL = GraphArea;
   FrameL.Left = Panel1->Width + 4;
   FrameL.Bottom += 1;
   FrameL.Right = FrameL.Left + WidthLeftGraph + 2;

   // set drawing mode to normal
   GraphCanvas->Pen->Mode = pmCopy;
   // gray frame
   GraphCanvas->Brush->Color = clBlack;
   GraphCanvas->FrameRect(FrameL);

   // note that FillRect and FrameRect draws excluding lower, right corner,
   // so we add one to either side
   FrameB = GraphArea;
   FrameB.Top = FrameB.Bottom + 30;
   FrameB.Bottom = FrameB.Top + HeightBottomGraph +2;
   FrameB.Right += 2;

   // set drawing mode to normal
   GraphCanvas->Pen->Mode = pmCopy;
   // gray frame
   GraphCanvas->Brush->Color = clBlack;
   GraphCanvas->FrameRect(FrameB);


  // Clear the DIB to white
  LeftGraphBM->Width = WidthLeftGraph;
  LeftGraphBM->Height = GD.PixelY;
  SetRect(&ClearArea,0, 0, WidthLeftGraph, GD.PixelY);
  LeftGraphBM->Canvas->Brush->Color = clWhite;
  LeftGraphBM->Canvas->FillRect(ClearArea);


  // Clear the DIB to white
  BottomGraphBM->Width = GD.PixelX;
  BottomGraphBM->Height = HeightBottomGraph;
  SetRect(&ClearArea,0, 0, GD.PixelX, HeightBottomGraph);
  BottomGraphBM->Canvas->Brush->Color = clWhite;
  BottomGraphBM->Canvas->FillRect(ClearArea);

  // find scaling factor
//  double Range = FF.MaxValue -
//                     FF.MinValue;
  double Range = 2 * fabs(Max(GraphMin,GraphMax));

  if (Range == 0) Range = 1;

  Scale = (WidthLeftGraph - 4) / Range;
          CSlant = X;
              LeftGraphBM->Canvas->Pen->Color = clGray;
              LeftGraphBM->Canvas->MoveTo(WidthLeftGraph*0.5,0);
              LeftGraphBM->Canvas->LineTo(WidthLeftGraph*0.5,GD.PixelY);
              LeftGraphBM->Canvas->Pen->Color = clBlue;
              for (CAzi = 0; CAzi < GD.PixelY; CAzi ++)
                {
                  Val = GetValue(DataType, Data1[CSlant][CAzi],
                    Data2[CSlant][CAzi])*Scale;
                  if (CAzi == 0)
                    LeftGraphBM->Canvas->MoveTo(WidthLeftGraph*0.5+Val,CAzi);
                  else
                   LeftGraphBM->Canvas->LineTo(WidthLeftGraph*0.5+Val,CAzi);
               }

  Scale = (HeightBottomGraph - 4) / Range;
      CAzi = Y;
              BottomGraphBM->Canvas->Pen->Color = clGray;
              BottomGraphBM->Canvas->MoveTo(0, HeightBottomGraph*0.5-1);
              BottomGraphBM->Canvas->LineTo(GD.PixelX,HeightBottomGraph*0.5-1);
              BottomGraphBM->Canvas->Pen->Color = clRed;
              for (CSlant = 0; CSlant < GD.PixelX; CSlant ++)
                {
                  Val = GetValue(DataType, Data1[CSlant][CAzi],
                    Data2[CSlant][CAzi])*Scale;
                  if (CSlant == 0)
                    BottomGraphBM->Canvas->MoveTo(CSlant,HeightBottomGraph*0.5-1-(Val));
                  else
                    BottomGraphBM->Canvas->LineTo(CSlant,HeightBottomGraph*0.5-1-(Val));
                } // end for

      LeftGraphBM->IgnorePalette = TRUE;
      Canvas->Draw(Panel1->Width + 4 + 1, GD.GraphArea.Top + 1,LeftGraphBM);
      BottomGraphBM->IgnorePalette = TRUE;
      Canvas->Draw(GD.GraphArea.Left + 1,GD.GraphArea.Bottom + 30 + 1,BottomGraphBM);

      Canvas->Pen->Mode = pmCopy;
      Canvas->Brush->Color = clGray;

      char str[100];
      int TextW, TextH;
      sprintf(str,"%3.3G",-Range*0.5);
      TextW = Canvas->TextWidth(str);
      TextH = Canvas->TextHeight(str);
      Canvas->TextOut(FrameL.Left,FrameL.Bottom + 3,str);
      Canvas->TextOut(FrameB.Left-TextW - 3,FrameB.Bottom - TextH,str);
      sprintf(str,"%3.3G",Range*0.5);
      TextW = Canvas->TextWidth(str);
      TextH = Canvas->TextHeight(str);
      Canvas->TextOut(FrameL.Right-TextW,FrameL.Bottom + 3,str);
      Canvas->TextOut(FrameB.Left-TextW - 3,FrameB.Top,str);
      sprintf(str,"0");
      TextW = Canvas->TextWidth(str);
      TextH = Canvas->TextHeight(str);
      Canvas->TextOut(FrameL.Left + 0.5*(FrameL.Right-FrameL.Left)-0.5*TextW,
      FrameL.Bottom + 3,str);
      Canvas->TextOut(FrameB.Left-TextW - 3,FrameB.Top + 0.5*(FrameB.Bottom - FrameB.Top)-
      TextH * 0.5,str);
 */
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::Graphs1Click(TObject *Sender)
{
  Graphs1->Checked = !Graphs1->Checked;
  if (Graphs1->Checked)
    SideGraphs = true;
  else
    SideGraphs = false;

  Busy = true;
  // set new graph size (changed due to side-graphs)
  SetupGraphSize();
  Busy = false;

  FormResize(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::ColorPalette1Click(TObject *Sender)
{
  LoadPalForm->ShowModal();
  ColorPal = LoadPalForm->Pal;
  MakePalette(ColorPal,127,127,127);
  SetupPaletteBitmap();
  if (FileLoaded)
    {
      UpdateGraphBM();
      UpdateThumbBM();
    }
//  if (RealizeMyPalette(false))
    Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::SaveasBitmap1Click(TObject *Sender)
{
  if (SaveBitmapDialog->Execute())
    {
      Graphics::TBitmap *SaveBM = new Graphics::TBitmap();
      SaveBM->Handle = GraphBM->hDIBSection;
      SaveBM->SaveToFile(SaveBitmapDialog->FileName);
      delete SaveBM;
      SetNewGraphSize();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::RGDataClick(TObject *Sender)
{
  if (RGReal->Checked)
    DataType = 0;
  else if (RGImag->Checked)
    DataType = 1;
  else if (RGMag->Checked)
    DataType = 2;
  else if (RGPhase->Checked)
    DataType = 3;
  else if (RGPower->Checked)
    DataType = 4;
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::Help1Click(TObject *Sender)
{
  Application->HelpJump("simout");
}
//---------------------------------------------------------------------------
