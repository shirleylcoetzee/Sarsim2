#include "dest.h"
#ifndef engineH
#define engineH
//---------------------------------------------------------------------------
#include "objectstructdef.h"
#include "general.h"
#include "loadsavedata.h"
#include <stdio.h>
#include <vector>
//---------------------------------------------------------------------------
#ifdef COMPILE_FOR_WINDOWS32
#include "selectpal.h"
void WriteDataDefMemo(struct SDataDefinition *DataDef,TMemo *ThisMemo);
void FillMemo(TMemo *ThisMemo);
/*void DrawArray(TCanvas *Canvas, class TBitM *BM, double **Data,
               double *PulseSendTime, unsigned long PulseNo, unsigned long SamplePoints, int
               PixelY, struct SSimulation *CurrentSim, int SBContrast, int SBBright,
               int SBSat,unsigned char ColPal[NCOLOR][3], int Scale);
 */
void DrawArray(int DrawAs, Graphics::TBitmap *LGraphBM, class TBitM *BM,
               double **Data, double *PulseSendTime, unsigned long PulseNo,
               unsigned long SamplePoints, int PixelY,
               struct SSimulation *CurrentSim, int SBContrast, int SBBright,
               int SBSat,unsigned char ColPal[NCOLOR][3], bool ShowdB);

#endif
//---------------------------------------------------------------------------
double FindPulseFreq(long PNo, struct SRadar *R);
void FindPulsesInRange(double TimeStart, double TimeEnd, struct SRadar *R,
							  long *FirstPulse, long *LastPulse);
double FindPulseSendTime(long PulseNo, struct SRadar *R);
void FindPlatformPosition(struct SPlatform *p, double time, double Pos[3]);
void FindTargetPosition(struct STarget *t, double Pos[3]);
void FindTargetDir(struct STarget *t, double *AziAng, double *EleAng);
void FindPlatformVelocity(struct SPlatform *p, double time, double Vel[3]);
void FindPlatformRotation(struct SPlatform *p, double time, double Rot[3]);
void FindAntennaDir(struct SRadar *Radar, double Time, double Pos[3], double Rot[3], double *ADirAzi,
						  double *ADirEle);
void CalcOnePulse(double *OnePulse, long SamplePoints, double SlantStartTime,
						double SecPerSample,long TargetNo, double *PulseFreq,
						double **RangeDelay, double **TargetRadialVel,
						double **ReturnAmp,long PNo, SRadar *r, double PulseCenter,
						double DelaySlope, double *MaxMagOfPulse, double ***SurfP,
            long SurfaceNo, double *Template, double OverSampleFactor,
            long Pow2SamplePoints, long UsedSamples, struct SSurface *FirstSurface,
            long ZeroedSamples,double **RadarDir);
void WriteData(FILE *OutFilep, struct SDataDefinition *DataDef);
//bool LoadDataFile(HWND hwnd, char FileName[],struct SDataDefinition *DataDef);
Complex MatchFilter(double time, struct SRadar *r);
double SinAntennaGain(double Offset, double BeamWidth);
double Pulse(struct SRadar *r, double t);
void FindPixelAziSlant(struct SRadar *FirstRadar,long *PointsAzi, long *PointsSlant,
							  struct SSimulation *sim);
//void FindAntennaDir(struct SRadar *Radar, double Time, double Pos[3], double Rot[3], double *ADirAzi,
//						  double *ADirEle);
//void FindPixelAziSlant(long *PointsAzi, long *PointsSlant,
//							  struct SSimulation *sim);
bool LoadDataFile(char FileName[],SDataDefinition *DataDef);
Complex MatchFilter(double time, struct SRadar *r, double BandWidth);
double SinAntennaGain(double Offset, double BeamWidth);
double Pulse(struct SRadar *r, double t);
double FindNyquistRate(struct SRadar *r);
long FindPulseForTime(double Time, struct SRadar *R);
void CalcArray2(struct SRadar *r, struct SSimulation *CurrentSim,
    double **Data, long PulseNo, long DataXSize, double *PulseFreq,
    double **RangeDelay,  double **TargetRadialVel, double **ReturnAmp,
    double *MaxMagnitude, struct STarget *FirstTarget,
    struct SPlatform *FirstPlatform,struct SSurface *FirstSurface, double ***SurfP
    ,double **RadarDir);
void CalcGeometry(double **RangeDelay,double **ReturnAmp,double **TargetRadialVel,
struct SRadar *Radar,double *PulseSendTime, long PulseNo, struct STarget *FirstTarget,
struct SPlatform *FirstPlatform, struct SSurface *FirstSurface, double ***SurfP,
double **RadarDir, long FirstPulse);


void FindSurfaceNormal(SSurface *s, double *AziAng, double *EleAng);
bool SendPulse(double time, double PulseWidth, double DelaySlope, double *Phase);
//int SaveSimuData(double *MaxMagnitude, struct SSimulation *ThisSim,
//  struct LinkedLists *FP, long PointsSlant);
int SaveSimuData(double *MaxMagnitude, struct SSimulation *ThisSim,
  struct LinkedLists *FP, long PointsSlant, double *TotalSampleNo,
  double *ClippedSamples,double *ZeroSamples);
double FindValuedB(double Value, double Max);
int SaveGeometryData(struct SGeometry *ThisGeo,
  struct LinkedLists *FP);
//void FindRadar(char *RadarName, struct SRadar **Radar,
//  struct SRadar *FirstRadar);
double FindBandwidth(struct SRadar *r);
void EditDataDef(struct SDataDefinition *DataDef);
void ConvertSurfaceToPTs(int UseWhatArray,  struct SSurface *s, long SNo, long *PTsUsed,
  long LimitPTs, double Density, double **CoordPT, long PNo, double ***SurfP,double *NormVecUnit
  );
double FindAntennaGainRT(double OffsetAzi, double OffsetElev,
  struct SRadar *r);


#endif
