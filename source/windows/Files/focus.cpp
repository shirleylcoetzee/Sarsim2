//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "focus.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TFocusForm *FocusForm;
//---------------------------------------------------------------------------
__fastcall TFocusForm::TFocusForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFocusForm::FormShow(TObject *Sender)
{
  struct SPlatform *platf;
  char s[180];

  if (MainForm->FocusPlatform == NULL) RBOther->Checked = TRUE;
  else RBPlat->Checked = TRUE;

  PlatformBox->Clear();
  //CheckDlgButton(ID_SELECT_PLATFORM,1);
  platf = MainForm->FirstPlatform;
  while (platf != NULL)
	{
	  sprintf(s,"%s",platf->Name);
	  PlatformBox->Items->Add(s);
	  platf = platf->next;
	} // end while

  // select first Simulation by default
  PlatformBox->ItemIndex = 0;

  sprintf(s,"%1.0f",MainForm->Focus[0]);
  XCoord->Text = s;
  sprintf(s,"%1.0f",MainForm->Focus[1]);
  YCoord->Text = s;
  sprintf(s,"%1.0f",MainForm->Focus[2]);
  ZCoord->Text = s;
}
//---------------------------------------------------------------------------
void __fastcall TFocusForm::BitBtn1Click(TObject *Sender)
{
  int Sel;
  struct SPlatform *platf;

  if (RBPlat->Checked)
    {
	  Sel=PlatformBox->ItemIndex;
	  platf=MainForm->FirstPlatform;
	  while (Sel > 0 && platf != NULL)
	    {
	  	  Sel--;
	  	  platf = platf->next;
	    } // end while

      MainForm->FocusPlatform = platf;
    }
  else
    {
      MainForm->FocusPlatform = NULL;
      MainForm->Focus[0] = atof(XCoord->Text.c_str());
      MainForm->Focus[1] = atof(YCoord->Text.c_str());
      MainForm->Focus[2] = atof(ZCoord->Text.c_str());
    }
  MainForm->CalcNewSTimeArray();
  MainForm->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFocusForm::BitBtn3Click(TObject *Sender)
{
  Application->HelpJump("focus");
}
//---------------------------------------------------------------------------