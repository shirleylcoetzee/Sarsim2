//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "CalcThread.h"
#include "simuform.h"
#include "main.h"
#include "engine.h"
#include "linkedlist.h"
#include "math.h"
extern bool UserAbort;
//---------------------------------------------------------------------------
__fastcall TCalcThread::TCalcThread(void) : TThread(True)
{
  // initialize arrays to a NULL pointer to make sure
  PulseSendTime = NULL;
  Data = NULL;
  // progress
  Percentage = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCalcThread::DoVisualUpdate(void)
{
  ParentForm->ProgressBar->Position = (unsigned char)Percentage;
}

//---------------------------------------------------------------------------
void __fastcall TCalcThread::Execute()
{
  long PNo,TNo;                   // loop variables for pulses and targets
  long TargetNo,SurfaceNo;
  long FirstPulse, LastPulse;
  // temporary arrays
  double *PulseFreq;
  double **RangeDelay;
  double **TargetRadialVel;
  double **ReturnAmp;
  double ***SurfP;
  double **RadarDir;

  // calculate number of pulses
  FindPulsesInRange(CurrentSim.AzimuthStart,CurrentSim.AzimuthEnd,CRadar,
                        &FirstPulse,&LastPulse);

  PulseNo = (LastPulse - FirstPulse) + 1;

  if (PulseNo <= 0) PulseNo = 1;

  // There are 2 cases : if the number of pulses is less than the number
  // of pixels (Y-direction), calculate every pulse, otherwise calculate
  // only pulses corresponding to each pixel row.
  if (PulseNo >= PixelY)
    {
		  // set number of pulses equal to the number of lines
		  PulseNo = PixelY;
		  // create array
		  PulseSendTime = DVector(0,PulseNo-1);
		  // calculate time when each pulse is sent
		  // NOTE : this is only an approximation, the PRF is not considered
		  for (PNo=0;PNo<PulseNo;PNo++)
		    PulseSendTime[PNo] = CurrentSim.AzimuthStart +
			   double(PNo)*((CurrentSim.AzimuthEnd-CurrentSim.AzimuthStart)/double(PulseNo));
	  }
  else
    {
		  // create array
		  PulseSendTime = DVector(0,PulseNo-1);
		  // calculate time when each pulse is sent
		  for (PNo=0;PNo<PulseNo;PNo++)
		    PulseSendTime[PNo] = FindPulseSendTime(PNo+FirstPulse, CRadar);
	  }

  // total number of targets
  TargetNo = CountTargets(MainForm->FirstTarget);
  // total number of surfaces
  SurfaceNo = CountSurfaces(MainForm->FirstSurface);
  // create arrays...
  PulseFreq = DVector(0,PulseNo-1);
  RangeDelay = DMatrix(0,TargetNo-1,0,PulseNo-1);
  TargetRadialVel = DMatrix(0,TargetNo-1,0,PulseNo-1);
  ReturnAmp = DMatrix(0,TargetNo-1,0,PulseNo-1);
  SurfP = DMatrix3(0,SurfaceNo-1,0,PulseNo-1,0,8);
  RadarDir = DMatrix(0,PulseNo-1,0,1);


  // calculate range-delay, return amp etc. for each pulse
  CalcGeometry(RangeDelay, ReturnAmp, TargetRadialVel, CRadar, PulseSendTime,
                PulseNo, MainForm->FirstTarget, MainForm->FirstPlatform,
                 MainForm->FirstSurface, SurfP, RadarDir, FirstPulse);


  if (ShowAllTargets)  // this is the special case there we want to
                       // adjust the range such that all targets are shown
    {
      // just to make sure
		  if (TargetNo > 0)
 		    {
			    // select display to show closest target
			    double MinRange = MAX_FLOAT;
			    double MaxRange = 0;

			    for (PNo=0;PNo<PulseNo;PNo++)
	  	  	  for (TNo=0;TNo<TargetNo;TNo++)
				      {
                if (RangeDelay[TNo][PNo] < MinRange)
				    	    MinRange = RangeDelay[TNo][PNo];
					      if (RangeDelay[TNo][PNo] > MaxRange)
						      MaxRange = RangeDelay[TNo][PNo];
			        }
			    CurrentSim.SlantStart = ((MinRange - CRadar->PulseWidth * 2) * LIGHT_SPEED)/2;
			    CurrentSim.SlantEnd = ((MaxRange + CRadar->PulseWidth * 2) * LIGHT_SPEED)/2;
          // check that ranges are within limits and update
          ParentForm->SetNewGraphRange(&(CurrentSim.SlantStart),
               &(CurrentSim.SlantEnd), &(CurrentSim.AzimuthStart),
               &(CurrentSim.AzimuthEnd));
         // ParentForm->AdjustGraphRangeValues(&(CurrentSim.SlantStart),
          //     &(CurrentSim.SlantEnd), &(CurrentSim.AzimuthStart),&(CurrentSim.AzimuthEnd));
	    }
		  else
		    {
			    CurrentSim.SlantStart = 0; CurrentSim.SlantEnd = 10000;
		    }
    } // end if ShowAllTargets

  // calculate the center frequency of each pulse sent
	for (PNo=0;PNo<PulseNo;PNo++)
	  {
	 	  if (PulseNo >= PixelY)
	  		{
	  		  PulseFreq[PNo] = FindPulseFreq(
	  			  FindPulseForTime(PulseSendTime[PNo],CRadar), CRadar);
	  		}
	  	else
	  		PulseFreq[PNo] = FindPulseFreq(PNo+FirstPulse, CRadar);
	  }

  long SamplePoints = PixelX;
	// allocate memory for data array (I and Q therfore SamplePoints*2)
	Data = DMatrix(0,PulseNo-1,0,SamplePoints*2-1);

  UserAbort = FALSE;
  // Calculate Data array of size (PulseNo x SamplePoints)
  CalcArray2(CRadar, &CurrentSim, Data, PulseNo, SamplePoints, PulseFreq,
             RangeDelay, TargetRadialVel, ReturnAmp, &MaxMagnitude,
             MainForm->FirstTarget, MainForm->FirstPlatform,
             MainForm->FirstSurface, SurfP,RadarDir);

  // free arrays allocated
  Free_DVector(PulseFreq,0);
  Free_DMatrix(RangeDelay,0,0);
  Free_DMatrix(TargetRadialVel,0,0);
  Free_DMatrix(ReturnAmp,0,0);
  Free_DMatrix3(SurfP,0,0,0);
  Free_DMatrix(RadarDir,0,0);
}
/*
//---------------------------------------------------------------------------
void __fastcall TCalcThread::CalcGivenArray(double **Data, double *PulseSendTime,
    long PulseNo, class TSimulationForm *ParentForm, bool ShowAllTargets,
    int PixelX, int PixelY,  struct SRadar *CRadar,  struct SSimulation CurrentSim,
  double *MaxMagnitude )
{
      long PNo,TNo;                   // loop variables for pulses and targets
      long TargetNo,SurfaceNo;
      long FirstPulse, LastPulse;
      // temporary arrays
      double *PulseFreq;
      double **RangeDelay;
      double **TargetRadialVel;
      double **ReturnAmp;
      double ***SurfP;

      // calculate number of pulses
      FindPulsesInRange(CurrentSim.AzimuthStart,CurrentSim.AzimuthEnd,CRadar,
                        &FirstPulse,&LastPulse);

      PulseNo = (LastPulse - FirstPulse) + 1;

      if (PulseNo <= 0) PulseNo = 1;

      // There are 2 cases : if the number of pulses is less than the number
      // of pixels (Y-direction), calculate every pulse, otherwise calculate
      // only pulses corresponding to each pixel row.
      if (PulseNo >= PixelY)
	    {
		  // set number of pulses equal to the number of lines
		  PulseNo = PixelY;
		  // create array
		  PulseSendTime = DVector(0,PulseNo-1);
		  // calculate time when each pulse is sent
		  // NOTE : this is only an approximation, the PRF is not considered
		  for (PNo=0;PNo<PulseNo;PNo++)
		    PulseSendTime[PNo] = CurrentSim.AzimuthStart +
			   double(PNo)*((CurrentSim.AzimuthEnd-CurrentSim.AzimuthStart)/double(PulseNo));
	    }
      else
	    {
		  // create array
		  PulseSendTime = DVector(0,PulseNo-1);
		  // calculate time when each pulse is sent
		  for (PNo=0;PNo<PulseNo;PNo++)
		    PulseSendTime[PNo] = FindPulseSendTime(PNo+FirstPulse, CRadar);
	    }

      // total number of targets
      TargetNo = CountTargets(MainForm->FirstTarget);
      // total number of surfaces
      SurfaceNo = CountSurfaces(MainForm->FirstSurface);
      // create arrays...
      PulseFreq = DVector(0,PulseNo-1);
      RangeDelay = DMatrix(0,TargetNo-1,0,PulseNo-1);
      TargetRadialVel = DMatrix(0,TargetNo-1,0,PulseNo-1);
      ReturnAmp = DMatrix(0,TargetNo-1,0,PulseNo-1);
      SurfP = DMatrix3(0,SurfaceNo-1,0,PulseNo-1,0,8);

      // calculate range-delay, return amp etc. for each pulse
      CalcGeometry(RangeDelay, ReturnAmp, TargetRadialVel, CRadar, PulseSendTime,
                  PulseNo, MainForm->FirstTarget, MainForm->FirstPlatform,
                  MainForm->FirstSurface, SurfP);

      if (ShowAllTargets)  // this is the special case there we want to
                           // adjust the range such that all targets are shown
        {
          // just to make sure
		  if (TargetNo > 0)
		    {
			  // select display to show closest target
			  double MinRange = MAX_FLOAT;
			  double MaxRange = 0;

			  for (PNo=0;PNo<PulseNo;PNo++)
	  	  	    for (TNo=0;TNo<TargetNo;TNo++)
				  {
                    if (RangeDelay[TNo][PNo] < MinRange)
				    	  MinRange = RangeDelay[TNo][PNo];
					if (RangeDelay[TNo][PNo] > MaxRange)
						  MaxRange = RangeDelay[TNo][PNo];
			      }
			 CurrentSim.SlantStart = ((MinRange - CRadar->PulseWidth * 2) * LIGHT_SPEED)/2;
			 CurrentSim.SlantEnd = ((MaxRange + CRadar->PulseWidth * 2) * LIGHT_SPEED)/2;
             // check that ranges are within limits
             ParentForm->AdjustGraphRangeValues(&(CurrentSim.SlantStart),
               &(CurrentSim.SlantEnd), &(CurrentSim.AzimuthStart),&(CurrentSim.AzimuthEnd));
		    }
		  else
		    {
			  CurrentSim.SlantStart = 0; CurrentSim.SlantEnd = 10000;
		    }
        } // end if ShowAllTargets

      // calculate the center frequency of each pulse sent
	  for (PNo=0;PNo<PulseNo;PNo++)
	    {
	  	 if (PulseNo >= PixelY)
	  		{
	  		  PulseFreq[PNo] = FindPulseFreq(
	  			  FindPulseForTime(PulseSendTime[PNo],CRadar), CRadar);
	  		}
	  	 else
	  		PulseFreq[PNo] = FindPulseFreq(PNo+FirstPulse, CRadar);
	    }

	  long SamplePoints = PixelX;
	  // allocate memory for data array (I and Q therfore SamplePoints*2)
	  Data = DMatrix(0,PulseNo-1,0,SamplePoints*2-1);

      UserAbort = FALSE;
      // Calculate Data array of size (PulseNo x SamplePoints)
      CalcArray2(CRadar, &CurrentSim, Data, PulseNo, SamplePoints, PulseFreq,
                 RangeDelay, TargetRadialVel, ReturnAmp, MaxMagnitude,
                 MainForm->FirstTarget, MainForm->FirstPlatform,
                 MainForm->FirstSurface, SurfP);

      // free arrays allocated
      Free_DVector(PulseFreq,0);
      Free_DMatrix(RangeDelay,0,0);
      Free_DMatrix(TargetRadialVel,0,0);
      Free_DMatrix(ReturnAmp,0,0);
      Free_DMatrix3(SurfP,0,0,0);
}

*/
