//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "surfaceform.h"
#include "main.h"
#include "linkedlist.h"
#include "graphform.h"
#include "general.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
extern double GlobalUnderSampleSurf;
TSurfaceF *SurfaceF;
//---------------------------------------------------------------------------
__fastcall TSurfaceF::TSurfaceF(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TSurfaceF::CalcAreaAndDir(double *Area, double *NormVecUnit,
  long *PTUsedPreview, long *PTUsedFinal)
{
  double L10[3], L20[3], L21[3], temp[3][3];
  double NormVec[3];//, NormVecUnit[3];
  double Density;

  temp[0][0] = atof(Xcoord1->Text.c_str());
  temp[0][1] = atof(Ycoord1->Text.c_str());
  temp[0][2] = atof(Zcoord1->Text.c_str());
  temp[1][0] = atof(Xcoord2->Text.c_str());
  temp[1][1] = atof(Ycoord2->Text.c_str());
  temp[1][2] = atof(Zcoord2->Text.c_str());
  temp[2][0] = atof(Xcoord3->Text.c_str());
  temp[2][1] = atof(Ycoord3->Text.c_str());
  temp[2][2] = atof(Zcoord3->Text.c_str());

  SubtractVec(temp[1], temp[0], L10);
  SubtractVec(temp[2], temp[0], L20);
  SubtractVec(temp[2], temp[1], L21);  //Lab[xyz] = vector from b to a

  // calculate unnormalized normal vector (perp vec)
  CrossP(L10, L20, NormVec);               // normal vector
  *Area = FindMag(NormVec)*0.5;
  if (*Area == 0)
    {
      NormVecUnit[0] = 0;
      NormVecUnit[1] = 0;
      NormVecUnit[2] = 0;
      *PTUsedPreview = 0;
      *PTUsedFinal = 0;
      return;
    }
  DivVec(NormVec, FindMag(NormVec),NormVecUnit);

                                  //atof(Zcoord2->Text.c_str());
  if (CBGlobal->Checked)
     Density = atof(ESpacing->Text.c_str()) / GlobalUnderSampleSurf;
  else
     Density = atof(ESpacing->Text.c_str()) / atof(ESample->Text.c_str());

  *PTUsedPreview = *Area * Density;
  *PTUsedFinal = *Area * atof(ESpacing->Text.c_str());

}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::FormShow(TObject *Sender)
{
  struct SPlatform *p;               // general pointer to platform
  char str[80];

  sprintf(str,"(%-.13G)",GlobalUnderSampleSurf);
  LGlobal->Caption = str;

  PlatformName->Items->Clear();
  // copy data into editboxes etc.
  int count=0, ItemNo;
  p=MainForm->FirstPlatform;
  while (p != NULL)
	{
       if(stricmp(Surface.Name, p->Name) == 0)
         ItemNo = count;
       count++;
       PlatformName->Items->Add(p->Name);
       p = p->next;                  // goto next platform
    }

  PlatformName->ItemIndex = ItemNo;
  PlatformName->Text = Surface.Name;
  sprintf(str,FORMAT_1,Surface.Tri[0][0]);
  Xcoord1->Text = str;
  sprintf(str,FORMAT_1,Surface.Tri[0][1]);
  Ycoord1->Text = str;
  sprintf(str,FORMAT_1,Surface.Tri[0][2]);
  Zcoord1->Text = str;
  sprintf(str,FORMAT_1,Surface.Tri[1][0]);
  Xcoord2->Text = str;
  sprintf(str,FORMAT_1,Surface.Tri[1][1]);
  Ycoord2->Text = str;
  sprintf(str,FORMAT_1,Surface.Tri[1][2]);
  Zcoord2->Text = str;
  sprintf(str,FORMAT_1,Surface.Tri[2][0]);
  Xcoord3->Text = str;
  sprintf(str,FORMAT_1,Surface.Tri[2][1]);
  Ycoord3->Text = str;
  sprintf(str,FORMAT_1,Surface.Tri[2][2]);
  Zcoord3->Text = str;

  if (Surface.GlobalUnderSample == 1)
    CBGlobal->Checked = true;
  else
    CBGlobal->Checked = false;
  sprintf(str,FORMAT_1,Surface.USampleFactor);
  ESample->Text = str;
  sprintf(str,FORMAT_1,Surface.PTDensity);
  ESpacing->Text = str;
  sprintf(str,FORMAT_1,Surface.InPlaneStdDev);
  EInPlane->Text = str;
  sprintf(str,FORMAT_1,Surface.OutPlaneStdDev);
  EOutPlane->Text = str;
//  RGPattern->ItemIndex = Surface.Pattern;
  if (Surface.Reflec == ISOTROPIC)
    Iso->Checked = TRUE;
  else
    Dir->Checked = TRUE;
  if (Surface.Gain == GAINCOS)
	Cos->Checked = TRUE;
  else
    Other->Checked = TRUE;
  if (Surface.BothSidesReflect == 1)
    CBBothSides->Checked = true;
  else
    CBBothSides->Checked = false;
  if (Surface.AssumeConstNormal == 1)
    CBConstNormal->Checked = true;
  else
    CBConstNormal->Checked = false;
  sprintf(str,FORMAT_1,Surface.RCSdev);
  ERCSdev->Text = str;
  sprintf(str,FORMAT_1,Surface.RCSmult);
  ERCSmult->Text = str;

  sprintf(str,FORMAT_1,Surface.PosSeed);
  EPosSeed->Text = str;
  sprintf(str,FORMAT_1,Surface.RCSSeed);
  ERCSSeed->Text = str;

  CcoordChange();

}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::OkBtnClick(TObject *Sender)
{
  if (CanClose())
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
bool TSurfaceF::CanClose(void)
{
  SPlatform *P;
  long PFN;

  strncpy(Surface.Name,PlatformName->Text.c_str(),MAX_NAME_LENGTH-1);
  FindPlatform(Surface.Name, &P, &PFN, MainForm->FirstPlatform);
  if (P == NULL)
    {
      Application->MessageBox("Platform name not found", "Error", MB_OK);
      PlatformName->SetFocus();
      return FALSE;
	}

  Surface.Tri[0][0] = atof(Xcoord1->Text.c_str());
  Surface.Tri[0][1] = atof(Ycoord1->Text.c_str());
  Surface.Tri[0][2] = atof(Zcoord1->Text.c_str());
  Surface.Tri[1][0] = atof(Xcoord2->Text.c_str());
  Surface.Tri[1][1] = atof(Ycoord2->Text.c_str());
  Surface.Tri[1][2] = atof(Zcoord2->Text.c_str());
  Surface.Tri[2][0] = atof(Xcoord3->Text.c_str());
  Surface.Tri[2][1] = atof(Ycoord3->Text.c_str());
  Surface.Tri[2][2] = atof(Zcoord3->Text.c_str());

  if (CBGlobal->Checked) Surface.GlobalUnderSample = 1;
  else Surface.GlobalUnderSample = 0;

  Surface.USampleFactor = atof(ESample->Text.c_str());
  Surface.PTDensity = atof(ESpacing->Text.c_str());
  Surface.InPlaneStdDev = atof(EInPlane->Text.c_str());
  Surface.OutPlaneStdDev = atof(EOutPlane->Text.c_str());
  Surface.PosSeed = atof(EPosSeed->Text.c_str());
  Surface.RCSSeed = atof(ERCSSeed->Text.c_str());

//  Surface.Pattern = RGPattern->ItemIndex;

  Surface.RCSdev = atof(ERCSdev->Text.c_str());
  Surface.RCSmult = atof(ERCSmult->Text.c_str());

  if (Cos->Checked) Surface.Gain = GAINCOS;
  else if (Other->Checked) Surface.Gain = GAINOTHER;

  if (CBBothSides->Checked)
    Surface.BothSidesReflect = 1;
  else
    Surface.BothSidesReflect = 0;

  if (CBConstNormal->Checked)
    Surface.AssumeConstNormal = 1;
  else
    Surface.AssumeConstNormal = 0;

  if (Iso->Checked)
    Surface.Reflec = 0;//ISOTROPIC;
  else
    Surface.Reflec = 1; //GAINCOS;

  return TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::BitBtn1Click(TObject *Sender)
{
  Application->HelpJump("nsurf");
}
//---------------------------------------------------------------------------
void TSurfaceF::CcoordChange()
{

  char str[80];
  double NormVecUnit[3],Area;
  long PTUsedPreview, PTUsedFinal;

  CalcAreaAndDir(&Area, NormVecUnit, &PTUsedPreview, &PTUsedFinal);
  sprintf(str,"%-8.2f",Area);
  Label37->Caption = str;
  sprintf(str,"%ld",PTUsedPreview);
  Label34->Caption = str;
  sprintf(str,"%ld",PTUsedFinal);
  Label35->Caption = str;

}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Button1Click(TObject *Sender)
{
  double temp[3];
  char str[80];

  temp[0] = atof(Xcoord1->Text.c_str());
  temp[1] = atof(Ycoord1->Text.c_str());
  temp[2] = atof(Zcoord1->Text.c_str());
  Xcoord1->Text = Xcoord2->Text;
  Ycoord1->Text = Ycoord2->Text;
  Zcoord1->Text = Zcoord2->Text;
  sprintf(str,FORMAT_1,temp[0]);
  Xcoord2->Text = str;
  sprintf(str,FORMAT_1,temp[1]);
  Ycoord2->Text = str;
  sprintf(str,FORMAT_1,temp[2]);
  Zcoord2->Text = str;
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Xcoord1KeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    Xcoord1Exit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Xcoord1Exit(TObject *Sender)
{
  CcoordChange();
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Ycoord1Exit(TObject *Sender)
{
  CcoordChange();
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Ycoord1KeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    Ycoord1Exit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Zcoord1Exit(TObject *Sender)
{
  CcoordChange();
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Zcoord1KeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    Zcoord1Exit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Xcoord2Exit(TObject *Sender)
{
  CcoordChange();
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Xcoord2KeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    Xcoord2Exit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Ycoord2Exit(TObject *Sender)
{
  CcoordChange();
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Ycoord2KeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    Ycoord2Exit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Zcoord2Exit(TObject *Sender)
{
  CcoordChange();
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Zcoord2KeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    Zcoord2Exit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Xcoord3Exit(TObject *Sender)
{
  CcoordChange();
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Xcoord3KeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    Xcoord3Exit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Ycoord3Exit(TObject *Sender)
{
  CcoordChange();
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Ycoord3KeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    Ycoord3Exit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Zcoord3Exit(TObject *Sender)
{
  CcoordChange();
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::Zcoord3KeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    Zcoord3Exit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::CBGlobalClick(TObject *Sender)
{
  ESample->Enabled = !CBGlobal->Checked;
  CcoordChange();
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::ESampleExit(TObject *Sender)
{
  CcoordChange();
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::ESampleKeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    ESampleExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::ESpacingExit(TObject *Sender)
{
  CcoordChange();
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::ESpacingKeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    ESpacingExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::IsoClick(TObject *Sender)
{
  Dir->Checked = false;	
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::DirClick(TObject *Sender)
{
  Iso->Checked = false;	
}
//---------------------------------------------------------------------------
