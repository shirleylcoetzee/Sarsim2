//---------------------------------------------------------------------------
#ifndef radarformH
#define radarformH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include "objectstructdef.h"

//---------------------------------------------------------------------------
class TRadarF : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TLabel *Label1;
	TComboBox *RadarName;
	TLabel *Label2;
	TComboBox *PlatformName;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TPanel *Panel5;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TRadioButton *RBMono;
	TRadioButton *RBChirp;
	TRadioButton *RBOtherPulse;
	TRadioButton *RBRect;
	TRadioButton *RBLinear;
	TRadioButton *RBOtherEnv;
	TRadioButton *RBConstantPRF;
	TRadioButton *RBOtherPRF;
	TRadioButton *RBSingle;
	TRadioButton *RBStepped;
	TRadioButton *RBOtherF;
	TEdit *ChirpBW;
	TEdit *PulseWidth;
	TEdit *RiseTime;
	TEdit *FallTime;
	TEdit *PRF;
	TEdit *CarrierF;
	TEdit *StartF;
	TEdit *StepF;
	TEdit *Steps;
	TEdit *PulsesPerF;
	TEdit *Power;
	TEdit *Losses;
	TEdit *NoiseT;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label16;
	TLabel *Label17;
	TButton *PulseDef;
	TBitBtn *OkBtn;
	TBitBtn *CancelBtn;
	TLabel *Label18;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TLabel *Label30;
	TLabel *Label31;
	TLabel *Label32;
	TLabel *Label33;
	TLabel *Label34;
	TPanel *Panel6;
	TPanel *Panel7;
	TPanel *Panel8;
	TPanel *Panel9;
	TPanel *Panel10;
	TLabel *Label19;
	TLabel *Label20;
	TLabel *Label21;
	TLabel *Label22;
	TLabel *Label23;
	TRadioButton *RBTransIso;
	TRadioButton *RBTransSim;
	TRadioButton *RBTransOther;
	TRadioButton *RBFixed;
	TRadioButton *RBConstRot;
	TRadioButton *RBSpotMode;
	TRadioButton *RBDirOther;
	TRadioButton *RBRecIso;
	TRadioButton *RBRecSim;
	TRadioButton *RBRecSame;
	TRadioButton *RBRecOther;
	TRadioButton *RBMFRec;
	TRadioButton *RBMFHan;
	TRadioButton *RBMFBar;
	TRadioButton *RBMFHam;
	TRadioButton *RBMFOther;
	TRadioButton *RBStcOther;
	TRadioButton *RBStc1over4;
	TEdit *EleBW;
	TEdit *AziBW;
	TEdit *DirEleAng;
	TEdit *DirAziAng;
	TEdit *DirCREle;
	TEdit *DirSpeed;
	TEdit *DirSMX;
	TEdit *DirSMY;
	TEdit *DirSMZ;
	TEdit *RecEleBW;
	TEdit *RecAziBW;
	TEdit *Constant;
	TCheckBox *STCApply;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TButton *Button7;
	TButton *Button8;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TLabel *Label25;
	TLabel *Label26;
	TLabel *Label27;
	TLabel *Label28;
	TLabel *Label29;
	TLabel *Label35;
	TLabel *Label36;
	TLabel *Label37;
	TLabel *Label38;
	TLabel *Label39;
	TLabel *Label40;
	TLabel *Label24;
  TCheckBox *CBBarker;
  TComboBox *CBBarkerC;
  TBitBtn *BitBtn3;
  TBitBtn *BitBtn4;
	void __fastcall OkBtnClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall PulseDefClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
  void __fastcall RBMonoClick(TObject *Sender);
  void __fastcall RBChirpClick(TObject *Sender);
  void __fastcall RBOtherPulseClick(TObject *Sender);
  void __fastcall CBBarkerClick(TObject *Sender);
  void __fastcall BitBtn3Click(TObject *Sender);
  void __fastcall BitBtn4Click(TObject *Sender);
private:	// User declarations
    bool CanClose(void);
    void UpdatePulseCheckBoxes();
public:		// User declarations
    // radar which is changed
    struct SRadar Radar;
	__fastcall TRadarF(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TRadarF *RadarF;
//---------------------------------------------------------------------------
#endif
