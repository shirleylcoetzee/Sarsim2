//---------------------------------------------------------------------------
#ifndef loadfileH
#define loadfileH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Dialogs.hpp>
//---------------------------------------------------------------------------
struct SFileFormat
  {
    AnsiString FileName;
    int FileFormat;   // 0 = SunRasterFile, 1 = Sarsim Sim, 2 = custom
    AnsiString ScriptFile;
    int FileType;    // 0 = Text, 1 = Binary, 2 = IEEE
    unsigned int BytesPerPixel;
    double Offset;
    int Endian;
    int DataType;
    unsigned long HeaderSize;
    unsigned int SizeX;
    unsigned int SizeY;
    double MinValueI, MaxValueI,
           MinValueQ, MaxValueQ,
           MinValueMag, MaxValueMag,
           MinValuePh, MaxValuePh,
           MinValuePower, MaxValuePower;
  };
//---------------------------------------------------------------------------
class TFormLoadSimFile : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GBFile;
	TLabel *Label1;
	TEdit *EFileName;
	TBitBtn *BBrowse;
	TBitBtn *BLoad;
	TGroupBox *GBFormat;
	TRadioButton *RBSunRaster;
	TRadioButton *RBSim;
	TRadioButton *RBCustom;
	TPanel *Panel1;
	TRadioGroup *RGDataType;
	TBitBtn *BCancel;
	TOpenDialog *OpenBitmapDialog;
	TOpenDialog *OpenScriptDialog;
  TGroupBox *GroupBox1;
  TRadioGroup *RGByteOrder;
  TEdit *EBytesPerValue;
  TLabel *Label9;
  TRadioButton *RBFloat;
  TRadioButton *RBBin;
  TRadioButton *RBText;
  TGroupBox *GroupBox2;
  TEdit *EHeader;
  TLabel *Label6;
  TLabel *Label7;
  TEdit *EWidth;
  TEdit *EHeight;
  TLabel *Label8;
  TLabel *Label11;
  TEdit *EOffset;
  TPanel *Panel2;
  TLabel *Label5;
  TEdit *EScriptFileName;
  TBitBtn *BFind;
  TBitBtn *BitBtn1;
	void __fastcall BLoadClick(TObject *Sender);
	void __fastcall BCancelClick(TObject *Sender);
	void __fastcall BBrowseClick(TObject *Sender);
	void __fastcall BFindClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall ChangeSelection(TObject *Sender);
  void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
  void EnableSelectedStuff();
public:		// User declarations
    struct SFileFormat FileFormat;

	__fastcall TFormLoadSimFile(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TFormLoadSimFile *FormLoadSimFile;
//---------------------------------------------------------------------------
#endif
