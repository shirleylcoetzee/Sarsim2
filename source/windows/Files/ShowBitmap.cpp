//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "ShowBitmap.h"
#include "main.h"
#include "general.h"
#include "math.h"
#include "Graph.h"
#include "quickload.h"
#include "viewsimloader.h"
#include "palette.h"

// minimum change of range values such that an update occurs
#define MIN_CHANGE 0.9
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TShowBitmapF *ShowBitmapF;
//---------------------------------------------------------------------------
__fastcall TShowBitmapF::TShowBitmapF(TComponent* Owner)
	: TForm(Owner)
{
  GD.XStart = 0;
  GD.XEnd = 10000;
  GD.YStart = 0;
  GD.YEnd = 10000;
  GD.XMin = 0;
  GD.XMax = 10000;
  GD.YMin = 0;
  GD.YMax = 10000;
  GD.UnitsX = 0;
  GD.UnitsY = 0;
  // Left and Right MUST be a multiple of 4
  GD.Border.Left = 254;
  GD.Border.Right = 40;
  GD.Border.Top = 84;
  GD.Border.Bottom = 65;
  GD.Display = 0;
  GD.MaxWindowSize.Left = 450;
  GD.MaxWindowSize.Top = 433;

  // initial size of DIB
  DIBWidth = 4;
  DIBHeight = 4;

  // Finally, set up the DIB section
  pbmiDIB = (BITMAPINFO *)new BYTE[sizeof(BITMAPINFOHEADER) + (256*sizeof(RGBQUAD))];
  pbmiDIB->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  pbmiDIB->bmiHeader.biWidth = DIBWidth;
  pbmiDIB->bmiHeader.biHeight = DIBHeight;
  pbmiDIB->bmiHeader.biPlanes = 1;
  pbmiDIB->bmiHeader.biBitCount = 24;
  pbmiDIB->bmiHeader.biCompression = BI_RGB;
  pbmiDIB->bmiHeader.biSizeImage = 0;
  pbmiDIB->bmiHeader.biXPelsPerMeter = 0;
  pbmiDIB->bmiHeader.biYPelsPerMeter = 0;
  pbmiDIB->bmiHeader.biClrUsed = 0;
  pbmiDIB->bmiHeader.biClrImportant = 0;

  hDIBSection = CreateDIBSection (Canvas->Handle, pbmiDIB, DIB_PAL_COLORS,
                          (void **)&pDIBBase, NULL, 0);
  // if (!hDIBSection) free(pbmiDIB);
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::FormShow(TObject *Sender)
{

  GraphBM = new Graphics::TBitmap();
  //intantiate new graph
  Graph = new class TGraph(Canvas,GD.UnitsX,GD.UnitsY,-2,-2);
  // create bitmap buffer
  DataI = DMatrix(0,0,0,0);
  DataQ = DMatrix(0,0,0,0);


  LeftButtonDown = FALSE;
  RightButtonDown = FALSE;

  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),&(GD.YStart),&(GD.YEnd));
  SetNewGraphSize();

  IsUptodate = FALSE;
  FileLoaded = FALSE;
  CursorCross = FALSE;
  Timer1->Enabled = TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::FormClose(TObject *Sender, TCloseAction &Action)
{
  Timer1->Enabled = FALSE;
  // delete various memory allocated
  Free_DMatrix(DataI,0,0);
  Free_DMatrix(DataQ,0,0);
  delete Graph;
  delete GraphBM;
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::FormPaint(TObject *Sender)
{
  char str[100];

  if (FileLoaded)
    sprintf(str,"SAR Image Viewer : %s",FormLoadSimFile->FileName.c_str());
  else
    sprintf(str,"SAR Image Viewer");

 Caption = str;

   // range
  UpdateRange();
  // draw frame and units
  Graph->Paint(FRAME);
  Graph->Paint(UNITS);

  // draw bitmap

  if (RGDrawData->ItemIndex == 0)
    {
      GraphBM->Handle = hDIBSection;
      GraphBM->IgnorePalette = FALSE;
      GraphBM->Palette = CreatePalette(MainForm->RedBluePalette);
      Canvas->Draw(GD.Border.Left + 1,GD.Border.Top + 1,GraphBM);
      GraphBM->ReleasePalette();
    }
  else
    {
      GraphBM->IgnorePalette = TRUE;
      Canvas->Draw(GD.Border.Left + 1,GD.Border.Top + 1,GraphBM);
    }
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::FormResize(TObject *Sender)
{
  // only allow widths a multiple of 8 such that bitmap width will be a multiple of 4
  Width = (Width + 7) & ~7;

  // put buttons and text in right positions
  BClose->Left = (Width )*0.5 - BClose->Width + 60;
  BClose->Top = ClientHeight - 30;
 // BUpdate->Left = (Width - BUpdate->Width) - 60;
 // BUpdate->Top = ClientHeight - 60;
  LabelSlant->Left = Width - (LabelSlant->Width + 20);
  LabelSlant->Top = ClientHeight - (LabelSlant->Height + 30);
  ZoomInX->Left = GD.Border.Left + (Width - GD.Border.Left - GD.Border.Right) * 0.5
                  - (ZoomInX->Width + 7);
  ZoomOutX->Left = GD.Border.Left + (Width - GD.Border.Left - GD.Border.Right) * 0.5;
  ZoomInY->Left = Width - (ZoomInY->Width + 11);
  ZoomOutY->Left = Width - (ZoomOutY->Width + 11);
  ZoomInY->Top = (GD.Border.Top + (ClientHeight - GD.Border.Bottom - GD.Border.Top) * 0.5) - 15;
  ZoomOutY->Top = (GD.Border.Top + (ClientHeight - GD.Border.Bottom - GD.Border.Top) * 0.5) + 15;

  BRLeft->Left = GD.Border.Left + (Width - GD.Border.Left - GD.Border.Right) * 0.5
                  - (2 * BRLeft->Width + 16);
  BRRight->Left = GD.Border.Left + (Width - GD.Border.Left - GD.Border.Right) * 0.5
                  + ZoomInX->Width + 7;
  BAUp->Left = Width - (BAUp->Width + 11);
  BADown->Left = Width - (BADown->Width + 11);
  BAUp->Top = (GD.Border.Top + (ClientHeight - GD.Border.Bottom - GD.Border.Top) * 0.5) - 44;
  BADown->Top = (GD.Border.Top + (ClientHeight - GD.Border.Bottom - GD.Border.Top) * 0.5) + 44;

  // initialize graph with new window size parameters
  if (Visible)
    {
      SetNewGraphSize();
      IsUptodate = FALSE;
      Invalidate();
    }
}
//---------------------------------------------------------------------------
void TShowBitmapF::UpdateGraphBitmap(void)
{
  double AziRange, SlantRange;
  unsigned long AziP, SlantP;
  int CAzi, CSlant;
  unsigned char byte1,byte2;
  unsigned long TotalSlantP, TotalAziP;
  char str[150];
  double Val;
  class TQuickRead QuickRead;
  #define MAX_VALUE 256

  int PixStep = TBGraph->Position;
  float MultFactor = (float(PixStep) / float(MAX_VALUE)) * 0.9;

  if (!FileLoaded) return;
  if (!QuickRead.OpenFile(FormLoadSimFile->FileName.c_str()))
    {
      sprintf(str,"Cannot open file '%s'",FormLoadSimFile->FileName.c_str());
      Application->MessageBox(str, "Error", MB_OK | MB_ICONEXCLAMATION);
      return;
    }
  TotalSlantP = FormLoadSimFile->SlantPoints;
  TotalAziP = FormLoadSimFile->AziPoints;
  AziRange = GD.YEnd - GD.YStart;
  SlantRange = GD.XEnd - GD.XStart;

  if (RGDrawData->ItemIndex == 0) // draw as color pixels
    {
      double ColorValue;
      double Contrast = 10.0/double(TBContrast->Position);
      double ContrastMult = (255.0/pow(255.0,Contrast));
      double ScaleColor = 1;

      // Clear the DIB
      memset(pDIBBase, 0, 3*DIBWidth*DIBHeight);
      // as bitmaps are shown inverted, values have to be subtracted from size
      unsigned long pDIBBaseInvert = (DIBHeight * DIBWidth * 3) - (DIBWidth * 3);

      for (CAzi = 0; CAzi < DIBHeight; CAzi ++)
        {
          sprintf(str,"Loading file : %3.0f %%",double(CAzi)/double(DIBHeight) * 100.0);
          Caption = str;
          AziP = (unsigned long)round(GD.YEnd - double(CAzi)*(AziRange / double(DIBHeight)));
          if (AziP < TotalAziP)
          for (CSlant = 0; CSlant < DIBWidth; CSlant ++)
            {
  	          SlantP = (unsigned long)round(GD.XStart + double(CSlant)*
		       (SlantRange / double(DIBWidth)));

              if (SlantP < TotalSlantP)
                {
                  QuickRead.ReadBytes((AziP * TotalSlantP * 2) + SlantP * 2, &byte1,
                                  &byte2);
                  DataI[CSlant][CAzi] = (double)byte1;
                  DataQ[CSlant][CAzi] = (double)byte2;
                  Val = GetValue(RGShow->ItemIndex, double(byte1 - 127), double(byte2- 127));
                  if (Val <= 0)
		            {
                      ColorValue = (unsigned long)(pow(-Val * ScaleColor,Contrast)*
                          ContrastMult);
                      pDIBBase[pDIBBaseInvert + (unsigned long)CSlant*3 + 2 -
                         ((unsigned long)CAzi * DIBWidth *3)] = ColorValue;
		            }
	     	      else
		            {
                      ColorValue = (unsigned long)(pow(Val * ScaleColor,Contrast)*
                        ContrastMult  );
                      pDIBBase[pDIBBaseInvert + (unsigned long)CSlant*3 -
                        ((unsigned long)CAzi * DIBWidth *3)] = ColorValue;
		            }
                } // end if
          }
       }
    } // end if item index
  else if (RGDrawData->ItemIndex == 1) // cuts in range
    {
      // Clear the DIB to white
      GraphBM->Width = DIBWidth;
      GraphBM->Height = DIBHeight;
      TRect ClearArea;
      SetRect(&ClearArea,0, 0, DIBWidth, DIBHeight);
      GraphBM->Canvas->Brush->Color = clWhite;
      GraphBM->Canvas->FillRect(ClearArea);
      double SlantPD;
      double SlantStep = ceil((SlantRange * PixStep)/double(DIBWidth));
      if (SlantStep < 1)
        SlantStep = 1;

      for (SlantPD = round(GD.XStart); SlantPD < GD.XEnd; SlantPD += SlantStep)
        {
          sprintf(str,"Loading file : %3.0f %%",((SlantPD-GD.XStart)/SlantRange) * 100.0);
          Caption = str;
          CSlant = (unsigned long)round(((SlantPD - GD.XStart) / SlantRange)*
		        double(DIBWidth));
          if (SlantPD < TotalSlantP)
          {
          GraphBM->Canvas->Pen->Color = clGray;
          GraphBM->Canvas->MoveTo(CSlant,0);
          GraphBM->Canvas->LineTo(CSlant,DIBHeight);
          GraphBM->Canvas->Pen->Color = clRed;
          for (CAzi = 0; CAzi < DIBHeight; CAzi ++)
            {
              AziP = (unsigned long)round(GD.YEnd - double(CAzi)*(AziRange / double(DIBHeight)));
              if (AziP < TotalAziP)
                {
                  QuickRead.ReadBytes((AziP * TotalSlantP * 2) + (unsigned long)SlantPD * 2, &byte1,
                                  &byte2);
                  Val = GetValue(RGShow->ItemIndex, double(byte1 - 127), double(byte2- 127));
                  if (CAzi == 0)
                    GraphBM->Canvas->MoveTo(CSlant+Val*MultFactor,CAzi);
                 // else if (CAzi == DIBHeight-1)
                 //   GraphBM->Canvas->Pixels[CSlant+Val*MultFactor][CAzi] = clRed;
                  else
                    GraphBM->Canvas->LineTo(CSlant+Val*MultFactor,CAzi);
                } // end if
            }
          }
        }
    }
  else if (RGDrawData->ItemIndex == 2) // cuts in azimuth
    {
      // Clear the DIB to white
      GraphBM->Width = DIBWidth;
      GraphBM->Height = DIBHeight;
      TRect ClearArea;
      SetRect(&ClearArea,0, 0, DIBWidth, DIBHeight);
      GraphBM->Canvas->Brush->Color = clWhite;
      GraphBM->Canvas->FillRect(ClearArea);
      double AziPD;
      double AziStep = ceil((AziRange * PixStep)/double(DIBHeight));
      if (AziStep < 1) AziStep = 1;
      for (AziPD = round(GD.YStart); AziPD < GD.YEnd; AziPD += AziStep)
        {
          sprintf(str,"Loading file : %3.0f %%",((AziPD-GD.YStart)/AziRange) * 100.0);
          Caption = str;
          CAzi = (unsigned long)round(((AziPD - GD.YStart)/AziRange)* double(DIBHeight));
          if (AziPD < TotalAziP)
          {
          GraphBM->Canvas->Pen->Color = clGray;
          GraphBM->Canvas->MoveTo(0, DIBHeight - 1 - CAzi);
          GraphBM->Canvas->LineTo(DIBWidth,DIBHeight - 1 - CAzi);
          GraphBM->Canvas->Pen->Color = clRed;
          for (CSlant = 0; CSlant < DIBWidth; CSlant ++)
            {
              SlantP = (unsigned long)round(GD.XStart + double(CSlant)*
		        (SlantRange / double(DIBWidth)));
              if (SlantP < TotalSlantP)
                {
                  QuickRead.ReadBytes(((unsigned long)AziPD * TotalSlantP * 2) + SlantP * 2, &byte1,
                                  &byte2);
                  Val = GetValue(RGShow->ItemIndex, double(byte1 - 127), double(byte2- 127));
                  if (CSlant == 0)
                    GraphBM->Canvas->MoveTo(CSlant,DIBHeight - 1 - (CAzi+Val*MultFactor));
                  else
                    GraphBM->Canvas->LineTo(CSlant,DIBHeight - 1 - (CAzi+Val*MultFactor));

                } // end if
            }
          }
        }
    }
  QuickRead.CloseFile();
  IsUptodate = TRUE;
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::RGShowClick(TObject *Sender)
{
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::RestrictSize(TMessage& Msg)
{
  // set the limits of the graph windows size
  ((POINT far *)Msg.LParam)[3].x = GD.MaxWindowSize.Left;
  ((POINT far *)Msg.LParam)[3].y = GD.MaxWindowSize.Top;
  TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::FormKeyPress(TObject *Sender, char &Key)
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
void __fastcall TShowBitmapF::FormMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if ((Button == mbLeft) && (!LeftButtonDown))
	{
      // clicked in graph area ?
	  if (PointInRect(Graph->GraphArea,X,Y))
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
	  if (PointInRect(Graph->GraphArea,X,Y))
        {
		  double Xtemp,Ytemp;
	      // convert window (pixel) coordinates to actual values
		  WindowToReal(X, Y, &Xtemp, &Ytemp);
		  GD.XStart = Xtemp - Graph->XRange;
		  GD.YStart = Ytemp - Graph->YRange;
		  GD.XEnd = Xtemp + Graph->XRange;
		  GD.YEnd = Ytemp + Graph->YRange;
          SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                           &(GD.YStart),&(GD.YEnd));
          UpdateGraphBitmap();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::FormMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
  if (!LeftButtonDown)
    {
     // TRect GraphArea;
     // SetRect(&GraphArea,GD.Border.Left, GD.Border.Top, GD.Border.Left + DIBWidth,
     //         GD.Border.Top + DIBHeight);
      if (PointInRect(GraphArea, X, Y))
        {
          if (!CursorCross)
            {
              Screen->Cursor = crCross;
              CursorCross = TRUE;
            }
          double Xtemp,Ytemp;
          char str[30];
          WindowToReal(X, Y, &Xtemp, &Ytemp);
          sprintf(str,"%d",(long)round(Xtemp));
          if (strlen(str) > 7) str[7]='\0';
          LX->Caption = str;
          sprintf(str,"%d",(long)round(Ytemp));
          if (strlen(str) > 7) str[7]='\0';
          LY->Caption = str;
          if (IsUptodate && (RGDrawData->ItemIndex == 0))
            {
              LIn->Caption = FloatToStr(DataI[X-GD.Border.Left][Y-GD.Border.Top]-127);
              LQuad->Caption = FloatToStr(DataQ[X-GD.Border.Left][Y-GD.Border.Top]-127);
            }
          else
            {
              LIn->Caption = "";
              LQuad->Caption = "";
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
	  if (X <= Graph->GraphArea.Left) X = Graph->GraphArea.Left + 1;
	  if (X > Graph->GraphArea.Right) X = Graph->GraphArea.Right;
	  if (Y <= Graph->GraphArea.Top) Y = Graph->GraphArea.Top + 1;
	  if (Y > Graph->GraphArea.Bottom) Y = Graph->GraphArea.Bottom;
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
void __fastcall TShowBitmapF::FormMouseUp(TObject *Sender, TMouseButton Button,
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
	  WindowToReal(ZoomRect.Left, ZoomRect.Bottom, &(GD.XStart),
        &(GD.YStart));
	  WindowToReal(ZoomRect.Right, ZoomRect.Top, &(GD.XEnd),
        &(GD.YEnd));
      if ( (GD.XStart != GD.XEnd) &&
           (GD.YStart != GD.YEnd) )
        {
          // initialize graph with new window size parameters
          SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                           &(GD.YStart),&(GD.YEnd));
          UpdateGraphBitmap();
        }
	}
  if ((Button == mbRight) && RightButtonDown)
    RightButtonDown = FALSE;
}
//---------------------------------------------------------------------------
void TShowBitmapF::SetNewGraphRange(double *XStart, double *XEnd,
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
  // update graph class (keep old window rect size)
  Graph->Init(Graph->GraphArea,*XStart,*XEnd,*YStart,*YEnd);
  // show new values in edit boxes
  UpdateRange();
  IsUptodate = FALSE;

}
//---------------------------------------------------------------------------
void TShowBitmapF::UpdateRange(void)
{
  char str[80];
  // setup window range values
  sprintf(str,"%.*f",0,Graph->XStart);
  EXStart->Text = str;
  sprintf(str,"%.*f",0,Graph->XEnd);
  EXEnd->Text = str;
  sprintf(str,"%.*f",0,Graph->YStart);
  EYStart->Text = str;
  sprintf(str,"%.*f",0,Graph->YEnd);
  EYEnd->Text = str;
/*
  EXStart->Invalidate();
  EXEnd->Invalidate();
  EYStart->Invalidate();
  EYEnd->Invalidate();
*/
}
//---------------------------------------------------------------------------
void TShowBitmapF::SetNewGraphSize(void)
{

  TRect SetBorder;
  // calculate new border
  SetBorder = GD.Border;
  SetBorder.Right = Width - GD.Border.Right - 1;
  SetBorder.Bottom = ClientHeight - GD.Border.Bottom - 1;

  // update rect, leave range unchanged
  Graph->Init(SetBorder,Graph->XStart,Graph->XEnd,Graph->YStart,Graph->YEnd);

  // have to reset bitmap (works fine without on NT but not 95)
  delete GraphBM;
  GraphBM = new Graphics::TBitmap();

  // set new size of DIB
  DIBWidth = ((Width - GD.Border.Right - GD.Border.Left - 2) + 3) & ~3;
  DIBHeight = ClientHeight - GD.Border.Bottom - GD.Border.Top - 2;
  pbmiDIB->bmiHeader.biWidth = DIBWidth;
  pbmiDIB->bmiHeader.biHeight = DIBHeight;

  DeleteObject(hDIBSection);
  hDIBSection = CreateDIBSection (Canvas->Handle, pbmiDIB, DIB_PAL_COLORS,
                          (void **)&pDIBBase, NULL, 0);
  // Clear the DIB
  memset(pDIBBase, 0, 3*DIBWidth*DIBHeight);

  Free_DMatrix(DataI,0,0);
  Free_DMatrix(DataQ,0,0);
  DataI = DMatrix(0,DIBWidth-1,0,DIBHeight-1);
  DataQ = DMatrix(0,DIBWidth-1,0,DIBHeight-1);

  SetRect(&GraphArea,GD.Border.Left, GD.Border.Top, GD.Border.Left + DIBWidth,
              GD.Border.Top + DIBHeight);

  IsUptodate = FALSE;
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::EXStartExit(TObject *Sender)
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
void __fastcall TShowBitmapF::EXEndExit(TObject *Sender)
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
void __fastcall TShowBitmapF::EYStartExit(TObject *Sender)
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
void __fastcall TShowBitmapF::EYEndExit(TObject *Sender)
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
void TShowBitmapF::WindowToReal(int PixX, int PixY, double *X, double *Y)
{
	*X = Graph->XStart + double(PixX - Graph->GraphArea.Left)*
		(Graph->XRange / double(Graph->PixelX));
	*Y = Graph->YEnd - double(PixY - Graph->GraphArea.Top)*
		(Graph->YRange / double(Graph->PixelY));
}
//---------------------------------------------------------------------------
void TShowBitmapF::RealToWindow(double X, double Y, int *PixX, int *PixY)
{
  *PixX = (X - Graph->XStart)*(double(Graph->PixelX)/Graph->XRange);
  *PixY = (Graph->YEnd - Y)*(double(Graph->PixelY)/Graph->YRange);
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::BCloseClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::ZoomInXClick(TObject *Sender)
{
  GD.XStart = GD.XStart + 0.25 * Graph->XRange;
  GD.XEnd = GD.XEnd - 0.25 * Graph->XRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::ZoomOutXClick(TObject *Sender)
{
  GD.XStart = GD.XStart - 0.5 * Graph->XRange;
  GD.XEnd = GD.XEnd + 0.5 * Graph->XRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::ZoomInYClick(TObject *Sender)
{
  GD.YStart = GD.YStart + 0.25 * Graph->YRange;
  GD.YEnd = GD.YEnd - 0.25 * Graph->YRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::ZoomOutYClick(TObject *Sender)
{
  GD.YStart = GD.YStart - 0.5 * Graph->YRange;
  GD.YEnd = GD.YEnd + 0.5 * Graph->YRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::BAllClick(TObject *Sender)
{
  unsigned long TotalSlantP, TotalAziP;

  TotalSlantP = FormLoadSimFile->SlantPoints;
  TotalAziP = FormLoadSimFile->AziPoints;

  GD.XMax = TotalSlantP - 1;
  GD.YMax = TotalAziP - 1;
  GD.XStart = 0;
  GD.XEnd = GD.XMax;
  GD.YStart = 0;
  GD.YEnd = GD.YMax;
  // initialize graph with new window size parameters
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd), &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::Timer1Timer(TObject *Sender)
{
  if ((!IsUptodate) && FileLoaded)
    UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::TBContrastChange(TObject *Sender)
{
  IsUptodate = FALSE;
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::BRLeftClick(TObject *Sender)
{
  GD.XStart = GD.XStart - 0.25 * Graph->XRange;
  GD.XEnd = GD.XEnd - 0.25 * Graph->XRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::BRRightClick(TObject *Sender)
{
  GD.XStart = GD.XStart + 0.25 * Graph->XRange;
  GD.XEnd = GD.XEnd + 0.25 * Graph->XRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::BAUpClick(TObject *Sender)
{
  GD.YStart = GD.YStart + 0.25 * Graph->YRange;
  GD.YEnd = GD.YEnd + 0.25 * Graph->YRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::BADownClick(TObject *Sender)
{
  GD.YStart = GD.YStart - 0.25 * Graph->YRange;
  GD.YEnd = GD.YEnd - 0.25 * Graph->YRange;
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd),
                  &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::Exit1Click(TObject *Sender)
{
  Close();	
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::FopenClick(TObject *Sender)
{
  if (FormLoadSimFile->ShowModal() == mrOk)
    {
  unsigned long TotalSlantP, TotalAziP;

  TotalSlantP = FormLoadSimFile->SlantPoints;
  TotalAziP = FormLoadSimFile->AziPoints;

  GD.XMax = TotalSlantP - 1;
  GD.YMax = TotalAziP - 1;
  GD.XStart = 0;
  GD.XEnd = GD.XMax;
  GD.YStart = 0;
  GD.YEnd = GD.YMax;
  // initialize graph with new window size parameters
  SetNewGraphRange(&(GD.XStart),&(GD.XEnd), &(GD.YStart),&(GD.YEnd));
  UpdateGraphBitmap();
  FileLoaded = TRUE;
    }
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::RGDrawDataClick(TObject *Sender)
{
  if (RGDrawData->ItemIndex == 0)
    {
      TBContrast->Enabled = TRUE;
      TBGraph->Enabled = FALSE;
    }
  else if (RGDrawData->ItemIndex == 1)
    {
      TBContrast->Enabled = FALSE;
      TBGraph->Enabled = TRUE;
    }
  else if (RGDrawData->ItemIndex == 2)
    {
      TBContrast->Enabled = FALSE;
      TBGraph->Enabled = TRUE;

    }

  UpdateGraphBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::TBGraphChange(TObject *Sender)
{
  IsUptodate = FALSE;
}
//---------------------------------------------------------------------------
void __fastcall TShowBitmapF::Palette1Click(TObject *Sender)
{
  PaletteForm->ShowModal();	
}
//---------------------------------------------------------------------------
