//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "platformform.h"
#include "linkedlist.h"
#include "general.h"
#include "main.h"
#include "engine.h"

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TPlatformF *PlatformF;
//---------------------------------------------------------------------------
__fastcall TPlatformF::TPlatformF(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPlatformF::OkBtnClick(TObject *Sender)
{
  if (CanClose())
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TPlatformF::TrajDefClick(TObject *Sender)
{
  RBTraj->Checked = TRUE;
  Platform.Stat_Traj = TRAJECTORY;
  EditDataDef(&(Platform.TrajDef));
}
//---------------------------------------------------------------------------
void __fastcall TPlatformF::PosDefClick(TObject *Sender)
{
  RBPosDef->Checked = TRUE;
  Platform.MotionErrorPos = 1;
  EditDataDef(&(Platform.PosDevDef));
}
//---------------------------------------------------------------------------
void __fastcall TPlatformF::RotDefClick(TObject *Sender)
{
  RBRotDef->Checked = TRUE;
  Platform.MotionErrorRot = 1;
  EditDataDef(&(Platform.RotDevDef));
}
//---------------------------------------------------------------------------
void __fastcall TPlatformF::FormShow(TObject *Sender)
{
  // copy data into editboxes etc.
  char str[80];
  PlatformName->Text = Platform.Name;

  if (Platform.Stat_Traj == STATIONARY)
    RBStat->Checked = TRUE;
  else
    RBTraj->Checked = TRUE;

  if (Platform.RotType == ROT_FIXED)
    RBRotFixed->Checked = TRUE;
  else
    RBRotUser->Checked = TRUE;

  sprintf(str,FORMAT_1,Platform.SPos[0]);
  Xcoord->Text = str;
  sprintf(str,FORMAT_1,Platform.SPos[1]);
  Ycoord->Text = str;
  sprintf(str,FORMAT_1,Platform.SPos[2]);
  Zcoord->Text = str;

  sprintf(str,FORMAT_1,Platform.Rot[0]*RadToDeg);
  Xrot->Text = str;
  sprintf(str,FORMAT_1,Platform.Rot[1]*RadToDeg);
  Yrot->Text = str;
  sprintf(str,FORMAT_1,Platform.Rot[2]*RadToDeg);
  Zrot->Text = str;

  sprintf(str,FORMAT_1,Platform.RotRate[0]*RadToDeg);
  Xrotrate->Text = str;
  sprintf(str,FORMAT_1,Platform.RotRate[1]*RadToDeg);
  Yrotrate->Text = str;
  sprintf(str,FORMAT_1,Platform.RotRate[2]*RadToDeg);
  Zrotrate->Text = str;

  if (Platform.Align)
    CBAlign->Checked = TRUE;

  if (Platform.MotionErrorPos == 0)
    RBPosStdDev->Checked = TRUE;
  else
    RBPosDef->Checked = TRUE;
  if (Platform.MotionErrorRot == 0)
    RBRotStdDev->Checked = TRUE;
  else
    RBRotDef->Checked = TRUE;

  sprintf(str,FORMAT_1,Platform.PosDev[0]);
  StdDevXPos->Text = str;
  sprintf(str,FORMAT_1,Platform.PosDev[1]);
  StdDevYPos->Text = str;
  sprintf(str,FORMAT_1,Platform.PosDev[2]);
  StdDevZPos->Text = str;

  sprintf(str,FORMAT_1,Platform.RotDev[0]*RadToDeg);
  StdDevXRot->Text = str;
  sprintf(str,FORMAT_1,Platform.RotDev[1]*RadToDeg);
  StdDevYRot->Text = str;
  sprintf(str,FORMAT_1,Platform.RotDev[2]*RadToDeg);
  StdDevZRot->Text = str;
}
//---------------------------------------------------------------------------
bool TPlatformF::CanClose(void)
{
  strncpy(Platform.Name,PlatformName->Text.c_str(),MAX_NAME_LENGTH-1);

  if (Modify_Old == false)  // make sure new platform does not have name of old one
    {
      // check if platform exists already
      SPlatform *P;
      long PFN;

      FindPlatform(Platform.Name, &P, &PFN, MainForm->FirstPlatform);
      if (P != NULL)
        {
          Application->MessageBox("Platform name exists already", "Error", MB_OK);
          PlatformName->SetFocus();
          return FALSE;
	      }
    }

  if (RBStat->Checked) Platform.Stat_Traj = STATIONARY;
  else if (RBTraj->Checked) Platform.Stat_Traj = TRAJECTORY;
  if (RBRotFixed->Checked) Platform.RotType = ROT_FIXED;
  else if (RBRotUser->Checked) Platform.RotType = ROT_USER;
  Platform.SPos[0] = atof(Xcoord->Text.c_str());
  Platform.SPos[1] = atof(Ycoord->Text.c_str());
  Platform.SPos[2] = atof(Zcoord->Text.c_str());
  Platform.Rot[0] = atof(Xrot->Text.c_str())*DegToRad;
  Platform.Rot[1] = atof(Yrot->Text.c_str())*DegToRad;
  Platform.Rot[2] = atof(Zrot->Text.c_str())*DegToRad;
  Platform.RotRate[0] = atof(Xrotrate->Text.c_str())*DegToRad;
  Platform.RotRate[1] = atof(Yrotrate->Text.c_str())*DegToRad;
  Platform.RotRate[2] = atof(Zrotrate->Text.c_str())*DegToRad;
  if (CBAlign->Checked) Platform.Align = TRUE;
  if (RBPosStdDev->Checked) Platform.MotionErrorPos = 0;
  else if (RBPosDef->Checked) Platform.MotionErrorPos = 1;
  if (RBRotStdDev->Checked) Platform.MotionErrorRot = 0;
  else if (RBRotDef->Checked) Platform.MotionErrorRot = 1;
  Platform.PosDev[0] = atof(StdDevXPos->Text.c_str());
  Platform.PosDev[1] = atof(StdDevYPos->Text.c_str());
  Platform.PosDev[2] = atof(StdDevZPos->Text.c_str());
  Platform.RotDev[0] = atof(StdDevXRot->Text.c_str())*DegToRad;
  Platform.RotDev[1] = atof(StdDevYRot->Text.c_str())*DegToRad;
  Platform.RotDev[2] = atof(StdDevZRot->Text.c_str())*DegToRad;


  return TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TPlatformF::Button1Click(TObject *Sender)
{
  RBRotUser->Checked = TRUE;
  Platform.RotType = ROT_USER;
  EditDataDef(&(Platform.RotDef));
}
//---------------------------------------------------------------------------
void __fastcall TPlatformF::BitBtn1Click(TObject *Sender)
{
  Application->HelpJump("nplat");
}
//---------------------------------------------------------------------------
