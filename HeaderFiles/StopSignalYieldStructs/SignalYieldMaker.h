#ifndef SignalYieldMaker_h
#define SignalYieldMaker_h
struct SignalYieldMaker {
    //Structure used to calculate the signal yield, using the smoothed efficiency map, for each point in the 2D SUSY plane to pass a given MT2 cut (whether 1D, 2D, or 3D)
    TFile * outFile;
    
    vector<TString> vecSystNames, vecSystFinalNames, vecSystFinalNames_TTBar;
    
    TH2F * inSmoothHist_CV;
    
    vector<TH2F *> vecInSmoothHist_SystVarUp;
    vector<TH2F *> vecInSmoothHist_SystVarDown;
    
    TH2F * outYieldHist_CV;
    vector<TH2F *> vecOutYieldHist_SystVarUp;
    vector<TH2F *> vecOutYieldHist_SystVarDown;
    
    float lumiData;
    bool loadXSec;
    TString xsecFileName, xsecHistName;
    TFile * crossSectionFile;
    TH1F * crossSectionHist;
    
    bool doSigContam;
    TString nameFileTTBarYieldControl, nameFileTTBarYieldSignal;
    float yieldTTBarControl, yieldTTBarSignal;
    float statTTBarControl, statTTBarSignal;
    vector<float> vecYieldTTBarControl_SystVarUp, vecYieldTTBarControl_SystVarDown;
    vector<float> vecYieldTTBarSignal_SystVarUp, vecYieldTTBarSignal_SystVarDown;
    vector<float> vecVals;

    void DefaultVals();
    
    float FindXSec(int);
    float FindXSec(float);
    void LoadXSec();
    void InitializeVecs();
    void NormalizeSigYieldByTTBar();
    void SetSigContamYields(TString, bool);
    
    void PrintYieldCV(float, float, TString);
    void PrintYieldSyst(float, float, float, TString, TString, bool);
    
    void PrintYieldMassPoint(massCombo *, bool, bool, SignalYieldMaker *);
    
    void PrintYields(bool, bool, SignalYieldMaker *);
    
    void MakeYieldHists(bool);
    
    
    void ReadSmoothMaps(SmoothMapMaker *, SmoothMapMaker *, bool);
    
    void InitializeHistsandOutfile(SUSYT2LimitParams *, CoarseMapMaker *, bool, bool, bool);
    void WriteFile();
    void SetParamsFromCommandLine(int argc, char **);
};
#endif