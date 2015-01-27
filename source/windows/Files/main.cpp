//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <vcl\registry.hpp>
#pragma hdrstop

#include "main.h"
#include "targetform.h"
#include "graphform.h"
#include "graph.h"
#include "SimuForm.h"
#include "linkedlist.h"
#include "platformform.h"
#include "radarform.h"
#include "general.h"
#include "engine.h"
#include "linkedlist.h"
#include <math.h>
#include "focus.h"
#include "parser.h"
#include "loadsavedata.h"
#include "OldSim.h"
#include "about.h"
#include "showscript.h"
#include "mainquikview.h"
#include "surfaceform.h"
#include "import.h"
#include "geomform.h"
#include "logo.h"


//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------

// this header file contains a few global variables and some initializations
#include "globalvariables.h"
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
  SetupAllVariables();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::NewTargetClick(TObject *Sender)
{
  // make a copy of the default target to TargetForm->Target
  CopyTarget(&(TargetF->Target),&DefaultTargetValues);
  TargetF->ShowModal();
  // add target
  if (TargetF->ModalResult == mrOk)
    {
      AddTarget(&(TargetF->Target), &FirstTarget);
      ChangedObject();
    }
  // don't want a new target
  // free memory allocated by copying Target
  else if (TargetF->ModalResult == mrCancel)
    FreeTargetDataDef(&(TargetF->Target));
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::NewSurface1Click(TObject *Sender)
{

  // make a copy of the default Surface to SurfaceForm->Surface
  CopySurface(&(SurfaceF->Surface),&DefaultSurfaceValues);
  SurfaceF->ShowModal();
  // add Surface
  if (SurfaceF->ModalResult == mrOk)
    {
      AddSurface(&(SurfaceF->Surface), &FirstSurface);
      IsDirty = TRUE;
      UpdateObjectList();
      CalcNewSTimeArray();
      Invalidate();
    }
  // don't want a new Surface
  // free memory allocated by copying Surface
  else if (SurfaceF->ModalResult == mrCancel)
    FreeSurfaceDataDef(&(SurfaceF->Surface));
    
}
//---------------------------------------------------------------------------
void TMainForm::ModifySurface(struct SSurface *s)
{
  // make a copy of the selected Surface to SurfaceForm->Surface
  CopySurface(&(SurfaceF->Surface),s);
  SurfaceF->ShowModal();
  // add Surface
  if (SurfaceF->ModalResult == mrOk)
    {
      AddSurface(&(SurfaceF->Surface), &FirstSurface);
      DeleteSurface(&FirstSurface, s);
      ChangedObject();
    }
  // don't want a new Surface
  // free memory allocated by copying Surface
  else if (SurfaceF->ModalResult == mrCancel)
    FreeSurfaceDataDef(&(SurfaceF->Surface));
}
//---------------------------------------------------------------------------
void TMainForm::ChangedObject(void)
{
  IsDirty = TRUE;
  CalcNewSTimeArray();
  UpdateObjectList();
  if (ShowScriptForm->Visible) FillMemo(ShowScriptForm->MScript);
  Invalidate();
}
//---------------------------------------------------------------------------
void TMainForm::DeleteSurfaceMain(struct SSurface *s)
{
  DeleteSurface(&FirstSurface,s);
  ChangedObject();
}
//---------------------------------------------------------------------------
void TMainForm::ModifyTarget(struct STarget *t)
{
  // make a copy of the selected target to TargetForm->Target
  CopyTarget(&(TargetF->Target),t);
  TargetF->ShowModal();
  // add target
  if (TargetF->ModalResult == mrOk)
    {
      AddTarget(&(TargetF->Target), &FirstTarget);
      DeleteTarget(&FirstTarget, t);
      ChangedObject();
    }
  // don't want a new target
  // free memory allocated by copying Target
  else if (TargetF->ModalResult == mrCancel)
    FreeTargetDataDef(&(TargetF->Target));
}
//---------------------------------------------------------------------------
void TMainForm::DeleteTargetMain(struct STarget *t)
{
  DeleteTarget(&FirstTarget,t);
  ChangedObject();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::NewPlatformClick(TObject *Sender)
{
  // make a copy of the default Platform to PlatformForm->Platform
  CopyPlatform(&(PlatformF->Platform),&DefaultPlatformValues);
  PlatformF->Modify_Old = false; // we don't want to modify an old platform
  PlatformF->ShowModal();
  if (PlatformF->ModalResult == mrOk)
    {
      // add Platform
      AddPlatform(&(PlatformF->Platform), &FirstPlatform);
      ChangedObject();
    }
  else if (PlatformF->ModalResult == mrCancel)
    {
      // don't want a new Platform
      // free memory allocated by copying Platform
      FreePlatformDataDef(&(PlatformF->Platform));
    }
}
//---------------------------------------------------------------------------
void TMainForm::ModifyPlatform(struct SPlatform *p)
{
  // make a copy of the selected Platform to PlatformForm->Platform
  CopyPlatform(&(PlatformF->Platform),p);
  PlatformF->Modify_Old = true; // we do want to modify an old platform
  PlatformF->ShowModal();
  // add Platform
  if (PlatformF->ModalResult == mrOk)
    {
      AddPlatform(&(PlatformF->Platform), &FirstPlatform);
      DeletePlatform(&FirstPlatform, p, &FirstTarget, &FirstRadar, &FirstSurface,FALSE);
      ChangedObject();
    }
  // don't want a new Platform
  // free memory allocated by copying Platform
  else if (PlatformF->ModalResult == mrCancel)
    FreePlatformDataDef(&(PlatformF->Platform));
}
//---------------------------------------------------------------------------
void TMainForm::DeletePlatformMain(struct SPlatform *p)
{
  DeletePlatform(&FirstPlatform, p, &FirstTarget, &FirstRadar, &FirstSurface,TRUE);
  ChangedObject();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::NewRadarClick(TObject *Sender)
{
  // make a copy of the default Radar to RadarForm->Radar
  CopyRadar(&(RadarF->Radar),&DefaultRadarValues);
  // select first page as active page
  RadarF->PageControl1->ActivePage = RadarF->TabSheet1;
  RadarF->ShowModal();
  if (RadarF->ModalResult == mrOk)
    {
      // add Radar
      AddRadar(&(RadarF->Radar), &FirstRadar);
      ChangedObject();
    }
  else if (RadarF->ModalResult == mrCancel)
    {
      // don't want a new Radar
      // free memory allocated by copying Radar
      FreeRadarDataDef(&(RadarF->Radar));
    }
}
//---------------------------------------------------------------------------
void TMainForm::ModifyRadar(struct SRadar *r)
{
  // make a copy of the selected Radar to RadarForm->Radar
  CopyRadar(&(RadarF->Radar),r);
  // select first page as active page
  RadarF->PageControl1->ActivePage = RadarF->TabSheet1;
  RadarF->ShowModal();
  // add Radar
  if (RadarF->ModalResult == mrOk)
    {
      AddRadar(&(RadarF->Radar), &FirstRadar);
      DeleteRadar(&FirstRadar, r);
      ChangedObject();
    }
  // don't want a new Radar
  // free memory allocated by copying Radar
  else if (RadarF->ModalResult == mrCancel)
    FreeRadarDataDef(&(RadarF->Radar));
}
//---------------------------------------------------------------------------
void TMainForm::DeleteRadarMain(struct SRadar *r)
{
  DeleteRadar(&FirstRadar,r);
  ChangedObject();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RawReturn1Click(TObject *Sender)
{
  NewSimulation(RAW_RETURN,&DefaultSimulationValues);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RangeCompressed1Click(TObject *Sender)
{
  NewSimulation(MATCHED_FILTER,&DefaultSimulationValues);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::NewSimulation(int Type, struct SSimulation *Sim)
{
//  int Type = RAW_RETURN;
  int i;
  char mess[80];

  // only run simulation if targets exists
  if ((FirstTarget == NULL) && (FirstSurface == NULL))
    Application->MessageBox("No reflecting objects entered...", "Message", MB_OK);
  else if (FirstRadar == NULL)
    Application->MessageBox("Create a Radar first...", "Message", MB_OK);
  else
    {
      // check through all available 'slots'
      for (i=0;i<MAX_GRAPHS;i++)
        {
          if (!(SimuGraphActive[i]))
            {
              // create new graph window and show it
              SimuGraphs[i] = new TSimulationForm(Application);
              // inform Form which 'slot' it has, so it can tell this
              // form later if it is closed
              SimuGraphs[i]->Count = i;
              // make a copy of the default Simulation to SimuGraphs[i]->CurrentSim
              CopySimulation(&(SimuGraphs[i]->CurrentSim),Sim);
              SimuGraphs[i]->CurrentSim.SimTYPE = Type;
              SimuGraphs[i]->FinishedInitializing = FALSE;
              SimuGraphs[i]->Show();
              return;
            }
        }  // end for i
      sprintf(mess,"Only %d graphs may be open at a time",MAX_GRAPHS);
      Application->MessageBox(mess,"Error",MB_OK);
    }
}
//---------------------------------------------------------------------------
void TMainForm::UpdateObjectList()
{
  STarget *t;
  SPlatform *p;
  SRadar *r;
  SSurface *s;
  char str[100];
  int Selection;
  Selection = ObjectBox->ItemIndex;

  ObjectBox->Clear();
  // add all objects to list
  p=FirstPlatform;
  while (p != NULL)
	 {
		sprintf(str,"Platform:%s",p->Name);
		ObjectBox->Items->Add(str);

		r=FirstRadar;
		while (r != NULL)
		 {
			if (stricmp(p->Name,r->PlatformName) == 0)
			  {
				 // radar with given platform name found
				 sprintf(str,"  Radar:%s",r->RadarName);
				 ObjectBox->Items->Add(str);
			  }
			r = r->next;
		 }

		t=FirstTarget;
		while (t != NULL)
		  {
			 if (stricmp(p->Name,t->Name) == 0)
				{
				  sprintf(str,"  PT:%-.6G,%-.6G,%-.6G",t->Pos[0],t->Pos[1],t->Pos[2]);
				  ObjectBox->Items->Add(str);
				}
			 t = t->next;
		  }
		s=FirstSurface;
		while (s != NULL)
		  {
			 if (stricmp(p->Name,s->Name) == 0)
				{
				  sprintf(str,"  S:(%-.6G,%-.6G,%-.6G)(%-.6G,%-.6G,%-.6G)(%-.6G,%-.6G,%-.6G)",
            s->Tri[0][0],s->Tri[0][1],s->Tri[0][2],
            s->Tri[1][0],s->Tri[1][1],s->Tri[1][2],
            s->Tri[2][0],s->Tri[2][1],s->Tri[2][2]);
				  ObjectBox->Items->Add(str);
				}
			 s = s->next;
		  }

		p = p->next;                     // goto next platform
	 }
  // restore previous selection (if possible)
  if (ObjectBox->Items->Count > Selection)
	 ObjectBox->ItemIndex = Selection;
  else
	 ObjectBox->ItemIndex = ObjectBox->Items->Count;
}
//---------------------------------------------------------------------------
bool TMainForm::FindObjectBoxSel(int *SelectionType, void **Object)
{
  int CurrentItemNumber,Selection;
  bool Found=FALSE;
  STarget *t;
  SPlatform *p;
  SRadar *r;
  SSurface *s;

  Selection = ObjectBox->ItemIndex;
  CurrentItemNumber = -1;

  // figure out what was selected....
  p=FirstPlatform;
  while (p != NULL)
	 {
		CurrentItemNumber++;
		if (CurrentItemNumber == Selection)
		  { *SelectionType = 0; Found=TRUE; *Object = p;break; }
		r=FirstRadar;
		while (r != NULL)
		 {
			if (stricmp(p->Name,r->PlatformName) == 0)
			  {
				 CurrentItemNumber++;
				 if (CurrentItemNumber == Selection)
					{ *SelectionType = 1; Found=TRUE; *Object = r;break; }
			  }
			r = r->next;
		 }
		if (Found) break;
		t=FirstTarget;
		while (t != NULL)
		  {
			 if (stricmp(p->Name,t->Name) == 0)
				{
				  CurrentItemNumber++;
				  if (CurrentItemNumber == Selection)
					{ *SelectionType = 2; Found=TRUE; *Object = t;break; }
				}
			 t = t->next;
		  }
		s=FirstSurface;
		while (s != NULL)
		  {
			 if (stricmp(p->Name,s->Name) == 0)
				{
				  CurrentItemNumber++;
				  if (CurrentItemNumber == Selection)
					{ *SelectionType = 3; Found=TRUE; *Object = s;break; }
				}
			 s = s->next;
		  }
		if (Found) break;
		p = p->next;                     // goto next platform
	 } // end while
  return Found;

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ObjectBoxDblClick(TObject *Sender)
{
  int SelectionType;
  void *Object;
  // what was selected ?
  if(FindObjectBoxSel(&SelectionType,&Object))
	 {
        // and modify it
  		switch(SelectionType) {
		  case 0 : // Platform selected
			 {
                ModifyPlatform((struct SPlatform*)Object);
				break;
			 }
		  case 1 : // Radar selected
			 {
                ModifyRadar((struct SRadar*)Object);
				break;
			 }
		  case 2 : // Target selected
			 {
                ModifyTarget((struct STarget*)Object);
				break;
			 }
		  case 3 : // Surface selected
			 {
                ModifySurface((struct SSurface*)Object);
				break;
			 }
		} // end switch
		UpdateObjectList();
		Invalidate();
	 } // end if
}
//---------------------------------------------------------------------------
void TMainForm::CalcNewSTimeArray()
{
  // simulation time STime has changed : update arrays
  long PFNo,TargetNo,
	   PlatformNo,TNo;
  STarget *t;                 // general pointer to target
  SPlatform *p;               // general pointer to platform
  long FocusPFNo;             // focus platform number

  // find focus coordinates
  if (FocusPlatform != NULL)
	 {
		FocusPFNo = FindPlatformCount(FirstPlatform,FocusPlatform);
		if (FocusPFNo >= 0)
		  {
			 Focus[0] = PlatformPos[FocusPFNo][0];
			 Focus[1] = PlatformPos[FocusPFNo][1];
			 Focus[2] = PlatformPos[FocusPFNo][2];
		  }
	 }

  // total number of platforms
  PlatformNo = CountPlatforms(FirstPlatform);
  // total number of targets
  TargetNo = CountTargets(FirstTarget);
  // find the center of the window

  // free previous memory allocated
  Free_DMatrix(PlatformPos,0,0);
  Free_DMatrix(PlatformVel,0,0);
  Free_DMatrix(PlatformRot,0,0);
  Free_DMatrix(TargetPos,0,0);

  // create new arrays...
  // position, velocity and rotation of each platform for each pulse
  PlatformPos = DMatrix(0,PlatformNo-1,0,2);
  PlatformVel = DMatrix(0,PlatformNo-1,0,2);
  PlatformRot = DMatrix(0,PlatformNo-1,0,2);
  TargetPos = DMatrix(0,TargetNo-1,0,2);

  for (TNo = 0;TNo<TargetNo;TNo++)      // for all target
	 {
		// find the target
		t = PointToTarget(FirstTarget, TNo+1);
		FindTargetPosition(t, TargetPos[TNo]);
	 }

  // find platform position and velocities for time STime
  for (PFNo = 0;PFNo<PlatformNo;PFNo++)      // for all platforms
	 {
		// find the platform
		p = PointToPlatform(FirstPlatform, PFNo+1);
		FindPlatformPosition(p,STime,PlatformPos[PFNo]);
		FindPlatformVelocity(p,STime,PlatformVel[PFNo]);
		FindPlatformRotation(p,STime,PlatformRot[PFNo]);
	 }
}
//---------------------------------------------------------------------------
void TMainForm::DrawLittleCoordSystem()
{
  // now create main coordinate system
  long i,j;
  double C[7][3],RotatedC[7][3];
  double AxisL = 30/ZoomX;
  double F[3];

  F[0]=0; F[1]=0; F[2]=0; //focus stays at center

  for (i=0;i<3;i++)
    for (j=0;j<7;j++)
      C[j][i] = 0;

  C[0][0] = AxisL;
  C[1][1] = AxisL;
  C[2][2] = AxisL;
  C[4][0] = AxisL*1.2;
  C[5][1] = AxisL*1.2;
  C[6][2] = AxisL*1.2;          //ObjectBox->Width

  RotatePoints3(C, RotatedC, 7,  ClientWidth - 50, Height - ClientHeight + 30,
   ZoomX, ZoomY, Rot,Pos, F);
  if (ShowLittleCoordSystemAxes)
	 {
        Canvas->Pen->Style = psSolid;
  //      Canvas->Pen->Width = 1;
        Canvas->Pen->Color = clRed;
     	Canvas->MoveTo(RotatedC[3][2],RotatedC[3][1]);
        Canvas->LineTo(RotatedC[0][2],RotatedC[0][1]);
        Canvas->Pen->Color = clGreen;
		Canvas->MoveTo(RotatedC[3][2],RotatedC[3][1]);
		Canvas->LineTo(RotatedC[1][2],RotatedC[1][1]);
        Canvas->Pen->Color = clBlue;
		Canvas->MoveTo(RotatedC[3][2],RotatedC[3][1]);
		Canvas->LineTo(RotatedC[2][2],RotatedC[2][1]);

        Canvas->Pen->Color = clBlack;
        Canvas->Brush->Color = clSilver;
        Canvas->Pen->Width = 1;
        Canvas->Font->Color = clBlack;
        Canvas->Brush->Style = bsClear;
		Canvas->Font->Size = 11;
//        Canvas->Font->Style = Canvas->Font->Style << fsBold;//fsBold;        Set
       	Canvas->TextOut(RotatedC[4][2]-4,RotatedC[4][1]-7,"X");
		Canvas->TextOut(RotatedC[5][2]-4,RotatedC[5][1]-7,"Y");
		Canvas->TextOut(RotatedC[6][2]-4,RotatedC[6][1]-7,"Z");
 //       Canvas->Font->Style = Canvas->Font->Style >> fsBold;//fsBold;        Set
	 }
  Canvas->Font->Size = 8;
  Canvas->Pen->Width = 1;
}
//---------------------------------------------------------------------------
void TMainForm::DrawCoordSystem()
{
  // now create main coordinate system
  long i,j;
  double C[10][3],RotatedC[10][3];
  char Name[10];
  double AxisL = 200/ZoomX;
  double Unit=1000;
  strcpy(Name,"1km");

  for (i=0;i<3;i++)
    for (j=0;j<10;j++)
      C[j][i] = 0;

  C[0][0] = AxisL;
  C[1][1] = AxisL;
  C[2][2] = AxisL;
  if ((10) < (0.9*AxisL)) { Unit=10; strcpy(Name,"10m");}
  if ((50) < (0.9*AxisL)) { Unit=50; strcpy(Name,"50m");}
  if ((100) < (0.9*AxisL)) { Unit=100; strcpy(Name,"100m");}
  if ((500) < (0.9*AxisL)) { Unit=500; strcpy(Name,"500m");}
  if ((1000) < (0.9*AxisL)) { Unit=1000; strcpy(Name,"1km");}
  if ((5000) < (0.9*AxisL)) { Unit=5000; strcpy(Name,"5km");}
  if ((10000) < (0.9*AxisL)) { Unit=10000; strcpy(Name,"10km");}
  if ((50000) < (0.9*AxisL)) { Unit=50000; strcpy(Name,"50km");}
  if ((100000) < (0.9*AxisL)) { Unit=100000; strcpy(Name,"100km");}
  C[4][0] = Unit;
  C[5][1] = Unit;
  C[6][2] = Unit;
  C[7][0] = AxisL*1.07;
  C[8][1] = AxisL*1.07;
  C[9][2] = AxisL*1.07;

  RotatePoints3(C, RotatedC, 10, OffsetX, OffsetY, ZoomX, ZoomY, Rot,Pos, Focus);
  if (ShowCoordSystemAxes)
	 {
        Canvas->Pen->Style = psSolid;
        Canvas->Pen->Width = 2;
        Canvas->Pen->Color = clRed;
     	Canvas->MoveTo(RotatedC[3][2],RotatedC[3][1]);
        Canvas->LineTo(RotatedC[0][2],RotatedC[0][1]);
        Canvas->Pen->Color = clGreen;
		Canvas->MoveTo(RotatedC[3][2],RotatedC[3][1]);
		Canvas->LineTo(RotatedC[1][2],RotatedC[1][1]);
        Canvas->Pen->Color = clBlue;
		Canvas->MoveTo(RotatedC[3][2],RotatedC[3][1]);
		Canvas->LineTo(RotatedC[2][2],RotatedC[2][1]);

        Canvas->Pen->Color = clBlack;
        Canvas->Brush->Color = clSilver;
        Canvas->Pen->Width = 1;

		Canvas->Ellipse(RotatedC[4][2]-2,RotatedC[4][1]-2,RotatedC[4][2]+3,RotatedC[4][1]+3);
		Canvas->Ellipse(RotatedC[5][2]-2,RotatedC[5][1]-2,RotatedC[5][2]+3,RotatedC[5][1]+3);
		Canvas->Ellipse(RotatedC[6][2]-2,RotatedC[6][1]-2,RotatedC[6][2]+3,RotatedC[6][1]+3);
        Canvas->Font->Color = clBlack;
        //Canvas->Font->Style << ((TFontStyles)fsUnderline);//fsBold;        Set
        Canvas->Brush->Style = bsClear;
	   	Canvas->TextOut(RotatedC[4][2]-14,RotatedC[4][1]+2,Name);
		Canvas->TextOut(RotatedC[5][2]-14,RotatedC[5][1]+2,Name);
		Canvas->TextOut(RotatedC[6][2]-14,RotatedC[6][1]+2,Name);
		Canvas->Font->Size = 11;
        Canvas->Font->Style = Canvas->Font->Style << fsBold;//fsBold;        Set
       	Canvas->TextOut(RotatedC[7][2]-4,RotatedC[7][1]-7,"X");
		Canvas->TextOut(RotatedC[8][2]-4,RotatedC[8][1]-7,"Y");
		Canvas->TextOut(RotatedC[9][2]-4,RotatedC[9][1]-7,"Z");
        Canvas->Font->Style = Canvas->Font->Style >> fsBold;//fsBold;        Set
	 }
  Canvas->Font->Size = 8;
  Canvas->Pen->Width = 1;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
  if ((Button == mbLeft) && (!LButtonDown))
	{
      LButtonDown = TRUE;
	  // capture mouse
	  SetCapture(Handle);  // Direct all subsequent mouse input to this window
      // store reference point
	  LeftBClickPoint.x = -(ROT_SPEED * Rot[1]*RadToDeg) + X;
	  LeftBClickPoint.y = ((ROT_SPEED/2) * Rot[2]*RadToDeg) + Y;
    }
  else if ((Button == mbRight) && (!RButtonDown))
    {
      // check if clicked an object
      bool Found = FALSE;
      struct STarget *t;
      long PFN,i;
      struct SPlatform *p;
      struct SRadar *r;
      int ClickDist = 6;
      // T contains target position, RotatedT the position projected onto the XY plane
      double T[1][3],RotatedT[1][3];
      t=FirstTarget;
      while (t != NULL && !Found)
	    {
		  // find pointer to target point platform
		  FindPlatform(t->Name, &p, &PFN, FirstPlatform);
		  // find target position
		  Copy3DPoint(T[0],TargetPos[FindTargetNo(FirstTarget,t)]);
		  // rotate point
		  RotateNPoints(T, RotatedT, 1, PlatformRot[PFN]);
		  // add relative point target position to platform position
		  for (i=0;i<3;i++)
		    T[0][i] = RotatedT[0][i] + PlatformPos[PFN][i];
		  // now rotate to get screen coordinates
		  RotatePoints3(T, RotatedT, 1, OffsetX, OffsetY, ZoomX, ZoomY, Rot, Pos,
						 Focus);
		  // clicked close enough ?
		  if (Distance(RotatedT[0][2],RotatedT[0][1],X,Y) <= ClickDist)
		    {
			   Found = TRUE;
               SelectedTarget = t;
			   TargetPopupMenu->Popup(X+Left, Y+Top);
		    }
		  t = t->next;
	    }

      r=FirstRadar;
      while (r != NULL && !Found)
	    {
		   // find pointer to radar platform
		   FindPlatform(r->PlatformName, &p, &PFN, FirstPlatform);
		   Copy3DPoint(T[0],PlatformPos[PFN]);
		   // now rotate to get screen coordinates
		   RotatePoints3(T, RotatedT, 1, OffsetX, OffsetY, ZoomX, ZoomY, Rot,
							  Pos, Focus);
		   // clicked close enough ?
		   if (Distance(RotatedT[0][2],RotatedT[0][1],X,Y) <= ClickDist)
		     {
			   Found = TRUE;
			   SelectedRadar = r;
        	   RadarPopupMenu->Popup(X+Left, Y+Top);
		     }
		   r = r->next;
	    }

    if (!Found)
        {
          ZoomXOld = ZoomX;
	      ZoomYOld = ZoomY;
	      RButtonDown = TRUE;
	  // capture mouse
	  SetCapture(Handle);  // Direct all subsequent mouse input to this window
          ZoomMode = TRUE;
	      RightBClickPoint.x = X;
	      RightBClickPoint.y = Y;
	      Invalidate();
        }
    } // end else if

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
  // rotate coord system
  if (LButtonDown)
	{
  	  Rot[1] = ((X - LeftBClickPoint.x)/ROT_SPEED)*DegToRad;
	  Rot[2] = ((LeftBClickPoint.y - Y)/(ROT_SPEED*0.5))*DegToRad;
      // check that elevation angle stays between -90 and +90 degrees
	  if (Rot[2] > 90*DegToRad)
	    {
	   	  Rot[2]=90*DegToRad;
		  LeftBClickPoint.y = ((ROT_SPEED*0.5) * Rot[2]*RadToDeg) + Y;
		}
	  if (Rot[2] < -90*DegToRad)
	    {
	      Rot[2]=-90*DegToRad;
		  LeftBClickPoint.y = ((ROT_SPEED*0.5) * Rot[2]*RadToDeg) + Y;
		}
      Invalidate();
    }
  // zoom in or out
  if (RButtonDown && ZoomMode)
	 {
		ZoomX = ZoomXOld * pow(10,((float)RightBClickPoint.y - (float)Y)/200.0);
		ZoomY = ZoomX;
 		Invalidate();
	 }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
  if ((Button == mbLeft) && LButtonDown)
	{
      LButtonDown = FALSE;
      ReleaseCapture();
      Invalidate();
    }
  else if ((Button == mbRight) && RButtonDown)
    {
	  RButtonDown = FALSE;
      ReleaseCapture();
      ZoomMode = FALSE;
	  RightBClickPoint.x = X;
	  RightBClickPoint.y = Y;
	  Invalidate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormResize(TObject *Sender)
{
  SpeedButton4->Left = Panel1->Left + Panel1->Width - SpeedButton4->Width - 8; 
  ObjectBox->Height = Panel2->Height - (Label1->Top + Label1->Height + 3); //136;
  CalcCoordSystemOffset();
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RestrictSize(TMessage& Msg)
{
  // set the limits of the graph windows size
  ((POINT far *)Msg.LParam)[3].x = 510;
  ((POINT far *)Msg.LParam)[3].y = 325;
  ((POINT far *)Msg.LParam)[4].x = 2000;
  ((POINT far *)Msg.LParam)[4].y = 2000;
  TForm::Dispatch(&Msg);
}
void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
  // clear all stuff
  FreeAllVariables();
}
//---------------------------------------------------------------------------
void TMainForm::CalcCoordSystemOffset()
{
  // find the center of the window for the coordinate system
  OffsetX = ObjectBox->Width + (Width-ObjectBox->Width)*0.5;
  OffsetY = Height - ClientHeight*0.5;
}
//---------------------------------------------------------------------------
void TMainForm::DrawSurfaces()
{
  struct SSurface *s;                 // general pointer to Surface
  struct SPlatform *p;               // general pointer to platform
  long i,j;
  // T contains Surface position, RotatedT the position projected
  // onto the XY plane
  // 0-2 - actual surface points, 3-5 - project onto x/y plane, 6-7 normal vector
  // 8 - PTMAX+8 PTs
//  double **T, **RotatedT;//T[1000][3],RotatedT[1000][3];


 // double AntennaDirAzi, AntennaDirElev, TRayAzi, TRayElev, OffsetAzi, OffsetElev;
//  double AntennaGain, AntennaGainT, AntennaGainR;

  #define PTMAX 5000
  double T[PTMAX+8][3],RotatedT[PTMAX+8][3];
  double DirVecL = (20/ZoomX);
  long PFN;
  double DirAzi,DirEle;
  double S[3],Ang;
  long Shade;
  double NormVecUnit[3];


  // now draw Surfaces
  long PTsUsed, LimitPTs;
  double Density, TriArea, **CoordPT;
  double L10[3], L20[3], NormVec[3], temp[3][3];
  long TotalPTs,CPT;
  struct SPlatform  *RadarPF;      // pointer to radar platform
  long RadarPFNo;             // number of radar platform
  double PlatformPosC[3],PlatformRotC[3];
  double AntennaDirAzi, AntennaDirElev;
  double OffsetAzi,OffsetElev, TRayAzi, TRayElev;
  struct SRadar *r;
  double AntennaGain;

  int TH, TW;
  char str[80];

  r = FirstRadar;
  if (r != NULL)
    {
      // find radar platform
      FindPlatform(r->PlatformName, &RadarPF, &RadarPFNo, FirstPlatform);
      FindPlatformPosition(RadarPF,STime,PlatformPosC);
      FindPlatformRotation(RadarPF,STime,PlatformRotC);

      // find the direction into which radar is pointing relative to
      // the radar platform
      FindAntennaDir(r, STime, PlatformPosC, PlatformRotC, &AntennaDirAzi,&AntennaDirElev);
    }

  s=FirstSurface;
  while (s != NULL)
	{

       if (GlobalUnderSampleSurf == 0)  // this is only true if we write to disk
      // sort of indicator - use all PTs
        {
          Density = s->PTDensity / 100000;
        }
       else
        {
      if (s->GlobalUnderSample == 1)
        Density = s->PTDensity / GlobalUnderSampleSurf;
      else
        Density = s->PTDensity / s->USampleFactor;
        }

      for (CPT=0;CPT<3;CPT++) // for all 3 points on the triangle
        for (j=0;j<3;j++)  // for all 3 coords
          temp[CPT][j] = s->Tri[CPT][j]; //SurfP[SNo][PNo][CPT*3+j];   // temp[0-2] = actual triangle points
      SubtractVec(temp[1], temp[0], L10);
      SubtractVec(temp[2], temp[0], L20);
      // calculate unnormalized normal vector (perp vec)
      CrossP(L10, L20, NormVec);               // normal vector
      TriArea = FindMag(NormVec)*0.5;
      LimitPTs = ceil(TriArea*Density*1.1+100);
      CoordPT = DMatrix(0,LimitPTs-1,0,2);
//      T = DMatrix(0,8+LimitPTs-1,0,2);
//      RotatedT = DMatrix(0,8+LimitPTs-1,0,2);

      ConvertSurfaceToPTs(USE_TRI_ARRAY,s,0, &PTsUsed, LimitPTs, Density,
        CoordPT, 0, NULL, NormVecUnit);

      TotalPTs = PTsUsed;

      for (CPT=0;(CPT<TotalPTs) && (CPT<PTMAX);CPT++) // for all 3 points on the triangle
        {
    	  Copy3DPoint(T[8+CPT],CoordPT[CPT]);
        }

    // find pointer to Surface point platform
	  FindPlatform(s->Name, &p, &PFN, FirstPlatform);
	  // find Surface position
	  Copy3DPoint(T[0],s->Tri[0]);
	  Copy3DPoint(T[1],s->Tri[1]);
	  Copy3DPoint(T[2],s->Tri[2]);
	  // make a copy
	  Copy3DPoint(T[3],T[0]);
	  Copy3DPoint(T[4],T[1]);
	  Copy3DPoint(T[5],T[2]);
	  // for projection onto XY plane set Z = 0
	  T[3][2] = 0;
	  T[4][2] = 0;
	  T[5][2] = 0;
	  // for normal vector
	  T[6][0] = (T[0][0]+T[1][0]+T[2][0])*0.33333333;
	  T[6][1] = (T[0][1]+T[1][1]+T[2][1])*0.33333333;
	  T[6][2] = (T[0][2]+T[1][2]+T[2][2])*0.33333333;
	  FindSurfaceNormal(s, &DirAzi, &DirEle);
	  ConvertAnglesToVec(T[7], DirVecL, DirAzi, DirEle);
    T[7][0] += T[6][0];
    T[7][1] += T[6][1];                   // castimg
    T[7][2] += T[6][2];
	  // rotate points
	  RotateNPoints(T, RotatedT, 8+TotalPTs, PlatformRot[PFN]);
	  // add relative point Surface position to platform position
	  for (j=0;j<8+TotalPTs;j++)
  	  for (i=0;i<3;i++)
      	T[j][i] = RotatedT[j][i] + PlatformPos[PFN][i];
	  // now rotate to get screen coordinates
	  RotatePoints3(T, RotatedT, 8+TotalPTs, OffsetX, OffsetY, ZoomX, ZoomY, Rot, Pos,
						 Focus);
	  // and draw Surface

    if (!ShowSurfaceOutlines)
    {
      for (i=0;i<3;i++)
        S[i] = T[7][i]- T[6][i];
      Ang = ElevAngle(S);
   //   Label5->Caption = Shade;
      if (Ang < 0) Shade=0xFFFFFF;
      else
        {
          Shade = 100 + Ang*TwoOverPI*154;
          Shade = Shade + 256*Shade +65536*Shade;
        }
      // draw solid surface
      POINT P[4];
 	    for (i=0;i<3;i++)
        {
          P[i].x = RotatedT[i][2];
          P[i].y = RotatedT[i][1];
        }
       P[3].x = RotatedT[0][2];
       P[3].y = RotatedT[0][1];
       Canvas->Pen->Color = clBlack;
       Canvas->Brush->Color = (TColor) Shade; //clSilver;
       Canvas->Polygon(P,3);
    }
    else
     {
 	    for (i=0;i<3;i++)
        {
          // outlines
          j = i+1; if (j==3) j=0;
          Canvas->Pen->Style = psSolid;
          Canvas->Pen->Color = clSilver;
 	  	    Canvas->MoveTo(RotatedT[i][2],RotatedT[i][1]);
	  	    Canvas->LineTo(RotatedT[j][2],RotatedT[j][1]);
        }

      Canvas->Pen->Style = psSolid;
      Canvas->Pen->Color = clBlack;
      Canvas->Brush->Color = clRed;
        // PTs on surface
 	    for (i=8;i<8+TotalPTs;i++)
        {
          if (r != NULL)
            {

          TRayAzi = AziAngle(CoordPT[i-8]);
          TRayElev = ElevAngle(CoordPT[i-8]);

          OffsetAzi = -AntennaDirAzi + TRayAzi + PI;
          OffsetElev = fabs(AntennaDirElev - TRayElev);

          AntennaGain = fabs(FindAntennaGainRT(OffsetAzi, OffsetElev, r));

//          sprintf(str,"%.3f",AntennaGain);

          if ((fabs(AntennaGain) < TINY_NUM) || (AntennaGain == 0))
            sprintf(str,"%.3G",double(-200));
          else if (AntennaGain > 0)
            {
              if (fabs(log10(AntennaGain)) < TINY_NUM)
                sprintf(str,"0");
              else
                sprintf(str,"%.3f",log10(AntennaGain));
            }
          else
            sprintf(str,"Err");

 		  Canvas->Pen->Style = psSolid;
          Canvas->Font->Color = clGray;
          TW = Canvas->TextWidth(str);
          TH = Canvas->TextHeight(str);
          Canvas->Brush->Color = clWhite;
		  Canvas->TextOut(RotatedT[i][2]-TW*0.5,RotatedT[i][1]-TH*0.5-7,str);
          Canvas->Brush->Color = clRed;
        }
   	     Canvas->Ellipse(RotatedT[i][2]-2,RotatedT[i][1]-2,RotatedT[i][2]+3,RotatedT[i][1]+3);
        }


     }
    // draw normal vector
    Canvas->Pen->Style = psSolid;
    Canvas->Pen->Color = clNavy;
	  Canvas->MoveTo(RotatedT[6][2],RotatedT[6][1]);
	  Canvas->LineTo(RotatedT[7][2],RotatedT[7][1]);
 	  for (i=0;i<3;i++)
      {
    	  // show guidelines ? - =dotted line from Surface to X-Y plane
	      if (ShowElevationLines)
	        {
	  	     Canvas->Pen->Style = psDot;
           Canvas->Pen->Color = clBlack;
           Canvas->Brush->Style = bsClear;
           Canvas->MoveTo(RotatedT[i][2],RotatedT[i][1]);
	  	     Canvas->LineTo(RotatedT[i+3][2],RotatedT[i+3][1]);
          }
      }
    // next surface
	  s = s->next;
      Free_DMatrix(CoordPT,0,0);
  //    Free_DMatrix(T,0,0);
  //    Free_DMatrix(RotatedT,0,0);
   } // end while

}
//---------------------------------------------------------------------------
void TMainForm::DrawTargets()
{
  STarget *t;                 // general pointer to target
  SPlatform *p;               // general pointer to platform
  long i;
  // T contains target position, RotatedT the position projected
  // onto the XY plane
  double T[3][3],RotatedT[3][3];
  double DirVecL = (20/ZoomX);
  long PFN;
  double DirAzi,DirEle;
  // now draw targets
  t=FirstTarget;

  while (t != NULL)
	{
      // find pointer to target point platform
	  FindPlatform(t->Name, &p, &PFN, FirstPlatform);
	  // find target position
	  Copy3DPoint(T[0],TargetPos[FindTargetNo(FirstTarget,t)]);
	  // make a copy
	  Copy3DPoint(T[1],T[0]);
	  // for projection onto XY plane set Z = 0
	  T[1][2] = 0;
	  // for directional vector
	  FindTargetDir(t, &DirAzi, &DirEle);
	  ConvertAnglesToVec(T[2], DirVecL, DirAzi, DirEle);
	  T[2][0] += T[0][0];
	  T[2][1] += T[0][1];
	  T[2][2] += T[0][2];
	  // rotate points
	  RotateNPoints(T, RotatedT, 3, PlatformRot[PFN]);
	  // add relative point target position to platform position
	  for (i=0;i<3;i++)
	    {
	  	 T[0][i] = RotatedT[0][i] + PlatformPos[PFN][i];
	  	 T[1][i] = RotatedT[1][i] + PlatformPos[PFN][i];
	  	 T[2][i] = RotatedT[2][i] + PlatformPos[PFN][i];
	    }
	  // now rotate to get screen coordinates
	  RotatePoints3(T, RotatedT, 3, OffsetX, OffsetY, ZoomX, ZoomY, Rot, Pos,
						 Focus);
	  // and draw target
      Canvas->Pen->Style = psSolid;
      Canvas->Pen->Color = clBlack;
      Canvas->Brush->Color = clRed;
	  Canvas->Ellipse(RotatedT[0][2]-3,RotatedT[0][1]-3,RotatedT[0][2]+4,RotatedT[0][1]+4);
	  // if necessary draw surface vector
	  if (t->ReflecType == 1)
	    {
	  	 Canvas->MoveTo(RotatedT[0][2],RotatedT[0][1]);
	  	 Canvas->LineTo(RotatedT[2][2],RotatedT[2][1]);
	    }
	  // show guidelines ? - =dotted line from target to X-Y plane
	  if (ShowElevationLines)
	    {
      Canvas->Brush->Style = bsClear;
	  	 Canvas->Pen->Style = psDot;
         Canvas->Pen->Color = clBlack;
	  	 Canvas->MoveTo(RotatedT[0][2],RotatedT[0][1]);
	  	 Canvas->LineTo(RotatedT[1][2],RotatedT[1][1]);
	    }
	  t = t->next;
   } // end while
}
//---------------------------------------------------------------------------
void TMainForm::DrawRadars()
{
  #define BEAM_RES 16
  // draw radars
  SRadar *r;
  SPlatform *p;               // general pointer to platform
  r=FirstRadar;
  double R[BEAM_RES+1][3],RotatedR[BEAM_RES+1][3],ADirAzi, ADirElev;
  double L,BL=10,RadiusEle,RadiusAzi;
  long PFN,i,j;
  double RotAng[3];

  Canvas->Pen->Style = psSolid;
  while (r != NULL)
	{
	  // find pointer to radar platform
	  FindPlatform(r->PlatformName, &p, &PFN, FirstPlatform);
      if (r->AntennaGainTypeT == TRANS_ISO)
	    {
          Copy3DPoint(R[0],PlatformPos[PFN]);
		  // now rotate to get screen coordinates
		  RotatePoints3(R, RotatedR, 1, OffsetX, OffsetY, ZoomX, ZoomY, Rot,
		  				  Pos, Focus);
          // and draw Radar
          Canvas->Pen->Color = clGray;
          Canvas->Brush->Color = clSilver;
          Canvas->Brush->Style = bsSolid;
		  Canvas->Ellipse(RotatedR[0][2]-5,RotatedR[0][1]-5,
							RotatedR[0][2]+5,RotatedR[0][1]+5);
          Canvas->Pen->Style = psDot;
          Canvas->Pen->Color = clBlack;
          Canvas->Brush->Style = bsClear;
		  Canvas->Ellipse(RotatedR[0][2]-BL,RotatedR[0][1]-BL,
		  			RotatedR[0][2]+BL,RotatedR[0][1]+BL);
		}
	  else if (r->AntennaGainTypeT == TRANS_SIN)
		{
		  // find the direction into which radar is pointing relative to
		  // the radar platform
		  FindAntennaDir(r, STime, PlatformPos[PFN], PlatformRot[PFN],
                         &ADirAzi,&ADirElev);

		  L = 50/ZoomX;
		  RadiusEle = tan(r->ElevBeamWidthT*0.5)*L;
		  RadiusAzi = tan(r->AziBeamWidthT*0.5)*L;
          for (i=0;i<BEAM_RES;i++)
            {
              R[i][0] = L;
              R[i][1] = RadiusAzi * cos((float(i)/float(BEAM_RES))*TwoPi);
              R[i][2] = RadiusEle * sin((float(i)/float(BEAM_RES))*TwoPi);
            }
          // rotate points
          RotAng[0] = 0; RotAng[1] = ADirElev; RotAng[2] = ADirAzi - PIOver2;
	      RotateNPoints(R, RotatedR, BEAM_RES, RotAng);
		  // add relative point target position to platform position
		  for (i=0;i<3;i++)
            {
              for (j=0;j<BEAM_RES;j++)
                R[j][i] = RotatedR[j][i] + PlatformPos[PFN][i];
              // we need the actual radat position as well
              R[BEAM_RES][i] = PlatformPos[PFN][i];
            }
          // now rotate to get screen coordinates
	      RotatePoints3(R, RotatedR, BEAM_RES+1, OffsetX, OffsetY, ZoomX, ZoomY,
                        Rot, Pos, Focus);
          // and draw Radar
          Canvas->Pen->Color = clGray;
          Canvas->Brush->Color = clSilver;
          Canvas->Brush->Style = bsSolid;
		  Canvas->Ellipse(RotatedR[BEAM_RES][2]-5,RotatedR[BEAM_RES][1]-5,
							RotatedR[BEAM_RES][2]+5,RotatedR[BEAM_RES][1]+5);
          Canvas->Brush->Style = bsClear;
          Canvas->Pen->Color = clBlack;
          Canvas->MoveTo(RotatedR[BEAM_RES-1][2],RotatedR[BEAM_RES-1][1]);
          // and draw ellipse
          for (j=0;j<BEAM_RES;j++)
            Canvas->LineTo(RotatedR[j][2],RotatedR[j][1]);
          // and draw lines from radar center to ellipse
          for (j=0;j<BEAM_RES;j+=4)
            {
              Canvas->MoveTo(RotatedR[BEAM_RES][2],RotatedR[BEAM_RES][1]);
              Canvas->LineTo(RotatedR[j][2],RotatedR[j][1]);
            }
      }
	  else if (r->AntennaGainTypeT == TRANS_OTHER)
		{
          Copy3DPoint(R[0],PlatformPos[PFN]);
		  // find the direction into which radar is pointing relative to
		  // the radar platform
		  FindAntennaDir(r, STime, PlatformPos[PFN],
				 PlatformRot[PFN],&ADirAzi,&ADirElev);
		  ConvertAnglesToVec(R[1],(30/ZoomX),ADirAzi,ADirElev);
          // add platform position
		  R[1][0] += PlatformPos[PFN][0];
		  R[1][1] += PlatformPos[PFN][1];
		  R[1][2] += PlatformPos[PFN][2];
		  // now rotate to get screen coordinates
		  RotatePoints3(R, RotatedR, 2, OffsetX, OffsetY, ZoomX, ZoomY, Rot,
						 Pos, Focus);
          // and draw Radar
          Canvas->Pen->Color = clGray;
          Canvas->Brush->Color = clSilver;
          Canvas->Brush->Style = bsSolid;
		  Canvas->Ellipse(RotatedR[0][2]-5,RotatedR[0][1]-5,
							RotatedR[0][2]+5,RotatedR[0][1]+5);
          Canvas->Brush->Style = bsClear;
          // and 'direction' line
          Canvas->Pen->Width = 2;
          Canvas->Pen->Color = clBlack;
          Canvas->MoveTo(RotatedR[0][2],RotatedR[0][1]);
		  Canvas->LineTo(RotatedR[1][2],RotatedR[1][1]);
          Canvas->Pen->Width = 1;
        }
	   r = r->next;
	 }
}
//---------------------------------------------------------------------------
void TMainForm::DrawPlatforms()
{
  // draw platforms
  struct SPlatform *p;               // general pointer to platform
  Canvas->Pen->Color = clNavy;
  int TW, TH; // text width, height
  long PFN,i,j;

  double P[5][3],RotatedP[5][3];
  p=FirstPlatform;
  Canvas->Brush->Style = bsClear;
  while (p != NULL)
	 {
	   //if (stricmp(p->Name,"Earth") != 0)

		  FindPlatform(p->Name, &p, &PFN, FirstPlatform);
		  // now create platform coordinate system P[0,1,2]
		  for (i=0;i<3;i++)
            for (j=0;j<3;j++)
              if (i==j) P[i][j] = 50/ZoomX; else P[j][i] = 0;
		  RotateNPoints(P, RotatedP, 3, PlatformRot[PFN]);
		  // 3 = platform position
          Copy3DPoint(P[3],PlatformPos[PFN]);
		  // 4 = project point onto X-Y plane for guidelines
		  P[4][0] = P[3][0];
		  P[4][1] = P[3][1];
		  P[4][2] = 0;
		  // add rotated system to platform position
		  for (i=0;i<3;i++)
			 {
				P[i][0] = RotatedP[i][0] + P[3][0];
				P[i][1] = RotatedP[i][1] + P[3][1];
				P[i][2] = RotatedP[i][2] + P[3][2];
			 }
		  // convert to screen coordinates
		  RotatePoints3(P, RotatedP, 5, OffsetX, OffsetY, ZoomX, ZoomY, Rot,
						  Pos, Focus);
          if (stricmp(p->Name,"Earth") != 0)
       {
		  // show guidelines ? - =dotted line from target to X-Y plane
		  if (ShowElevationLines)
			 {
				Canvas->Pen->Style = psDot;
                Canvas->Pen->Color = clBlack;
                Canvas->MoveTo(RotatedP[3][2],RotatedP[3][1]);
				Canvas->LineTo(RotatedP[4][2],RotatedP[4][1]);
			 }
/*		  // show name of platform
		  Canvas->Pen->Style = psSolid;
          Canvas->Font->Color = clGray;
          TW = Canvas->TextWidth(p->Name);
          TH = Canvas->TextHeight(p->Name);
		  Canvas->TextOut(RotatedP[3][2]-TW*0.5,RotatedP[3][1]-TH-4,p->Name);
 */
          Canvas->Font->Color = clBlack;
		  Canvas->TextOut(RotatedP[0][2]-4,RotatedP[0][1]-4,"X");
		  Canvas->TextOut(RotatedP[1][2]-4,RotatedP[1][1]-4,"Y");
		  Canvas->TextOut(RotatedP[2][2]-4,RotatedP[2][1]-4,"Z");

		  Canvas->Pen->Style = psSolid;
          Canvas->Pen->Color = clRed;
          Canvas->MoveTo(RotatedP[3][2],RotatedP[3][1]);
		  Canvas->LineTo(RotatedP[0][2],RotatedP[0][1]);
		  Canvas->Pen->Color = clGreen;
		  Canvas->MoveTo(RotatedP[3][2],RotatedP[3][1]);
		  Canvas->LineTo(RotatedP[1][2],RotatedP[1][1]);
		  Canvas->Pen->Color = clBlue;
          Canvas->MoveTo(RotatedP[3][2],RotatedP[3][1]);
		  Canvas->LineTo(RotatedP[2][2],RotatedP[2][1]);
       }


       	  // show name of platform
		  Canvas->Pen->Style = psSolid;
          Canvas->Font->Color = clGray;
          TW = Canvas->TextWidth(p->Name);
          TH = Canvas->TextHeight(p->Name);
		  Canvas->TextOut(RotatedP[3][2]-TW*0.5,RotatedP[3][1]-TH-4,p->Name);


		 // end if
		p = p->next;                     // goto next platform
	 }
}
//---------------------------------------------------------------------------
void TMainForm::FreeAllVariables()
{
  int i;
  // check through all available 'slots'
  for (i=0;i<MAX_GRAPHS;i++)
    if (SimuGraphActive[i])
      SimuGraphs[i]->Close();
  delete SimuGraphs;
  // free memory allocated
  Free_DMatrix(PlatformPos,0,0);
  Free_DMatrix(PlatformVel,0,0);
  Free_DMatrix(PlatformRot,0,0);
  Free_DMatrix(TargetPos,0,0);
  // free all linked lists
  FreeTargetList(&FirstTarget);
  FreeSurfaceList(&FirstSurface);
  FreePlatformList(&FirstPlatform,&FirstTarget,&FirstSurface,&FirstRadar);
  FreeRadarList(&FirstRadar);
  FreeSimulationList(&FirstSimulation);
  FreeGeometryList(&FirstGeometry);

}
//---------------------------------------------------------------------------
void TMainForm::SetupAllVariables()
{
  // initialize help file
  Application->HelpFile = "SARSIMHLP.HLP";

  // initialize pointers to Target, Platform and Radar linked lists
  FirstTarget = NULL;
  FirstPlatform = NULL;
  FirstRadar = NULL;
  FirstSimulation = NULL;
  FirstGeometry = NULL;
  CalcCoordSystemOffset();
  // allocate memory for graph forms array
  SimuGraphs = new TSimulationForm*[MAX_GRAPHS];
  // and initially all are inactive
  long i;
  for (i=0;i<MAX_GRAPHS;i++)
    SimuGraphActive[i] = FALSE;
  // create arrays...
  // position, velocity and rotation of each platform and target
  PlatformPos = DMatrix(0,1,0,2);
  PlatformVel = DMatrix(0,1,0,2);
  PlatformRot = DMatrix(0,1,0,2);
  TargetPos = DMatrix(0,1,0,2);
  // Simulation time
  STime = 0;
  // initialize window view parameters
  Rot[0] = -90*DegToRad; Rot[1] = -30*DegToRad; Rot[2] = -30*DegToRad;
  ZoomX = 0.01; ZoomY = ZoomX;
  Pos[0] = 0; Pos[1] = 0; Pos[2] = 0;
  Focus[0] = 1000; Focus[1] = 0; Focus[2] = 0;
  FocusPlatform = NULL;
  LButtonDown = FALSE;
  RButtonDown = FALSE;
  ZoomMode = FALSE;
  ShowLittleCoordSystemAxes = TRUE;
  ShowCoordSystemAxes = TRUE;
  ShowElevationLines = TRUE;
  ShowSurfaceOutlines = true;
  STime = 0;
  LeftBClickPoint.x = 0;
  LeftBClickPoint.y = 0;
  RightBClickPoint.x = 0;
  RightBClickPoint.y = 0;
  IsNewFile = true;
  IsDirty   = false;
  TimeMinusButtonDown = FALSE;
  TimePlusButtonDown = FALSE;

  // create palette used to draw graphs
  RedBluePalette = (LOGPALETTE*)new char[sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * 128];
//  RedBluePalette = (LOGPALETTE*)new char[sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * 212];//128];
  RedBluePalette->palVersion = 0x300;
//  RedBluePalette->palNumEntries = 212;//128;
  RedBluePalette->palNumEntries = 128;
  for (i = 0; i < (128/2); ++i) {
	 RedBluePalette->palPalEntry[i].peRed = double(32)*sqrt(i);
	 RedBluePalette->palPalEntry[i].peGreen = 0;
	 RedBluePalette->palPalEntry[i].peBlue = 0;
	 RedBluePalette->palPalEntry[i].peFlags = PC_NOCOLLAPSE;//PC_EXPLICIT;//PC_RESERVED;
  }
  for (i = (128/2); i < 128; ++i) {
	 RedBluePalette->palPalEntry[i].peRed = 0;
	 RedBluePalette->palPalEntry[i].peGreen = 0;
	 RedBluePalette->palPalEntry[i].peBlue = double(32)*sqrt(i-(128/2));
	 RedBluePalette->palPalEntry[i].peFlags = PC_NOCOLLAPSE;//PC_EXPLICIT;//PC_RESERVED;
  }
/*
  for (i = 128; i < 192; ++i) {
	 RedBluePalette->palPalEntry[i].peRed = 0;
	 RedBluePalette->palPalEntry[i].peGreen = double(32)*sqrt(i-128);
	 RedBluePalette->palPalEntry[i].peBlue = 0;
	 RedBluePalette->palPalEntry[i].peFlags = PC_NOCOLLAPSE;//PC_EXPLICIT;//PC_RESERVED;
  }
  for (i = 192; i < 212; ++i) {
	 RedBluePalette->palPalEntry[i].peRed = double(32)*sqrt(i-192);
	 RedBluePalette->palPalEntry[i].peGreen = double(32)*sqrt(i-192);
	 RedBluePalette->palPalEntry[i].peBlue = double(32)*sqrt(i-192);
	 RedBluePalette->palPalEntry[i].peFlags = PC_NOCOLLAPSE;//PC_EXPLICIT;//PC_RESERVED;
  }
*/
  // add earth Platform
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormPaint(TObject *Sender)
{
  char str[80];

  // show azimuth and elevation angles
  sprintf(str,"%-.0f°",round(fmod(fmod(Rot[1]*RadToDeg-90.0,360.0)+360.0,360.0)-180.0));
  AziAng->Caption = str;
  sprintf(str,"%-.0f°",round(-Rot[2]*RadToDeg));
  EleAng->Caption = str;

  // Simulation time
  sprintf(str,"%-.3f",STime);
  EditSTime->Text = str;

  DrawSurfaces();
  DrawCoordSystem();
  DrawLittleCoordSystem();
  DrawTargets();
  DrawRadars();
  DrawPlatforms();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Focus1Click(TObject *Sender)
{
  FocusForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ModifyTClick(TObject *Sender)
{
  ModifyTarget(SelectedTarget);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DeleteTClick(TObject *Sender)
{
  DeleteTargetMain(SelectedTarget);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ModifyRClick(TObject *Sender)
{
  ModifyRadar(SelectedRadar);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DeleteRClick(TObject *Sender)
{
  DeleteRadarMain(SelectedRadar);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ModifyPlatform1Click(TObject *Sender)
{
  ModifyPlatform(SelectedPlatform);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DeletePlatform1Click(TObject *Sender)
{
  DeletePlatformMain(SelectedPlatform);
}
//---------------------------------------------------------------------------//---------------------------------------------------------------------------
void __fastcall TMainForm::ObjectBoxClick(TObject *Sender)
{
  int SelectionType;
  void *Object;
  // what was selected ?
  if(FindObjectBoxSel(&SelectionType,&Object))
	 {
        // and do something with it
  		switch(SelectionType) {
		  case 0 : // Platform selected
			 {
                SelectedPlatform = (struct SPlatform*)Object;
			    PlatformPopupMenu->Popup(ObjectBox->Left+ObjectBox->Width*0.5+Left,
                   Panel2->Top+ObjectBox->Top+Top+
                   (ObjectBox->ItemIndex - ObjectBox->TopIndex)*
                   ObjectBox->ItemHeight);
				break;
			 }
		  case 1 : // Radar selected
			 {
                SelectedRadar = (struct SRadar*)Object;
			    RadarPopupMenu->Popup(ObjectBox->Left+ObjectBox->Width*0.5+Left,
                   Panel2->Top+ObjectBox->Top+Top+
                   (ObjectBox->ItemIndex - ObjectBox->TopIndex)*
                   ObjectBox->ItemHeight);
				break;
			 }
		  case 2 : // Target selected
			 {
                SelectedTarget = (struct STarget*)Object;
			    TargetPopupMenu->Popup(ObjectBox->Left+ObjectBox->Width*0.5+Left,
                   Panel2->Top+ObjectBox->Top+Top+
                   (ObjectBox->ItemIndex - ObjectBox->TopIndex)*
                   ObjectBox->ItemHeight);
				break;
			 }
		  case 3 : // Surface selected
			 {
          SelectedSurface = (struct SSurface*)Object;
			    SurfacePopupMenu->Popup(ObjectBox->Left+ObjectBox->Width*0.5+Left,
          Panel2->Top+ObjectBox->Top+Top+
               (ObjectBox->ItemIndex - ObjectBox->TopIndex)*
                ObjectBox->ItemHeight);
				break;
			 }
		} // end switch
	 } // end if

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Save1Click(TObject *Sender)
{
  if (IsNewFile)
	 SaveAs1Click(Sender);
  else
    {
	  SaveFile(SaveSimDialog->FileName.c_str());
	  IsNewFile = IsDirty = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveAs1Click(TObject *Sender)
{
  if (IsNewFile)
	SaveSimDialog->FileName = "";

  if (SaveSimDialog->Execute())
	 {
		SaveFile(SaveSimDialog->FileName.c_str());
        char buff[150];
        sprintf(buff,"Sarsim II (Ver. %s) - ",version);
        strcat(buff,SaveSimDialog->FileName.c_str());
        Caption = buff;
	    IsNewFile = IsDirty = false;
	 }
}
//---------------------------------------------------------------------------
void TMainForm::ReloadScript()
{
  struct LinkedLists FirstObjectP;

  // clear all stuff
  FreeAllVariables();
  SetupAllVariables();

  FirstObjectP.FirstGeometry = FirstGeometry;
  FirstObjectP.FirstSimulation = FirstSimulation;
  FirstObjectP.FirstPlatform = FirstPlatform;
  FirstObjectP.FirstTarget = FirstTarget;
  FirstObjectP.FirstRadar = FirstRadar;
  FirstObjectP.FirstSurface = FirstSurface;
  OpenFile(TEMP_SCRIPT,&FirstObjectP);
  FirstGeometry =   FirstObjectP.FirstGeometry;
  FirstSimulation =   FirstObjectP.FirstSimulation;
  FirstPlatform =   FirstObjectP.FirstPlatform;
  FirstTarget =   FirstObjectP.FirstTarget;
  FirstRadar =   FirstObjectP.FirstRadar;
  FirstSurface =   FirstObjectP.FirstSurface;

  DeleteFile(TEMP_SCRIPT);   // delete temporary script file
  UpdateObjectList();
  CalcNewSTimeArray();
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Open1Click(TObject *Sender)
{
  char buff[150];
  struct LinkedLists FirstObjectP;

  // make sure no data gets lost
  if (IsDirty)
	if(Application->MessageBox("Do you want to save?",
       "Simulation has changed", MB_YESNO | MB_ICONQUESTION) ==
		IDYES)
      SaveAs1Click(Sender);
  if (OpenSimDialog->Execute())
    {
      // clear all stuff
      FreeAllVariables();
      SetupAllVariables();
      sprintf(buff,"Sarsim II (Ver. %s) - ",version);
//      strcpy(buff,"Sarsim II - ");
      strcat(buff,OpenSimDialog->FileName.c_str());
      Caption = buff;
      SaveSimDialog->FileName = OpenSimDialog->FileName;

      FirstObjectP.FirstGeometry = FirstGeometry;
      FirstObjectP.FirstSimulation = FirstSimulation;
      FirstObjectP.FirstPlatform = FirstPlatform;
      FirstObjectP.FirstTarget = FirstTarget;
      FirstObjectP.FirstRadar = FirstRadar;
      FirstObjectP.FirstSurface = FirstSurface;
      OpenFile(OpenSimDialog->FileName.c_str(),&FirstObjectP);
      FirstSimulation =   FirstObjectP.FirstSimulation;
      FirstGeometry =   FirstObjectP.FirstGeometry;
      FirstPlatform =   FirstObjectP.FirstPlatform;
      FirstTarget =   FirstObjectP.FirstTarget;
      FirstRadar =   FirstObjectP.FirstRadar;
      FirstSurface =   FirstObjectP.FirstSurface;

      IsNewFile = false;
      UpdateObjectList();
      CalcNewSTimeArray();
      Invalidate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::OpenLastFileClick(TObject *Sender)
{
  char buff[150];
  struct LinkedLists FirstObjectP;

  // make sure no data gets lost
  if (IsDirty)
	if(Application->MessageBox("Do you want to save?",
       "Simulation has changed", MB_YESNO | MB_ICONQUESTION) ==
		IDYES)
      SaveAs1Click(Sender);

      OpenSimDialog->FileName = LastFile;
      // clear all stuff
      FreeAllVariables();
      SetupAllVariables();
      sprintf(buff,"Sarsim II (Ver. %s) - ",version);
//      strcpy(buff,"Sarsim II - ");
      strcat(buff,OpenSimDialog->FileName.c_str());
      Caption = buff;
      SaveSimDialog->FileName = OpenSimDialog->FileName;

      FirstObjectP.FirstGeometry = FirstGeometry;
      FirstObjectP.FirstSimulation = FirstSimulation;
      FirstObjectP.FirstPlatform = FirstPlatform;
      FirstObjectP.FirstTarget = FirstTarget;
      FirstObjectP.FirstRadar = FirstRadar;
      FirstObjectP.FirstSurface = FirstSurface;
      OpenFile(OpenSimDialog->FileName.c_str(),&FirstObjectP);
      FirstSimulation =   FirstObjectP.FirstSimulation;
      FirstGeometry =   FirstObjectP.FirstGeometry;
      FirstPlatform =   FirstObjectP.FirstPlatform;
      FirstTarget =   FirstObjectP.FirstTarget;
      FirstRadar =   FirstObjectP.FirstRadar;
      FirstSurface =   FirstObjectP.FirstSurface;

      IsNewFile = false;
      UpdateObjectList();
      CalcNewSTimeArray();
      Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::New2Click(TObject *Sender)
{
  // make sure no data gets lost
  if (IsDirty)
	if(Application->MessageBox("Do you want to save?",
       "Simulation has changed", MB_YESNO | MB_ICONQUESTION) ==
		IDYES)
      SaveAs1Click(Sender);

  // clear all stuff
  FreeAllVariables();
  SetupAllVariables();
  // add earth Platform
  AddPlatform(&DefaultPlatformValues, &FirstPlatform);
  UpdateObjectList();
  CalcNewSTimeArray();
  char s[100];
  sprintf(s,"Sarsim II (Ver. %s) - ",version);
  Caption = s;
  IsDirty = FALSE;
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  // make sure no data gets lost
  if (IsDirty)
	if(Application->MessageBox("Do you want to save?",
       "Simulation has changed", MB_YESNO | MB_ICONQUESTION) ==
		IDYES)
      SaveAs1Click(Sender);
  CanClose = TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ExitClick(TObject *Sender)
{
  MainForm->Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Previous1Click(TObject *Sender)
{
  if (FirstSimulation == NULL)
	Application->MessageBox("No previous simulation stored.", "Message", MB_OK);
  else
    {
      PreviousSimForm->ShowModal();
      if ((PreviousSimForm->ModalResult == mrOk) && PreviousSimForm->SimSelection != NULL)
        NewSimulation(PreviousSimForm->SimSelection->SimTYPE,PreviousSimForm->SimSelection);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DecTimeMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
  TimeMinusButtonDown = TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::IncTimeMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
  TimePlusButtonDown = TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DecTimeMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
  TimeMinusButtonDown = FALSE;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::IncTimeMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
  TimePlusButtonDown = FALSE;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Timer1Timer(TObject *Sender)
{
  double StepSize;

  StepSize = atof(EStep->Text.c_str());
  if (StepSize == 0)
   {
     // EStep->Text = "0";
   }
  else
    {
      if (BAni->Enabled == false)
        {
          STime += StepSize;
          CalcNewSTimeArray();
          Invalidate();
        }

      if (TimeMinusButtonDown)
        {
          STime -= StepSize;
          CalcNewSTimeArray();
          Invalidate();
        }
      if (TimePlusButtonDown)
        {
          STime += StepSize;
          CalcNewSTimeArray();
          Invalidate();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::EditSTimeKeyPress(TObject *Sender, char &Key)
{
  if (Key == ENTER_KEY)
    {
      STime = atof(EditSTime->Text.c_str());
      CalcNewSTimeArray();
      Invalidate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::About1Click(TObject *Sender)
{
  AboutForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender)
{
  LogoForm->Timer1->Enabled = true;

  LogoForm->Show();

  // expired ? - these constants are defined in dest.h
  Caption = "Sarsim II - expires on "+AnsiString(Expire_YEAR)+":"+
    AnsiString(Expire_MONTH)+":"+AnsiString(Expire_DAY);

  TDateTime  ExpireDate(Expire_YEAR,Expire_MONTH,Expire_DAY),CDate;
  if (CDate.CurrentDate() > ExpireDate)
    {
	  Application->MessageBox("This version has expired.\nPlease obtain an"
      " updated version on : http://rrsg.ee.uct.ac.za.", "Warning", MB_OK);
      MainForm->Close();
    }
  // add earth Platform
  AddPlatform(&DefaultPlatformValues, &FirstPlatform);
  CalcNewSTimeArray();
  UpdateObjectList();
  IsDirty = FALSE;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SimulationOutput1Click(TObject *Sender)
{
  MainViewForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ScriptFile1Click(TObject *Sender)
{
  ShowScriptForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SpeedButton4Click(TObject *Sender)
{
//  Application->HelpFile = "SARSIMHLP.HLP";
  Application->HelpCommand(HELP_CONTENTS, 0);
//  Application->HelpJump("main");
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DeleteSurface1Click(TObject *Sender)
{
  DeleteSurfaceMain(SelectedSurface);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ModifySurface1Click(TObject *Sender)
{
  ModifySurface(SelectedSurface);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CoordClick(TObject *Sender)
{
  Coord->Checked = !Coord->Checked;
  ShowCoordSystemAxes = Coord->Checked;
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ElevationLines1Click(TObject *Sender)
{
  ElevationLines1->Checked = !ElevationLines1->Checked;
  ShowElevationLines = ElevationLines1->Checked;
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FilledSurfaces1Click(TObject *Sender)
{
  FilledSurfaces1->Checked = !FilledSurfaces1->Checked;
  ShowSurfaceOutlines = !FilledSurfaces1->Checked;
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  if (SaveSimDialog->FileName.Length() < 4)
  return;
	TRegistry *SarsimReg;     //Define pointer

	SarsimReg = new TRegistry;   //Allocate space and assign pointer
	SarsimReg->RootKey = HKEY_CURRENT_USER;    //Set Root key inside registry

	//Write position info
	if (SarsimReg->OpenKey("\\Software\\Sarsim2\\TRegistry\\SCRFilename", TRUE))
	  {
    	//Write last open file
    	try
		    {
    			SarsimReg->WriteString("LastFile", SaveSimDialog->FileName);
		    }
		  catch (...)
		    {
       		;  //do nothing
		    }
	  }
	SarsimReg->Free();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	TRegistry *SarsimReg;     //Define pointer
  LastFile = "";

	SarsimReg = new TRegistry;   //Allocate space and assign pointer

 	SarsimReg->RootKey = HKEY_CURRENT_USER;    //Set Root key inside registry

 	//Get last filename
  if (SarsimReg->OpenKey("\\Software\\Sarsim2\\TRegistry\\SCRFilename", FALSE))
   	{
   		//read last open file
    	try
      	{
    	  	LastFile = SarsimReg->ReadString("LastFile");
        }
      catch (...)
        {
        	; // Do nothing.
       	}
   	}
  SarsimReg->Free();

  if (LastFile.Length() > 3)
    {
      MItemLine = new TMenuItem(MainForm->MainMenu1);
      MItemLastFile = new TMenuItem(MainForm->MainMenu1);
      MItemLine->Caption = "-";
      MItemLastFile->Caption = LastFile;
      FileMenu->Add(MItemLine);
      FileMenu->Add(MItemLastFile);
      MItemLastFile->OnClick = OpenLastFileClick;
    }
  else
    {
      MItemLine = NULL;
      MItemLastFile = NULL;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Import1Click(TObject *Sender)
{
  ImportForm->ShowModal();
  UpdateObjectList();
  CalcNewSTimeArray();
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Geometry2Click(TObject *Sender)
{
  // only run simulation if targets exists
  if ((FirstTarget == NULL) && (FirstSurface == NULL))
    Application->MessageBox("No reflecting objects entered...", "Message", MB_OK);
  else if (FirstRadar == NULL)
    Application->MessageBox("Create a Radar first...", "Message", MB_OK);
  else
    {
       // make a copy of the default Simulation to SimuGraphs[i]->CurrentSim
       CopyGeometry(&(GeoForm->CurrentGeo),&DefaultGeometryValues);
     //  SimuGraphs[i]->CurrentSim.SimTYPE = Type;
     //  SimuGraphs[i]->FinishedInitializing = FALSE;
       GeoForm->Show();
       return;
    }  // end for i
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BAniClick(TObject *Sender)
{
  BStop->Enabled = true;
  BAni->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BStopClick(TObject *Sender)
{
  BAni->Enabled = true;
  BStop->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Contents1Click(TObject *Sender)
{
  Application->HelpFile = "SARSIMHLP.HLP";
  Application->HelpCommand(HELP_CONTENTS, 0);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LCoordClick(TObject *Sender)
{
  LCoord->Checked = !LCoord->Checked;
  ShowLittleCoordSystemAxes = LCoord->Checked;
  Invalidate();
}
//---------------------------------------------------------------------------
