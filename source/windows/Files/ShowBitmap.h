//---------------------------------------------------------------------------
#ifndef ShowBitmapH
#define ShowBitmapH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Menus.hpp>


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
    int    UnitsX;    // units on x-coordinate
    int    UnitsY;    // units on y-coordinate
    TRect  Border;    // border between window and graph
    int    Display;   // I,Q,Mag or phase
    TRect  MaxWindowSize; // Min and Max window size
  };
//---------------------------------------------------------------------------
class TShowBitmapF : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *RGDisplay;
	TRadioGroup *RGShow;
	TGroupBox *GBContrast;
	TLabel *Label5;
	TLabel *Label7;
	TTrackBar *TBContrast;
	TGroupBox *GBWindow;
	TLabel *XStart;
	TLabel *XEnd;
	TLabel *YStart;
	TLabel *YEnd;
	TEdit *EXStart;
	TEdit *EXEnd;
	TEdit *EYStart;
	TEdit *EYEnd;
	TBitBtn *BClose;
	TLabel *LabelAzi;
	TLabel *LabelSlant;
	TBevel *Bevel1;
	TBevel *Bevel2;
	TBevel *Bevel3;
	TBevel *Bevel4;
	TBevel *Bevel5;
	TLabel *Label6;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *LX;
	TLabel *LY;
	TLabel *LIn;
	TLabel *LQuad;
	TBitBtn *ZoomInX;
	TBitBtn *ZoomOutX;
	TBitBtn *ZoomInY;
	TBitBtn *ZoomOutY;
	TBitBtn *BAll;
	TTimer *Timer1;
	TBitBtn *BRLeft;
	TBitBtn *BRRight;
	TBitBtn *BAUp;
	TBitBtn *BADown;
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *Fopen;
	TMenuItem *Exit1;
	TBevel *Bevel6;
	TMenuItem *Options1;
	TMenuItem *Palette1;
	TMenuItem *Help1;
	TRadioGroup *RGDrawData;
	TGroupBox *GBGraph;
	TLabel *Label1;
	TLabel *Label2;
	TTrackBar *TBGraph;
	TBevel *Bevel7;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall RGShowClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall EXStartExit(TObject *Sender);
	void __fastcall EXEndExit(TObject *Sender);
	void __fastcall EYStartExit(TObject *Sender);
	void __fastcall EYEndExit(TObject *Sender);
	void __fastcall BCloseClick(TObject *Sender);
	void __fastcall ZoomInXClick(TObject *Sender);
	void __fastcall ZoomOutXClick(TObject *Sender);
	void __fastcall ZoomInYClick(TObject *Sender);
	void __fastcall ZoomOutYClick(TObject *Sender);
	
	void __fastcall BAllClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	
	void __fastcall TBContrastChange(TObject *Sender);
	void __fastcall BRLeftClick(TObject *Sender);
	void __fastcall BRRightClick(TObject *Sender);
	void __fastcall BAUpClick(TObject *Sender);
	void __fastcall BADownClick(TObject *Sender);
	
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall FopenClick(TObject *Sender);
	void __fastcall RGDrawDataClick(TObject *Sender);
	void __fastcall TBGraphChange(TObject *Sender);
	void __fastcall Palette1Click(TObject *Sender);
private:	// User declarations
    // class graph for drawing frames,units etc
    class TGraph *Graph;
    // bitmap buffer for fast scrolling
    Graphics::TBitmap *GraphBM;
    // is left or right mouse button down ?
    bool LeftButtonDown;
    bool RightButtonDown;
	TPoint        ClickPoint;          // coordinates of point clicked
	TRect         ZoomRect;            // rectangle to be zoomed in
    bool IsUptodate;
    bool FileLoaded;
    // used to determine which cursor to show
    bool CursorCross;
    TRect GraphArea;

    double **DataI, **DataQ; // data array = bitmap

    char *pDIBBase;		// pointers to DIB section we'll draw into
    HBITMAP hDIBSection;        // handle of DIB section
    int DIBWidth, DIBHeight;
    BITMAPINFO *pbmiDIB;		// pointer to the BITMAPINFO

    // ------------- functions ----------------
    void SetNewGraphRange(double *XStart, double *XEnd,
                          double *YStart, double *YEnd);
    void SetNewGraphSize(void);
    void UpdateRange(void);
    void UpdateGraphBitmap(void);
    void WindowToReal(int PixX, int PixY, double *X, double *Y);
    void RealToWindow(double X, double Y, int *PixX, int *PixY);

public:		// User declarations
    struct SGraphData GD;
    void virtual __fastcall RestrictSize(TMessage& Msg);
   	__fastcall TShowBitmapF(TComponent* Owner);
    // This message map maps the WM_GETMINMAXINFO message to the
    // RestrictSize function.
    BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER(WM_GETMINMAXINFO,TMessage,RestrictSize)
      // Add any number of additional message handlers here.
    END_MESSAGE_MAP(TForm)

};
//---------------------------------------------------------------------------
extern TShowBitmapF *ShowBitmapF;
//---------------------------------------------------------------------------
#endif
