//---------------------------------------------------------------------------
#ifndef importH
#define importH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
//---------------------------------------------------------------------------
class TImportForm : public TForm
{
__published:	// IDE-managed Components
  TOpenDialog *ImportDialog1;
  TPanel *Panel1;
  TBitBtn *BitBtn1;
  TComboBox *PlatformName;
  TEdit *FileName;
  TLabel *Label1;
  TRadioGroup *RGObject;
  TBitBtn *BitBtn3;
  TBitBtn *BitBtn2;
  TBitBtn *BitBtn4;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall BitBtn2Click(TObject *Sender);
  void __fastcall BitBtn1Click(TObject *Sender);
  
  void __fastcall BitBtn4Click(TObject *Sender);
private:	// User declarations
  void LoadData(void);
public:		// User declarations
  __fastcall TImportForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TImportForm *ImportForm;
//---------------------------------------------------------------------------
#endif
