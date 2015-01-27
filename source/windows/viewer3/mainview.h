//---------------------------------------------------------------------------
#ifndef mainviewH
#define mainviewH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\Menus.hpp>
#include "loaddata.h"
#include "quikgeneral.h"
//---------------------------------------------------------------------------
struct SGraphData
  {
    double XStart;    // graph x-coordinate start value
    double XEnd;      // graph x-coordinate end value
    double YStart;    // graph y-coordinate start value
    double YEnd;      // graph y-coordinate end value
    double XMin;      // minimum x-value allowed
    double XMax;      // maximum x-value allowed
    double YMin;      // minimum y-value allowed
    double YMax;      // maximum y-value allowed
    double XRange;    // x range
    double YRange;    // y range
    TRect  Border;    // border between window and graph
    int    Display;   // I,Q,Mag, phase or dB Power
    TRect  MaxWindowSize; // Min and Max window size
      // bitmap area
    TRect GraphArea;
    // size of shown bitmap in pixels
    unsigned int PixelX, PixelY;
 //   int Interpolation;   // 0 = none, 1= LPF
  };
//---------------------------------------------------------------------------
class TMainViewForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TGroupBox *RGDisplay;
  TGroupBox *GBContrast;
  TScrollBar *SBBright;
  TGroupBox *GBWindow;
  TLabel *Label1;
  TLabel *LabelY;
  TLabel *Label2;
  TLabel *LabelX;
  TEdit *EXStart;
  TEdit *EXEnd;
  TEdit *EYStart;
  TEdit *EYEnd;
  TBitBtn *BAll;
  TRadioGroup *RGDrawData;
  TGroupBox *GroupBox1;
  TScrollBar *SBContrast;
  TGroupBox *GroupBox2;
  TScrollBar *SBSat;
  TRadioGroup *RadioGroup1;
  TGroupBox *GroupBox3;
  TRadioButton *RGReal;
  TRadioButton *RGImag;
  TRadioButton *RGMag;
  TRadioButton *RGPhase;
  TRadioButton *RGPower;
  TMemo *Memo1;
  TMainMenu *MainMenu1;
  TMenuItem *File1;
  TMenuItem *Open1;
  TMenuItem *N2;
  TMenuItem *SaveSelection1;
  TMenuItem *SaveasBitmap1;
  TMenuItem *N1;
  TMenuItem *Exit1;
  TMenuItem *Options1;
  TMenuItem *ColorPalette1;
  TMenuItem *Graphs1;
  TMenuItem *Help1;
  TBevel *Bevel2;
  TPanel *Panel3;
  TBevel *Bevel1;
  TBitBtn *ZoomInY;
  TBitBtn *ZoomOutY;
  TBitBtn *BAUp;
  TBitBtn *BADown;
  TBitBtn *ZoomInX;
  TBitBtn *ZoomOutX;
  TBitBtn *BRLeft;
  TBitBtn *BRRight;
  TPanel *Panel2;
  TBevel *Bevel8;
  TBevel *Bevel5;
  TBevel *Bevel6;
  TLabel *LX;
  TBevel *Bevel3;
  TLabel *Label6;
  TLabel *LY;
  TBevel *Bevel4;
  TLabel *Label8;
  TLabel *LIn;
  TLabel *Label9;
  TLabel *LQuad;
  TLabel *Label3;
  TLabel *Label4;
  TBevel *Bevel7;
  TLabel *LMag;
  TLabel *LPh;
  TTimer *Timer1;
  TLabel *Label5;
  TLabel *Label7;
  TBitBtn *BPalette;
  void __fastcall Open1Click(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);

  void __fastcall FormResize(TObject *Sender);
  void __fastcall FormPaint(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall Timer1Timer(TObject *Sender);
  void __fastcall EnterKeyPress(TObject *Sender, char &Key);
  void __fastcall EXStartExit(TObject *Sender);
  void __fastcall EYStartExit(TObject *Sender);
  void __fastcall EXEndExit(TObject *Sender);
  void __fastcall EYEndExit(TObject *Sender);
  
  void __fastcall FormDestroy(TObject *Sender);
  
  void __fastcall SBBrightScroll(TObject *Sender, TScrollCode ScrollCode,
  int &ScrollPos);
  void __fastcall SBContrastScroll(TObject *Sender, TScrollCode ScrollCode,
  int &ScrollPos);
  void __fastcall SBSatScroll(TObject *Sender, TScrollCode ScrollCode,
  int &ScrollPos);
  void __fastcall BPaletteClick(TObject *Sender);
  void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
  TShiftState Shift, int X, int Y);
  void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X,
  int Y);
  void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
  TShiftState Shift, int X, int Y);
  
  
  
  
private:	// User declarations
    // graph details
    struct SGraphData GD;
    // the format of the current loaded file
    struct SFileFormat FF;
    // graph has been resized
    bool UpdateSize;
    // contents of graph needs updating
    bool UpdateContents;
    bool Busy;  // busy updating - don't redraw
    // Is a file loaded ?
    bool FileLoaded;
    // side graphs
    bool SideGraphs;
    int WidthLeftGraph, HeightBottomGraph;
    Graphics::TBitmap *LeftGraphBM;  // bitmap used to show cross-sections
    Graphics::TBitmap *BottomGraphBM;  // bitmap used to show cross-sections
    double **Data1, **Data2; // graph data array = bitmap (size of bitmap)
    // initialized all variables ?
    bool InitVar;
    // current color palette
    unsigned char ColPal[NCOLOR][3];
    class TBitM *PalBM;       // current palette bitmap
    HPALETTE Cpal;           // current palette
    class TBitM *ThumbBM;     // full image bitmap
    class TBitM *GraphBM;     // main graph window bitmap

    // is left or right mouse button down ?
    bool LeftButtonDown;
    bool RightButtonDown;
  	TPoint        ClickPoint;          // coordinates of point clicked
	  TRect         ZoomRect;            // rectangle to be zoomed in
    // used to determine which cursor to show
    bool CursorCross;

    void SetNewGraphSize(void);
    void SetupGraphSize();
    void DrawFrame(TCanvas *Canvas, TRect GraphArea, double XStart, double XEnd,
               double YStart, double YEnd);
    void LoadDataGivenRange(double XStart, double XRange, long PixelX,
       double YStart, double YRange, long PixelY, double **D1, double **D2);
    void SetNewGraphRange(double *XStart, double *XEnd,
                                       double *YStart, double *YEnd);
    void UpdateRange(void);
    void UpdateDialogsGivenFileFormat();
    void MakePalette(unsigned char Type,int Bright,
                            int Contrast, int Sat);
    void SetupPaletteBitmap();
    void DrawArray(class TBitM *BM, double **Data1, double **Data2,
         unsigned long PixelX, unsigned long PixelY, int DisplayType,
         int SBContrast, int SBBright,
               int SBSat,unsigned char ColPal[NCOLOR][3]);
    void WindowToReal(int PixX, int PixY, double *X, double *Y);
    void RealToWindow(double X, double Y, int *PixX, int *PixY);
    void InterpolateArray(double **A1D1, double **A1D2,
   long A1SizeX, long A1SizeY, double **A2D1, double **A2D2,
   long A2SizeX, long A2SizeY);

public:		// User declarations
    unsigned char ColorPal;            // which palette (red-blue, gray etc.)
  __fastcall TMainViewForm(TComponent* Owner);
    void virtual __fastcall RestrictSize(TMessage& Msg);
    // This message map maps the WM_GETMINMAXINFO message to the
    // RestrictSize function.
    BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER(WM_GETMINMAXINFO,TMessage,RestrictSize)
      // Add any number of additional message handlers here.
    END_MESSAGE_MAP(TForm)

};
//---------------------------------------------------------------------------
extern TMainViewForm *MainViewForm;
//---------------------------------------------------------------------------
#endif
