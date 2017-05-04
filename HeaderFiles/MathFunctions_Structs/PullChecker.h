#ifndef PullChecker_h
#define PullChecker_h

struct PullChecker {
    int currCheckBinX, currCheckBinY;
    int nBinsToCheckOver_X, nBinsToCheckOver_Y;
    //How many bins to look left of right of along axis;
    
    int nBinsExtraToSync_X, nBinsExtraToSync_Y;
    //How many bins to look left of right of along axis for purposes of synchronizing optimal kernel;
    
    
    vector<TH1F *> vecPullHists;
    //vector of pull distributions for the current region being checked
    //used in smoothing
    
    void DefaultVarVals();
    
    TH1F * GetPullDistribution(TH2F *, TH2F *, TString, float, bool);
    void MakePullVec(TH2F *, vector<TH2F *> *, float, bool);
    void SmoothWhichOptimalKernel(TH2F *, vector< vector<int> > *);
    void SetParamsFromCommandLine(int, char **);
    
    void DeleteCurrPullHistsVecs();
    
    int ComparePulls(bool);
    
};
#endif