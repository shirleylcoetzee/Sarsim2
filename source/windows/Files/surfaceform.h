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
  TLabel *Label17;
  TBitBtn *BitBtn1;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall OkBtnClick(TObject *Sender);
  
  
  
  void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
    bool CanClose(void);
public:		// User declarations
    // Surface which is changed
    struct SSurface Surface;
  __fastcall TSurfaceF(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TSurfaceF *SurfaceF;
//---------------------------------------------------------------------------
#endif
