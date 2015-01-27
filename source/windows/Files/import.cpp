//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "import.h"
#include "io.h"
#include "main.h"
#include "general.h"
#include "linkedlist.h"

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TImportForm *ImportForm;
extern STarget  DefaultTargetValues;
//---------------------------------------------------------------------------
__fastcall TImportForm::TImportForm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TImportForm::FormShow(TObject *Sender)
{
  struct SPlatform *p;               // general pointer to platform

  PlatformName->Items->Clear();
  // copy data into editboxes etc.
  p=MainForm->FirstPlatform;
  while (p != NULL)
	  {
      PlatformName->Items->Add(p->Name);
      p = p->next;                  // goto next platform
    }
  if (MainForm->FirstPlatform == NULL)
    PlatformName->ItemIndex = -1;
  else
    {
      PlatformName->ItemIndex = 0;
      PlatformName->Text = MainForm->FirstPlatform->Name;
    }
}
//---------------------------------------------------------------------------
void __fastcall TImportForm::BitBtn2Click(TObject *Sender)
{
  LoadData();
}
//---------------------------------------------------------------------------
void __fastcall TImportForm::BitBtn1Click(TObject *Sender)
{
  if (ImportDialog1->Execute())
    {
      FileName->Text = ImportDialog1->FileName;
      LoadData();
    }
}
//---------------------------------------------------------------------------
void TImportForm::LoadData(void)
{
  #define MAX_DEM_POINTS 10000
  // data points (x,y,z)
  double **DataPoints;
  // allocate memory for data
  DataPoints = DMatrix(0, MAX_DEM_POINTS-1, 0, 2);
  int i;
  long CP, TotalP;
  float x,y,z;
  FILE *infile;

  infile = fopen(ImportDialog1->FileName.c_str(),"rt");
  if (infile)
    {
      CP = 0;
      while (!feof(infile) && (CP<MAX_DEM_POINTS))
        {
          fscanf(infile, "%f", &x);
          fscanf(infile, "%f", &y);
          fscanf(infile, "%f", &z);
          DataPoints[CP][0] = (double)x;// - 3222000;
          DataPoints[CP][1] = (double)y;// - 48000;
          DataPoints[CP][2] = (double)z;
          CP++;
        }
      fclose(infile);
    }
  else
    Application->MessageBox("Error loading data file !","Import Data",MB_OK);

  TotalP = CP;

  if (RGObject->ItemIndex == 1)
    {
      struct STarget t;
      for (CP=0;CP<TotalP;CP++)
        {
          // make a copy of the default target to TargetForm->Target
          CopyTarget(&t,&DefaultTargetValues);
          for (i=0;i<3;i++)
            t.Pos[i] = DataPoints[CP][i];
          AddTarget(&t, &(MainForm->FirstTarget));
        }
    }
 /*
  else if (RGObject->ItemIndex == 0)
    {
      long StartP;
      long MinDistP1, MinDistP2;
      double MinDist1, MinDist2,d;
      StartP = 0;
      MinDist1 = 999999999;
      MinDist2 = 999999999;
      for (CP = 0;CP < TotalP; CP++)
        {
          if (CP != StartP)
            {
              d = Dist3D(DataPoints[StartP],DataPoints[CP]);


            }
        }

      struct SSurface s;
      // make a copy of the default target to TargetForm->Target
      CopySurface(&s,&DefaultSurfaceValues);




    }
  */  
  Free_DMatrix(DataPoints,0,0);
  MainForm->IsDirty = TRUE;
  Close();
/*
// Create a point target structure
struct STarget
  {
	 char   Name[MAX_NAME_LENGTH];      // Name of moving platform structure
	 double Pos[3];                     // Position (m)
	 double PosDev[3];                  // Standard deviation of position (m)
	 double RCS,RCSdev;                 // Cross Section and Std. Deviation
	 int    ReflecType;                 // 0 = Isotropic, 1 = Directional
	 double DirAzi;                     // Pointing direction azimuth
	 double DirEle;                     // Pointing direction elevation
	 double DirAziDev;                  // Standard deviation of azimuth
	 double DirEleDev;                  // Standard deviation of elevation
	 int    GainType;                   // Gain pattern, 0 = cos, 1 = other
	 SDataDefinition DataDef;           // define 'other' gain pattern
	 struct STarget *next;              // Pointer to next element
	 struct STarget *previous;          // Pointer to previous element
  };
*/
}
//---------------------------------------------------------------------------
void __fastcall TImportForm::BitBtn4Click(TObject *Sender)
{
  Application->HelpJump("dem");
}
//---------------------------------------------------------------------------