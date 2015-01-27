//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "targetform.h"
#include "main.h"
#include "linkedlist.h"
#include "graphform.h"
#include "general.h"
#include "engine.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TTargetF *TargetF;
//---------------------------------------------------------------------------
__fastcall TTargetF::TTargetF(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TTargetF::DefClick(TObject *Sender)
{
  Other->Checked = TRUE;
  Target.GainType = GAINOTHER;
  EditDataDef(&(Target.DataDef));
}

//---------------------------------------------------------------------------
void __fastcall TTargetF::FormShow(TObject *Sender)
{
  struct SPlatform *p;               // general pointer to platform
  char str[80];

  PlatformName->Items->Clear();
  // copy data into editboxes etc.
  int count=0, ItemNo;
  p=MainForm->FirstPlatform;
  while (p != NULL)
	{
       if(stricmp(Target.Name, p->Name) == 0)
         ItemNo = count;
       count++;
       PlatformName->Items->Add(p->Name);
       p = p->next;                  // goto next platform
    }

  PlatformName->ItemIndex = ItemNo;
  PlatformName->Text = Target.Name;
  sprintf(str,FORMAT_1,Target.Pos[0]);
  Xcoord->Text = str;
  sprintf(str,FORMAT_1,Target.Pos[1]);
  Ycoord->Text = str;
  sprintf(str,FORMAT_1,Target.Pos[2]);
  Zcoord->Text = str;
  sprintf(str,FORMAT_1,Target.PosDev[0]);
  XcoordDev->Text = str;
  sprintf(str,FORMAT_1,Target.PosDev[1]);
  YcoordDev->Text = str;
  sprintf(str,FORMAT_1,Target.PosDev[2]);
  ZcoordDev->Text = str;
  sprintf(str,FORMAT_1,Target.RCS);
  RCS->Text = str;
  sprintf(str,FORMAT_1,Target.RCSdev);
  RCSDev->Text = str;
  sprintf(str,FORMAT_1,Target.DirAzi*RadToDeg);
  Azi->Text = str;
  sprintf(str,FORMAT_1,Target.DirEle*RadToDeg);
  Ele->Text = str;
  sprintf(str,FORMAT_1,Target.DirAziDev*RadToDeg);
  AziDev->Text = str;
  sprintf(str,FORMAT_1,Target.DirEleDev*RadToDeg);
  EleDev->Text = str;
  if (Target.ReflecType == ISOTROPIC)
    Iso->Checked = TRUE;
  else
    Dir->Checked = TRUE;
  if (Target.GainType == GAINCOS)
	Cos->Checked = TRUE;
  else
    Other->Checked = TRUE;
}

//---------------------------------------------------------------------------
void __fastcall TTargetF::OkBtnClick(TObject *Sender)
{
  if (CanClose())
    ModalResult = mrOk;
}

//---------------------------------------------------------------------------
bool TTargetF::CanClose(void)
{
  SPlatform *P;
  long PFN;

  strncpy(Target.Name,PlatformName->Text.c_str(),MAX_NAME_LENGTH-1);
  FindPlatform(Target.Name, &P, &PFN, MainForm->FirstPlatform);
  if (P == NULL)
    {
      Application->MessageBox("Platform name not found", "Error", MB_OK);
      PlatformName->SetFocus();
      return FALSE;
	}

  Target.Pos[0] = atof(Xcoord->Text.c_str());
  Target.Pos[1] = atof(Ycoord->Text.c_str());
  Target.Pos[2] = atof(Zcoord->Text.c_str());
  Target.PosDev[0] = atof(XcoordDev->Text.c_str());
  Target.PosDev[1] = atof(YcoordDev->Text.c_str());
  Target.PosDev[2] = atof(ZcoordDev->Text.c_str());
  Target.RCS = atof(RCS->Text.c_str());
  Target.RCSdev = atof(RCSDev->Text.c_str());
  if (Iso->Checked) Target.ReflecType = ISOTROPIC;
  else if (Dir->Checked) Target.ReflecType = DIRECTIONAL;
  Target.DirAzi = DegToRad * atof(Azi->Text.c_str());
  Target.DirEle = DegToRad * atof(Ele->Text.c_str());
  Target.DirAziDev = DegToRad * atof(AziDev->Text.c_str());
  Target.DirEleDev = DegToRad * atof(EleDev->Text.c_str());
  if (Cos->Checked) Target.GainType = GAINCOS;
  else if (Other->Checked) Target.GainType = GAINOTHER;
  return TRUE;
}
void __fastcall TTargetF::BitBtn1Click(TObject *Sender)
{
  Application->HelpJump("ntarget");
}
//---------------------------------------------------------------------------
