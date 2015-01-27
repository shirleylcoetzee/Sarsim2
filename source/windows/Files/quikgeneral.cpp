//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "quikgeneral.h"
#include "math.h"

//-------------------------------------------------------------------------//
// create a logical palette from given color array
//-------------------------------------------------------------------------//
bool CreateLogPal(LOGPALETTE **plogpal, unsigned char ColPal[NCOLOR][3])
{
  int i;
  // there are 236 non-system slots available - let's use them all
  *plogpal = (LOGPALETTE*)new char[sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * NCOLOR];
  if (*plogpal == NULL) return false;

  (*plogpal)->palVersion = 0x300;
  (*plogpal)->palNumEntries = NCOLOR;

  for (i=0; i<NCOLOR; i++)
    {
   	  (*plogpal)->palPalEntry[i].peRed = ColPal[i][0];
      (*plogpal)->palPalEntry[i].peGreen = ColPal[i][1];
      (*plogpal)->palPalEntry[i].peBlue = ColPal[i][2];
//      (*plogpal)->palPalEntry[i].peFlags = PC_RESERVED | PC_NOCOLLAPSE;
      (*plogpal)->palPalEntry[i].peFlags = PC_NOCOLLAPSE;
    }
  // success
  return true;
}
//---------------------------------------------------------------------------
void CreateRGBPal(unsigned char ColorPal[NCOLOR][3],int Bright,
                            int Contrast, int Sat, unsigned char Type)
{
  int i;
  if (Type == GRAY_SCALE)   // gray scale
    {
      // n goes from 0.2 (0), 1 (127) to 3.4 (255)
      double n = 1.0/(0.0000492*Bright*Bright + 0.2);
      double m = 0.01271*Contrast*Contrast + 50;
      double y;
      for (i=0;i<NCOLOR;i++)
        {
          y = (pow(double(i),n)*(double(1)/pow(NCOLOR,n)))*m -127 + Sat;
          if (y<0) y=0;
          if (y>255) y=255;
          ColorPal[i][RED] = (unsigned char)y;
          ColorPal[i][GREEN] = (unsigned char)y;
          ColorPal[i][BLUE] = (unsigned char)y;
        }
    }
  else if (Type == RED_BLUE_SCALE) // red-blue
    {
      // n goes from 0.2 (0), 1 (127) to 3.4 (255)
      double n = 1.0/(0.0000492*Bright*Bright + 0.2);
      // m goes from 50 to 875
      double m = 0.01271*Contrast*Contrast + 50;
      double y;
      for (i=0;i<NCOLOR*0.5;i++)
        {
          y = (pow(double(2*i),n)*(double(1)/pow(NCOLOR,n)))*m -127 + Sat;
          if (y<0) y=0;
          if (y>255) y=255;
          ColorPal[(int)(NCOLOR*0.5-1)-i][RED] = (unsigned char)0;
          ColorPal[(int)(NCOLOR*0.5-1)-i][GREEN] = (unsigned char)0;
          ColorPal[(int)(NCOLOR*0.5-1)-i][BLUE] = (unsigned char)y;
        }
      for (i=NCOLOR*0.5;i<NCOLOR;i++)
        {
          y = (pow(double(2*i-NCOLOR),n)*(double(1)/pow(NCOLOR,n)))*m -127 + Sat;
          if (y<0) y=0;
          if (y>255) y=255;
          ColorPal[i][RED] = (unsigned char)y;
          ColorPal[i][GREEN] = (unsigned char)0;
          ColorPal[i][BLUE] = (unsigned char)0;
        }
    }
  else if (Type == RAINBOW_SCALE) // rainbow
    {
      double H,L,S;
      unsigned char r,g,b;
      double Con = 0.3 + Contrast * 0.00274;
      for (i=0;i<NCOLOR;i++)
        {
          H = Con-((double)i/(double)NCOLOR)*Con;
          L = (double)Bright / (double)256;
          S = (double)Sat / (double)256;
          HLStoRGB(H, L, S, &r, &g, &b);
          ColorPal[i][RED] = r;
          ColorPal[i][GREEN] = g;
          ColorPal[i][BLUE] = b;
       }
    }
  else if (Type == USERDEFINED_SCALE) // user
    {
      double H,L,S;
      unsigned char r,g,b;
      for (i=0;i<NCOLOR;i++)
        {
          L = (double)i/(double)NCOLOR;
          H = (double)Bright / (double)256;
          S = (double)Sat / (double)256;
          HLStoRGB(H, L, S, &r, &g, &b);
          ColorPal[i][RED] = r;
          ColorPal[i][GREEN] = g;
          ColorPal[i][BLUE] = b;
       }
    }
}
//---------------------------------------------------------------------------
TBitM::TBitM(HWND OutHwnd, int Width, int Height)
{
  DIBWidth = Width;
  DIBHeight = Height;
  // set up the DIB section
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
  // get base pointer
  HDC BMdc = GetDC(OutHwnd);
  hDIBSection = CreateDIBSection (BMdc, pbmiDIB, DIB_PAL_COLORS,
                          (void **)&pDIBBase, NULL, 0);
  ReleaseDC(OutHwnd, BMdc);

  //GraphBM = new Graphics::TBitmap();
}
//---------------------------------------------------------------------------
void TBitM::Resize(HWND OutHwnd, int Width, int Height)
{
  delete[] pbmiDIB;
  DIBWidth = Width;
  DIBHeight = Height;
  // set up the DIB section
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
  // get base pointer
  HDC BMdc = GetDC(OutHwnd);
  DeleteObject(hDIBSection);
  hDIBSection = CreateDIBSection (BMdc, pbmiDIB, DIB_PAL_COLORS,
                          (void **)&pDIBBase, NULL, 0);
  ReleaseDC(OutHwnd, BMdc);
  //delete GraphBM;
  //GraphBM = new Graphics::TBitmap();
}
//---------------------------------------------------------------------------
TBitM::~TBitM()
{
  delete[] pbmiDIB;
  //delete GraphBM;
}
//---------------------------------------------------------------------------
void TBitM::Draw(TCanvas *ThisCanvas, HWND OutHwnd,int X, int Y, HPALETTE hpalDIB)
{
//  GraphBM->Handle = hDIBSection;
//  GraphBM->Palette = hpalDIB;
//  ThisCanvas->Draw(X,Y,GraphBM);

  // straight window bitmap blt
  HDC             hdcScreen, hdcDIBSection;
  HBITMAP         holdbitmap;
  HPALETTE        holdpal;

  // We've drawn the frame; copy it to the screen
  hdcScreen = GetDC(OutHwnd);
  holdpal = SelectPalette(hdcScreen, hpalDIB, FALSE);
  RealizePalette(hdcScreen);

  hdcDIBSection = CreateCompatibleDC(hdcScreen);
  holdbitmap = SelectObject(hdcDIBSection, hDIBSection);

  BitBlt(hdcScreen, X, Y, DIBWidth, DIBHeight, hdcDIBSection, 0, 0, SRCCOPY);
  SelectPalette(hdcScreen, holdpal, FALSE);
  ReleaseDC(OutHwnd, hdcScreen);
  SelectObject(hdcDIBSection, holdbitmap);
  DeleteDC(hdcDIBSection);

}
//-------------------------------------------------------------------------//
double HuetoRGB(double m1, double m2, double h )
{
   if( h < 0 ) h += 1.0;
   if( h > 1 ) h -= 1.0;
   if( 6.0*h < 1 )
      return (m1+(m2-m1)*h*6.0);
   if( 2.0*h < 1 )
      return m2;
   if( 3.0*h < 2.0 )
      return (m1+(m2-m1)*((2.0/3.0)-h)*6.0);
   return m1;
}
//-------------------------------------------------------------------------//
void HLStoRGB( double H, double L, double S, unsigned char *r,
  unsigned char *g, unsigned char *b)
{
 // double r,g,b;
  double m1, m2;

  if(S==0)
    {
     *r=*g=*b=L;
    }
  else
    {
     if(L <=0.5)
        m2 = L*(1.0+S);
     else
        m2 = L+S-L*S;
     m1 = 2.0*L-m2;
     *r = HuetoRGB(m1,m2,H+1.0/3.0)*double(255);
     *g = HuetoRGB(m1,m2,H)*double(255);
     *b = HuetoRGB(m1,m2,H-1.0/3.0)*double(255);
    }
//  return RGB((BYTE)(r*255),(BYTE)(g*255),(BYTE)(b*255));
}
//-------------------------------------------------------------------------//
void RGBtoHSL(double r, double g, double b, double *H, double *S, double *L )
{
   double delta;
/*   double r = (double)GetRValue(rgb)/255;
   double g = (double)GetGValue(rgb)/255;
   double b = (double)GetBValue(rgb)/255;
*/   double cmax = max(r,max(g,b));
   double cmin = min(r,min(g,b));
   *L=(cmax+cmin)/2.0;
   if(cmax==cmin) {
      *S = 0;
      *H = 0; // it's really undefined
   } else {
      if(*L < 0.5)
         *S = (cmax-cmin)/(cmax+cmin);
      else
         *S = (cmax-cmin)/(2.0-cmax-cmin);
      delta = cmax - cmin;
      if(r==cmax)
          *H = (g-b)/delta;
      else if(g==cmax)
          *H = 2.0 +(b-r)/delta;
      else
          *H=4.0+(r-g)/delta;
      *H /= 6.0;
      if(*H < 0.0)
          *H += 1;
   }
}
/*
//---------------------------------------------------------------------------
void Swap(double *A, double *B)
{
  double temp;
  temp = *A;
  *A = *B;
  *B = temp;
}
//-------------------------------------------------------------------------//
bool PointInRect(TRect Rect, int x, int y)
  {
    if ( (x > Rect.Left) && (x < Rect.Right) &&
         (y > Rect.Top)  && (y < Rect.Bottom) )
      return TRUE;
    else
      return FALSE;
  }
//-------------------------------------------------------------------------//
// Function DMatrix                                                        //
//                                                                         //
// This function creates allocates memory for a DOUBLE array of size       //
// (nrh-nrl)+1 x (nch-ncl)+1. It returns a pointer to this array.          //
//-------------------------------------------------------------------------//
double **DMatrix(long nrl, long nrh, long ncl, long nch)
{
  // make sure nrh > nrl etc.
  if (nrh<nrl)	nrh = nrl;
  if (nch<ncl)	nch = ncl;

  long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
  double **m;

  m=(double **) malloc((size_t)((nrow+1)*sizeof(double*)));
  if (!m) return NULL;
  m += 1;
  m -= nrl;

  m[nrl]=(double *) malloc((size_t)((nrow*ncol+1)*sizeof(double)));
  if (!m[nrl]) return NULL;
  m[nrl] += 1;
  m[nrl] -= ncl;

  for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

  return m;
}

//-------------------------------------------------------------------------//
// Function Free_DMatrix                                                   //
//                                                                         //
// This function frees the memory  allocated by DMatrix function.          //
//-------------------------------------------------------------------------//
void Free_DMatrix(double **m, long nrl, long ncl)
{
  free((char *) (m[nrl]+ncl-1));
  free((char *) (m+nrl-1));
}

//-------------------------------------------------------------------------//
double GetValue(int what, double I, double Q)
{
  switch (what)
	 {
		case REAL:	{ return I; }
		case IMAGINARY: { return Q; }
		case MAGNITUDE: { return sqrt(I * I + Q * Q); }
		case PHASE: {
				if ((I != 0) && (Q != 0)) return atan2(I,Q);
				else return 0;
		}
	 } // end switch
  return 0;
}
//-------------------------------------------------------------------------//
// Function round()                                                        //
//                                                                         //
// Round to the nearest integer.                                           //
//-------------------------------------------------------------------------//
double round(double x)
{
  if (x-floor(x)<0.5) return floor(x); else return ceil(x);
}

//---------------------------------------------------------------------------
void SetRect(TRect *Rect, int left, int top, int right, int bottom)
  {
    Rect->Left = left;
    Rect->Right = right;
    Rect->Top = top;
    Rect->Bottom = bottom;
  }
//-------------------------------------------------------------------------//
// Class Member SciNotation                                                //
// ------------------------                                                //
// Inputs  : double N       - actual number                                //
// Outputs : double mant10  - mantissa                                     //
//           int    &expo   - exponent                                     //
// Globals : -none-                                                        //
//                                                                         //
// Returns the mantissa and exponent of a number (mantissa between +/-1.00 //
// and +/-9.99999999)                                                      //
// e.g. 434.999 = 4.34999 * 10^2 => mantissa = 4.34999, exponent=2         //
// Function also works with negative numbers and negative exponents,       //
// e.g. -0.000332 = -3.32 * 10^-4 => mantissa = -3.32, exponent=-4         //
//-------------------------------------------------------------------------//
double SciNotation(double N,int *expo)
{
  int expo2;
  double mant2,expo10,mant10,frac_expo10;

  // calculate the power 2 mantissa (mant2) and exponent (expo2)
  mant2 = frexp(N, &expo2);
  // find the basis 10 exponent by multiplying with log(2)
  // i.e. a*2^b = a*10^(b*(log2))
  // as exponent (b*log2) might not be an integer, multiply
  // a by 10^(fraction of (b*log2)
  frac_expo10 = modf((double)expo2*0.301029995663981198,&expo10);
  mant10 = pow((double)10,frac_expo10)*mant2;
  // adjust values so output is in given range
  if (fabs(mant10) < 1) {mant10 *= 10; expo10 -= 1;}
  if (fabs(mant10) < 1) {mant10 *= 10; expo10 -= 1;}

  *expo = (int)expo10;
  return mant10;
}
*/







