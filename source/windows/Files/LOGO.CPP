//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "logo.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TLogoForm *LogoForm;
//---------------------------------------------------------------------------
__fastcall TLogoForm::TLogoForm(TComponent* Owner)
  : TForm(Owner)
{
  Width = Image1->Width;
  Height = Image1->Height;
//  Timer1->Enabled = true;
  Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TLogoForm::Timer1Timer(TObject *Sender)
{
  Close();
  Timer1->Enabled = false;
  MainForm->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TLogoForm::FormPaint(TObject *Sender)
{
  Width = Image1->Width;
  Height = Image1->Height;
}
//---------------------------------------------------------------------------