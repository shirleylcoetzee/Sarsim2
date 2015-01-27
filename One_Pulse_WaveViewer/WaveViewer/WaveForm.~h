//---------------------------------------------------------------------------

#ifndef WaveFormH
#define WaveFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <jpeg.hpp>

#include <vector>
//---------------------------------------------------------------------------
class TfWave : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *Open1;
        TOpenDialog *odWave;
        TImage *iWave;
        TMemo *mCon;
        TMenuItem *Exit1;
        TGroupBox *gbData;
        TRadioButton *rbReal;
        TRadioButton *rbImag;
        TRadioButton *rbMag;
        TRadioButton *rbPhase;
        TButton *bSaveWave;
        TEdit *eImgFilename;
        TLabel *lPos;
        void __fastcall Open1Click(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall rbRealClick(TObject *Sender);
        void __fastcall rbImagClick(TObject *Sender);
        void __fastcall rbMagClick(TObject *Sender);
        void __fastcall rbPhaseClick(TObject *Sender);
        void __fastcall bSaveWaveClick(TObject *Sender);
        void __fastcall iWaveMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall iWaveMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall iWaveMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
        std::vector<int *> iqs;
        Graphics::TBitmap * offbmp;
        TPoint clickLoc;
        bool clicked;
        int __fastcall LoadFile(char * filename);
        void __fastcall p2c(AnsiString s);
        void __fastcall ParseLine(char * line, int & iVal, int & qVal);
        void __fastcall ClearMem();
        void __fastcall DrawWave(char type,int sampPerPix,int startsmp,int samples);
public:		// User declarations
        __fastcall TfWave(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfWave *fWave;
//---------------------------------------------------------------------------
#endif
