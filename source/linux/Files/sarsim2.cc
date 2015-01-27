#include <stdio.h>

#include "objectstructdef.h"
#include "globalvariables.h"
#include "loadsavedata.h"
#include "general.h"
#include "linkedlist.h"
#include "engine.h"
#include <time.h>
#include <math.h>
#include <cstring>

// structure containing entry points (first element pointers)
// for all linked lists
struct LinkedLists LLP;

int main(int argc, char *argv[])
{
  // setup pointers
  LLP.FirstSimulation = NULL;
  LLP.FirstPlatform = NULL;
  LLP.FirstTarget = NULL;
  LLP.FirstRadar = NULL;
  LLP.FirstSurface = NULL;
  LLP.FirstGeometry = NULL;
  printf("\nRadar Simulator (C) 1997,1998 UCT Radar Remote Sensing Group\n"
         "    (Version : %s - written by R.L. with help of J. Horrell and R.T. Lord)\n"
         "    (Web     : http://rrsg.ee.uct.ac.za)\n", version);
   
  char ScriptFileName[100];
  if (argc <= 1 || argc >2)
    {
      printf("\nUsage : sarsim2 [scriptfile]\n\n");
      return -1;       
    }
  strcpy(ScriptFileName, (char *) *++argv);
  printf("\nInterpreting script file : %s\n",ScriptFileName);

  
  OpenFile(ScriptFileName, &LLP);
  printf("  %d Point target(s) specified\n",CountTargets(LLP.FirstTarget));
  printf("  %d Platform(s) specified\n",CountPlatforms(LLP.FirstPlatform));
  printf("  %d Radar(s) specified\n",CountRadars(LLP.FirstRadar));
  printf("  %d Surface(s) specified\n",CountSurfaces(LLP.FirstSurface));
  printf("  %d Simulation file(s) to written\n",CountSimulations(LLP.FirstSimulation));
  printf("  %d Geometry file(s) to be written\n",CountGeometrys(LLP.FirstGeometry)); 
  printf("\n");
  
  if (CountSimulations(LLP.FirstSimulation) == 0)
    {
      printf("No simulation to be done...\n\n");
      return 1;
    }  
  struct SSimulation *CurrentSim = LLP.FirstSimulation;
  int ErrorM,i;
  long PointsAzi, PointsSlant;
  time_t StartTime,EndTime;
  double MaxMagnitude, TotalSamples, ClippedSamples, ZeroSamples;
  double SatPer,ZeroPer,MaxPossibleMag,Percentage,BestLSB;
  // perform all simulations
  for (i=0;i<CountSimulations(LLP.FirstSimulation);i++)
    {
      printf("Working on simulation file : %s\n",CurrentSim->FileName);
      FindPixelAziSlant(LLP.FirstRadar, &PointsAzi, &PointsSlant, CurrentSim);
      StartTime=time(NULL);
      ErrorM = SaveSimuData(&MaxMagnitude, CurrentSim, &LLP, PointsSlant,
        &TotalSamples, &ClippedSamples, &ZeroSamples);
      if (ErrorM == 2) // radar not found
        {
          printf("Radar '%s' specified in simulation not found.\n",
            CurrentSim->RadarName);
        }    
      else if (ErrorM == 3) // error writing file
        {
          printf("Error writing file %s.\n",CurrentSim->FileName);
        }
      else
        {
          // calculate maximium possible magnitude without clipping
          MaxPossibleMag = CurrentSim->LSBvalue*(0.5*pow(2,CurrentSim->A2Dbits)-1);
          if (MaxPossibleMag != 0)
            Percentage = (MaxMagnitude / MaxPossibleMag)*100.0;
          else
            Percentage = 0;
          BestLSB = MaxMagnitude/(0.5*pow(2,CurrentSim->A2Dbits)-1);

          SatPer = (ClippedSamples / (0.01+TotalSamples))*100;
          ZeroPer = (ZeroSamples / (0.01+TotalSamples))*100;
        
          EndTime=time(NULL);
          printf("Simulation file '%s' successfully written to disk\n"
           "  Size                   : %ld (Azimuth) x %ld (Range)\n"
           "  Calculation time       : %ld second(s)\n"
           "  Maximum amplitude (mV) : %-.9G (%-3.2f %% dynamic range used)\n"
           "  Optimum LSB value (mV) : %-.9G\n" 
           "  Saturated Samples      : %-.6G %%\n"
           "  Zero Samples           : %-.6G %%\n", 
           CurrentSim->FileName,PointsAzi,PointsSlant,EndTime-StartTime,
           MaxMagnitude*KILO,Percentage,BestLSB*KILO,SatPer,ZeroPer);
        }     
    
      CurrentSim = CurrentSim->next;  
    }
 
printf("\n\n");

 struct SGeometry *CurrentGeo = LLP.FirstGeometry; 
 // perform all geometries simulations
  for (i=0;i<CountGeometrys(LLP.FirstGeometry);i++)
    {
      printf("Working on geometry file : %s ",CurrentGeo->FileName);
      ErrorM = SaveGeometryData(CurrentGeo, &LLP);

      if (ErrorM != 0) // error
        {
          printf("Error writing file '%s'.\n",
            CurrentGeo->RadarName);
        }   
      else 
        {
          printf(" - successfully written... \n");
        }
      CurrentGeo = CurrentGeo->next;  
    }
   
  printf("\n\n");
  // free all linked lists
  FreeTargetList(&(LLP.FirstTarget));
  FreeSurfaceList(&(LLP.FirstSurface)); 
  FreePlatformList(&(LLP.FirstPlatform),&(LLP.FirstTarget),&(LLP.FirstSurface),
    &(LLP.FirstRadar));
  FreeRadarList(&(LLP.FirstRadar));
  FreeSimulationList(&(LLP.FirstSimulation));
  
  return 1;
}
