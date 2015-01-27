#include "dest.h"
//---------------------------------------------------------------------------

#ifdef COMPILE_FOR_WINDOWS32
  #include <vcl\vcl.h>
  #pragma hdrstop
  #include "main.h"
  #include "linkedlist.h"
#endif

//---------------------------------------------------------------------------
#include "general.h"
#include "objectstructdef.h"
#include <stdlib.h>
#include <math.h>
#include <values.h>
#include <ctype.h>
#include "vector.h"

#ifdef COMPILE_FOR_WINDOWS32
void SetRect(TRect *Rect, int left, int top, int right, int bottom)
  {
    Rect->Left = left;
    Rect->Right = right;
    Rect->Top = top;
    Rect->Bottom = bottom;
  }
//-------------------------------------------------------------------------//
bool PointInRect(TRect Rect, int x, int y)
  {
    if ( (x > Rect.Left) && (x < Rect.Right) &&
         (y > Rect.Top)  && (y < Rect.Bottom) )
      return TRUE;
    else
      return FALSE;
  }
#endif
//-------------------------------------------------------------------------//
int CompStrNI(char *str1, char *str2,int n)
{
  int c,U1,U2;    

  c = 0;
  while (c < n)
    {
      if (str1[c] == '\0' || str2[c] == '\0') return 0;
      U1 = (int)toupper((int)str1[c]);
      U2 = (int)toupper((int)str2[c]);
      if (U1 != U2) return 1;  // they are not equal
      c++;
    }
  return 0;
}

//-------------------------------------------------------------------------//
// Function DMatrix                                                        //
//                                                                         //
// This function creates allocates memory for a DOUBLE array of size       //
// (nrh-nrl)+1 x (nch-ncl)+1. It returns a pointer to this array.          //
//-------------------------------------------------------------------------//
double **DMatrix(long nrl, long nrh, long ncl, long nch)
{
  // make sure nrh > nrl etc.
  if (nrh<nrl)	nrh = nrl;
  if (nch<ncl)	nch = ncl;

  long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
  double **m;

  m=(double **) malloc((size_t)((nrow+1)*sizeof(double*)));
  if (!m) return NULL;
  m += 1;
  m -= nrl;

  m[nrl]=(double *) malloc((size_t)((nrow*ncol+1)*sizeof(double)));
  if (!m[nrl]) return NULL;
  m[nrl] += 1;
  m[nrl] -= ncl;

  for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

  return m;
}

//-------------------------------------------------------------------------//
// Function Free_DMatrix                                                   //
//                                                                         //
// This function frees the memory  allocated by DMatrix function.          //
//-------------------------------------------------------------------------//

void Free_DMatrix(double **m, long nrl, long ncl)
{
  free((char *) (m[nrl]+ncl-1));
  free((char *) (m+nrl-1));
}

//-------------------------------------------------------------------------//
// Function DVector                                                        //
//                                                                         //
// This function creates allocates memory for a DOUBLE vector of size      //
// (nh-nl)+1. It returns a pointer to this array.                          //
//-------------------------------------------------------------------------//
double *DVector(long nl, long nh)
// allocate a double vector with subscript range v[nl..nh]
{
  double *v;

  if (nh < nl) nh=nl;

  v=(double *)malloc((size_t) ((nh-nl+2)*sizeof(double)));
  if (!v) return NULL;
  return v-nl+1;
}

//-------------------------------------------------------------------------//
// Function Free_DVector                                                   //
//                                                                         //
// This function frees the memory  allocated by DVector function.          //
//-------------------------------------------------------------------------//
void Free_DVector(double *v, long nl)
// free a double vector allocated with dvector()
{
  free((char *) (v+nl-1));
}

//-------------------------------------------------------------------------//
// Function DMatrix3                                                       //
//                                                                         //
// This function creates allocates memory for a DOUBLE array of size       //
// (nrh-nrl)+1 x (nch-ncl)+1 x (ndh-ndl)+1.                                //
// It returns a pointer to this array.                                     //
//-------------------------------------------------------------------------//

double ***DMatrix3(long nrl,long nrh, long ncl, long nch,long ndl,long ndh)
// allocate a double 3D-matrix with range t[nrl..nrh][ncl..nch][ndl..ndh]
{
  if (nrh<nrl)	nrh = nrl;
  if (nch<ncl)	nch = ncl;
  if (ndh<ndl)	ndh = ndl;

  long i,j,nrow=nrh-nrl+1,ncol=nch-ncl+1,ndep=ndh-ndl+1;
  double ***t;

	// allocate pointers to pointers to rows
	t=(double ***) malloc((unsigned int)((nrow+1)*sizeof(double**)));
	if (!t) return NULL;
	t += 1;
	t -= nrl;

	// allocate pointers to rows and set pointers to them
	t[nrl]=(double **) malloc((unsigned int)((nrow*ncol+1)*sizeof(double*)));
	if (!t[nrl]) return NULL;
	t[nrl] += 1;
	t[nrl] -= ncl;

	// allocate rows and set pointers to them
	t[nrl][ncl]=(double *) malloc((unsigned int)((nrow*ncol*ndep+1)*sizeof(double)));
	if (!t[nrl][ncl]) return NULL;
	t[nrl][ncl] += 1;
	t[nrl][ncl] -= ndl;

	for(j=ncl+1;j<=nch;j++) t[nrl][j]=t[nrl][j-1]+ndep;
	for(i=nrl+1;i<=nrh;i++) {
		t[i]=t[i-1]+ncol;
		t[i][ncl]=t[i-1][ncl]+ncol*ndep;
		for(j=ncl+1;j<=nch;j++) t[i][j]=t[i][j-1]+ndep;
	}

	// return pointer to array of pointers to rows
	return t;
}

//-------------------------------------------------------------------------//
// Function Free_DMatrix                                                   //
//                                                                         //
// This function frees the memory  allocated by DMatrix function.          //
//-------------------------------------------------------------------------//
void Free_DMatrix3(double ***t,long nrl,long ncl,long ndl)
// free a double 3D-matrix allocated by DMatrix3()
{
	free((char *) (t[nrl][ncl]+ndl-1));
	free((char *) (t[nrl]+ncl-1));
	free((char *) (t+nrl-1));
}

//-------------------------------------------------------------------------//
// Function Interpolate                                                    //
//                                                                         //
// The following procedure calculates the Y coordinate of a given X        //
// coordinate and also given the spline coefficient matrix ...             //
//-------------------------------------------------------------------------//
double Interpolate(long n, double *x, double *y, double **Coeff,
                   double X,int method)
{
  // no points defined - not allowed
  if (n == 0) return 0;
  if (n == 1) return y[0];

  if (method == CUBIC)
    {
      // cubic splines
      long i;
      // find between which given points the requested value is lying
      i=0;
      while ((X > x[i]) && (i<n)) {i++;}
      if (i>=1) i--;
      if (i >= (n-1)) i = n-2;
      if (n==1) i=0;
      // and calculate the required value
      return (Coeff[i][0] + Coeff[i][1]*(X-x[i]) +
		   Coeff[i][2]*(X-x[i])*(X-x[i]) +
		   Coeff[i][3]*(X-x[i])*(X-x[i])*(X-x[i]));
    }
  else if (method == FILTER)
    {
      // bandlimited filter
/*      // find between which given points the requested value is lying
      long i = n-1;
      while ((X < x[i]) && (i>0)) {i--;}
      if (X < x[0]) i = -1;
*/
      double CX,Ave,sum,NearestInt,Step;
      long Count,i;
      double IntegerSteps,LowerLimit,UpperLimit;

      // assume the step size is the range divide by the number of points-1
      Step = (x[n-1] - x[0])/(n-1);
      IntegerSteps = X/Step;
      NearestInt = round(IntegerSteps);
      sum = 0;

      for (CX=NearestInt-10;CX<=NearestInt+10;CX++)
        {
          Ave = 0;
          Count = 0;
          // calculate the range of the 'bin'
          LowerLimit = (CX-0.5)*Step;
          UpperLimit = (CX+0.5)*Step;
          for (i=0;i<n;i++)
            {
              if ((x[i] > LowerLimit) && (x[i] <= UpperLimit))
                {Count++;Ave += y[i];}
              else
                if (x[i] > UpperLimit) break;
            }
          if (Count > 1) Ave = Ave / Count;
          // Ave contains now the avarage value of the samples in that bin

          // draw a sinx/x shape at the bin center (CX)
          sum += Ave*Sa(PI* (IntegerSteps-CX) );
        }
      return sum;
    }
  else  // linear interpolation
    {
      long i;
      if (X < x[0])
        {
          if (fabs(x[1]-x[0]) > 0)
            return (y[0]+((y[1]-y[0])*(X-x[0])/(x[1]-x[0])));  // left of first point defined
          else return y[0];
        }
      if (X > x[n-1])
        {
          if (fabs(x[n-1]-x[n-2]) > 0)
            return (y[n-2]+((y[n-1]-y[n-2])*(X-x[n-2])/(x[n-1]-x[n-2])));  //  right of last point defined
          else return y[0];
        }

      // find between which given points the requested value is lying
      i=0;
      while ((X > x[i]) && (i<n)) {i++;}
      if (i>=1) i--;
      if (i >= (n-1)) i = n-2;
      double dx = x[i+1]-x[i];
      double dy = y[i+1]-y[i];
      if (dx == 0) return y[i];
      return (y[i]+(dy*(X-x[i])/dx));
    }
}
//-------------------------------------------------------------------------//
double Sa(double x)
{
  if (x == 0) return 1;
  else return (sin(x)/x);
}
//-------------------------------------------------------------------------//
double dBToFac(double dB)
{
  return pow(10,dB/(double)10);
}
//-------------------------------------------------------------------------//
double FacTodB(double Fac)
{
  if (Fac <= 0) return -100;
  else return 10*log10(Fac);
}
//-------------------------------------------------------------------------//
// Function round()                                                        //
//                                                                         //
// Round to the nearest integer.                                           //
//-------------------------------------------------------------------------//
double round(double x)
{
  if (x-floor(x)<0.5) return floor(x); else return ceil(x);
}
//-------------------------------------------------------------------------//
// Function FindPolyCoeff                                                  //
//                                                                         //
// The following procedure fits through each adjacent pair of given n      //
// points t[x], y[x] a polynomial of degree 3 (cubic) such that the        //
// function will be smooth (differentiable).                               //
// It returns n-1 x 4 coefficients, representing the cubic spline          //
// function between each adjacent pair of points.                          //
// At least 3 points are required (n>=3).                                  //
//-------------------------------------------------------------------------//
void FindPolyCoeff(double *t, double *y, int n, double **Coeff)
{
  int i,k;                             // loop variables
  double h;                            // x distance between 2 points
  double *temp,*x,*slope;
  double **A;

  if (n == 0) return;

  if (n == 1)
	 {
		Coeff[0][0] = y[0];
		Coeff[0][1] = 0;
		Coeff[0][2] = 0;
		Coeff[0][3] = 0;
	 }
  else if (n == 2)
	 {
		Coeff[0][0] = y[0];
		if (t[1] != t[0])
		  Coeff[0][1] = (y[1]-y[0])/(t[1]-t[0]);
		else
		  Coeff[0][1] = MAXFLOAT;
		Coeff[0][2] = 0;
		Coeff[0][3] = 0;
	 }
  else
  {

  // create arrays...
  temp = DVector(0,n+1);
  A = DMatrix(0,n+1,0,n+1);
  x = DVector(0,n+1);
  slope = DVector(0,n+1);

  // estimate slope at first and last point as these are arbitary
  if ((t[1]-t[0]) == 0) slope[0] = MAXFLOAT;
  else slope[0] = (y[1]-y[0])/(t[1]-t[0]);
  if ((t[n-1]-t[n-2]) == 0) slope[n-1] = MAXFLOAT;
  else slope[n-1] = (y[n-1]-y[n-2])/(t[n-1]-t[n-2]);

  // create triangular matrix (000141000) etc.
  for (i=0;i<n-2;i++)
	 for (k=0;k<n-2;k++)
		{
		  if (i==k) A[i][k] = 4;
		  else if (i==(k-1)) A[i][k] = 1;
		  else if (i==(k+1)) A[i][k] = 1;
		  else	 A[i][k] = 0;
		}

  // find vector for matrix inverse
  for (k=0;k<n-2;k++)
	 {
		 if (k == 0) temp[k] = 3*(y[k+2] - y[k]) - slope[0];
		 else if (k == (n-3)) temp[k] = 3*(y[k+2] - y[k]) - slope[n-1];
		 else	temp[k] = 3*(y[k+2] - y[k]);
	 }

  // find slopes (x) at each point = solution of : Ax = temp
  gauss(A,temp,x,n-2);

  // insert values into slope arrays (as first and last points are given)
  for (k=0;k<n-2;k++)
	 slope[k+1] = x[k];

  // calculate coefficients
  for (i=0;i<n-1;i++)
	 {
		h = t[i+1] - t[i];
		if (h == 0) h = 1E-6;    // temp -> should be done properly
		Coeff[i][0] = y[i];
		Coeff[i][1] = slope[i];
		Coeff[i][2] = (3/(h*h))*(y[i+1]-y[i])-(1/h)*(slope[i+1]+2*slope[i]);
		Coeff[i][3] = (2/(h*h*h))*(y[i]-y[i+1])+(1/(h*h))*(slope[i]+slope[i+1]);
	 }

  // free arrays
  Free_DMatrix(A,0,0);
  Free_DVector(x,0);
  Free_DVector(temp,0);
  Free_DVector(slope,0);
  }
}
//-------------------------------------------------------------------------//
// Function gauss                                                          //
//                                                                         //
// Finds the solution x, given A.x=c by using Gauss reduction.             //
//                                                                         //
//  PARAMETERS:                                                            //
//    a[] matrix of size (order+1)x(order+1)                               //
//    c[],x[] vectors                                                      //
//-------------------------------------------------------------------------//
void gauss(double **a,double *c,double *x, int n)
{
  int i,j,k;
  double factor,sum;

  // do substitution and backsubstitution
  for (k=0;k<(n-1);k++)
	 {
		for (i=k+1;i<n;i++)
		  {
			 if (a[k][k] != 0)
				factor = a[i][k]/a[k][k];
			 else
				factor = MAXFLOAT;

			 for (j=k+1;j<n;j++)
				a[i][j] -= factor*a[k][j];
			 c[i] -= factor*c[k];
		  }
	 }

  // calculate result
  if (a[n-1][n-1] != 0)
	 x[n-1] = c[n-1]/a[n-1][n-1];
  else
	 x[n-1] = MAXFLOAT;
  for (i=n-2;i>=0;i--)
	 {
		sum = 0;
		for (j=i+1;j<n;j++)
		  sum += a[i][j]*x[j];
		if (a[i][i] != 0)
		  x[i] = (c[i]-sum)/a[i][i];
		else
		  x[i] = MAXFLOAT;
	 }
}

//-------------------------------------------------------------------------//
// Class Member SciNotation                                                //
// ------------------------                                                //
// Inputs  : double N       - actual number                                //
// Outputs : double mant10  - mantissa                                     //
//           int    &expo   - exponent                                     //
// Globals : -none-                                                        //
//                                                                         //
// Returns the mantissa and exponent of a number (mantissa between +/-1.00 //
// and +/-9.99999999)                                                      //
// e.g. 434.999 = 4.34999 * 10^2 => mantissa = 4.34999, exponent=2         //
// Function also works with negative numbers and negative exponents,       //
// e.g. -0.000332 = -3.32 * 10^-4 => mantissa = -3.32, exponent=-4         //
//-------------------------------------------------------------------------//
double SciNotation(double N,int *expo)
{
  int expo2;
  double mant2,expo10,mant10,frac_expo10;

  // calculate the power 2 mantissa (mant2) and exponent (expo2)
  mant2 = frexp(N, &expo2);
  // find the basis 10 exponent by multiplying with log(2)
  // i.e. a*2^b = a*10^(b*(log2))
  // as exponent (b*log2) might not be an integer, multiply
  // a by 10^(fraction of (b*log2)
  frac_expo10 = modf((double)expo2*0.301029995663981198,&expo10);
  mant10 = pow((double)10,frac_expo10)*mant2;
  // adjust values so output is in given range
  if (fabs(mant10) < 1) {mant10 *= 10; expo10 -= 1;}
  if (fabs(mant10) < 1) {mant10 *= 10; expo10 -= 1;}

  *expo = (int)expo10;
  return mant10;
}
//-------------------------------------------------------------------------//
// uniform random distribution 0..1
extern double r_seed;
double rnd()
{
  r_seed = fmod(A_MULT * r_seed, M_MOD);
  return (r_seed * 4.656612875e-10);
}
//-------------------------------------------------------------------------//
// Generate a gaussian variable with mean a and standard deviation d
double Gaussian(double a, double d, bool Reset)
{
  static double t = 0.0;
  double x,v1,v2,r;
  if ((t == 0) || Reset)
    {
      do {
        v1 = 2.0 * rnd() - 1.0;
        v2 = 2.0 * rnd() - 1.0;
        r = v1 * v1 + v2 * v2;
      } while (r >= 1.0);
      r = sqrt((-2.0 * log(r))/r);
      t = v2*r;
      return (a + v1*r*d);
    }
  else
    {
      x = t;
      t = 0.0;
      return (a + x*d);
    }
}
//-------------------------------------------------------------------------//
// Function AziAngle                                                       //
//                                                                         //
// The following function finds the angle between the positive x-axis      //
// and the given point.                                                    //
//-------------------------------------------------------------------------//
double AziAngle(double v[3])
{
  if ( (v[0] == 0) && (v[1] == 0) )
	 return 0;
  if ( (v[0] > MAXFLOAT) || (v[1] > MAXFLOAT) ) return 0;
  return atan2(v[0],v[1]);
}
//-------------------------------------------------------------------------//
// Function ElevAngle                                                      //
//                                                                         //
// The following function finds the angle between the X-Y plane and a      //
// and a vector to the given point.                                        //
//-------------------------------------------------------------------------//
double ElevAngle(double v[3])
{
  const double ZERO_THRESHOLD = 1E-100;

  if (sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) < ZERO_THRESHOLD)
	 return 0;

  return asin(v[2]/(sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2])));
}
//-------------------------------------------------------------------------//
// Function UpperCase                                                      //
//                                                                         //
// Conver string into uppercase.                                           //
//-------------------------------------------------------------------------//
void ConvertToUpperCase( char *DataString )
{
  int i=0;

  while (DataString[i] != '\0' )
	 {
		DataString[i] = (char)toupper(DataString[i]);
		i++;
	 }
}
//--------------------------------------------------------------------//
// Procedure Rot2                                                      //
// find rotation coeffs
//--------------------------------------------------------------------//
void FindRotMatrix(double a[3], double RotMatrix[3][3])
{
  // calculate rotation matrix
  RotMatrix[0][0] =  cos(a[1]) * cos(a[2]);
  RotMatrix[0][1] = -cos(a[1]) * sin(a[2]);
  RotMatrix[0][2] =  sin(a[1]);

  RotMatrix[1][0] =  cos(a[0]) * sin(a[2]) + sin(a[0]) * sin(a[1]) * cos(a[2]);
  RotMatrix[1][1] =  cos(a[0]) * cos(a[2]) - sin(a[0]) * sin(a[1]) * sin(a[2]);
  RotMatrix[1][2] = -sin(a[0]) * cos(a[1]);

  RotMatrix[2][0] =  sin(a[0]) * sin(a[2]) - cos(a[0]) * sin(a[1]) * cos(a[2]);
  RotMatrix[2][1] =  sin(a[0]) * cos(a[2]) + cos(a[0]) * sin(a[1]) * sin(a[2]);
  RotMatrix[2][2] =  cos(a[0]) * cos(a[1]);
}

//--------------------------------------------------------------------//
// Function RotatePoints3                                             //
//--------------------------------------------------------------------//
void RotatePoints3(double P[][3], double NewP[][3], unsigned int CountP,
				   int OffsetX, int OffsetY, float ZoomX, float ZoomY,
				   double RotAng[3], double Pos[3], double Focus[3])
{
  unsigned int i,k;
  double RotM[3][3];
  FindRotMatrix(RotAng,RotM);
  for (i=0;i<CountP;i++)
	 for (k=0;k<3;k++)
		NewP[i][k] = RotM[0][k] * (P[i][0]-Focus[0]) + RotM[1][k] * (P[i][1]-Focus[1]) +
						 RotM[2][k] * (P[i][2]-Focus[2]) ;
  for (i=0;i<CountP;i++)
	 {
		NewP[i][2] = OffsetX + ZoomX * (NewP[i][2]+Pos[0]);
		NewP[i][1] = OffsetY + ZoomY * (NewP[i][1]+Pos[1]);
		NewP[i][0] = NewP[i][0]+Pos[2]; // above line not used
	 }

}
//--------------------------------------------------------------------//
void Copy3DPoint(double DP[3], double SP[3])
{
  DP[0] = SP[0];
  DP[1] = SP[1];
  DP[2] = SP[2];
}
//--------------------------------------------------------------------//
void ConvertAnglesToVec(double Vec[3], double L, double DirAzi, double DirEle)
{
  double t1;
  t1 = cos(DirEle)*L;
  Vec[0] = sin(DirAzi)*t1;
  Vec[1] = cos(DirAzi)*t1;
  Vec[2] = sin(DirEle)*L;
}
//--------------------------------------------------------------------//
// Function RotateNPoints                                              //
//--------------------------------------------------------------------//
void RotateNPoints(double P[][3], double NewP[][3], long CountP,
						double Rot[3])
{
  long i,k;
  double RotM[3][3];

  // find rotation matrix
  FindRotMatrix(Rot,RotM);
  for (i=0;i<CountP;i++)
	 for (k=0;k<3;k++)
		NewP[i][k] = RotM[0][k] * P[i][0] + RotM[1][k] * P[i][1] +
						 RotM[2][k] * P[i][2];
}

//-------------------------------------------------------------------------//
// Function Distance                                                       //
//                                                                         //
// Returns the distance between 2 points.                                  //
//-------------------------------------------------------------------------//
double Distance(double x1, double y1, double x2, double y2)
{
  return sqrt( ((x1 - x2) * (x1 - x2)) +  ((y1 - y2) * (y1 - y2)) );
}

//-------------------------------------------------------------------------//
// Procedure CFFT                                                          //
//                                                                         //
// Finds the FFT of an array data[1]-data[nn] (IQIQIQIQI etc),             //
// isign = 1 : FFT                                                         //
// isign = -1 IFFT (NOTE : no data[0])                                     //
//-------------------------------------------------------------------------//

void CFFT(double data[],int nn,int isign)

{ register int n, mmax, m, j, istep, i;
  double wtemp, wr, wpr, wpi, wi, theta, tempr, tempi;

  n=nn << 1; j=1;

  for (i=1; i<n; i+=2)
	 { if (j>i) { SWAP(data[j],data[i]); SWAP(data[j+1],data[i+1]); }
		m=n >> 1;
		while ( (m>=2) && (j>m) ) { j -= m; m >>= 1; }
		j += m;
	 }
  mmax=2;

  while (n>mmax)
	 { istep = 2*mmax; theta = 6.28318530717959/(isign*mmax);
		wtemp = -sin(0.5*theta); wpr = -2.0*wtemp*wtemp;
		wpi = -sin(theta); wr = 1.0; wi = 0.0;

		for (m=1; m<mmax; m+=2)
	{ for (i=m; i<=n; i+=istep)
		 { j = i+mmax;
			tempr = wr*data[j]-wi*data[j+1];
			tempi = wr*data[j+1]+wi*data[j];
			data[j]   = data[i]-tempr;
			data[j+1] = data[i+1]-tempi;
			data[i]   += tempr;
			data[i+1] += tempi;
		 }
	  wr = (wtemp=wr)*wpr-wi*wpi+wr;
	  wi = wi*wpr+wtemp*wpi+wi;
	}
		mmax=istep;
	 }
}
//-------------------------------------------------------------------------//
// Function DotProduct (double version)                                    //
//                                                                         //
// This function returns the dot product of the 2 double arrays given.     //
//-------------------------------------------------------------------------//
double DotProduct(double Array1[], double Array2[], int n)
{
  int i;
  double x = 0;

  for (i=0;i<n;i++)
	 x += Array1[i] * Array2[i];

  return x;
}
//-------------------------------------------------------------------------//
double GetValue(int what, double I, double Q)
{
  switch (what)
	 {
		case REAL:	{ return I; }
		case IMAGINARY: { return Q; }
		case MAGNITUDE: { return sqrt(I * I + Q * Q); }
		case PHASE: {
				if ((I != 0) || (Q != 0)) return atan2(Q,I);    
				else return 0; }
//    case DB_POWER_MAG : {     }



	 } // end switch
  return 0;
}
//-------------------------------------------------------------------------//
// return magnitude
double FindMag(double x[3])
{
  return sqrt(x[0]*x[0]+x[1]*x[1]+x[2]*x[2]);
}
//-------------------------------------------------------------------------//
// return cross product
void CrossP(double a[3], double b[3], double Result[3])
{
  Result[0] = a[1]*b[2] - a[2]*b[1];
  Result[1] = a[2]*b[0] - a[0]*b[2];
  Result[2] = a[0]*b[1] - a[1]*b[0];
}
//-------------------------------------------------------------------------//
void SubtractVec(double a[3], double b[3], double Result[3])
{
  Result[0] = a[0] - b[0];
  Result[1] = a[1] - b[1];
  Result[2] = a[2] - b[2];
}
//-------------------------------------------------------------------------//
void DivVec(double a[3], double fac, double Result[3])
{
  Result[0] = a[0] / fac;
  Result[1] = a[1] / fac;
  Result[2] = a[2] / fac;
}
//-------------------------------------------------------------------------//
void MultVec(double a[3], double fac, double Result[3])
{
  Result[0] = a[0] * fac;
  Result[1] = a[1] * fac;
  Result[2] = a[2] * fac;
}
//-------------------------------------------------------------------------//
void AddVec(double a[3], double b[3], double Result[3])
{
  Result[0] = a[0] + b[0];
  Result[1] = a[1] + b[1];
  Result[2] = a[2] + b[2];
}
//-------------------------------------------------------------------------//

// addition surf

// Return the angle between vector (1,0) and the given
// vector (x0,x1). The angle will increase in an anti-clockwise
// direction  and will lie between 0 and 2 Pi.
//---------------------------------------------------------------------------
double Angle2D(double x0,double x1)
{
  double an;
  if (x0==0 && x1==0) return 0; // undefined case
  an = atan2(x1,x0);
  if (an < 0) an += 2*PI;
  return an;
}
//---------------------------------------------------------------------------
void Swap(double *a, double *b)
{
  double temp;
  temp = *a;
  *a = *b;
  *b = temp;
}
//---------------------------------------------------------------------------
void Swap(int *a, int *b)
{
  int temp;
  temp = *a;
  *a = *b;
  *b = temp;
}
//---------------------------------------------------------------------------
double Dist3D(double x1[3], double x2[3])
{
  double dx,dy,dz,d;
  dx = x2[0]-x1[0];
  dy = x2[1]-x1[1];
  dz = x2[2]-x1[2];
  d = sqrt(dx*dx + dy*dy + dz*dz);
  return d;
}




// added for surfaces
#ifdef WITH_SURFACE_ALGORITHM
//---------------------------------------------------------------------------
void Intersect2D(double a,Vec w, Vec y, Vec *I1, Vec *I2, int *Sol)
{
  double det,sw0,sw1,sy0,sy1,yw0,yw1,sa;
  double lam1,lam2;

  Vec temp;
  // pre-calculate some squares and other constants
  sw0 = w[0]*w[0];
  sw1 = w[1]*w[1];
  sy0 = y[0]*y[0];
  sy1 = y[1]*y[1];

  yw0 = y[0]*w[0];
  yw1 = y[1]*w[1];

  sa = a*a;
  // find determinant
  det = 2*yw0*yw1-sw0*sy1+sw0*sa-sw1*sy0+sw1*sa;
  // no intersection points -> return
  if (det<0)
    {
      *Sol = 0;
      return;
    }
  // two (or one) intersections
 lam1 = (-yw0-yw1+sqrt(det))/(sw0+sw1);
 lam2 = (-yw0-yw1-sqrt(det))/(sw0+sw1);
 *Sol = 0;
 if ((lam1 >= 0) && (lam1 <=1))
   {
     *Sol=1;
     temp =lam1 * w;
     *I1 = y + temp;
   }
 if ((lam2 >= 0) && (lam2 <=1))
   {
     if (*Sol == 0)
       {
         *Sol=1;
         temp =lam2 * w;
         *I1 = y + temp;
         return;
       }
     else
       {
         *Sol=2;
         temp =lam2 * w;
         *I2 = y + temp;
         return;
       }
   }
}

//---------------------------------------------------------------------------
bool PutTriOnXY(Vec Tri[3],Vec FlatTri[3], Vec *Normal, double *d)
{
  Vec v0,v1,v2,temp,D,STri[3],X,Y;
  double TriArea,Az,El;
  int i;

  // set some initial values
  *d = 0;
  (*Normal)[0] = (*Normal)[1] = (*Normal)[2] = 0;
  for (i=0;i<3;i++) FlatTri[i] = !Tri[i];
  // create 'spanning' vectors
  v0 = !Tri[0];                // notice the ! is necessary to avoid ambiguities
  v1 = Tri[1] - Tri[0];        // vector from 0 to 1
  v2 = Tri[2] - Tri[0];        // vector from 0 to 2
  // calculate triangle area
  temp = v1.Cross(v2);
  TriArea = temp.Mag()*0.5;
  if (TriArea == 0) return false;  // no area -> return
  // calculate normal vector
  *Normal = temp * (double(1)/(TriArea * 2));
  // calculate distance to surface
  *d = (*Normal).Dot(Tri[0]);
  // if *d==0 then the plane will be containing the origin -> return
 // if (*d == 0) return;  // note shouldn't matter
  // calculate where normal sector would intersect surface
  D=(*Normal)* (*d);
  //shift the triangle by D
  STri[0] = Tri[0] - D;
  STri[1] = Tri[1] - D;
  STri[2] = Tri[2] - D;
  Az = Angle2D((*Normal)[0],(*Normal)[1]);
  El = -(*Normal).Elev() + 0.5*PI;
  X.Set(cos(El)*cos(Az),cos(El)*sin(Az),-sin(El));
  Y.Set(-sin(Az),cos(Az),0);
  for (i=0;i<3;i++)
    {
      FlatTri[i][0] = STri[i].Dot(X);
      FlatTri[i][1] = STri[i].Dot(Y);
      FlatTri[i][2] = 0;
    }
  return true;
}
//---------------------------------------------------------------------------
bool PtInTriangle2D(Vec Tri[3], Vec P)
{
  Vec a,b,c,d;
  double temp,l1,l2;
  a = !Tri[0];
  b = Tri[1] - Tri[0];
  c = Tri[2] - Tri[0];
  d = P - Tri[0];

  temp = b[0]*c[1] - b[1]*c[0];
  if (temp !=0) // take first two equations
    {
      l1 = (c[1]*d[0]-c[0]*d[1])/temp;
      l2 = (-b[1]*d[0]+b[0]*d[1])/temp;
      if ((l1>=0) && (l1<=1) && (l2>=0) && (l2<=1) && (l1+l2 <=1))
        return true;
      else
        return false;
    }
  else return false;  // triangle points are on a line ->  no solution
}
//---------------------------------------------------------------------------
Vec CenterPoint(Vec v1, Vec v2, double r)
{
  double l,a1,a2,a,ra;
  Vec temp;

  // find length of vector
  l = v1.Mag();
  // find angles relative to x-axis (counterclockwise = +)
  a1 = Angle2D(v1[0],v1[1]);
  a2 = Angle2D(v2[0],v2[1]);
  // how much is the second ahead of the first one ?
  a = a2-a1; if (a<0) a+=2*PI;
  // find a angle inbetween
  ra = a1+0.5*a;
  // create vector
  temp[0] = l*cos(ra);
  temp[1] = l*sin(ra);
  temp[2] = 0;
  return temp;
}
//---------------------------------------------------------------------------
double IntersectRadius(double a, double da, Vec Tri[3])
{
  Vec v0,v1,v2,temp,n,D,STri[3],RotTri[3],NRot,NRot2,FlatTri[3],w[3],
    I1,I2,Inter[6],Zero;
  double d,r,Res=0;
  int i,j, Sol,TotalSol;
  double VAngle[6];
  int AngleP[6];
  int p1,p2;
  Vec V;
  double t4;
  double InAng;
  double dP;

  Zero.Set(0,0,0);
  // check if triangle lies within the defined sphere - if yes, there
  // will be no line cutting the sphere => return 0
  if ((a > Tri[0].Mag()) && (a > Tri[1].Mag()) && (a > Tri[2].Mag()))
    return 0;
  // rotate triangle onto x-y plane
  if(!PutTriOnXY(Tri,FlatTri, &n, &d)) return 0;
  if (d<0) return 0; //loking at backface -> no return
  // calculate the radius of the 'intersection' circle
  if (a <= fabs(d)) return 0; //sphere is not touching or intersecting plane -> return0
  // find intersection circle radius
  r=sqrt(a*a - d*d);
  w[0] = FlatTri[1] - FlatTri[0];
  w[1] = FlatTri[2] - FlatTri[1];
  w[2] = FlatTri[0] - FlatTri[2];
  // calculate the intersection points of the triangle with the sphere
  TotalSol = 0;
  for (i=0;i<3;i++)
    {
      Intersect2D(r, w[i], FlatTri[i], &I1, &I2, &Sol);
      if (Sol == 1)
        Inter[TotalSol++] = !I1;
      else if(Sol == 2)
        {
          Inter[TotalSol++] = !I2;
          Inter[TotalSol++] = !I1;
        }
    }
  if (TotalSol == 1) return 0; // sphere is just touching a line
  // sphere intersecting triangle, but does not intersect triangle sides
  // -> check, normal vector should then intersect the triangle
  if (TotalSol == 0)
    {
      if (PtInTriangle2D(FlatTri, Zero))
        {
          Res = (2*PI);     // 'intersection' circle on triangle
          goto fini;
        }
      else
        return 0;
    }
  // calculate angles relative to K[0] (first intersection) to all
  // the other intersections
  for (i=0;i<TotalSol;i++)
    VAngle[i] = Angle2D(Inter[i][0],Inter[i][1]);
  // bubble sort the angles in ascending order
  for (i=0;i<TotalSol;i++)
    AngleP[i]=i;
  for (i=0;i<TotalSol-1;i++)
    for (j=i+1;j<TotalSol;j++)
      if (VAngle[i]>VAngle[j])
        {
          Swap(&(AngleP[i]),&(AngleP[j]));
          Swap(&(VAngle[i]),&(VAngle[j]));
        }


  for (i=0;i<TotalSol;i++)
    {
      p1 = i;
      p2 = i + 1;
      if (p2 == TotalSol) p2 = 0;
      V=CenterPoint(Inter[AngleP[p1]],Inter[AngleP[p2]],r);
      if (PtInTriangle2D(FlatTri, V))
        {
          t4 = VAngle[p2]-VAngle[p1];
          if (t4 < 0) t4 += 2*PI;
          Res += t4;
        }
    }

fini :
  dP = sqrt((a+da)*(a+da)-d*d)-r;
  // incidence angle of ray with plane
  InAng = atan2(r+0.5*dP,d);

  Res = Res*dP*(r+0.5*dP)*cos(InAng);
  return Res;
}
#endif
//#ifdef WITH_SURFACE_ALGORITHM



double Min(double a, double b)
{
  if (a<b) return a;
  else return b;
}

double Max(double a, double b)
{
  if (a>b) return a;
  else return b;
}
 /*
// TextSpace = how much space is available for text ?
// Range     = range of axis
// MinTicks  = minimum number of ticks drawn
// Unit      = returns one tick unit
// Ticks     = number of ticks actually used
void DrawLabels(double Range, int MinTicks, double *Unit, int *Ticks)
{
  int expo;
  double tempTicks;

  // find how how many ticks should be drawn
  tempTicks = SciNotation(Range,&expo);
  // Unit = difference from one tick to the next
  *Unit = pow10(expo);
	// adjust number of ticks
  while (tempTicks < MinTicks)
    {tempTicks *= 2; *Unit /= 2;}
	*Ticks = (int)floor(tempTicks);
}
*/
//---------------------------------------------------------------------------
