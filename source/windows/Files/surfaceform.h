//---------------------------------------------------------------------------
#ifndef surfaceformH
#define surfaceformH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include "objectstructdef.h"
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>

//---------------------------------------------------------------------------
class TSurfaceF : public TForm
{
__published:	// IDE-managed Components
  TComboBox *PlatformName;
	TPanel *Panel1;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label5;
  TEdit *Xcoord1;
  TEdit *Ycoord1;
  TEdit *Zcoord1;
  TBitBtn *OkBtn;
  TBitBtn *CancelBtn;

  TLabel *Label6;
  TLabel *Label7;
  TLabel *Label8;
  TEdit *Xcoord2;
  TEdit *Ycoord2;
  TEdit *Zcoord2;
  TLabel *Label9;
  TLabel *Label10;
  TLabel *Label11;
  TEdit *Xcoord3;
  TEdit *Ycoord3;
  TEdit *Zcoord3;
  TLabel *Label12;
  TLabel *Label13;
  TLabel *Label14;
  TLabel *Label15;
  TPanel *Panel2;
  TLabel *Label16;
  TBitBtn *BitBtn1;
	TLabel *Label18;
	TPanel *Panel3;
	TRadioButton *Iso;
	TLabel *Label24;
	TEdit *ESpacing;
	TLabel *Label29;
	TCheckBox *CBConstNormal;
	TLabel *Label25;
	TEdit *ERCSdev;
	TLabel *Label26;
	TEdit *ERCSmult;
	TCheckBox *CBBothSides;
	TLabel *Label30;
	TLabel *Label31;
	TPanel *Panel6;
	TLabel *Label27;
	TEdit *ESample;
	TCheckBox *CBGlobal;
	TLabel *Label28;
	TGroupBox *GroupBox1;
	TLabel *Label17;
	TEdit *EInPlane;
	TLabel *Label20;
	TEdit *EOutPlane;
	TLabel *Label19;
	TEdit *EPosSeed;
	TLabel *Label21;
	TEdit *ERCSSeed;
	TLabel *LGlobal;
	TButton *Button1;
	TLabel *Label22;
	TPanel *Panel5;
	TLabel *Label32;
	TLabel *Label33;
	TLabel *Label34;
	TLabel *Label35;
	TLabel *Label36;
	TLabel *Label37;
	TPanel *Panel4;
	TRadioButton *Dir;
	TLabel *Label23;
	TPanel *Panel7;
	TRadioButton *Cos;
	TRadioButton *Other;
	TButton *Def;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall OkBtnClick(TObject *Sender);
  void __fastcall BitBtn1Click(TObject *Sender);


	void __fastcall Button1Click(TObject *Sender);

	void __fastcall Xcoord1KeyPress(TObject *Sender, char &Key);
	void __fastcall Xcoord1Exit(TObject *Sender);
	void __fastcall Ycoord1Exit(TObject *Sender);
	void __fastcall Ycoord1KeyPress(TObject *Sender, char &Key);
	void __fastcall Zcoord1Exit(TObject *Sender);
	void __fastcall Zcoord1KeyPress(TObject *Sender, char &Key);
	void __fastcall Xcoord2Exit(TObject *Sender);
	void __fastcall Xcoord2KeyPress(TObject *Sender, char &Key);
	void __fastcall Ycoord2Exit(TObject *Sender);
	void __fastcall Ycoord2KeyPress(TObject *Sender, char &Key);
	void __fastcall Zcoord2Exit(TObject *Sender);
	void __fastcall Zcoord2KeyPress(TObject *Sender, char &Key);
	void __fastcall Xcoord3Exit(TObject *Sender);
	void __fastcall Xcoord3KeyPress(TObject *Sender, char &Key);
	void __fastcall Ycoord3Exit(TObject *Sender);
	void __fastcall Ycoord3KeyPress(TObject *Sender, char &Key);
	void __fastcall Zcoord3Exit(TObject *Sender);
	void __fastcall Zcoord3KeyPress(TObject *Sender, char &Key);
	void __fastcall CBGlobalClick(TObject *Sender);
	void __fastcall ESampleExit(TObject *Sender);
	void __fastcall ESampleKeyPress(TObject *Sender, char &Key);
	void __fastcall ESpacingExit(TObject *Sender);
	void __fastcall ESpacingKeyPress(TObject *Sender, char &Key);
	void __fastcall IsoClick(TObject *Sender);
	void __fastcall DirClick(TObject *Sender);
private:	// User declarations
    bool CanClose(void);
    void CalcAreaAndDir(double *Area, double *NormVecUnit,
                       long *PTUsedPreview, long *PTUsedFinal);
    void CcoordChange();

public:		// User declarations
    // Surface which is changed
    struct SSurface Surface;
  __fastcall TSurfaceF(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TSurfaceF *SurfaceF;
//---------------------------------------------------------------------------
#endif
