//---------------------------------------------------------------------------
#ifndef WaitForThreadH
#define WaitForThreadH
//---------------------------------------------------------------------------
class TWaitThread : public TThread
{
  private:
    unsigned char Percent;
    void __fastcall DoVisualUpdate();
  protected:
	virtual void __fastcall Execute(void);
  public:
	__fastcall TWaitThread(void);
    double MaxMagnitude, TotalSampleNo, ClippedSamples,ZeroSamples;

    void UpdateProgress();
};
#endif
