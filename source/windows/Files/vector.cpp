//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
#include "vector.h"
#include "math.h"

//---------------------------------------------------------------------------
Vec::Vec(int m)   // constructor Vec a(3)
{
  if (m<=0) m = 1;
  row=m;
  dm=(double *) new double [row];
  int i;
  for(i=0;i<row;i++) dm[i]=0;
}
//---------------------------------------------------------------------------
Vec::Vec(void)   // constructor Vec a()
{
  row=3;
  dm=(double *) new double [row];
  int i;
  for(i=0;i<row;i++) dm[i]=0;
}
//---------------------------------------------------------------------------
Vec::Vec(Vec &a)   // constructor Vec a(b);
{
  int i;
  dm=(double *) new double [a.row];
  row=a.row;
  for(i=0;i<row;i++)
    dm[i]=a.dm[i];
}
//---------------------------------------------------------------------------
Vec::~Vec(void)  // destructor
{
 delete dm;
 row=0;
}
//---------------------------------------------------------------------------
int Vec::rownum(void)  // size
{
 return row;
}
//---------------------------------------------------------------------------
double & Vec::operator[](int i)  // access : double x = a[3]
{
 return dm[i];
}
//---------------------------------------------------------------------------
Vec Vec::operator+(Vec &op)      // + operator
{
 Vec temp(row);
 if(row!=op.row) {cout<<"Parameter not match in Vec +\n";exit(0);}
 int i;
 for(i=0;i<row;i++) temp.dm[i]=dm[i]+op.dm[i];
 return temp;
}
//---------------------------------------------------------------------------
Vec Vec::operator-(Vec &op)     // - operator
{
 int size=min(row,op.row);
 Vec temp(size);
 int i;
 for(i=0;i<size;i++) temp.dm[i]=dm[i]-op.dm[i];
 return temp;
}
//---------------------------------------------------------------------------
Vec Vec::operator*(Vec &op)        // multiplication a=b*c
{
 int size=min(row,op.row);
 Vec temp(size);
 int i;
 for(i=0;i<size;i++) temp.dm[i]=dm[i]*op.dm[i];
 return temp;
}
//---------------------------------------------------------------------------
Vec &Vec::operator=(Vec &op)        // handle the a=b case
{
 int i;
 int size=min(row,op.row);
 for(i=0;i<size;i++)
  dm[i]=op.dm[i];
 return *this;
}
//---------------------------------------------------------------------------
Vec &Vec::operator=(Vec op)        // handle the a=b+c case
{
 int i;
 int size=min(row,op.row);
 for(i=0;i<size;i++)
  dm[i]=op.dm[i];
 return *this;
}
//---------------------------------------------------------------------------
Vec operator*(double x,Vec &op)    // handle the a=3*b case
{
 int i;
 Vec temp(op.row);
 for(i=0;i<op.row;i++)
  temp.dm[i]=x*op.dm[i];
 return temp;
}
//---------------------------------------------------------------------------
Vec operator*(Vec &op,double x)     // handle the a=b*3 case
{
 int i;
 Vec temp(op.row);
 for(i=0;i<op.row;i++)
  temp.dm[i]=x*op.dm[i];
 return temp;
}

//---------------------------------------------------------------------------
double Vec::Dot(Vec v)        // dot product a=b.c
{
 int size=min(row,v.row);
 int i;
 double temp=0;
 for(i=0;i<size;i++)
   temp += dm[i]*v.dm[i];
 return temp;
}
//---------------------------------------------------------------------------
Vec Vec::Cross(Vec v)   // return cross product
{
 Vec temp(3);  // cross product only defined for 3d
 if ((v.row !=3) || (row!=3)) return temp;
 temp[0] = dm[1]*v[2]- dm[2]*v[1];
 temp[1] = dm[2]*v[0]- dm[0]*v[2];
 temp[2] = dm[0]*v[1]- dm[1]*v[0];
 return temp;
}
//---------------------------------------------------------------------------
double Vec::Mag(void)        // magnitude
{
 double temp = 0;
 int i;
 for(i=0;i<row;i++)
   {
     temp += dm[i]*dm[i];
   }
 temp = sqrt(temp);
 return temp;
 }
//---------------------------------------------------------------------------
Vec Vec::operator!(void)     // use this operator to get a pointer to the
                             // vector (used to adress arrays) 
{
 return *this;
}
//---------------------------------------------------------------------------
Vec Vec::Set(double a, double b, double c)     //initialize e.g. a.Set(2,3,4)
{
 int size=row;
 if (size > 0) dm[0] = a;
 if (size > 1) dm[1] = b;
 if (size > 2) dm[2] = c;
 return *this;
}
//---------------------------------------------------------------------------
// The following function finds the angle between the X-Y plane and a      //
// and a vector to the given point.                                        //
double Vec::Elev(void)
{
  const double ZERO_THRESHOLD = 1E-100;

  double temp;
  temp = sqrt(dm[0]*dm[0]+dm[1]*dm[1]+dm[2]*dm[2]);
  if (temp < ZERO_THRESHOLD)
	  return 0;
  return asin(dm[2]/temp);
}


