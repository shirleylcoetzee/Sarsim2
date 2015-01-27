//---------------------------------------------------------------------------
#ifndef SaveProgressH
#define SaveProgressH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Buttons.hpp>
#include "waitforthread.h"
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TSaveProgressForm : public TForm
{
__published:	// IDE-managed Components
	TProgressBar *ProgressBar1;
	TLabel *Label1;
	TBitBtn *BAbort;
	TTimer *Timer1;
	void __fastcall BAbortClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
    void __fastcall ThreadDone(TObject * );
public:		// User declarations
    TWaitThread *ClickCancelThread;
	__fastcall TSaveProgressForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TSaveProgressForm *SaveProgressForm;
//---------------------------------------------------------------------------
#endif
