//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "graph.h"
#include "objectstructdef.h"
#include "general.h"
#include <math.h>
#include <values.h>
//---------------------------------------------------------------------------

// global variables
const char *UnitStr[8][7] = { {"·10^-9","·10^-6","·10^-3","","·10^3","·10^6","·10^9"},
                              {"nm","µm","mm","m","km","·10^3 km","·10^6 km"},
                              {"ns","µs","ms","s","·10^3 s","·10^6 s","·10^6 s"},
                              {"n°","µ°","m°","°","·10^3°","·10^6°","·10^9°"},
                              {"nV","µV","mV","V","kV","MV","GV"},
                              {"nHz","µHz","mHz","Hz","kHz","MHz","GHz"},
                              {"·10^-9 dB","·10^-6 dB","·10^-3 dB","dB","·10^3 dB","·10^6 dB","·10^9 dB"},
                              {"Hz","kHz","MHz","GHz","THz","·10^6GHz","·10^9GHz"},
                            };

//-------------------------------------------------------------------------//
// Class Member Graph::Graph (Constructor)                                 //
// -------------------------                                               //
// Inputs  : -none-                                                        //
//                                                                         //
// Outputs : -none-                                                        //
// Globals : -none-                                                        //
//                   																		//
// Initializes fonts and pens.                                             //
//-------------------------------------------------------------------------//
TGraph::TGraph(TCanvas *ThisCanvas,int UnitsX1,
                 int UnitsY1, int XDecPlaces1, int YDecPlaces1)
  {
    GraphCanvas = ThisCanvas;
    UnitsX = UnitsX1;
    UnitsY = UnitsY1;
    XDecPlaces = XDecPlaces1;
    YDecPlaces = YDecPlaces1;
  }

//-------------------------------------------------------------------------//
// Class Member Graph::~Graph (Destructor)                                 //
// --------------------------                                              //
// Inputs  : -none-                                                        //
//                                                                         //
// Outputs : -none-                                                        //
// Globals : -none-                                                        //
//                   																		//
// Frees fonts and pens.                                                   //
//-------------------------------------------------------------------------//
TGraph::~TGraph(void)
  {
  }


//-------------------------------------------------------------------------//
// Class Member TGraph::Init                                               //
// -------------------------                                               //
// Inputs  : TRect    Border - Position of square containing graph         //
//           TCanvas* ThisCanvas - canvas of graph                                                     //
//           double   XStart                                               //
//                    XEnd                                                 //
//                    YStart                                               //
//                    YEnd   - Extreme coordinates of X and Y axis         //
//           int      UnitsX                                               //
//                    UnitsY - units of graph labels (none, s, m etc.)     //
//           int      XDecPlaces                                           //
//                    YDecPlaces - Places behind decimal point to show     //
//                                                                         //
// Outputs : -none-                                                        //
// Globals : -none-                                                        //
//                   							   //
// Initializes graph variables, fonts and pens.                            //
//-------------------------------------------------------------------------//
void TGraph::Init(TRect Border,double XStart1,
                 double XEnd1, double YStart1,double YEnd1)
  {
    GraphArea = Border;
    //GraphArea.Left = Border.Left;
    //GraphArea.Right = Border.Right;
    //GraphArea.Top = Border.Top;
    //GraphArea.Bottom = Border.Bottom;
    XStart = XStart1;
    XEnd = XEnd1;
    YStart = YStart1;
    YEnd = YEnd1;
    XRange = XEnd - XStart;
    YRange = YEnd - YStart;
    // notice that the 'drawable' area of the graph excludes the
    // borders and lower right corner, i.e. if Border =
    // (10,10,100,100) then usable area
    // would be from (11,11,99,99) i.e. 89 x 89 pixels
    PixelX = (Border.Right-Border.Left)-1;
    PixelY = (Border.Bottom-Border.Top)-1;
  }

//-------------------------------------------------------------------------//
// Class Member TGraph::Paint                                              //
// --------------------------                                              //
// Inputs  : TRect   Border - Position of square containing graph          //
// Outputs : -none-                                                        //
// Globals : -none-                                                        //
//                   																		//
// Initializes graph variables, fonts and pens.                            //
//-------------------------------------------------------------------------//
void TGraph::Paint(int What)
  {
    TRect Frame;
    // all-purpose rect
    TRect TempRect;

    // note that FillRect and FrameRect draws excluding lower, right corner,
    // so we add one to either side
    Frame = GraphArea;
    Frame.Right++;
    Frame.Bottom++;
    // set font to 8 pt
    GraphCanvas->Font->Size = 8;
    // repaint specified objects
    switch (What) {
      case FRAME:
        {
          // set drawing mode to normal
          GraphCanvas->Pen->Mode = pmCopy;
          // gray frame
//col          GraphCanvas->Brush->Color = clGray;
          GraphCanvas->Brush->Color = clLtGray;//White;
          GraphCanvas->FrameRect(Frame);
          // draw shadow
//col          GraphCanvas->Brush->Color = clBtnShadow;
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
          break;
        }
      case UNITS:
        {
          double CValue; // current value
	      double Mark, Ticks, Unit;
	      int StartTick, expo, TextSpace,i,TextWidth,TextHeight;
          char str[80];
          // get background color
         // TColor BackColor = GraphCanvas->Pixels[GraphArea.Left - 15][GraphArea.Bottom + 1];

          // overpaint old x units
          GraphCanvas->Brush->Color = clGray; //BackColor;//clSilver;
//          GraphCanvas->Brush->Style = bsClear;
          TempRect.Left = GraphArea.Left - 15;
          TempRect.Right = GraphArea.Right + 15;
          TempRect.Top = GraphArea.Bottom + 1;
          TempRect.Bottom = GraphArea.Bottom + 21;
          GraphCanvas->FillRect(TempRect);

          // overpaint old y units
          GraphCanvas->Brush->Color = clGray; //BackColor;
          TempRect.Left = GraphArea.Left - 65;
          TempRect.Right = GraphArea.Left - 1;
          TempRect.Top = GraphArea.Top - 6;
          TempRect.Bottom = GraphArea.Bottom + 6;
          GraphCanvas->FillRect(TempRect);

	      // how much space is available for text ?
	      TextSpace = PixelX;
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
	          Mark = PixelX*((CValue-XStart)/XRange);
	          if (Mark < PixelX)
		        {
		          GraphCanvas->Pen->Width = 1;
//col		          GraphCanvas->Pen->Color = clRed;
		          GraphCanvas->Pen->Color = clBlack;
                  GraphCanvas->MoveTo(GraphArea.Left + Mark + 1, GraphArea.Bottom+1);
		          GraphCanvas->LineTo(GraphArea.Left + Mark + 1, GraphArea.Bottom+5);
		          CreateLabel(CValue,XDecPlaces,UnitsX,str);
//col		          GraphCanvas->Font->Color = clBlack;
		          GraphCanvas->Font->Color = clWhite;
              GraphCanvas->Brush->Color = clLtGray;
		          GraphCanvas->Brush->Style = bsClear;
		           TextWidth = GraphCanvas->TextWidth(str);
		          if (TextSpace >= (TextWidth+5))
		            {
		              GraphCanvas->TextOut(GraphArea.Left + Mark-TextWidth*0.5 + 2,
                                           GraphArea.Bottom+8,str);
		              TextSpace = (Unit / XRange)*PixelX;
		            }
		          else
		            TextSpace += (Unit / XRange)*PixelX;
		        } // end if
	        }  // end for i

 	      // how much space is available for text ?
	      TextSpace = PixelY;
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
	          Mark = PixelY*((CValue-YStart)/YRange);
	          if (Mark < PixelY)
		        {
		          GraphCanvas->Pen->Width = 1;
//col		          GraphCanvas->Pen->Color = clRed;
		          GraphCanvas->Pen->Color = clBlack;
                  GraphCanvas->MoveTo(GraphArea.Left - 2, GraphArea.Bottom - Mark );
		          GraphCanvas->LineTo(GraphArea.Left - 6, GraphArea.Bottom - Mark );
		          GraphCanvas->MoveTo(GraphArea.Left - 2, GraphArea.Bottom - Mark - 1);
		          GraphCanvas->LineTo(GraphArea.Left - 6, GraphArea.Bottom - Mark - 1);
		          CreateLabel(CValue,YDecPlaces,UnitsY,str);
//col		          GraphCanvas->Font->Color = clBlack;
		          GraphCanvas->Font->Color = clWhite;
              GraphCanvas->Brush->Color = clLtGray;
              GraphCanvas->Brush->Style = bsClear;

		          TextWidth = GraphCanvas->TextWidth(str);
		          TextHeight = GraphCanvas->TextHeight(str);
		          if (TextSpace >= (TextHeight+2))
		            {
		              GraphCanvas->TextOut(GraphArea.Left - TextWidth - 8,
                          GraphArea.Bottom - Mark - TextHeight * 0.5 - 2,str);
		              TextSpace = (Unit / YRange)*PixelY;
		            }
		          else
		            TextSpace += (Unit / YRange)*PixelY;
		        }  // end if
	        }  // end for i
          break;
        }
    } // end switch
 }

//-------------------------------------------------------------------------//
// Class Member TGraph::CreateLabel                                        //
// --------------------------------                                        //
// Inputs  : double Value - number to be formated                          //
//           int    DecPlaces - decimal places                             //
//           int    Units - none/meters/seconds etc.                       //
// Outputs : char[] str - string containing formated number                //
// Globals : -none-                                                        //
//-------------------------------------------------------------------------//
void TGraph::CreateLabel(double Value,int DecPlaces, int Units, char *str)
{
  if (Units == 0) // we don't want units
    {
      if (DecPlaces < 0)
        sprintf(str,"%-.15G ",Value);
      else
        sprintf(str,"%.*f ",DecPlaces,Value);
      return;
    }

  // if DecPlaces < 0 it means the number of decimal places should
  // be adjusted automatically
  if (DecPlaces < 0)
    {
      if (fabs(Value) < 1E-10) sprintf(str,"0 %s",UnitStr[Units][3]);
      else if (fabs(Value) < 1E-8) sprintf(str,"%-.15G %s",Value*1E9,UnitStr[Units][0]);
      else if (fabs(Value) < 1E-5) sprintf(str,"%-.15G %s",Value*1E6,UnitStr[Units][1]);
      else if (fabs(Value) < 1E-2) sprintf(str,"%-.15G %s",Value*1E3,UnitStr[Units][2]);
      else if (fabs(Value) < 1E+3) sprintf(str,"%-.15G %s",Value,UnitStr[Units][3]);
      else if (fabs(Value) < 1E+6) sprintf(str,"%-.15G %s",Value*1E-3,UnitStr[Units][4]);
      else if (fabs(Value) < 1E+9) sprintf(str,"%-.15G %s",Value*1E-6,UnitStr[Units][5]);
      else if (fabs(Value) < 1E+12) sprintf(str,"%-.15G %s",Value*1E-9,UnitStr[Units][6]);
      else sprintf(str,"%-.15G %s",Value,UnitStr[Units][3]);
    }
  else  // use given number of decimal places
    {
      if (fabs(Value) < 1E-10) sprintf(str,"0 %s",UnitStr[Units][3]);
      else if (fabs(Value) < 1E-8) sprintf(str,"%.*f %s",DecPlaces,Value*1E9,UnitStr[Units][0]);
      else if (fabs(Value) < 1E-5) sprintf(str,"%.*f %s",DecPlaces,Value*1E6,UnitStr[Units][1]);
      else if (fabs(Value) < 1E-2) sprintf(str,"%.*f %s",DecPlaces,Value*1E3,UnitStr[Units][2]);
      else if (fabs(Value) < 1E+3) sprintf(str,"%.*f %s",DecPlaces,Value,UnitStr[Units][3]);
      else if (fabs(Value) < 1E+6) sprintf(str,"%.*f %s",DecPlaces,Value*1E-3,UnitStr[Units][4]);
      else if (fabs(Value) < 1E+9) sprintf(str,"%.*f %s",DecPlaces,Value*1E-6,UnitStr[Units][5]);
      else if (fabs(Value) < 1E+12) sprintf(str,"%.*f %s",DecPlaces,Value*1E-9,UnitStr[Units][6]);
      else sprintf(str,"%.*f %s",DecPlaces,Value,UnitStr[Units][3]);
    }
}
//-------------------------------------------------------------------------//
// Class Member TGraph::DrawLineCanvas                                     //
// -----------------------------------                                     //
// Inputs  :                                                               //
//                                                                         //
//                                                                         //
// Outputs : -none-                                                        //
//                                                                         //
// Globals : -none-                                                        //
//                                                                         //
//                                                                         //
//-------------------------------------------------------------------------//
void TGraph::DrawLineCanvas(TCanvas *ThisCanvas, double X1, double Y1,
                            double X2, double Y2)
{
  int GraphX1, GraphX2, GraphY1, GraphY2;
  // convert the values back to screen coordinates
  GraphX1 = (X1 - XStart)*(double(PixelX)/XRange);
  GraphY1 = (YEnd - Y1)*(double(PixelY)/YRange);
  GraphX2 = (X2 - XStart)*(double(PixelX)/XRange);
  GraphY2 = (YEnd - Y2)*(double(PixelY)/YRange);
  ThisCanvas->MoveTo(GraphX1, GraphY1);
  ThisCanvas->LineTo(GraphX2, GraphY2);
  // make sure endpoints are also drawn
  ThisCanvas->Pixels[GraphX1][GraphY1] = ThisCanvas->Pen->Color;
  ThisCanvas->Pixels[GraphX2][GraphY2] = ThisCanvas->Pen->Color;
}

void TGraph::DrawGraph(TCanvas *ThisCanvas, long NP, double *XP, double *YP,
                      double **Coeff,int GraphType, int IntMethod)
{
  int GraphX, GraphY;
  double X,Y;
  long i;
  if (NP > 0)
    {
      if ((GraphType == LINE_ONLY) || (GraphType == LINE_MARKER))
	    {
		  // draw the graph with simple clipping (line only)
          ThisCanvas->Brush->Color = clSilver;
		  ThisCanvas->Pen->Color = clBlue;
          ThisCanvas->Pen->Width = 1;
		  for (GraphX = 0; GraphX < PixelX; GraphX++)
		    {
			  // for each screen pixel find the corresponding X-value
			  X = (double(GraphX)/double(PixelX))*
					XRange + XStart;
			  // for the given X-value, find the Y-value
			  Y = Interpolate(NP, XP, YP, Coeff,  X, IntMethod);
			  // convert the Y-value back to screen coordinates
			  GraphY = (PixelY - 1) - (Y - YStart)*(double(PixelY)/YRange);
			  // and draw the line
			  if (GraphX == 0)
			    ThisCanvas->MoveTo(GraphX, GraphY);
			  else
			    ThisCanvas->LineTo(GraphX, GraphY);
		  }
	    } // end if

      if ((GraphType == MARKER_ONLY) || (GraphType == LINE_MARKER)
		   || (GraphType == BAR))
        {
	      // draw markers (circles) for the specified points
	      for (i=0;i<NP;i++)
		    {
			  if (  ( (XP[i] >= XStart) && (XP[i] <= XEnd) &&
						(YP[i] >= YStart) && (YP[i] <= YEnd) ) ||
					 ( (XP[i] >= XStart) && (XP[i] <= XEnd) &&
						(GraphType == BAR) )  )
				{
				  GraphX = ((XP[i] - XStart)/XRange)*double(PixelX);
				  Y = YP[i];
				  GraphY = (PixelY - 1) - (Y - YStart)*(double(PixelY)/YRange);
				  ThisCanvas->Pen->Color = clRed;
				  ThisCanvas->Ellipse(GraphX-2, GraphY-2, GraphX+3, GraphY+3);
				  if (GraphType == BAR)
					{
                      // only draw line if within window
					  if (GraphY < PixelY)
						{
						  if (GraphY < 0) GraphY = 0;
						  ThisCanvas->Pen->Color = clBlue;
						  ThisCanvas->MoveTo(GraphX, GraphY);
						  ThisCanvas->LineTo(GraphX, PixelY-1);
						}
					} // end if
				}
		    } // end for
	    } // end if
  }  // end if NP > 0

}

