//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "graphform.h"
#include "Graph.h"
#include "math.h"
#include "main.h"
#include "general.h"
#include "parser.h"
#include "engine.h"

//---------------------------------------------------------------------------
#pragma link "Grids"
#pragma resource "*.dfm"
TGraphForm1 *GraphForm1;
//---------------------------------------------------------------------------
__fastcall TGraphForm1::TGraphForm1(TComponent* Owner)
	: TForm(Owner)
{
  //intantiate new graph
  Graph = new class TGraph(Canvas,1,1,-2,-2);
  // create bitmap buffer
  GraphBM = new Graphics::TBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::FormPaint(TObject *Sender)
{

  // now fill in the holes
  if (DataDef.Source == ID_FILE)
    FileRadioBtn->Checked = TRUE;
  else if (DataDef.Source == ID_INLINE)
    InlineRadioBtn->Checked = TRUE;
  FileName->Text = DataDef.FileName;
  RGDisplay->ItemIndex = Display;
  IntGroup->ItemIndex = DataDef.IntMethod[Display];
  // setup window range values
  UpdateRange();
  // write names
  int TextH, TextW;
  Canvas->Font->Color = clBlue;
  Canvas->Brush->Color = clGray;//clBtnFace;

  TextH = Canvas->TextHeight(DataDef.YAxis[Display].Name);
  TextW = Canvas->TextWidth(DataDef.YAxis[Display].Name);
  Canvas->TextOut(Graph->GraphArea.Left-TextW*0.5,
                  Graph->GraphArea.Top-TextH-5,DataDef.YAxis[Display].Name);
  TextH = Canvas->TextHeight(DataDef.XAxis[Display].Name);
  TextW = Canvas->TextWidth(DataDef.XAxis[Display].Name);
  Canvas->TextOut(Graph->GraphArea.Right-TextW*0.8,
                  Graph->GraphArea.Bottom+TextH+10,DataDef.XAxis[Display].Name);
  RangeX->Caption = DataDef.XAxis[Display].Name;
  RangeY->Caption = DataDef.YAxis[Display].Name;

  // draw frame and units
  Graph->Paint(FRAME);
  Graph->Paint(UNITS);
  // show graph bitmap
  Canvas->Draw(Border.Left + 1,Border.Top + 1,GraphBM);
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::FormResize(TObject *Sender)
{
  // draw OK and Cancel button at right position
  OKButton->Top = ClientHeight - (OKButton->Height+10);
  CancelButton->Top = ClientHeight - (CancelButton->Height + 10);//70;
  HelpButton->Top = ClientHeight - (CancelButton->Height + 10);//70;
  OKButton->Left = Border.Left - OKButton->Width  +
            (ClientWidth - Border.Left - Border.Right) * 0.25;
  CancelButton->Left = Border.Left - CancelButton->Width * 0.5 +
            (ClientWidth - Border.Left - Border.Right) * 0.5;
  HelpButton->Left = Border.Left +
            (ClientWidth - Border.Left - Border.Right) * 0.75;

  ZoomInX->Left = Border.Left + (ClientWidth - Border.Left - Border.Right) * 0.5
                  - (ZoomInX->Width + 5);
  ZoomOutX->Left = Border.Left + (ClientWidth - Border.Left - Border.Right) * 0.5
                   + 5;
  ZoomInY->Left = ClientWidth - (ZoomInY->Width + 2);
  ZoomOutY->Left = ClientWidth - (ZoomOutY->Width + 2);

  // initialize graph with new window size parameters
  InitGraph(Graph->XStart, Graph->XEnd, Graph->YStart, Graph->YEnd);
  DrawGraph(GraphBM->Canvas, true);
}
//---------------------------------------------------------------------------
void TGraphForm1::UpdateRange(void)
{
  char str[80];
  // setup window range values
  sprintf(str,"%.*f",2,Graph->XStart);
  EXStart->Text = str;
  sprintf(str,"%.*f",2,Graph->XEnd);
  EXEnd->Text = str;
  sprintf(str,"%.*f",2,Graph->YStart);
  EYStart->Text = str;
  sprintf(str,"%.*f",2,Graph->YEnd);
  EYEnd->Text = str;
}
//---------------------------------------------------------------------------
void TGraphForm1::InitGraph(double XStart, double XEnd, double YStart,
                            double YEnd)
{
  // set units etc.
  Graph->XDecPlaces = DataDef.XAxis[Display].DecPlaces;
  Graph->YDecPlaces = DataDef.YAxis[Display].DecPlaces;
  Graph->UnitsX = DataDef.XAxis[Display].Unit;
  Graph->UnitsY = DataDef.YAxis[Display].Unit;

  TRect SetBorder;
  // calculate new border
  SetBorder = Border;
  SetBorder.Right = ClientWidth - Border.Right - 1;
  SetBorder.Bottom = ClientHeight - Border.Bottom - 1;

  // do some range checking
  if (XStart <  DataDef.XAxis[Display].Min) XStart = DataDef.XAxis[Display].Min;
  if (YStart <  DataDef.YAxis[Display].Min) YStart = DataDef.YAxis[Display].Min;
  if (XEnd >  DataDef.XAxis[Display].Max) XEnd = DataDef.XAxis[Display].Max;
  if (YEnd >  DataDef.YAxis[Display].Max) YEnd = DataDef.YAxis[Display].Max;
  if (XStart > XEnd) Swap(&XStart,&XEnd);
  if (YStart > YEnd) Swap(&YStart,&YEnd);
  if (XStart == XEnd) XEnd = XStart + 1;
  if (YStart == YEnd) YEnd = YStart + 1;

  DataDef.XAxis[Display].Start = XStart;
  DataDef.XAxis[Display].End = XEnd;
  DataDef.YAxis[Display].Start = YStart;
  DataDef.YAxis[Display].End = YEnd;

  Graph->Init(SetBorder,XStart,XEnd,YStart,YEnd);

  // free memory of old bitmap
  delete GraphBM;
  // create bitmap buffer
  GraphBM = new Graphics::TBitmap();
  GraphBM->Height = ClientHeight - Border.Bottom - Border.Top - 2;
  GraphBM->Width = ClientWidth - Border.Right - Border.Left - 2;
  Invalidate();
}

//---------------------------------------------------------------------------
void TGraphForm1::DrawGraph(TCanvas *DCanvas, bool Clear)
{
  if (Clear)
    {
      // clear old bitmap
      TRect BMRect;
      BMRect.Top = 0;
      BMRect.Bottom = 2000;
      BMRect.Left = 0;
      BMRect.Right = 2000;
      DCanvas->Brush->Color = clWhite;
      DCanvas->FillRect(BMRect);
    }

  // graph type to be drawn if CONT or INTEGER
  const int GraphType[2] = {LINE_MARKER, BAR};

  Graph->DrawGraph(DCanvas, DataDef.NoP[Display], (DataDef.XAxis[Display].DataArray),
  (DataDef.YAxis[Display].DataArray),DataDef.Coeff[Display], GraphType[DataDef.Type[Display]],
  DataDef.IntMethod[Display]);
}

//---------------------------------------------------------------------------
void __fastcall TGraphForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
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
          // zoom or drag sample ?
          ClickedSample = ClickedOnSample(X,Y);
          if (ClickedSample >= 0)
            {
              DragMode = TRUE;
              PointCoord(X,Y);
          //    OldPoint.x = X;
          //    OldPoint.y = Y;
              // use special drag cursor
              Screen->Cursors[crDragCursor] = LoadCursor(HInstance, "DRAG_SAMPLE");
              Screen->Cursor = (TCursor)crDragCursor;
              // keep mouse within graph region
              RECT ClipRegion;
 /*             SetRect(&ClipRegion,Left + Border.Left + (Width - ClientWidth)/2 + 1,
                     Top + Border.Top + (Height - ClientHeight - (Width - ClientWidth)/2) + 1,
                     Left + Border.Left + Graph->PixelX + (Width - ClientWidth)/2 + 1,
                     Top + Border.Top + (Height - ClientHeight - (Width - ClientWidth)/2) + 1 + Graph->PixelY);
 */             SetRect(&ClipRegion,Left + Border.Left + (Width - ClientWidth)/2 + 1,
                     Top + Border.Top + (Height - ClientHeight - (Width - ClientWidth)/2) + 1,
                     Left + Border.Left + Graph->PixelX + (Width - ClientWidth)/2 + 1,
                     Top + Border.Top + (Height - ClientHeight - (Width - ClientWidth)/2) + 1 + Graph->PixelY);
              ClipCursor(&ClipRegion);
              // erase old sample point
              Canvas->Brush->Color = clLtGray;//White;
		      Canvas->Pen->Color = clLtGray;//clWhite;
              int OldX, OldY;
              RealToWindow(DataDef.XAxis[Display].DataArray[ClickedSample],
                          DataDef.YAxis[Display].DataArray[ClickedSample],
                          &OldX, &OldY);
		      Canvas->Ellipse(Graph->GraphArea.Left+OldX-1,Graph->GraphArea.Top+OldY-2,
                              Graph->GraphArea.Left+OldX+4, Graph->GraphArea.Top+OldY+3);
            }
          else
            {
              SetRect(&ZoomRect,X, Y, X, Y);
		      // set drawing mode to inverted
              Canvas->Pen->Mode = pmNot;
		      // draw rectangle
  		      Canvas->DrawFocusRect(ZoomRect);
            }
		}  // end if
	}  // end if
  if ((Button == mbRight) && (!LeftButtonDown) && (!RightButtonDown))  // zoom out by a factor of 2
	{
      RightButtonDown = TRUE;
      // clicked in graph area ?
	  if (PointInRect(Graph->GraphArea,X,Y))
        {
		  double Xtemp,Ytemp,X1,Y1,X2,Y2;
	      // convert window (pixel) coordinates to actual values
		  WindowToReal(X, Y, &Xtemp, &Ytemp);
		  X1 = Xtemp - Graph->XRange;
		  Y1 = Ytemp - Graph->YRange;
		  X2 = Xtemp + Graph->XRange;
		  Y2 = Ytemp + Graph->YRange;
          InitGraph(X1, X2, Y1, Y2);
          DrawGraph(GraphBM->Canvas, FALSE);
          UpdateRange();
        }
    }
}

//---------------------------------------------------------------------------
long TGraphForm1::ClickedOnSample(int XPix, int YPix)
{
  double XReal, YReal;
  double XDist, YDist;
  long j;
  long P = -1;

  // calculate the maximum distance to a point such that it will be selected
  XDist = 8 * (Graph->XRange / Graph->PixelX);
  YDist = 8 * (Graph->YRange / Graph->PixelY);
  // find the corresponding 'real life' coordinates for the pixel chosen
  WindowToReal(XPix, YPix, &XReal, &YReal);
  // search through all samples
  for (j=0;j<DataDef.NoP[Display];j++)
	{
      if ( (fabs(DataDef.XAxis[Display].DataArray[j] - XReal) < XDist)
           && (fabs(DataDef.YAxis[Display].DataArray[j] - YReal) < YDist) )
        {
          P = j;
          break;
        }
	}
  return P;
}

//---------------------------------------------------------------------------
void __fastcall TGraphForm1::FormMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
  if ((Button == mbLeft) && LeftButtonDown)
	{
      if (DragMode) // drag a sample
        {
	      // release mouse
          ClipCursor(NULL);
          ReleaseCapture();
	      LeftButtonDown = FALSE;
  	      DragMode = FALSE;
          Screen->Cursor = crDefault;
          double XReal, YReal;
          // find the corresponding 'real life' coordinates for the pixel chosen
          WindowToReal(X, Y, &XReal, &YReal);
          // round them slightly (2 digit accuracy)
          int Expo;
          SciNotation(Graph->XRange,&Expo);
          XReal = round(XReal / pow10(Expo - 2))*pow10(Expo - 2);
          SciNotation(Graph->YRange,&Expo);
          YReal = round(YReal / pow10(Expo - 2))*pow10(Expo - 2);
       //   int Expo;
       //   SciNotation(XReal,&Expo);
       //   XReal = round(XReal / pow10(Expo - 4))*pow10(Expo - 4);
       //   SciNotation(YReal,&Expo);
       //   YReal = round(YReal / pow10(Expo - 4))*pow10(Expo - 4);
          // and update lists and arrays
          DataDef.XAxis[Display].DataArray[ClickedSample] = XReal;
          DataDef.YAxis[Display].DataArray[ClickedSample] = YReal;
          SampleGrid->Cells[1][ClickedSample+1] = FloatToStr(XReal);
          SampleGrid->Cells[2][ClickedSample+1] = FloatToStr(YReal);
		  UpdateSampleValues();
        }
      else
        {
	      // set drawing mode to inverted
          Canvas->Pen->Mode = pmNot;
          // erase rect
  	      Canvas->DrawFocusRect(ZoomRect);
	      // release mouse
          ReleaseCapture();
	      LeftButtonDown = FALSE;
          double X1,Y1,X2,Y2;
	      // convert window (pixel) coordinates to actual values
	      WindowToReal(ZoomRect.Left, ZoomRect.Bottom, &X1, &Y1);
	      WindowToReal(ZoomRect.Right, ZoomRect.Top, &X2, &Y2);
          if ( (X1 != X2) && (Y1 != Y2) )
            {
              // initialize graph with new window size parameters
              InitGraph(X1, X2, Y1, Y2);
              DrawGraph(GraphBM->Canvas, FALSE);
              UpdateRange();
            }
       }  // end else
	}
  if ((Button == mbRight) && RightButtonDown)
    RightButtonDown = FALSE;
}
//---------------------------------------------------------------------------
void TGraphForm1::PointCoord(int X, int Y)
{
          char s[60];
          double XReal, YReal;
          // find the corresponding 'real life' coordinates for the pixel chosen
          WindowToReal(X, Y, &XReal, &YReal);
          // round them slightly (2 digit accuracy)
          int Expo;

          SciNotation(Graph->XRange,&Expo);
          XReal = round(XReal / pow10(Expo - 2))*pow10(Expo - 2);
          SciNotation(Graph->YRange,&Expo);
          YReal = round(YReal / pow10(Expo - 2))*pow10(Expo - 2);
          sprintf(s,"[%-.8G,%-.8G]",XReal,YReal);
          TRect ClearRect = Rect(OldPoint.x+5,OldPoint.y+5,OldPoint.x+5+
            Canvas->TextWidth(s)+20,OldPoint.y+5+Canvas->TextHeight(s)+1);
          if (ClearRect.Right > Border.Left + Graph->PixelX+1)
             ClearRect.Right =  Border.Left + Graph->PixelX+1;
          if (ClearRect.Bottom > Border.Top + Graph->PixelY+1)
             ClearRect.Bottom =  Border.Top + Graph->PixelY+1;
          if (ClearRect.Left > Border.Left + Graph->PixelX+1)
             ClearRect.Left =  Border.Left + Graph->PixelX+1;
          if (ClearRect.Top > Border.Top + Graph->PixelY+1)
             ClearRect.Top =  Border.Top + Graph->PixelY+1;
          TRect B = Rect(X+5,Y+5,X+5+Canvas->TextWidth(s),Y+5+Canvas->TextHeight(s));
          if (B.Left > Border.Left + Graph->PixelX+1)
             B.Left =  Border.Left + Graph->PixelX+1;
          if (B.Top > Border.Top + Graph->PixelY+1)
             B.Top =  Border.Top + Graph->PixelY+1;
         if (B.Right > Border.Left + Graph->PixelX+1)
             B.Right =  Border.Left + Graph->PixelX+1;
          if (B.Bottom > Border.Top + Graph->PixelY+1)
             B.Bottom =  Border.Top + Graph->PixelY+1;
          Canvas->Brush->Color = clLtGray;//clWhite;
     	    Canvas->FillRect(ClearRect);
          Canvas->TextRect(B,X+5,Y+5,s);
          OldPoint.x = X;
          OldPoint.y = Y;
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::FormMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
  if (LeftButtonDown)
	{
      if (DragMode)
        {
          // show coordinates
          PointCoord(X,Y);
        }
      else
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
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::RGDisplayClick(TObject *Sender)
{
  Display = RGDisplay->ItemIndex;
  if (IntGroup->ItemIndex == CUBIC)
    FindPolyCoeff(DataDef.XAxis[Display].DataArray, DataDef.YAxis[Display].DataArray,
                  DataDef.NoP[Display], (DataDef.Coeff[Display]));
  UpdateSampleGrid();
  InitGraph(DataDef.XAxis[Display].Start,DataDef.XAxis[Display].End,
    DataDef.YAxis[Display].Start,DataDef.YAxis[Display].End);
  DrawGraph(GraphBM->Canvas, TRUE);
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::TEExit(TObject *Sender)
{
  // initialize graph with new window size parameters
  InitGraph(atof(EXStart->Text.c_str()), atof(EXEnd->Text.c_str()),
       atof(EYStart->Text.c_str()), atof(EYEnd->Text.c_str()));
  DrawGraph(GraphBM->Canvas, FALSE);
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::EnterKeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)13)
    {
      // initialize graph with new window size parameters
      InitGraph(atof(EXStart->Text.c_str()), atof(EXEnd->Text.c_str()),
       atof(EYStart->Text.c_str()), atof(EYEnd->Text.c_str()));
      DrawGraph(GraphBM->Canvas, FALSE);
    }
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
  strncpy(DataDef.FileName,FileName->Text.c_str(),25);
  if (FileRadioBtn->Checked)
    DataDef.Source = ID_FILE;
  else if (InlineRadioBtn->Checked)
    DataDef.Source = ID_INLINE;
  DataDef.IntMethod[Display] = IntGroup->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::FormShow(TObject *Sender)
{
  // define borders (pixel space between graph and window on each side)
  Border.Left = EditBox->Width + 80;//320;
  Border.Right = 38;
  Border.Top = 34;
  Border.Bottom = 115;
  // -2 because we don't need the frame
  GraphBM->Height = ClientHeight - Border.Bottom - Border.Top - 2;
  GraphBM->Width = ClientWidth - Border.Right - Border.Left - 2;
  LeftButtonDown = FALSE;
  RightButtonDown = FALSE;
  DragMode = FALSE;
  // current displayed variable
  Display = 0;

  int i;
  // if no points defined yet, define at least one
  for (i=0;i<DataDef.DataTypes;i++)
    {
      if (DataDef.NoP[i] == 0)
        {
          DataDef.XAxis[i].DataArray = DVector(0,0);
          DataDef.YAxis[i].DataArray = DVector(0,0);
		  DataDef.Coeff[i] = DMatrix(0,0,0,3);
          DataDef.NoP[i] = 1;
          DataDef.XAxis[i].DataArray[0] = 0;
          DataDef.YAxis[i].DataArray[0] = 0;
          DataDef.Coeff[i][0][0] = 0;
          DataDef.Coeff[i][0][1] = 0;
          DataDef.Coeff[i][0][2] = 0;
          DataDef.Coeff[i][0][3] = 0;
        }
    }
  // adjust column width to accomodate scroll bar
  AdjustSampleGrid();

  // make display variables radio group
  RGDisplay->Items->Clear();
  for (i=0;i<DataDef.DataTypes;i++)
    RGDisplay->Items->Add(DataDef.YAxis[i].Name);

  UpdateSampleGrid();
  if (IntGroup->ItemIndex == CUBIC)
    FindPolyCoeff(DataDef.XAxis[Display].DataArray, DataDef.YAxis[Display].DataArray,
                  DataDef.NoP[Display], (DataDef.Coeff[Display]));
  InitGraph(DataDef.XAxis[Display].Start,DataDef.XAxis[Display].End,
    DataDef.YAxis[Display].Start,DataDef.YAxis[Display].End);
}
//---------------------------------------------------------------------------
void TGraphForm1::UpdateSampleGrid(void)
{
  int i;
  SampleNo->Text = IntToStr(DataDef.NoP[Display]);
  SampleGrid->RowCount = DataDef.NoP[Display] + 1;
  AdjustSampleGrid();
//  SampleGrid->Height = SampleGrid->RowCount * (SampleGrid->DefaultRowHeight+2);
//  if (SampleGrid->Height > 122) SampleGrid->Height = 122;
//  if (SampleGrid->RowCount > 7) SampleGrid->ColWidths[2] = 91;
//  else SampleGrid->ColWidths[2] = 107;
  for (i=0;i<DataDef.NoP[Display];i++)
    {
      SampleGrid->Cells[0][i+1] = IntToStr(i+1);
      SampleGrid->Cells[1][i+1] = FloatToStr(DataDef.XAxis[Display].DataArray[i]);
      SampleGrid->Cells[2][i+1] = FloatToStr(DataDef.YAxis[Display].DataArray[i]);
    }
  SampleGrid->Cells[1][0] = DataDef.XAxis[Display].Name;
  SampleGrid->Cells[2][0] = DataDef.YAxis[Display].Name;
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::SampleNoKeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)13) // enter key
    {
      SampleNoExit(Sender);
      SampleNoExit(Sender);
    }
}
//---------------------------------------------------------------------------
void TGraphForm1::UpdateSampleValues(void)
{
  int Count, i;
  SortSampleGrid();

  Count = atoi(SampleNo->Text.c_str());
  if (Count == DataDef.NoP[Display]) // number of samples has not changed
    {
	  for (i=0;i<Count;i++)
	    {
          // transfer strings into float array
          DataDef.XAxis[Display].DataArray[i] =
              atof(SampleGrid->Cells[1][i+1].c_str());
          DataDef.YAxis[Display].DataArray[i] =
              atof(SampleGrid->Cells[2][i+1].c_str());
		} // end for
      if (IntGroup->ItemIndex == CUBIC)
        FindPolyCoeff(DataDef.XAxis[Display].DataArray, DataDef.YAxis[Display].DataArray,
           DataDef.NoP[Display], (DataDef.Coeff[Display]));
      DrawGraph(GraphBM->Canvas, TRUE);
      Invalidate();
    }
  else if ((Count > 0) && (Count <= MAX_SAMPLES))  // within limits ?
    {
      // make box big enough
      SampleGrid->RowCount = Count + 1;
      AdjustSampleGrid();
      //SampleGrid->Height = SampleGrid->RowCount * 18;
      //if (SampleGrid->Height > 122) SampleGrid->Height = 122;
      //if (SampleGrid->RowCount > 7) SampleGrid->ColWidths[2] = 91;
      //else SampleGrid->ColWidths[2] = 107;

      // create temporary arrays and copy data from stringbox
	  double *TempX, *TempY;
	  TempX = DVector(0,Count-1);
	  TempY = DVector(0,Count-1);
	  for (i=0;i<Count;i++)
	    {
		  if (i<DataDef.NoP[Display])
			{
			  TempX[i] = (DataDef.XAxis[Display].DataArray)[i];
			  TempY[i] = (DataDef.YAxis[Display].DataArray)[i];
			}
		  else
		    {
		   	  TempX[i] = i;
			  TempY[i] = 0;
              SampleGrid->Cells[0][i+1] = IntToStr(i+1);
              SampleGrid->Cells[1][i+1] = FloatToStr(TempX[i]);
              SampleGrid->Cells[2][i+1] = FloatToStr(TempY[i]);
			}
	    } // end for
	  DataDef.NoP[Display] = Count;
      // free previous data arrays
	  Free_DVector(DataDef.XAxis[Display].DataArray,0);
	  Free_DVector(DataDef.YAxis[Display].DataArray,0);
	  Free_DMatrix(DataDef.Coeff[Display],0,0);
	  DataDef.XAxis[Display].DataArray = TempX;
	  DataDef.YAxis[Display].DataArray = TempY;
	  DataDef.Coeff[Display] = DMatrix(0,Count-1,0,3);
      if (IntGroup->ItemIndex == CUBIC)
        FindPolyCoeff(DataDef.XAxis[Display].DataArray, DataDef.YAxis[Display].DataArray,
           DataDef.NoP[Display], (DataDef.Coeff[Display]));
      // and update graph window
      DrawGraph(GraphBM->Canvas, TRUE);
      Invalidate();
  }
}
//---------------------------------------------------------------------------
void TGraphForm1::SortSampleGrid(void)
{
  int Samples = SampleGrid->RowCount - 1;
  double *X,*Y;
  int i;
  X = DVector(0,Samples); // actually Samples-1
  Y = DVector(0,Samples);
  for (i=0;i<Samples;i++)
    {
      X[i] = atof(SampleGrid->Cells[1][i+1].c_str());
      Y[i] = atof(SampleGrid->Cells[2][i+1].c_str());
    }
  QuickSort(X, Y, Samples-1, 0, Samples-1);
  for (i=0;i<Samples;i++)
    {
      SampleGrid->Cells[1][i+1] = FloatToStr(X[i]);
      SampleGrid->Cells[2][i+1] = FloatToStr(Y[i]);
    }
  Free_DVector(X,0);
  Free_DVector(Y,0);
}
//---------------------------------------------------------------------
void __fastcall TGraphForm1::QuickSort(double *A, double *B,int const AHigh, int iLo, int iHi)
{
  int Lo, Hi;
  double Mid, T;

  Lo = iLo;
  Hi = iHi;
  Mid = A[(Lo+Hi)/2];
  do
    {
      while (A[Lo] < Mid) Lo++;
      while (A[Hi] > Mid) Hi--;
      if (Lo <= Hi)
        {

          T = A[Lo];
          A[Lo] = A[Hi];
          A[Hi] = T;
          T = B[Lo];
          B[Lo] = B[Hi];
          B[Hi] = T;
          Lo++;
          Hi--;
        }
    }
  while (Lo <= Hi);
  if (Hi > iLo)
    QuickSort(A, B, AHigh, iLo, Hi);
  if (Lo < iHi)
    QuickSort(A, B, AHigh, Lo, iHi);
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::SampleNoExit(TObject *Sender)
{
  UpdateSampleValues();
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::IntGroupClick(TObject *Sender)
{
  DataDef.IntMethod[Display] = IntGroup->ItemIndex;
  if (IntGroup->ItemIndex == CUBIC)
    FindPolyCoeff(DataDef.XAxis[Display].DataArray, DataDef.YAxis[Display].DataArray,
                  DataDef.NoP[Display], (DataDef.Coeff[Display]));
  DrawGraph(GraphBM->Canvas, TRUE);
  Invalidate();
}
//---------------------------------------------------------------------------

void TGraphForm1::WindowToReal(int PixX, int PixY, double *X, double *Y)
{
	*X = Graph->XStart + double(PixX - Graph->GraphArea.Left)*
		(Graph->XRange / double(Graph->PixelX));
	*Y = Graph->YEnd - double(PixY - Graph->GraphArea.Top)*
		(Graph->YRange / double(Graph->PixelY));
}
//---------------------------------------------------------------------------
void TGraphForm1::RealToWindow(double X, double Y, int *PixX, int *PixY)
{
  *PixX = (X - Graph->XStart)*(double(Graph->PixelX)/Graph->XRange);
  *PixY = (Graph->YEnd - Y)*(double(Graph->PixelY)/Graph->YRange);
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::RestrictSize(TMessage& Msg)
{
  // set the limits of the graph windows size
  ((POINT far *)Msg.LParam)[3].x = 520;
  ((POINT far *)Msg.LParam)[3].y = 525;
  ((POINT far *)Msg.LParam)[4].x = 2000;
  ((POINT far *)Msg.LParam)[4].y = 2000;
  TForm::Dispatch(&Msg);
}

//---------------------------------------------------------------------------
void __fastcall TGraphForm1::OKButtonClick(TObject *Sender)
{
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::CancelButtonClick(TObject *Sender)
{
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::AdjustSampleGrid()
{
  SampleGrid->Height = SampleGrid->RowCount * (SampleGrid->DefaultRowHeight+2);
  if (SampleGrid->Height > 122) SampleGrid->Height = 122;
  if (SampleGrid->RowCount > 7) SampleGrid->ColWidths[2] = SampleGrid->Width -
  (SampleGrid->ColWidths[0] + SampleGrid->ColWidths[1] + 22); //91;
  else SampleGrid->ColWidths[2] = SampleGrid->Width -
  (SampleGrid->ColWidths[0] + SampleGrid->ColWidths[1] + 6); //107;
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::InsertSampleClick(TObject *Sender)
{
  int Samples;
  int i;
  double t1,t2;
  Samples = SampleGrid->RowCount;
  if (Samples < (MAX_SAMPLES+1))
    {
      SampleGrid->RowCount += 1;
      SampleGrid->Cells[0][SampleGrid->RowCount-1] =
        IntToStr(SampleGrid->RowCount-1);
      SampleNo->Text = IntToStr(SampleGrid->RowCount-1);
  //    SampleGrid->Height = SampleGrid->RowCount * 18;
      AdjustSampleGrid();
      //if (SampleGrid->Height > 122) SampleGrid->Height = 122;
      //if (SampleGrid->RowCount > 7) SampleGrid->ColWidths[2] = 91;
      //else SampleGrid->ColWidths[2] = 107;
      // from the selected row onwards, move everything down
      for (i=SampleGrid->RowCount-1;i>SampleGrid->Row;i--)
       {
         SampleGrid->Cells[1][i] = SampleGrid->Cells[1][i-1];
         SampleGrid->Cells[2][i] = SampleGrid->Cells[2][i-1];
       }
      // set the new values to the average of the surrounding ones
      if (SampleGrid->Row-1 > 0)
        t1 = atof(SampleGrid->Cells[1][SampleGrid->Row-1].c_str());
      else
        t1 = atof(SampleGrid->Cells[1][SampleGrid->Row].c_str())-1;
      if (SampleGrid->Row+1 < SampleGrid->RowCount)
        t2 = atof(SampleGrid->Cells[1][SampleGrid->Row+1].c_str());
      else
        t2 = atof(SampleGrid->Cells[1][SampleGrid->Row].c_str())+1;
      SampleGrid->Cells[1][SampleGrid->Row] =  FloatToStr(0.5*(t1+t2));
      if (SampleGrid->Row-1 > 0)
        t1 = atof(SampleGrid->Cells[2][SampleGrid->Row-1].c_str());
      else
        t1 = atof(SampleGrid->Cells[2][SampleGrid->Row].c_str())-1;
      if (SampleGrid->Row+1 < SampleGrid->RowCount)
        t2 = atof(SampleGrid->Cells[2][SampleGrid->Row+1].c_str());
      else
        t2 = atof(SampleGrid->Cells[2][SampleGrid->Row].c_str())+1;
      SampleGrid->Cells[2][SampleGrid->Row] = FloatToStr(0.5*(t1+t2));

      // free previous data arrays
	  Free_DVector(DataDef.XAxis[Display].DataArray,0);
	  Free_DVector(DataDef.YAxis[Display].DataArray,0);
	  Free_DMatrix(DataDef.Coeff[Display],0,0);
      // and create bigger ones
      DataDef.NoP[Display] = Samples;
      DataDef.XAxis[Display].DataArray = DVector(0,Samples-1);
	  DataDef.YAxis[Display].DataArray = DVector(0,Samples-1);
	  DataDef.Coeff[Display] = DMatrix(0,Samples-1,0,3);
 	  for (i=0;i<Samples;i++)
	    {
          // transfer strings into float array
          DataDef.XAxis[Display].DataArray[i] =
              atof(SampleGrid->Cells[1][i+1].c_str());
          DataDef.YAxis[Display].DataArray[i] =
              atof(SampleGrid->Cells[2][i+1].c_str());
		} // end for
      if (IntGroup->ItemIndex == CUBIC)
        FindPolyCoeff(DataDef.XAxis[Display].DataArray, DataDef.YAxis[Display].DataArray,
                    DataDef.NoP[Display], (DataDef.Coeff[Display]));
 //     if (SampleGrid->Row + 6 < SampleGrid->RowCount)
 //       SampleGrid->TopRow = SampleGrid->Row;
      DrawGraph(GraphBM->Canvas, TRUE);
      Invalidate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::DeleteSampleClick(TObject *Sender)
{
  int Samples;
  int i;
  Samples = SampleGrid->RowCount-1;
  if (Samples > 1)
    {
      // from the selected row onwards, move everything up
      for (i=SampleGrid->Row;i<SampleGrid->RowCount-1;i++)
       {
         SampleGrid->Cells[1][i] = SampleGrid->Cells[1][i+1];
         SampleGrid->Cells[2][i] = SampleGrid->Cells[2][i+1];
       }
      SampleGrid->RowCount -= 1;
      Samples -= 1;
      SampleNo->Text = IntToStr(Samples);
 //     SampleGrid->Height = SampleGrid->RowCount * 18;
      AdjustSampleGrid();
      //if (SampleGrid->Height > 122) SampleGrid->Height = 122;
      //if (SampleGrid->RowCount > 7) SampleGrid->ColWidths[2] = 91;
      //else SampleGrid->ColWidths[2] = 107;
      // free previous data arrays
	  Free_DVector(DataDef.XAxis[Display].DataArray,0);
	  Free_DVector(DataDef.YAxis[Display].DataArray,0);
	  Free_DMatrix(DataDef.Coeff[Display],0,0);
      // and create smaller ones
      DataDef.NoP[Display] = Samples;
      DataDef.XAxis[Display].DataArray = DVector(0,Samples-1);
	  DataDef.YAxis[Display].DataArray = DVector(0,Samples-1);
	  DataDef.Coeff[Display] = DMatrix(0,Samples-1,0,3);
	  for (i=0;i<Samples;i++)
	    {
          // transfer strings into float array
          DataDef.XAxis[Display].DataArray[i] =
              atof(SampleGrid->Cells[1][i+1].c_str());
          DataDef.YAxis[Display].DataArray[i] =
              atof(SampleGrid->Cells[2][i+1].c_str());
		} // end for
      if (IntGroup->ItemIndex == CUBIC)
        FindPolyCoeff(DataDef.XAxis[Display].DataArray, DataDef.YAxis[Display].DataArray,
                      DataDef.NoP[Display], (DataDef.Coeff[Display]));
  //    if (SampleGrid->Row + 6 < SampleGrid->RowCount)
  //      SampleGrid->TopRow = SampleGrid->Row;
      DrawGraph(GraphBM->Canvas, TRUE);
      Invalidate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::BrowseBtnClick(TObject *Sender)
{
  FileRadioBtn->Checked = TRUE;
  DataDef.Source = ID_FILE;
  if (OpenDialog->Execute())
    {
      FileName->Text = OpenDialog->FileName;
      strcpy(DataDef.FileName, FileName->Text.c_str());
    }
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::LoadBtnClick(TObject *Sender)
{
  bool Error;
  FileRadioBtn->Checked = TRUE;
  strcpy(DataDef.FileName, FileName->Text.c_str());
  DataDef.Source = ID_FILE;
  Error = LoadDataFile(DataDef.FileName,&DataDef);
  if (!Error)
 	{
      UpdateSampleGrid();
      InitGraph(DataDef.XAxis[Display].Start,DataDef.XAxis[Display].End,
        DataDef.YAxis[Display].Start,DataDef.YAxis[Display].End);
      DrawGraph(GraphBM->Canvas, TRUE);
 	  char s[MAXPATH+50];
 	  sprintf(s,"File '%s' successfully loaded.",DataDef.FileName);
 	  Application->MessageBox(s, "Info", MB_OK);
   }
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::SaveBtnClick(TObject *Sender)
{
  FileRadioBtn->Checked = TRUE;
  FILE *OutFilep;
  char s[512],s2[512];
  long i,j;

  strcpy(DataDef.FileName, FileName->Text.c_str());
  DataDef.Source = ID_FILE;
  OutFilep = fopen(DataDef.FileName, "w+t");  // append to file, if not exists, create it
  if (!OutFilep)
	 {
		sprintf(s,"Cannot open file '%s'",DataDef.FileName);
		Application->MessageBox(s, "Error", MB_OK | MB_ICONEXCLAMATION);
		return;
	 }
  // now save data
  sprintf(s,"! Data file for : ");
  for (i=0;i<DataDef.DataTypes;i++)
	 {
		sprintf(s2,"%s vs %s",DataDef.YAxis[i].Name,DataDef.XAxis[i].Name);
		strcat(s,s2);
		if (i < (DataDef.DataTypes-1))
		  strcat(s,", ");
		else
		  strcat(s,"\n");
	 }
  fprintf(OutFilep,s);
  sprintf(s,"! Structure : ");
  for (i=0;i<DataDef.DataTypes;i++)
	 {
		sprintf(s2,"[No. Samples] [%s 1] [%s 1] [%s 2] [%s 2] ...",
		DataDef.XAxis[i].Name,DataDef.YAxis[i].Name,
		DataDef.XAxis[i].Name,DataDef.YAxis[i].Name);
		strcat(s,s2);
		if (i < (DataDef.DataTypes-1))
		  strcat(s,"\n!             ");
		else
		  strcat(s,"\n\n");
	 }
  fprintf(OutFilep,s);
  for (i=0;i<DataDef.DataTypes;i++)
	 {
		sprintf(s,"%d \n",DataDef.NoP[i]);
		fprintf(OutFilep,s);
		for (j=0;j<DataDef.NoP[i];j++)
		  {
			 sprintf(s,"%-.15G  %-.15G",DataDef.XAxis[i].DataArray[j],
						DataDef.YAxis[i].DataArray[j]);
			 if (j < (DataDef.NoP[i]-1))
				strcat(s,", ");
			 fprintf(OutFilep,s);
		  }
		fprintf(OutFilep,"\n");
	 }
  fclose (OutFilep);
  sprintf(s,"File '%s' successfully saved.",DataDef.FileName);
  Application->MessageBox(s, "Info", MB_OK);

}
/*
//---------------------------------------------------------------------------
bool TGraphForm1::LoadDataFile(char FileName[],SDataDefinition *DataDef)
{
  FILE *OutFilep;
  double *DataArray;
  char s[MAX_PATH];
  char DataString[MAX_LENGTH_OF_PARA_STRING];
  long ItemCounter, CurrentItem;
  int CurrentDataType;
  long i;
  int ItemCode, LineCount = 0;
  BOOL Error;

  OutFilep = fopen(FileName, "rt");  // read from file, text
  if (!OutFilep)
	 {
		sprintf(s,"Cannot open file '%s'",FileName);
		Application->MessageBox(s, "Error", MB_OK | MB_ICONEXCLAMATION);
		Error = TRUE;
		return Error;
	 }
  // allocate memory to read whole file
  DataArray = DVector(0,(MAX_SAMPLES * MAX_VARIABLES_PER_GRAPH * 2)-1);
  ItemCounter = 0;
  // now load data
  do
	 {
		// Get the _ITEM_ code of the next item found in command file
		ItemCode = ParseOneItem(OutFilep, DataString, LineCount+1 );
		if ((ItemCode != _ITEM_EOF) && (ItemCode != _ITEM_EOL))
		 DataArray[ItemCounter++] = atof(DataString);
		if (ItemCounter >= MAX_SAMPLES)
		 {
			sprintf(s,"Too many items in file, limit = %d - truncated",MAX_SAMPLES);
			Application->MessageBox(s, "Error", MB_OK);
			break;
		 }
	 }  // end do
  while ( ItemCode != _ITEM_EOF );
  fclose (OutFilep);
  // check if data is OK
  CurrentItem = 0;
  CurrentDataType = 0;
  Error = FALSE;
  if (ItemCounter == 0)
	 {
		Application->MessageBox("No items found in file", "Error", MB_OK);
		Error = TRUE;
		return Error;
	 }
  else
	 {
		while ((CurrentItem < ItemCounter) && (CurrentDataType < DataDef->DataTypes))
		  {
			 if (DataArray[CurrentItem] != round(DataArray[CurrentItem]))
				{
				  sprintf(s,"Number of samples for %s must be an integer (not %-.6G)",
								DataDef->YAxis[CurrentDataType].Name,DataArray[CurrentItem]);
				  Application->MessageBox(s, "Error", MB_OK);
				  Error = TRUE;
				  return Error;
				}
			 if (DataArray[CurrentItem] < 1)
				{
				  sprintf(s,"Number of samples for %s must be greater than 0 (not %-.6G)",
							DataDef->YAxis[CurrentDataType].Name,DataArray[CurrentItem]);
				  Application->MessageBox(s, "Error", MB_OK);
				  Error = TRUE;
				  return Error;
				}
			 if ((CurrentItem + (DataArray[CurrentItem]*2)+1) > ItemCounter)
				{
				  sprintf(s,"%.0f items for %s expected, only %d found",DataArray[CurrentItem]*2,
				  DataDef->YAxis[CurrentDataType].Name, ItemCounter - CurrentItem - 1);
				  Application->MessageBox(s, "Error", MB_OK);
				  Error = TRUE;
				  return Error;
				}
			 CurrentItem += (DataArray[CurrentItem]*2)+1;
			 CurrentDataType++;
		  } // end while
		if ((CurrentDataType < DataDef->DataTypes) && !Error)
		 {
			sprintf(s,"%d Datatypes (%s etc.) expected, only %d found",DataDef->DataTypes,
					  DataDef->YAxis[0].Name, CurrentDataType);
			Application->MessageBox(s, "Error", MB_OK);
			Error = TRUE;
		  }
	 } // end else
  if (Error) Application->MessageBox("File was not loaded", "Message", MB_OK);
  else
	{
	  // now really insert data into array
	  CurrentItem = 0;
	  CurrentDataType = 0;
	  while ((CurrentItem < ItemCounter) && (CurrentDataType < DataDef->DataTypes))
		 {
			// free old data arrays
			Free_DVector(DataDef->XAxis[CurrentDataType].DataArray,0);
			Free_DVector(DataDef->YAxis[CurrentDataType].DataArray,0);
			Free_DMatrix(DataDef->Coeff[CurrentDataType],0,0);
				// number of samples
			DataDef->NoP[CurrentDataType] = DataArray[CurrentItem++];
				// allocate memory for the data
			DataDef->XAxis[CurrentDataType].DataArray = DVector(0,DataDef->NoP[CurrentDataType]-1);
			DataDef->YAxis[CurrentDataType].DataArray = DVector(0,DataDef->NoP[CurrentDataType]-1);
			DataDef->Coeff[CurrentDataType] = DMatrix(0,DataDef->NoP[CurrentDataType]-1,0,3);
				// copy data from temporary array
			for (i = 0; i < DataDef->NoP[CurrentDataType]; i++)
			  {
				 DataDef->XAxis[CurrentDataType].DataArray[i] = DataArray[CurrentItem++];
				 DataDef->YAxis[CurrentDataType].DataArray[i] = DataArray[CurrentItem++];
			  }
			// update interpolation coeffs
			if (IntGroup->ItemIndex == CUBIC)
              FindPolyCoeff(DataDef->XAxis[CurrentDataType].DataArray,
							  DataDef->YAxis[CurrentDataType].DataArray,
				 DataDef->NoP[CurrentDataType], (DataDef->Coeff[CurrentDataType]));
			// next data type
			CurrentDataType++;
		 } // end while
	}  // end else
  // free memory allocated
  Free_DVector(DataArray,0);
  return Error;
}
*/
void __fastcall TGraphForm1::InlineRadioBtnClick(TObject *Sender)
{
  DataDef.Source = ID_INLINE;
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::ZoomInXClick(TObject *Sender)
{
  InitGraph(Graph->XStart + 0.25 * Graph->XRange,
            Graph->XEnd - 0.25 * Graph->XRange,
            Graph->YStart,Graph->YEnd);
  DrawGraph(GraphBM->Canvas, FALSE);
  UpdateRange();
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::ZoomOutXClick(TObject *Sender)
{
  InitGraph(Graph->XStart - 0.5 * Graph->XRange,
            Graph->XEnd + 0.5 * Graph->XRange,
            Graph->YStart, Graph->YEnd);
  DrawGraph(GraphBM->Canvas, FALSE);
  UpdateRange();
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::ZoomInYClick(TObject *Sender)
{
  InitGraph(Graph->XStart, Graph->XEnd,
            Graph->YStart + 0.25 * Graph->YRange,
            Graph->YEnd - 0.25 * Graph->YRange);
  DrawGraph(GraphBM->Canvas, FALSE);
  UpdateRange();
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::ZoomOutYClick(TObject *Sender)
{
  InitGraph(Graph->XStart, Graph->XEnd,
            Graph->YStart - 0.5 * Graph->YRange,
            Graph->YEnd + 0.5 * Graph->YRange);
  DrawGraph(GraphBM->Canvas, FALSE);
  UpdateRange();
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::SampleGridKeyPress(TObject *Sender, char &Key)
{
  if ((Key == (unsigned char)ENTER_KEY) || (Key == (unsigned char)TAB_KEY))
    SampleGridExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::SampleGridExit(TObject *Sender)
{
  UpdateSampleValues();
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::SampleGridClick(TObject *Sender)
{
  UpdateSampleValues();
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm1::HelpButtonClick(TObject *Sender)
{
  Application->HelpJump("userdef");
}
//---------------------------------------------------------------------------
