#include "Math/QuantFuncMathCore.h"
//Collection of functions used for basic mathematical calculations
//that are used in many stages of the analysis

#include <vector>

using namespace std;

typedef pair<float, float> valPlusError;

void CorrectSignificance(TH2F * inputHist, TF1 * inputTF1, float fudgeFactorEuler = 0.25) {
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    float currSignif;
    float currGlobPValue;
    float currEstSignif;
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        for (int iBinY = 1; iBinY <= vecInputHistAxisNBins[0]; ++iBinY) {
            currSignif = inputHist->GetBinContent(iBinX, iBinY);
            currGlobPValue = inputTF1->Eval(currSignif);
            currGlobPValue *= fudgeFactorEuler;
            if (currGlobPValue > 0.5) currGlobPValue = 0.5;
            currEstSignif = ROOT::Math::normal_quantile_c(currGlobPValue,1);
            inputHist->SetBinContent(iBinX, iBinY, currEstSignif);
        }
    }
}

valPlusError VarianceWeightedMean(vector<valPlusError> * inVecVPE, bool doVerb = false) {
    //Formula for combining measurements x_{i} \pm \sigma_{i}
    //http://en.wikipedia.org/wiki/Weighted_arithmetic_mean#Dealing_with_variance
    
    float numSum = 0.0, denomSum = 0.0;
    float currX, currSig, currWeight;
    
    for (unsigned int iX = 0; iX < inVecVPE->size(); ++iX) {
        currX = inVecVPE->at(iX).first;
        currSig = inVecVPE->at(iX).second;
        if (currSig < 1E-12) {
            if (currX > 1E-12) {
                cout << "something weird with inputs!" << endl;
                cout << "iX " << iX << endl;
                cout << "currX " << currX << endl;
                cout << "currSig " << currSig << endl;

            }
            currSig = 1.;
            currX = 0.;
        }
        currWeight = 1./TMath::Power(currSig, 2);
        
        if (doVerb) {
            cout << "iX " << iX << endl;
            cout << "currX " << currX << endl;
            cout << "currSig " << currSig << endl;
            cout << "currWeight " << currWeight << endl;
        }
        
        numSum += currX * currWeight;
        denomSum += currWeight;
    }
    
    if (doVerb) {
        cout << "numSum " << numSum << endl;
        cout << "denomSum " << denomSum << endl;
    }
    
    valPlusError outVal(numSum/denomSum, 1./TMath::Sqrt(denomSum));
    return outVal;
}

float GetError(vector<float> * inVecVarTerms, vector<float> * inVecCovarTerms) {
  //function to calculate a total approximate error for a function of random variables, assuming you have an input vector, inVecVarTerms,
  //containing the variance terms and another vector, inVecCovarTerms, containing the covariance terms
  //the input inVecVarTerms is assumed to have the un-squared variance terms, so they get squared
  //while the input inVecCovarTerms is assumed to have full terms, which are not squared so that relative signs can come into the picture
  float outErr = 0.0;
  for (unsigned int iVarVal = 0; iVarVal < inVecVarTerms->size(); ++iVarVal) {
    outErr += pow(inVecVarTerms->at(iVarVal), 2);
  }
  for (unsigned int iCoVarVal = 0; iCoVarVal < inVecCovarTerms->size(); ++iCoVarVal) {
    outErr += inVecCovarTerms->at(iCoVarVal);
  }
  return TMath::Sqrt(outErr);
}

valPlusError GetRatio(valPlusError * inVPENum, valPlusError * inVPEDenom, bool doConsistency = false, valPlusError * inVPEUncertUp = 0, valPlusError * inVPEUncertDown = 0) {
    //Function that, at a basic level, returns the ratio of
    //the input values (first entries in the VPE pair) -- simple...I know
    //However, you also have the opportunity to do "consistency checks"
    //That is, testing the assumption of how consistent each point is with 1.0
    //Included as well with this option then is also including uncertainties that
    //are asymmetric
    //When doing the consistency check with asymmetric uncertainties, the
    //underlying assumption is that R_true = 1.0
    //thus, for the purpose of doing the consistency check;
    //if the ratio is < 1.0, the sigma ratio used will be the
    //up shift (as that would bring the ratio closer to one);
    //if the ratio is > 1.0, the sigma ratio used will be the
    //down shift (for similar reasons as down shift)
    
    float thresh = 1E-9; //threshold for which to return null results
    if (inVPEDenom->first < thresh) {
        if (inVPENum->first < thresh) {
            return valPlusError(-1E3, 1.);
        }
        else {
            return valPlusError(5E2, inVPENum->second);
        }
    }
    float ratio = inVPENum->first / inVPEDenom->first;
    float divSigma = 1.0;
    
    vector<float> vecVarTerms(0);
    vector<float> vecCoVarTerms(0);
    
    float varTerm_Num = inVPENum->second / inVPEDenom->first;
    float varTerm_Denom = ratio * inVPEDenom->second / inVPEDenom->first;
    vecVarTerms.push_back(varTerm_Num);
    vecVarTerms.push_back(varTerm_Denom);
    divSigma = GetError(&vecVarTerms, &vecCoVarTerms);
    
    if (doConsistency) {
        if (inVPEUncertUp && inVPEUncertDown) {
            divSigma = ratio > 1.0 ? inVPEUncertUp->second : inVPEUncertDown->second;
        }
        ratio = (ratio - 1.0) / divSigma;
    }
    
    valPlusError outVPE = valPlusError(TMath::Abs(ratio), divSigma);
    
    return outVPE;
}