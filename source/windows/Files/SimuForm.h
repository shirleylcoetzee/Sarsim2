//---------------------------------------------------------------------------
#ifndef SimuFormH
#define SimuFormH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include "objectstructdef.h"
#include "calcthread.h"
#include <vcl\ComCtrls.hpp>
#include <vcl\Menus.hpp>
#include "quikgeneral.h"
#include <vcl\Dialogs.hpp>

// maximum number of graphs open at a time
#define MAX_GRAPHS 4
//---------------------------------------------------------------------------
class TSimulationForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *LabelAzi;
	TLabel *LabelSlant;
	TTimer *Timer1;
	TImage *Image1;
	TBitBtn *ZoomInX;
	TBitBtn *ZoomOutX;
	TBitBtn *ZoomInY;
	TBitBtn *ZoomOutY;
  TBitBtn *BRLeft;
  TBitBtn *BRRight;
  TBitBtn *BAUp;
  TBitBtn *BADown;
  TPanel *Panel1;
  TGroupBox *GroupBox1;
  TRadioGroup *RGDisplay;
  TGroupBox *GBRadar;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TComboBox *RadarList;
  TEdit *EditSampleF;
  TRadioGroup *PulsePos;
  TGroupBox *GroupBox2;
  TScrollBar *SBBright;
  TGroupBox *GroupBox4;
  TScrollBar *SBContrast;
  TGroupBox *GroupBox5;
  TScrollBar *SBSat;
  TBitBtn *BPalette;
  TPanel *Panel3;
  TProgressBar *ProgressBar;
  TPanel *Panel2;
  TBevel *Bevel4;
  TLabel *LMag;
  TLabel *LPh;
  TLabel *LQuad;
  TLabel *LY;
  TLabel *LX;
  TBevel *Bevel6;
  TBevel *Bevel1;
  TBevel *Bevel8;
  TBevel *Bevel5;
  TBevel *Bevel3;
  TLabel *Label8;
  TLabel *Label9;
  TLabel *LIn;
  TLabel *Label10;
  TLabel *Label11;
  TLabel *Label12;
  TBevel *Bevel7;
  TLabel *Label5;
  TLabel *Label6;
  TLabel *Label13;
  TLabel *Label14;
  TBitBtn *BSaveData;
  TBitBtn *BClose;
  TBevel *Bevel2;
  TPanel *Panel4;
  TGroupBox *GBWindow;
  TLabel *XStart;
  TLabel *XEnd;
  TLabel *YStart;
  TLabel *YEnd;
  TEdit *EXStart;
  TEdit *EXEnd;
  TEdit *EYStart;
  TEdit *EYEnd;
  TButton *BShowAll;
  TPanel *Panel5;
  TPanel *Panel6;
  TRadioGroup *RGDrawData;
  TLabel *Label4;
  TLabel *Label7;
  TLabel *Label15;
  TSaveDialog *SaveBitmapDialog;
  TBitBtn *BBitmap;
  TBitBtn *BHelp;
	TGroupBox *GroupBox3;
	TEdit *EOverSample;
	TLabel *Label16;
	TEdit *EPWidth;
	TLabel *Label17;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall EXStartExit(TObject *Sender);
	void __fastcall EXEndExit(TObject *Sender);
	void __fastcall EYStartExit(TObject *Sender);
	void __fastcall EYEndExit(TObject *Sender);
	void __fastcall BShowAllClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall TBContrastChange(TObject *Sender);
	void __fastcall ZoomInXClick(TObject *Sender);
	void __fastcall ZoomOutXClick(TObject *Sender);
	void __fastcall ZoomInYClick(TObject *Sender);
	void __fastcall ZoomOutYClick(TObject *Sender);
	void __fastcall EnterKeyPress(TObject *Sender, char &Key);
	void __fastcall RadarListChange(TObject *Sender);
	void __fastcall RGDisplayClick(TObject *Sender);
	void __fastcall BSaveDataClick(TObject *Sender);
	void __fastcall BCancelClick(TObject *Sender);
	void __fastcall EditSampleFKeyPress(TObject *Sender, char &Key);
	void __fastcall EditSampleFExit(TObject *Sender);
	void __fastcall PulsePosClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall SBContrastScroll(TObject *Sender, TScrollCode ScrollCode,
  int &ScrollPos);

  void __fastcall BRLeftClick(TObject *Sender);
  void __fastcall BRRightClick(TObject *Sender);
  void __fastcall BADownClick(TObject *Sender);
  void __fastcall BAUpClick(TObject *Sender);

  void __fastcall SBBrightScroll(TObject *Sender, TScrollCode ScrollCode,
  int &ScrollPos);
  void __fastcall SBSatScroll(TObject *Sender, TScrollCode ScrollCode,
  int &ScrollPos);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall BPaletteClick(TObject *Sender);
  void __fastcall RGDrawDataClick(TObject *Sender);
  void __fastcall RGScaleClick(TObject *Sender);

  void __fastcall BBitmapClick(TObject *Sender);
  void __fastcall BHelpClick(TObject *Sender);
	
	void __fastcall EOverSampleKeyPress(TObject *Sender, char &Key);
	void __fastcall EOverSampleExit(TObject *Sender);
	
	void __fastcall EPWidthExit(TObject *Sender);
	void __fastcall EPWidthKeyPress(TObject *Sender, char &Key);
	
	
	
private:	// User declarations
    // -------------- variables ---------------
    // border between window and graph
    TRect Border;
    // min and max values of graph
    double XMin, XMax, YMin, YMax;
    // class graph for drawing frames,units etc
    class TGraph *Graph;
    // bitmap buffer for fast scrolling
    //Graphics::TBitmap *GraphBM;
    // is left or right mouse button down ?
    bool LeftButtonDown;
    bool RightButtonDown;
	TPoint        ClickPoint;          // coordinates of point clicked
	TRect         ZoomRect;            // rectangle to be zoomed in
    // the calculation thread
    class TCalcThread *AThread;
    // estimated maximum magnitude
    double MaxMag;
    // current radar
    struct SRadar *CRadar;
    // running thread ?
    bool Busy;
    // does the bitmap need updating  (recalculation)
    bool IsDirtyNewCalc;
    // bitmap just needs to be redrawn (not recalculated)
    bool IsDirtyRedraw;
    // running for the first time ? -> automatically select slant range
    bool AutoSlantRange;
    // user pressed close windows
    bool WantToClose;
    // currently modifying bitmap
    bool BusyWithBitmap;
    // is cursor in graph area
    bool CursorInGraph;

    // current data IQ array for info window
    double **Data;
    double *PulseSendTime;
    // actual number of pulses shown
    long PulseNo;

    // current color palette
    unsigned char ColPal[NCOLOR][3];
    class TBitM *PalBM;       // current palette bitmap
    HPALETTE Cpal;           // current palette
    unsigned char ColorPal;            // which palette (red-blue, gray etc.)


    class TBitM *GraphBM2;     // main graph window bitmap
    Graphics::TBitmap *LGraphBM;  // bitmap used to show cross-sections
    // size of current bitmap
    int PixelX, PixelY;
    // show in dB Power
    bool ShowdB;

    // min and max values for current shown type (I, Q, Mag etc.)
    double PMin, PMax;
    // ------------- functions ----------------
    // convert between windows (pixel) and real coordinates
    void WindowToReal(int PixX, int PixY, double *X, double *Y);
    void RealToWindow(double X, double Y, int *PixX, int *PixY);
    void SetNewGraphSize(void);
    void UpdateRange(void);
    // called when thread has finished
    void __fastcall ThreadDone(TObject * );
    void UpdateGraphBitmap(bool ShowAllTargets);
    void StopThread(void);
    void __fastcall TSimulationForm::FastUpdate(void);
    void __fastcall FindCurrentRadar();
    void AdjustGraphRangeValues(double *XStart, double *XEnd,
                                double *YStart, double *YEnd);
    void RedrawRange();
    void DrawBitmap(TCanvas *Canvas, double **Data,
               double *PulseSendTime, unsigned long PulseNo);
    void SetupPaletteBitmap();
    void MakePalette(unsigned char Type,int Bright,
                            int Contrast, int Sat);
    int RealizeMyPalette(bool ForceBackGround);
    void FindMinMax();


public:		// User declarations
    // -------------- variables ---------------
    // window number
    int Count;
    struct SSimulation CurrentSim;
    // thread terminated;
    bool Terminat;
    // make sure initialization has finished before any function are called
    bool FinishedInitializing;
    // ------------- functions ----------------
    void SetNewGraphRange(double *XStart, double *XEnd,
                          double *YStart, double *YEnd);
	  __fastcall TSimulationForm(TComponent* Owner);
    void virtual __fastcall RestrictSize(TMessage& Msg);
    // This message map maps the WM_GETMINMAXINFO message to the
    // RestrictSize function.
    BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER(WM_GETMINMAXINFO,TMessage,RestrictSize)
      // Add any number of additional message handlers here.
    END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern TSimulationForm *SimulationForm;
//---------------------------------------------------------------------------
#endif




