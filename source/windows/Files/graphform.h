//---------------------------------------------------------------------------
#ifndef graphformH
#define graphformH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include "Grids.hpp"
#include <vcl\Buttons.hpp>
#include "objectstructdef.h"
#include <vcl\ComCtrls.hpp>
#include <vcl\Dialogs.hpp>
// cursor for draging samples
#define crDragCursor 1
//---------------------------------------------------------------------------
class TGraphForm1 : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *OKButton;
	TBitBtn *CancelButton;
	TOpenDialog *OpenDialog;
	TBitBtn *ZoomInX;
	TBitBtn *ZoomOutX;
	TBitBtn *ZoomInY;
	TBitBtn *ZoomOutY;
  TPanel *Panel1;
  TGroupBox *SourceBox;
  TRadioButton *FileRadioBtn;
  TRadioButton *InlineRadioBtn;
  TEdit *FileName;
  TButton *BrowseBtn;
  TButton *LoadBtn;
  TButton *SaveBtn;
  TRadioGroup *RGDisplay;
  TRadioGroup *IntGroup;
  TGroupBox *EditBox;
  TLabel *Label1;
  TStringGrid *SampleGrid;
  TEdit *SampleNo;
  TButton *InsertSample;
  TButton *DeleteSample;
  TGroupBox *GBWindow;
  TLabel *RangeX;
  TLabel *RangeY;
  TLabel *to2;
  TLabel *to1;
  TEdit *EXStart;
  TEdit *EXEnd;
  TEdit *EYStart;
  TEdit *EYEnd;
  TBitBtn *HelpButton;
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall RGDisplayClick(TObject *Sender);
	void __fastcall TEExit(TObject *Sender);
	void __fastcall EnterKeyPress(TObject *Sender, char &Key);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall SampleNoKeyPress(TObject *Sender, char &Key);
  	
	void __fastcall SampleNoExit(TObject *Sender);
  	void __fastcall IntGroupClick(TObject *Sender);
	void __fastcall OKButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall InsertSampleClick(TObject *Sender);
	void __fastcall DeleteSampleClick(TObject *Sender);

	void __fastcall BrowseBtnClick(TObject *Sender);
	void __fastcall LoadBtnClick(TObject *Sender);
	void __fastcall SaveBtnClick(TObject *Sender);
	void __fastcall InlineRadioBtnClick(TObject *Sender);
	void __fastcall ZoomInXClick(TObject *Sender);
	void __fastcall ZoomOutXClick(TObject *Sender);
	void __fastcall ZoomInYClick(TObject *Sender);
	void __fastcall ZoomOutYClick(TObject *Sender);
  void __fastcall SampleGridKeyPress(TObject *Sender, char &Key);
  void __fastcall SampleGridExit(TObject *Sender);
  void __fastcall SampleGridClick(TObject *Sender);
  void __fastcall HelpButtonClick(TObject *Sender);
private:	// User declarations
    // -------------- variables ---------------
    // class graph for drawing frames,units etc
    class TGraph *Graph;
    // bitmap buffer for fast scrolling
    Graphics::TBitmap *GraphBM;
    // border between window and graph
    TRect Border;
    // is left or right mouse button down ?
    bool LeftButtonDown;
    bool RightButtonDown;
    bool DragMode;
	TPoint        ClickPoint;          // coordinates of point clicked
	TPoint        OldPoint;          // coordinates of point clicked
 //	TPoint        DragPoint;          // coordinates of point clicked
	TRect         ZoomRect;            // rectangle to be zoomed in
    int Display;          // currently displayed variable
    long ClickedSample;
    // ------------- functions ----------------
    // set border and allocate memory for bitmap
    void InitGraph(double XStart, double XEnd, double YStart,
                   double YEnd);
    // convert between windows (pixel) and real coordinates
    void WindowToReal(int PixX, int PixY, double *X, double *Y);
    void RealToWindow(double X, double Y, int *PixX, int *PixY);
    // redraw whole graph area
    void DrawGraph(TCanvas *DCanvas,bool Clear);
    void UpdateSampleValues(void);
    void UpdateSampleGrid(void);
    void UpdateRange(void);
    //bool LoadDataFile(char FileName[],SDataDefinition *DataDef);
    long ClickedOnSample(int XPix, int YPix);
    void SortSampleGrid(void);
    void __fastcall QuickSort(double *A, double *B,int const AHigh, int iLo, int iHi);
    void __fastcall AdjustSampleGrid();
    void PointCoord(int X, int Y);

public:		// User declarations
    // -------------- variables ---------------
    int Type;  // type of graph, i.e. single line, multiline, pixels etc
    // the actual data
    struct SDataDefinition DataDef;
    // ------------- functions ----------------
	__fastcall TGraphForm1(TComponent* Owner);
    int MessageHandler(int WhatsUp);
        void virtual __fastcall RestrictSize(TMessage& Msg);
    // This message map maps the WM_GETMINMAXINFO message to the
    // RestrictSize function.
    BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER(WM_GETMINMAXINFO,TMessage,RestrictSize)
      // Add any number of additional message handlers here.
    END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern TGraphForm1 *GraphForm1;
//---------------------------------------------------------------------------
#endif
