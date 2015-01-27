//---------------------------------------------------------------------------
#ifndef CalcThreadH
#define CalcThreadH
#include "objectstructdef.h"
#include "simuform.h"
//---------------------------------------------------------------------------
class TCalcThread : public TThread
{
  private:
    void __fastcall DoVisualUpdate(void);
    int Percentage;
  protected:
	virtual void __fastcall Execute(void);
  public:
	__fastcall TCalcThread(void);
    // data is written into following arrays (used to transfer data)
    double **Data;
    double *PulseSendTime;
    long PulseNo;
    class TSimulationForm *ParentForm; // for showing progress
    // should the range be adjusted to show all targets ?
    bool ShowAllTargets;
  // parameters used to do calculations
  int PixelX, PixelY;
  struct SRadar *CRadar;
  struct SSimulation CurrentSim;
  double MaxMagnitude;
};
#endif