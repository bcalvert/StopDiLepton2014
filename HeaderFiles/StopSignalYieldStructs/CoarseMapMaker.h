#ifndef CoarseMapMaker_h
#define CoarseMapMaker_h
struct CoarseMapMaker {
    //Structure used to calculate the "coarse" -- that is, no smoothing done to the signal -- efficiency
    //for each mass point in the 2D SUSY plane to pass a given MT2 cut (whether 1D, 2D, or 3D)
    
    LimitParametersContainer * LPC;
    vector<TString> vecDilepChannelName;
    
    vector<TFile *> vecSignalFiles;
    TFile * inNormFile;
    bool isLeptFilt;
    
    TString histBaseName;
    
    
    TString baseDirFile;
    TString baseNameFile, midNameFile, endNameFile;
    TString strStop, strChi0, strChargino, strPerPol;
    TString baseDirNormFile;
    
    TString strFullSel;
    
    int MT2llCut, MT2lblbCut, MT2bb_ZMETCut;
    bool whichIntType;
    
    TH2F * outHist_CV;
    TFile * outFile;
    
    bool doControl;
    int cutMT2ll_Control, cutMT2lblb_Control, cutMT2bb_ZMET_Control;
    
    bool loadXSec;
    TString xsecFileName, xsecHistName;
    TFile * crossSectionFile;
    TH1F * crossSectionHist;
    
    double effCV;
    double errEffCV;
    vector<double> vecEff_SystVarUp, vecErrEff_SystVarUp;
    vector<double> vecEff_SystVarDown, vecErrEff_SystVarDown;
    
    
    //Some variables for cross-checking//
    double numPassCutScaled, numPassCutScaledErr, numTotal;
    //Some variables for cross-checking//
    
    vector<TString> vecSystNames, vecSystFinalNames;
    
    vector<TH2F *> vecOutHist_SystVarUp;
    vector<TH2F *> vecOutHist_SystVarDown;
    
    int massStopThresh;
    
    void PrintBasic_Part1(bool, int);
    void PrintBasic(bool, int);
    float FindXSec(int);
    void LoadXSec();
    void DeleteHists();
    void CloseFiles();
    
    void DefaultCuts();
    void DefaultVals(LeptFiltSignalAssociator *, LimitParametersContainer *);
    void DoOverflow(TH2F *);
    void DoOverflow();
    void CheckCombineHistBin(double, double, double);
    void CombineHistBin(int, int, int, TH2F *, bool);
    void SetBin(int, int);
    TString FullPathFile(TString, int, int, int, int);
    void SetFiles(int, int, SUSYT2LimitParams *, massMap *, bool);
    
    void CalculatePDFUncert(vector<TFile *> *, TString, int, float, float, int *, double&, double&, double&, double&, bool);
    
    void SetEff(LeptFiltSignalAssociator *, int, int, float, bool, bool, int);
    void SetHistString();
    void InitializeVecs(bool);
    TString GetMT2String(bool);
    TString GetMT2LatexString(bool);
    TString GetMT2TitleString(bool);
    
    void InitializeHistsandOutfile(SUSYT2LimitParams *, int, bool);
    void WriteFile();
    
    void SetMCStatEff(bool);
    
    void SetParamsFromCommandLine(int, char **);
    
    
    void CombineCoarseEffs(int, int, CoarseMapMaker *, int);
};
#endif