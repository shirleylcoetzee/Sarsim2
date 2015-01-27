//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "radarform.h"
#include "general.h"
#include "main.h"
#include "linkedlist.h"
#include "engine.h"


// barker codes
extern int BarkerCode[7][13];
extern int BarkerCodeLength[7];

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TRadarF *RadarF;
//---------------------------------------------------------------------------
__fastcall TRadarF::TRadarF(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::OkBtnClick(TObject *Sender)
{
  if (CanClose())
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::FormShow(TObject *Sender)
{
  char str[80];
  struct SPlatform *p;               // general pointer to platform
  int ItemNo;

  // copy data into editboxes etc.
  RadarName->Text = Radar.RadarName;

  PlatformName->Items->Clear();
  int count=0;
  p=MainForm->FirstPlatform;
  while (p != NULL)
	{
       if(stricmp(Radar.PlatformName, p->Name) == 0)
         ItemNo = count;
       count++;
       PlatformName->Items->Add(p->Name);
       p = p->next;                  // goto next platform
    }

  PlatformName->ItemIndex = ItemNo;

  UpdatePulseCheckBoxes();

  CBBarkerC->ItemIndex = Radar.BarkerCode;
  sprintf(str,"%d",BarkerCodeLength[Radar.BarkerCode]);
  CBBarkerC->Text = str;
//  sprintf(str,"%d",Radar.Repetitions);
//  EBarker->Text = str;

  sprintf(str,FORMAT_1,Radar.ChirpBW*NANO);
  ChirpBW->Text = str;
  sprintf(str,FORMAT_1,Radar.PulseWidth*GIGA);
  PulseWidth->Text = str;

  if (Radar.Envelope == RECT_ENV)
    RBRect->Checked = TRUE;
  else if (Radar.Envelope == LINEAR_ENV)
    RBLinear->Checked = TRUE;
  else if (Radar.Envelope == OTHER_ENV)
    RBOtherEnv->Checked = TRUE;

  sprintf(str,FORMAT_1,Radar.RiseTime*GIGA);
  RiseTime->Text = str;
  sprintf(str,FORMAT_1,Radar.FallTime*GIGA);
  FallTime->Text = str;

  if (Radar.PRFType == CONSTANT_PRF)
    RBConstantPRF->Checked = TRUE;
  else if (Radar.PRFType == OTHER_PRF)
    RBOtherPRF->Checked = TRUE;
  sprintf(str,FORMAT_1,Radar.PRF);
  PRF->Text = str;

  if (Radar.FreqType == SINGLE_F)
    RBSingle->Checked = TRUE;
  else if (Radar.FreqType == STEPPED_F)
    RBStepped->Checked = TRUE;
  else if (Radar.FreqType == OTHER_F)
    RBOtherF->Checked = TRUE;

  sprintf(str,FORMAT_1,Radar.SingleFreq*NANO);
  CarrierF->Text = str;
  sprintf(str,FORMAT_1,Radar.StartFreq*NANO);
  StartF->Text = str;
  sprintf(str,FORMAT_1,Radar.StepSize*NANO);
  StepF->Text = str;
  sprintf(str,FORMAT_2,Radar.FreqSteps);
  Steps->Text = str;
  sprintf(str,FORMAT_2,Radar.PulsesPerFreq);
  PulsesPerF->Text = str;

  sprintf(str,FORMAT_1,Radar.PowerOutput*MILLI);
  Power->Text = str;
  sprintf(str,FORMAT_1,FacTodB(Radar.Losses));
  Losses->Text = str;
  sprintf(str,FORMAT_1,Radar.NoiseTemp);
  NoiseT->Text = str;

  if (Radar.AntennaGainTypeT == TRANS_ISO)
    RBTransIso->Checked = TRUE;
  else if (Radar.AntennaGainTypeT == TRANS_SIN)
    RBTransSim->Checked = TRUE;
  else if (Radar.AntennaGainTypeT == TRANS_OTHER)
    RBTransOther->Checked = TRUE;
  sprintf(str,FORMAT_1,Radar.ElevBeamWidthT*RadToDeg);
  EleBW->Text = str;
  sprintf(str,FORMAT_1,Radar.AziBeamWidthT*RadToDeg);
  AziBW->Text = str;

  if (Radar.BeamDirection == DIR_FIX)
    RBFixed->Checked = TRUE;
  else if (Radar.BeamDirection == DIR_CONSTROT)
    RBConstRot->Checked = TRUE;
  else if (Radar.BeamDirection == DIR_SPOT)
    RBSpotMode->Checked = TRUE;
  else if (Radar.BeamDirection == DIR_OTHER)
    RBDirOther->Checked = TRUE;

  sprintf(str,FORMAT_1,Radar.ElevationAngleF*RadToDeg);
  DirEleAng->Text = str;
  sprintf(str,FORMAT_1,Radar.AzimuthAngleF*RadToDeg);
  DirAziAng->Text = str;
  sprintf(str,FORMAT_1,Radar.ElevationAngleR*RadToDeg);
  DirCREle->Text = str;
  sprintf(str,FORMAT_1,Radar.RotationRate*RadToDeg);
  DirSpeed->Text = str;
  sprintf(str,FORMAT_1,Radar.Spot[0]);
  DirSMX->Text = str;
  sprintf(str,FORMAT_1,Radar.Spot[1]);
  DirSMY->Text = str;
  sprintf(str,FORMAT_1,Radar.Spot[2]);
  DirSMZ->Text = str;

  if (Radar.AntennaGainTypeR == REC_ISO)
    RBRecIso->Checked = TRUE;
  else if (Radar.AntennaGainTypeR == REC_SIN)
    RBRecSim->Checked = TRUE;
  else if (Radar.AntennaGainTypeR == REC_SAME)
    RBRecSame->Checked = TRUE;
  else if (Radar.AntennaGainTypeR == REC_OTHER)
    RBRecOther->Checked = TRUE;

  sprintf(str,FORMAT_1,Radar.ElevBeamWidthR*RadToDeg);
  RecEleBW->Text = str;
  sprintf(str,FORMAT_1,Radar.AziBeamWidthR*RadToDeg);
  RecAziBW->Text = str;

  if (Radar.MatchedFilterW == MF_RECT)
    RBMFRec->Checked = TRUE;
  else if (Radar.MatchedFilterW == MF_HANNING)
    RBMFHan->Checked = TRUE;
  else if (Radar.MatchedFilterW == MF_BARTLETT)
    RBMFBar->Checked = TRUE;
  else if (Radar.MatchedFilterW == MF_HAMMING)
    RBMFHam->Checked = TRUE;
  else if (Radar.MatchedFilterW == MF_OTHER)
    RBMFOther->Checked = TRUE;

  sprintf(str,FORMAT_1,Radar.HanningC);
  Constant->Text = str;

  if (Radar.ApplyAGC)
    STCApply->Checked = TRUE;
  if (Radar.AGCType == STC_R4)
    RBStc1over4->Checked = TRUE;
  else if (Radar.AGCType == STC_OTHER)
    RBStcOther->Checked = TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::PulseDefClick(TObject *Sender)
{
  RBOtherPulse->Checked = TRUE;
  Radar.PulseType = OTHER_PULSE;
  EditDataDef(&(Radar.PulseDef));
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::Button1Click(TObject *Sender)
{
  RBOtherEnv->Checked = TRUE;
  Radar.Envelope = OTHER_ENV;
  EditDataDef(&(Radar.EnvelopeDef));
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::Button2Click(TObject *Sender)
{
  RBOtherPRF->Checked = TRUE;
  Radar.PRFType = OTHER_PRF;
  EditDataDef(&(Radar.PRFDef));
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::Button3Click(TObject *Sender)
{
  RBOtherF->Checked = TRUE;
  Radar.FreqType = OTHER_F;
  EditDataDef(&(Radar.FreqDef));
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::Button4Click(TObject *Sender)
{
  RBTransOther->Checked = TRUE;
  Radar.AntennaGainTypeT = TRANS_OTHER;
  EditDataDef(&(Radar.AntennaTDef));
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::Button5Click(TObject *Sender)
{
  RBDirOther->Checked = TRUE;
  Radar.BeamDirection = DIR_OTHER;
  EditDataDef(&(Radar.DirectionTDef));
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::Button6Click(TObject *Sender)
{
  RBRecOther->Checked = TRUE;
  Radar.AntennaGainTypeR = REC_OTHER;
  EditDataDef(&(Radar.AntennaRDef));
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::Button7Click(TObject *Sender)
{
  RBMFOther->Checked = TRUE;
  Radar.MatchedFilterW = MF_OTHER;
  EditDataDef(&(Radar.MFDef));
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::Button8Click(TObject *Sender)
{
  RBStcOther->Checked = TRUE;
  Radar.AGCType = STC_OTHER;
  EditDataDef(&(Radar.AGCDef));
}
//---------------------------------------------------------------------------
bool TRadarF::CanClose(void)
{
  SPlatform *P;
  long PFN;

  strncpy(Radar.PlatformName,PlatformName->Text.c_str(),MAX_NAME_LENGTH-1);
  FindPlatform(Radar.PlatformName, &P, &PFN, MainForm->FirstPlatform);
  if (P == NULL)
    {
      Application->MessageBox("Platform name not found", "Error", MB_OK);
      PlatformName->SetFocus();
      return FALSE;
	}

  strncpy(Radar.RadarName,RadarName->Text.c_str(),MAX_NAME_LENGTH-1);

  if (RBMono->Checked)
    {
      if (CBBarker->Checked)
        Radar.PulseType = BARKER;
      else
        Radar.PulseType = MONO;
    }
  else if (RBChirp->Checked) Radar.PulseType = CHIRP;
  else if (RBOtherPulse->Checked) Radar.PulseType = OTHER_PULSE;

  Radar.BarkerCode = CBBarkerC->ItemIndex;
//  Radar.Repetitions = (int)atof(EBarker->Text.c_str());

  Radar.ChirpBW = atof(ChirpBW->Text.c_str())*GIGA;
  Radar.PulseWidth = atof(PulseWidth->Text.c_str())*NANO;

  if (RBRect->Checked) Radar.Envelope = RECT_ENV;
  else if (RBLinear->Checked) Radar.Envelope = LINEAR_ENV;
  else if (RBOtherEnv->Checked) Radar.Envelope = OTHER_ENV;

  Radar.RiseTime = atof(RiseTime->Text.c_str())*NANO;
  Radar.FallTime = atof(FallTime->Text.c_str())*NANO;

  if (RBConstantPRF->Checked) Radar.PRFType = CONSTANT_PRF;
  else if (RBOtherPRF->Checked) Radar.PRFType = OTHER_PRF;
  Radar.PRF = atof(PRF->Text.c_str());

  if (RBSingle->Checked) Radar.FreqType = SINGLE_F;
  else if (RBStepped->Checked) Radar.FreqType = STEPPED_F;
  else if (RBOtherF->Checked) Radar.FreqType = OTHER_F;

  Radar.SingleFreq = atof(CarrierF->Text.c_str())*GIGA;
  Radar.StartFreq = atof(StartF->Text.c_str())*GIGA;
  Radar.StepSize = atof(StepF->Text.c_str())*GIGA;
  Radar.FreqSteps = atoi(Steps->Text.c_str());
  Radar.PulsesPerFreq = atoi(PulsesPerF->Text.c_str());

  Radar.PowerOutput = atof(Power->Text.c_str())*KILO;
  Radar.Losses = dBToFac(atof(Losses->Text.c_str()));
  Radar.NoiseTemp = atof(NoiseT->Text.c_str());

  if (RBTransIso->Checked) Radar.AntennaGainTypeT = TRANS_ISO;
  else if (RBTransSim->Checked) Radar.AntennaGainTypeT = TRANS_SIN;
  else if (RBTransOther->Checked) Radar.AntennaGainTypeT = TRANS_OTHER;

  Radar.ElevBeamWidthT = atof(EleBW->Text.c_str())*DegToRad;
  Radar.AziBeamWidthT = atof(AziBW->Text.c_str())*DegToRad;

  if (RBFixed->Checked) Radar.BeamDirection = DIR_FIX;
  else if (RBConstRot->Checked) Radar.BeamDirection = DIR_CONSTROT;
  else if (RBSpotMode->Checked) Radar.BeamDirection = DIR_SPOT;
  else if (RBDirOther->Checked) Radar.BeamDirection = DIR_OTHER;

  Radar.ElevationAngleF = atof(DirEleAng->Text.c_str())*DegToRad;
  Radar.AzimuthAngleF = atof(DirAziAng->Text.c_str())*DegToRad;
  Radar.ElevationAngleR = atof(DirCREle->Text.c_str())*DegToRad;
  Radar.RotationRate = atof(DirSpeed->Text.c_str())*DegToRad;
  Radar.Spot[0] = atof(DirSMX->Text.c_str());
  Radar.Spot[1] = atof(DirSMY->Text.c_str());
  Radar.Spot[2] = atof(DirSMZ->Text.c_str());

  if (RBRecIso->Checked) Radar.AntennaGainTypeR = REC_ISO;
  else if (RBRecSim->Checked) Radar.AntennaGainTypeR = REC_SIN;
  else if (RBRecSame->Checked) Radar.AntennaGainTypeR = REC_SAME;
  else if (RBRecOther->Checked) Radar.AntennaGainTypeR = REC_OTHER;

  Radar.ElevBeamWidthR = atof(RecEleBW->Text.c_str())*DegToRad;
  Radar.AziBeamWidthR = atof(RecAziBW->Text.c_str())*DegToRad;

  if (RBMFRec->Checked) Radar.MatchedFilterW = MF_RECT;
  else if (RBMFHan->Checked) Radar.MatchedFilterW = MF_HANNING;
  else if (RBMFBar->Checked) Radar.MatchedFilterW = MF_BARTLETT;
  else if (RBMFHam->Checked) Radar.MatchedFilterW = MF_HAMMING;
  else if (RBMFOther->Checked) Radar.MatchedFilterW = MF_OTHER;

  Radar.HanningC = atof(Constant->Text.c_str());

  Radar.ApplyAGC = STCApply->Checked;

  if (RBStc1over4->Checked) Radar.AGCType = STC_R4;
  else if (RBStcOther->Checked) Radar.AGCType = STC_OTHER;
  return TRUE;
}
//---------------------------------------------------------------------------
void TRadarF::UpdatePulseCheckBoxes()
{
  if (Radar.PulseType == MONO)
     {
       RBMono->Checked = TRUE;
       CBBarker->Enabled = true;
     }
  else if (Radar.PulseType == CHIRP)
    RBChirp->Checked = TRUE;
  else if (Radar.PulseType == OTHER_PULSE)
    RBOtherPulse->Checked = TRUE;
  else if (Radar.PulseType == BARKER)
   {
    RBMono->Checked = true;
    CBBarker->Checked = true;
 //   EBarker->Enabled = true;
    CBBarker->Enabled = true;
    CBBarkerC->Enabled = true;
   }

  if (Radar.PulseType != BARKER)
    {
      CBBarkerC->Enabled = false;
//      EBarker->Enabled = false;
    }
  if (RBChirp->Checked) ChirpBW->Enabled = true;
  else ChirpBW->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::RBMonoClick(TObject *Sender)
{
  Radar.PulseType = MONO;
  if (CBBarker->Checked)
    Radar.PulseType = BARKER;
  UpdatePulseCheckBoxes();
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::RBChirpClick(TObject *Sender)
{
  Radar.PulseType = CHIRP;
  UpdatePulseCheckBoxes();
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::RBOtherPulseClick(TObject *Sender)
{
  Radar.PulseType = OTHER_PULSE;
  UpdatePulseCheckBoxes();
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::CBBarkerClick(TObject *Sender)
{
  if (CBBarker->Checked)
    Radar.PulseType = BARKER;
  else
    Radar.PulseType = MONO;
  UpdatePulseCheckBoxes();
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::BitBtn3Click(TObject *Sender)
{
  Application->HelpJump("nradar");
}
//---------------------------------------------------------------------------
void __fastcall TRadarF::BitBtn4Click(TObject *Sender)
{
  Application->HelpJump("nradar");
}
//---------------------------------------------------------------------------
