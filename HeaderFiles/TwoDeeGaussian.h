#include "TF2.h"
#include "TH2.h"
#include "TMath.h"
#include "TAxis.h"
#include "TString.h"
#include <vector>
#include <map>

#define numSigmaToScan 4

typedef pair<float, float> kernelParams;
typedef pair<TH2F *, TH2F *> optimalKernelVals;

typedef pair<int, int> massSUSY;
typedef map<massSUSY, TH2F *> map_SUSYtoKernel;
/*
Double_t g2(Double_t *x, Double_t *par) {
    Double_t r1 = Double_t((x[0]-par[1])/par[2]);
    Double_t r2 = Double_t((x[1]-par[3])/par[4]);
    return par[0]*TMath::Exp(-0.5*(r1*r1+r2*r2));
}
Double_t fun2(Double_t *x, Double_t *par) {
    Double_t *p1 = &par[0];
    Double_t *p2 = &par[5];
    Double_t *p3 = &par[10];
    Double_t result = g2(x,p1) + g2(x,p2) + g2(x,p3);
    return result;
}

TF2 * TwoDeeGaussianKernel(kernelParams * inKParams) {
    float sigX = inKParams->first;
    float sigY = inKParams->second;
    
    float xLB = - numSigmaToScan * sigX;
    float xUB = + numSigmaToScan * sigX;
    
    float yLB = - numSigmaToScan * sigY;
    float yUB = + numSigmaToScan * sigY;
    
    TString nameFunc = "f_2DGaussKernel_SigmaX";
    nameFunc += sigX;
    nameFunc += "_SigmaY";
    nameFunc += sigY;
    
    Double_t f2params[5] = {1, 0, sigX, 0, sigY};
    TF2 *f2 = new TF2(nameFunc,g2, xLB, xUB, yLB, yUB, 5);
    f2->SetParameters(f2params);
    f2->SetNpx(((int)(xUB - xLB))/sigX);
    f2->SetNpy(((int)(yUB - yLB))/sigY);
    
    return f2;
}

vector<float> KernelAverageValue(float xVal, float yVal, TH2F * inputHist, TF2 * inputKernel, bool doLogNormal, float thresh = 0.00, int doVerbosity = false) {
    float currSumWeightedVals = 0., currSumWeights = 0.;
    float currSumWeightedErrs = 0., currSumWeightsSq = 0.;
    vector<float> vecOutVals(0);
    
    //Set up histogram data
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    int currBinX = FindBinInHistRange(vecInputHistAxes[0], xVal);
    int currBinY = FindBinInHistRange(vecInputHistAxes[1], yVal);
    

    //scan up to 4 sigma away from gaussian center
    int paramIndex_sigX = 2;
    int paramIndex_sigY = 4;
    
    int maxBinScanX = (int) ((numSigmaToScan * inputKernel->GetParameter(paramIndex_sigX))/vecInputHistAxisBinWidths[0] + 1 / 2);
    int maxBinScanY = (int) ((numSigmaToScan * inputKernel->GetParameter(paramIndex_sigY))/vecInputHistAxisBinWidths[1] + 1 / 2);
    
    float currDeltX, currDeltY;
    float currKernelWeight, currBinContent, currBinError;
    //iterate over all relevant bins in original histogram
    for (int iBinX = TMath::Max(1, currBinX - maxBinScanX); iBinX <= TMath::Min(vecInputHistAxisNBins[0], currBinX + maxBinScanX); ++iBinX) {
        currDeltX = xVal - vecInputHistAxes[0]->GetBinCenter(iBinX);
        for (int iBinY = TMath::Max(1, currBinY - maxBinScanY); iBinY <= TMath::Min(vecInputHistAxisNBins[1], currBinY + maxBinScanY); ++iBinY) {
            currDeltY = yVal - vecInputHistAxes[1]->GetBinCenter(iBinY);
            
            currKernelWeight = inputKernel->Eval(currDeltX, currDeltY);
            currBinContent = inputHist->GetBinContent(iBinX, iBinY);
            currBinError = inputHist->GetBinError(iBinX, iBinY);
            
            if (doVerbosity) {
                cout << "xVal:yVal" << xVal << ":" << yVal << endl;
                cout << "currDeltX:currDeltY" << currDeltX << ":" << currDeltY << endl;
                cout << "currKernelWeight " << currKernelWeight << endl;
                cout << "currBinContent " << currBinContent << endl;
                cout << "currBinError " << currBinError << endl;
            }
            
            if (currBinContent <= thresh) continue;
            if (doLogNormal) {
                currBinContent = TMath::Log(currBinContent);
                if (doVerbosity) {
                    cout << "currBinContent post log " << currBinContent << endl;
                }
            }
            currSumWeightedVals += currKernelWeight * currBinContent;
            currSumWeightedErrs += TMath::Power(currKernelWeight * currBinError, 2);
            currSumWeights += currKernelWeight;
            currSumWeightsSq += TMath::Power(currKernelWeight, 2);
        }
    }
    vecOutVals.push_back(currSumWeightedVals);
    vecOutVals.push_back(currSumWeightedErrs);
    vecOutVals.push_back(currSumWeights);
    vecOutVals.push_back(currSumWeightsSq);
    return vecOutVals;
}

TH2F * KernelAveragedHistogram(TH2F * inputHist, TF2 * inputKernel, bool doLogNormal, TString addName = "_2DGaussSmoothed", float thresh = 0.0001, int doVerbosity = 0) {
    //thresh represents the threshold value that a bin has to have in order to contribute to the average
    
    TH2F * outHist = (TH2F *) inputHist->Clone(inputHist->GetName() + addName);
    
    //Set up histogram data
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    float currSumWeightedVals, currSumWeights;
    float currSumWeightedErrs, currSumWeightsSq;
    
    float currXVal, currYVal, currBinContent;
    
    cout << "I'm assuming that the scan stops at mStop = mLSP " << endl;
    
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        for (int iBinY = 1; iBinY <= TMath::Min(iBinX, vecInputHistAxisNBins[1]); ++iBinY) {
            currXVal = vecInputHistAxes[0]->GetBinCenter(iBinX);
            currYVal = vecInputHistAxes[1]->GetBinCenter(iBinY);
            
            currBinContent = inputHist->GetBinContent(iBinX, iBinY);
            
            vector<float> vecVals = KernelAverageValue(currXVal, currYVal, inputHist, inputKernel, doLogNormal, thresh, doVerbosity);
            currSumWeightedVals = vecVals[0];
            currSumWeightedErrs = vecVals[1];
            currSumWeights = vecVals[2];
            currSumWeightsSq = vecVals[3];
            if (currSumWeights < 1E-3) continue;
            currSumWeightedVals /= currSumWeights;
            currSumWeightedErrs /= currSumWeightsSq;
            
            if (doLogNormal) {
                if (doVerbosity) {
                    cout << "currSumWeightedVals " << currSumWeightedVals << endl;
                }
                currSumWeightedVals = TMath::Exp(currSumWeightedVals);
                if (doVerbosity) {
                    cout << "currSumWeightedVals Post log " << currSumWeightedVals << endl;
                }
            }
            currSumWeightedErrs = TMath::Sqrt(currSumWeightedErrs);
            if (doVerbosity) {
                cout << "iBinX " << iBinX << endl;
                cout << "iBinY " << iBinY << endl;
                cout << "currBinContent " << currBinContent << endl;
                cout << "currSumWeights " << currSumWeights << endl;
                cout << "currSumWeightedVals " << currSumWeightedVals << endl;
                cout << "currSumWeightedErrs " << currSumWeightedErrs << endl;
            }
            outHist->SetBinContent(iBinX, iBinY, currSumWeightedVals);
            outHist->SetBinError(iBinX, iBinY, currSumWeightedErrs);
        }
    }
    return outHist;
}

*/


TH2F * TwoDeeGaussianKernel(float sigX, float sigY) {
    /*
    float xLB = -62.5;
    float xUB = 62.5;
    float yLB = -62.5;
    float yUB = 62.5;
    */

    
    float xLB = -112.5;
    float xUB = 112.5;
    float yLB = -112.5;
    float yUB = 112.5;
    

    Double_t f2params[5] = {1, 0, sigX, 0, sigY};
    TF2 *f2 = new TF2("f2",g2, xLB, xUB, yLB, yUB, 5);
    f2->SetParameters(f2params);
    f2->SetNpx(((int)(xUB - xLB))/25);
    f2->SetNpy(((int)(yUB - yLB))/25);
    TH2F * outKernel = (TH2F *) f2->GetHistogram();
    TString outName = "GaussKernel_2D_SigmaX";
    outName += sigX;
    outName += "_SigmaY";
    outName += sigY;
    outKernel->SetName(outName);
    outKernel->GetXaxis()->SetTitle("#Delta x");
    outKernel->GetYaxis()->SetTitle("#Delta y");
    return outKernel;
}

vector<float> KernelAverageValue(float xVal, float yVal, TH2F * inputHist, TH2F * inputKernel, bool doLogNormal, float thresh = 0.00, int doVerbosity = false) {

    float currSumWeightedVals = 0., currSumWeights = 0.;
    float currSumWeightedErrs = 0., currSumWeightsSq = 0.;
    vector<float> vecOutVals(0);
    
    int currBinX, currBinY;
    float currBinContent, currBinError;
    float currDeltX, currDeltY;
    float currKernelWeight;
    
    int nBinsX = inputHist->GetNbinsX();
    int nBinsY = inputHist->GetNbinsY();
    int nKernelBinsX = inputKernel->GetNbinsX();
    int nKernelBinsY = inputKernel->GetNbinsY();
    
    if (doVerbosity) {
        cout << "xVal " << xVal << endl;
        cout << "yVal " << yVal << endl;
    }
    
    for (int iKernelBinX = 1; iKernelBinX <= nKernelBinsX; ++iKernelBinX) {
        currDeltX = inputKernel->GetXaxis()->GetBinCenter(iKernelBinX);
        currBinX = inputHist->GetXaxis()->FindBin(xVal + currDeltX);
        if (currBinX < 1 || currBinX > nBinsX) continue;
        for (int iKernelBinY = 1; iKernelBinY <= nKernelBinsY; ++iKernelBinY) {
            currDeltY = inputKernel->GetYaxis()->GetBinCenter(iKernelBinY);
            currBinY = inputHist->GetYaxis()->FindBin(yVal + currDeltY);
            if (doVerbosity > 1) {
                cout << "xVal " << xVal << endl;
                cout << "yVal " << yVal << endl;
                cout << "iKernelBinX " << iKernelBinX << endl;
                cout << "currDeltX " << currDeltX << endl;
                cout << "currBinX " << currBinX << endl;
                cout << "iKernelBinY " << iKernelBinY << endl;
                cout << "currDeltY " << currDeltY << endl;
                cout << "currBinY " << currBinY << endl;
            }
            if (currBinY < 1 || currBinY > nBinsY) continue;
            currKernelWeight = inputKernel->GetBinContent(iKernelBinX, iKernelBinY);

            currBinContent = inputHist->GetBinContent(currBinX, currBinY);
            currBinError = inputHist->GetBinError(currBinX, currBinY);
            

            
            if (doVerbosity) {
                cout << "iKernelBinX " << iKernelBinX << endl;
                cout << "iKernelBinY " << iKernelBinY << endl;
                cout << "currKernelWeight " << currKernelWeight << endl;
                cout << "currBinContent " << currBinContent << endl;
                cout << "currBinError " << currBinError << endl;
            }
            
            if (currBinContent <= thresh) continue;
            if (doLogNormal) {
                currBinContent = TMath::Log(currBinContent);
            }
            /*

                cout << "currBinContent " << currBinContent << endl;
                cout << "currBinContent post log " << currBinContent << endl;
                currBinError = TMath::Log(currBinError); //.........ugh
            }
            */
            
            currSumWeightedVals += currKernelWeight * currBinContent;
            currSumWeightedErrs += TMath::Power(currKernelWeight * currBinError, 2);
            currSumWeights += currKernelWeight;
            currSumWeightsSq += TMath::Power(currKernelWeight, 2);
        }
    }
    vecOutVals.push_back(currSumWeightedVals);
    vecOutVals.push_back(currSumWeightedErrs);
    vecOutVals.push_back(currSumWeights);
    vecOutVals.push_back(currSumWeightsSq);
    return vecOutVals;
}

TH2F * KernelAveragedHistogram(TH2F * inputHist, TH2F * inputKernel, bool doLogNormal, TString addName = "_2DGaussSmoothed", float thresh = 0.0001, int doVerbosity = 0) {
    //thresh represents the threshold value that a bin has to have in order to contribute to the average
    
    TH2F * outHist = (TH2F *) inputHist->Clone(inputHist->GetName() + addName);
    
    float binWidthInHist = inputHist->GetXaxis()->GetBinWidth(1);
    float binWidthKernel = inputKernel->GetXaxis()->GetBinWidth(1);
    
    if (TMath::Abs(binWidthInHist - binWidthKernel) > 1E-3) {
        cout << "bins for kernel and input hist aren't same width!" << endl;
        return NULL;
    }
    
    int nBinsX = outHist->GetNbinsX();
    int nBinsY = outHist->GetNbinsY();
    
    float currSumWeightedVals, currSumWeights;
    float currSumWeightedErrs, currSumWeightsSq;
    
    float currXVal, currYVal;
    
    float currBinContent;

    int maxBinX = nBinsX;
    int maxBinY = nBinsY;
    
    for (int iBinX = 1; iBinX <= maxBinX; ++iBinX) {
        for (int iBinY = 1; iBinY <= maxBinY; ++iBinY) {
            currXVal = inputHist->GetXaxis()->GetBinCenter(iBinX);
            currYVal = inputHist->GetYaxis()->GetBinCenter(iBinY);
            
            currBinContent = inputHist->GetBinContent(iBinX, iBinY);
            if (currBinContent < 1E-6) continue;
            
            vector<float> vecVals = KernelAverageValue(currXVal, currYVal, inputHist, inputKernel, doLogNormal, thresh, doVerbosity);
            currSumWeightedVals = vecVals[0];
            currSumWeightedErrs = vecVals[1];
            currSumWeights = vecVals[2];
            currSumWeightsSq = vecVals[3];
            if (currSumWeights < 1E-3) continue;
            currSumWeightedVals /= currSumWeights;
            currSumWeightedErrs /= currSumWeightsSq;
            /*
            if (doLogNormal) {
                cout << "currSumWeightedVals " << currSumWeightedVals << endl;
                currSumWeightedVals = TMath::Exp(currSumWeightedVals);
                cout << "currSumWeightedVals " << currSumWeightedVals << endl;
                currSumWeightedErrs = TMath::Exp(currSumWeightedErrs);
            }
            */
            currSumWeightedErrs = TMath::Sqrt(currSumWeightedErrs);
            if (doVerbosity) {
                cout << "iBinX " << iBinX << endl;
                cout << "iBinY " << iBinY << endl;
                cout << "currBinContent " << currBinContent << endl;
                cout << "currSumWeights " << currSumWeights << endl;
                cout << "currSumWeightedVals " << currSumWeightedVals << endl;
                cout << "currSumWeightedErrs " << currSumWeightedErrs << endl;
            }
            outHist->SetBinContent(iBinX, iBinY, currSumWeightedVals);
            outHist->SetBinError(iBinX, iBinY, currSumWeightedErrs);
        }
    }
    return outHist;
}









