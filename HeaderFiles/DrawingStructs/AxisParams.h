#ifndef AxisParams_h
#define AxisParams_h

#include "TAxis.h"
#include "TString.h"
struct AxisParams {
    
    float sizeLabel, sizeTitle;
    float offsetLabel, offsetTitle;
    int fontLabel, fontTitle;
    
    TString stringTitle;
    
    void DefaultVals();
    
    void SetAsLimitAxis(int, TString);
    
    void SetInputAxisParams(TAxis *);
};
#endif