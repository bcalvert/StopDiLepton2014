#ifndef LeptFiltSignalAssociator_h
#define LeptFiltSignalAssociator_h
struct LeptFiltSignalAssociator {
    //Structure used as part of calculating the lepton filter efficiency for input points
    //The lepton filter claims to be approximately a 53% efficient filter, but this can depend upon
    //the given input mass point and decay mode -- we have non-lepton filtered data for every lepton-filtered point
    //however, so we can just calculate the damn efficiency ourselves
    
    vector<TFile *> vecFileLeptFilter, vecFileNonLeptFilter;
    
    TFile * inNormFileLeptFilt, * inNormFileNonLeptFilt;
    TString baseDirFile;
    TString baseNameFile, midNameFile, endNameFile;
    vector<TString> vecDilepChannelName;
    TString strStop, strChi0, strChargino, strPerPol;
    TString baseDirNormFile;
    
    TH2F * outHist;
    TFile * outFile;
    
    
    LimitParametersContainer * LPC;
    
    /*
     void PrintBasic(SUSYT2LimitParams * inSUSYT2LP) {
     TString strSMS = isT2tt ? "T2tt" : "T2bw";
     cout << "Running on " << strSMS << " with paramSMS = " << paramSMS << endl;
     cout << "Saving to " << outFile->GetName() << endl;
     }
     */
    
    void FillInGapsTightBin();
    void CloseFiles();
    void WriteFile();
    
    void DefaultVals(int, LimitParametersContainer *);
    
    void SetHistAndOutFile(SUSYT2LimitParams *, bool, bool);
    void SetVecs();
    
    TString FullPathFile(TString, int, int, int, int);
    
    void SetFiles(int, int, SUSYT2LimitParams *, labelMap *, bool);
    
    void SetBin(int, int, float, bool, bool);
    void SetBinT2tb(int, int, bool);
    
    void SmoothMap();
    
};
#endif