//---------------------------------------------------------------------------
#ifndef selectpalH
#define selectpalH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Dialogs.hpp>
#include "mainquikview.h"
//---------------------------------------------------------------------------
class TLoadPalForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TRadioButton *RBGray;
	TRadioButton *RBRedBlue;
	TRadioButton *RBRainbow;
	TRadioButton *RBCustom;
	TBitBtn *BLoadPal;
	TBitBtn *BOk;
	TOpenDialog *OpenPalDialog;
	TBevel *Bevel1;
  TBitBtn *BitBtn1;
	void __fastcall RBGrayClick(TObject *Sender);
	void __fastcall BLoadPalClick(TObject *Sender);
	void __fastcall BOkClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall RBRedBlueClick(TObject *Sender);
	void __fastcall RBRainbowClick(TObject *Sender);
	void __fastcall RBCustomClick(TObject *Sender);
  void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
    void __fastcall DrawPal();
    class TBitM *PalBM;       // current palette bitmap
    HPALETTE Cpal;           // current palette
    // current color palette
    unsigned char ColPal[NCOLOR][3];
    void SetupPaletteBitmap();
    void MakePalette(unsigned char Type,int Bright,
                            int Contrast, int Sat);
    int RealizeMyPalette(bool ForceBackGround);                        
public:		// User declarations
    unsigned char Pal;
	__fastcall TLoadPalForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TLoadPalForm *LoadPalForm;
//---------------------------------------------------------------------------
#endif
