//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "viewsimloader.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TFormLoadSimFile *FormLoadSimFile;
//---------------------------------------------------------------------------
__fastcall TFormLoadSimFile::TFormLoadSimFile(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormLoadSimFile::BBrowseClick(TObject *Sender)
{
  OpenBitmapDialog->FileName = EFileName->Text;

  if (OpenBitmapDialog->Execute())
	 {
        EFileName->Text = OpenBitmapDialog->FileName;
	 }
}
//---------------------------------------------------------------------------
void __fastcall TFormLoadSimFile::BLoadClick(TObject *Sender)
{
  unsigned long TotalSlantP, TotalAziP;

  SlantPoints = StrToInt(ESlantP->Text);
  AziPoints = StrToInt(EAziP->Text);
  PixelDepth = StrToInt(EAziP->Text);

  ModalResult = mrOk;
  //Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormLoadSimFile::FormCreate(TObject *Sender)
{
  EFileName->Text = "sim1.bin";
  ESlantP->Text = "1530";
  EAziP->Text = "933";
  EBytesP->Text = "1";
  EBytesP->Enabled = FALSE;
}
//---------------------------------------------------------------------------
void __fastcall TFormLoadSimFile::EFileNameChange(TObject *Sender)
{
  FileName = EFileName->Text;
}
//---------------------------------------------------------------------------
void __fastcall TFormLoadSimFile::ESlantPChange(TObject *Sender)
{
  SlantPoints = StrToInt(ESlantP->Text);
}
//---------------------------------------------------------------------------
void __fastcall TFormLoadSimFile::EAziPChange(TObject *Sender)
{
  AziPoints = StrToInt(EAziP->Text);
}
//---------------------------------------------------------------------------
void __fastcall TFormLoadSimFile::EBytesPChange(TObject *Sender)
{
  PixelDepth = StrToInt(EAziP->Text);
}
//---------------------------------------------------------------------------