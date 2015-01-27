//---------------------------------------------------------------------------
#ifndef vectorH
#define vectorH

class Vec
{
  double *dm;
  int row;
 public:
  Vec(int m);
  Vec(Vec &a);
  Vec(void);
  ~Vec(void);
  int rownum(void);
  double &operator[](int i);
  Vec operator+(Vec &op);
  Vec operator-(Vec &op);
  Vec operator*(Vec &op);
  Vec &operator=(Vec &op);
  Vec &operator=(Vec op);
  friend Vec operator*(double x,Vec &op);
  friend Vec operator*(Vec &op,double x);
  Vec Cross(Vec v);   // return cross product
  double Mag(void);       // magnitude
  Vec operator!(void);
  Vec Set(double a, double b, double c);     //initialize e.g. a.Set(2,3,4)
  double Dot(Vec v);        // dot product a=b.c
  double Elev(void);

 };

//---------------------------------------------------------------------------
#endif
