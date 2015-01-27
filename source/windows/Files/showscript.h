//---------------------------------------------------------------------------
#ifndef showscriptH
#define showscriptH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include "objectstructdef.h"
#include <vcl\Buttons.hpp>
//---------------------------------------------------------------------------
class TShowScriptForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *MScript;
	TBitBtn *BClose;
  TBitBtn *BInterpret;
  TBitBtn *BHelp;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BUpdateClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
  void __fastcall BInterpretClick(TObject *Sender);
  
  void __fastcall BHelpClick(TObject *Sender);
  
private:	// User declarations
public:		// User declarations
	__fastcall TShowScriptForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TShowScriptForm *ShowScriptForm;
//---------------------------------------------------------------------------
#endif
