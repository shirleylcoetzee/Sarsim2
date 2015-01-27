//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "loaddata.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TFormLoadSimFile *FormLoadSimFile;
//---------------------------------------------------------------------------
__fastcall TFormLoadSimFile::TFormLoadSimFile(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TFormLoadSimFile::EnableSelectedStuff()
{
  if (RBSunRaster->Checked)
    {
      Label9->Enabled = false;
      Label11->Enabled = false;
      EBytesPerValue->Enabled = false;
      EOffset->Enabled = false;
      RGByteOrder->Enabled = false;
      RBText->Enabled = false;
      RBBin->Enabled = false;
      RBFloat->Enabled = false;
      RGDataType->Enabled = false;
      Label6->Enabled = false;
      Label7->Enabled = false;
      Label8->Enabled = false;
      EHeader->Enabled = false;
      EWidth->Enabled = false;
      EHeight->Enabled = false;
      Label5->Enabled = false;
      EScriptFileName->Enabled = false;
      BFind->Enabled = false;

    }
  else if (RBSim->Checked)
    {
      Label9->Enabled = false;
      Label11->Enabled = false;
      EBytesPerValue->Enabled = false;
      EOffset->Enabled = false;
      RGByteOrder->Enabled = false;
      RBText->Enabled = false;
      RBBin->Enabled = false;
      RBFloat->Enabled = false;
      RGDataType->Enabled = false;
      Label6->Enabled = false;
      Label7->Enabled = false;
      Label8->Enabled = false;
      EHeader->Enabled = false;
      EWidth->Enabled = false;
      EHeight->Enabled = false;
      Label5->Enabled = true;
      EScriptFileName->Enabled = true;
      BFind->Enabled = true;
    }
  else
    {
      Label9->Enabled = true;
      Label11->Enabled = true;
      EBytesPerValue->Enabled = true;
      EOffset->Enabled = true;
      RGByteOrder->Enabled = true;
      RBText->Enabled = true;
      RBBin->Enabled = true;
      RBFloat->Enabled = true;
      RGDataType->Enabled = true;
      Label6->Enabled = true;
      Label7->Enabled = true;
      Label8->Enabled = true;
      EHeader->Enabled = true;
      EWidth->Enabled = true;
      EHeight->Enabled = true;
      Label5->Enabled = false;
      EScriptFileName->Enabled = false;
      BFind->Enabled = false;

      if (RBText->Checked)
        {
      Label9->Enabled = false;
      Label11->Enabled = false;
      EBytesPerValue->Enabled = false;
      EOffset->Enabled = false;
      RGByteOrder->Enabled = false;
        }
      else if (RBBin->Checked)
        {
      Label9->Enabled = true;
      Label11->Enabled = true;
      EBytesPerValue->Enabled = true;
      EOffset->Enabled = true;
      RGByteOrder->Enabled = false;
        }
      else
        {
      Label9->Enabled = false;
      Label11->Enabled = false;
      EBytesPerValue->Enabled = false;
      EOffset->Enabled = false;
      RGByteOrder->Enabled = true;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormLoadSimFile::ChangeSelection(TObject *Sender)
{
  // depending on selection some options might be disabled
  EnableSelectedStuff();
}

void __fastcall TFormLoadSimFile::FormShow(TObject *Sender)
{
  // depending on selection some options might be disabled
  EnableSelectedStuff();
}
//---------------------------------------------------------------------------
void __fastcall TFormLoadSimFile::FormCreate(TObject *Sender)
{
  EHeader->Text = "0";
  EFileName->Text = "test.bin";
  EWidth->Text = "256";
  EHeight->Text = "512";
  EBytesPerValue->Text = "2";
  EOffset->Text = "127";
}
//---------------------------------------------------------------------------
void __fastcall TFormLoadSimFile::BFindClick(TObject *Sender)
{
  OpenScriptDialog->FileName = EScriptFileName->Text;

  if (OpenScriptDialog->Execute())
	 {
        EScriptFileName->Text = OpenScriptDialog->FileName;
	 }
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
void __fastcall TFormLoadSimFile::BCancelClick(TObject *Sender)
{
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormLoadSimFile::BLoadClick(TObject *Sender)
{
  if (RBSunRaster->Checked)
    FileFormat.FileFormat = 0;
  else if (RBSim->Checked)
    FileFormat.FileFormat = 1;
  else if (RBCustom->Checked)
    FileFormat.FileFormat = 2;
  FileFormat.FileName = EFileName->Text;
  FileFormat.ScriptFile = EScriptFileName->Text;
  if (RBText->Checked)
    FileFormat.FileType = 0;
  else if (RBBin->Checked)
    FileFormat.FileType = 1;
  else if (RBFloat->Checked)
    FileFormat.FileType = 2;
  FileFormat.BytesPerPixel = atoi(EBytesPerValue->Text.c_str());
  FileFormat.Offset = atof(EOffset->Text.c_str());
  FileFormat.Endian = RGByteOrder->ItemIndex;
  FileFormat.DataType = RGDataType->ItemIndex;
  FileFormat.HeaderSize = atoi(EHeader->Text.c_str());
  FileFormat.SizeX = atoi(EWidth->Text.c_str());
  FileFormat.SizeY = atoi(EHeight->Text.c_str());
  FileFormat.MinValueI = 0;
  FileFormat.MaxValueI = 0;
  FileFormat.MinValueQ = 0;
  FileFormat.MaxValueQ = 0;
  FileFormat.MinValueMag = 0;
  FileFormat.MaxValueMag = 0;
  FileFormat.MinValuePh = 0;
  FileFormat.MaxValuePh = 0;
  FileFormat.MinValuePower = 0;
  FileFormat.MaxValuePower = 0;

  ModalResult = mrOk;
}
//---------------------------------------------------------------------------