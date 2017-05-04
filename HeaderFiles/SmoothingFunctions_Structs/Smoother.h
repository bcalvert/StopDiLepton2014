#ifndef Smoother_h
#define Smoother_h
struct Smoother {
    bool doLogNormal;
    //Whether to smooth the logarithmic values
    
    float errMultiplier;
    // How much to multiply original bin content's error by when generating pull distribution
    bool doCheckMaxRelChange;
    float maxRelErrChangeThresh, maxRelChange;
    // How much each bin is allowed to change by relative to the original bin content's error

    TString nameScan;
    
    int minSigX, maxSigX;
    int minSigY, maxSigY;
    int numSigX, numSigY;
    //how far to scan in the sigma X, sigmaY space of kernel widths
    //with how many points
    
    /*******Member variables used in smoothing*******/
    unsigned int numKernels;
    
    vector<kernelParams> vecKPs;
    //vector of kernel parameters (sigmaX, sigmaY) for smoothing
    
    vector<TF2 *> vecKernels;
    //Vector of the TF2 kernels used for smoothing
    
    PullChecker smoothingPC;
    //The PullChecker struct used for evaluating the smoothing
    /*******Member variables used in smoothing*******/

    /*******Member variables of the smoothing output*******/
    vector<TH2F *> vecOptimalKernelWidths;
    //Structure that contains information on which kernel widths were optimal
    
    vector< vector<int> > vecVecIndex_OptKernel;
    //A 2D array of which kernel yielded the optimal smoothing
    
    vector<TH2F *> vecSmoothedHists;
    //Vector of the output smoothed histograms
    /*******Member variables of the smoothing output*******/
    
    void DefaultVarVals();
    void DefaultVarVals_EfficiencySmoothing();
    void SetKernelParams();
    void SetParamsFromCommandLine(int, char **);
    
    void SetVecSizes();
    void DeleteVecs();
    
    TH2F * GenerateOptimalSmoothedMap(TH2F *, bool);
    
    void SetSmoothMapFromOptimalKernel(TH2F *, bool);
    
    void SetName();
    
};
#endif
