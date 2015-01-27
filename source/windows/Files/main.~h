//---------------------------------------------------------------------------
#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include "objectstructdef.h"
#include "graphform.h"
#include "simuform.h"
#include "focus.h"
#include <vcl\ExtCtrls.hpp>
#include <vcl\Menus.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Dialogs.hpp>

#define ROT_SPEED 4                    // Rotation Speed (pixel/degree)

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TSpeedButton *SpeedButton3;
	TStatusBar *StatusBar1;
	TPanel *Panel2;
	TMainMenu *MainMenu1;
  TMenuItem *FileMenu;
	TMenuItem *New1;
	TMenuItem *NewTarget;
	TMenuItem *NewPlatform;
	TMenuItem *NewRadar;
	TMenuItem *Simulation1;
	TMenuItem *RawReturn1;
	TMenuItem *Help1;
	TLabel *EleAng;
	TEdit *EditSTime;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TListBox *ObjectBox;
	TLabel *Label1;
	TBitBtn *DecTime;
	TBitBtn *IncTime;
	TBevel *Bevel1;
	TBevel *Bevel2;
	TLabel *AziAng;
	TMenuItem *Options;
	TMenuItem *Focus1;
	TPopupMenu *TargetPopupMenu;
	TMenuItem *ModifyT;
	TPopupMenu *RadarPopupMenu;
	TMenuItem *ModifyR;
	TPopupMenu *PlatformPopupMenu;
	TMenuItem *ModifyPlatform1;
	TMenuItem *New2;
	TMenuItem *Open1;
	TMenuItem *Save1;
	TMenuItem *SaveAs1;
	TOpenDialog *OpenSimDialog;
	TSaveDialog *SaveSimDialog;
	TMenuItem *N1;
	TMenuItem *Exit;
	TMenuItem *RangeCompressed1;
	TMenuItem *Previous1;
	TTimer *Timer1;
	TMenuItem *About1;
	TMenuItem *DeleteT;
	TMenuItem *N2;
	TMenuItem *DeleteR;
	TMenuItem *N5;
	TMenuItem *N6;
	TMenuItem *DeletePlatform1;
	TMenuItem *Show1;
	TMenuItem *SimulationOutput1;
	TMenuItem *ScriptFile1;
	TSpeedButton *SpeedButton4;
  TSpeedButton *SpeedButton5;
  TMenuItem *NewSurface1;
  TPopupMenu *SurfacePopupMenu;
  TMenuItem *DeleteSurface1;
  TMenuItem *N3;
  TMenuItem *ModifySurface1;
  TMenuItem *Coord;
  TMenuItem *ElevationLines1;
  TMenuItem *FilledSurfaces1;
  TMenuItem *Import1;
  TSpeedButton *SpeedButton6;
  TMenuItem *Geometry2;
  TBitBtn *BAni;
  TEdit *EStep;
  TBitBtn *BStop;
  TLabel *Label5;
  TMenuItem *Contents1;
  TMemo *Memo1;
	TMenuItem *LCoord;
	void __fastcall FormDestroy(TObject *Sender);
   	void __fastcall NewTargetClick(TObject *Sender);
	void __fastcall NewPlatformClick(TObject *Sender);
	void __fastcall NewRadarClick(TObject *Sender);
	void __fastcall RawReturn1Click(TObject *Sender);
	void __fastcall ObjectBoxDblClick(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall Focus1Click(TObject *Sender);
	void __fastcall ModifyTClick(TObject *Sender);
	void __fastcall DeleteTClick(TObject *Sender);
	void __fastcall ModifyRClick(TObject *Sender);
	void __fastcall DeleteRClick(TObject *Sender);
	void __fastcall ObjectBoxClick(TObject *Sender);
	void __fastcall ModifyPlatform1Click(TObject *Sender);
	void __fastcall DeletePlatform1Click(TObject *Sender);
	void __fastcall Save1Click(TObject *Sender);
	void __fastcall SaveAs1Click(TObject *Sender);
	void __fastcall Open1Click(TObject *Sender);
	void __fastcall New2Click(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall ExitClick(TObject *Sender);
	void __fastcall RangeCompressed1Click(TObject *Sender);
	void __fastcall Previous1Click(TObject *Sender);
	void __fastcall DecTimeMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall IncTimeMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall DecTimeMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall IncTimeMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall EditSTimeKeyPress(TObject *Sender, char &Key);
	void __fastcall About1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall SimulationOutput1Click(TObject *Sender);
	void __fastcall ScriptFile1Click(TObject *Sender);
	void __fastcall SpeedButton4Click(TObject *Sender);
  void __fastcall NewSurface1Click(TObject *Sender);
  void __fastcall DeleteSurface1Click(TObject *Sender);
  void __fastcall ModifySurface1Click(TObject *Sender);
  void __fastcall CoordClick(TObject *Sender);
  void __fastcall ElevationLines1Click(TObject *Sender);
  void __fastcall FilledSurfaces1Click(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall OpenLastFileClick(TObject *Sender);
  void __fastcall Import1Click(TObject *Sender);
  
  
  void __fastcall Geometry2Click(TObject *Sender);
  void __fastcall BAniClick(TObject *Sender);
  void __fastcall BStopClick(TObject *Sender);
  void __fastcall Contents1Click(TObject *Sender);
	
	void __fastcall LCoordClick(TObject *Sender);
	
private:	// User declarations
        // array of pointers to simulation graph forms
        TSimulationForm **SimuGraphs;
        void UpdateObjectList();
        void ModifyTarget(struct STarget *t);
        void DeleteTargetMain(struct STarget *t);
        void ModifyPlatform(struct SPlatform *p);
        void DeletePlatformMain(struct SPlatform *p);
        void ModifyRadar(struct SRadar *r);
        void DeleteRadarMain(struct SRadar *r);
        void ModifySurface(struct SSurface *s);
        void DeleteSurfaceMain(struct SSurface *s);
        void CalcCoordSystemOffset();
        void CalcNewSTimeArray();
        void DrawCoordSystem();
        void DrawLittleCoordSystem();
        void DrawTargets();
        void DrawSurfaces();
        void DrawRadars();
        void DrawPlatforms();
        bool FindObjectBoxSel(int *Selection, void **Object);
        void FreeAllVariables();
        void SetupAllVariables();
        void ChangedObject();
       void __fastcall TMainForm::NewSimulation(int Type, struct SSimulation *Sim);
        // new file ?
        bool IsNewFile;
        // DRAWING stuff
        // array to store positions etc. used in paint
	    double **PlatformPos, **PlatformVel, **PlatformRot, **TargetPos;
        // Simulation time
        double STime;
	    // Visual guidelines ...
	    bool ShowCoordSystemAxes;
	    bool ShowLittleCoordSystemAxes;
	    // bool ShowGridLines;
	    bool ShowElevationLines;
      // Painted or wireframe surfaces
      bool ShowSurfaceOutlines;
	    // Rotation angles, Focus point, Magnification and screen pixel offset
	    double Rot[3];
	    double Pos[3];
        double ZoomX,ZoomY,ZoomXOld,ZoomYOld;
        int OffsetX,OffsetY;
	    // storage for reference point = screen coordinates where
        // left / right Button pushed
	    TPoint LeftBClickPoint,RightBClickPoint;
        // current focus point
        double Focus[3];
	    struct SPlatform *FocusPlatform;
        // time scanning
        bool TimeMinusButtonDown;
	    bool TimePlusButtonDown;
	    // is right/left button pressed
        bool LButtonDown;
        bool RButtonDown;
        // zooming ?
        bool ZoomMode;
        // popmenu selection
        struct STarget *SelectedTarget;
        struct SRadar *SelectedRadar;
        struct SPlatform *SelectedPlatform;
        struct SSurface *SelectedSurface;
        // last file menu
        TMenuItem *MItemLine,*MItemLastFile;
        AnsiString LastFile;

public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
    // called from showscript - reloads temporary script file
    void ReloadScript();
    // pointers to an element in the linked list
    STarget *FirstTarget;
    SSurface *FirstSurface;
    SPlatform *FirstPlatform;
    SRadar *FirstRadar;
    SSimulation *FirstSimulation;
    SGeometry *FirstGeometry;
    // file changed ?
    bool IsDirty;
    // are the simulation windows active  ?
    bool SimuGraphActive[MAX_GRAPHS];
    // Logical palette struct filled with colors
    // used to draw graphics
    LOGPALETTE* RedBluePalette;
    void virtual __fastcall RestrictSize(TMessage& Msg);
    // RestrictSize function.
    BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER(WM_GETMINMAXINFO,TMessage,RestrictSize)
      // Add any number of additional message handlers here.
    END_MESSAGE_MAP(TForm)
	friend class TFocusForm;
};
//---------------------------------------------------------------------------
extern TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
