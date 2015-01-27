//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "GeoOpt.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TGeoOptForm *GeoOptForm;
//---------------------------------------------------------------------------
__fastcall TGeoOptForm::TGeoOptForm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGeoOptForm::BCloseClick(TObject *Sender)
{
  Options = CheckBox1->Checked + 2*CheckBox2->Checked + 4*CheckBox3->Checked
     + 8*CheckBox4->Checked + 16*CheckBox5->Checked + 32*CheckBox6->Checked
      + 64*CheckBox7->Checked + 128*CheckBox8->Checked;
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TGeoOptForm::FormShow(TObject *Sender)
{
  if (((Options >> 0) & 1) == 1)  CheckBox1->Checked = true;
  if (((Options >> 1) & 1) == 1)  CheckBox2->Checked = true;
  if (((Options >> 2) & 1) == 1)  CheckBox3->Checked = true;
  if (((Options >> 3) & 1) == 1)  CheckBox4->Checked = true;
  if (((Options >> 4) & 1) == 1)  CheckBox5->Checked = true;
  if (((Options >> 5) & 1) == 1)  CheckBox6->Checked = true;
  if (((Options >> 6) & 1) == 1)  CheckBox7->Checked = true;
  if (((Options >> 7) & 1) == 1)  CheckBox8->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TGeoOptForm::BitBtn1Click(TObject *Sender)
{
  Application->HelpJump("geomopt");
}
//---------------------------------------------------------------------------