#include "dest.h"
//---------------------------------------------------------------------------

#ifdef COMPILE_FOR_WINDOWS32
  #include <vcl\vcl.h>
  #pragma hdrstop
  #include "main.h"
#endif
//---------------------------------------------------------------------------

#include "loadsavedata.h"
#include "general.h"
#include "parser.h"
#include "engine.h"
#include "objectstructdef.h"

//---------------------------------------------------------------------------
bool LoadDataFile(char FileName[],SDataDefinition *DataDef)
{
  FILE *OutFilep;
  double *DataArray;
  char s[MAXPATH];
  char DataString[MAX_LENGTH_OF_PARA_STRING];
  long ItemCounter, CurrentItem;
  int CurrentDataType;
  long i;
  int ItemCode, LineCount = 0;
  bool Error;

  OutFilep = fopen(FileName, "rt");  // read from file, text
  if (!OutFilep)
	 {
		sprintf(s,"Cannot open file '%s'",FileName);
    #ifdef COMPILE_FOR_WINDOWS32
		Application->MessageBox(s, "Error", MB_OK | MB_ICONEXCLAMATION);
    #else
    printf("%s\n",s);
    #endif
		Error = TRUE;
		return Error;
	 }
  // allocate memory to read whole file
  DataArray = DVector(0,(MAX_SAMPLES * MAX_VARIABLES_PER_GRAPH * 2)-1);
  ItemCounter = 0;
  // now load data
  do
	 {
		// Get the _ITEM_ code of the next item found in command file
		ItemCode = ParseOneItem(OutFilep, DataString, LineCount+1 );
		if ((ItemCode != _ITEM_EOF) && (ItemCode != _ITEM_EOL))
		 DataArray[ItemCounter++] = atof(DataString);
		if (ItemCounter >= MAX_SAMPLES)
		 {
			sprintf(s,"Too many items in file, limit = %d - truncated",MAX_SAMPLES);
      #ifdef COMPILE_FOR_WINDOWS32
	  	Application->MessageBox(s, "Error", MB_OK | MB_ICONEXCLAMATION);
      #else
      printf("%s\n",s);
      #endif
			break;
		 }
	 }  // end do
  while ( ItemCode != _ITEM_EOF );
  fclose (OutFilep);
  // check if data is OK
  CurrentItem = 0;
  CurrentDataType = 0;
  Error = FALSE;
  if (ItemCounter == 0)
	 {
    sprintf(s,"No items found in file");
    #ifdef COMPILE_FOR_WINDOWS32
		Application->MessageBox(s, "Error", MB_OK | MB_ICONEXCLAMATION);
    #else
    printf("%s\n",s);
    #endif
		Error = TRUE;
		return Error;
	 }
  else
	 {
		while ((CurrentItem < ItemCounter) && (CurrentDataType < DataDef->DataTypes))
		  {
			 if (DataArray[CurrentItem] != round(DataArray[CurrentItem]))
				{
				  sprintf(s,"Number of samples for %s must be an integer (not %-.6G)",
								DataDef->YAxis[CurrentDataType].Name,DataArray[CurrentItem]);
          #ifdef COMPILE_FOR_WINDOWS32
	      	Application->MessageBox(s, "Error", MB_OK | MB_ICONEXCLAMATION);
          #else
          printf("%s\n",s);
          #endif

				  Error = TRUE;
				  return Error;
				}
			 if (DataArray[CurrentItem] < 1)
				{
				  sprintf(s,"Number of samples for %s must be greater than 0 (not %-.6G)",
							DataDef->YAxis[CurrentDataType].Name,DataArray[CurrentItem]);
          #ifdef COMPILE_FOR_WINDOWS32
		      Application->MessageBox(s, "Error", MB_OK | MB_ICONEXCLAMATION);
          #else
          printf("%s\n",s);
          #endif
				  Error = TRUE;
				  return Error;
				}
			 if ((CurrentItem + (DataArray[CurrentItem]*2)+1) > ItemCounter)
				{
				  sprintf(s,"%.0f items for %s expected, only %d found",DataArray[CurrentItem]*2,
				  DataDef->YAxis[CurrentDataType].Name, (int)(ItemCounter - CurrentItem - 1));
          #ifdef COMPILE_FOR_WINDOWS32
	      	Application->MessageBox(s, "Error", MB_OK | MB_ICONEXCLAMATION);
          #else
          printf("%s\n",s);
          #endif
				  Error = TRUE;
				  return Error;
				}
			 CurrentItem += (long)(DataArray[CurrentItem]*2)+1;
			 CurrentDataType++;
		  } // end while
		if ((CurrentDataType < DataDef->DataTypes) && !Error)
		 {
			sprintf(s,"%d Datatypes (%s etc.) expected, only %d found",DataDef->DataTypes,
					  DataDef->YAxis[0].Name, CurrentDataType);
      #ifdef COMPILE_FOR_WINDOWS32
	  	Application->MessageBox(s, "Error", MB_OK | MB_ICONEXCLAMATION);
      #else
      printf("%s\n",s);
      #endif
			Error = TRUE;
		  }
	 } // end else
  if (Error)
    {
   	  sprintf(s,"File was not loaded");
      #ifdef COMPILE_FOR_WINDOWS32
		  Application->MessageBox(s, "Error", MB_OK | MB_ICONEXCLAMATION);
      #else
       printf("%s\n",s);
      #endif
    }
  else
	{
	  // now really insert data into array
	  CurrentItem = 0;
	  CurrentDataType = 0;
	  while ((CurrentItem < ItemCounter) && (CurrentDataType < DataDef->DataTypes))
		 {
			// free old data arrays
			Free_DVector(DataDef->XAxis[CurrentDataType].DataArray,0);
			Free_DVector(DataDef->YAxis[CurrentDataType].DataArray,0);
			Free_DMatrix(DataDef->Coeff[CurrentDataType],0,0);
				// number of samples
			DataDef->NoP[CurrentDataType] = (long)DataArray[CurrentItem++];
				// allocate memory for the data
			DataDef->XAxis[CurrentDataType].DataArray = DVector(0,DataDef->NoP[CurrentDataType]-1);
			DataDef->YAxis[CurrentDataType].DataArray = DVector(0,DataDef->NoP[CurrentDataType]-1);
			DataDef->Coeff[CurrentDataType] = DMatrix(0,DataDef->NoP[CurrentDataType]-1,0,3);
				// copy data from temporary array
			for (i = 0; i < DataDef->NoP[CurrentDataType]; i++)
			  {
				 DataDef->XAxis[CurrentDataType].DataArray[i] = DataArray[CurrentItem++];
				 DataDef->YAxis[CurrentDataType].DataArray[i] = DataArray[CurrentItem++];
			  }
			// update interpolation coeffs
		  //	if (IntGroup->ItemIndex == CUBIC)
              FindPolyCoeff(DataDef->XAxis[CurrentDataType].DataArray,
							  DataDef->YAxis[CurrentDataType].DataArray,
				 DataDef->NoP[CurrentDataType], (DataDef->Coeff[CurrentDataType]));
			// next data type
			CurrentDataType++;
		 } // end while
	}  // end else
  // free memory allocated
  Free_DVector(DataArray,0);
  return Error;
}
//---------------------------------------------------------------------------
void OpenFile(char *FileName, struct LinkedLists *FirstObjectP)
{
  char FunctionNames[MAX_FUNCTIONS][12] =
    { "TARGET", "PLATFORM", "RADAR", "SIMULATION", "SURFACE", "GEOMETRY"};
  char s[100];
  FILE *OutFilep;
  char ParameterList [MAX_PARMS][MAX_LENGTH_OF_PARA_STRING];
  char DataString[MAX_LENGTH_OF_PARA_STRING];
  int  ParameterCount =  0,
		 LineCount      =  0,
		 LastFunctionLineCount = 0,
		 LastFunction   = -1;

  char NewFunction = FALSE;
  int ItemCode;

  OutFilep = fopen(FileName, "rt");  // read text file
  if (!OutFilep)
	 {
		 sprintf(s,"Cannot open file !");
     #ifdef COMPILE_FOR_WINDOWS32
	   Application->MessageBox(s, "Error", MB_OK | MB_ICONEXCLAMATION);
     #else
     printf("%s\n",s);
     #endif
	 }
  else
	 {
		// now load data
		do
		  {
			 // Get the _ITEM_ code of the next item found in command file
			 ItemCode = ParseOneItem(OutFilep, DataString, LineCount+1 );
			 switch( ItemCode )
				{
				  case _ITEM_EOL    : LineCount++; break;

				  case _ITEM_EOF       :
				  case _ITEM_FUNCTION  :
					 // Call init routine for last function that was found
					 if ( NewFunction == TRUE )
						{
						  ExecuteCommand(LastFunction, ParameterList,
											ParameterCount, LastFunctionLineCount+1,FirstObjectP);
						  LastFunctionLineCount = LineCount;
						}
					 // Reset relevant counters and flags
					 if ( ItemCode != _ITEM_EOF )
						{
						  NewFunction    = TRUE;
						  ParameterCount =  0;
						  LastFunction   =  0;

						  // Find function index of the newly found function call
						  while (LastFunction < MAX_FUNCTIONS)
							 {
								if (CompStrNI(DataString, FunctionNames[LastFunction],5 ) == 0)
								  break;
								else
								  LastFunction++;
							 }
						  // function not found
						  if ( LastFunction >= MAX_FUNCTIONS )
							  ErrorM(ERROR_UNKNOWN_FUNC, LineCount+1, 0, DataString );

						} // end if
					 break;

				  case _ITEM_PARAMETER :
					 // Save the newly found parameter in the parameter list
					 strcpy( ParameterList[ParameterCount++], DataString );

					 if ( ParameterCount >= MAX_PARMS )
						{
						  ErrorM(ERROR_TOO_MANY_PARM, LineCount+1,
									 (long)MAX_PARMS,NULL );
						  ParameterCount--;
						}

				  default         : break;
				}  // end switch
		 }  // end do
	  while ( ItemCode != _ITEM_EOF );
	  fclose (OutFilep);
	} // end else
}
#ifdef COMPILE_FOR_WINDOWS32
//---------------------------------------------------------------------------
void SaveFile(char *FileName)
{
  // save to a memo box
 // TMemo *AMemoBox = new TMemo(MainForm);

  FILE *OutFilep;
  int i;

  OutFilep = fopen(FileName, "wt");  // append to file, if not exists, create it
  if (!OutFilep)
	 {
		Application->MessageBox("Cannot write file !","Error", MB_OK | MB_ICONEXCLAMATION);
	 }
 	// now save data
  FillMemo(MainForm->Memo1);
  for (i=0; i<MainForm->Memo1->Lines->Count;i++)
    {
      fprintf(OutFilep,MainForm->Memo1->Lines->Strings[i].c_str());
      fprintf(OutFilep,"\n");
    }

  fclose(OutFilep);
 // delete AMemoBox;

/*

  FILE *OutFilep;
  struct STarget *t;
  struct SPlatform *p;
  struct SRadar *r;
  struct SSurface *sur;
  struct SSimulation *sim;
  char s[255];
  int i;

  OutFilep = fopen(FileName, "wt");  // append to file, if not exists, create it
  if (!OutFilep)
	 {
		Application->MessageBox("Cannot write file !","Error", MB_OK | MB_ICONEXCLAMATION);
	 }
  else
	 {
		// now save data
    sprintf(s,"! Radar Simulator SARSIM II version (%s)\n",version);
		fprintf(OutFilep,s);
    sprintf(s,"! (C) 1997 UCT (Radar Remote Sensing Group)");
		fprintf(OutFilep,"\n\n");

		p=MainForm->FirstPlatform;
		while (p != NULL)
		  {
			 fprintf(OutFilep,"$PLATFORM  ");
			 fprintf(OutFilep,"%s\n", p->Name);
			 if (p->Stat_Traj == 0)
				{
				  // stationary target
				  fprintf(OutFilep,"STATIONARY\n");
				  fprintf(OutFilep,"%-20.6G ! (m) X-Position\n",p->SPos[0]);
				  fprintf(OutFilep,"%-20.6G ! (m) Y-Position\n",p->SPos[1]);
				  fprintf(OutFilep,"%-20.6G ! (m) Z-Position\n",p->SPos[2]);
				}
			 else
				{
				  // trajectory
				  fprintf(OutFilep,"TRAJECTORY\n");
				  WriteDataDef(OutFilep, &(p->TrajDef));
				  if (p->Align == 1)
					 fprintf(OutFilep,"ALIGNED\n");
				  else
					 fprintf(OutFilep,"NOT_ALIGNED\n");
				}
			 if (p->RotType == 0)
				{
				  // fixed rotation angles
				  fprintf(OutFilep,"CONST\n");
   			  fprintf(OutFilep,"%-20.6G ! (deg) X-axis Rotation (deg)\n",p->Rot[0]*RadToDeg);
			    fprintf(OutFilep,"%-20.6G ! (deg) Y-axis Rotation (deg)\n",p->Rot[1]*RadToDeg);
			    fprintf(OutFilep,"%-20.6G ! (deg) Z-axis Rotation (deg)\n",p->Rot[2]*RadToDeg);
				}
			 else
				{
				  // user-defined rotation angles
				  fprintf(OutFilep,"OTHER\n");
				  WriteDataDef(OutFilep, &(p->RotDef));
				}
			 fprintf(OutFilep,"%-20.6G ! (deg) X-axis Rotation rate (deg/s)\n",p->RotRate[0]*RadToDeg);
			 fprintf(OutFilep,"%-20.6G ! (deg) Y-axis Rotation rate (deg/s)\n",p->RotRate[1]*RadToDeg);
			 fprintf(OutFilep,"%-20.6G ! (deg) Z-axis Rotation rate (deg/s)\n",p->RotRate[2]*RadToDeg);
			 if (p->MotionErrorPos == 0)
				{
				  // use only std dev
				  fprintf(OutFilep,"STDDEV\n");
				  fprintf(OutFilep,"%-20.6G ! (m) X-Position standard deviation\n",p->PosDev[0]);
				  fprintf(OutFilep,"%-20.6G ! (m) Y-Position standard deviation\n",p->PosDev[1]);
				  fprintf(OutFilep,"%-20.6G ! (m) Z-Position standard deviation\n",p->PosDev[2]);
				}
			 else
				{
				  // user defined
				  fprintf(OutFilep,"OTHER                       ! User defined position motion errors\n");
				  WriteDataDef(OutFilep, &(p->PosDevDef));
				}
			 if (p->MotionErrorRot == 0)
				{
				  // use only std dev
				  fprintf(OutFilep,"STDDEV\n");
				  fprintf(OutFilep,"%-20.6G ! (deg) X-axis Rotation standard deviation\n",p->RotDev[0]*RadToDeg);
				  fprintf(OutFilep,"%-20.6G ! (deg) Y-axis Rotation standard deviation\n",p->RotDev[1]*RadToDeg);
				  fprintf(OutFilep,"%-20.6G ! (deg) Z-axis Rotation standard deviation\n",p->RotDev[2]*RadToDeg);
				}
			 else
				{
				  // user defined
				  fprintf(OutFilep,"OTHER                       ! User defined rotation motion errors\n");
				  WriteDataDef(OutFilep, &(p->RotDevDef));
				}

			 fprintf(OutFilep,"\n");
			 p = p->next;                     // goto next platform
		  }

		fprintf(OutFilep,"\n");
		sur=MainForm->FirstSurface;
		while (sur != NULL)
		  {
			 fprintf(OutFilep,"$SURFACE\n");
			 fprintf(OutFilep,"%s  ! Platform\n",sur->Name);
       for (i=0;i<3;i++)
         {
			     fprintf(OutFilep,"%-.6G, ", sur->Tri[i][0]);
			     fprintf(OutFilep,"%-.6G, ", sur->Tri[i][1]);
			     fprintf(OutFilep,"%-.6G  ! Point %d X,Y,Z (m)\n", sur->Tri[i][2],i+1);
         }
			 fprintf(OutFilep,"\n");
			 sur = sur->next;
		  } // end while




		fprintf(OutFilep,"\n");
		t=MainForm->FirstTarget;
		while (t != NULL)
		  {
			 fprintf(OutFilep,"$TARGET ");
			 fprintf(OutFilep,"%s  ! Platform\n",t->Name);
			 fprintf(OutFilep,"%-.6G, ", t->Pos[0]);
			 fprintf(OutFilep,"%-.6G, ", t->Pos[1]);
			 fprintf(OutFilep,"%-.6G  ! Position X,Y,Z (m)\n", t->Pos[2]);
			 fprintf(OutFilep,"%-.6G, ", t->PosDev[0]);
			 fprintf(OutFilep,"%-.6G, ", t->PosDev[1]);
			 fprintf(OutFilep,"%-.6G  ! Position standard deviation X,Y,Z (m)\n", t->PosDev[2]);
			 fprintf(OutFilep,"%-.6G, ", t->RCS);
			 fprintf(OutFilep,"%-.6G  ! Radar cross section (m*m), RCS std. dev. (m*m)\n", t->RCSdev);
			 if (t->ReflecType == ISOTROPIC)
				{
				  // Isotropic point target
				  fprintf(OutFilep,"ISOTROPIC\n");
				}
			 else
				{
				  // trajectory
				  fprintf(OutFilep,"DIRECTIONAL\n");
				  fprintf(OutFilep,"%-.6G, ", t->DirAzi*RadToDeg);
				  fprintf(OutFilep,"%-.6G, ", t->DirEle*RadToDeg);
				  fprintf(OutFilep,"%-.6G, ", t->DirAziDev*RadToDeg);
				  fprintf(OutFilep,"%-.6G  ! Azi, Ele, std. dev. of Azi, std. dev. of Ele (deg.)\n", t->DirEleDev*RadToDeg);
				  if (t->GainType == 0) // cos
					 fprintf(OutFilep,"COS_GAIN\n");
				  else
					 {
						fprintf(OutFilep,"OTHER_GAIN\n");
						WriteDataDef(OutFilep, &(t->DataDef));
					 }
				}

			 fprintf(OutFilep,"\n");
			 t = t->next;
		  } // end while

		fprintf(OutFilep,"\n\n");
		r=MainForm->FirstRadar;
		while (r != NULL)
		  {
			 fprintf(OutFilep,"$RADAR\n");
			 fprintf(OutFilep,"%-20s ! Name of radar\n", r->RadarName);
			 fprintf(OutFilep,"%-20s ! Platform name of radar\n", r->PlatformName);
			 if (r->PulseType == MONO)
				fprintf(OutFilep,"MONO\n");
			 else if (r->PulseType == CHIRP)
				{
				  fprintf(OutFilep,"CHIRP\n");
				  fprintf(OutFilep,"%-20.6G ! (GHz) Chirp bandwidth\n", r->ChirpBW*1E-9);
				}
			 else if (r->PulseType == BARKER)
				{
				  fprintf(OutFilep,"BARKER\n");
				  fprintf(OutFilep,"%-20d ! Barker code\n", r->BarkerCode);
				}
			 else // user defined pulse
				{
				  fprintf(OutFilep,"%-20s ! User defined pulse\n","OTHER_PULSE");
				  WriteDataDef(OutFilep, &(r->PulseDef));
				}
			 fprintf(OutFilep,"%-20.6G ! (ns)  zero to zero Pulse width\n", r->PulseWidth*1E9);
			 if (r->Envelope == 0)
				fprintf(OutFilep,"%-20s ! Rectangular envelope\n","RECT");
			 else if (r->Envelope == 1)
				{
				  fprintf(OutFilep,"%-20s ! Linear ramp envelope\n","LINEAR");
				  fprintf(OutFilep,"%-20.6G ! (ns)  Pulse rise time\n", r->RiseTime*1E9);
				  fprintf(OutFilep,"%-20.6G ! (ns)  Pulse fall time\n", r->FallTime*1E9);
				}
			 else // user defined envelope
				{
				  fprintf(OutFilep,"%-20s ! User defined envelope\n","OTHER");
				  WriteDataDef(OutFilep, &(r->EnvelopeDef));
				}
			 if (r->PRFType == 0)  // constant PRI
				{
				  fprintf(OutFilep,"%-20s ! Constant PRI\n","CONSTANT");
				  fprintf(OutFilep,"%-20.6G ! (Hz)  Pulse repetition frequency\n", r->PRF);
				}
			 else // user defined PRI
				{
				  fprintf(OutFilep,"%-20s ! User defined PRIs\n","OTHER");
				  WriteDataDef(OutFilep, &(r->PRFDef));
				}
			 if (r->FreqType == 0) // single frequency
				{
				  fprintf(OutFilep,"%-20s ! Constant frequency\n","SINGLE");
				  fprintf(OutFilep,"%-20.6G ! (GHz)  Center frequency\n", r->SingleFreq*1E-9);
				}
			 else if (r->FreqType == 1)   // stepped frequency
				{
				  fprintf(OutFilep,"%-20s ! Stepped frequency\n","STEPPED");
				  fprintf(OutFilep,"%-20.6G ! (GHz) Start frequency\n", r->StartFreq*1E-9);
				  fprintf(OutFilep,"%-20.6G ! (GHz) Frequency step size\n", r->StepSize*1E-9);
				  fprintf(OutFilep,"%-20.0d ! Number of frequency steps\n", r->FreqSteps);
				  fprintf(OutFilep,"%-20.0d ! Pulses per frequency\n", r->PulsesPerFreq);
				}
			 else // user defined frequencies
				{
				  fprintf(OutFilep,"%-20s ! User defined frequencies\n","OTHER");
				  WriteDataDef(OutFilep, &(r->FreqDef));
				}
			 fprintf(OutFilep,"%-20.6G ! (kW)  Power output\n" , r->PowerOutput*1E-3);
			 fprintf(OutFilep,"%-20.6G ! (dB)  Total system losses\n", FacTodB(r->Losses));
			 fprintf(OutFilep,"%-20.6G ! (K)   Noise temperature\n", r->NoiseTemp);
			 if (r->AntennaGainTypeT == 0) // Isotropic
				{
				  fprintf(OutFilep,"%-20s ! Isotropic transmitter antenna\n","ISOTROPIC");
				}
			 else if (r->AntennaGainTypeT == 1)   // simple (sinx/x)
				{
				  fprintf(OutFilep,"%-20s ! Simple sin(x)/x transmitter antenna\n","SINX");
				  fprintf(OutFilep,"%-20.6G ! (deg) Elevation beam width\n", r->ElevBeamWidthT*RadToDeg);
				  fprintf(OutFilep,"%-20.6G ! (deg) Azimuth beam width\n", r->AziBeamWidthT*RadToDeg);
				}
			 else // user defined antenna gain
				{
				  fprintf(OutFilep,"%-20s ! User defined transmitter antenna\n","OTHER");
				  WriteDataDef(OutFilep, &(r->AntennaTDef));
				}
			 if (r->AntennaGainTypeR == 0) // Isotropic
				{
				  fprintf(OutFilep,"%-20s ! Isotropic receiver antenna\n","ISOTROPIC");
				}
			 else if (r->AntennaGainTypeR == 1)   // simple (sinx/x)
				{
				  fprintf(OutFilep,"%-20s ! Simple sin(x)/x receiver antenna\n","SIN");
				  fprintf(OutFilep,"%-20.6G ! (deg) Elevation beam width\n", r->ElevBeamWidthR*RadToDeg);
				  fprintf(OutFilep,"%-20.6G ! (deg) Azimuth beam width\n", r->AziBeamWidthR*RadToDeg);
				}
			 else if (r->AntennaGainTypeR == 2)   // same as transmitter
				{
				  fprintf(OutFilep,"%-20s ! Same as transmitter antenna\n","SAME");
				}
			 else // user defined antenna gain
				{
				  fprintf(OutFilep,"%-20s ! User defined receiver antenna\n","OTHER");
				  WriteDataDef(OutFilep, &(r->AntennaRDef));
				}
			 if (r->BeamDirection == 0) // fixed
				{
				  fprintf(OutFilep,"%-20s ! Fixed antenna direction\n","FIXED");
				  fprintf(OutFilep,"%-20.6G ! (deg) Beam-direction - Elevation\n", r->ElevationAngleF*RadToDeg);
				  fprintf(OutFilep,"%-20.6G ! (deg) Beam-direction - Azimuth\n", r->AzimuthAngleF*RadToDeg);
				}
			 else if (r->BeamDirection == 1)   // rotating antenna
				{
				  fprintf(OutFilep,"%-20s ! Rotating antenna\n","ROTATING");
				  fprintf(OutFilep,"%-20.6G ! (deg) Beam-direction - Elevation\n", r->ElevationAngleR*RadToDeg);
				  fprintf(OutFilep,"%-20.6G ! (deg/s) Rotation rate\n", r->RotationRate*RadToDeg);
				}
			 else if (r->BeamDirection == 2)   // spot mode
				{
				  fprintf(OutFilep,"%-20s ! Spot mode antenna\n","SPOT");
				  fprintf(OutFilep,"%-20.6G ! (m) Spot X-Position\n",r->Spot[0]);
				  fprintf(OutFilep,"%-20.6G ! (m) Spot Y-Position\n",r->Spot[1]);
				  fprintf(OutFilep,"%-20.6G ! (m) Spot Z-Position\n",r->Spot[2]);
				}
			 else // other direction
				{
				  fprintf(OutFilep,"%-20s ! User defined direction\n","OTHER");
				  WriteDataDef(OutFilep, &(r->DirectionTDef));
				}
			 if (r->MatchedFilterW == 0) // rect
				{
				  fprintf(OutFilep,"%-20s ! Rectangular MF window\n","RECT");
				}
			 else if (r->MatchedFilterW == 1)   // hanning
				{
				  fprintf(OutFilep,"%-20s ! Hanning MF window\n","HANNING");
				  fprintf(OutFilep,"%-20.6G ! Hanning constant\n", r->HanningC);
				}
			 else if (r->MatchedFilterW == 2)   // bartlett
				{
				  fprintf(OutFilep,"%-20s ! Bartlett MF window\n","BARTLETT");
				}
			 else if (r->MatchedFilterW == 3)   // hamming
				{
				  fprintf(OutFilep,"%-20s ! Hamming MF window\n","HAMMING");
				}
			 else // other
				{
				  fprintf(OutFilep,"%-20s ! User defined MF window\n","OTHER");
				  WriteDataDef(OutFilep, &(r->MFDef));
				}

			 if (r->ApplyAGC) // sensitivity time control
				{
				  fprintf(OutFilep,"%-20s ! apply sensitivity time control\n","APPLY_STC");
				  if (r->AGCType == 0)
					 fprintf(OutFilep,"%-20s ! multiply by R^4\n","1OVERR4");
				  else
					 {
						fprintf(OutFilep,"%-20s ! User define STC function\n","OTHER");
						WriteDataDef(OutFilep, &(r->AGCDef));
					 }
				}
			 else // no STC
				{
				  fprintf(OutFilep,"%-20s ! no STC\n","NO_STC");
				}
				  fprintf(OutFilep,"\n\n");

				  r = r->next;
				} // end while
			 sim=MainForm->FirstSimulation;
			 while (sim != NULL)
				{
				  fprintf(OutFilep,"$SIMULATION\n");
				  fprintf(OutFilep,"%-20s ! Name of radar\n", sim->RadarName);
				  fprintf(OutFilep,"%-20.6G ! (m) Slant range start\n", sim->SlantStart);
				  fprintf(OutFilep,"%-20.6G ! (m) Slant range end\n", sim->SlantEnd);
				  fprintf(OutFilep,"%-20.6G ! (s) Azimuth range start\n", sim->AzimuthStart);
				  fprintf(OutFilep,"%-20.6G ! (s) Azimuth range end\n", sim->AzimuthEnd);

				  fprintf(OutFilep,"%-20d ! A/D bit accuracy\n", sim->A2Dbits);
				  fprintf(OutFilep,"%-20.6G ! (GHz) Sample frequency\n", sim->SampleFreq*1E-9);
				  fprintf(OutFilep,"%-20.9G ! (mV) Least Significant Bit value\n", sim->LSBvalue*1E3);
				  if (sim->FileType == 0) sprintf(s,"ASCII");
				  else sprintf(s,"BINARY");
				  fprintf(OutFilep,"%-20s ! Save file format\n", s);
				  if (sim->SimTYPE == 0) sprintf(s,"RAW");
				  else if (sim->SimTYPE == 1) sprintf(s,"MATCHED_FILTER");
				  else if (sim->SimTYPE == 2) sprintf(s,"SRP");
				  fprintf(OutFilep,"%-20s ! Processing\n", s);
				  fprintf(OutFilep,"%-20s ! File Name of output file\n", sim->FileName);

				  long PointsAzi, PointsSlant;
				  FindPixelAziSlant(&PointsAzi, &PointsSlant, sim);
				  fprintf(OutFilep,"%-20s ! Size : %d (Azimuth) x %d (Slant Range) points\n",
				  "",PointsAzi,PointsSlant);
				  if (sim->PTPos == TRUE) sprintf(s,"CENTRE");
				  else sprintf(s,"BEGIN");
				  fprintf(OutFilep,"%-20s ! Point Target position relative to pulse\n", s);
				  fprintf(OutFilep,"\n\n");

				  sim = sim->next;
				} // end while

			  fclose(OutFilep);
		  }  // end else
*/
}
#endif
/*
//---------------------------------------------------------------------------
void WriteDataDef(FILE *OutFilep, struct SDataDefinition *DataDef)
{
  long i,j;
  char s[256];

  for (i=0;i<DataDef->DataTypes;i++)
    {
      if (DataDef->IntMethod[i] == CUBIC)
         fprintf(OutFilep,"%-20s ! Interpolation method for %s vs %s\n","CUBIC",
           DataDef->YAxis[i].Name, DataDef->XAxis[i].Name);
      else if (DataDef->IntMethod[i] == FILTER)
         fprintf(OutFilep,"%-20s ! Interpolation method for %s vs %s\n","MFILTER",
           DataDef->YAxis[i].Name, DataDef->XAxis[i].Name);
      else
        fprintf(OutFilep,"%-20s ! Interpolation method for %s vs %s\n","LINEAR",
          DataDef->YAxis[i].Name, DataDef->XAxis[i].Name);
    }
  if (DataDef->Source == ID_FILE)
	 {
		fprintf(OutFilep,"FILE  ");
		fprintf(OutFilep,"%s\n", DataDef->FileName);
	 }
  else if (DataDef->Source == ID_INLINE)
	 {
		fprintf(OutFilep,"INLINE\n");
		for (i=0;i<DataDef->DataTypes;i++)
		  {
			 sprintf(s,"%d                    ! Number of samples for %s vs %s\n",
              DataDef->NoP[i],DataDef->YAxis[i].Name, DataDef->XAxis[i].Name);
			 fprintf(OutFilep,s);
			 for (j=0;j<DataDef->NoP[i];j++)
				{
				  sprintf(s,"%-.15G, %-.15G",DataDef->XAxis[i].DataArray[j],
						DataDef->YAxis[i].DataArray[j]);
				  if (j < (DataDef->NoP[i]-1))
					 strcat(s,"  ");
				  fprintf(OutFilep,s);
				}
			 fprintf(OutFilep,"\n");
		  }

	 }
}
*/
//---------------------------------------------------------------------------
