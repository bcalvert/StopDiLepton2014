#ifndef AXIS_T_H
#define AXIS_T_H

#include "InputHistBinBounds.h"

struct AxisT {
    TString axisLabel;
    int numBins;
    int rebinNum;
    float axisMin, axisMax;
    TString axisVarKey;
    bool doAxisSyst;
    float axisLB, axisUB;
    int axisSpecialType;
    int axisVarMapType; // 0 NonMET; 1 MET
    int axisVarCode;

    void SetAxis(TString inLabel, TString inAxisVarKey, int inNBins, float inAxisMin, float inAxisMax, bool inDoAxisSyst) {
        axisLabel = inLabel;
        axisVarKey = inAxisVarKey;
        numBins = inNBins;
        axisMin = inAxisMin;
        axisMax = inAxisMax;
        doAxisSyst = inDoAxisSyst;
    }
    void SetAxis(TString inLabel, TString inAxisVarKey, InputHistBinBounds * inHBB, bool inDoAxisSyst, float nBinsScale = 1) {
        axisLabel = inLabel;
        axisVarKey = inAxisVarKey;        
        numBins = (int) (inHBB->nBins * nBinsScale);
        axisMin = inHBB->axisLB;
        axisMax = inHBB->axisUB;
        doAxisSyst = inDoAxisSyst;
    }
    void SetAxisSpecial(TString inLabel, TString inAxisVarKey, int inAxisSpecialType, bool inDoAxisSyst) {
        axisLabel = inLabel;
        axisVarKey = inAxisVarKey;
        numBins = -1;
        axisSpecialType = inAxisSpecialType;
        doAxisSyst = inDoAxisSyst;
    }
    void SetAxisAsDefaultCountAxis(TString unitString, TString addString = "NUM ") {
        axisLabel = "Number of Events / ";
        axisLabel += addString;
        axisLabel += unitString;
    }
};

#endif // AXIS_T_H
