//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "geomform.h"
#include "GeoOpt.h"
#include "main.h"
#include "engine.h"
#include "linkedlist.h"
#include "objectstructdef.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TGeoForm *GeoForm;
//---------------------------------------------------------------------------
__fastcall TGeoForm::TGeoForm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::Options1Click(TObject *Sender)
{
  GeoOptForm->Options = CurrentGeo.ShowVar;
  GeoOptForm->ShowModal();
  CurrentGeo.ShowVar = GeoOptForm->Options;
  UpdateList();
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::RestrictSize(TMessage& Msg)
{
  // set the limits of the graph windows size
  ((POINT far *)Msg.LParam)[3].x = 752;
  ((POINT far *)Msg.LParam)[3].y = 335;
  ((POINT far *)Msg.LParam)[4].x = 2000;
  ((POINT far *)Msg.LParam)[4].y = 2000;
  TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::ClickScrollBar(TMessage& Msg)
{
  Caption = "4";
  TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------
void TGeoForm::UpdateDialog()
{
  struct SGeometry *geo;
  char s[255];
  ListBox1->Items->Clear();
  geo=MainForm->FirstGeometry;
	 while (geo != NULL)
		{
		  sprintf(s,"%s : %.3Gs - %.3Gs (%.4Gm - %.4Gm)",
					 geo->RadarName,geo->AzimuthStart, geo->AzimuthEnd
           , geo->SlantStart, geo->SlantEnd);
		  ListBox1->Items->Add(s);
		  geo = geo->next;
		} // end while

  // select first Geometry by default
 // ListBox1->ItemIndex=0;

  // setup radar list box and find number of selected radar
  char str[80];
  int Sel=0,Counter=0;
  struct SRadar *R;
  RadarList->Items->Clear();
  R = MainForm->FirstRadar;
  // in case name not defined choose first radar
  CRadar =  MainForm->FirstRadar;
  while (R != NULL)
	{
	  if (stricmp(R->RadarName, CurrentGeo.RadarName) == 0)
	    { Sel = Counter; CRadar = R; }
	  RadarList->Items->Add(R->RadarName);
	  R = R->next;
	  Counter++;
	}
  strcpy(CurrentGeo.RadarName, CRadar->RadarName);
  // select by default the first radar
  RadarList->ItemIndex = Sel;

  // set filename
  EFilename->Text = CurrentGeo.FileName;

  // setup window range values
  sprintf(str,FORMAT_5,CurrentGeo.SlantStart);
  EXStart->Text = str;
  sprintf(str,FORMAT_5,CurrentGeo.SlantEnd);
  EXEnd->Text = str;
  sprintf(str,FORMAT_6,CurrentGeo.AzimuthStart);
  EYStart->Text = str;
  sprintf(str,FORMAT_6,CurrentGeo.AzimuthEnd);
  EYEnd->Text = str;

  RGType->ItemIndex = CurrentGeo.FileType;

  UpdateList();
}
//---------------------------------------------------------------------------
void TGeoForm::FindCurrentRadar()
{
  // setup radar list box and find number of selected radar
  strncpy(CurrentGeo.RadarName, RadarList->Text.c_str(),MAX_NAME_LENGTH - 1);
  struct SRadar *R;
  R = MainForm->FirstRadar;
  // in case name not defined choose first radar
  CRadar =  MainForm->FirstRadar;
  while (R != NULL)
	{
	  if (stricmp(R->RadarName, CurrentGeo.RadarName) == 0)
	    { CRadar = R; break; }
	  R = R->next;
	}
  RadarList->Text = CRadar->RadarName;
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::FormShow(TObject *Sender)
{
  UpdateDialog();
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::RadarListChange(TObject *Sender)
{
 FindCurrentRadar();
 UpdateList();
}
//---------------------------------------------------------------------------
void TGeoForm::UpdateList()
{
  #define LIMIT_LINES (long)10000
  long Lines = 0;
  FILE *OutFilep;
  char str[65536];
  SaveGeometry(true);
  // clear the list
  GeoList->Lines->Clear();
  Caption = "Calculating - please wait...";
  GeoList->Visible = false;

  OutFilep = fopen(TEMP_GEO, "rt");  // read from file, text
  if (!OutFilep)
	 {
		sprintf(str,"Cannot open temporary file '%s'.\n"
              "Make sure enough disk space is available.",TEMP_GEO);
		Application->MessageBox(str, "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
	 }
  while (fgets(str,65535,OutFilep) != NULL)
    {
      // get rid of new line characters
      if (str[strlen(str)-1] == 10) str[strlen(str)-1] = 0;
      GeoList->Lines->Add(str);
      Lines++;
      if (Lines>=LIMIT_LINES) break;
    }
  fclose (OutFilep);
  if (Lines>=LIMIT_LINES)
    {
      sprintf(str,"Only first %d pulses shown in list (however all of them "
       "will be written to disk)...", LIMIT_LINES);
		  Application->MessageBox(str, "Warning", MB_OK | MB_ICONEXCLAMATION);
    }
  DeleteFile(TEMP_GEO);   // delete temporary script file
  GeoList->Visible = true;
  Caption = "Geometry";
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::BSelectFileClick(TObject *Sender)
{
  SaveDataDialog->FileName = CurrentGeo.FileName;

  if (SaveDataDialog->Execute())
	 {
        strcpy(CurrentGeo.FileName, SaveDataDialog->FileName.c_str());
        EFilename->Text = CurrentGeo.FileName;
	 }

}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::BCloseClick(TObject *Sender)
{
  ModalResult = mrCancel;
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::FormResize(TObject *Sender)
{
  // put buttons and text in right positions
  BClose->Top = ClientHeight - BClose->Height - 7;
  BSave->Top = ClientHeight - BSave->Height - 7;
  BHelp->Top = ClientHeight - BSave->Height - 7;

  BSave->Left = (ClientWidth *0.5 - BSave->Width) * 0.5;
  BClose->Left = (ClientWidth *1.0 - BClose->Width) * 0.5;
  BHelp->Left = (ClientWidth *1.5 - BSave->Width) * 0.5;
  GeoList->Width = ClientWidth - 16;
//  GeoHeader->Width = ClientWidth - 16;
  GeoList->Height = ClientHeight - GeoList->Top - BClose->Height - 32;
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::EXStartExit(TObject *Sender)
{
  if (CurrentGeo.SlantStart != atof(EXStart->Text.c_str()))
    {
      CurrentGeo.SlantStart =  atof(EXStart->Text.c_str());
      UpdateList();
    }
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::EnterKeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    {
      if (EXStart->Focused()) EXStartExit(Sender);
      if (EXEnd->Focused()) EXEndExit(Sender);
      if (EYStart->Focused()) EYStartExit(Sender);
      if (EYEnd->Focused()) EYEndExit(Sender);
    }
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::EXEndExit(TObject *Sender)
{
  if (CurrentGeo.SlantEnd != atof(EXEnd->Text.c_str()))
    {
      CurrentGeo.SlantEnd =  atof(EXEnd->Text.c_str());
      UpdateList();
    }
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::EYStartExit(TObject *Sender)
{
  if (CurrentGeo.AzimuthStart != atof(EYStart->Text.c_str()))
    {
      CurrentGeo.AzimuthStart =  atof(EYStart->Text.c_str());
      UpdateList();
    }
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::EYEndExit(TObject *Sender)
{
  if (CurrentGeo.AzimuthEnd != atof(EYEnd->Text.c_str()))
    {
      CurrentGeo.AzimuthEnd =  atof(EYEnd->Text.c_str());
      UpdateList();
    }
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::RGTypeClick(TObject *Sender)
{
  CurrentGeo.FileType = RGType->ItemIndex;
  UpdateList();
}
//---------------------------------------------------------------------------
void TGeoForm::SaveGeometry(bool Temp)
{
 struct LinkedLists FP;
 int errorM;
 char str[256];
 char   CopyFileName[MAXPATH];
 strcpy(CopyFileName,CurrentGeo.FileName);
 // necessary to create temporary file ?
 if (Temp)
   strcpy(CurrentGeo.FileName,TEMP_GEO);

      FP.FirstSimulation = MainForm->FirstSimulation;
      FP.FirstPlatform = MainForm->FirstPlatform;
      FP.FirstTarget = MainForm->FirstTarget;
      FP.FirstRadar = MainForm->FirstRadar;
      FP.FirstSurface = MainForm->FirstSurface;
      FP.FirstGeometry = MainForm->FirstGeometry;

      errorM = SaveGeometryData(&CurrentGeo, &FP);
      if (errorM == 1)
   	    {
          sprintf(str,"Error opening file '%s'",CurrentGeo.FileName);
 	        Application->MessageBox(str, "Error", MB_OK);
    	  }
      else if (errorM == 2)
   	    {
          sprintf(str,"Cannot find specified radar '%s'",CurrentGeo.RadarName);
 	        Application->MessageBox(str, "Error", MB_OK);
    	  }
      else if (errorM != 0)
   	    {
          sprintf(str,"File '%s' NOT written due to unknown error...",CurrentGeo.FileName);
 	        Application->MessageBox(str, "Error", MB_OK);
    	  }
 // copy name back
 strcpy(CurrentGeo.FileName,CopyFileName);
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::BSaveClick(TObject *Sender)
{
  char str[255];

  if ((RGScript->ItemIndex == 0) || (RGScript->ItemIndex == 2))
    {
      SaveGeometry(false);
      sprintf(str,"File '%s' successfully written",CurrentGeo.FileName);
	        Application->MessageBox(str, "Info", MB_OK);

      if (RGScript->ItemIndex == 2)
        AddGeometry(&CurrentGeo, &(MainForm->FirstGeometry));
    }
  else if (RGScript->ItemIndex == 1)
    {
      AddGeometry(&CurrentGeo, &(MainForm->FirstGeometry));
    }

  Close();
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::EFilenameKeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    EFilenameExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::EFilenameExit(TObject *Sender)
{
  strcpy(CurrentGeo.FileName,EFilename->Text.c_str());
}
//---------------------------------------------------------------------------
void TGeoForm::FindSelectedGeo()
{
  if (ListBox1->ItemIndex < 0)
	 {
		 Application->MessageBox("Choose a Geometry setup first", "Error", MB_OK);
     GeoSelection = NULL;
     return;
	 }
   // find a pointer to the selected Geometry structure
   int counter=0;
   bool found = FALSE;
   GeoSelection=MainForm->FirstGeometry;
   while (GeoSelection != NULL)
  	 {
  	   if (counter == ListBox1->ItemIndex)
	  	 {
	  		found = TRUE;
	  		break;
	     }
	   counter++;
	   GeoSelection = GeoSelection->next;
	 } // end while
  // just in case
  if (!found) GeoSelection=MainForm->FirstGeometry;
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::BDeleteClick(TObject *Sender)
{
  FindSelectedGeo();
  if (GeoSelection != NULL)
    {
      MainForm->IsDirty = TRUE;
      DeleteGeometry(&(MainForm->FirstGeometry),GeoSelection);
      UpdateDialog();
    }
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::BShowClick(TObject *Sender)
{
  FindSelectedGeo();
  if (GeoSelection != NULL)
    {
      CopyGeometry(&CurrentGeo,GeoSelection);

      UpdateDialog();
    }
}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::ListBox1DblClick(TObject *Sender)
{
  FindSelectedGeo();
  if (GeoSelection != NULL)
    {
      CopyGeometry(&CurrentGeo,GeoSelection);

      UpdateDialog();
    }

}
//---------------------------------------------------------------------------
void __fastcall TGeoForm::BHelpClick(TObject *Sender)
{
  Application->HelpJump("geom");
}
//---------------------------------------------------------------------------
