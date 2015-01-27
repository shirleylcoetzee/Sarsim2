//---------------------------------------------------------------------------
#ifndef viewsimloaderH
#define viewsimloaderH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormLoadSimFile : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GBFile;
	TLabel *Label1;
	TEdit *EFileName;
	TBitBtn *BBrowse;
	TGroupBox *GBRes;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *ESlantP;
	TEdit *EAziP;
	TEdit *EBytesP;
	TBitBtn *BLoad;
	TOpenDialog *OpenBitmapDialog;
	void __fastcall BBrowseClick(TObject *Sender);
	void __fastcall BLoadClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall EFileNameChange(TObject *Sender);
	void __fastcall ESlantPChange(TObject *Sender);
	void __fastcall EAziPChange(TObject *Sender);
	void __fastcall EBytesPChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    unsigned long SlantPoints;
    unsigned long AziPoints;
    int PixelDepth;
    AnsiString FileName;

	__fastcall TFormLoadSimFile(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TFormLoadSimFile *FormLoadSimFile;
//---------------------------------------------------------------------------
#endif
