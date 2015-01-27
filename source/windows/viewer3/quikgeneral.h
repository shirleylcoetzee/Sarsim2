//---------------------------------------------------------------------------
#ifndef quikgeneralH
#define quikgeneralH
//---------------------------------------------------------------------------
// class definitions
//---------------------------------------------------------------------------
class TBitM {
  private :
    // ------------- functions ----------------
    // ------------- variables ----------------
    BITMAPINFO *pbmiDIB;		// pointer to the BITMAPINFO
   public :
    // ------------- functions ----------------
    // constructor
    // Graphics::TBitmap *GraphBM;
    HBITMAP hDIBSection;        // handle of DIB section
    TBitM(HWND Hwnd, int Width, int Height);
    void Draw(TCanvas *ThisCanvas, HWND OutHwnd, int X, int Y, HPALETTE hpalDIB);
    void Resize(HWND OutWwnd, int Width, int Height);
    ~TBitM();                   // destructor
    // ------------- variables ----------------
    int DIBWidth, DIBHeight;    // width and height of bitmap
    char *pDIBBase;		        // pointers to DIB section we'll draw into
};
//---------------------------------------------------------------------------
// constants
//---------------------------------------------------------------------------
const unsigned char RED = 0;
const unsigned char GREEN = 1;
const unsigned char BLUE = 2;

const unsigned char GRAY_SCALE = 0;
const unsigned char RED_BLUE_SCALE = 1;
const unsigned char RAINBOW_SCALE = 2;
const unsigned char USERDEFINED_SCALE = 3;
/*
const unsigned char REAL      = 0;
const unsigned char IMAGINARY = 1;
const unsigned char MAGNITUDE = 2;
const unsigned char PHASE     = 3;

//const double PI = 3.14159265358979323;
//const double DegToRad = 0.017453292;
//const double RadToDeg = 57.29577951;
#define PI 3.14159265358979323
#define DegToRad 0.017453292
#define RadToDeg 57.29577951
*/
// colors available for bitmaps
const unsigned char NCOLOR = 236;

//---------------------------------------------------------------------------
// function prototypes
//---------------------------------------------------------------------------
bool CreateLogPal(LOGPALETTE **plogpal, unsigned char ColPal[236][3]);
void CreateRGBPal(unsigned char ColorPal[236][3],int Bright,
                            int Contrast, int Sat, unsigned char Type);
double HuetoRGB(double m1, double m2, double h );
void RGBtoHSL(double r, double g, double b, double *H, double *S, double *L );
void HLStoRGB( double H, double L, double S, unsigned char *r,
  unsigned char *g, unsigned char *b);
void CopyFileFormat(struct SFileFormat *Dest,
                                   struct SFileFormat *Src);

/*
void Swap(double *A, double *B);
bool PointInRect(TRect Rect, int x, int y);
double **DMatrix(long nrl, long nrh, long ncl, long nch);
void Free_DMatrix(double **m, long nrl, long ncl);
double GetValue(int what, double I, double Q);
double round(double x);
void SetRect(TRect *Rect, int left, int top, int right, int bottom);
double SciNotation(double N,int *expo);
*/

//---------------------------------------------------------------------------
#endif
