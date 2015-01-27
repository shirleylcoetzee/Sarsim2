//---------------------------------------------------------------------------
#ifndef linkedlistH
#define linkedlistH
#include "objectstructdef.h"
void CopyTarget(struct STarget *TargetSource, struct STarget *TargetDest);
bool AddTarget(struct STarget *Target, struct STarget **FirstTarget);
void FreeTarget(struct STarget *Target);
void DeleteTarget(struct STarget **FirstTarget, struct STarget *DestTarget);
void FreeTargetList(struct STarget **FirstTarget);
void FreeTargetDataDef(struct STarget *T);
int CountTargets(struct STarget *first);
struct STarget *PointToTarget(struct STarget *first, long n);
long FindTargetNo(struct STarget *first, struct STarget *x);

void CopyPlatform(struct SPlatform *PlatformDest,struct SPlatform *PlatformSource);
bool AddPlatform(struct SPlatform *Platform, struct SPlatform **FirstPlatform);
void DeletePlatform(SPlatform **FirstPlatform, SPlatform *DestPlatform,
                    struct STarget **FirstTarget, struct SRadar **FirstRadar,
                    SSurface **FirstSurface,bool DeleteObjectsOn);
bool FindPlatform(char name[16], struct SPlatform **p, long *PFN, struct SPlatform *first);
void FreePlatformList(struct SPlatform **FirstPlatform,struct STarget **FirstTarget,
     struct SSurface **FirstSurface,struct SRadar **FirstRadar);
void FreePlatformDataDef(struct SPlatform *PF);
int CountPlatforms(struct SPlatform *first);
struct SPlatform *PointToPlatform(struct SPlatform *first, long n);
long FindPlatformCount(struct SPlatform *first, struct SPlatform *PF);

void CopyRadar(struct SRadar *RadarDest, struct SRadar *RadarSource);
bool AddRadar(struct SRadar *Radar, struct SRadar **FirstRadar);
void DeleteRadar(struct SRadar **FirstRadar, struct SRadar *DestRadar);
void FreeRadarList(struct SRadar **FirstRadar);
void FreeRadarDataDef(struct SRadar *R);
bool FindRadar(char name[MAX_NAME_LENGTH], struct SRadar **r, int *PFN, struct SRadar *first);
int CountRadars(struct SRadar *first);

bool AddSimulation(struct SSimulation *Simulation, struct SSimulation **FirstSimulation);
void FreeSimulationList(struct SSimulation **FirstSimulation);
void CopySimulation(struct SSimulation *SimulationDest, struct SSimulation *SimulationSource);
void DeleteSimulation(struct SSimulation **FirstSimulation, struct SSimulation *DestSimulation);
int CountSimulations(struct SSimulation *first);

void AllocDataDefMem(struct SDataDefinition *DataDef);
void CopyDataDef(struct SDataDefinition *DestDataDef,struct SDataDefinition *SourceDataDef);
void FreeDataDef(struct SDataDefinition *DataDef);
long FindSurfaceNo(struct SSurface *first, struct SSurface *x);
int CountSurfaces(struct SSurface *first);
struct SSurface *PointToSurface(struct SSurface *first, long n);
void FreeSurfaceList(struct SSurface **FirstSurface);
void DeleteSurface(struct SSurface **FirstSurface, struct SSurface *DestSurface);
bool AddSurface(struct SSurface *Surface, struct SSurface **FirstSurface);
void FreeSurfaceDataDef(struct SSurface *T);
void CopySurface(struct SSurface *SurfaceDest, struct SSurface *SurfaceSource);

//void CopyGeometry(struct SGeometry *GeometryDest, struct SGeometry *GeometrySource);

void CopyGeometry(struct SGeometry *GeometryDest, struct SGeometry *GeometrySource);
bool AddGeometry(SGeometry *Geometry, SGeometry **FirstGeometry);
void DeleteGeometry(struct SGeometry **FirstGeometry, struct SGeometry *DestGeometry);
void FreeGeometryList(SGeometry **FirstGeometry);
int CountGeometrys(struct SGeometry *first);

//---------------------------------------------------------------------------
#endif
