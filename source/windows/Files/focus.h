//---------------------------------------------------------------------------
#ifndef focusH
#define focusH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Buttons.hpp>
//---------------------------------------------------------------------------
class TFocusForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TRadioButton *RBPlat;
	TRadioButton *RBOther;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TLabel *Label2;
	TEdit *XCoord;
	TComboBox *PlatformBox;
	TLabel *Label3;
	TEdit *YCoord;
	TLabel *Label4;
	TEdit *ZCoord;
  TBitBtn *BitBtn3;
	void __fastcall FormShow(TObject *Sender);
	
	void __fastcall BitBtn1Click(TObject *Sender);
  void __fastcall BitBtn3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFocusForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TFocusForm *FocusForm;
//---------------------------------------------------------------------------
#endif
