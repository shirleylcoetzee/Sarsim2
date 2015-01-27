//---------------------------------------------------------------------------
#ifndef aboutH
#define aboutH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
//---------------------------------------------------------------------------
class TAboutForm : public TForm
{
__published:	// IDE-managed Components
	TTimer *Timer1;
	TPanel *Panel2;
	TLabel *Label2;
	TLabel *Label3;
	TImage *Image1;
	TBitBtn *BitBtn1;
	TImage *Image2;
  TLabel *Label4;
  TLabel *Label5;
  TLabel *Label6;
  TPanel *Panel1;
  TLabel *Label1;
  TLabel *LExp;
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TAboutForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TAboutForm *AboutForm;
//---------------------------------------------------------------------------
#endif
