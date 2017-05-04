#ifndef SignalYieldMaker_IndivPoint_h
#define SignalYieldMaker_IndivPoint_h
struct SignalYieldMaker_IndivPoint {
    //Struct that prints out all the relevant info, including central value
    //as well as systematics for each mass point in the SUSY 2D mass plane
    //Also calculates signal contamination for these points
    
    vector<TString> vecSystNames; //vector that contains names for input systs and names for final systs;
    
    //The current histogram being read in -- this is a 2D histogram in the mass plane
    TH2F * currInHist;
    //The current systematic variation histograms being read in -- these are also 2D histograms in the mass plane
    vector<TH2F *> vecCurrInHist_SystVarUp;
    vector<TH2F *> vecCurrInHist_SystVarDown;
    
    int numDims;
    
    ////Stuff for the output
    //The output MT2 histograms -- for now will be 2D
    TH1F * outMT2Hist_CV_1D;
    vector<TH1F *> vecOutMT2Hist_1D_SystVarUp;
    vector<TH1F *> vecOutMT2Hist_1D_SystVarDown;
    
    TH2F * outMT2Hist_CV_2D;
    vector<TH2F *> vecOutMT2Hist_2D_SystVarUp;
    vector<TH2F *> vecOutMT2Hist_2D_SystVarDown;
    
    TH3F * outMT2Hist_CV_3D;
    vector<TH3F *> vecOutMT2Hist_3D_SystVarUp;
    vector<TH3F *> vecOutMT2Hist_3D_SystVarDown;
    
    TH1 * histSig_SigStatControlUp, * histSig_SigStatControlDown;
    TH1 * histSig_TTBarStatContUp, * histSig_TTBarStatContDown;
    TH1 * histSig_TTBarMCStatUp, * histSig_TTBarMCStatDown;
    
    //vector to contain the histogram axes
    vector< vector<Double_t> > vecAxes;
    vector<int> vecNumBinsAxes;
    // vector to specify which axis is which MT2
    vector<int> vecWhichMT2;
    vector<TString> vecMT2Name;
    TString axisString;
    
    
    //Variables used for file load and out
    
    TFile * outFile; // file that will be made containing the individual file
    TString inBaseDir, outBaseDir;
    
    TString massStop, massLSP;
    TString outFileName, outHistBaseName;
    
    TString baseNameInFile;
    
    // I have a function to set the axes and a function to initialize very basic variables
    
    // I need a function to initialize the input directory and output directorys based on input stop mass
    // and also book the output file
    // I need a function to read in an individual MT2Cut Signal Yield histogram and fill in the appropriate bin entry in my output histogram
    // I need a function to iterate over the MT2 cuts and call the above histogram
    // I need a function that calls the above function for central value and systematic variations
    // A function that runs the whole damn chain
    
    void DoSignalContamination_SpecHist(TH1 *, TH1 *, int, int, bool);
    void DoSignalContamination(TFile *, bool);
    void SetAxes();
    void DefaultVals(int);
    void SetStringSignal(SUSYT2LimitParams *, SUSYMassLimitParams *, LimitParametersContainer *, bool);
    void InitializeVecs();
    void FixMCStats(bool);
    void SetYieldInputHist(float, vector<int> *, TH1 *, bool);
    void SetYieldInputFile(massCombo *, TFile *, TString, vector<int> *, bool);
    void IterateOverCuts(massCombo *, bool);
    void InitializeHistsandOutfile(bool);
    void WriteFile(bool);
};
#endif