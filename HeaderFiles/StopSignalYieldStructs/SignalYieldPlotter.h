#ifndef SignalYieldPlotter_h
#define SignalYieldPlotter_h

struct SignalYieldPlotter {
    //struct designed to plot signal efficiency maps or yields
    TCanvas * canvCV;
    TCanvas * tempCanv_SystUp;
    TCanvas * tempCanv_SystDown;
    vector<TCanvas *> vecCanv_SystUp;
    vector<TCanvas *> vecCanv_SystDown;
    
    vector<TString> vecSystNames, vecSystFinalNames;
    
    TString baseCanvName;
    TString baseCanvDir;
    TString canvAddName;
    TString baseCanvTitle;
    
    int whichPlotType;
    
    bool doDraw, callLatexSedScript, doAverage, showYields, doRatio, doSigContamRatio;
    bool drawSysts, makeFullSystCombo;
    
    TString strInFile, strFileDir, nameEffMap, strMT2Cut, strFileName, strT2SMS;
    
    
    float xLatex, yLatex, sizeLatex;
    TLatex strT2, strT2_SecondTitle;
    void SetLatex();
    
    void CallLatexSedScript(bool);
    void SetStringsOutLatex(CoarseMapMaker *, SUSYT2LimitParams *);
    void SetCanvas(int);
    void SaveCanvas(int);
    void SetHist(TH2F *, TString);
    void DrawHist(LeptFiltSignalAssociator *, CoarseMapMaker *, SmoothMapMaker *, SignalYieldMaker *, SUSYT2LimitParams *, bool);
    void DrawHist(SignalYieldMaker *, CoarseMapMaker *, bool);
    
    void DefaultVals(int);
    void SetCanvasAddName(SUSYT2LimitParams *, CoarseMapMaker *);
    void InitializeVecs(int);
    
    void Draw(LeptFiltSignalAssociator *, CoarseMapMaker *, SmoothMapMaker *, SignalYieldMaker *, SUSYT2LimitParams *, bool);
    
    void SetParamsFromCommandLine(int argc, char **);
};
#endif