#ifndef SUSYT2LimitParams_h
#define SUSYT2LimitParams_h
struct SUSYT2LimitParams {
    int typeT2, typeT2Fix; //0 for T2bw and 1 for T2tt
    int indexPol, indexPolFix;
    //11 different values for T2tt (representing SinTheta Eff in steps from 0 to 1 in 0.1 steps)
    //9 different values for T2bw (representing different coupling/chirality scenarios	for T2bw)
    
    int charFrac, charFracFix; //25, 50, or 75 for T2bw
    
    bool useFixParams;
    
    ScenarioDrawingParams currSDP; //structure that contains all relevant drawing information
    
    
    void DefaultVals();
    bool IsNominalScenario();
    
    void PrintStrings();
    
    TString LatexString();
    TString TitleString();
    
    TString SavingName(bool, int);
    
    void FixTheFixParams();
    
    void SetParams(bool, int, int);
    
    void SetStrings(float, float, float, bool);
    
    void DrawOnCanvas(TCanvas *, bool, bool);
    
    void FillVectorOfSUSYT2LPs(vector<SUSYT2LimitParams> *);
    
    void SetParamsFromCommandLine(int, char **);
    
    void FullSequence(int, char **);
};
#endif