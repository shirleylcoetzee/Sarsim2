#include "dest.h"
//---------------------------------------------------------------------------

#ifdef COMPILE_FOR_WINDOWS32
  #include <vcl\vcl.h>
  #pragma hdrstop
  #include "main.h"
#endif

#include "linkedlist.h"
#include "general.h"
#include <ctype.h>
#include <cstring>

//-------------------------------------------------------------------------//
// Function CopySurface                                                     //
//-------------------------------------------------------------------------//
void CopySurface(struct SSurface *SurfaceDest, struct SSurface *SurfaceSource)
{
  // first copy the easy stuff
  memcpy(SurfaceDest, SurfaceSource, sizeof(struct SSurface));
  // now copy all the pointing stuff
}
//-------------------------------------------------------------------------//
void FreeSurfaceDataDef(struct SSurface *T)
{
 *T = *T;
 // no data defs
}
//-------------------------------------------------------------------------//
// Function AddSurface                                                      //
//-------------------------------------------------------------------------//
bool AddSurface(struct SSurface *Surface, struct SSurface **FirstSurface)
{
  struct SSurface *NewSurface,*LastSurface;

  NewSurface =  (struct SSurface*)malloc(sizeof(struct SSurface));
  if (NewSurface == NULL)
	 return FALSE;
  // copy all parameters
  memcpy(NewSurface,Surface,sizeof(struct SSurface));

  // attach new struct SSurface to list
  if (*FirstSurface == NULL)
	 {
		*FirstSurface = NewSurface;
		NewSurface->next = NULL;
		NewSurface->previous = NULL;
	 }
  else
	 {
		// find last struct SSurface in the list
		LastSurface = *FirstSurface;
		while (LastSurface->next != NULL)
		  LastSurface = LastSurface->next;

		// insert new struct SSurface after last struct SSurface
		LastSurface->next    = NewSurface;
		NewSurface->previous = LastSurface;
		NewSurface->next     = NULL;
	 }
  return TRUE;
}
//-------------------------------------------------------------------------//
// Function DeleteSurface                                                   //
//-------------------------------------------------------------------------//
void DeleteSurface(struct SSurface **FirstSurface, struct SSurface *DestSurface)
{
  struct SSurface *Surface;

  // free all struct SSurfaces of the list
  Surface = *FirstSurface;
  while (Surface != DestSurface)
	 {
		Surface = Surface->next;
	 }

  if (Surface->previous != NULL)
	 (Surface->previous)->next = Surface->next;
  if (Surface->next != NULL)
	 (Surface->next)->previous = Surface->previous;

  if (Surface == *FirstSurface) *FirstSurface = Surface->next;
  FreeSurfaceDataDef(Surface);
  free(Surface);
}

//-------------------------------------------------------------------------//
// Function FreeSurfaceList                                                 //
//-------------------------------------------------------------------------//
void FreeSurfaceList(struct SSurface **FirstSurface)
{
  struct SSurface *Surface,*NextSurface;

  // free all struct SSurfaces of the list
  Surface = *FirstSurface;
  while (Surface != NULL)
	 {
		NextSurface = Surface->next;
		DeleteSurface(FirstSurface,Surface);
		Surface = NextSurface;
	 }
  *FirstSurface = NULL;
}

//-------------------------------------------------------------------------//
// Function PointToSurface                                                  //
//                                                                         //
// This function returns a pointer to the nth (1.st = 1 NOT 0) Surface.                      //
//-------------------------------------------------------------------------//
struct SSurface *PointToSurface(struct SSurface *first, long n)
{
  long count = 0;
  SSurface *t;

  t=first;
  while (t != NULL)
	 {
		count++;
		if (count == n) return t;
		t = t->next;
	 }
  return NULL;
}


//-------------------------------------------------------------------------//
// Function CountSurfaces                                                   //
//                                                                         //
// This function returns the total number of Surfaces in memory.            //
//-------------------------------------------------------------------------//
int CountSurfaces(struct SSurface *first)
{
  int count = 0;
  struct SSurface *t;

  t=first;
  while (t != NULL)
	 {
		count++;
		t = t->next;
	 }
  return count;
}

//-------------------------------------------------------------------------//
// Function FindSurfaceNo                                                  //
//                                                                         //
// This function returns the number of the Surface, given a pointer.        //
// 0 = first
//-------------------------------------------------------------------------//
long FindSurfaceNo(struct SSurface *first, struct SSurface *x)
{
  long count = 0;
  struct SSurface *t;

  t=first;
  while (t != NULL)
	 {
		if (t == x) return count;
		count++;
		t = t->next;
	 }
  return 0;
}

//-----------

//-------------------------------------------------------------------------//
// Function CopyTarget                                                     //
//-------------------------------------------------------------------------//
void CopyTarget(struct STarget *TargetDest, struct STarget *TargetSource)
{
  // first copy the easy stuff
  memcpy(TargetDest, TargetSource, sizeof(struct STarget));
  // now copy all the pointing stuff
  CopyDataDef(&(TargetDest->DataDef),&(TargetSource->DataDef));
}

//-------------------------------------------------------------------------//
// Function CopyDataDef                                                    //
//-------------------------------------------------------------------------//
void CopyDataDef(SDataDefinition *DestDataDef,SDataDefinition *SourceDataDef)
{
  long i,j,k;

  // allocate memory for arrays and copy all data across  
  DestDataDef->Source = SourceDataDef->Source;
  strcpy(DestDataDef->FileName,SourceDataDef->FileName);
  DestDataDef->DataTypes = SourceDataDef->DataTypes;
  for (i=0;i<MAX_VARIABLES_PER_GRAPH;i++)
	 {
        (DestDataDef->IntMethod)[i] = (SourceDataDef->IntMethod)[i];
  		(DestDataDef->Type)[i] = (SourceDataDef->Type)[i];
		(DestDataDef->NoP)[i] = (SourceDataDef->NoP)[i];
		(DestDataDef->CurrentP)[i] = (SourceDataDef->CurrentP)[i];

		strcpy((DestDataDef->XAxis)[i].Name,(SourceDataDef->XAxis)[i].Name);
		(DestDataDef->XAxis)[i].Unit = (SourceDataDef->XAxis)[i].Unit;
		(DestDataDef->XAxis)[i].MultFactor = (SourceDataDef->XAxis)[i].MultFactor;
		(DestDataDef->XAxis)[i].DecPlaces = (SourceDataDef->XAxis)[i].DecPlaces;
		(DestDataDef->XAxis)[i].Start = (SourceDataDef->XAxis)[i].Start;
		(DestDataDef->XAxis)[i].End = (SourceDataDef->XAxis)[i].End;
		(DestDataDef->XAxis)[i].Min = (SourceDataDef->XAxis)[i].Min;
		(DestDataDef->XAxis)[i].Max = (SourceDataDef->XAxis)[i].Max;

		strcpy((DestDataDef->YAxis)[i].Name,(SourceDataDef->YAxis)[i].Name);
		(DestDataDef->YAxis)[i].MultFactor = (SourceDataDef->YAxis)[i].MultFactor;
		(DestDataDef->YAxis)[i].Unit = (SourceDataDef->YAxis)[i].Unit;
		(DestDataDef->YAxis)[i].DecPlaces = (SourceDataDef->YAxis)[i].DecPlaces;
		(DestDataDef->YAxis)[i].Start = (SourceDataDef->YAxis)[i].Start;
		(DestDataDef->YAxis)[i].End = (SourceDataDef->YAxis)[i].End;
		(DestDataDef->YAxis)[i].Min = (SourceDataDef->YAxis)[i].Min;
		(DestDataDef->YAxis)[i].Max = (SourceDataDef->YAxis)[i].Max;

		if (SourceDataDef->NoP[i] >= 0)
		  {
			 DestDataDef->Coeff[i] = DMatrix(0,SourceDataDef->NoP[i]-1,0,3);
			 DestDataDef->XAxis[i].DataArray = DVector(0,SourceDataDef->NoP[i]-1);
			 DestDataDef->YAxis[i].DataArray = DVector(0,SourceDataDef->NoP[i]-1);
			 for (j=0;j<DestDataDef->NoP[i];j++)
				{
				  for (k=0;k<4;k++)
					 {
						(DestDataDef->Coeff[i])[j][k] =
						(SourceDataDef->Coeff[i])[j][k];
					 }
				  DestDataDef->XAxis[i].DataArray[j] =
					 SourceDataDef->XAxis[i].DataArray[j];
				  DestDataDef->YAxis[i].DataArray[j] =
					 SourceDataDef->YAxis[i].DataArray[j];
				}
		  }
		else
		  {
			 DestDataDef->Coeff[i] = NULL;
			 DestDataDef->XAxis[i].DataArray = NULL;
			 DestDataDef->YAxis[i].DataArray = NULL;
		  }
	 }
}

//-------------------------------------------------------------------------//
void FreeTargetDataDef(struct STarget *T)
{
	FreeDataDef(&(T->DataDef));
}

//-------------------------------------------------------------------------//
// Function FreeDataDef                                                    //
//-------------------------------------------------------------------------//
void FreeDataDef(SDataDefinition *DataDef)
{
  int i;
  for (i=0;i<MAX_VARIABLES_PER_GRAPH;i++)
	 {
		if (DataDef->NoP[i] > 0)
		  {
			 // free old data arrays
			 Free_DVector(DataDef->XAxis[i].DataArray,0);
			 Free_DVector(DataDef->YAxis[i].DataArray,0);
			 Free_DMatrix(DataDef->Coeff[i],0,0);
		  }
	 }
}

//-------------------------------------------------------------------------//
// Function AddTarget                                                      //
//-------------------------------------------------------------------------//
bool AddTarget(struct STarget *Target, struct STarget **FirstTarget)
{
  struct STarget *NewTarget,*LastTarget;

  NewTarget =  (struct STarget*)malloc(sizeof(struct STarget));
  if (NewTarget == NULL)
	 return FALSE;
  // copy all parameters
  memcpy(NewTarget,Target,sizeof(struct STarget));

  // attach new struct STarget to list
  if (*FirstTarget == NULL)
	 {
		*FirstTarget = NewTarget;
		NewTarget->next = NULL;
		NewTarget->previous = NULL;
	 }
  else
	 {
		// find last struct STarget in the list
		LastTarget = *FirstTarget;
		while (LastTarget->next != NULL)
		  LastTarget = LastTarget->next;

		// insert new struct STarget after last struct STarget
		LastTarget->next    = NewTarget;
		NewTarget->previous = LastTarget;
		NewTarget->next     = NULL;
	 }
  return TRUE;
}
//-------------------------------------------------------------------------//
// Function DeleteTarget                                                   //
//-------------------------------------------------------------------------//
void DeleteTarget(struct STarget **FirstTarget, struct STarget *DestTarget)
{
  struct STarget *Target;

  // free all struct STargets of the list
  Target = *FirstTarget;
  while (Target != DestTarget)
	 {
		Target = Target->next;
	 }

  if (Target->previous != NULL)
	 (Target->previous)->next = Target->next;
  if (Target->next != NULL)
	 (Target->next)->previous = Target->previous;

  if (Target == *FirstTarget) *FirstTarget = Target->next;
  FreeTargetDataDef(Target);
  free(Target);
}

//-------------------------------------------------------------------------//
// Function FreeTargetList                                                 //
//-------------------------------------------------------------------------//
void FreeTargetList(struct STarget **FirstTarget)
{
  struct STarget *Target,*NextTarget;

  // free all struct STargets of the list
  Target = *FirstTarget;
  while (Target != NULL)
	 {
		NextTarget = Target->next;
		DeleteTarget(FirstTarget,Target);
		Target = NextTarget;
	 }
  *FirstTarget = NULL;
}


//-------------------------------------------------------------------------//
// Function AddPlatform                                              //
//-------------------------------------------------------------------------//
bool AddPlatform(SPlatform *Platform, SPlatform **FirstPlatform)
{
  SPlatform *NewPlatform,*LastPlatform;

  NewPlatform =  (SPlatform*)malloc(sizeof(SPlatform));
  if (NewPlatform == NULL)
	 return FALSE;
  // copy all parameters
  memcpy(NewPlatform,Platform,sizeof(SPlatform));

  // attach new SPlatform to list
  if (*FirstPlatform == NULL)
	 {
		*FirstPlatform = NewPlatform;
		NewPlatform->next = NULL;
		NewPlatform->previous = NULL;
	 }
  else
	 {
		// find last SPlatform in the list
		LastPlatform = *FirstPlatform;
		while (LastPlatform->next != NULL)
		  LastPlatform = LastPlatform->next;

		// insert new SPlatform after last SPlatform
		LastPlatform->next    = NewPlatform;
		NewPlatform->previous = LastPlatform;
		NewPlatform->next     = NULL;
	 }
  return TRUE;
}

//-------------------------------------------------------------------------//
// Function DeletePlatform                                                   //
//-------------------------------------------------------------------------//
void DeletePlatform(SPlatform **FirstPlatform, SPlatform *DestPlatform,
                    STarget **FirstTarget, SRadar **FirstRadar, SSurface **FirstSurface,
                    bool DeleteObjectsOn)
{
  SPlatform *Platform;

  // find SPlatforms on the list
  Platform = *FirstPlatform;
  while (Platform != DestPlatform)
	 {
		Platform = Platform->next;
	 }

  if (Platform->previous != NULL)
	 (Platform->previous)->next = Platform->next;
  if (Platform->next != NULL)
	 (Platform->next)->previous = Platform->previous;

  if (Platform == *FirstPlatform) *FirstPlatform = Platform->next;

  if (DeleteObjectsOn)
    {
       struct STarget *Target, *NextTarget;
       // free all struct STargets on this platform
       Target = *FirstTarget;
       while (Target != NULL)
	     {
		   NextTarget = Target->next;
	       if (CompStrNI(Target->Name,Platform->Name,MAX_NAME_LENGTH) == 0)
		      DeleteTarget(FirstTarget, Target);
		   Target = NextTarget;
	     }

       struct SSurface *Surface, *NextSurface;
       // free all struct SSurfaces on this platform
       Surface = *FirstSurface;
       while (Surface != NULL)
	     {
		   NextSurface = Surface->next;
	       if (CompStrNI(Surface->Name,Platform->Name,MAX_NAME_LENGTH) == 0)
		      DeleteSurface(FirstSurface, Surface);
		   Surface = NextSurface;
	     }

        SRadar *Radar, *NextRadar;

        // free all SRadars on this platform
        Radar = *FirstRadar;
        while (Radar != NULL)
	      {
            NextRadar = Radar->next;
		    if (CompStrNI(Platform->Name,Radar->PlatformName,MAX_NAME_LENGTH) == 0)
		      DeleteRadar(FirstRadar, Radar);
		    Radar = NextRadar;
	      }
    }
  FreePlatformDataDef(Platform);
  free(Platform);
}

//-------------------------------------------------------------------------//
// Function FreePlatformList                                         //
//-------------------------------------------------------------------------//
void FreePlatformList(SPlatform **FirstPlatform,STarget **FirstTarget,
  SSurface **FirstSurface, SRadar **FirstRadar)
{
  SPlatform *Platform,*NextPlatform;

  // free all SPlatforms of the list
  Platform = *FirstPlatform;
  while (Platform != NULL)
	 {
		NextPlatform = Platform->next;
		DeletePlatform(FirstPlatform,Platform, FirstTarget, FirstRadar, FirstSurface,TRUE);
		Platform = NextPlatform;
	 }
  *FirstPlatform = NULL;
}

//-------------------------------------------------------------------------//
// Function CopyPlatform                                                   //
//-------------------------------------------------------------------------//
void CopyPlatform(SPlatform *PlatformDest, SPlatform *PlatformSource)
{
  // first copy the easy stuff
  memcpy(PlatformDest, PlatformSource, sizeof(struct SPlatform));
  // now copy all the pointing stuff
  CopyDataDef(&(PlatformDest->RotDef),&(PlatformSource->RotDef));
  CopyDataDef(&(PlatformDest->TrajDef),&(PlatformSource->TrajDef));
  CopyDataDef(&(PlatformDest->PosDevDef),&(PlatformSource->PosDevDef));
  CopyDataDef(&(PlatformDest->RotDevDef),&(PlatformSource->RotDevDef));
}
/*
//-------------------------------------------------------------------------//
// Function CopyGeometry                                                   //
//-------------------------------------------------------------------------//
void CopyGeometry(struct SGeometry *GeometryDest, struct SGeometry *GeometrySource)
{
  // first copy the easy stuff
  memcpy(GeometryDest, GeometrySource, sizeof(struct SGeometry));
}
*/
//-------------------------------------------------------------------------//
// Function AddRadar                                                       //
//-------------------------------------------------------------------------//
bool AddRadar(SRadar *Radar, SRadar **FirstRadar)
{
  SRadar *NewRadar,*LastRadar;

  NewRadar =  (SRadar*)malloc(sizeof(SRadar));
  if (NewRadar == NULL)
	 return FALSE;
  // copy all parameters
  memcpy(NewRadar,Radar,sizeof(SRadar));

  // attach new SRadar to list
  if (*FirstRadar == NULL)
	 {
		*FirstRadar = NewRadar;
		NewRadar->next = NULL;
		NewRadar->previous = NULL;
	 }
  else
	 {
		// find last SRadar in the list
		LastRadar = *FirstRadar;
		while (LastRadar->next != NULL)
		  LastRadar = LastRadar->next;

		// insert new SRadar after last SRadar
		LastRadar->next    = NewRadar;
		NewRadar->previous = LastRadar;
		NewRadar->next     = NULL;
	 }
  return TRUE;
}

//-------------------------------------------------------------------------//
// Function DeleteRadar                                                   //
//-------------------------------------------------------------------------//
void DeleteRadar(struct SRadar **FirstRadar, struct SRadar *DestRadar)
{
  SRadar *Radar;

  // free all SRadars of the list
  Radar = *FirstRadar;
  while (Radar != DestRadar)
	 {
		Radar = Radar->next;
	 }

  if (Radar->previous != NULL)
	 (Radar->previous)->next = Radar->next;
  if (Radar->next != NULL)
	 (Radar->next)->previous = Radar->previous;

  if (Radar == *FirstRadar) *FirstRadar = Radar->next;

  FreeRadarDataDef(Radar);
  free(Radar);
}

//-------------------------------------------------------------------------//
// Function FreeRadarList                                                  //
//-------------------------------------------------------------------------//
void FreeRadarList(SRadar **FirstRadar)
{
  SRadar *Radar,*NextRadar;

  // free all SRadars of the list
  Radar = *FirstRadar;
  while (Radar != NULL)
	 {
		NextRadar = Radar->next;
		DeleteRadar(FirstRadar,Radar);
		Radar = NextRadar;
	 }
  *FirstRadar = NULL;
}

//-------------------------------------------------------------------------//
// Function CopyRadar                                                   //
//-------------------------------------------------------------------------//
void CopyRadar(SRadar *RadarDest, SRadar *RadarSource)
{
  // first copy the easy stuff
  memcpy(RadarDest, RadarSource, sizeof(struct SRadar));
  // now copy all the pointing stuff
  CopyDataDef(&(RadarDest->PulseDef),&(RadarSource->PulseDef));
  CopyDataDef(&(RadarDest->EnvelopeDef),&(RadarSource->EnvelopeDef));
  CopyDataDef(&(RadarDest->PRFDef),&(RadarSource->PRFDef));
  CopyDataDef(&(RadarDest->FreqDef),&(RadarSource->FreqDef));
  CopyDataDef(&(RadarDest->AntennaTDef),&(RadarSource->AntennaTDef));
  CopyDataDef(&(RadarDest->AntennaRDef),&(RadarSource->AntennaRDef));
  CopyDataDef(&(RadarDest->DirectionTDef),&(RadarSource->DirectionTDef));
  CopyDataDef(&(RadarDest->MFDef),&(RadarSource->MFDef));
  CopyDataDef(&(RadarDest->AGCDef),&(RadarSource->AGCDef));
}

//-------------------------------------------------------------------------//
// Function CopySimulation                                                   //
//-------------------------------------------------------------------------//
void CopySimulation(struct SSimulation *SimulationDest, struct SSimulation *SimulationSource)
{
  // first copy the easy stuff
  memcpy(SimulationDest, SimulationSource, sizeof(struct SSimulation));
}

//-------------------------------------------------------------------------//
// Function AddSimulation                                                      //
//-------------------------------------------------------------------------//
bool AddSimulation(SSimulation *Simulation, SSimulation **FirstSimulation)
{
  SSimulation *NewSimulation,*LastSimulation;

  NewSimulation =  (SSimulation*)malloc(sizeof(SSimulation));
  if (NewSimulation == NULL)
	 return FALSE;
  // copy all parameters
  memcpy(NewSimulation,Simulation,sizeof(SSimulation));

  // attach new SSimulation to list
  if (*FirstSimulation == NULL)
	 {
		*FirstSimulation = NewSimulation;
		NewSimulation->next = NULL;
		NewSimulation->previous = NULL;
	 }
  else
	 {
		// find last SSimulation in the list
		LastSimulation = *FirstSimulation;
		while (LastSimulation->next != NULL)
		  LastSimulation = LastSimulation->next;

		// insert new SSimulation after last SSimulation
		LastSimulation->next    = NewSimulation;
		NewSimulation->previous = LastSimulation;
		NewSimulation->next     = NULL;
	 }
  return TRUE;
}

//-------------------------------------------------------------------------//
// Function DeleteSimulation                                                   //
//-------------------------------------------------------------------------//
void DeleteSimulation(struct SSimulation **FirstSimulation, struct SSimulation *DestSimulation)
{
  struct SSimulation *Simulation;

  Simulation = *FirstSimulation;
  while (Simulation != DestSimulation)
	 {
		Simulation = Simulation->next;
	 }

  if (Simulation->previous != NULL)
	 (Simulation->previous)->next = Simulation->next;
  if (Simulation->next != NULL)
	 (Simulation->next)->previous = Simulation->previous;

  if (Simulation == *FirstSimulation) *FirstSimulation = Simulation->next;
  free(Simulation);
}

//-------------------------------------------------------------------------//
// Function FreeSimulationList                                                 //
//-------------------------------------------------------------------------//
void FreeSimulationList(SSimulation **FirstSimulation)
{
  SSimulation *Simulation,*NextSimulation;

  // free all SSimulations of the list
  Simulation = *FirstSimulation;
  while (Simulation != NULL)
	 {
		NextSimulation = Simulation->next;
		free(Simulation);
		Simulation = NextSimulation;
	 }
  *FirstSimulation = NULL;
}

//-------------------------------------------------------------------------//
// Function CountSimulations                                                 //
//                                                                         //
// This function returns the total number of Simulations in memory.          //
//-------------------------------------------------------------------------//
int CountSimulations(struct SSimulation *first)
{
  int count = 0;
  SSimulation *s;

  s=first;
  while (s != NULL)
	 {
		count++;
		s = s->next;
	 }
  return count;
}


//-------------------------------------------------------------------------//
// Function CountPlatforms                                                 //
//                                                                         //
// This function returns the total number of platforms in memory.          //
//-------------------------------------------------------------------------//
int CountPlatforms(struct SPlatform *first)
{
  int count = 0;
  SPlatform *p;

  p=first;
  while (p != NULL)
	 {
		count++;
		p = p->next;
	 }
  return count;
}

//-------------------------------------------------------------------------//
// Function CountRadars                                                 //
//                                                                         //
// This function returns the total number of Radars in memory.          //
//-------------------------------------------------------------------------//
int CountRadars(struct SRadar *first)
{
  int count = 0;
  SRadar *r;

  r=first;
  while (r != NULL)
	 {
		count++;
		r = r->next;
	 }
  return count;
}

//-------------------------------------------------------------------------//
// Function CountTargets                                                   //
//                                                                         //
// This function returns the total number of targets in memory.            //
//-------------------------------------------------------------------------//
int CountTargets(struct STarget *first)
{
  int count = 0;
  struct STarget *t;

  t=first;
  while (t != NULL)
	 {
		count++;
		t = t->next;
	 }
  return count;
}

//-------------------------------------------------------------------------//

long FindPlatformCount(struct SPlatform *first, struct SPlatform *PF)
{
  long Count = -1;
  SPlatform *p;

  p=first;
  while (p != NULL)
	 {
		Count++;
		if (PF == p) return Count;
		p = p->next;
	 }
  return Count;
}

//-------------------------------------------------------------------------//
// Function FindTargetNo                                                  //
//                                                                         //
// This function returns the number of the target, given a pointer.        //
// 0 = first
//-------------------------------------------------------------------------//
long FindTargetNo(struct STarget *first, struct STarget *x)
{
  long count = 0;
  struct STarget *t;

  t=first;
  while (t != NULL)
	 {
		if (t == x) return count;
		count++;
		t = t->next;
	 }
  return 0;
}

//-------------------------------------------------------------------------//
// Function FindRadar                                                   //
//                                                                         //
// The following procedure searches for a radar with given name.        //
//-------------------------------------------------------------------------//
bool FindRadar(char name[MAX_NAME_LENGTH], struct SRadar **r, int *PFN,
              struct SRadar *first)
{
  bool found=false;
  struct SRadar *R;
  R=first;
  *PFN = 0;

  while (R != NULL)
	 {
		if (CompStrNI(R->RadarName,name,MAX_NAME_LENGTH) == 0) {found = true;break;}
		(*PFN)++;
		R = R->next;                     // goto next radar
	 }
  *r = R;
  return found;
}

//-------------------------------------------------------------------------//
// Function FindPlatform                                                   //
//                                                                         //
// The following procedure searches for a platform with given name.        //
//-------------------------------------------------------------------------//
bool FindPlatform(char name[16], SPlatform **p, long *PFN, SPlatform *first)
{
  bool found=false;
  struct SPlatform *P;
  P=first;
  *PFN = 0;

  while (P != NULL)
	 {
		if (CompStrNI(P->Name,name,MAX_NAME_LENGTH) == 0) { found = true;break;}
		(*PFN)++;
		P = P->next;                     // goto next platform
	 }
  *p = P;
  return found;
}
//-------------------------------------------------------------------------//
void FreePlatformDataDef(SPlatform *PF)
{
	FreeDataDef(&(PF->RotDef));
	FreeDataDef(&(PF->TrajDef));
	FreeDataDef(&(PF->PosDevDef));
	FreeDataDef(&(PF->RotDevDef));
}
//-------------------------------------------------------------------------//
void FreeRadarDataDef(SRadar *R)
{
	FreeDataDef(&(R->PulseDef));
	FreeDataDef(&(R->EnvelopeDef));
	FreeDataDef(&(R->PRFDef));
	FreeDataDef(&(R->FreqDef));
	FreeDataDef(&(R->AntennaTDef));
	FreeDataDef(&(R->AntennaRDef));
	FreeDataDef(&(R->DirectionTDef));
	FreeDataDef(&(R->MFDef));
	FreeDataDef(&(R->AGCDef));
}
//-------------------------------------------------------------------------//
// Function PointToTarget                                                  //
//                                                                         //
// This function returns a pointer to the nth target.                      //
//-------------------------------------------------------------------------//
struct STarget *PointToTarget(struct STarget *first, long n)
{
  long count = 0;
  STarget *t;

  t=first;
  while (t != NULL)
	 {
		count++;
		if (count == n) return t;
		t = t->next;
	 }
  return NULL;
}

//-------------------------------------------------------------------------//
// Function PointToPlatform                                                //
//                                                                         //
// This function returns a pointer to the nth platform.                    //
//-------------------------------------------------------------------------//
struct SPlatform *PointToPlatform(struct SPlatform *first, long n)
{
  long count = 0;
  SPlatform *p;

  p=first;
  while (p != NULL)
	 {
		count++;
		if (count == n) return p;
		p = p->next;
	 }
  return NULL;
}

//-------------------------------------------------------------------------//
// Function CopyGeometry                                                   //
//-------------------------------------------------------------------------//
void CopyGeometry(struct SGeometry *GeometryDest, struct SGeometry *GeometrySource)
{
  // first copy the easy stuff
  memcpy(GeometryDest, GeometrySource, sizeof(struct SGeometry));
}

//-------------------------------------------------------------------------//
// Function AddGeometry                                                      //
//-------------------------------------------------------------------------//
bool AddGeometry(SGeometry *Geometry, SGeometry **FirstGeometry)
{
  SGeometry *NewGeometry,*LastGeometry;

  NewGeometry =  (SGeometry*)malloc(sizeof(SGeometry));
  if (NewGeometry == NULL)
	 return FALSE;
  // copy all parameters
  memcpy(NewGeometry,Geometry,sizeof(SGeometry));

  // attach new SGeometry to list
  if (*FirstGeometry == NULL)
	 {
		*FirstGeometry = NewGeometry;
		NewGeometry->next = NULL;
		NewGeometry->previous = NULL;
	 }
  else
	 {
		// find last SGeometry in the list
		LastGeometry = *FirstGeometry;
		while (LastGeometry->next != NULL)
		  LastGeometry = LastGeometry->next;

		// insert new SGeometry after last SGeometry
		LastGeometry->next    = NewGeometry;
		NewGeometry->previous = LastGeometry;
		NewGeometry->next     = NULL;
	 }
  return TRUE;
}

//-------------------------------------------------------------------------//
// Function DeleteGeometry                                                   //
//-------------------------------------------------------------------------//
void DeleteGeometry(struct SGeometry **FirstGeometry, struct SGeometry *DestGeometry)
{
  struct SGeometry *Geometry;

  Geometry = *FirstGeometry;
  while (Geometry != DestGeometry)
	 {
		Geometry = Geometry->next;
	 }

  if (Geometry->previous != NULL)
	 (Geometry->previous)->next = Geometry->next;
  if (Geometry->next != NULL)
	 (Geometry->next)->previous = Geometry->previous;

  if (Geometry == *FirstGeometry) *FirstGeometry = Geometry->next;
  free(Geometry);
}

//-------------------------------------------------------------------------//
// Function FreeGeometryList                                                 //
//-------------------------------------------------------------------------//
void FreeGeometryList(SGeometry **FirstGeometry)
{
  SGeometry *Geometry,*NextGeometry;

  // free all SGeometrys of the list
  Geometry = *FirstGeometry;
  while (Geometry != NULL)
	 {
		NextGeometry = Geometry->next;
		free(Geometry);
		Geometry = NextGeometry;
	 }
  *FirstGeometry = NULL;
}

//-------------------------------------------------------------------------//
// Function CountGeometrys                                                 //
//                                                                         //
// This function returns the total number of Geometrys in memory.          //
//-------------------------------------------------------------------------//
int CountGeometrys(struct SGeometry *first)
{
  int count = 0;
  SGeometry *s;

  s=first;
  while (s != NULL)
	 {
		count++;
		s = s->next;
	 }
  return count;
}

