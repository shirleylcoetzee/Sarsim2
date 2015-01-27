//---------------------------------------------------------------------------
#ifndef mainquikviewH
#define mainquikviewH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\Menus.hpp>
#include <vcl\Dialogs.hpp>

#include "quikgeneral.h"
#include "loadfile.h"
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
	TMemo *Memo1;
	TBitBtn *ZoomInX;
	TBitBtn *ZoomOutX;
	TBitBtn *BRLeft;
	TBitBtn *BRRight;
	TPanel *Panel2;
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
	TBitBtn *ZoomInY;
	TBitBtn *ZoomOutY;
	TBitBtn *BAUp;
	TBitBtn *BADown;
	TPanel *Panel3;
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *Open1;
	TMenuItem *Exit1;
	TMenuItem *Options1;
	TMenuItem *Help1;
	TBevel *Bevel1;
	TBevel *Bevel2;
	TTimer *Timer1;
	TLabel *Label3;
	TLabel *Label4;
	TBevel *Bevel7;
	TLabel *LMag;
	TBevel *Bevel8;
	TLabel *LPh;
  TMenuItem *Graphs1;
  TSaveDialog *SaveBitmapDialog;
  TRadioGroup *RadioGroup1;
  TMenuItem *ColorPalette1;
  TMenuItem *N1;
  TMenuItem *SaveasBitmap1;
  TMenuItem *N2;
  TMenuItem *SaveSelection1;
  TGroupBox *GroupBox3;
  TRadioButton *RGReal;
  TRadioButton *RGImag;
  TRadioButton *RGMag;
  TRadioButton *RGPhase;
  TRadioButton *RGPower;
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall Open1Click(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BRLeftClick(TObject *Sender);
	void __fastcall BRRightClick(TObject *Sender);
	void __fastcall BAUpClick(TObject *Sender);
	void __fastcall BADownClick(TObject *Sender);
	void __fastcall ZoomInXClick(TObject *Sender);
	void __fastcall ZoomOutXClick(TObject *Sender);
	void __fastcall ZoomInYClick(TObject *Sender);
	void __fastcall ZoomOutYClick(TObject *Sender);
	void __fastcall EXStartExit(TObject *Sender);
	void __fastcall EXEndExit(TObject *Sender);
	void __fastcall EYStartExit(TObject *Sender);
	void __fastcall EYEndExit(TObject *Sender);
	void __fastcall EnterKeyPress(TObject *Sender, char &Key);
	void __fastcall BAllClick(TObject *Sender);
	void __fastcall RGShowClick(TObject *Sender);
	void __fastcall RGDrawDataClick(TObject *Sender);
    void __fastcall SBScroll(TObject *Sender,
	  TScrollCode ScrollCode, int &ScrollPos);
	void __fastcall Exit1Click(TObject *Sender);
  void __fastcall Graphs1Click(TObject *Sender);
  void __fastcall ColorPalette1Click(TObject *Sender);
  void __fastcall SaveasBitmap1Click(TObject *Sender);

  
  void __fastcall RGDataClick(TObject *Sender);
  void __fastcall Help1Click(TObject *Sender);
private:	// User declarations
    class TBitM *GraphBM;     // main graph window bitmap
    class TBitM *PalBM;       // current palette bitmap
    class TBitM *ThumbBM;     // full image bitmap
    Graphics::TBitmap *LGraphBM;  // bitmap used to show cross-sections
    struct SGraphData GD;
    HPALETTE Cpal;           // current palette
    int RealizeMyPalette(bool ForceBackGround);
    void MakePalette(unsigned char Type,int Bright,int Contrast, int Sat);
     // is left or right mouse button down ?
    bool LeftButtonDown;
    bool RightButtonDown;
  	TPoint        ClickPoint;          // coordinates of point clicked
	  TRect         ZoomRect;            // rectangle to be zoomed in
	  TRect         CursorRectV;            // cursor Vertical
    TRect         CursorRectH;            // cursor Horizontal
    bool IsUptodate;
    bool FileLoaded;
    // used to determine which cursor to show
  //  bool CursorCross;
    double **Data1, **Data2; // graph data array = bitmap
    double **DataThumb1, **DataThumb2; // graph data array = bitmap
    // side graphs
    bool SideGraphs;
    int WidthLeftGraph, HeightBottomGraph;
    Graphics::TBitmap *LeftGraphBM;  // bitmap used to show cross-sections
    Graphics::TBitmap *BottomGraphBM;  // bitmap used to show cross-sections
    bool Busy;  // busy updating - don't redraw
    // current min and max value of selected format (I,Q, Mag phase etc)
 //   double GraphMin, GraphMax;
    int DataType; // selected data to show (I,Q,Mag etc..)
    // the format of the current loaded file
    struct SFileFormat FF;

    // ------------- functions ----------------
    void SetNewGraphRange(double *XStart, double *XEnd,
                          double *YStart, double *YEnd);
    void SetNewGraphSize(void);
    void UpdateRange(void);
    void UpdateGraphBitmap(void);
    void WindowToReal(int PixX, int PixY, double *X, double *Y);
    void RealToWindow(double X, double Y, int *PixX, int *PixY);
    void DrawFrame(TCanvas *Canvas, TRect GraphArea, double XStart, double XEnd,
               double YStart, double YEnd);
    void __fastcall ShowAll();
    void __fastcall DrawPal();
    void SetupPaletteBitmap();
    void UpdateGraphBM(void);
    void LoadBitmap(class TBitM *BM, double XStart, double XRange,
          double YStart, double YRange, double **DI, double **DQ);
    void UpdateBM(class TBitM *BM, double **DI, double **DQ);
    void UpdateThumbBM(void);
    void DrawCut();
    void DrawSideGraph(TCanvas *Canvas, TRect GraphArea, double XStart, double XEnd,
               double YStart, double YEnd, int X, int Y);
    void SetupGraphSize();
    void FindMinMaxForWindow();
    void UpdateDialogsGivenFileFormat();
    void LoadDataGivenRange(long XStart, long PixelX, long YStart,
        long PixelY, double **D1, double **D2);


public:		// User declarations
    // current color palette
    unsigned char ColPal[NCOLOR][3];
    unsigned char ColorPal;            // which palette (red-blue, gray etc.)
	__fastcall TMainViewForm(TComponent* Owner);
    void virtual __fastcall RestrictSize(TMessage& Msg);
    void virtual __fastcall ChangingSize(TMessage& Msg);
    void virtual __fastcall ChangedSize(TMessage& Msg);
    void virtual __fastcall PalChanged(TMessage& Msg);
    void virtual __fastcall QueryPal(TMessage& Msg);
    // This message map maps the WM_GETMINMAXINFO message to the
    // RestrictSize function.
    BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER(WM_PALETTECHANGED,TMessage,PalChanged)
      MESSAGE_HANDLER(WM_GETMINMAXINFO,TMessage,RestrictSize)
      MESSAGE_HANDLER(WM_SIZING,TMessage,ChangedSize)
      MESSAGE_HANDLER(WM_SIZE,TMessage,ChangingSize)
      MESSAGE_HANDLER(WM_QUERYNEWPALETTE,TMessage,QueryPal)
      // Add any number of additional message handlers here.
    END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern TMainViewForm *MainViewForm;
//---------------------------------------------------------------------------
#endif
