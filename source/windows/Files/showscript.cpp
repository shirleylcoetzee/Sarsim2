//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "showscript.h"
#include "main.h"
#include "general.h"
#include "engine.h"

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TShowScriptForm *ShowScriptForm;
//---------------------------------------------------------------------------
__fastcall TShowScriptForm::TShowScriptForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TShowScriptForm::FormShow(TObject *Sender)
{
  FillMemo(MScript);
}
//---------------------------------------------------------------------------
void __fastcall TShowScriptForm::BUpdateClick(TObject *Sender)
{
  FillMemo(MScript);
}
//---------------------------------------------------------------------------
void __fastcall TShowScriptForm::FormResize(TObject *Sender)
{
  BClose->Top = ClientHeight - BClose->Height - 8;
  BClose->Left = ClientWidth * 0.5 - BClose->Width - BInterpret->Width*0.5 - 30;
  BHelp->Top = ClientHeight - BClose->Height - 8;
  BHelp->Left = ClientWidth * 0.5 + BInterpret->Width*0.5 + 30;
  BInterpret->Top = ClientHeight - BInterpret->Height - 8;
  BInterpret->Left = ClientWidth * 0.5 - BInterpret->Width*0.5;
  MScript->Height = ClientHeight - 50;
}
//---------------------------------------------------------------------------
void __fastcall TShowScriptForm::BInterpretClick(TObject *Sender)
{
  // save memo box contents to a temporary file
  FILE *OutFilep;
  int i;

  OutFilep = fopen(TEMP_SCRIPT, "wt");  // append to file, if not exists, create it
  if (!OutFilep)
	 {
		Application->MessageBox("Cannot write temporary file !","Error", MB_OK | MB_ICONEXCLAMATION);
    return;
	 }
 	// now save data
  for (i=0; i<MScript->Lines->Count;i++)
    {
      fprintf(OutFilep,MScript->Lines->Strings[i].c_str());
      fprintf(OutFilep,"\n");
    }

  fclose(OutFilep);

  MainForm->ReloadScript();
  Application->MessageBox("Finished re-interpreting script...","Info", MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TShowScriptForm::BHelpClick(TObject *Sender)
{
  Application->HelpJump("scriptf");
}
//---------------------------------------------------------------------------
