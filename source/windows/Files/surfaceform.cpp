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
TSurfaceF *SurfaceF;
//---------------------------------------------------------------------------
__fastcall TSurfaceF::TSurfaceF(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::FormShow(TObject *Sender)
{
  struct SPlatform *p;               // general pointer to platform
  char str[80];

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
  return TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TSurfaceF::BitBtn1Click(TObject *Sender)
{
  Application->HelpJump("nsurf");
}
//---------------------------------------------------------------------------
