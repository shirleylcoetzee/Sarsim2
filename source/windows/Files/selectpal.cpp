//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "selectpal.h"
#include "quikgeneral.h"
#include "mainquikview.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TLoadPalForm *LoadPalForm;
//---------------------------------------------------------------------------
__fastcall TLoadPalForm::TLoadPalForm(TComponent* Owner)
	: TForm(Owner)
{
  // set some initial palette
  Pal = RED_BLUE_SCALE;
  PalBM = new class TBitM(Handle, 200, 18);//, BMPal);
}
//---------------------------------------------------------------------------
void __fastcall TLoadPalForm::BLoadPalClick(TObject *Sender)
{
  /*
  unsigned char Red, Green, Blue,Dummy;
  int i,j;
  FILE *InFilep;
  int Code;
  char str[255];
  if(OpenPalDialog->Execute())
    {
       InFilep = fopen(OpenPalDialog->FileName.c_str(), "rt");  // read from text file
       if (!InFilep)
         {
           char str[100];
           sprintf(str,"Cannot open file '%s'",OpenPalDialog->FileName.c_str());
           Application->MessageBox(str, "Error", MB_OK | MB_ICONEXCLAMATION);
         }
       else
         {
           for (i=0;i<256;i++)
             {
                for (j=0;j<3;j++)
                  {
                    fscanf(InFilep, "%d", &Dummy);
                    if (j==0) ColPal[i][BLUE] = Dummy*4;
                    if (j==1) ColPal[i][GREEN] = Dummy*4;
                    if (j==2) ColPal[i][RED] = Dummy*4;
                  }
              }
           fclose(InFilep);

           MakeNewPalette(BMPal,ColPal);

           DrawPal();
           RBCustom->Checked = TRUE;
         }
    }
 */
}
//---------------------------------------------------------------------------
void __fastcall TLoadPalForm::BOkClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TLoadPalForm::DrawPal()
{
 /*
  int SizeX = Image1->Width,
      SizeY = Image1->Height;
  Graphics::TBitmap *BM;
  unsigned long Off = 0x2000000;
  unsigned long Col;
  int i,j;

  BM = new Graphics::TBitmap();
  BM->Width = SizeX;
  BM->Height = SizeY;
  BM->IgnorePalette = FALSE;
  BM->Palette = CreatePalette(BMPal);

  for (i=0;i<SizeX;i++)
    {
      j = (double(i)/double(SizeX))*double(256);
      Col = Off + ColPal[j][BLUE] + 256*ColPal[j][GREEN] + 65536*ColPal[j][RED];
      BM->Canvas->Pen->Color = (TColor)Col;
      BM->Canvas->LineTo(i-1, -1);
      BM->Canvas->MoveTo(i-1, SizeY);
      BM->Canvas->LineTo(i, -1);
      BM->Canvas->MoveTo(i, SizeY);
      BM->Canvas->LineTo(i+1, -1);
      BM->Canvas->MoveTo(i+1, SizeY);
    }

  Image1->Picture = (Graphics::TPicture*)BM;
  BM->ReleasePalette();
  delete BM;
 */
}
//---------------------------------------------------------------------------
void __fastcall TLoadPalForm::FormShow(TObject *Sender)
{
  Pal = MainViewForm->ColorPal;
  if (Pal == GRAY_SCALE)
    {
      RBGray->Checked = true;
    }
  else if (Pal == RED_BLUE_SCALE)
    {
      RBRedBlue->Checked = true;
    }
  else if (Pal == RAINBOW_SCALE)
    {
      RBRainbow->Checked = true;
    }
  else if (Pal == USERDEFINED_SCALE)
    {
      RBCustom->Checked = true;
    }
  MakePalette(Pal,127,127,127);
  SetupPaletteBitmap();
}
//---------------------------------------------------------------------------
void __fastcall TLoadPalForm::FormDestroy(TObject *Sender)
{
  delete PalBM;
}
//---------------------------------------------------------------------------
void __fastcall TLoadPalForm::FormPaint(TObject *Sender)
{
  // palette
  PalBM->Draw(Canvas,Handle,Bevel1->Left,Bevel1->Top, Cpal);
}
//---------------------------------------------------------------------------
void TLoadPalForm::SetupPaletteBitmap()
{
  // setup palette
  unsigned char Col;
  int i,j;
  for (i=0;i<PalBM->DIBHeight;i++)
    for (j=0;j<PalBM->DIBWidth;j++)
      {
        Col = double(j)/double(PalBM->DIBWidth) * double(NCOLOR);
        PalBM->pDIBBase[i*3*PalBM->DIBWidth + j*3] = ColPal[Col][RED];
        PalBM->pDIBBase[i*3*PalBM->DIBWidth + j*3+1] = ColPal[Col][GREEN];
        PalBM->pDIBBase[i*3*PalBM->DIBWidth + j*3+2] = ColPal[Col][BLUE];
      }
}
//---------------------------------------------------------------------------
void TLoadPalForm::MakePalette(unsigned char Type,int Bright,
                            int Contrast, int Sat)
{
  // create default lookup table
  CreateRGBPal(ColPal, Bright, Contrast, Sat, Type);
  // current logical palette for bitmap
  LOGPALETTE* BMPal;
  if (!CreateLogPal(&BMPal, ColPal))
    Application->MessageBox("Cannot create palette", "Error", MB_OK | MB_ICONEXCLAMATION);
  Cpal = CreatePalette(BMPal);
}
//---------------------------------------------------------------------------
void __fastcall TLoadPalForm::RBGrayClick(TObject *Sender)
{
  Pal = GRAY_SCALE;
  MakePalette(Pal,127,127,127);
  SetupPaletteBitmap();
  RealizeMyPalette(false);
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TLoadPalForm::RBRedBlueClick(TObject *Sender)
{
  Pal = RED_BLUE_SCALE;
  MakePalette(Pal,127,127,127);
  SetupPaletteBitmap();
  RealizeMyPalette(false);
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TLoadPalForm::RBRainbowClick(TObject *Sender)
{
  Pal = RAINBOW_SCALE;
  MakePalette(Pal,127,127,127);
  SetupPaletteBitmap();
  RealizeMyPalette(false);
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TLoadPalForm::RBCustomClick(TObject *Sender)
{
  Pal = USERDEFINED_SCALE;
  MakePalette(Pal,127,127,127);
  SetupPaletteBitmap();
  RealizeMyPalette(false);
  Invalidate();
}
//---------------------------------------------------------------------------
// ForceBackGround should be true on WM_PALETTE_CHANGED
// and false on WM_QUERYNEWPALETTE
//---------------------------------------------------------------------------
int TLoadPalForm::RealizeMyPalette(bool ForceBackGround)
{
  // realize current palette
  HDC hdc = GetDC(Handle);
  HPALETTE hOldPal = SelectPalette(hdc, Cpal, ForceBackGround);
  // if some other window fiddeled with palette repaint is necessary
  // (nChanged != 0)
  int nChanged = RealizePalette(hdc);
  SelectPalette(hdc, hOldPal, false);
  ReleaseDC(Handle, hdc);
  return nChanged;
}
void __fastcall TLoadPalForm::BitBtn1Click(TObject *Sender)
{
  Application->HelpJump("simtime");
}
//---------------------------------------------------------------------------
