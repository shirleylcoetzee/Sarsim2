//---------------------------------------------------------------------------
#ifndef platformformH
#define platformformH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include "objectstructdef.h"
//---------------------------------------------------------------------------
class TPlatformF : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TLabel *Label1;
	TComboBox *PlatformName;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TRadioButton *RBStat;
	TRadioButton *RBTraj;
	TRadioButton *RBPosStdDev;
	TRadioButton *RBPosDef;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TButton *TrajDef;
	TEdit *Xcoord;
	TEdit *Ycoord;
	TEdit *Zcoord;
	TEdit *Xrot;
	TEdit *Yrot;
	TEdit *Zrot;
	TLabel *Label5;
	TLabel *Label12;
	TLabel *Label13;
	TCheckBox *CBAlign;
	TBitBtn *OkBtn;
	TBitBtn *CancelBtn;
	TEdit *StdDevXPos;
	TEdit *StdDevYPos;
	TEdit *StdDevZPos;
	TButton *PosDef;
	TLabel *Label17;
	TPanel *Panel4;
	TRadioButton *RBRotStdDev;
	TLabel *Label16;
	TLabel *Label15;
	TLabel *Label14;
	TRadioButton *RBRotDef;
	TButton *RotDef;
	TEdit *StdDevZRot;
	TEdit *StdDevYRot;
	TEdit *StdDevXRot;
	TLabel *Label18;
  TLabel *Label19;
  TLabel *Label20;
  TLabel *Label21;
  TEdit *Xrotrate;
  TEdit *Yrotrate;
  TEdit *Zrotrate;
  TRadioButton *RBRotUser;
  TRadioButton *RBRotFixed;
  TButton *Button1;
  TBevel *Bevel1;
  TBevel *Bevel2;
  TBitBtn *BitBtn1;
	void __fastcall OkBtnClick(TObject *Sender);
	
	void __fastcall TrajDefClick(TObject *Sender);
	void __fastcall PosDefClick(TObject *Sender);
	void __fastcall RotDefClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	
  void __fastcall Button1Click(TObject *Sender);
  
  void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
    bool CanClose(void);
public:		// User declarations
    // platform which is added / changed
    struct SPlatform Platform;
    // modify existing platform ?
    bool Modify_Old;
	__fastcall TPlatformF(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TPlatformF *PlatformF;
//---------------------------------------------------------------------------
#endif
