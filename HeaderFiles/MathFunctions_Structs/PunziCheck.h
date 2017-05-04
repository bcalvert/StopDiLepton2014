#ifndef PunziCheck_h
#define PunziCheck_h

/*
float GetPunziParam(float effSig, float yieldBkg, float nSigmaA, float nSigmaB, bool doVerb) {
    //equation 7 of http://www-cdf.fnal.gov/physics/statistics/notes/punzi-optimal-search.ps
    
    if (doVerb) {
        cout << "nSigmaA " << nSigmaA << endl;
        cout << "nSigmaB " << nSigmaB << endl;
        cout << "yieldBkg " << yieldBkg << endl;
        cout << "effSig " << effSig << endl;
    }
    
    float sqrtBkg = TMath::Sqrt(yieldBkg);
    
    float sigmaMin_Part1 = pow(nSigmaA, 2) / 8 + (9 * pow(nSigmaB, 2))/13 + nSigmaA * sqrtBkg;
    float sigmaMin_Part2 = (nSigmaB / 2) * TMath::Sqrt(pow(nSigmaB, 2) + 4 * nSigmaA * sqrtBkg + 4 * yieldBkg);
    
    //    float paramPunzi = effSig / (TMath::Sqrt(yieldBkg) + (nSigma/2));
    float paramPunzi = effSig / (sigmaMin_Part1 + sigmaMin_Part2);
    if (doVerb) {
        cout << "Punzi Parameter " << paramPunzi << endl;
    }
    return paramPunzi;
}
*/

struct BinPunziCheck {
    int globalBin;
    
    float nSigmaA, nSigmaB;
    
    float sigEff;
    float backYield;
    float punziParam;
    void SetPunziParam(bool doVerb = false) {
        punziParam = GetPunziParam(sigEff, backYield, nSigmaA, nSigmaB, doVerb);
    }
    void SetValues(int whichGB, float inSigmaA, float inSigmaB, float inSigEff, float inBkgYield, bool doVerb = false) {
        globalBin = whichGB;
        nSigmaA = inSigmaA;
        nSigmaB = inSigmaB;
        backYield = inBkgYield;
        sigEff = inSigEff;
        SetPunziParam(doVerb);
    }
    void PrintVals() {
        cout << "sigEff " << sigEff << endl;
        cout << "backYield " << backYield << endl;
        cout << "punziParam " << punziParam << endl;
    }
};
#endif