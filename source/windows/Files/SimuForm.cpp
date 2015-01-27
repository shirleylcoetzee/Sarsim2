//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "SimuForm.h"
#include "Graph.h"
#include "math.h"
#include "general.h"
#include "objectstructdef.h"
#include "engine.h"
#include "linkedlist.h"
#include "main.h"
#include "SaveSimData.h"
#include "selectpal.h"
#include "quikgeneral.h"

extern bool UserAbort;
extern int CalcProgress;
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TSimulationForm *SimulationForm;
//---------------------------------------------------------------------------
__fastcall TSimulationForm::TSimulationForm(TComponent* Owner)
	: TForm(Owner)
{
  XMin = 0; XMax = MAX_FLOAT; YMin = MIN_FLOAT; YMax = MAX_FLOAT;
  // define borders (pixel space between graph and window on each side)
  // mod((Left - Right),4) =2 : Borders must be a multiple of 4
  Border.Left = Panel1->Width + 70;//276;
  Border.Left = ((Border.Left + 3) & ~3);
  Border.Right = ((BAUp->Width + 12 + 3) & ~3) + 2; //34;
  Border.Top = BRLeft->Top + BRLeft->Height + 7;//54;
  Border.Bottom = ClientHeight - Panel2->Top + 40;

  // create palette (Cpal)
  ColorPal = RED_BLUE_SCALE;
  MakePalette(ColorPal,127,127,127);
  int temp;
  temp = (Bevel2->Width - 4)& ~3;
  PalBM = new class TBitM(Handle, temp, Bevel2->Height - 3);    // 200 18
  GraphBM2 = new class TBitM(Handle, 4, 4);
  // this one used for cross-sections
  LGraphBM = new Graphics::TBitmap();
  PixelX = 4;
  PixelY = 4;
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::RestrictSize(TMessage& Msg)
{
  // set the limits of the graph windows size
  ((POINT far *)Msg.LParam)[3].x = 488;
  ((POINT far *)Msg.LParam)[3].y = 537;
  ((POINT far *)Msg.LParam)[4].x = 2000;
  ((POINT far *)Msg.LParam)[4].y = 2000;
  TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::FormClose(TObject *Sender,
	TCloseAction &Action)
{
  Timer1->Enabled = FALSE;
  Free_DMatrix(Data,0,0);
  Free_DVector(PulseSendTime,0);
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::FormMouseDown(TObject *Sender,
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
      Canvas->Brush->Color = clWhite;
	        Canvas->Pen->Mode = pmMergePenNot;//	pmNot;
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
 		  CurrentSim.SlantStart = Xtemp - Graph->XRange;
		  CurrentSim.AzimuthStart = Ytemp - Graph->YRange;
		  CurrentSim.SlantEnd = Xtemp + Graph->XRange;
		  CurrentSim.AzimuthEnd = Ytemp + Graph->YRange;
          // busy updating -> quit that
          if (Busy) StopThread();
          FastUpdate();

          SetNewGraphRange(&(CurrentSim.SlantStart),&(CurrentSim.SlantEnd),
                           &(CurrentSim.AzimuthStart),&(CurrentSim.AzimuthEnd));
          Graph->Paint(UNITS);
          UpdateGraphBitmap(FALSE);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::FormMouseMove(TObject *Sender,
	TShiftState Shift, int X, int Y)
{

  if (!LeftButtonDown)
    {
      // in graph area ?
	    if (PointInRect(Graph->GraphArea,X,Y))
        {
          CursorInGraph = true;

          // display coordinates
          double Xtemp,Ytemp;
          char str[30];
          long CX,CY;
          WindowToReal(X, Y, &Xtemp, &Ytemp);
          sprintf(str,"%10.6G m",Xtemp);
          LX->Caption = str;
          sprintf(str,"%10.6G s",Ytemp);
          LY->Caption = str;
          if (!IsDirtyNewCalc && !Busy)
            {
              CX = (X-1-Graph->GraphArea.Left);
              CY = GraphBM2->DIBHeight-1-(Y-1-Graph->GraphArea.Top);
              if (PulseNo != GraphBM2->DIBHeight)
                CY = (double(CY)/double(GraphBM2->DIBHeight))*
                  (double)PulseNo;
              // check (maybe rounding errors)
              if (CX >= GraphBM2->DIBWidth) CX = GraphBM2->DIBWidth - 1;
              if (CY >= PulseNo) CY = PulseNo - 1;
              if (CX < 0) CX = 0;
              if (CY < 0) CY = 0;

                  sprintf(str,"%-10.6G V",Data[CY][CX*2]);
                  LIn->Caption = str;
                  sprintf(str,"%-10.6G V",Data[CY][CX*2+1]);
                  LQuad->Caption = str;
                  sprintf(str,"%-8.6G deg",GetValue(PHASE, Data[CY][CX*2],
                    Data[CY][CX*2+1])*RadToDeg);
                  LPh->Caption = str;

              if (ShowdB)
                {
                  double D;
                  D = FindValuedB(GetValue(MAGNITUDE,Data[CY][CX*2],
                      Data[CY][CX*2+1]), PMax);
                  if (D == -DYNAMIC_RANGE)
                    sprintf(str,"0",D);
                  else
                    sprintf(str,"%-9.6G dBW",D);
                  LMag->Caption = str;
                }
              else
                {
                  sprintf(str,"%-10.6G V",GetValue(MAGNITUDE,Data[CY][CX*2],
                     Data[CY][CX*2+1]));
                  LMag->Caption = str;
                }
            }
          else
            {
              LIn->Caption = "";
              LQuad->Caption = "";
              LMag->Caption = "";
              LPh->Caption = "";
            }
        } // end if PointInRect
      else
        {
          // cursor was move out of graph area - clear info
          if (CursorInGraph)
            {
              LX->Caption = "";
              LY->Caption = "";
              LIn->Caption = "";
              LQuad->Caption = "";
              LMag->Caption = "";
              LPh->Caption = "";
              CursorInGraph = false;
            }
        }
    }  // end if LeftButtonDown

  if (LeftButtonDown)
	{
      // set drawing mode to inverted
      Canvas->Pen->Mode = pmNot;
      Canvas->Brush->Color = clWhite;
      Canvas->Pen->Mode = pmWhite;//	pmNot;
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
      Canvas->Brush->Color = clWhite;
  	  Canvas->DrawFocusRect(ZoomRect);
	 }
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::FormMouseUp(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if ((Button == mbLeft) && LeftButtonDown)
	{
	  // set drawing mode to inverted
    Canvas->Brush->Color = clWhite;
    Canvas->Pen->Mode = pmWhite;
      // erase rect
  	  Canvas->DrawFocusRect(ZoomRect);
	  // release mouse
      ReleaseCapture();
	  LeftButtonDown = FALSE;
	  // convert window (pixel) coordinates to actual values
	  WindowToReal(ZoomRect.Left, ZoomRect.Bottom, &(CurrentSim.SlantStart),
        &(CurrentSim.AzimuthStart));
	  WindowToReal(ZoomRect.Right, ZoomRect.Top, &(CurrentSim.SlantEnd),
        &(CurrentSim.AzimuthEnd));
      if ( (CurrentSim.SlantStart != CurrentSim.SlantEnd) &&
           (CurrentSim.AzimuthStart != CurrentSim.AzimuthEnd) )
        {
          // busy updating -> quit that
          if (Busy) StopThread();
          FastUpdate();

          // initialize graph with new window size parameters
          SetNewGraphRange(&(CurrentSim.SlantStart),&(CurrentSim.SlantEnd),
                           &(CurrentSim.AzimuthStart),&(CurrentSim.AzimuthEnd));
          Graph->Paint(UNITS);
          UpdateGraphBitmap(FALSE);
        }
	}
  if ((Button == mbRight) && RightButtonDown)
    RightButtonDown = FALSE;

}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::FormPaint(TObject *Sender)
{
  // palette
  PalBM->Draw(Canvas,Handle,Panel6->Width+1,Panel1->Height, Cpal); //346

  // range
  UpdateRange();
  // draw frame and units
  Graph->Paint(FRAME);
  Graph->Paint(UNITS);
  // show graph bitmap
  if ((!BusyWithBitmap) && !IsDirtyNewCalc && !IsDirtyRedraw)
    {
      // draw graph bitmap
      // (in auto mode only draw line graphs if pulses are PixStep or
      // more pixels apart)
      if ((RGDrawData->ItemIndex == 0) || ((RGDrawData->ItemIndex == 3) &&
      ((double(PixelY)/double(PulseNo))<(double)PixStep))) // colour pixels
        {
          GraphBM2->Draw(Canvas, Handle, Border.Left + 1, Border.Top + 1, Cpal);
        }
      else // crossections
        {
          LGraphBM->IgnorePalette = TRUE;
          Canvas->Draw(Border.Left + 1, Border.Top + 1,LGraphBM);
        }
      // label color palette  
      char str[30];
      if (ShowdB)
        sprintf(str,"%d",-DYNAMIC_RANGE);
      else
        sprintf(str,"%5.3G",PMin);
      Label4->Caption = str;
      if (ShowdB)
        sprintf(str,"%d",(int)(double(-DYNAMIC_RANGE)*0.5));
      else
        {
          sprintf(str,"%5.3G",(PMin+PMax)*0.5);
          if (fabs(PMin+PMax)*100 < PMax)
            sprintf(str,"0");
        }
      Label15->Left = (Panel4->Width - Canvas->TextWidth(str))*0.5;
      Label15->Caption = str;
      if (ShowdB)
        sprintf(str,"%d",0);
      else
        sprintf(str,"%5.3G",PMax);
      Label7->Left = Panel4->Width - Canvas->TextWidth(str) - 12;
      Label7->Caption = str;
    }
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::FormResize(TObject *Sender)
{
  // only allow widths a multiple of 8 such that bitmap width will be a multiple of 4
  ClientWidth = (ClientWidth + 7) & ~7;

  // center of open space right of panel1
  int CenterImage = (ClientWidth + Panel1->Width) * 0.5;

  // put buttons and text in right positions
  // progress bar
  Panel3->Width = ClientWidth - Panel1->Width - Image1->Width - 22;
  ProgressBar->Width = Panel3->Width - 7;
  Image1->Left = ClientWidth - Image1->Width - 6;

  // zoom and pan buttons
  ZoomInX->Left = Border.Left + (ClientWidth - Border.Left - Border.Right) * 0.5
                  - (ZoomInX->Width + 12);
  ZoomOutX->Left = Border.Left + (ClientWidth - Border.Left - Border.Right) * 0.5 - 5;
  BRLeft->Left = ZoomInX->Left - BRLeft->Width - 5;
  BRRight->Left = ZoomOutX->Left + ZoomOutX->Width + 5;

  ZoomInY->Left = ClientWidth - (ZoomInY->Width + 3);
  ZoomOutY->Left = ClientWidth - (ZoomOutY->Width + 3);
  BAUp->Left = ClientWidth - (ZoomInY->Width + 3);
  BADown->Left = ClientWidth - (ZoomInY->Width + 3);

  ZoomInY->Top = (Border.Top + (ClientHeight - Border.Bottom - Border.Top) * 0.5) - 15;
  ZoomOutY->Top = (Border.Top + (ClientHeight - Border.Bottom - Border.Top) * 0.5) + 15;
  BAUp->Top = ZoomInY->Top - ZoomInY->Height - 5;
  BADown->Top = ZoomOutY->Top + ZoomOutY->Height + 5;

  // label
  LabelSlant->Left = ClientWidth - (LabelSlant->Width + 10);   //abc
  LabelSlant->Top = ClientHeight - (Border.Bottom - 21);

  // info panel
  Panel2->Top = ClientHeight - Panel2->Height - BSaveData->Height - 24;//154;
  Panel2->Left = CenterImage - Panel2->Width*0.5;

  // buttons
  BSaveData->Top = ClientHeight - 36;
  BBitmap->Top = ClientHeight - 36;
  BClose->Top = ClientHeight - 36;
  BHelp->Top = ClientHeight - 36;

  BSaveData->Left = ClientWidth * 0.5 - BSaveData->Width - 30 - BBitmap->Width ;
  BBitmap->Left = ClientWidth * 0.5 - BBitmap->Width - 15;
  BClose->Left = ClientWidth * 0.5  + 15 ;
  BHelp->Left = ClientWidth * 0.5 + BClose->Width + 30;

  if (Busy)
    {
      if (Busy) StopThread();
      SetNewGraphSize();
    }
  else
    {
      // initialize graph with new window size parameters
      SetNewGraphSize();
      IsDirtyNewCalc = TRUE;
    }
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::FormShow(TObject *Sender)
{
  // TRUE if user wants to close window
  WantToClose = FALSE;
  // if we showing raw return, sampling frequency is
  // always at least as high as necessary
  if (CurrentSim.SimTYPE == RAW_RETURN)
    {
      EditSampleF->Text = "oo";
      EditSampleF->Enabled = FALSE;
    }
  // estimated maximum magnitude
  MaxMag = 0;
  // running for first time -> therefore set range such that all targets
  // are shown
  if ((CurrentSim.SlantStart) == 0 && (CurrentSim.SlantEnd == 0))
    AutoSlantRange = TRUE;
  // no thread is running
  Busy = FALSE;
  // not terminated
  Terminat = FALSE;
  // graph needs updating
  IsDirtyNewCalc = TRUE;
  IsDirtyRedraw = false;
  // modyfying GraphBM ?
  BusyWithBitmap = FALSE;
  //intantiate new graph
  Graph = new class TGraph(Canvas,1,2,-2,-2);
  // create bitmap buffer
  LeftButtonDown = FALSE;
  RightButtonDown = FALSE;
  if (CurrentSim.SimTYPE == RAW_RETURN)
    Caption = "Raw Return Simulation";
  else if (CurrentSim.SimTYPE == MATCHED_FILTER)
    Caption = "Range Compressed Simulation";

  // setup radar list box and find number of selected radar
  int Sel=0,Counter=0;
  struct SRadar *R;
  R = MainForm->FirstRadar;
  // in case name not defined choose first radar
  CRadar =  MainForm->FirstRadar;
  while (R != NULL)
	{
	  if (stricmp(R->RadarName, CurrentSim.RadarName) == 0)
	    { Sel = Counter; CRadar = R; }
	  RadarList->Items->Add(R->RadarName);
	  R = R->next;
	  Counter++;
	}
  strcpy(CurrentSim.RadarName, CRadar->RadarName);

  PulsePos->ItemIndex = CurrentSim.PTPos;
  RGDisplay->ItemIndex = CurrentSim.Display;
  // set sampling frequency
  if (CurrentSim.SampleFreq == 0)
    CurrentSim.SampleFreq = FindNyquistRate(CRadar);
  if (CurrentSim.SimTYPE != RAW_RETURN)
    EditSampleF->Text = FloatToStr(CurrentSim.SampleFreq /
                          FindNyquistRate(CRadar));
  // select by default the first radar
  RadarList->ItemIndex = Sel;

  SetNewGraphRange(&(CurrentSim.SlantStart),&(CurrentSim.SlantEnd),
            &(CurrentSim.AzimuthStart),&(CurrentSim.AzimuthEnd));
  SetNewGraphSize();
  Timer1->Enabled = TRUE;
  // set data to some arbitary value
  Data = DMatrix(0,1,0,1);
  PulseSendTime = DVector(0,1);
  // cursor not in graph area
  CursorInGraph = false;
  SetupPaletteBitmap();
  // don't show in dB
  ShowdB = false;
  FinishedInitializing = TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::EXStartExit(TObject *Sender)
{
  if (CurrentSim.SlantStart != atof(EXStart->Text.c_str()))
    {
      // busy updating -> quit that
      if (Busy) StopThread();
      CurrentSim.SlantStart =  atof(EXStart->Text.c_str());
      SetNewGraphRange(&(CurrentSim.SlantStart),&(CurrentSim.SlantEnd),
                       &(CurrentSim.AzimuthStart),&(CurrentSim.AzimuthEnd));
      UpdateGraphBitmap(FALSE);
    }
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::EXEndExit(TObject *Sender)
{
  if (CurrentSim.SlantEnd != atof(EXEnd->Text.c_str()))
    {
  // busy updating -> quit that
  if (Busy) StopThread();
  CurrentSim.SlantEnd =  atof(EXEnd->Text.c_str());
  SetNewGraphRange(&(CurrentSim.SlantStart),&(CurrentSim.SlantEnd),
                  &(CurrentSim.AzimuthStart),&(CurrentSim.AzimuthEnd));
  UpdateGraphBitmap(FALSE);
    }
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::EYStartExit(TObject *Sender)
{
  if (CurrentSim.AzimuthStart !=  atof(EYStart->Text.c_str()))
    {
  // busy updating -> quit that
  if (Busy) StopThread();
  CurrentSim.AzimuthStart =  atof(EYStart->Text.c_str());
  SetNewGraphRange(&(CurrentSim.SlantStart),&(CurrentSim.SlantEnd),
                  &(CurrentSim.AzimuthStart),&(CurrentSim.AzimuthEnd));
  UpdateGraphBitmap(FALSE);
    }
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::EYEndExit(TObject *Sender)
{
  if (CurrentSim.AzimuthEnd != atof(EYEnd->Text.c_str()))
    {
  // busy updating -> quit that
  if (Busy) StopThread();
  CurrentSim.AzimuthEnd =  atof(EYEnd->Text.c_str());
  SetNewGraphRange(&(CurrentSim.SlantStart),&(CurrentSim.SlantEnd),
                  &(CurrentSim.AzimuthStart),&(CurrentSim.AzimuthEnd));
  UpdateGraphBitmap(FALSE);
    }
}
//---------------------------------------------------------------------------
void TSimulationForm::WindowToReal(int PixX, int PixY, double *X, double *Y)
{
	*X = Graph->XStart + double(PixX - Graph->GraphArea.Left)*
		(Graph->XRange / double(Graph->PixelX));
	*Y = Graph->YEnd - double(PixY - Graph->GraphArea.Top)*
		(Graph->YRange / double(Graph->PixelY));
}
//---------------------------------------------------------------------------
void TSimulationForm::RealToWindow(double X, double Y, int *PixX, int *PixY)
{
  *PixX = (X - Graph->XStart)*(double(Graph->PixelX)/Graph->XRange);
  *PixY = (Graph->YEnd - Y)*(double(Graph->PixelY)/Graph->YRange);
}
//---------------------------------------------------------------------------
void TSimulationForm::AdjustGraphRangeValues(double *XStart, double *XEnd,
                                       double *YStart, double *YEnd)
{
  // do some range checking
  if (*XStart <  XMin) *XStart = XMin;
  if (*YStart <  YMin) *YStart = YMin;
  if (*XEnd >  XMax) *XEnd = XMax;
  if (*YEnd >  YMax) *YEnd = YMax;
  if (*XStart > *XEnd) Swap(XStart,XEnd);
  if (*YStart > *YEnd) Swap(YStart,YEnd);
  if (*XStart == *XEnd) *XEnd = *XStart + 1;
  if (*YStart == *YEnd) *YEnd = *YStart + 1;

}
//---------------------------------------------------------------------------
void TSimulationForm::SetNewGraphRange(double *XStart, double *XEnd,
                                       double *YStart, double *YEnd)
{
  AdjustGraphRangeValues(XStart, XEnd, YStart, YEnd);
  // update graph class (keep old window rect size)
  Graph->Init(Graph->GraphArea,*XStart,*XEnd,*YStart,*YEnd);
  // show new values in edit boxes
  UpdateRange();
  Refresh();
}
//---------------------------------------------------------------------------
void TSimulationForm::SetNewGraphSize(void)
{
  TRect SetBorder;
  // calculate new border
  SetBorder = Border;
  SetBorder.Right = ClientWidth - Border.Right - 1;
  SetBorder.Bottom = ClientHeight - Border.Bottom - 1;

  // update rect, leave range unchanged
  Graph->Init(SetBorder,Graph->XStart,Graph->XEnd,Graph->YStart,Graph->YEnd);
  PixelX = ClientWidth - Border.Right - Border.Left - 2;
  PixelY = ClientHeight - Border.Bottom - Border.Top - 2;
  GraphBM2->Resize(Handle, PixelX, PixelY);
}
//---------------------------------------------------------------------------
void TSimulationForm::UpdateRange(void)
{
  char str[80];
  // setup window range values
  sprintf(str,"%.*f",3,Graph->XStart);
  EXStart->Text = str;
  sprintf(str,"%.*f",3,Graph->XEnd);
  EXEnd->Text = str;
  sprintf(str,"%.*f",6,Graph->YStart);
  EYStart->Text = str;
  sprintf(str,"%.*f",6,Graph->YEnd);
  EYEnd->Text = str;

  EXStart->Invalidate();
  EXEnd->Invalidate();
  EYStart->Invalidate();
  EYEnd->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::BShowAllClick(TObject *Sender)
{
  // busy updating -> quit that
  if (Busy) StopThread();
  AutoSlantRange = TRUE;
  UpdateGraphBitmap(TRUE);
}
//---------------------------------------------------------------------------
void TSimulationForm::DrawBitmap(TCanvas *ThisCanvas, double **ThisData,
               double *ThisPulseSendTime, unsigned long ThisPulseNo)
{
      // just draw a frame to inform user about delay
      Canvas->Brush->Color = clBtnFace;
      Canvas->Brush->Style = bsSolid;
      Canvas->Pen->Color = clYellow;
      Canvas->Pen->Width = 2;
      Canvas->Pen->Style = psSolid;
      Canvas->Font->Size = 14;
      Canvas->Font->Color = clBlack;
      Canvas->Rectangle(Border.Left + GraphBM2->DIBWidth * 0.5 -
         0.5 * Canvas->TextWidth("Copying Image") - 10, Border.Top +
         GraphBM2->DIBHeight * 0.5 - 0.5 * Canvas->TextHeight("Copying Image")-10,
         Border.Left + GraphBM2->DIBWidth * 0.5 +
         0.5 * Canvas->TextWidth("Copying Image") + 10, Border.Top +
         GraphBM2->DIBHeight * 0.5 + 0.5 * Canvas->TextHeight("Copying Image")+10);
       Canvas->TextOut(Border.Left + GraphBM2->DIBWidth * 0.5 -
         0.5 * Canvas->TextWidth("Copying Image") , Border.Top +
         GraphBM2->DIBHeight * 0.5 - 0.5 * Canvas->TextHeight("Copying Image"),"Copying Image");
      BusyWithBitmap = TRUE;
      Canvas->Refresh();
      // draw graph bitmap
      DrawArray((int)RGDrawData->ItemIndex, LGraphBM, GraphBM2, ThisData,
                ThisPulseSendTime, ThisPulseNo, GraphBM2->DIBWidth,
                GraphBM2->DIBHeight, &CurrentSim, (int)SBContrast->Position,
                (int)SBBright->Position,(int)SBSat->Position,
                 ColPal,ShowdB);
      BusyWithBitmap = FALSE;
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::ThreadDone(TObject * )
{
  if (Terminat) // means that thread was terminated
    {
      Terminat = FALSE;
      IsDirtyNewCalc = TRUE;
      MaxMag = 0;
      Label6->Caption = "Unknown";
    }
  else  // thread finished its calculations
    {
      // show the data on the screen
      Screen->Cursor = crHourGlass;

      DrawBitmap(Canvas, AThread->Data, AThread->PulseSendTime,
        AThread->PulseNo);

      // copy data into local array for info panel
      // allocate memory for data array (I and Q therfore SamplePoints*2)
      Free_DMatrix(Data,0,0);
     	Data = DMatrix(0,AThread->PulseNo-1,0,GraphBM2->DIBWidth*2-1);
      Free_DVector(PulseSendTime,0);
 		  PulseSendTime = DVector(0,AThread->PulseNo-1);

      // copy data across
      long i,j;
      PulseNo = AThread->PulseNo;
      for (i=0;i<AThread->PulseNo;i++)
        {
          PulseSendTime[i] = AThread->PulseSendTime[i];
          for (j=0;j< GraphBM2->DIBWidth*2;j++)
            Data[i][j] = AThread->Data[i][j];
        }
      FindMinMax();

      Screen->Cursor = crDefault;
      if (AThread->ShowAllTargets)
        {
          CurrentSim.SlantStart = AThread->CurrentSim.SlantStart;
          CurrentSim.SlantEnd = AThread->CurrentSim.SlantEnd;
          SetNewGraphRange(&(CurrentSim.SlantStart),&(CurrentSim.SlantEnd),
                      &(CurrentSim.AzimuthStart),&(CurrentSim.AzimuthEnd));
        }
      // show maximum amplitude calculated from array
      char str[40];
      MaxMag = AThread->MaxMagnitude;
      sprintf(str,"%12.6G V",MaxMag);
      Label6->Caption = str;
      IsDirtyNewCalc = FALSE;
    }
  Free_DVector(AThread->PulseSendTime,0);
  Free_DMatrix(AThread->Data,0,0);

  Image1->Picture->Bitmap->LoadFromResourceName((int)HInstance,"BITMAP_2");

  Busy = FALSE;
  if (WantToClose) Close();
  Invalidate();
}
//---------------------------------------------------------------------------
void  TSimulationForm::UpdateGraphBitmap(bool/* ShowAllTargets*/)
{
  if (!Busy && FinishedInitializing)
    {
      Image1->Picture->Bitmap->LoadFromResourceName((int)HInstance,"BITMAP_1");
      ProgressBar->Position = 0;
      ProgressBar->Invalidate();
      Busy = TRUE;
      AThread = new TCalcThread();
      // now copy necessary variables over
      AThread->ParentForm = this;
      FindCurrentRadar();   // just in case user fiddeled with radar and
      // didn't close this form
      AThread->CRadar = CRadar;
      CopySimulation(&(AThread->CurrentSim),&CurrentSim);
      // note that for the screen output we have to double the frequency
      // as we are displaying range (which is 2*range-delay)
      AThread->CurrentSim.SampleFreq *= 2.0;
      AThread->PixelX = GraphBM2->DIBWidth;
      AThread->PixelY = GraphBM2->DIBHeight;

      // if ShowAllTargets is TRUE, range will be altered such that
      // all targets are shown
      AThread->ShowAllTargets = AutoSlantRange;
      AutoSlantRange = FALSE;
      AThread->Priority = tpNormal; //tpIdle;
      AThread->OnTerminate = ThreadDone;
      AThread->FreeOnTerminate = TRUE;
      // start thread
      AThread->Resume();
    }
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::Timer1Timer(TObject *Sender)
{
  if (Busy) ProgressBar->Position = (unsigned char)CalcProgress;
  else ProgressBar->Position = 0;
  if (IsDirtyNewCalc && !Busy)
    {
      UpdateGraphBitmap(FALSE);
    }
  else if (!IsDirtyNewCalc && !Busy && IsDirtyRedraw)
    {
      DrawBitmap(Canvas, Data, PulseSendTime, PulseNo);
      IsDirtyRedraw = false;
      Invalidate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::TBContrastChange(TObject *Sender)
{
  IsDirtyNewCalc = TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::ZoomInYClick(TObject *Sender)
{
  // busy updating -> quit that
  if (Busy) StopThread();
  CurrentSim.AzimuthStart = CurrentSim.AzimuthStart + 0.25 * Graph->YRange;
  CurrentSim.AzimuthEnd = CurrentSim.AzimuthEnd - 0.25 * Graph->YRange;
  RedrawRange();
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::ZoomOutYClick(TObject *Sender)
{
  // busy updating -> quit that
  if (Busy) StopThread();
  CurrentSim.AzimuthStart = CurrentSim.AzimuthStart - 0.5 * Graph->YRange;
  CurrentSim.AzimuthEnd = CurrentSim.AzimuthEnd + 0.5 * Graph->YRange;
  RedrawRange();
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::ZoomInXClick(TObject *Sender)
{
  // busy updating -> quit that
  if (Busy) StopThread();
  CurrentSim.SlantStart = CurrentSim.SlantStart + 0.25 * Graph->XRange;
  CurrentSim.SlantEnd = CurrentSim.SlantEnd - 0.25 * Graph->XRange;
  RedrawRange();
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::ZoomOutXClick(TObject *Sender)
{
  // busy updating -> quit that
  if (Busy) StopThread();
  CurrentSim.SlantStart = CurrentSim.SlantStart - 0.5 * Graph->XRange;
  CurrentSim.SlantEnd = CurrentSim.SlantEnd + 0.5 * Graph->XRange;
  RedrawRange();
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::BRLeftClick(TObject *Sender)
{
  // busy updating -> quit that
  if (Busy) StopThread();
  CurrentSim.SlantStart = CurrentSim.SlantStart - 0.25 * Graph->XRange;
  CurrentSim.SlantEnd = CurrentSim.SlantEnd - 0.25 * Graph->XRange;
  RedrawRange();
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::BRRightClick(TObject *Sender)
{
  // busy updating -> quit that
  if (Busy) StopThread();
  CurrentSim.SlantStart = CurrentSim.SlantStart + 0.25 * Graph->XRange;
  CurrentSim.SlantEnd = CurrentSim.SlantEnd + 0.25 * Graph->XRange;
  RedrawRange();
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::BADownClick(TObject *Sender)
{
  // busy updating -> quit that
  if (Busy) StopThread();
  CurrentSim.AzimuthStart = CurrentSim.AzimuthStart - 0.25 * Graph->YRange;
  CurrentSim.AzimuthEnd = CurrentSim.AzimuthEnd - 0.25 * Graph->YRange;
  RedrawRange();
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::BAUpClick(TObject *Sender)
{
  // busy updating -> quit that
  if (Busy) StopThread();
  CurrentSim.AzimuthStart = CurrentSim.AzimuthStart + 0.25 * Graph->YRange;
  CurrentSim.AzimuthEnd = CurrentSim.AzimuthEnd + 0.25 * Graph->YRange;
  RedrawRange();
}
//---------------------------------------------------------------------------
void TSimulationForm::RedrawRange()
{
  FastUpdate();
  SetNewGraphRange(&(CurrentSim.SlantStart),&(CurrentSim.SlantEnd),
                  &(CurrentSim.AzimuthStart),&(CurrentSim.AzimuthEnd));
  Graph->Paint(UNITS);
  UpdateGraphBitmap(FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::FastUpdate(void)
{
  BusyWithBitmap = true;
/*
  if (!BusyWithBitmap)
    {
          TRect SourceRect,TargetRect,BitmapRect;
          // find pixel coordinates of new range
          RealToWindow(CurrentSim.SlantStart, CurrentSim.AzimuthStart,
                       &(SourceRect.Left),&(SourceRect.Top));
          RealToWindow(CurrentSim.SlantEnd, CurrentSim.AzimuthEnd,
                       &(SourceRect.Right),&(SourceRect.Bottom));

          // set target to the graph window
          TargetRect.Top = Graph->GraphArea.Bottom;
          TargetRect.Left = Graph->GraphArea.Left + 1;
          TargetRect.Bottom = Graph->GraphArea.Top;
          TargetRect.Right = Graph->GraphArea.Right;
          Canvas->Pen->Color = clWhite;
          Canvas->Brush->Color = clWhite;
          Canvas->Brush->Style = bsSolid;
          // clear old rect
          TRect CleanRect;
          SetRect(&CleanRect, Graph->GraphArea.Left + 1, Graph->GraphArea.Top + 1,
            Graph->GraphArea.Right,Graph->GraphArea.Bottom);
          Canvas->FillRect(CleanRect);
       //   Canvas->Rectangle(Border.Left + 1,Border.Top + 1,
       //   Border.Left + 1 + GraphBM->Width,Border.Top + 1 + GraphBM->Height);

          TRect SourceRect2,TargetRect2;
          SourceRect2 = SourceRect;
          TargetRect2 = TargetRect;
          if (SourceRect.Left < 0)
            {
              TargetRect2.Left += (double(-SourceRect.Left)/
                       double(SourceRect.Right - SourceRect.Left)) * double(Graph->PixelX);
              SourceRect2.Left = 0;
            }
          if (SourceRect.Right > Graph->PixelX)
            {
              TargetRect2.Right -= (double(SourceRect.Right - Graph->PixelX)/
                       double(SourceRect.Right - SourceRect.Left)) * double(Graph->PixelX);
              SourceRect2.Right = Graph->PixelX;
            }
          if (SourceRect.Bottom < 0)
            {
              TargetRect2.Bottom += (double(-SourceRect.Bottom)/
                       double(SourceRect.Top - SourceRect.Bottom)) * double(Graph->PixelY);
              SourceRect2.Bottom = 0;
            }
          if (SourceRect.Top > Graph->PixelY)
            {
              TargetRect2.Top -= (double(SourceRect.Top - Graph->PixelY)/
                       double(SourceRect.Top - SourceRect.Bottom)) * double(Graph->PixelY);
              SourceRect2.Top = Graph->PixelY;
            }

          //GraphBM->IgnorePalette = FALSE;
          //GraphBM->Palette = CreatePalette(MainForm->RedBluePalette);
          Canvas->CopyMode = cmSrcCopy;
//check          Canvas->CopyRect(TargetRect2, GraphBM->Canvas, SourceRect2);
          //GraphBM->ReleasePalette();
          BitmapRect.Top = 0;
          BitmapRect.Left = 0;
          BitmapRect.Right = Graph->PixelX;
          BitmapRect.Bottom = Graph->PixelY;
//"          GraphBM->Canvas->CopyMode = cmSrcCopy;
//"          GraphBM->Canvas->CopyRect(BitmapRect, Canvas, Graph->GraphArea);

      //    GraphBM->Canvas->StretchDraw(BitmapRect, Canvas, Graph->GraphArea);
   }
*/
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::EnterKeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    {
      // busy updating -> quit that
      if (Busy) StopThread();

      if (EXStart->Focused())
        CurrentSim.SlantStart =  atof(EXStart->Text.c_str());
      if (EXEnd->Focused())
        CurrentSim.SlantEnd =  atof(EXEnd->Text.c_str());
      if (EYStart->Focused())
        CurrentSim.AzimuthStart =  atof(EYStart->Text.c_str());
      if (EYEnd->Focused())
        CurrentSim.AzimuthEnd =  atof(EYEnd->Text.c_str());
      SetNewGraphRange(&(CurrentSim.SlantStart),&(CurrentSim.SlantEnd),
                      &(CurrentSim.AzimuthStart),&(CurrentSim.AzimuthEnd));
      UpdateGraphBitmap(FALSE);
    }
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::RadarListChange(TObject *Sender)
{
  // busy updating -> quit that
  if (Busy) StopThread();
  FindCurrentRadar();
  UpdateGraphBitmap(FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::FindCurrentRadar()
{
  // setup radar list box and find number of selected radar
  strncpy(CurrentSim.RadarName, RadarList->Text.c_str(),MAX_NAME_LENGTH - 1);
  struct SRadar *R;
  R = MainForm->FirstRadar;
  // in case name not defined choose first radar
  CRadar =  MainForm->FirstRadar;
  while (R != NULL)
	{
	  if (stricmp(R->RadarName, CurrentSim.RadarName) == 0)
	    { CRadar = R; break; }
	  R = R->next;
	}
  RadarList->Text = CRadar->RadarName;
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::RGDisplayClick(TObject *Sender)
{
  // busy updating -> quit that
  if (Busy) StopThread();
  BusyWithBitmap = true;
  if (RGDisplay->ItemIndex == 4) // dB chosen
    {
      CurrentSim.Display = 2; // show mag
      ShowdB = true;
    }
  else
    {
      CurrentSim.Display = RGDisplay->ItemIndex;
      ShowdB = false;
    }
  FindMinMax();
  IsDirtyRedraw = true;
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::BSaveDataClick(TObject *Sender)
{
  // busy updating -> quit that
  if (Busy)
    {
      Timer1->Enabled = FALSE;
      if (Busy) StopThread();
    }
  SaveSimForm->MaxMag = MaxMag;
  CopySimulation(&(SaveSimForm->CurrentSim),&CurrentSim);
  SaveSimForm->CRadar = CRadar;
  //SaveSimForm->ParentForm = this;
  SaveSimForm->ShowModal();

  Timer1->Enabled = TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::BCancelClick(TObject *Sender)
{
  WantToClose = TRUE;
  // busy updating -> quit that
  if (Busy) StopThread();
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::EditSampleFKeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    {
      // busy updating -> quit that
      if (Busy) StopThread();
      CurrentSim.SampleFreq = FindNyquistRate(CRadar)*atof(EditSampleF->Text.c_str());
      if (CurrentSim.SampleFreq <= 0)
        {
          CurrentSim.SampleFreq = FindNyquistRate(CRadar);
          EditSampleF->Text = "1";
        }
      UpdateGraphBitmap(FALSE);
    }
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::EditSampleFExit(TObject *Sender)
{
  // busy updating -> quit that
  if (Busy) StopThread();
  CurrentSim.SampleFreq = FindNyquistRate(CRadar)*atof(EditSampleF->Text.c_str());
  if (CurrentSim.SampleFreq <= 0)
    {
      CurrentSim.SampleFreq = FindNyquistRate(CRadar);
      EditSampleF->Text = "1";
    }
  UpdateGraphBitmap(FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::PulsePosClick(TObject *Sender)
{
  // busy updating -> quit that
  if (Busy) StopThread();
  CurrentSim.PTPos = PulsePos->ItemIndex;
  UpdateGraphBitmap(FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  WantToClose = TRUE;
  // busy updating -> quit that
  if (Busy) StopThread();
  if (!Busy) CanClose = TRUE;
  else CanClose = FALSE;
}
//---------------------------------------------------------------------------
void TSimulationForm::StopThread(void)
{
  UserAbort = TRUE;
  Terminat = TRUE;
  IsDirtyNewCalc=TRUE;
  AThread->Terminate();
}


void __fastcall TSimulationForm::SBContrastScroll(TObject *Sender,
  TScrollCode ScrollCode, int &ScrollPos)
{
  MakePalette(ColorPal,SBBright->Position,SBContrast->Position,
              SBSat->Position);
  SetupPaletteBitmap();
  Invalidate();
  IsDirtyRedraw = true;
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::SBBrightScroll(TObject *Sender,
  TScrollCode ScrollCode, int &ScrollPos)
{
  MakePalette(ColorPal,SBBright->Position,SBContrast->Position,
              SBSat->Position);
  SetupPaletteBitmap();
  Invalidate();
  IsDirtyRedraw = true;
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::SBSatScroll(TObject *Sender,
  TScrollCode ScrollCode, int &ScrollPos)
{
   MakePalette(ColorPal,SBBright->Position,SBContrast->Position,
              SBSat->Position);
   SetupPaletteBitmap();
   Invalidate();
   IsDirtyRedraw = true;
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::FormDestroy(TObject *Sender)
{
  delete PalBM;
  delete GraphBM2;
  delete LGraphBM;
}
//---------------------------------------------------------------------------
void TSimulationForm::SetupPaletteBitmap()
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
void TSimulationForm::MakePalette(unsigned char Type,int Bright,
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
void __fastcall TSimulationForm::BPaletteClick(TObject *Sender)
{
  LoadPalForm->ShowModal();
  ColorPal = LoadPalForm->Pal;
  MakePalette(ColorPal,127,127,127);
  SetupPaletteBitmap();
//  RealizeMyPalette(true);
 // if (RealizeMyPalette(false))
   IsDirtyRedraw = true;
//    Invalidate();
}
//---------------------------------------------------------------------------
// ForceBackGround should be true on WM_PALETTE_CHANGED
// and false on WM_QUERYNEWPALETTE
//---------------------------------------------------------------------------
int TSimulationForm::RealizeMyPalette(bool ForceBackGround)
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
void __fastcall TSimulationForm::RGDrawDataClick(TObject *Sender)
{
  if ((RGDrawData->ItemIndex == 0) || ((RGDrawData->ItemIndex == 3) &&
   ((double(PixelY)/double(PulseNo))<(double)PixStep))) // colour pixels
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
  UpdateGraphBitmap(FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::RGScaleClick(TObject *Sender)
{
  IsDirtyNewCalc=true;
}
//---------------------------------------------------------------------------
void TSimulationForm::FindMinMax()
{
  long PNo,Sample;
  double temp;
  // find min and max values
  PMax = MIN_DOUBLE;
  PMin = MAX_DOUBLE;
  for (PNo=0;PNo<PulseNo;PNo++)
    {
      for (Sample=0; Sample<GraphBM2->DIBWidth; Sample++)
		    {
          temp = GetValue(CurrentSim.Display,Data[PNo][2*Sample],Data[PNo][2*Sample+1]);
          if (temp > PMax) PMax = temp;
          if (temp < PMin) PMin = temp;
        }
    }
}
void __fastcall TSimulationForm::BBitmapClick(TObject *Sender)
{
  if (SaveBitmapDialog->Execute())
    {
      Graphics::TBitmap *SaveBM = new Graphics::TBitmap();
      SaveBM->Handle = GraphBM2->hDIBSection;
      SaveBM->SaveToFile(SaveBitmapDialog->FileName);
      delete SaveBM;
      SetNewGraphSize();
    }
}
//---------------------------------------------------------------------------
void __fastcall TSimulationForm::BHelpClick(TObject *Sender)
{
  Application->HelpJump("simwing");
}
//---------------------------------------------------------------------------
