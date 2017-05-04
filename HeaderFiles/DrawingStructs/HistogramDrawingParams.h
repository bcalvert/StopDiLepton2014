#ifndef HistogramDrawingParams_h
#define HistogramDrawingParams_h

#include "TCanvas.h"
#include "TPad.h"
#include "TString.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
struct HistogramDrawingParams {
    TString histTitle;
    
    
    AxisParams xAxisParams, yAxisParams, zAxisParams;
    
    void SetHistParams(TH1 *);
    
    void DefaultVals();
    
    void SetAxesAsLimitAxes(TString, TString, TString);    
    
};

#endif