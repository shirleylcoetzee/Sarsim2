#include "dest.h"
//---------------------------------------------------------------------------

#ifdef COMPILE_FOR_WINDOWS32
  #include <vcl\vcl.h>
  #pragma hdrstop
  #include "main.h"
#endif

#include "engine.h"
#include "objectstructdef.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "general.h"
#include "linkedlist.h"
#include "vector.h"

// small number to encounter rounding errors
const double ROUNDERROR = 1E-12;
extern bool UserAbort;
extern int CalcProgress;

// barker codes
extern int BarkerCode[7][13];
extern int BarkerCodeLength[7];

//extern char version[30];

/* for autorotation
SaveSimData line 27
SaveProgress line 60
engine.cpp 2355, 2403, 2384
(in total 5 changes)
*/

#ifdef COMPILE_FOR_WINDOWS32

//---------------------------------------------------------------------------
void EditDataDef(SDataDefinition *DataDef)
{
  // make a copy of the current Datadef to GraphForm1->DataDef
  CopyDataDef(&(GraphForm1->DataDef),(DataDef));
  GraphForm1->ShowModal();
  if (GraphForm1->ModalResult == mrOk)
    {
      // free memory of old DataDefinition
      FreeDataDef((DataDef));
      // let it point to the new DataDef
      *DataDef = GraphForm1->DataDef;
    }
  else if (GraphForm1->ModalResult == mrCancel)
    {
      // free memory allocated by copying DataDef
      FreeDataDef(&(GraphForm1->DataDef));
    }
}

//---------------------------------------------------------------------------
void FillMemo(TMemo *ThisMemo)
{
  char str[256];
  char s[81];
  struct STarget *t;
  struct SPlatform *p;
  struct SRadar *r;
  struct SSimulation *sim;
  struct SSurface *sur;
  struct SGeometry *geo;
  int i;

  ThisMemo->Lines->Clear();

  // now save data
  sprintf(str,"! Radar Simulator SARSIM II (version %s)",version);
  ThisMemo->Lines->Add(str);
  sprintf(str,"! (C)1997,1998 UCT (Radar Remote Sensing Group)");
  ThisMemo->Lines->Add(str);
  ThisMemo->Lines->Add("");
  ThisMemo->Lines->Add("");

		p=MainForm->FirstPlatform;
		while (p != NULL)
		  {
			 sprintf(str,"$PLATFORM  %s", p->Name);
             ThisMemo->Lines->Add(str);

			 if (p->Stat_Traj == 0)
				{
				  // stationary target
				  sprintf(str,"STATIONARY");
          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (m) X-Position",p->SPos[0]);
          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (m) Y-Position",p->SPos[1]);
          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (m) Z-Position",p->SPos[2]);
          ThisMemo->Lines->Add(str);
				}
			 else
				{
				  // trajectory
				  sprintf(str,"TRAJECTORY");
                  ThisMemo->Lines->Add(str);
				  WriteDataDefMemo(&(p->TrajDef),ThisMemo);
				  if (p->Align == 1)
					 sprintf(str,"ALIGNED");
				  else
					 sprintf(str,"NOT_ALIGNED");
                  ThisMemo->Lines->Add(str);

				}
			 if (p->RotType == 0)
				{
				  // fixed rotation angles
				  sprintf(str,"CONST");
          ThisMemo->Lines->Add(str);
			    sprintf(str,"%-20.6G ! (deg) X-axis Rotation (deg)",p->Rot[0]*RadToDeg);
             ThisMemo->Lines->Add(str);
			    sprintf(str,"%-20.6G ! (deg) Y-axis Rotation (deg)",p->Rot[1]*RadToDeg);
             ThisMemo->Lines->Add(str);
			    sprintf(str,"%-20.6G ! (deg) Z-axis Rotation (deg)",p->Rot[2]*RadToDeg);
             ThisMemo->Lines->Add(str);
				}
			 else
				{
				  // user-defined rotation angles
				  sprintf(str,"OTHER");
          ThisMemo->Lines->Add(str);
				  WriteDataDefMemo(&(p->RotDef),ThisMemo);
				}

			 sprintf(str,"%-20.6G ! (deg) X-axis Rotation rate (deg/s)",p->RotRate[0]*RadToDeg);
             ThisMemo->Lines->Add(str);
			 sprintf(str,"%-20.6G ! (deg) Y-axis Rotation rate (deg/s)",p->RotRate[1]*RadToDeg);
             ThisMemo->Lines->Add(str);
			 sprintf(str,"%-20.6G ! (deg) Z-axis Rotation rate (deg/s)",p->RotRate[2]*RadToDeg);
             ThisMemo->Lines->Add(str);
			 if (p->MotionErrorPos == 0)
				{
				  // use only std dev
				  sprintf(str,"STDDEV");
	              ThisMemo->Lines->Add(str);
   			      sprintf(str,"%-20.6G ! (m) X-Position standard deviation",p->PosDev[0]);
				  ThisMemo->Lines->Add(str);
   			      sprintf(str,"%-20.6G ! (m) Y-Position standard deviation",p->PosDev[1]);
				  ThisMemo->Lines->Add(str);
   			      sprintf(str,"%-20.6G ! (m) Z-Position standard deviation",p->PosDev[2]);
				  ThisMemo->Lines->Add(str);
   			    }
			 else
				{
				  // user defined
				  sprintf(str,"OTHER                       ! User defined position motion errors");
		          ThisMemo->Lines->Add(str);
   			      WriteDataDefMemo(&(p->PosDevDef),ThisMemo);
				}
			 if (p->MotionErrorRot == 0)
				{
				  // use only std dev
				  sprintf(str,"STDDEV");
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (deg) X-axis Rotation standard deviation",p->RotDev[0]*RadToDeg);
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (deg) Y-axis Rotation standard deviation",p->RotDev[1]*RadToDeg);
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (deg) Z-axis Rotation standard deviation",p->RotDev[2]*RadToDeg);
		          ThisMemo->Lines->Add(str);
				}
			 else
				{
				  // user defined
				  sprintf(str,"OTHER                       ! User defined rotation motion errors");
		          ThisMemo->Lines->Add(str);
				  WriteDataDefMemo(&(p->RotDevDef),ThisMemo);
				}

                 ThisMemo->Lines->Add("");
  			 p = p->next;                     // goto next platform
		  }


    ThisMemo->Lines->Add("");
		sur=MainForm->FirstSurface;
		while (sur != NULL)
		  {
			 sprintf(str,"$SURFACE ");
       ThisMemo->Lines->Add(str);
			 sprintf(str,"%s  ! Platform",sur->Name);
       ThisMemo->Lines->Add(str);
       for (i=0;i<3;i++)
         {
			     sprintf(str,"%-.16G, %-.16G, %-.16G  ! Point %d X,Y,Z (m)",
            sur->Tri[i][0],sur->Tri[i][1],sur->Tri[i][2],i+1);
           ThisMemo->Lines->Add(str);
         }
       ThisMemo->Lines->Add("");
			 sur = sur->next;
		  } // end while


    ThisMemo->Lines->Add("");
 		t=MainForm->FirstTarget;
		while (t != NULL)
		  {
			 sprintf(str,"$TARGET %s  ! Platform",t->Name);
	         ThisMemo->Lines->Add(str);
			 sprintf(str,"%-.16G, %-.16G, %-.16G  ! Position X,Y,Z (m)",
               t->Pos[0], t->Pos[1], t->Pos[2]);
	         ThisMemo->Lines->Add(str);
			 sprintf(str,"%-.16G, %-.16G, %-.16G  ! Position standard deviation X,Y,Z (m)",
               t->PosDev[0], t->PosDev[1], t->PosDev[2]);
	         ThisMemo->Lines->Add(str);
			 sprintf(str,"%-.16G, %-.16G  ! Radar cross section (m*m), RCS std. dev. (m*m)", t->RCS, t->RCSdev);
	         ThisMemo->Lines->Add(str);
			 if (t->ReflecType == ISOTROPIC)
				{
				  // Isotropic point target
				  sprintf(str,"ISOTROPIC");
	              ThisMemo->Lines->Add(str);
				}
			 else
				{
				  // trajectory
				  sprintf(str,"DIRECTIONAL");
		          ThisMemo->Lines->Add(str);
         		  sprintf(str,"%-.16G, %-.16G, %-.16G, %-.16G  ! Azi, Ele, std. dev. of Azi, std. dev. of Ele (deg.)",
                  t->DirAzi*RadToDeg, t->DirEle*RadToDeg, t->DirAziDev*RadToDeg, t->DirEleDev*RadToDeg);
		          ThisMemo->Lines->Add(str);
				  if (t->GainType == 0) // cos
                    {
					  sprintf(str,"COS_GAIN");
       		          ThisMemo->Lines->Add(str);
                    }
				  else
					 {
						sprintf(str,"OTHER_GAIN");
					 	ThisMemo->Lines->Add(str);
	                    WriteDataDefMemo(&(t->DataDef),ThisMemo);
					 }
				}

             ThisMemo->Lines->Add("");
			 t = t->next;
		  } // end while

    ThisMemo->Lines->Add("");
		r=MainForm->FirstRadar;
		while (r != NULL)
		  {
			 sprintf(str,"$RADAR");
             ThisMemo->Lines->Add(str);
			 sprintf(str,"%-20s ! Name of radar", r->RadarName);
			 ThisMemo->Lines->Add(str);
			 sprintf(str,"%-20s ! Platform name of radar", r->PlatformName);
			 ThisMemo->Lines->Add(str);
			 if (r->PulseType == MONO)
               {
				 sprintf(str,"MONO");
                 ThisMemo->Lines->Add(str);
               }
			 else if (r->PulseType == CHIRP)
				{
				  sprintf(str,"CHIRP");
				  ThisMemo->Lines->Add(str);
			      sprintf(str,"%-20.6G ! (GHz) Chirp bandwidth", r->ChirpBW*1E-9);
				  ThisMemo->Lines->Add(str);
				}
			 else if (r->PulseType == BARKER)
				{
				  sprintf(str,"BARKER");
				  ThisMemo->Lines->Add(str);
			      sprintf(str,"%-20d ! Barker Code", r->BarkerCode);
				  ThisMemo->Lines->Add(str);
				}
			 else // user defined pulse
				{
				  sprintf(str,"%-20s ! User defined pulse","OTHER_PULSE");
			  	  ThisMemo->Lines->Add(str);
              	  WriteDataDefMemo(&(r->PulseDef),ThisMemo);
				}
			 sprintf(str,"%-20.6G ! (ns)  zero to zero Pulse width", r->PulseWidth*1E9);
   	  	     ThisMemo->Lines->Add(str);
			 if (r->Envelope == 0)
               {
			   	  sprintf(str,"%-20s ! Rectangular envelope","RECT");
                  ThisMemo->Lines->Add(str);
               }
			 else if (r->Envelope == 1)
				{
				  sprintf(str,"%-20s ! Linear ramp envelope","LINEAR");
                  ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (ns)  Pulse rise time", r->RiseTime*1E9);
                  ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (ns)  Pulse fall time", r->FallTime*1E9);
                  ThisMemo->Lines->Add(str);
				}
			 else // user defined envelope
				{
				  sprintf(str,"%-20s ! User defined envelope","OTHER");
	              ThisMemo->Lines->Add(str);
   			      WriteDataDefMemo(&(r->EnvelopeDef),ThisMemo);
				}
			 if (r->PRFType == 0)  // constant PRI
				{
				  sprintf(str,"%-20s ! Constant PRI","CONSTANT");
	              ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (Hz)  Pulse repetition frequency", r->PRF);
	              ThisMemo->Lines->Add(str);
				}
			 else // user defined PRI
				{
				  sprintf(str,"%-20s ! User defined PRIs","OTHER");
	              ThisMemo->Lines->Add(str);
				  WriteDataDefMemo(&(r->PRFDef),ThisMemo);
				}
			 if (r->FreqType == 0) // single frequency
				{
				  sprintf(str,"%-20s ! Constant frequency","SINGLE");
	              ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (GHz)  Center frequency", r->SingleFreq*1E-9);
	              ThisMemo->Lines->Add(str);
				}
			 else if (r->FreqType == 1)   // stepped frequency
				{
				  sprintf(str,"%-20s ! Stepped frequency","STEPPED");
	              ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (GHz) Start frequency", r->StartFreq*1E-9);
	              ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (GHz) Frequency step size", r->StepSize*1E-9);
	              ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.0d ! Number of frequency steps", r->FreqSteps);
	              ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.0d ! Pulses per frequency", r->PulsesPerFreq);
	              ThisMemo->Lines->Add(str);
				}
			 else // user defined frequencies
				{
				  sprintf(str,"%-20s ! User defined frequencies","OTHER");
	              ThisMemo->Lines->Add(str);
				  WriteDataDefMemo(&(r->FreqDef),ThisMemo);
				}
			 sprintf(str,"%-20.6G ! (kW)  Power output" , r->PowerOutput*1E-3);
             ThisMemo->Lines->Add(str);
			 sprintf(str,"%-20.6G ! (dB)  Total system losses", FacTodB(r->Losses));
             ThisMemo->Lines->Add(str);
			 sprintf(str,"%-20.6G ! (K)   Noise temperature", r->NoiseTemp);
             ThisMemo->Lines->Add(str);
			 if (r->AntennaGainTypeT == 0) // Isotropic
				{
				  sprintf(str,"%-20s ! Isotropic transmitter antenna","ISOTROPIC");
		          ThisMemo->Lines->Add(str);
				}
			 else if (r->AntennaGainTypeT == 1)   // simple (sinx/x)
				{
				  sprintf(str,"%-20s ! Simple sin(x)/x transmitter antenna","SINX");
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (deg) Elevation beam width", r->ElevBeamWidthT*RadToDeg);
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (deg) Azimuth beam width", r->AziBeamWidthT*RadToDeg);
		          ThisMemo->Lines->Add(str);
				}
			 else // user defined antenna gain
				{
				  sprintf(str,"%-20s ! User defined transmitter antenna","OTHER");
		          ThisMemo->Lines->Add(str);
				  WriteDataDefMemo(&(r->AntennaTDef),ThisMemo);
				}
			 if (r->AntennaGainTypeR == 0) // Isotropic
				{
				  sprintf(str,"%-20s ! Isotropic receiver antenna","ISOTROPIC");
		          ThisMemo->Lines->Add(str);
				}
			 else if (r->AntennaGainTypeR == 1)   // simple (sinx/x)
				{
				  sprintf(str,"%-20s ! Simple sin(x)/x receiver antenna","SIN");
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (deg) Elevation beam width", r->ElevBeamWidthR*RadToDeg);
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (deg) Azimuth beam width", r->AziBeamWidthR*RadToDeg);
		          ThisMemo->Lines->Add(str);
				}
			 else if (r->AntennaGainTypeR == 2)   // same as transmitter
				{
				  sprintf(str,"%-20s ! Same as transmitter antenna","SAME");
		          ThisMemo->Lines->Add(str);
				}
			 else // user defined antenna gain
				{
				  sprintf(str,"%-20s ! User defined receiver antenna","OTHER");
		          ThisMemo->Lines->Add(str);
				  WriteDataDefMemo(&(r->AntennaRDef),ThisMemo);
				}
			 if (r->BeamDirection == 0) // fixed
				{
				  sprintf(str,"%-20s ! Fixed antenna direction","FIXED");
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (deg) Beam-direction - Elevation", r->ElevationAngleF*RadToDeg);
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (deg) Beam-direction - Azimuth", r->AzimuthAngleF*RadToDeg);
		          ThisMemo->Lines->Add(str);
				}
			 else if (r->BeamDirection == 1)   // rotating antenna
				{
				  sprintf(str,"%-20s ! Rotating antenna","ROTATING");
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (deg) Beam-direction - Elevation", r->ElevationAngleR*RadToDeg);
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (deg/s) Rotation rate", r->RotationRate*RadToDeg);
		          ThisMemo->Lines->Add(str);
				}
			 else if (r->BeamDirection == 2)   // spot mode
				{
				  sprintf(str,"%-20s ! Spot mode antenna","SPOT");
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (m) Spot X-Position",r->Spot[0]);
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (m) Spot Y-Position",r->Spot[1]);
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (m) Spot Z-Position",r->Spot[2]);
		          ThisMemo->Lines->Add(str);
				}
			 else // other direction
				{
				  sprintf(str,"%-20s ! User defined direction","OTHER");
		          ThisMemo->Lines->Add(str);
				  WriteDataDefMemo(&(r->DirectionTDef),ThisMemo);
				}
			 if (r->MatchedFilterW == 0) // rect
				{
				  sprintf(str,"%-20s ! Rectangular MF window","RECT");
		          ThisMemo->Lines->Add(str);
				}
			 else if (r->MatchedFilterW == 1)   // hanning
				{
				  sprintf(str,"%-20s ! Hanning MF window","HANNING");
		          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! Hanning constant", r->HanningC);
		          ThisMemo->Lines->Add(str);
				}
			 else if (r->MatchedFilterW == 2)   // bartlett
				{
				  sprintf(str,"%-20s ! Bartlett MF window","BARTLETT");
		          ThisMemo->Lines->Add(str);
				}
			 else if (r->MatchedFilterW == 3)   // hamming
				{
				  sprintf(str,"%-20s ! Hamming MF window","HAMMING");
		          ThisMemo->Lines->Add(str);
				}
			 else // other
				{
				  sprintf(str,"%-20s ! User defined MF window","OTHER");
		          ThisMemo->Lines->Add(str);
				  WriteDataDefMemo(&(r->MFDef),ThisMemo);
				}

			 if (r->ApplyAGC) // sensitivity time control
				{
				  sprintf(str,"%-20s ! apply sensitivity time control","APPLY_STC");
		          ThisMemo->Lines->Add(str);
				  if (r->AGCType == 0)
                    {
					   sprintf(str,"%-20s ! multiply by R^4","1OVERR4");
	     	           ThisMemo->Lines->Add(str);
                    }
				  else
					{
					  sprintf(str,"%-20s ! User define STC function","OTHER");
 	     	          ThisMemo->Lines->Add(str);
					  WriteDataDefMemo(&(r->AGCDef),ThisMemo);
					}
				}
			 else // no STC
				{
				  sprintf(str,"%-20s ! no STC","NO_STC");
    	          ThisMemo->Lines->Add(str);
				}
			     ThisMemo->Lines->Add("");
			     ThisMemo->Lines->Add("");

				  r = r->next;
				} // end while
			 sim=MainForm->FirstSimulation;
			 while (sim != NULL)
				{
				  sprintf(str,"$SIMULATION");
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20s ! Name of radar", sim->RadarName);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (m) Slant range start", sim->SlantStart);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (m) Slant range end", sim->SlantEnd);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (s) Azimuth range start", sim->AzimuthStart);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (s) Azimuth range end", sim->AzimuthEnd);
    	          ThisMemo->Lines->Add(str);

				  sprintf(str,"%-20d ! A/D bit accuracy", sim->A2Dbits);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (GHz) Sample frequency", sim->SampleFreq*1E-9);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.9G ! (mV) Least Significant Bit value", sim->LSBvalue*1E3);
    	          ThisMemo->Lines->Add(str);
				  if (sim->FileType == 0) sprintf(s,"ASCII");
				  else if (sim->FileType == 1) sprintf(s,"BINARY");
				  else if (sim->FileType == 2) sprintf(s,"BRACKET");
				  sprintf(str,"%-20s ! Save file format", s);
    	          ThisMemo->Lines->Add(str);
				  if (sim->SimTYPE == 0) sprintf(s,"RAW");
				  else if (sim->SimTYPE == 1) sprintf(s,"MATCHED_FILTER");
				  else if (sim->SimTYPE == 2) sprintf(s,"SRP");
				  sprintf(str,"%-20s ! Processing", s);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20s ! File Name of output file", sim->FileName);
    	          ThisMemo->Lines->Add(str);

				  long PointsAzi, PointsSlant;
				  FindPixelAziSlant(MainForm->FirstRadar,&PointsAzi, &PointsSlant, sim);
				  sprintf(str,"%-20s ! Size : %d (Azimuth) x %d (Slant Range) points",
				  "",PointsAzi,PointsSlant);
    	          ThisMemo->Lines->Add(str);
				  if (sim->PTPos == TRUE) sprintf(s,"CENTRE");
				  else sprintf(s,"BEGIN");
				  sprintf(str,"%-20s ! Point Target position relative to pulse", s);
    	          ThisMemo->Lines->Add(str);
			      ThisMemo->Lines->Add("");
			      ThisMemo->Lines->Add("");

				  sim = sim->next;
				} // end while

			 geo=MainForm->FirstGeometry;
			 while (geo != NULL)
				{
				  sprintf(str,"$GEOMETRY");
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20s ! Name of radar", geo->RadarName);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (m) Slant range start", geo->SlantStart);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (m) Slant range end", geo->SlantEnd);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (s) Azimuth range start", geo->AzimuthStart);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (s) Azimuth range end", geo->AzimuthEnd);
    	          ThisMemo->Lines->Add(str);

				  if (geo->FileType == 0) sprintf(s,"SPACE");
				  else sprintf(s,"COMMA");
			//	  sprintf(str,"%-20s ! Separation character", s);
    		  sprintf(str,"%-20s", s);
    	          ThisMemo->Lines->Add(str);

				  sprintf(str,"%-20d ! Info included in file", geo->ShowVar);
    	          ThisMemo->Lines->Add(str);

				  sprintf(str,"%-20s ! File Name of output file", geo->FileName);
    	          ThisMemo->Lines->Add(str);
		      ThisMemo->Lines->Add("");
		      ThisMemo->Lines->Add("");

				  geo = geo->next;
				} // end while
}
//---------------------------------------------------------------------------
void WriteDataDefMemo(struct SDataDefinition *DataDef,TMemo *ThisMemo)
{
  long i,j;
  char str[256];

  for (i=0;i<DataDef->DataTypes;i++)
    {
      if (DataDef->IntMethod[i] == CUBIC)
         sprintf(str,"%-20s ! Interpolation method for %s vs %s","CUBIC",
           DataDef->YAxis[i].Name, DataDef->XAxis[i].Name);
      else if (DataDef->IntMethod[i] == FILTER)
         sprintf(str,"%-20s ! Interpolation method for %s vs %s","MFILTER",
           DataDef->YAxis[i].Name, DataDef->XAxis[i].Name);
      else
        sprintf(str,"%-20s ! Interpolation method for %s vs %s","LINEAR",
          DataDef->YAxis[i].Name, DataDef->XAxis[i].Name);
      ThisMemo->Lines->Add(str);
    }
  if (DataDef->Source == ID_FILE)
	 {
		sprintf(str,"FILE  %s", DataDef->FileName);
        ThisMemo->Lines->Add(str);
	 }
  else if (DataDef->Source == ID_INLINE)
	 {
		sprintf(str,"INLINE");
        ThisMemo->Lines->Add(str);
		for (i=0;i<DataDef->DataTypes;i++)
		  {
			 sprintf(str,"%d                    ! Number of samples for %s vs %s",
               DataDef->NoP[i],DataDef->YAxis[i].Name, DataDef->XAxis[i].Name);
             ThisMemo->Lines->Add(str);
			 for (j=0;j<DataDef->NoP[i];j++)
				{
				  sprintf(str,"%-.15G, %-.15G",DataDef->XAxis[i].DataArray[j],
						DataDef->YAxis[i].DataArray[j]);
				  if (j < (DataDef->NoP[i]-1))
					 strcat(str,"  ");
		          ThisMemo->Lines->Add(str);
				}
		  }
	 }
}
//-------------------------------------------------------------------------//
// DrawAs     -  Draw Data as coloured pixels or lines
// LGraphBM   -  If data is drawn as lines return result bitmap in LGraphBM
// BM         -  If data is drawn as pixels it will be in this structure
// Data       -  actual data to be drawn
// PulseSendTime -  Times when pulses are sent
// PulseNo       -  Number of pulses in array
// SamplePoints  -  Size of array in x-direction
// PixelY        -  Size of array in y-direction (not necessarily equal to PulseNo)
// CurrentSim    -  CurrentSim structure data for range etc.
// SBContrast, SBBright, SBSat  -  Display variables
// ColPal        -  RGB Color palette
// ShowdB        -  normal or dB scale
void DrawArray(int DrawAs, Graphics::TBitmap *LGraphBM, class TBitM *BM,
               double **Data, double *PulseSendTime, unsigned long PulseNo,
               unsigned long SamplePoints, int PixelY,
               struct SSimulation *CurrentSim, int SBContrast, int SBBright,
               int SBSat,unsigned char ColPal[NCOLOR][3], bool ShowdB)
{
  unsigned long PNo;                   // loop variables for pulses
  unsigned long Sample;                // loop variable for samples (in azimuth)
  double PMax,PMin,temp,Range,Scale,D;
  unsigned long AziPix,Offset;
  unsigned char Val;
  int PulseStep;
  unsigned long CSlant;
  double ScaleGraph;

  // find min and max values
  PMax = MIN_DOUBLE;
  PMin = MAX_DOUBLE;
  for (PNo=0;PNo<PulseNo;PNo++)
    {
      for (Sample=0; Sample<SamplePoints; Sample++)
		    {
          temp = GetValue(CurrentSim->Display,Data[PNo][2*Sample],Data[PNo][2*Sample+1]);
          if (temp > PMax) PMax = temp;
          if (temp < PMin) PMin = temp;
        }
    }
  Range = PMax - PMin;
  if (Range <= 0) Range = MIN_FLOAT;  // let's make sure
  if (ShowdB) // dB scale
    {
      Range = -DYNAMIC_RANGE; // let's show a dynamic range of x dB
    }
  // calculate scaling factor
  switch (CurrentSim->Display)
    {
      case REAL:
      case IMAGINARY:
      case MAGNITUDE:
           Scale = (NCOLOR - 1)/Range;
           break;
      case PHASE:
           Scale = (NCOLOR - 1)/(2*PI); // let's use full 2 Pi for phase
           break;
    }
  if ((DrawAs == 0) || ((DrawAs == 3) && ((double(PixelY)/double(PulseNo))<(double)PixStep))) // colour pixels
    {
      // Clear the DIB
      memset(BM->pDIBBase, 0, 3*BM->DIBWidth*BM->DIBHeight);
      double PixPerPulse = double(PixelY) / (CurrentSim->AzimuthEnd-CurrentSim->AzimuthStart);

      for (PNo=0;PNo<PulseNo;PNo++)
        {
		      // find equivalent screen pixel value in y direction for Time
		      AziPix = (unsigned long) round( (PulseSendTime[PNo] - CurrentSim->AzimuthStart)*
                          PixPerPulse );
          if ((AziPix >= 0) && (AziPix < (unsigned long)PixelY))
  		    for (Sample=0; Sample<SamplePoints; Sample++)
		        {
  	 		      // Val = actual value at that instance
              if (!ShowdB) //normal scale
                Val = (GetValue(CurrentSim->Display,Data[PNo][2*Sample],
                          Data[PNo][2*Sample+1])-PMin)*Scale;
              else                // dB Scale
                {
                  D = FindValuedB(GetValue(CurrentSim->Display,Data[PNo][2*Sample],
                      Data[PNo][2*Sample+1]), PMax);
                  Val = (NCOLOR - 1) - D * Scale;
                }
              Offset = (Sample + AziPix * BM->DIBWidth)*3;
              BM->pDIBBase[Offset] = ColPal[Val][RED];
              BM->pDIBBase[Offset+1] = ColPal[Val][GREEN];
              BM->pDIBBase[Offset+2] = ColPal[Val][BLUE];
		        }  // end for sample
	      }  // end for PNo
    }
  else
    {
      // Clear the DIB to white
      LGraphBM->Width = SamplePoints;
      LGraphBM->Height = PixelY;
      TRect ClearArea;
      SetRect(&ClearArea,0, 0, SamplePoints, PixelY);
      LGraphBM->Canvas->Brush->Color = clWhite;
      LGraphBM->Canvas->FillRect(ClearArea);
      LGraphBM->Canvas->Pen->Color = clRed;

      if (DrawAs == 1) // cuts in range
        {
          double SlantPD;
          // step size in slant (x) range for which a graph will be drawn
          double SlantStep = ceil(((CurrentSim->SlantEnd-CurrentSim->SlantStart) * PixStep)/double(SamplePoints));
          if (!ShowdB) //normal scale
            ScaleGraph = (double(PixStep)*0.75) / (2*Max(fabs(PMax), fabs(PMin)));
          else
            ScaleGraph = (double(PixStep)*0.9) / (DYNAMIC_RANGE);
          // Pixel per pulse in azimuth direction
          double PixPerPulse = double(PixelY) / (CurrentSim->AzimuthEnd-CurrentSim->AzimuthStart);
          // we don't want to go below pixel level
          if (SlantStep < 1) SlantStep = 1;
          for (SlantPD = round(CurrentSim->SlantStart); SlantPD <
            CurrentSim->SlantEnd; SlantPD += SlantStep)
            {
              // CSlant = corresponding pixel position
              CSlant = (unsigned long)round(((SlantPD - CurrentSim->SlantStart) /
              (CurrentSim->SlantEnd-CurrentSim->SlantStart))* double(SamplePoints));
              if ((CSlant >= 0) && (CSlant < SamplePoints))
                {
                  LGraphBM->Canvas->Pen->Color = clGray;
                  LGraphBM->Canvas->MoveTo(CSlant,0);
                  LGraphBM->Canvas->LineTo(CSlant,PixelY);
                  LGraphBM->Canvas->Pen->Color = clRed;
                  for (PNo=0;PNo<PulseNo;PNo++)
                    {
	         	          // find equivalent screen pixel value in y direction for Time
		                  AziPix = PixelY - 1 - (unsigned long) round( (PulseSendTime[PNo] -
                          CurrentSim->AzimuthStart)*PixPerPulse );
                      if (!ShowdB) //normal scale
                        D = GetValue(CurrentSim->Display, Data[PNo][CSlant*2],
                         Data[PNo][CSlant*2+1])*ScaleGraph;
                      else                // dB Scale
                        {
                          D = FindValuedB(GetValue(CurrentSim->Display, Data[PNo][CSlant*2],
                          Data[PNo][CSlant*2+1]), PMax)*ScaleGraph;
                        }
                      if (PNo == 0)
                        LGraphBM->Canvas->MoveTo(CSlant+D,AziPix);
                      else
                        LGraphBM->Canvas->LineTo(CSlant+D,AziPix);
                    }
                }
            }  // end for
        }
      else if ((DrawAs == 2) || (DrawAs == 3)) // cuts in azimuth
        {
          // determine how many pulses should be shown
          if (double(PulseNo * PixStep) > double(PixelY))
            PulseStep = double(PulseNo)*double(PixStep)/double(PixelY);
          else
            PulseStep = 1;
          // Pixel per pulse in azimuth direction
          double PixPerPulse = double(PixelY) / (CurrentSim->AzimuthEnd-CurrentSim->AzimuthStart);
          if (!ShowdB)
            {
              if (Max(fabs(PMax), fabs(PMin)) != 0)
                ScaleGraph = ((double(PixelY)*double(PulseStep)/double(PulseNo))*0.75) /
                 (2*Max(fabs(PMax), fabs(PMin)));
              else
                ScaleGraph = 0;
            }    
          else
            ScaleGraph = ((double(PixelY)*double(PulseStep)/double(PulseNo))*0.9) /
             (DYNAMIC_RANGE);

          for (PNo=0;PNo<PulseNo;PNo+=PulseStep)
	          {
		          // find equivalent screen pixel value in y direction for Time
		          AziPix = PixelY - 1 - (unsigned long) round( (PulseSendTime[PNo] -
                CurrentSim->AzimuthStart)*PixPerPulse );
              LGraphBM->Canvas->Pen->Color = clGray;
              LGraphBM->Canvas->MoveTo(0,AziPix);
              LGraphBM->Canvas->LineTo(SamplePoints, AziPix);
              LGraphBM->Canvas->Pen->Color = clRed;
  		        for (Sample=0; Sample<SamplePoints; Sample++)
		            {
  	 		          // D = actual value at that instance
                  if (!ShowdB) //normal scale
                     D = GetValue(CurrentSim->Display,Data[PNo][2*Sample],
                     Data[PNo][2*Sample+1])*ScaleGraph;
	                else                // dB Scale
                    {
                      D = FindValuedB(GetValue(CurrentSim->Display,Data[PNo][2*Sample],
                     Data[PNo][2*Sample+1]), PMax)*ScaleGraph;
                    }

			            // don't draw anything for the first time
			            if (Sample == 0)
	  		            LGraphBM->Canvas->MoveTo(Sample,AziPix - D);
			            else
			 	            LGraphBM->Canvas->LineTo(Sample,AziPix - D);
		            }  // end for sample
	          }  // end for PNo
        } // end else if ...
    }
}
#endif
//-------------------------------------------------------------------------//
// Function FindPulseFreq                                                  //
// Returns the frequency (in Hz) of the given pulse
// PNo, first pulse is at PNo = 0
//-------------------------------------------------------------------------//
double FindPulseFreq(long PNo, struct SRadar *R)
{
  // note that F(PNo=0) = StartF etc. i.e. PNo should start with 0
  if (R->FreqType == 0) // constant
	 return R->SingleFreq;
  else if (R->FreqType == 1) // stepped
	 {
		// sometimes rounding errors creep in e.g. 7.0000 is internally
		// stored as 6.99999etc which 'floors' to 6 instead of the wanted 7
       if (PNo < 0)
         {
           // if PNo < 0 the pulse-frequency would decrease with increasing
           // PNo, therefore correct for that
           double temp = fmod((double(PNo)/double(R->FreqSteps*R->PulsesPerFreq)),1);
           if (temp < 0) temp += 1;
		     return (( R->StartFreq +  floor( temp *
					 (double)R->FreqSteps + ROUNDERROR ) * R->StepSize
				  ));
         }
       else
		   return (( R->StartFreq +
			  floor( fmod((double(PNo)/double(R->FreqSteps*R->PulsesPerFreq)),1) *
					 (double)R->FreqSteps + ROUNDERROR ) * R->StepSize
				  ));
	 }
  else // other
	 {
		 // if PNo > NoP take the modulus
		 long ModPNo = (long)fmod(double(PNo),double((R->FreqDef).NoP[0]));
		 if (ModPNo < 0)
			return ( (R->FreqDef).YAxis[0].DataArray[ModPNo+(R->FreqDef).NoP[0]]*1E9 );
		 else
			return ( (R->FreqDef).YAxis[0].DataArray[ModPNo]*1E9 );

	 }
}
//-------------------------------------------------------------------------//
long FindPulseForTime(double Time, struct SRadar *R)
{
  if (R->PRFType == 0)
	 {
		// constant PRF
		return((long)ceil(Time * R->PRF));
	 }
  else
	 {
		long i,Pulse;
		// user defined PRI
		double SumPRI=0;  // sum of all defined PRI's
		// find sum of all PRI's defined (i.e. PRI0+PRI1+PRI2 etc.)
		for (i=0;i<(R->PRFDef).NoP[0];i++)
		  SumPRI += (R->PRFDef).YAxis[0].DataArray[i];
		// estimate what number the first pulse will have
		Pulse = (long)((floor((Time / SumPRI)+ROUNDERROR)*(R->PRFDef).NoP[0])-1);
		// and now find the exact one
		while (FindPulseSendTime(Pulse,R) < Time)
		  Pulse++;
		return Pulse;
	 }
}
//-------------------------------------------------------------------------//
void FindPulsesInRange(double TimeStart, double TimeEnd, struct SRadar *R,
							  long *FirstPulse, long *LastPulse)
{
  if (R->PRFType == 0)
	 {
		// constant PRF
		*FirstPulse = (long)ceil(TimeStart * R->PRF);
		*LastPulse = (long)floor(TimeEnd * R->PRF);
	 }
  else
	 {
		long i;
		// user defined PRI
		double SumPRI=0;  // sum of all defined PRI's
		// find sum of all PRI's defined (i.e. PRI0+PRI1+PRI2 etc.)
		for (i=0;i<(R->PRFDef).NoP[0];i++)
		  SumPRI += (R->PRFDef).YAxis[0].DataArray[i];
		// estimate what number the first pulse will have
		*FirstPulse = (long)((floor((TimeStart / SumPRI)+ROUNDERROR)*(R->PRFDef).NoP[0])-1);
		// and now find the exact one
		while (FindPulseSendTime(*FirstPulse,R) < TimeStart)
		  (*FirstPulse)++;
		*LastPulse = (long)((floor((TimeEnd / SumPRI)+ROUNDERROR)*(R->PRFDef).NoP[0])-1);
		while (FindPulseSendTime(*LastPulse,R) < TimeEnd)
		  (*LastPulse)++;
		// overshot by one, so subtract one again
		if (*LastPulse > 0) (*LastPulse)--;
	 }
}

//-------------------------------------------------------------------------//
// Function FindPulseSendTime                                                  //
// Returns the time (in seconds) at which the given pulse
// is sent (i.e. the beginning of the pulse)
// Pulse 0 is always sent at time = 0
//-------------------------------------------------------------------------//
double FindPulseSendTime(long PulseNo, struct SRadar *R)
{
  if (R->PRFType == 0)
	 {
		// constant PRF
		return (double(PulseNo)/R->PRF);
	 }
  else
	 {
		long i,FracPRINo;
		double frac, ipart, FracPRI;
		double SumPRI=0;  // sum of all defined PRI's
		// find sum of all PRI's defined (i.e. PRI0+PRI1+PRI2 etc.)
		for (i=0;i<(R->PRFDef).NoP[0];i++)
		  SumPRI += (R->PRFDef).YAxis[0].DataArray[i];
		// find out how many complete PRI cycles there are in the given
		// PulseNo (1 cycle = sum of all defined PRI's)
		frac = modf(double(PulseNo)/double((R->PRFDef).NoP[0]), &ipart);
		// calculate the number of remaining PRI's
		FracPRINo = long(round(frac * double((R->PRFDef).NoP[0])));
		// if PulseNo <0 goto the next lower integral time (ipart -= 1)
		// and add positive PRI's from there
		if (FracPRINo < 0)
		  {
			 FracPRINo += (R->PRFDef).NoP[0];
			 ipart -= 1;
		  }
		FracPRI = 0;
		for (i=0;i<FracPRINo;i++)
		  FracPRI += (R->PRFDef).YAxis[0].DataArray[i];
		return (ipart * SumPRI + FracPRI);
	 }
}



//-------------------------------------------------------------------------//
// Function FindPlatformPosition                                           //
//                                                                         //
// The following procedure calculates the position of platform p at        //
// a given time.                                                           //
//-------------------------------------------------------------------------//
void FindPlatformPosition(struct SPlatform *p, double time, double Pos[3])
{
  int i;
  // case stationary
  if (p->Stat_Traj == 0)
	 {
		for (i=0;i<3;i++)
		  Pos[i] = p->SPos[i];
	 }
  else
  // case of trajectory
	 {
		for (i=0;i<3;i++)
		  Pos[i] = Interpolate((p->TrajDef).NoP[i], (p->TrajDef).XAxis[i].DataArray,
                             (p->TrajDef).YAxis[i].DataArray,
									  (p->TrajDef).Coeff[i], time,(p->TrajDef).IntMethod[i]);
	 }
  // now add the motion errors
  if (p->MotionErrorPos == 0)
	 {
	 	for (i=0;i<3;i++)
		  Pos[i] +=  Gaussian(0,p->PosDev[i], FALSE);
	 }
  else
	 {
		//temp - ??
	 }
}

//-------------------------------------------------------------------------//
// Function FindTargetPosition                                           //
//                                                                         //
// The following procedure calculates the position of platform p at        //
// a given time.                                                           //
//-------------------------------------------------------------------------//
void FindTargetPosition(struct STarget *t, double Pos[3])
{
  int i;
  for (i=0;i<3;i++)
    Pos[i] = t->Pos[i] + Gaussian(0,t->PosDev[i], FALSE);
}

//-------------------------------------------------------------------------//
// Function FindTargetDir                                                //
//                                                                         //
// The following procedure calculates the direction into which the         //
// target is pointing (directional target).                                //
//-------------------------------------------------------------------------//
void FindTargetDir(STarget *t, double *AziAng, double *EleAng)
{
  // calculate direction with standard deviation
 //temp
//  *AziAng = t->DirAzi + (t->DirAziDev * GaussDist(&SEED));
//  *EleAng = t->DirEle + (t->DirEleDev * GaussDist(&SEED));
  *AziAng = t->DirAzi;
  *EleAng = t->DirEle;
}
//-------------------------------------------------------------------------//
// Function FindSurfaceNormal                                              //
void FindSurfaceNormal(SSurface *s, double *AziAng, double *EleAng)
{
  int i;
  double v1[3],v2[3],temp[3];
  *AziAng = 0;
  *EleAng = 0;
  // create 'spanning' vectors
  for (i=0;i<3;i++)
    {
      v1[i] = s->Tri[1][i] - s->Tri[0][i];        // vector from 0 to 1
      v2[i] = s->Tri[2][i] - s->Tri[0][i];        // vector from 0 to 2
    }
  // calculate normal vector (not normalized to 1)
  CrossP(v1,v2,temp);
  // calculate triangle area
//  Area = FindMag(temp)*0.5;
//  if (Area == 0) return;  // no area -> return
  *AziAng = AziAngle(temp);
  *EleAng = ElevAngle(temp);
}

//-------------------------------------------------------------------------//
// Function FindPlatformVelocity                                           //
//                                                                         //
// The following procedure calculates the velocity of a platform p at      //
// a given time.                                                           //
//-------------------------------------------------------------------------//
void FindPlatformVelocity(struct SPlatform *p, double time, double Vel[3])
{
  double Pos1[3],Pos2[3];
  // find positions a ms before and after
  FindPlatformPosition(p, time-0.001 , Pos1);
  FindPlatformPosition(p, time+0.001 , Pos2);
  // velocity is change of position divided by time...
  Vel[0] = (Pos2[0]-Pos1[0])/0.002;
  Vel[1] = (Pos2[1]-Pos1[1])/0.002;
  Vel[2] = (Pos2[2]-Pos1[2])/0.002;
}

//-------------------------------------------------------------------------//
// Function FindPlatformRotation                                           //
//                                                                         //
// The following procedure calculates the rotation of platform p at        //
// a given time.                                                           //
//-------------------------------------------------------------------------//
void FindPlatformRotation(struct SPlatform *p, double time, double Rot[3])
{
  int i;

  if (p->RotType == ROT_FIXED)  // fixed rotation angles
    {
      // copy rotation angles
      for (i=0;i<3;i++)
	      Rot[i] = p->Rot[i] + p->RotRate[i]*time;
    }
  else // user defined rotation angles
    {
      for (i=0;i<3;i++)
	      Rot[i] = p->RotRate[i]*time + Interpolate((p->RotDef).NoP[i],
										  (p->RotDef).XAxis[i].DataArray,
										  (p->RotDef).YAxis[i].DataArray,
										  (p->RotDef).Coeff[i], time,(p->RotDef).IntMethod[i])*DegToRad;
    }

  // now add the motion errors
  for (i=0;i<3;i++)
    {
		  Rot[i] +=  Gaussian(0,p->RotDev[i], FALSE);
    }

//	 Rot[i] += p->RotDev[i] * GaussDist(&SEED);
  // should the platform be aligned to the path ?
  if (p->Align == 1)
	 {
		double PFVel[3];
		FindPlatformVelocity(p, time, PFVel);
		Rot[1] += ElevAngle(PFVel);
		// subtract 90 deg. because azimuth is defined in respect
		// to the y-axis
		Rot[2] += (AziAngle(PFVel)-PIOver2);
	 }
}
//-------------------------------------------------------------------------//
void FindAntennaDir(struct SRadar *Radar, double Time, double Pos[3], double Rot[3], double *ADirAzi,
						  double *ADirEle)
{
  // find direction into which antenna is pointing
  if (Radar->BeamDirection == DIR_FIX) //fixed
	 {
		*ADirAzi = Radar->AzimuthAngleF + Rot[2];// + PI*0.5;
		*ADirEle = Radar->ElevationAngleF + Rot[1];
		return;
	 }
  else if (Radar->BeamDirection == DIR_CONSTROT) // constant rotation
	 {
		*ADirAzi = Radar->RotationRate * Time + Rot[2];
		*ADirEle = Radar->ElevationAngleR + Rot[1];
		return;
	 }
  else if (Radar->BeamDirection == DIR_SPOT) // spot mode
	 {
		double RadarToSpotVec[3];
		int i;
		// find vector from radar to spot
		for (i=0;i<3;i++)
		  RadarToSpotVec[i] = Radar->Spot[i] - Pos[i];
		*ADirAzi = AziAngle(RadarToSpotVec);
		*ADirEle = ElevAngle(RadarToSpotVec);
		return;
	 }
  else if (Radar->BeamDirection == DIR_OTHER) // other
	 {
		double RadarToSpotVec[3];
		int i;
		// find vector from radar to spot at given Time
		for (i=0;i<3;i++)
		  RadarToSpotVec[i] = ( Interpolate((Radar->DirectionTDef).NoP[i],
										  (Radar->DirectionTDef).XAxis[i].DataArray,
										  (Radar->DirectionTDef).YAxis[i].DataArray,
										  (Radar->DirectionTDef).Coeff[i], Time,(Radar->DirectionTDef).IntMethod[i])
									  ) - Pos[i];
		*ADirAzi = AziAngle(RadarToSpotVec);
		*ADirEle = ElevAngle(RadarToSpotVec);
		return;
	 }
}
//-------------------------------------------------------------------------//
void FindPixelAziSlant(struct SRadar *FirstRadar, long *PointsAzi, long *PointsSlant,
							  struct SSimulation *sim)
{
  // FastTime(seconds) = slant range in time
  double FastTimeStart = sim->SlantStart / LIGHT_SPEED;
  double FastTimeEnd = sim->SlantEnd / LIGHT_SPEED;
  *PointsSlant = (long)(round(2*sim->SampleFreq*(FastTimeEnd-FastTimeStart)));
  // calculate number of pulses
  long FirstPulse, LastPulse;
  int PFN;
  struct SRadar *R;
  if (!FindRadar(sim->RadarName, &R, &PFN, FirstRadar))
    { return; } //temp
  FindPulsesInRange(sim->AzimuthStart, sim->AzimuthEnd,R,
	 &FirstPulse,&LastPulse);
  *PointsAzi = (LastPulse - FirstPulse) + 1;
  if (*PointsAzi <= 0) *PointsAzi = long(1);
}
//int kw=0;
//-------------------------------------------------------------------------//
void CalcOnePulse(double *OnePulse, long SamplePoints, double SlantStartTime,
						double SecPerSample,long TargetNo, double *PulseFreq,
						double **RangeDelay, double **TargetRadialVel,
						double **ReturnAmp,long PNo, SRadar *r, double PulseCenter,
						double DelaySlope, double *MaxMagOfPulse, double ***SurfP,
            long SurfaceNo )
{
  long CurrentSample,i, TNo;
  double t, time,RangeInTime,temp0,temp3,phase;
  double RootOfNoisePower,Amp;
  double Cycle;


  ***SurfP = ***SurfP;
  SurfaceNo = SurfaceNo; // avoid warnings
#ifdef WITH_SURFACE_ALGORITHM
  double Area,RangeInM,deltaRangeInM,RetAmp,GainFactor;
  Vec Tri[3] ;
  double PPhase,CP[3],DistToC;//,DistToC2;
  long SNo,j;
  double RangeTime, PulseTime;
  long PulseSample;
  // compute return gain factor independent of time
  GainFactor = ((LIGHT_SPEED/r->StartFreq)*sqrt(r->PowerOutput)) /
			((4*PI)*sqrt(4*PI)*sqrt(r->Losses));
#endif


  // calculate noise amplitude
  if ((r->PulseType == MONO) || (r->PulseType == BARKER))
    RootOfNoisePower = sqrt(BOLTZMANN_CONSTANT*r->NoiseTemp*(1/r->PulseWidth));
  else if (r->PulseType == CHIRP)
    RootOfNoisePower = sqrt(BOLTZMANN_CONSTANT*r->NoiseTemp*r->ChirpBW);
  else if (r->PulseType == OTHER_PULSE)
    RootOfNoisePower = 0;
  else  RootOfNoisePower = 0;

      *MaxMagOfPulse = 0;
		CurrentSample=0;
		// set array to zero
		for (i=0;i<SamplePoints*2;i++)
		  OnePulse[i] = 0;

  // for every sampling instance t
  // (note it is necessary to go back 1/2 a pulsewidth in time)
		for (t=SlantStartTime;CurrentSample<SamplePoints;t+=SecPerSample)
		  {

#ifdef WITH_SURFACE_ALGORITHM
// added for surfaces -start
       RangeInM = t*LIGHT_SPEED;
       deltaRangeInM = SecPerSample*LIGHT_SPEED;
			 // for all surfaces
			 for (SNo=0;SNo<SurfaceNo;SNo++)
				{
          for (j=0;j<3;j++)  // for all 3 coords
              CP[j] = 0;
          for (i=0;i<3;i++)    // for all 3 points on the triangle
            for (j=0;j<3;j++)  // for all 3 coords
              {
                Tri[i][j] = SurfP[SNo][PNo][i*3+j];
                CP[j] += Tri[i][j]; // find center
              }
          DistToC = (FindMag(CP)/double(3)) / LIGHT_SPEED;
          Area = IntersectRadius(RangeInM,deltaRangeInM,Tri);
          if (RangeInM > 0)
            RetAmp=(GainFactor*sqrt(Area))/(RangeInM*RangeInM);
          else
            RetAmp = 0;

          if ((RetAmp > 0))// && (kw==0))// && (PNo ==1))
            {
  //            kw = 1;
              for (RangeTime=t-double(0.5)*PulseCenter;
                    (RangeTime<(t+r->PulseWidth*0.5-double(0.5)*PulseCenter));
                    RangeTime+=SecPerSample)
                {
                  PulseTime = RangeTime-t+double(0.5)*PulseCenter;
                  PulseSample = round((RangeTime-SlantStartTime)/SecPerSample);
                  if ((PulseSample >= 0) && (PulseSample <SamplePoints))
                  {
                  if (SendPulse(PulseTime*2, r->PulseWidth, DelaySlope, &PPhase))
                    {
                      PPhase = TwoPi*(PPhase-PulseFreq[PNo]*2*(DistToC+(t-SlantStartTime)));
							        OnePulse[PulseSample*2] += RetAmp*cos(fmod(PPhase,TwoPi));
							        OnePulse[PulseSample*2+1] += RetAmp*sin(fmod(PPhase,TwoPi));
                    }
                  }
                }
           }  // end - if there is return

        } // end for all surfaces do
// added for surfaces - end
#endif

			 // for all targets
			 for (TNo=0;TNo<TargetNo;TNo++)
				{
              // distance to target in time (not returndelay which would be twice that)
				  RangeInTime = double(0.5)*RangeDelay[TNo][PNo]; // distance to target in time (s)
			  	  temp0 = t-RangeInTime + double(0.5)*PulseCenter;
				  if ((temp0 > 0) && (temp0 < r->PulseWidth*0.5))
					 {
						if ((r->PulseType == MONO) || (r->PulseType == BARKER)) //mono
						  {
							 time = 2*(t - RangeInTime) - (r->PulseWidth*0.5) + PulseCenter;
							 phase = ( TwoPi* ( -(PulseFreq[PNo]*2*RangeInTime)-
							  ( TwoOverLight*TargetRadialVel[TNo][PNo]*
								  (PulseFreq[PNo]*time) ) ));

               if (r->PulseType == BARKER)
                 {
                   Cycle = fmod(floor((temp0*2 / r->PulseWidth)*
                     BarkerCodeLength[r->BarkerCode]),
                     BarkerCodeLength[r->BarkerCode]);
                   if (BarkerCode[r->BarkerCode][(int)Cycle] < 0)
                     phase += PI;
                 }
							 Amp = ReturnAmp[TNo][PNo]*Pulse(r,temp0*2);
 							 OnePulse[CurrentSample*2] += Amp*cos(fmod(phase,TwoPi));
							 OnePulse[CurrentSample*2+1] += Amp*sin(fmod(phase,TwoPi));
						  }
						else if (r->PulseType == CHIRP) // chirp
						  {
							 time = 2*(t - RangeInTime) - (r->PulseWidth*0.5) + PulseCenter;
							 temp3 = DelaySlope*time*time*0.5;

							 phase =  TwoPi* ( temp3-(PulseFreq[PNo]*2*RangeInTime)-
							  ( TwoOverLight*TargetRadialVel[TNo][PNo]*
								  (PulseFreq[PNo]*time+temp3) ) );
                     // no doppler
                     // phase = TwoPi* (temp3-PulseFreq[PNo]*2*RangeInTime);

							 Amp = ReturnAmp[TNo][PNo]*Pulse(r,temp0*2);
							 OnePulse[CurrentSample*2] += Amp*cos(fmod(phase,TwoPi));
							 OnePulse[CurrentSample*2+1] += Amp*sin(fmod(phase,TwoPi));
						  }
						else if (r->PulseType == OTHER_PULSE) // user defined
						  {
							 OnePulse[CurrentSample*2] += Interpolate(  (r->PulseDef).NoP[0],
											 (r->PulseDef).XAxis[0].DataArray,
											 (r->PulseDef).YAxis[0].DataArray,
											 (r->PulseDef).Coeff[0], temp0/r->PulseWidth,(r->PulseDef).IntMethod[0])*Pulse(r,temp0*2);
							 OnePulse[CurrentSample*2+1] += Interpolate(  (r->PulseDef).NoP[1],
											 (r->PulseDef).XAxis[1].DataArray,
										 	 (r->PulseDef).YAxis[1].DataArray,
											 (r->PulseDef).Coeff[1], temp0/r->PulseWidth,(r->PulseDef).IntMethod[1])*Pulse(r,temp0*2);
						  }
						else    // error
						  {
							 OnePulse[CurrentSample*2] = 0;
							 OnePulse[CurrentSample*2+1] = 0;
						  }
					 }
				} // end for all targets
          // if the amplitude of this sample is bigger than the current
          // maximum, replace the current maximum, note the SQRT will be
          // done at the end

          // add noise
          if (RootOfNoisePower != 0)
            {
              OnePulse[CurrentSample*2] += Gaussian(0,RootOfNoisePower,false);
              OnePulse[CurrentSample*2+1] += Gaussian(0,RootOfNoisePower,false);
            }
          Amp = OnePulse[CurrentSample*2] * OnePulse[CurrentSample*2] +
                OnePulse[CurrentSample*2+1] * OnePulse[CurrentSample*2+1];
          if (Amp > *MaxMagOfPulse)
            *MaxMagOfPulse = Amp;

			 CurrentSample++;
		  } // end for t= Slant
      *MaxMagOfPulse = sqrt(*MaxMagOfPulse);
    //  kw = 0;

}


//-------------------------------------------------------------------------//
// Function CalcGeometry                                                  //
// Find the Range delay, Return Amplitude, Radial Target Velocity
// of all targets, given the pulse send time and number of pulses.
//-------------------------------------------------------------------------//
void CalcGeometry(double **RangeDelay,double **ReturnAmp,double **TargetRadialVel,
struct SRadar *Radar,double *PulseSendTime, long PulseNo, struct STarget *FirstTarget,
struct SPlatform *FirstPlatform, struct SSurface *FirstSurface, double ***SurfP)
{
  long PNo, TNo, SNo,PFNo,        // loop variables
		 PlatformNo, TargetNo, SurfaceNo;  // total number of platforms and targets
                                  // and surfaces
  double ***PlatformPos;      // 3D array containing position of all platforms
										// for each pulse ([platform][pulse][x|y|z])
  double ***PlatformVel;      // 3D array containing velocity of all platforms
										// for each pulse ([platform][pulse][x|y|z])
  double ***PlatformRot;      // 3D array containing the rotation of all platforms
										// for each pulse ([platform][pulse][x|y|z])
  STarget *t;                 // general pointer to target
  SPlatform *p,               // general pointer to platform
				*RadarPF;         // pointer to radar platform
  SSurface *s;
  long RadarPFNo,             // number of radar platform
		 PFN;                   // general platform number
  double RotM[3][3];          // rotation coefficients
  long i,j;
  double GainFactor;
  double TargetPosRelativeToRadar[3],TPos[3];
  double TargetDist;
  double TargetPosRelativeToRadar2[3],TPos2[3];
  double PlatPosT[3],PlatPosR[3];
  double TargetDist2;
  double PlatformRot2[3];
  double RotM2[3][3];          // rotation coefficients
  double RCS;
  double AntennaDirAzi, AntennaDirElev, TRayAzi, TRayElev;
  double OffsetAzi, OffsetElev;
  double AntennaGain;
//  double RelVel[3];
  // difference in time between 2 position calculations to estimate
  // velocity - must be less than the minimum PRI possible
  #define DELTA_TIME double(0.0000001)
  #define ONE_OVER_DELTA_TIME double(10000000)


  // total number of targets
  TargetNo = CountTargets(FirstTarget);
  // total number of Surfaces
  SurfaceNo = CountSurfaces(FirstSurface);
  // total number of platforms
  PlatformNo = CountPlatforms(FirstPlatform);

  // create arrays...
  // position, velocity and rotation of each platform for each pulse
  PlatformPos = DMatrix3(0,PlatformNo-1,0,PulseNo-1,0,2);
  PlatformVel = DMatrix3(0,PlatformNo-1,0,PulseNo-1,0,2);
  PlatformRot = DMatrix3(0,PlatformNo-1,0,PulseNo-1,0,2);

  // find platform position and velocities for all pulses
  for (PFNo = 0;PFNo<PlatformNo;PFNo++)      // for all platforms
	 {
		// find the platform
		p = PointToPlatform(FirstPlatform, PFNo+1);
		for (PNo = 0;PNo<PulseNo;PNo++)        // for all pulses
		  {
			 FindPlatformPosition(p,PulseSendTime[PNo],PlatformPos[PFNo][PNo]);
			 FindPlatformVelocity(p,PulseSendTime[PNo],PlatformVel[PFNo][PNo]);
			 FindPlatformRotation(p,PulseSendTime[PNo],PlatformRot[PFNo][PNo]);
		  }
	 }

  // find radar platform
  FindPlatform(Radar->PlatformName, &RadarPF, &RadarPFNo, FirstPlatform);

  // compute return gain factor independent of time
  GainFactor = ((LIGHT_SPEED/Radar->StartFreq)*sqrt(Radar->PowerOutput)) /
			((4*PI)*sqrt(4*PI)*sqrt(Radar->Losses));

// start surface

  for (SNo=0;SNo<SurfaceNo;SNo++)       // for each Surface do
	 {
		// find pointer to Surface and corresponding platform
		s = PointToSurface(FirstSurface, SNo+1);
		FindPlatform(s->Name,&p,&PFN,FirstPlatform);

		for (PNo=0;PNo<PulseNo;PNo++)    // for each pulse do
		  {
			 // find rotation coeffs
			 FindRotMatrix(PlatformRot[PFN][PNo],RotM);

			 // calculate rotated Position and add platform position offset
			 // and subtract radar position resulting in a vector from radar
			 // to Surface
       for (j=0;j<3;j++) // for all three triangle points do
			   for (i=0;i<3;i++)  // for x y z do
				   {
    			   SurfP[SNo][PNo][j*3+i] = RotM[0][i] * s->Tri[j][0] +
						   RotM[1][i] * s->Tri[j][1] +  RotM[2][i] * s->Tri[j][2] +
						   PlatformPos[PFN][PNo][i] -
						   PlatformPos[RadarPFNo][PNo][i];
           }
		  } // for each pulse end
	 }  // for each Surface end
   // end surface



  for (TNo=0;TNo<TargetNo;TNo++)       // for each point target do
	 {
		// find pointer to target and corresponding platform
		t = PointToTarget(FirstTarget, TNo+1);
		FindPlatform(t->Name,&p,&PFN,FirstPlatform);

		for (PNo=0;PNo<PulseNo;PNo++)    // for each pulse do
		  {
			 // find rotation coeffs
			 FindRotMatrix(PlatformRot[PFN][PNo],RotM);

			 // calculate rotated Position and add platform position offset
			 // and subtract radar position resulting in a vector from radar
			 // to target
			 for (i=0;i<3;i++)
				{
				  FindTargetPosition(t, TPos);
				  TargetPosRelativeToRadar[i] = RotM[0][i] * TPos[0] +
						  RotM[1][i] * TPos[1] +  RotM[2][i] * TPos[2] +
						  PlatformPos[PFN][PNo][i] -
						  PlatformPos[RadarPFNo][PNo][i];
				}
			 // distance between target and radar
			 TargetDist = sqrt( DotProduct(TargetPosRelativeToRadar,
									 TargetPosRelativeToRadar, 3) );

       // now find target position a little bit later to estimate radial
       // velocity
			 FindPlatformPosition(p,PulseSendTime[PNo]+DELTA_TIME,PlatPosT);
			 FindPlatformPosition(RadarPF,PulseSendTime[PNo]+DELTA_TIME,PlatPosR);
			 FindPlatformRotation(p,PulseSendTime[PNo]+DELTA_TIME,PlatformRot2);
			 // find rotation coeffs
			 FindRotMatrix(PlatformRot2,RotM2);
			 // calculate rotated Position and add platform position offset
			 // and subtract radar position resulting in a vector from radar
			 // to target
			 for (i=0;i<3;i++)
				{
				  FindTargetPosition(t, TPos2);
				  TargetPosRelativeToRadar2[i] = RotM2[0][i] * TPos2[0] +
						  RotM2[1][i] * TPos2[1] +  RotM2[2][i] * TPos2[2] +
						  PlatPosT[i] -
						  PlatPosR[i];
				}
			 // distance between target and radar
			 TargetDist2 = sqrt( DotProduct(TargetPosRelativeToRadar2,
									 TargetPosRelativeToRadar2, 3) );

       // now we've got our two range values (range from radar)
       // calculate the radial velocity from that
			 TargetRadialVel[TNo][PNo] = (TargetDist2 - TargetDist)*ONE_OVER_DELTA_TIME;
       // never mid speeds slower than a um per second
       if (TargetRadialVel[TNo][PNo] < 1E-6)
         TargetRadialVel[TNo][PNo] = 0;
			 // range delay in seconds
			 RangeDelay[TNo][PNo] = 2*TargetDist/
						(LIGHT_SPEED - TargetRadialVel[TNo][PNo]);

			 RCS = t->RCS + (t->RCSdev);// temp * GaussDist(&SEED));
			 if (RCS < 0) RCS = 0;

			 if (t->ReflecType == 0) // isotropic
				{
				  //RCS_Average = t->RCS;
				}
			 else  // directional
				{
				  double TPos[3],PTSurfaceNormal[3];
				  double RayAngle;
				  double temp;
				  double AziAng, EleAng;

				  // calculate direction with standard deviation
				  FindTargetDir(t, &AziAng, &EleAng);
				  ConvertAnglesToVec(TPos, 1, AziAng, EleAng);

				  for (i=0;i<3;i++)
					 PTSurfaceNormal[i] = RotM[0][i] * TPos[0] + RotM[1][i] * TPos[1] +
												 RotM[2][i] * TPos[2];

				  temp = DotProduct(PTSurfaceNormal,
										  TargetPosRelativeToRadar, 3);

				  if (TargetDist == 0)
					 {
						// point target too close
						RayAngle = 0;
					 }
				  else
					 {
						// calculate angle between normal vector and the vector
						// from the radar to the target
						// (cos a= b1*b2/(mag(b1)*mag(b2)))
						// 0 = head an, PI/2 = sideways, PI = from behind
						RayAngle = fabs(PI - acos(temp/TargetDist));
					 }
				  if (t->GainType == 0)  // cos
					 {
						RCS = cos(RayAngle) * RCS;
						// don't reflect on the backface
						if (RCS < 0) RCS = 0;
					 }
				  else if (t->GainType == 1)  // other
					 {
						RCS *= dBToFac(Interpolate((t->DataDef).NoP[0],
										  (t->DataDef).XAxis[0].DataArray,
                                (t->DataDef).YAxis[0].DataArray,
										  (t->DataDef).Coeff[0], RayAngle*RadToDeg,(t->DataDef).IntMethod[0]));
					 }

				} // end else dir

			 // find the direction into which radar is pointing relative to
			 // the radar platform
			 FindAntennaDir(Radar, PulseSendTime[PNo], PlatformPos[RadarPFNo][PNo],
				  PlatformRot[RadarPFNo][PNo],&AntennaDirAzi,&AntennaDirElev);

			 TRayAzi = AziAngle(TargetPosRelativeToRadar);
			 TRayElev = ElevAngle(TargetPosRelativeToRadar);

			 OffsetAzi = fabs(AntennaDirAzi - TRayAzi);
			 OffsetElev = fabs(AntennaDirElev - TRayElev);

			 double AntennaGainT = 1;
			 double AntennaGainR = 1;

			 if (Radar->AntennaGainTypeT == 0)
				{
				  AntennaGainT = 1;
				}
			 else	if (Radar->AntennaGainTypeT == 1)
				{
				  AntennaGainT = SinAntennaGain(OffsetAzi,Radar->AziBeamWidthT)*
									 SinAntennaGain(OffsetElev,Radar->ElevBeamWidthT);
				}
			 else	if (Radar->AntennaGainTypeT == 2)
				{
				  AntennaGainT = dBToFac(Interpolate((Radar->AntennaTDef).NoP[0],
										  (Radar->AntennaTDef).XAxis[0].DataArray,
                                (Radar->AntennaTDef).YAxis[0].DataArray,
										  (Radar->AntennaTDef).Coeff[0], OffsetAzi*RadToDeg,(Radar->AntennaTDef).IntMethod[0]))*
									  dBToFac(Interpolate((Radar->AntennaTDef).NoP[1],
										  (Radar->AntennaTDef).XAxis[1].DataArray,
										  (Radar->AntennaTDef).YAxis[1].DataArray,
										  (Radar->AntennaTDef).Coeff[1], OffsetElev*RadToDeg,(Radar->AntennaTDef).IntMethod[1]));

				}
			 if (Radar->AntennaGainTypeR == 0)
				{
				  AntennaGainR = 1;
				}
			 else	if (Radar->AntennaGainTypeR == 1)
				{
				  AntennaGainR = SinAntennaGain(OffsetAzi,Radar->AziBeamWidthR)*
									 SinAntennaGain(OffsetElev,Radar->ElevBeamWidthR);
				}
			 else	if (Radar->AntennaGainTypeR == 2)
				{
				  AntennaGainR = AntennaGainT;
				}
			 else	if (Radar->AntennaGainTypeR == 3)
				{
				  AntennaGainR = dBToFac(Interpolate((Radar->AntennaRDef).NoP[0],
										  (Radar->AntennaRDef).XAxis[0].DataArray,
										  (Radar->AntennaRDef).YAxis[0].DataArray,
										  (Radar->AntennaRDef).Coeff[0], OffsetAzi*RadToDeg,(Radar->AntennaRDef).IntMethod[0]))*
									  dBToFac(Interpolate((Radar->AntennaRDef).NoP[1],
										  (Radar->AntennaRDef).XAxis[1].DataArray,
										  (Radar->AntennaRDef).YAxis[1].DataArray,
										  (Radar->AntennaRDef).Coeff[1], OffsetElev*RadToDeg,(Radar->AntennaRDef).IntMethod[1]));
				}

			 AntennaGain = sqrt(AntennaGainR * AntennaGainT);

			 // the radar equation : NOTE that ReturnAmp contains the amplitude
			 // not the power (which would be the square of that)
			 if (TargetDist > 0)
         {
           if (Radar->ApplyAGC)
             {
               if (Radar->AGCType == 0)
				         ReturnAmp[TNo][PNo] = (GainFactor * AntennaGain * sqrt(RCS));
               else
/*temp*/         ReturnAmp[TNo][PNo] = (GainFactor * AntennaGain * sqrt(RCS));
             }
           else
				     ReturnAmp[TNo][PNo] = (GainFactor * AntennaGain * sqrt(RCS)) /
											 (TargetDist * TargetDist);
         }
			 else
				ReturnAmp[TNo][PNo] = sqrt(MAX_DOUBLE)/100;

		  } // for each pulse end
	 }  // for each target end


  // free memory allocated
  Free_DMatrix3(PlatformPos,0,0,0);
  Free_DMatrix3(PlatformVel,0,0,0);
  Free_DMatrix3(PlatformRot,0,0,0);
}

//-------------------------------------------------------------------------//
// Function SendPulse                                                      //
//                                                                         //
// Returns the real and imaginary part of a pulse of given                 //
// parameters.                                                             //
// time = 0 - pulsewidth                                                   //
//-------------------------------------------------------------------------//
bool SendPulse(double time, double PulseWidth, double DelaySlope, double *Phase)
{
  double temp;

  if ((time<0) || (time>PulseWidth))
		return false;
  temp = time-0.5*PulseWidth;
  temp = 0.5*temp*temp*DelaySlope;

  *Phase = temp;
  return true;
}

//-------------------------------------------------------------------------//
// Function MatchFilter                                                    //
//                                                                         //
// Returns the real and imaginary part of a matched filter of given        //
// parameters.                                                             //
//-------------------------------------------------------------------------//
Complex MatchFilter(double time, struct SRadar *r)
{
  double DelaySlope,BW;
  double temp, fac,Cycle;
  Complex MF;

  BW = FindBandwidth(r);
/*  // find bandwidth
 	if ((r->PulseType == MONO) || (r->PulseType == BARKER)) BW = 0;      // mono pulse
	else if (r->PulseType == CHIRP) BW = r->ChirpBW;   // chirp pulse
  else BW = r->ChirpBW;
*/
  DelaySlope = BW/r->PulseWidth;

  if ((time<0) || (time>r->PulseWidth))
	 {
		MF.real = 0;
		MF.imag = 0;
		return MF;
	 }
  temp = time-0.5*r->PulseWidth;
  temp = temp*temp*DelaySlope*PI;

  if (r->PulseType == BARKER)
                 {
                   Cycle = fmod(floor((time / r->PulseWidth)*
                     BarkerCodeLength[r->BarkerCode]),
                     BarkerCodeLength[r->BarkerCode]);
                   if (BarkerCode[r->BarkerCode][BarkerCodeLength[r->BarkerCode]-1-(int)Cycle] < 0)
                     temp += PI;
                 }

  MF.real = cos(temp);
  MF.imag = -sin(temp);

  // rect
  if (r->MatchedFilterW == 0)
	 return MF;
  // hanning
  else if (r->MatchedFilterW == 1)
	 {
		fac = 0.5*(1+r->HanningC -
			cos(TwoPi*time/r->PulseWidth)*(1-r->HanningC));
		MF.real *= fac;
		MF.imag *= fac;
		return MF;
	 }
  // bartlett
  else if (r->MatchedFilterW == 2)
	 {
		if (time <= (r->PulseWidth/2)) fac = 2*time/r->PulseWidth;
		else fac = 2 - 2*time/r->PulseWidth;
		MF.real *= fac;
		MF.imag *= fac;
		return MF;
	 }
  // hamming
  else if (r->MatchedFilterW == 3)
	 {
		fac = 0.54 - 0.46*cos(TwoPi*time/r->PulseWidth);
		MF.real *= fac;
		MF.imag *= fac;
		return MF;
	 }
  // user defined
  else if (r->MatchedFilterW == 4)
	 {
		 fac = Interpolate(  (r->MFDef).NoP[0],
									(r->MFDef).XAxis[0].DataArray,
									(r->MFDef).YAxis[0].DataArray,
									(r->MFDef).Coeff[0], time/r->PulseWidth,(r->MFDef).IntMethod[0]);
		MF.real *= fac;
		MF.imag *= fac;
		return MF;
	 }
  else return MF;
}


//-------------------------------------------------------------------------//
// Function SinAntennaGain(double Offset, double BeamWidth)                //
//                                                                         //
// This function returns the relative gain for a sinusoidal beam shape.    //
//-------------------------------------------------------------------------//
double SinAntennaGain(double Offset, double BeamWidth)
{
  double temp;

  Offset = fmod(fabs(Offset), 2*PI);
  if (Offset > PI) Offset -= 2*PI;
  if ( (Offset >= (PI/2)) || (Offset <= -(PI/2)) )
	 return 0;
  if (Offset == 0) return 1;
  if (BeamWidth == 0) return 0;

  temp = 0.88*PI*(sin(Offset)/BeamWidth);

  return fabs(sin(temp)/temp);
}
//-------------------------------------------------------------------------//
// Function Pulse                                                          //
//                                                                         //
// This function returns the amplitude of pulse with given sampling time,  //
// rise time, fall time and pulsewidth. The max amplitude is 1.            //
// Note that TotalWidth includes rise and fall time.                       //
//-------------------------------------------------------------------------//
double Pulse(struct SRadar *r, double t)
{
  if (r->Envelope == 0) // rect
	 {
		if ( (t >= 0) && (t < (r->PulseWidth)) ) return 1;
		return 0;
	 }
  else if (r->Envelope == 1)  // linear ramp
	 {
		if ( (t >= r->RiseTime) && (t < (r->PulseWidth-r->FallTime)) ) return 1;
		if ( (t < r->RiseTime) && (t >= 0) ) return (t / r->RiseTime);
		if ( (t >= (r->PulseWidth - r->FallTime)) && (t<r->PulseWidth) ) return
		  (1 - ((t - (r->PulseWidth - r->FallTime))/r->FallTime));
		return 0;
	 }
  else if (r->Envelope == 2)  // user defined
	 {
		return (Interpolate(  (r->EnvelopeDef).NoP[0],
				  (r->EnvelopeDef).XAxis[0].DataArray,
				  (r->EnvelopeDef).YAxis[0].DataArray,
				  (r->EnvelopeDef).Coeff[0], t/r->PulseWidth,(r->EnvelopeDef).IntMethod[0]));

	 }
  else return 0;
}
//-------------------------------------------------------------------------//
double FindNyquistRate(struct SRadar *r)
{
  if (r == NULL) return 1;
  // calculate sample frequency
  if (r->PulseType == CHIRP)
    return (r->ChirpBW);
  else
    return (double(1.0)/r->PulseWidth);
}

//-------------------------------------------------------------------------//
// CalcArray2                                  //
//-------------------------------------------------------------------------//
void CalcArray2(struct SRadar *r, struct SSimulation *CurrentSim,
    double **Data, long PulseNo, long DataXSize, double *PulseFreq,
    double **RangeDelay,  double **TargetRadialVel, double **ReturnAmp,
    double *MaxMagnitude, struct STarget *FirstTarget,
    struct SPlatform *FirstPlatform,struct SSurface *FirstSurface, double ***SurfP)
{
  long PNo, TargetNo, SurfaceNo;
  double DelaySlope,PulseCenter,MaxMagOfPulse,MaxAmp;
  double SecPerSample;

  *FirstPlatform = *FirstPlatform;

  // calculate slant range in time (s)
  double SlantStartTime = CurrentSim->SlantStart / LIGHT_SPEED;
  double SlantEndTime = CurrentSim->SlantEnd / LIGHT_SPEED;
  // total number of targets
  TargetNo = CountTargets(FirstTarget);
  // total number of Surfaces
  SurfaceNo = CountSurfaces(FirstSurface);
  // should sit point target in the middle of the pulse ?
  if (CurrentSim->PTPos == 1)  // yes
    PulseCenter = 0.5*r->PulseWidth;
  else               // no
    PulseCenter = 0;
  // calculate frequency slope of chirp pulse (rate at which frequency changes)
  // for mono it will be zero
  if ((r->PulseType == MONO) || (r->PulseType == BARKER))
    DelaySlope = 0;   //mono
  else
    DelaySlope = r->ChirpBW/r->PulseWidth;   //chirp and user-defined

  // find also maximum magnitude of all pulses
  *MaxMagnitude = 0;

  if (CurrentSim->SimTYPE == RAW_RETURN)
	 {
    // time (seconds) per sample = sampling period
   // modified note due to round function in CalcAziPoint function
  //  SecPerSample = double(0.5)/(CurrentSim->SampleFreq);
    SecPerSample = (SlantEndTime-SlantStartTime)/double(DataXSize);
		// for every pulse do
		for (PNo=0;PNo<PulseNo;PNo++)
		  {
            // tell somebody how far we are... (CalcProcess is global)
            CalcProgress = (int)((float(PNo)/float(PulseNo))*100);
            if (UserAbort) return;
			 // calculate one pulse
			 CalcOnePulse(Data[PNo], DataXSize, SlantStartTime,
					 SecPerSample, TargetNo, PulseFreq,
					 RangeDelay, TargetRadialVel, ReturnAmp,PNo, r,
					 PulseCenter, DelaySlope, &MaxMagOfPulse, SurfP,SurfaceNo);
          if (MaxMagOfPulse > *MaxMagnitude)
            *MaxMagnitude = MaxMagOfPulse;
		  }
	 }
  else if (CurrentSim->SimTYPE == MATCHED_FILTER)
	 {
		double *OnePulse, Fac, temp,*MatchFilterA, norm;
		long i,j, Shift, SamplePoints;
		Complex MF;

		// actual number of points calculated (>Samplepoints because
		// 1. calculations needs to extend beyond the screen output range because
		// of matched filter
		// 2. FFT is faster with power of twos
		// find the next higher power of 2
        // Only 1/2 a PW needs to be added as the PW is half in slant range
        // - see documentation  RLORD
		SamplePoints = (long)(pow(2,ceil(log(CurrentSim->SampleFreq*2*
			((SlantEndTime-SlantStartTime)+double(0.5)*r->PulseWidth + PulseCenter) )*double(1.442695041))));
		// as the peak of the matched filter occurs at the end of the pulse, i.e.
		// when the MF overlaps with the pulse (which occurs at 1/2 pulsewidth
		// behind the actual target), move the whole array by that amount +
        // we have to sample 1/2 a PW before.. therefor 1 PW
        Shift = (long)(r->PulseWidth*CurrentSim->SampleFreq*2*double(0.5));
		// time between two samples
    SecPerSample = double(1) / (CurrentSim->SampleFreq*2);
		// conversion factor from samples in memory to pixels, i.e. Samples/Pizel
		Fac = (SlantEndTime-SlantStartTime)*CurrentSim->SampleFreq*2/(double)DataXSize;
        // normalization factor such that amplitude is independent of sampling freq
        norm = double(1)/(SamplePoints * CurrentSim->SampleFreq*2);
		OnePulse = DVector(0,SamplePoints*2);
		// matched filter
		MatchFilterA = DVector(0,SamplePoints*2);
		// set all to zero
		for (i=0;i<SamplePoints*2;i++)
		  MatchFilterA[i] = 0;
		// find matched filter values
		for (i=0;i<SamplePoints;i++)
		  {
			 MF = MatchFilter(double(2*i)/(CurrentSim->SampleFreq*2), r);
			 MatchFilterA[2*i] = MF.real;
			 MatchFilterA[2*i+1] = MF.imag;
		  }
		// convert MatchFilter into frequency domain
 		CFFT(MatchFilterA-1,SamplePoints,1);
		// for every pulse do
		for (PNo=0;PNo<PulseNo;PNo++)
		  {
          // Note that it is necessary to start sampling 1/2 a PW before the
          // actual window starts (= 1/4 PW in slant range)
		    // tell somebody how far we are... (CalcProcess is global)
             CalcProgress = (int)((float(PNo)/float(PulseNo))*100);
             if (UserAbort) return;
			 CalcOnePulse(OnePulse, SamplePoints, SlantStartTime-(PulseCenter*0.5),
					 SecPerSample, TargetNo, PulseFreq,
  					 RangeDelay, TargetRadialVel, ReturnAmp,PNo, r,
					 PulseCenter, DelaySlope, &MaxMagOfPulse, SurfP, SurfaceNo );
			 // convert pulse to frequency domain
			 CFFT(OnePulse-1,SamplePoints,1);
			 // multiply matched filter with return pulse = time convolution
			 for (i=0;i<SamplePoints;i++)
				{
 				  temp = OnePulse[2*i];
 				  OnePulse[2*i] = OnePulse[2*i]*MatchFilterA[2*i]-OnePulse[2*i+1]*MatchFilterA[2*i+1];
 				  OnePulse[2*i+1] = temp*MatchFilterA[2*i+1]+OnePulse[2*i+1]*MatchFilterA[2*i];
				}
			 // convert back to time domain
 			 CFFT(OnePulse-1,SamplePoints,-1);
          // In case of the matched filter it is
          // necessary to divide by SamplePoints * SampleFreq*2, to get a factor
          // which is independent of Sampling frequency (i.e. to overcome the
          // problem of getting higher convolution integrals with higher
          // sampling frequencies) =>
          // normalize such that amplitude is independent of sampling freq
          // and find highest amp
          MaxMagOfPulse = 0;
 		  for (i=0;i<SamplePoints;i++)
            {
              OnePulse[2*i] *= norm;
              OnePulse[2*i+1] *= norm;
              MaxAmp = (OnePulse[i*2] * OnePulse[i*2] +
                       OnePulse[i*2+1] * OnePulse[i*2+1]);
              if (MaxAmp > MaxMagOfPulse)
                MaxMagOfPulse = MaxAmp;
            }
          MaxMagOfPulse = sqrt(MaxMagOfPulse);
          if (MaxMagOfPulse > *MaxMagnitude)
            *MaxMagnitude = MaxMagOfPulse;
			 // scale into pixel space
			 for (i=0;i<DataXSize;i++)
				{
				  j = long(double(i) * Fac) + Shift;
				  if (j >= SamplePoints) j = SamplePoints-1;
				  Data[PNo][i*2] = OnePulse[2*j];
				  Data[PNo][i*2+1] = OnePulse[2*j+1];
				}
		  } // end for PNo
		Free_DVector(MatchFilterA,0);
		Free_DVector(OnePulse,0);

	 }  // end if SimTYPE = MF
}
//-------------------------------------------------------------------------//
extern int CalcProgress2;
//-------------------------------------------------------------------------//
int SaveSimuData(double *MaxMagnitude, struct SSimulation *ThisSim,
  struct LinkedLists *FP, long PointsSlant, double *TotalSampleNo,
  double *ClippedSamples,double *ZeroSamples)
{
  #define PulsesPerSession 100
  long FirstPulse, LastPulse,TotalPulseNo,PulseNo,SurfaceNo;
  long SavePulseStart, SavePulseEnd;
  long SamplePoints, TargetNo,PNo,i;
  bool Finished=FALSE;
  double *PulseFreq, **RangeDelay, **TargetRadialVel, **ReturnAmp,
			**Data,*PulseSendTime,***SurfP;
  double I,Q,MaxMagnitudeTemp;
  double FracPart,IntPart, Norm, Center;
  int bytes;
  int A2DBytes = (int)ceil((float)ThisSim->A2Dbits*(float)0.125);
  FILE *OutFilep;
  struct SRadar *CRadar, *R; // current radar
  bool Clipped;

  bool RadarFound = false;
  R =  FP->FirstRadar;
  CRadar = R;
  while (R != NULL)
	{
	  if (CompStrNI(R->RadarName, ThisSim->RadarName, strlen(R->RadarName)) == 0)
	    {
        CRadar = R;
        RadarFound = true;
        break;
      }
	  R = R->next;
	}
  if (!RadarFound) return 2;

  OutFilep = fopen(ThisSim->FileName, "wb");  // append to file, if not exists, create it
  if (!OutFilep)
 	{
 	  return 3;
 	}

  // figure out how many pulses to write
  FindPulsesInRange(ThisSim->AzimuthStart,ThisSim->AzimuthEnd,
				    CRadar,&FirstPulse,&LastPulse);
  TotalPulseNo = (LastPulse - FirstPulse) + 1;
  if (TotalPulseNo <= 0) TotalPulseNo = 1;
  // total number of targets
  TargetNo = CountTargets(FP->FirstTarget);
  // total number of surfaces
  SurfaceNo = CountSurfaces(FP->FirstSurface);
   // number of sample points in slant range
  SamplePoints = PointsSlant;
  // create arrays...
  PulseFreq = DVector(0,PulsesPerSession-1);
  RangeDelay = DMatrix(0,TargetNo-1,0,PulsesPerSession-1);
  TargetRadialVel = DMatrix(0,TargetNo-1,0,PulsesPerSession-1);
  ReturnAmp = DMatrix(0,TargetNo-1,0,PulsesPerSession-1);
  SurfP = DMatrix3(0,SurfaceNo-1,0,PulsesPerSession-1,0,8);
  PulseSendTime = DVector(0,PulsesPerSession-1);
  Data = DMatrix(0,PulsesPerSession-1,0,SamplePoints*2-1);
  // now write the pulses in session for memory reasons (e.g. 0-99,100-199 etc.)
  SavePulseEnd = -1;
  // calculate the multiplication factor such that LSB is the right amp.
  Norm = (double)1/(ThisSim->LSBvalue);
  *MaxMagnitude = 0; // let's find also maximum magnitude
   #ifndef COMPILE_FOR_WINDOWS32
     printf("[100%% = %d *] :",(int)((float)TotalPulseNo/(float)PulsesPerSession));
   #endif

  // for statistics
  *TotalSampleNo = 0;
  *ClippedSamples = 0;
  *ZeroSamples = 0;
  do
	  {
	    SavePulseStart = SavePulseEnd+1;
	    SavePulseEnd = SavePulseStart+PulsesPerSession-1;
	    if (SavePulseEnd >= (TotalPulseNo-1))
		    {
			    SavePulseEnd = TotalPulseNo-1;
			    Finished = TRUE;
		    }
      PulseNo = SavePulseEnd - SavePulseStart + 1;

      #ifdef COMPILE_FOR_WINDOWS32
        CalcProgress2 = (int)(((float)SavePulseStart/(float)TotalPulseNo)*100);
      #else
        printf("*");
      #endif

	    // calculate time when each pulse is sent and the corresponding freq
	    for (PNo=0;PNo<PulseNo;PNo++)
		    {
			    PulseSendTime[PNo] = FindPulseSendTime(PNo+FirstPulse+SavePulseStart,
									 CRadar);
			    PulseFreq[PNo] = FindPulseFreq(PNo+FirstPulse+SavePulseStart,
					  			 CRadar);
		    }

      // calculate range-delay, return amp etc. for each pulse
      CalcGeometry(RangeDelay, ReturnAmp, TargetRadialVel, CRadar, PulseSendTime,
                  PulseNo, FP->FirstTarget, FP->FirstPlatform,
                  FP->FirstSurface, SurfP);

      // Calculate Data array of size (PulseNo x SamplePoints)
      CalcArray2(CRadar, ThisSim, Data, PulseNo, SamplePoints, PulseFreq,
                 RangeDelay, TargetRadialVel, ReturnAmp, &MaxMagnitudeTemp,
                 FP->FirstTarget, FP->FirstPlatform,
                 FP->FirstSurface, SurfP);
      if (MaxMagnitudeTemp > *MaxMagnitude) *MaxMagnitude = MaxMagnitudeTemp;
      if (UserAbort) break;
	    // calculate the offset
      if (ThisSim->A2Dbits == 1)
        Center = 1;
      else
	      Center = pow(2,ThisSim->A2Dbits-1)-1;
	    // write array to file
		  for (PNo=0;PNo<PulseNo;PNo++)
	      {
          if (UserAbort) break;
	        for (i=0;i<SamplePoints;i++)
					 {
             (*TotalSampleNo)++;
             Clipped = false;
	  		     I = round(Norm*Data[PNo][2*i]);
			       Q = round(Norm*Data[PNo][2*i+1]);
             // check limits
             if (I>Center+1) {I = Center+1; Clipped=true;}
             if (I<-Center) {I = -Center; Clipped=true;}
             if (Q>Center+1) {Q = Center+1; Clipped=true;}
             if (Q<-Center) {Q = -Center; Clipped=true;}
             if (Clipped) (*ClippedSamples)++;
             if ((I == 0) && (Q == 0)) (*ZeroSamples)++;
             // now write data in either ascii or binary
    	       if (ThisSim->FileType == ASCII) //ASCII
		           {
					       fprintf(OutFilep,"%1.0f %1.0f\n",I,Q);
// special case for for automatic rotation
//                 fprintf(OutFilep,"%4.0f %4.0f  ",I,Q);
               }
             else if (ThisSim->FileType == ASCIIwithBrackets) //ASCII but (I,Q)
		           {
					       fprintf(OutFilep,"(%1.0f,%1.0f) ",I,Q);
               }
             else // BINARY
               {
					       I += Center;
					       Q += Center; // add offset such that range is from 0 to (2^x)-1
					       for (bytes = 0;bytes < A2DBytes; bytes++)
						       {
							       I = I/256;
							       FracPart = modf(I,&IntPart);
							       fprintf(OutFilep,"%c",(char)(FracPart*256));
							       I = IntPart;
						       }
					       for (bytes = 0;bytes < A2DBytes; bytes++)
						       {
							       Q = Q/256;
							       FracPart = modf(Q,&IntPart);
							       fprintf(OutFilep,"%c",(char)(FracPart*256));
							       Q = IntPart;
						       }
					     }  // end else binary
           } // for all samplepoints
          // add separator between succesive pulses
// comment next line out for no lf between pulses
			    if (ThisSim->FileType == ASCII) fprintf(OutFilep,"\n");
			    if (ThisSim->FileType == ASCIIwithBrackets) fprintf(OutFilep,"\n");
        } // PNo
	  } while (!Finished);
   #ifndef COMPILE_FOR_WINDOWS32
     printf("\n");
   #endif

  // and free arrays again
  Free_DVector(PulseFreq,0);
  Free_DMatrix(RangeDelay,0,0);
  Free_DMatrix(TargetRadialVel,0,0);
  Free_DMatrix(ReturnAmp,0,0);
  Free_DVector(PulseSendTime,0);
  Free_DMatrix(Data,0,0);
  Free_DMatrix3(SurfP,0,0,0);

// special case for rotation
// fprintf(OutFilep,"\n");


  fclose(OutFilep);
  return true;
}
//-------------------------------------------------------------------------//
int SaveGeometryData(struct SGeometry *ThisGeo,
  struct LinkedLists *FP)
{
  char str[20000], temp[2560];
  char Head1[512], Head2[512], Head3[512], Head4[512];
  bool HeaderSetup = false;
  long PNo,TNo;          // loop variables for pulses and targets
  long FirstPulse, LastPulse, PulseNo,PlatformNo,TargetNo,SurfaceNo,PFNo;
  unsigned int VisibleOpt=ThisGeo->ShowVar;
  double *PulseSendTime;
  bool Comma = ThisGeo->FileType;
  double PlatformPos[3],PlatformVel[3],PlatformRot[3];
  struct SPlatform *p;
  double SinglePulseSendTime[1];
  FILE *OutFilep;
  struct SRadar *CRadar;
  double DistToTarget;
  int PFN;

  OutFilep = fopen(ThisGeo->FileName, "wt");  // write text file
  if (!OutFilep)
    return 1;

  // total number of platforms
  PlatformNo = CountPlatforms(FP->FirstPlatform);
  // total number of targets
  TargetNo = CountTargets(FP->FirstTarget);
  // total number of surfaces
  SurfaceNo = CountSurfaces(FP->FirstSurface);

  // clear header lines
  strcpy(Head1,"Pulse No. ");
  strcpy(Head2,"          ");
  strcpy(Head3,"          ");
  strcpy(Head4,"--------- ");

  FindRadar(ThisGeo->RadarName, &CRadar, &PFN, FP->FirstRadar);

  if (CRadar == NULL)
    return 2;

  // calculate number of pulses
  FindPulsesInRange(ThisGeo->AzimuthStart,ThisGeo->AzimuthEnd,CRadar,
                        &FirstPulse,&LastPulse);

  // because PulseSendTime is used often create an array with values
  PulseNo = (LastPulse - FirstPulse) + 1;
  // create array
  PulseSendTime = DVector(0,PulseNo-1);
  // calculate time when each pulse is sent
  for (PNo=0;PNo<PulseNo;PNo++)
    PulseSendTime[PNo] = FindPulseSendTime(PNo+FirstPulse, CRadar);

  for (PNo=FirstPulse;PNo<LastPulse+1;PNo++)
   {
    // clear string and setup number
    sprintf(str,"%9ld", PNo);
//    if (Comma) strcat(str,CommaSep); else strcat(str,SpaceSep);
        if (Comma) strcat(str,","); else strcat(str," ");

    // show pulse transmiting time
    if (((VisibleOpt >> 0) & 1) == 1)
      {
        if (!HeaderSetup)
          {
            strcat(Head1,"Pulse trans. ");
            strcat(Head2,"   time      ");
            strcat(Head3,"    (s)      ");
            strcat(Head4,"------------ ");
          }
        sprintf(temp,"%12.8G", PulseSendTime[PNo-FirstPulse]);
        strcat(str,temp);
        if (Comma) strcat(str,","); else strcat(str," ");
      }

    // show pulse frequency
    if (((VisibleOpt >> 1) & 1) == 1)
      {
        if (!HeaderSetup)
          {
            strcat(Head1,"Pulse freq.  ");
            strcat(Head2,"   (GHz)     ");
            strcat(Head3,"             ");
            strcat(Head4,"------------ ");
          }
        // calculate the center frequency of each pulse sent
        sprintf(temp,"%12.8G", FindPulseFreq(PNo, CRadar)*NANO);
        strcat(str,temp);
        if (Comma) strcat(str,","); else strcat(str," ");
      }

    // find platform position and velocities for all pulses
    for (PFNo = 0;PFNo<PlatformNo;PFNo++)      // for all platforms
	    {
	  	  // find the platform
		    p = PointToPlatform(FP->FirstPlatform, PFNo+1);
        // show platform position
        if (((VisibleOpt >> 2) & 1) == 1)
          {
            if (!HeaderSetup)
              {
                strcat(Head1,"      Platform Position          ");
                sprintf(temp,"     (%-20s)      ",p->Name);
                strcat(Head2,temp);
                strcat(Head3,"   X (m)      Y (m)      Z (m)   ");
                strcat(Head4,"---------- ---------- ---------- ");
              }
         	  // calculate the platform position when pulse was sent
            FindPlatformPosition(p,PulseSendTime[PNo-FirstPulse],PlatformPos);
	          if (Comma)
              sprintf(temp,"%10.4G,%10.4G,%10.4G", PlatformPos[0],
                PlatformPos[1], PlatformPos[2]);
            else
              sprintf(temp,"%10.4G %10.4G %10.4G", PlatformPos[0],
                PlatformPos[1], PlatformPos[2]);
            strcat(str,temp);
            if (Comma) strcat(str,","); else strcat(str," ");
          }
		    // show platform velocity
        if (((VisibleOpt >> 3) & 1) == 1)
          {
            if (!HeaderSetup)
              {
                strcat(Head1,"      Platform Velocity          ");
                sprintf(temp,"     (%-20s)      ",p->Name);
                strcat(Head2,temp);
                strcat(Head3,"  X (m/s)    Y (m/s)    Z (m/s)  ");
                strcat(Head4,"---------- ---------- ---------- ");
              }
         	  // calculate the platform position when pulse was sent
            FindPlatformVelocity(p,PulseSendTime[PNo-FirstPulse],PlatformVel);
	          if (Comma)
              sprintf(temp,"%10.4G,%10.4G,%10.4G", PlatformVel[0],
                PlatformVel[1], PlatformVel[2]);
            else
              sprintf(temp,"%10.4G %10.4G %10.4G", PlatformVel[0],
                PlatformVel[1], PlatformVel[2]);
            strcat(str,temp);
            if (Comma) strcat(str,","); else strcat(str," ");
          }
		    // show platform rotation
        if (((VisibleOpt >> 4) & 1) == 1)
          {
            if (!HeaderSetup)
              {
                strcat(Head1,"      Platform Rotation          ");
                sprintf(temp,"     (%-20s)      ",p->Name);
                strcat(Head2,temp);
                strcat(Head3,"  X (deg)    Y (deg)    Z (deg)  ");
                strcat(Head4,"---------- ---------- ---------- ");
              }
         	  // calculate the platform position when pulse was sent
            FindPlatformRotation(p,PulseSendTime[PNo-FirstPulse],PlatformRot);
	          if (Comma)
              sprintf(temp,"%10.4G,%10.4G,%10.4G", PlatformRot[0]*RadToDeg,
                PlatformRot[1]*RadToDeg, PlatformRot[2]*RadToDeg);
            else
              sprintf(temp,"%10.4G %10.4G %10.4G", PlatformRot[0]*RadToDeg,
                PlatformRot[1]*RadToDeg, PlatformRot[2]*RadToDeg);
            strcat(str,temp);
            if (Comma) strcat(str,","); else strcat(str," ");
          }
		  }  // end for PFNo

     // show target range-delay, return-amp, radial velocity
     if ((((VisibleOpt >> 5) & 1) == 1) || (((VisibleOpt >> 6) & 1) == 1) ||
         (((VisibleOpt >> 7) & 1) == 1))
       {
         // temporary arrays
         double *PulseFreq,**RangeDelay,**TargetRadialVel,**ReturnAmp,***SurfP;
         // create arrays...
         PulseFreq = DVector(0,0);
         RangeDelay = DMatrix(0,TargetNo-1,0,0);
         TargetRadialVel = DMatrix(0,TargetNo-1,0,0);
         ReturnAmp = DMatrix(0,TargetNo-1,0,0);
         SurfP = DMatrix3(0,SurfaceNo-1,0,0,0,8);

         SinglePulseSendTime[0] = PulseSendTime[PNo-FirstPulse];

         // calculate range-delay, return amp etc. for each pulse
         CalcGeometry(RangeDelay, ReturnAmp, TargetRadialVel, CRadar,
             SinglePulseSendTime, 1, FP->FirstTarget,
             FP->FirstPlatform, FP->FirstSurface, SurfP);

         for (TNo=0;TNo<TargetNo;TNo++)       // for each point target do
	         {
             // check if with given range
             DistToTarget = RangeDelay[TNo][0]*0.5*LIGHT_SPEED; // in m
             if ((DistToTarget >= ThisGeo->SlantStart) &&
                 (DistToTarget <= ThisGeo->SlantEnd))
             {
             if (((VisibleOpt >> 5) & 1) == 1)
               {
                 if (!HeaderSetup)
                   {
                     strcat(Head1,"  Range Delay     ");
                     sprintf(temp,"  (Target %4ld)   ",TNo+1);
                     strcat(Head2,temp);
                     strcat(Head3,"       (s)        ");
                     strcat(Head4,"----------------- ");
                   }
               //  if (Comma) // strcat(str,","); else strcat(str," ");
                 sprintf(temp,"%17.12G", RangeDelay[TNo][0]);
               //  else
               //  sprintf(temp,"%17.12G ", RangeDelay[TNo][0]);
                 strcat(str,temp);
                 if (Comma) strcat(str,","); else strcat(str," ");
               }
             if (((VisibleOpt >> 6) & 1) == 1)
               {
                 if (!HeaderSetup)
                   {
                     strcat(Head1," Return Amplitude ");
                     sprintf(temp,"  (Target %4ld)   ",TNo+1);
                     strcat(Head2,temp);
                     strcat(Head3,"       (V)        ");
                     strcat(Head4,"----------------- ");
                   }
                 sprintf(temp,"%17.12G", ReturnAmp[TNo][0]);
                 strcat(str,temp);
                 if (Comma) strcat(str,","); else strcat(str," ");
               }
             if (((VisibleOpt >> 7) & 1) == 1)
               {
                 if (!HeaderSetup)
                   {
                     strcat(Head1,"  Radial Velocity ");
                     sprintf(temp,"  (Target %4ld)   ",TNo+1);
                     strcat(Head2,temp);
                     strcat(Head3,"      (m/s)       ");
                     strcat(Head4,"----------------- ");
                   }
                 sprintf(temp,"%17.11G", TargetRadialVel[TNo][0]);
                 strcat(str,temp);
                 if (Comma) strcat(str,","); else strcat(str," ");
               }
             } // end if in range
           } // end for TNo

           // free arrays allocated
           Free_DVector(PulseFreq,0);
           Free_DMatrix(RangeDelay,0,0);
           Free_DMatrix(TargetRadialVel,0,0);
           Free_DMatrix(ReturnAmp,0,0);
           Free_DMatrix3(SurfP,0,0,0);
       } // end if

    // on first iteration add header to TMemo
    if (!HeaderSetup)
      {
        HeaderSetup = true;
        fprintf(OutFilep,"%s\n",Head1);
        fprintf(OutFilep,"%s\n",Head2);
        fprintf(OutFilep,"%s\n",Head3);
        fprintf(OutFilep,"%s\n",Head4);
       // GeoList->Lines->Add(Head1);
       // GeoList->Lines->Add(Head2);
       // GeoList->Lines->Add(Head3);
       // GeoList->Lines->Add(Head4);
      }
    // transfer string to TMemo
   // GeoList->Lines->Add(str);
    fprintf(OutFilep,"%s\n",str);

   } // end for PNo

  // free arrays allocated
  Free_DVector(PulseSendTime,0);
  fclose(OutFilep);
  return 0;
}
//-------------------------------------------------------------------------//
double FindValuedB(double Value, double Max)
{
  double temp,D;
  // divide by maximum
  if (Max !=0 )
    temp =  fabs(Value) / Max;
  else
    temp = MAX_FLOAT;
  if (temp > 0)
    {
      D = 20*log10(temp);
      if  (D < -DYNAMIC_RANGE) D = -DYNAMIC_RANGE;
    }
  else
    D = -DYNAMIC_RANGE;
  return D;
}
/*
//-------------------------------------------------------------------------//
void FindRadar(char *RadarName, struct SRadar **Radar,
  struct SRadar *FirstRadar)
{
  struct SRadar *R;
  R = FirstRadar;
  // in case name not defined return NULL
  *Radar =  NULL;
  while (R != NULL)
	{
	  if (stricmp((const char *)R->RadarName, (const char *)RadarName) == 0)
	    { *Radar = R; break; }
	  R = R->next;
	}
}
*/
//-------------------------------------------------------------------------//
double FindBandwidth(struct SRadar *r)
{
  double BW;
  // find bandwidth
 	if ((r->PulseType == MONO) || (r->PulseType == BARKER)) BW = 0;      // mono pulse
	else if (r->PulseType == CHIRP) BW = r->ChirpBW;   // chirp pulse
  else BW = r->ChirpBW;

  return BW;
}



