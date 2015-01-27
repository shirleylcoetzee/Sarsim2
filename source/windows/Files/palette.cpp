//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "palette.h"
#include "general.h"
#include "main.h"
#include "math.h"

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TPaletteForm *PaletteForm;
//---------------------------------------------------------------------------
__fastcall TPaletteForm::TPaletteForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPaletteForm::FormPaint(TObject *Sender)
{
  TRect ColorBar,Frame;
  Graphics::TBitmap *BM;
  SetRect(&ColorBar, 20, 20, 300, 60);
  SetRect(&Frame, ColorBar.Left, ColorBar.Top, ColorBar.Left + ColorBar.Right + 2,
  ColorBar.Top + ColorBar.Bottom +2);
  unsigned long Off = 0x2000000;

  BM = new Graphics::TBitmap();
  BM->Width = ColorBar.Right;
  BM->Height = ColorBar.Bottom;
  BM->IgnorePalette = FALSE;
  BM->Palette = CreatePalette(MainForm->RedBluePalette);

  Canvas->Brush->Color = clBlack;
  Canvas->FrameRect(Frame);

  int i;
  int Color;

      double ColorValue;
      double Contrast = 10.0/double(TBContrast->Position);
      double ContrastMult = (255.0/pow(255.0,Contrast));

  BM->Canvas->Pen->Width = 2;
  for (i=0;i<ColorBar.Right*0.5;i++)
    {
      Color = (int)((double(ColorBar.Right*0.5-i)/double(ColorBar.Right*0.5))*256.0);
      ColorValue = (unsigned long)(pow(Color,Contrast)* ContrastMult);
      BM->Canvas->Pen->Color = (TColor)(ColorValue+Off);
      BM->Canvas->LineTo(i, 0);
      BM->Canvas->MoveTo(i, ColorBar.Bottom);
    }
  for (i=ColorBar.Right*0.5;i<ColorBar.Right;i++)
    {
      Color = (int)((double(i-ColorBar.Right*0.5)/double(ColorBar.Right*0.5))*256.0);
      ColorValue = (unsigned long)(pow(Color,Contrast)* ContrastMult);
      BM->Canvas->Pen->Color = (TColor)(65536*ColorValue+Off);
      BM->Canvas->LineTo(i, 0);
      BM->Canvas->MoveTo(i, ColorBar.Bottom);
    }

  Canvas->Draw(ColorBar.Left + 1,ColorBar.Top + 1,BM);
  BM->ReleasePalette();

  delete BM;


}
//---------------------------------------------------------------------------
void __fastcall TPaletteForm::BCloseClick(TObject *Sender)
{
  Close();	
}
//---------------------------------------------------------------------------
void __fastcall TPaletteForm::TBContrastChange(TObject *Sender)
{
  Invalidate();
}
//---------------------------------------------------------------------------