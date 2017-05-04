#ifndef SmoothMapMaker_h
#define SmoothMapMaker_h
struct SmoothMapMaker {
    //Structure used to calculate the "smoothed" -- that is, efficiency with Swiss-Cross smoothing applied -- efficiency
    //for each mass point in the 2D SUSY plane to pass a given MT2 cut (whether 1D, 2D, or 3D)
    TFile * inFile;
    TFile * outFile;
    
    TH2F * histCV;
    
    vector<TH2F *> vecHist_SystVarUp;
    vector<TH2F *> vecHist_SystVarDown;
    
    vector<TH2F *> vecInterpHist;
    
    Smoother scenarioSmoother;
    
    int numRebins;
    TString addName, addNameSmooth;
    string dirToRW;
    void DefaultVals();
    
    void PrintBasic();
    
    void SetFile(CoarseMapMaker *);
    
    void SetHists(CoarseMapMaker *);
    void DoSmoothing(CoarseMapMaker *, bool);
    
    void DoInterpolation(bool);
    void DoInterpolation_Old(bool);
    
    void InitializeHistsandOutfile(SUSYT2LimitParams *, CoarseMapMaker *, bool, bool);
    void WriteFile(bool);
    
    void SetParamsFromCommandLine(int, char **);
    
    void CombineSmoothEffs(SmoothMapMaker *, int);
    
    void DefaultSmootherValues();
    
};
#endif