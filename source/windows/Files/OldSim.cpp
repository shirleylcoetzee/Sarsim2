//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "OldSim.h"
#include "main.h"
#include "linkedlist.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TPreviousSimForm *PreviousSimForm;
//---------------------------------------------------------------------------
__fastcall TPreviousSimForm::TPreviousSimForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPreviousSimForm::BOkClick(TObject *Sender)
{
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TPreviousSimForm::BCancelClick(TObject *Sender)
{
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TPreviousSimForm::ListBox1DblClick(TObject *Sender)
{
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TPreviousSimForm::FormShow(TObject *Sender)
{
  struct SSimulation *sim;
  char s[255];
  const char TypeDesc[2][12] = {"Raw","Compressed"};
  ListBox1->Items->Clear();
  sim=MainForm->FirstSimulation;
	 while (sim != NULL)
		{
		  sprintf(s,"%s : %s, SR %-.4f to %-.4f, A %-.4f to %-.4f, A/D: %-.4f GHz %d bits, %-.4G mV LSB value",
					 TypeDesc[sim->SimTYPE],sim->RadarName, sim->SlantStart, sim->SlantEnd,
					 sim->AzimuthStart, sim->AzimuthEnd, sim->SampleFreq*1E-9,
					 sim->A2Dbits, sim->LSBvalue*1E3);
		  ListBox1->Items->Add(s);
		  sim = sim->next;
		} // end while

  // select first Simulation by default
  ListBox1->ItemIndex=0;

}
//---------------------------------------------------------------------------
void __fastcall TPreviousSimForm::FormCloseQuery(TObject *Sender,
	bool &CanClose)
{
   int counter=0;
   bool found = FALSE;
   SimSelection=MainForm->FirstSimulation;
   while (SimSelection != NULL)
  	 {
  	   if (counter == ListBox1->ItemIndex)
	  	 {
	  		found = TRUE;
	  		break;
	     }
	   counter++;
	   SimSelection = SimSelection->next;
	 } // end while
  // just in case
  if (!found) SimSelection=MainForm->FirstSimulation;

  CanClose = TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TPreviousSimForm::BDeleteClick(TObject *Sender)
{
  if (ListBox1->ItemIndex < 0)
	 {
		Application->MessageBox("Choose a simulation first", "Error", MB_OK);
        return;
	 }
   // find a pointer to the selected simulation structure
   int counter=0;
   bool found = FALSE;
   SimSelection=MainForm->FirstSimulation;
   while (SimSelection != NULL)
  	 {
  	   if (counter == ListBox1->ItemIndex)
	  	 {
	  		found = TRUE;
	  		break;
	     }
	   counter++;
	   SimSelection = SimSelection->next;
	 } // end while
  // just in case
  if (!found) SimSelection=MainForm->FirstSimulation;

  if (SimSelection != NULL)
    {
      MainForm->IsDirty = TRUE;
      DeleteSimulation(&(MainForm->FirstSimulation),SimSelection);
      FormShow(Sender);
    }
}
//---------------------------------------------------------------------------
void __fastcall TPreviousSimForm::BitBtn1Click(TObject *Sender)
{
  Application->HelpJump("prevsim");
}
//---------------------------------------------------------------------------