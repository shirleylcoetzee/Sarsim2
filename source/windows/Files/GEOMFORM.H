//---------------------------------------------------------------------------
#ifndef geomformH
#define geomformH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Menus.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include "objectstructdef.h"
#include <vcl\Dialogs.hpp>
//---------------------------------------------------------------------------
class TGeoForm : public TForm
{
__published:	// IDE-managed Components
  TMainMenu *MainMenu1;
  TMenuItem *Options1;
  TBevel *Bevel1;
  TMemo *GeoList;
  TGroupBox *GBRadar;
  TComboBox *RadarList;
  TGroupBox *GroupBox3;
  TLabel *Label18;
  TEdit *EFilename;
  TBitBtn *BSelectFile;
  TRadioGroup *RGType;
  TRadioGroup *RGScript;
  TBitBtn *BSave;
  TBitBtn *BClose;
  TLabel *Label7;
  TGroupBox *GBWindow;
  TLabel *XStart;
  TLabel *XEnd;
  TLabel *YStart;
  TLabel *YEnd;
  TEdit *EXStart;
  TEdit *EXEnd;
  TEdit *EYStart;
  TEdit *EYEnd;
  TSaveDialog *SaveDataDialog;
  TListBox *ListBox1;
  TLabel *Label1;
  TBitBtn *BShow;
  TBitBtn *BDelete;
  TBitBtn *BHelp;
  void __fastcall Options1Click(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall RadarListChange(TObject *Sender);
  void __fastcall BSelectFileClick(TObject *Sender);
  void __fastcall BCloseClick(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall EXStartExit(TObject *Sender);
  void __fastcall EnterKeyPress(TObject *Sender, char &Key);
  void __fastcall EXEndExit(TObject *Sender);
  void __fastcall EYStartExit(TObject *Sender);
  void __fastcall EYEndExit(TObject *Sender);
  void __fastcall RGTypeClick(TObject *Sender);
  void __fastcall BSaveClick(TObject *Sender);
  void __fastcall EFilenameKeyPress(TObject *Sender, char &Key);
  void __fastcall EFilenameExit(TObject *Sender);
  void __fastcall BDeleteClick(TObject *Sender);
  void __fastcall BShowClick(TObject *Sender);
  void __fastcall ListBox1DblClick(TObject *Sender);
  
  void __fastcall BHelpClick(TObject *Sender);
private:	// User declarations
    // current radar
    struct SRadar *CRadar;
    void FindCurrentRadar();
    void UpdateList();
    void UpdateDialog();
    void SaveGeometry(bool Temp);
    void FindSelectedGeo();
    struct SGeometry *GeoSelection;

public:		// User declarations
    void virtual __fastcall RestrictSize(TMessage& Msg);
    void virtual __fastcall ClickScrollBar(TMessage& Msg);

    struct SGeometry CurrentGeo;
  __fastcall TGeoForm(TComponent* Owner);
    // This message map maps the WM_GETMINMAXINFO message to the
    // RestrictSize function.
    BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER(WM_GETMINMAXINFO,TMessage,RestrictSize)
      MESSAGE_HANDLER(EN_HSCROLL,TMessage,ClickScrollBar)
      // Add any number of additional message handlers here.
    END_MESSAGE_MAP(TForm)

};
//---------------------------------------------------------------------------
extern TGeoForm *GeoForm;
//---------------------------------------------------------------------------
#endif
