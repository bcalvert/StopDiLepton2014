#include "TLatex.h"
#include "TString.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TFile.h"
#include "TH2F.h"

#include "TStyle.h"
#include "TPaletteAxis.h"

#ifndef LimitDrawer_h
#define LimitDrawer_h
struct LimitDrawer {
        
    
    //Basic parameters of the analysis
    vector<TString> vecAnaCMSString;
    vector<float> vecAnaIntLumi;
    float intLumi;
    float beamEnergy;
    TString textPrelim;
    
    bool tryGlobal;
    TF1 * inputFuncEuler;
    float fudgeFactorEuler;
    ///parameters for grabbing contours
    TString baseHistName;
    bool isBasicExclusion;
    bool grabSmoothedCurves;
    TString nameSmoothedAddition;
    int minNumPoints;
    float desiredExclusion;
    
    
    //max observed significance
    float maxSigma;    
    
    ///Structure used to contain SUSY scenario information
    SUSYT2LimitParams SUSYT2LPs;
    
    ///Vectors of graph parameter information for drawing
    int numObs, numExp, numExtra;
    vector<GraphParams> vecGPs_Obs, vecGPs_Exp;
    vector<GraphParams> vecGPs_Extra;
    
    //Information for drawing the Legend
    vector<TGraph> vecLegendHackGraphs_Obs, vecLegendHackGraphs_Exp;
    vector<TGraph> vecLegendHackGraphs_Extra;
    
    vector<TString> vecStr_ExtraLegendHack;
    
    pairFloats xyStartVals, xyStartVals_ExtraTop, xyStartVals_ExtraBot;
    
    ////variables for making the limit contours
    vector<TString> vecAddName;
    
    
    vector<TString> vecHistNames_Obs, vecHistNames_Exp, vecHistNames_Extra;
    vector<TH2F *> vecSigStrengthLimitHists_Obs, vecSigStrengthLimitHists_Exp;
    vector<TH2F *> vecSigStrengthLimitHists_Extra;
    
    vector< vector<TGraph *> > vecVecContours_Obs, vecVecContours_Exp;
    vector< vector<TGraph *> > vecVecContours_Extra;
    
    //TFile containing all the info
    TFile * inFile;
    TString nameFile;
    vector<TFile *> vecCouplingFiles;
    //Histogram containing xsection limit map
    TH2F * h_XSecLimitMap;
    TString nameXSecMap;
    
    TH1F * h_SigDist;
    
    //Struct containing information about the SUSY scenario
    ////ScenarioDrawingParams inT2_DrawParams;
    //Canvas used for drawing everything
    TCanvas * drawCanv;
    CanvasParams canvDrawParams;
    

    //Variables used for drawing scenario information
    
    void InitializeEuler();
    void CorrectToGlobalSignificance();
    void DefaultVals(LimitParametersContainer *, bool);
    void SetSignifVals();
    void DefaultSigHistNames();
    void SetCanvasName(TString);
    void SetLoadingInformation(TFile *, TString, bool);
    
    ///Functions for grabbing histograms and making contours
    void GrabXSecLimitHist(bool);
    void GrabSignalStrengthHists(bool);
    void GrabExtraSignalStrengthHists(bool);
    void MakeContourVectors(bool, bool);
    
    
    ///Functions for drawing the data
    void FullDrawSequence(bool, bool);
    

    void DrawLegend_BasicExclusion(bool, bool);
    
    void DrawContours(bool);
    
    
    void FullSetupSequence_BasicExclusion(bool, bool);
    void FullSetupSequence_BasicSignif(bool, bool);
    void FullSetupSequence_Coupling(vector<SUSYT2LimitParams> *, vector<TFile *> *, bool);
//    void FullSetupSequence_Coupling_Hack(vector<SUSYT2LimitParams> *, vector<TFile *> *, bool);
    void FullSetupSequence_Coupling_Hack(bool);

    void InitializeCanvas();
    
    void SetHistoParams(bool);
    
    ///functions and member variables related to drawing of text
    LatexString latexCMS, latexModel, latexPaletteLabel;
    vector<LatexString> vecLatexAna;
    vector<LatexString> vecLatexLegendHack;
    GraphParams params_WhiteBackgroundGraph;
    TGraph graph_WhiteBackground;
    
    void SetText_All(bool, bool);
    void SetText_CMS(bool);
    void SetText_Ana(LimitParametersContainer *, bool);
    void SetText_Model(bool);
    void SetText_PaletteLabel(bool);
    void SetText_LegHack(bool, bool);
    void SetText_WhiteBackground(bool);
    
    
    void DrawText_All(bool, bool);
    void DrawText_CMS(bool);
    void DrawText_Ana(bool);
    void DrawText_Model(bool);
    void DrawText_PaletteLabel(bool);
    void DrawText_WhiteBackground();

    
    //Parameter setting functions
    
    
    void SetBasicParams();
    void SetBasicParamsSignif();
    void SetCouplingParams(vector<SUSYT2LimitParams> *, vector<TFile *> *);
    void SetVectorSizes();
    void SetVectorSizesSignif();
    void SetGraphParams();
    void SetSignifGraphParams();
    
    
    //variables for improving drawing quality and saving things
    int numInterpolate;
    bool runInterpolation;
    TString dirToSaveTo;
    
    ///Functions for improving drawing quality and saving things
    void DoInterpolation();
    void SaveCanvas();
    void SaveLimitInfo();
    
    
    void SetParamsFromCommandLine(int, char **);
};
#endif