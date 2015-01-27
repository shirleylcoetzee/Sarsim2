#ifndef graphH
#define graphH

// What has to be painted
#define FRAME 1
#define UNITS 2

// number of ticks on graph
#define MIN_XTICK 7
#define MIN_YTICK 7

//-------------------------------------------------------------------------//
// Class TGraph                                                            //
// ------------                                                            //
// Generic class to draw frames with units and graphs.                     //
//-------------------------------------------------------------------------//
class TGraph {
  private:
    // private functions
  //  double SciNotation(double N,int *expo);
    void CreateLabel(double Value,int DecPlaces, int Units, char *str);
    // private variables
    // pixels available for drawing
    TCanvas *GraphCanvas;         // Canvas of graph

  public:
    // functions
    TGraph(TCanvas *ThisCanvas,int UnitsX,int UnitsY, int XDecPlaces,
           int YDecPlaces);       // constructor
    ~TGraph();                    // destructor
    void Init(TRect Border,double XStart,
              double XEnd, double YStart,double YEnd);
    void Paint(int What);
    //bool ClipLine(double YStart, double YEnd, double *X1, double *Y1,
//		          double *X2, double *Y2);
    void DrawLineCanvas(TCanvas *ThisCanvas, double X1, double Y1,
                        double X2, double Y2);
    void DrawGraph(TCanvas *ThisCanvas, long NP, double *XP, double *YP,
                      double **Coeff,int GraphType, int IntMethod);

    // variables
    // corners of graph
    double XStart, XEnd, YStart, YEnd;
    double XRange,YRange;
    double PixelX, PixelY;
    TRect   GraphArea;            // total available area excluding frame
    int UnitsX, UnitsY;
    int XDecPlaces, YDecPlaces;

};
//---------------------------------------------------------------------------
#endif