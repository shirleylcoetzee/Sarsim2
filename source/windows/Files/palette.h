//---------------------------------------------------------------------------
#ifndef paletteH
#define paletteH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Buttons.hpp>
//---------------------------------------------------------------------------
class TPaletteForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GBContrast;
	TLabel *Label5;
	TLabel *Label7;
	TTrackBar *TBContrast;
	TBitBtn *BClose;
	TBitBtn *BitBtn1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall BCloseClick(TObject *Sender);
	void __fastcall TBContrastChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TPaletteForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TPaletteForm *PaletteForm;
//---------------------------------------------------------------------------
#endif
