#include "dest.h"
//---------------------------------------------------------------------------
#ifndef generalH
#define generalH
//---------------------------------------------------------------------------
#define A_MULT 16807.0       // multiplier
#define M_MOD 2147483647.0  // modulus
#include <stdlib.h>

#include "vector.h"

#ifdef COMPILE_FOR_WINDOWS32
  void SetRect(TRect *Rect, int left, int top, int right, int bottom);
  bool PointInRect(TRect Rect, int x, int y);
#endif

int CompStrNI(char *str1, char *str2,int n);
double **DMatrix(long nrl, long nrh, long ncl, long nch);
void Free_DMatrix(double **m, long nrl, long ncl);
double *DVector(long nl, long nh);
void Free_DVector(double *v, long nl);
double ***DMatrix3(long nrl,long nrh, long ncl, long nch,long ndl,long ndh);
void Free_DMatrix3(double ***t,long nrl,long ncl,long ndl);
double Interpolate(long n, double *x, double *y,double **Coeff, double X, int method);
double Sa(double x);
double dBToFac(double dB);
double FacTodB(double Fac);
void Swap(double *A, double *B);
double round(double x);
void FindPolyCoeff(double *t, double *y, int n, double **Coeff);
void gauss(double **a,double *c,double *x, int n);
double SciNotation(double N,int *expo);
double rnd();
double Gaussian(double a, double d, bool Reset);
double AziAngle(double v[3]);
double ElevAngle(double v[3]);
void FindRotMatrix(double a[3], double RotMatrix[3][3]);
void RotatePoints3(double P[][3], double NewP[][3], unsigned int CountP,
				   int OffsetX, int OffsetY, float ZoomX, float ZoomY,
				   double RotAng[3], double Pos[3], double Focus[3]);
void Copy3DPoint(double DP[3], double SP[3]);
void ConvertAnglesToVec(double Vec[3], double L, double DirAzi, double DirEle);
void RotateNPoints(double P[][3], double NewP[][3], long CountP,
						double Rot[3]);
double Distance(double x1, double y1, double x2, double y2);
void CFFT(double data[],int nn,int isign);
double DotProduct(double Array1[], double Array2[], int n);
double GetValue(int what, double I, double Q);
void CrossP(double a[3], double b[3], double Result[3]);
double FindMag(double x[3]);
double Dist3D(double x1[3], double x2[3]);
void ConvertToUpperCase( char *DataString );
double Min(double a, double b);
double Max(double a, double b);

#ifdef WITH_SURFACE_ALGORITHM
// add surf
double Angle2D(double x0,double x1);
double IntersectRadius(double a, double da,Vec Tri[3]);
double AziAngle(double v[3]);
double ElevAngle(double v[3]);
bool PutTriOnXY(Vec Tri[3],Vec FlatTri[3], Vec *Normal, double *d);
void Intersect2D(double a,Vec w, Vec y, Vec *I1, Vec *I2, int *Sol);
bool PtInTriangle2D(Vec Tri[3], Vec P);
Vec CenterPoint(Vec v1, Vec v2, double r);
void Swap(int *a, int *b);
#endif

#endif
