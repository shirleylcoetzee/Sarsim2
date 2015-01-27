//---------------------------------------------------------------------------
#ifndef SaveSimDataH
#define SaveSimDataH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Dialogs.hpp>
#include "objectstructdef.h"
//---------------------------------------------------------------------------
class TSaveSimForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TBitBtn *BitBtn1;
	TBitBtn *BClose;
	TBevel *Bevel1;
	TBevel *Bevel2;
	TBevel *Bevel3;
	TBevel *Bevel4;
	TBevel *Bevel5;
	TBevel *Bevel6;
	TBevel *Bevel7;
	TBevel *Bevel8;
	TBevel *Bevel9;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
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
	TLabel *Label18;
	TEdit *ESampleF;
	TEdit *EAtoDbits;
	TEdit *ELSBValue;
	TEdit *EFilename;
	TBitBtn *BSelectFile;
	TLabel *SlantStart;
	TLabel *SlantEnd;
	TLabel *AzimuthStart;
	TLabel *AzimuthEnd;
	TLabel *LNyquist;
	TLabel *SlantP;
	TLabel *AzimuthP;
	TLabel *BytesPerIQ;
	TLabel *FileSize;
	TSaveDialog *SaveDataDialog;
	TRadioGroup *RGType;
	TRadioGroup *RGScript;
	TRadioGroup *RGLSBvalue;
  TLabel *Label19;
  TLabel *Label20;
  TBitBtn *BitBtn2;
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BCloseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BSelectFileClick(TObject *Sender);
	void __fastcall EFilenameExit(TObject *Sender);
	void __fastcall EFilenameKeyPress(TObject *Sender, char &Key);
	void __fastcall ELSBValueExit(TObject *Sender);
	void __fastcall ELSBValueKeyPress(TObject *Sender, char &Key);
	void __fastcall EAtoDbitsExit(TObject *Sender);
	void __fastcall EAtoDbitsKeyPress(TObject *Sender, char &Key);
	void __fastcall ESampleFKeyPress(TObject *Sender, char &Key);
	void __fastcall ESampleFExit(TObject *Sender);
	void __fastcall RGTypeClick(TObject *Sender);
	
	void __fastcall RGLSBvalueClick(TObject *Sender);
	
  
  
  void __fastcall BitBtn2Click(TObject *Sender);
private:	// User declarations
    long PointsSlant;
    long PointsAzi;
    void CalcFileSize(void);
    void CalcLSBvalue();

public:		// User declarations
    struct SSimulation CurrentSim;
    struct SRadar *CRadar;
    double MaxMag;
    void SaveData(double *MaxMagnitude, double *TotalSampleNo,
  double *ClippedSamples,double *ZeroSamples);
	__fastcall TSaveSimForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TSaveSimForm *SaveSimForm;
//---------------------------------------------------------------------------
#endif
