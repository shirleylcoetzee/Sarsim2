  PulseExtendInSec = CurrentSim->PWidth * r->PulseWidth;
  if (CurrentSim->SimTYPE == RAW_RETURN)
    SecPerSample = (SlantEndTime-SlantStartTime)/double(DataXSize);
  else if (CurrentSim->SimTYPE == MATCHED_FILTER)
    SecPerSample = double(1) / (CurrentSim->SampleFreq*2);

  // output (NOT oversampled) array parameters
  // used (non-zero) samples
  UsedSamples = round(0.5*PulseExtendInSec / SecPerSample);
  // how many samples need to be stored ?
  // NOTE - as we actually display range and not range-delay, the
  // pulse is actually only half it's real length
  // Also use power of 2 samples
  Pow2SamplePoints = (long)(pow(2,ceil(log(ceil(0.5*PulseExtendInSec /
    SecPerSample))*double(1.442695040888964))));

  // calculate pulse template
  Template = DVector(0,Pow2SamplePoints*2);
  // set all to zero
  for (CSample=0;CSample<Pow2SamplePoints*2;CSample++)
    Template[CSample] = 0;

  // internal (oversampled) array parameters
  SecPerSample2 = double(0.5)/(r->ChirpBW*CurrentSim->OverSampleFactor);
  if (SecPerSample2 > SecPerSample)
    SecPerSample2 = SecPerSample;
  // used (non-zero) samples
  UsedSamples2 = round(0.5*PulseExtendInSec*(CurrentSim->OverSampleFactor/
  CurrentSim->PWidth) / SecPerSample2);
  Pow2SamplePoints2 = (long)(pow(2,ceil(log(ceil(0.5*PulseExtendInSec*
  (CurrentSim->OverSampleFactor/ CurrentSim->PWidth) /SecPerSample2))*double(1.442695040888964))));
  // calculate pulse OverSampledTemplate
  OverSampledTemplate = DVector(0,Pow2SamplePoints2*2);
  // set all to zero
  for (CSample=0;CSample<Pow2SamplePoints2*2;CSample++)
    OverSampledTemplate[CSample] = 0;

  // from what time to what time do we have the pulse
  PulseBegin2 = round(double(CurrentSim->OverSampleFactor-1)*double(UsedSamples2)/
    (double(2)* double(CurrentSim->OverSampleFactor)));
  PulseEnd2 = round(double(CurrentSim->OverSampleFactor+1)*double(UsedSamples2)/
    (double(2)* double(CurrentSim->OverSampleFactor)));

  // calculate OverSampledTemplate
  for (CSample=0; CSample<UsedSamples2; CSample++)
    {
      t = (double(2)*(double)CSample - (double)UsedSamples2) * SecPerSample2;
      else if (r->PulseType == CHIRP)
        {
          Phase = fmod(PI*DelaySlope*t*t,TwoPi);
          if ( (CSample > PulseBegin2) && (CSample <= PulseEnd2) )
            {
              OverSampledTemplate[CSample*2] = cos(Phase);
              OverSampledTemplate[CSample*2+1] = sin(Phase);
            }
          else
            {
              OverSampledTemplate[CSample*2] = 0;//.01*cos(Phase);
              OverSampledTemplate[CSample*2+1] = 0;//.01*sin(Phase);
            }
        }
        }
    } // end for CSample

  // convert into bandlimited pulse
  if (CurrentSim->OverSampleFactor != 1)
    {
      HalfBWSamples = double(Pow2SamplePoints2) / (2*CurrentSim->OverSampleFactor);
      CFFT(OverSampledTemplate-1,Pow2SamplePoints2,1);
      for (CSample=HalfBWSamples;CSample<Pow2SamplePoints2-HalfBWSamples;CSample++)
        {
         OverSampledTemplate[CSample*2] = 0;
         OverSampledTemplate[CSample*2+1] = 0;
        }
      CFFT(OverSampledTemplate-1,Pow2SamplePoints2,-1);
      // normalize
      Mult = double(1)/double(Pow2SamplePoints2);
      for (CSample=0; CSample<UsedSamples2; CSample++)
        {
          OverSampledTemplate[CSample*2] *= Mult;
          OverSampledTemplate[CSample*2+1] *= Mult;
        }
    }
  // downsample again
  //
//  ZeroedSamples = (double(UsedSamples)-((double(UsedSamples)/CurrentSim->OverSampleFactor)*
 //      CurrentSim->PWidth))*double(0.5);
 // if (ZeroedSamples <= 0) ZeroedSamples = 0;
  ZeroedSamples = 0;
  for (CSample=0; CSample<UsedSamples2; CSample++)
    {
      RSample = round(double(CSample) * (double(UsedSamples)/double(UsedSamples2)));
      if ((RSample >=ZeroedSamples) && (RSample < (UsedSamples-ZeroedSamples)))
        {
              Template[RSample*2] = OverSampledTemplate[CSample*2];
              Template[RSample*2+1] = OverSampledTemplate[CSample*2+1];
        }
    }





      // for all targets
  for (TNo=0;TNo<TargetNo;TNo++)
    {
      AddPhase = fmod(TwoPi * (   // phase shift due to range
                             -(PulseFreq[PNo]*RangeDelay[TNo][PNo])  ),TwoPi);
      SinCalc = sin(AddPhase);
      CosCalc = cos(AddPhase);

      PulseInsertStart = round((double(0.5)*(RangeDelay[TNo][PNo]+
        double(0.5)*r->PulseWidth - PulseCenter) - SlantStartTime)
        /SecPerSample - double(UsedSamples)*double(0.5))
        ;
/*
      PulseInsertStart = round((double(0.5)*(RangeDelay[TNo][PNo]+
        double(0.5)*r->PulseWidth - PulseCenter) - SlantStartTime)
        /SecPerSample);
 */
      for (CSample=ZeroedSamples;CSample<UsedSamples-ZeroedSamples;CSample++)   //
        {
          RealSample = CSample + PulseInsertStart;
          if ((RealSample >= 0) && (RealSample < SamplePoints))
            {
              Amp = ReturnAmp[TNo][PNo];
              OnePulse[RealSample*2] += Amp*
                ((Template[CSample*2]*CosCalc)-((Template[CSample*2+1]*SinCalc)));
              OnePulse[RealSample*2+1] += Amp*
                ((Template[CSample*2+1]*CosCalc)+((Template[CSample*2]*SinCalc)));
            }

        }
    } // end for all targets






