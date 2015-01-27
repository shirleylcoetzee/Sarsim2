//---------------------------------------------------------------------------
#ifndef targetformH
#define targetformH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include "objectstructdef.h"

//---------------------------------------------------------------------------
class TTargetF : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label16;
	TLabel *Label17;
	TComboBox *PlatformName;
	TEdit *Xcoord;
	TEdit *Ycoord;
	TEdit *Zcoord;
	TEdit *XcoordDev;
	TEdit *YcoordDev;
	TEdit *ZcoordDev;
	TEdit *RCS;
	TEdit *RCSDev;
	TRadioButton *Iso;
	TRadioButton *Dir;
	TEdit *Azi;
	TEdit *Ele;
	TEdit *AziDev;
	TEdit *EleDev;
	TRadioButton *Cos;
	TRadioButton *Other;
	TButton *Def;
	TBitBtn *OkBtn;
	TBitBtn *CancelBtn;
  TBitBtn *BitBtn1;
	void __fastcall DefClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall OkBtnClick(TObject *Sender);
	
	
  
  
  void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
    bool CanClose(void);
public:		// User declarations
    // target which is changed
    struct STarget Target;
	__fastcall TTargetF(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TTargetF *TargetF;
//---------------------------------------------------------------------------
#endif
