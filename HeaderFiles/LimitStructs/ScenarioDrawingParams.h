#ifndef ScenarioDrawingParams_h
#define ScenarioDrawingParams_h

#include "TString.h"
#include "TLatex.h"
#include "TLine.h"

typedef pair<float, float> pairFloats;

//Structure containing the relevant SUSY information about the model I'm plotting limits for

struct ScenarioDrawingParams {
    //Strings used for saving
    TString string_SaveName; //string used for saving canvases, root files, etc. -- is a complete string including strPol
    TString strPol, strT2SMS, strCharFracDir, strCharFracName;
    
    ////Strings used for LaTeX///
      //main strings
    LatexString latexTitleString_TLatex, latexSecondTitleString_TLatex;
    TString strLegend;
    //strings used for drawing TLatex in ROOT plots, includes information on relative positioning
    
    
    TString latexTitleString_RealLatex; //string used for latex string in actual latex files;
    TString latexSecondTitleString_RealLatex; //string used for latex string in actual latex files;

    bool isHack;
    
    TString labelXaxis, labelYaxis;
    
    TString SavingName(bool isDir, int);
    
    //Functions/variables related to drawing kinematic lines
    vector<KinematicLine> vecKinematicLines;
    XYBounds bounds_SUSYMasses;
    float binWidth;
    float hackDeltaMRegion;
    void SetVectorSize();
    
    void DrawOnCanvas(TCanvas *, bool, bool);
    
    //Functions for setting the kinematic scenarios
    void SetAsT2tt(int, float, float);
    void SetAsT2bw(int, int, float, float);
    void SetAsT2tb(int);
    
    ///debugging functions
    void Print();
};
#endif