//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "SaveSimData.h"
#include "objectstructdef.h"
#include "engine.h"
#include "general.h"
#include "math.h"
#include "SaveProgress.h"
#include "WaitForThread.h"
#include "linkedlist.h"
#include "main.h"
extern bool UserAbort;
extern int CalcProgress2;
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TSaveSimForm *SaveSimForm;
//---------------------------------------------------------------------------
__fastcall TSaveSimForm::TSaveSimForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::BitBtn1Click(TObject *Sender)
{
/*
// modification for automatic turning
  int i;
  for (i=0;i<180;i++)
  {
  char drive[MAXDRIVE],dir[MAXDIR],
		 name[MAXFILE],ext[MAXEXT], s[MAXPATH];
  strcpy(s,EFilename->Text.c_str());
  fnsplit(s,drive,dir,name,ext);

  sprintf(s,"%s%s%s%03d.prn",drive,dir,name,i);
  strcpy(CurrentSim.FileName,s);
  MainForm->FirstPlatform->next->Rot[2] = (270-i)*DegToRad;
*/
  strcpy(CurrentSim.FileName,EFilename->Text.c_str());


  if (RGScript->ItemIndex == 0)
    {
      SaveProgressForm->ShowModal();
    }
  else if (RGScript->ItemIndex == 1)
    {
      MainForm->IsDirty = TRUE;
      AddSimulation(&CurrentSim, &(MainForm->FirstSimulation));
    }
  else if (RGScript->ItemIndex == 2)
    {
      MainForm->IsDirty = TRUE;
      SaveProgressForm->ShowModal();
      AddSimulation(&CurrentSim, &(MainForm->FirstSimulation));
    }
  Close();
//  }
}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::BCloseClick(TObject *Sender)
{
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::FormShow(TObject *Sender)
{
  char s[80];

  sprintf(s,FORMAT_5, CurrentSim.SlantStart);
  SlantStart->Caption = s;
  sprintf(s,FORMAT_5, CurrentSim.SlantEnd);
  SlantEnd->Caption = s;
  sprintf(s,FORMAT_6, CurrentSim.AzimuthStart);
  AzimuthStart->Caption = s;
  sprintf(s,FORMAT_6, CurrentSim.AzimuthEnd);
  AzimuthEnd->Caption = s;
  sprintf(s,FORMAT_6, CurrentSim.SampleFreq / FindNyquistRate(CRadar));
  ESampleF->Text = s;
  sprintf(s,"%-.4G", FindNyquistRate(CRadar)*NANO);
  LNyquist->Caption = s;

  FindPixelAziSlant(MainForm->FirstRadar,&PointsAzi, &PointsSlant, &CurrentSim);
  SlantP->Caption = IntToStr(PointsSlant);
  AzimuthP->Caption = IntToStr(PointsAzi);

  CalcFileSize();

  EAtoDbits->Text = IntToStr(CurrentSim.A2Dbits);

   // calculate LSB value
  CalcLSBvalue();

/*  if (CurrentSim.A2Dbits == 1)
    CurrentSim.LSBvalue = MaxMag/2;
  else
    CurrentSim.LSBvalue = MaxMag/(pow(2,(CurrentSim.A2Dbits-1))-1);
  sprintf(s,"%-.6G", CurrentSim.LSBvalue*KILO);
  ELSBValue->Text = s;
*/
  EFilename->Text = CurrentSim.FileName;
  if (CurrentSim.LSBvalue == 0)
    {
      RGLSBvalue->ItemIndex = 1; // LSBvalue is not known, has to be set by user
      ELSBValue->Enabled = TRUE;
    }
  else
    {
      RGLSBvalue->ItemIndex = 0; // estimated
      ELSBValue->Enabled = FALSE;
    }

  if (CurrentSim.FileType == ASCII)
    {
      RGType->ItemIndex = 1;
    }
  else if (CurrentSim.FileType == BINARY)
    {
      RGType->ItemIndex = 0;
    }
  else if (CurrentSim.FileType == ASCIIwithBrackets)
    {
      RGType->ItemIndex = 2;
    }

}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::BSelectFileClick(TObject *Sender)
{
  SaveDataDialog->FileName = CurrentSim.FileName;

  if (SaveDataDialog->Execute())
	 {
        strcpy(CurrentSim.FileName, SaveDataDialog->FileName.c_str());
        EFilename->Text = CurrentSim.FileName;
	 }
}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::EFilenameExit(TObject *Sender)
{
  strcpy(CurrentSim.FileName, SaveDataDialog->FileName.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::EFilenameKeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    EFilenameExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::ELSBValueExit(TObject *Sender)
{
  char s[60];
  double temp;
  temp = atof(ELSBValue->Text.c_str());

  if (temp <= 0)
    {
      sprintf(s,"%-.6G", CurrentSim.LSBvalue*KILO);
      ELSBValue->Text = s;
      return;
    }
  else
    CurrentSim.LSBvalue = temp * MILLI;
}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::ELSBValueKeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    ELSBValueExit(Sender);
}
//---------------------------------------------------------------------------
void TSaveSimForm::CalcLSBvalue()
{
  char s[60];
  #define MARGIN 1.03  // to avoid clipping let's assume MaxMag = MARGIN*actual
  // MaxMag
   // calculate LSB value
  if (CurrentSim.A2Dbits == 1)
    CurrentSim.LSBvalue = (MARGIN*MaxMag)/2;
  else
    CurrentSim.LSBvalue = (MARGIN*MaxMag)/(pow(2,(CurrentSim.A2Dbits-1))-1);
  sprintf(s,"%-.6G", CurrentSim.LSBvalue*KILO);
  ELSBValue->Text = s;
}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::EAtoDbitsExit(TObject *Sender)
{
  char s[60];
  int temp;
  temp = atoi(EAtoDbits->Text.c_str());

  if ((temp < 1) || (temp > 64))
    {
      sprintf(s,"%d", CurrentSim.A2Dbits);
      EAtoDbits->Text = s;
      return;
    }
  CurrentSim.A2Dbits = temp;

  if (RGLSBvalue->ItemIndex == 0)  // estimated
    {
      // calculate LSB value
      CalcLSBvalue();
     /*
      if (CurrentSim.A2Dbits == 1)
        CurrentSim.LSBvalue = MaxMag/2;
      else
        CurrentSim.LSBvalue = MaxMag/(pow(2,(CurrentSim.A2Dbits-1))-1);
      sprintf(s,"%-.6G", CurrentSim.LSBvalue*KILO);
      ELSBValue->Text = s;
     */
    }

  CalcFileSize();
}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::EAtoDbitsKeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    EAtoDbitsExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::ESampleFKeyPress(TObject *Sender, char &Key)
{
  if (Key == (unsigned char)ENTER_KEY)
    ESampleFExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::ESampleFExit(TObject *Sender)
{
  char s[60];
  double temp;
  temp = atof(ESampleF->Text.c_str());

  if (temp <= 0)
    {
      sprintf(s,"%-.6G", CurrentSim.SampleFreq / FindNyquistRate(CRadar));
      ESampleF->Text = s;
      return;
    }
  CurrentSim.SampleFreq = temp * FindNyquistRate(CRadar);
  FindPixelAziSlant(MainForm->FirstRadar, &PointsAzi, &PointsSlant, &CurrentSim);
  SlantP->Caption = IntToStr(PointsSlant);
  CalcFileSize();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TSaveSimForm::SaveData(double *MaxMagnitude, double *TotalSampleNo,
  double *ClippedSamples,double *ZeroSamples)
{
  int errorM;
  struct LinkedLists FP;
  char str[150];

  FP.FirstSimulation = MainForm->FirstSimulation;
  FP.FirstPlatform = MainForm->FirstPlatform;
  FP.FirstTarget = MainForm->FirstTarget;
  FP.FirstRadar = MainForm->FirstRadar;
  FP.FirstSurface = MainForm->FirstSurface;
  FP.FirstGeometry = MainForm->FirstGeometry;

  errorM = SaveSimuData(MaxMagnitude, &CurrentSim, &FP,PointsSlant,
    TotalSampleNo, ClippedSamples,ZeroSamples);

  if (errorM == 2) // radar not found
    {
      sprintf(str,"Could not find radar '%s'",CurrentSim.RadarName);
 	    Application->MessageBox(str, "Error", MB_OK);
    }
  else if (errorM == 3)
    {
      sprintf(str,"Error writing file '%s'",CurrentSim.FileName);
 	    Application->MessageBox(str, "Error", MB_OK);
    }
/*
  #define PulsesPerSession 100
  long FirstPulse, LastPulse,TotalPulseNo,PulseNo,SurfaceNo;
  long SavePulseStart, SavePulseEnd;
  long SamplePoints, TargetNo,PNo,i;
  bool Finished=FALSE;
  double *PulseFreq, **RangeDelay, **TargetRadialVel, **ReturnAmp,
			**Data,*PulseSendTime,***SurfP;
  double I,Q,MaxMagnitudeTemp;
  double FracPart,IntPart, Norm, Center;
  int bytes;
  int A2DBytes = (int)ceil((float)CurrentSim.A2Dbits*(float)0.125);
  FILE *OutFilep;
  char str[150];

  OutFilep = fopen(CurrentSim.FileName, "wb");  // append to file, if not exists, create it
  if (!OutFilep)
 	{
      sprintf(str,"Error writing file '%s'",CurrentSim.FileName);
 	  Application->MessageBox(str, "Error", MB_OK);
 	  return ;
 	}

  // figure out how many pulses to write
  FindPulsesInRange(CurrentSim.AzimuthStart,CurrentSim.AzimuthEnd,
				    CRadar,&FirstPulse,&LastPulse);
  TotalPulseNo = (LastPulse - FirstPulse) + 1;
  if (TotalPulseNo <= 0) TotalPulseNo = 1;
  // total number of targets
  TargetNo = CountTargets(MainForm->FirstTarget);
  // total number of surfaces
  SurfaceNo = CountSurfaces(MainForm->FirstSurface);
   // number of sample points in slant range
  SamplePoints = PointsSlant;
  // create arrays...
  PulseFreq = DVector(0,PulsesPerSession-1);
  RangeDelay = DMatrix(0,TargetNo-1,0,PulsesPerSession-1);
  TargetRadialVel = DMatrix(0,TargetNo-1,0,PulsesPerSession-1);
  ReturnAmp = DMatrix(0,TargetNo-1,0,PulsesPerSession-1);
  SurfP = DMatrix3(0,SurfaceNo-1,0,PulsesPerSession-1,0,8);
  PulseSendTime = DVector(0,PulsesPerSession-1);
  Data = DMatrix(0,PulsesPerSession-1,0,SamplePoints*2-1);
  // now write the pulses in session for memory reasons (e.g. 0-99,100-199 etc.)
  SavePulseEnd = -1;
  // calculate the multiplication factor such that LSB is the right amp.
  Norm = (double)1/(CurrentSim.LSBvalue);
  *MaxMagnitude = 0; // let's find also maximum magnitude
  do
	{
	  SavePulseStart = SavePulseEnd+1;
	  SavePulseEnd = SavePulseStart+PulsesPerSession-1;
	  if (SavePulseEnd >= (TotalPulseNo-1))
		 {
			SavePulseEnd = TotalPulseNo-1;
			Finished = TRUE;
		 }
      PulseNo = SavePulseEnd - SavePulseStart + 1;

      CalcProgress2 = ((float)SavePulseStart/(float)TotalPulseNo)*100;

	  // calculate time when each pulse is sent and the corresponding freq
	  for (PNo=0;PNo<PulseNo;PNo++)
		 {
			PulseSendTime[PNo] = FindPulseSendTime(PNo+FirstPulse+SavePulseStart,
									 CRadar);
			PulseFreq[PNo] = FindPulseFreq(PNo+FirstPulse+SavePulseStart,
								 CRadar);
		 }

      // calculate range-delay, return amp etc. for each pulse
      CalcGeometry(RangeDelay, ReturnAmp, TargetRadialVel, CRadar, PulseSendTime,
                  PulseNo, MainForm->FirstTarget, MainForm->FirstPlatform,
                  MainForm->FirstSurface, SurfP);

      // Calculate Data array of size (PulseNo x SamplePoints)
      CalcArray2(CRadar, &CurrentSim, Data, PulseNo, SamplePoints, PulseFreq,
                 RangeDelay, TargetRadialVel, ReturnAmp, &MaxMagnitudeTemp,
                 MainForm->FirstTarget, MainForm->FirstPlatform,
                 MainForm->FirstSurface, SurfP);
      if (MaxMagnitudeTemp > *MaxMagnitude) *MaxMagnitude = MaxMagnitudeTemp;
     if (UserAbort) break;
	  // calculate the offset
     if (CurrentSim.A2Dbits == 1)
       Center = 1;
     else
	    Center = pow(2,(CurrentSim).A2Dbits-1)-1;
	  // write array to file
	  if ((CurrentSim).FileType == ASCII) //ASCII
		 {
			for (PNo=0;PNo<PulseNo;PNo++)
			  {
			     if (UserAbort) break;
            	 for (i=0;i<SamplePoints;i++)
					{
					  I = round(Norm*Data[PNo][2*i]);
					  Q = round(Norm*Data[PNo][2*i+1]);
					  fprintf(OutFilep,"%1.0f %1.0f\n",I,Q);
					}
				 fprintf(OutFilep,"\n");
			  }
		  }
	  else     // BINARY
		 {
			for (PNo=0;PNo<PulseNo;PNo++)
			  {
                 if (UserAbort) break;
				 for (i=0;i<SamplePoints;i++)
					{
					  I = Center + round(Norm*Data[PNo][2*i]);
					  Q = Center + round(Norm*Data[PNo][2*i+1]);
					  for (bytes = 0;bytes < A2DBytes; bytes++)
						 {
							I = I/256;
							FracPart = modf(I,&IntPart);
							fprintf(OutFilep,"%c",(char)(FracPart*256));
							I = IntPart;
						 }
					  for (bytes = 0;bytes < A2DBytes; bytes++)
						 {
							Q = Q/256;
							FracPart = modf(Q,&IntPart);
							fprintf(OutFilep,"%c",(char)(FracPart*256));
							Q = IntPart;
						 }
					}
			  }
		  }
	} while (!Finished);

  // and free arrays again
  Free_DVector(PulseFreq,0);
  Free_DMatrix(RangeDelay,0,0);
  Free_DMatrix(TargetRadialVel,0,0);
  Free_DMatrix(ReturnAmp,0,0);
  Free_DVector(PulseSendTime,0);
  Free_DMatrix(Data,0,0);
  Free_DMatrix3(SurfP,0,0,0);
  fclose(OutFilep);
*/
}

//---------------------------------------------------------------------------
void TSaveSimForm::CalcFileSize(void)
{
  char s[80];
  if (RGType->ItemIndex == 0)
    {
      int A2DBytes = (int)ceil((float)CurrentSim.A2Dbits*(float)0.125);
      BytesPerIQ->Caption = A2DBytes * 2;
    }
  else if (RGType->ItemIndex == 1)
    {
  	  // estimate number of bytes necessary for each IQ value
	  sprintf(s,"%1.0f ",pow(2,CurrentSim.A2Dbits));
	  BytesPerIQ->Caption = IntToStr(2*strlen(s));
    }
  else if (RGType->ItemIndex == 2)
    {
  	  // estimate number of bytes necessary for each IQ value+brackets
	    sprintf(s,"%1.0f ",pow(2,CurrentSim.A2Dbits));
	    BytesPerIQ->Caption = IntToStr(2*strlen(s)+3);
    }

  sprintf(s,"%d", int(ceil(PointsSlant * PointsAzi * atoi(BytesPerIQ->Caption.c_str()) / 1024)));
  FileSize->Caption = s;
}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::RGTypeClick(TObject *Sender)
{
  char drive[MAXDRIVE],dir[MAXDIR],
		 name[MAXFILE],ext[MAXEXT], s[MAXPATH];
  strcpy(s,EFilename->Text.c_str());
  fnsplit(s,drive,dir,name,ext);

  if (RGType->ItemIndex == 0)
    sprintf(s,"%s%s%s.bin",drive,dir,name);
  else
    sprintf(s,"%s%s%s.asc",drive,dir,name);
  EFilename->Text = s;
  strcpy(CurrentSim.FileName, s);
  if (RGType->ItemIndex == 0)    // binary
    CurrentSim.FileType = BINARY;
  else if (RGType->ItemIndex == 1) // ascii 1
    CurrentSim.FileType = ASCII;
  else                             // ascii with brackets
    CurrentSim.FileType = ASCIIwithBrackets;

  CalcFileSize();
}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::RGLSBvalueClick(TObject *Sender)
{
  if  (RGLSBvalue->ItemIndex == 0) //estimated
    {
      ELSBValue->Enabled = FALSE;
    }
  else
    {
      ELSBValue->Enabled = TRUE;
    }
}
//---------------------------------------------------------------------------
void __fastcall TSaveSimForm::BitBtn2Click(TObject *Sender)
{
  Application->HelpJump("savedat");
}
//---------------------------------------------------------------------------
