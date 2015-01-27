//---------------------------------------------------------------------------
#ifndef GeoOptH
#define GeoOptH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Buttons.hpp>
//---------------------------------------------------------------------------
class TGeoOptForm : public TForm
{
__published:	// IDE-managed Components
  TGroupBox *GroupBox1;
  TCheckBox *CheckBox1;
  TCheckBox *CheckBox2;
  TCheckBox *CheckBox3;
  TCheckBox *CheckBox4;
  TCheckBox *CheckBox5;
  TCheckBox *CheckBox6;
  TCheckBox *CheckBox7;
  TCheckBox *CheckBox8;
  TBitBtn *BClose;
  TBitBtn *BitBtn1;
  void __fastcall BCloseClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  unsigned int Options;
  __fastcall TGeoOptForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TGeoOptForm *GeoOptForm;
//---------------------------------------------------------------------------
#endif
