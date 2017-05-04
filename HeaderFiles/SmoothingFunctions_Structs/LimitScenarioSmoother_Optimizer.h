#ifndef LimitScenarioSmoother_Optimizer_h
#define LimitScenarioSmoother_Optimizer_h

#include "TCanvas.h"
#include "TString.h"

struct LimitScenarioSmoother_Optimizer {
    
    TString addNameSmoothing;
    
    LimitScenarioContainer LSC_Unsmoothed, LSC_Smoothed;
    SignifScenarioContainer SSC_Unsmoothed, SSC_Smoothed;
    
    Smoother scenarioSmoother;
    
    void SetZeroNonExcl(bool);
    void DrawLatex(TCanvas *, int);
    void DrawHist(bool, int, int, int, int, TString);
    void DrawSignifHist(bool, int, int, int, int, TString);
    
    void SetHistsAsConstDeltaM(bool);
    
    void LoadHists(TFile *, bool, bool);
    void LoadSignifHists(TFile *, bool);
    
    void DefaultVarVals(LimitParametersContainer *, SUSYT2LimitParams *);
    void SetPathAsDrawingPath();
    void DoSmoothing(TH1F *, bool);
    void DoSignifSmoothing(bool);
    void WriteFile(TFile *, bool);
    void WriteSignifToFile(TFile *, bool);
    
    void SetParamsFromCommandLine(int, char **);
};
#endif