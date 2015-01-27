//---------------------------------------------------------------------------
#ifndef OldSimH
#define OldSimH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Buttons.hpp>
//---------------------------------------------------------------------------
class TPreviousSimForm : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBox1;
	TLabel *Label1;
	TBitBtn *BOk;
	TBitBtn *BCancel;
	TBitBtn *BDelete;
  TBitBtn *BitBtn1;
	void __fastcall BOkClick(TObject *Sender);
	void __fastcall BCancelClick(TObject *Sender);
	void __fastcall ListBox1DblClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall BDeleteClick(TObject *Sender);
  void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    struct SSimulation *SimSelection;
	__fastcall TPreviousSimForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TPreviousSimForm *PreviousSimForm;
//---------------------------------------------------------------------------
#endif
