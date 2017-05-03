#include <cmath>
#include "TCanvas.h"
#include "TH1.h"
#include "TH1F.h"
#include "TString.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TLorentzVector.h"
#include "./HistogramStyleFunctions.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TAxis.h"

#include <vector>
using namespace std;
inline void DebugStatement(int inputIntToCheck, vector<int> * inputAllowedValsForInt, TString intVarName, TString nameFunction) {
    bool intFails = true;
    for (unsigned int iCheck = 0; iCheck < inputAllowedValsForInt->size(); ++iCheck) {
        if (inputIntToCheck == inputAllowedValsForInt->at(iCheck)) {
            intFails = false;
        }
    }
    if (intFails) {
        cout << "in function: " << nameFunction << " the integer with variable name " << intVarName;
        cout << " failed a check because its value " << inputIntToCheck << " was not one of the allowed values" << endl;
    }
}

inline bool EqualBinWidths(TH1F * inputHist) {
    bool hasEqualBinWidths = true;
    float currBinWidth, nextBinWidth;
    for (int iBinX = 1; iBinX < inputHist->GetNbinsX(); ++iBinX) {
        currBinWidth = inputHist->GetXaxis()->GetBinWidth(iBinX);
        nextBinWidth = inputHist->GetXaxis()->GetBinWidth(iBinX + 1);
        if (fabs(currBinWidth - nextBinWidth) > 1E-3) {
            hasEqualBinWidths = false;
        }
    }
    return hasEqualBinWidths;
}
inline float GetPercentile(TH1F * inputHist, float valPercentile, bool doVerbosity = 0) {
    int nBinsX = inputHist->GetNbinsX();
    float totInt = inputHist->Integral();
    float currCDFVal = 0.0;
    TAxis * xAxis = inputHist->GetXaxis();
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        currCDFVal += (inputHist->GetBinContent(iBinX))/totInt;
        if (doVerbosity) {
            cout << "iBinX " << iBinX << endl;
            cout << "currCDFVal " << currCDFVal << endl;
        }
        if (currCDFVal > valPercentile) {
            //current bin is the threshold bin
            return xAxis->GetBinCenter(iBinX);
        }
    }
    return 1.0;
}
inline float GetStatValue(TH1F * inputHist, int whichPercentile, bool doVerbosity = 0) {
    //whichPercentile: 0 -- use mean, otherwise, look at array of definitions
    float arrPercentile[3] = {0.0, 0.5, 0.9};

    float returnVal = 0.0;
    if (!whichPercentile) {
        if (doVerbosity) {
            cout << "inputHist mean " << inputHist->GetMean() << endl;
        }
        returnVal = inputHist->GetMean();
    }
    else {
        returnVal = GetPercentile(inputHist, arrPercentile[whichPercentile], doVerbosity);
    }
    return returnVal;
}

inline float CalculateRecoil(float inVecMag, float inVecPhi, float axisPhi, int whichComponent) {
    //whichComponent = 0 for parallel
    //whichComponent = 1 for perpendicular
    if (whichComponent == 0) {
        return inVecMag * TMath::Cos(inVecPhi - axisPhi);
    }
    else if (whichComponent == 1) {
        return inVecMag * TMath::Sin(inVecPhi - axisPhi);
    }
    else {
        cout << "whichComponent needs to be 0 or 1; it is " << whichComponent << endl;
    }
    return -9999;
}

inline void PrintHistogram(TH1 * inputHist, bool doUnderflow, bool doOverflow, TH1 * referenceHist = NULL) {

    int nBinsX = inputHist->GetNbinsX();
    int nBinsY = inputHist->GetNbinsY();
    int nBinsZ = inputHist->GetNbinsZ();
    
    if (referenceHist) {
        int nBinsXRef = referenceHist->GetNbinsX();
        int nBinsYRef = referenceHist->GetNbinsY();
        int nBinsZRef = referenceHist->GetNbinsZ();
        
        if (nBinsX != nBinsXRef || nBinsY != nBinsYRef || nBinsZ != nBinsZRef) {
            cout << "reference hist and input hist don't match in number of bins!" << endl;
            return;
        }
    }
    
    TAxis * xAxis = inputHist->GetXaxis();
    TAxis * yAxis = inputHist->GetYaxis();
    TAxis * zAxis = inputHist->GetZaxis();
    
    int yLB = nBinsY > 1 ? !doUnderflow : 1;
    int yUB = nBinsY > 1 ? nBinsY + doOverflow : 1;
    
    int zLB = nBinsZ > 1 ? !doUnderflow : 1;
    int zUB = nBinsZ > 1 ? nBinsZ + doOverflow : 1;
    
    float currBinContent, currBinErr;
    float currBinContentRef;
    
    float currBinXLowEdge, currBinXUpEdge;
    float currBinYLowEdge, currBinYUpEdge;
    float currBinZLowEdge, currBinZUpEdge;
    
    cout << "currHistName " << inputHist->GetName();
    for (int iBinX = !doUnderflow; iBinX <= nBinsX + doOverflow; ++iBinX) {
        currBinXLowEdge = xAxis->GetBinLowEdge(iBinX);
        currBinXUpEdge = xAxis->GetBinUpEdge(iBinX);
        for (int iBinY = yLB; iBinY <= yUB; ++iBinY) {
            currBinYLowEdge = yAxis->GetBinLowEdge(iBinY);
            currBinYUpEdge = yAxis->GetBinUpEdge(iBinY);
            for (int iBinZ = zLB; iBinZ <= zUB; ++iBinZ) {
                currBinZLowEdge = zAxis->GetBinLowEdge(iBinZ);
                currBinZUpEdge = zAxis->GetBinUpEdge(iBinZ);
                currBinContent = inputHist->GetBinContent(iBinX, iBinY, iBinZ);
                currBinErr = inputHist->GetBinError(iBinX, iBinY, iBinZ);
                
                cout << "{iBinX:iBinY:iBinZ} = {" << iBinX << ":" << iBinY << ":" << iBinZ <<"}" << endl;
                cout << "{xLB:xUB} = {" << currBinXLowEdge << ":" << currBinXUpEdge << "}" << endl;
                cout << "{yLB:yUB} = {" << currBinYLowEdge << ":" << currBinYUpEdge << "}" << endl;
                cout << "{zLB:zUB} = {" << currBinZLowEdge << ":" << currBinZUpEdge << "}" << endl;
                if (referenceHist) {
                    currBinContentRef = referenceHist->GetBinContent(iBinX, iBinY, iBinZ);
                    cout << "currRelPercentVal " << 100 * ((currBinContent / currBinContentRef) - 1.0) << endl;
                }
                else {
                    cout << "currVal +- err " << currBinContent << " +- " << currBinErr << endl;
                }
            }
        }
    }
}

inline void SetMCStatSystHist(TH1 * inputHist, int dirSyst = 0, bool doVerbosity = 0) {
    //Used for turning a clone of a central value histogram into a central value +- statistical error
    if (dirSyst == 0) {
        cout << "about to make a syst variation but no syst direction!" << endl;
    }
    int nBinsX = inputHist->GetNbinsX();
    int nBinsY = inputHist->GetNbinsY();
    int nBinsZ = inputHist->GetNbinsZ();
    float currBinContent, currBinErr;
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            for (int iBinZ = 1; iBinZ <= nBinsZ; ++iBinZ) {
                currBinContent = inputHist->GetBinContent(iBinX, iBinY, iBinZ);
                currBinErr = inputHist->GetBinError(iBinX, iBinY, iBinZ);
                inputHist->SetBinContent(iBinX, iBinY, iBinZ, currBinContent + (dirSyst * currBinErr));
                if (doVerbosity) {
                    cout << "currBinContent " << currBinContent << endl;
                    cout << "currBinErr " << currBinErr << endl;
                    cout << "dirSyst " << dirSyst << endl;
                    cout << "resulting bin content " << currBinContent + (dirSyst * currBinErr) << endl;
                }
            }
        }
    }
}

inline void SetHistStatErrFromMCStatSyst(TH1 * inputHistCV, TH1 * inputHistMCStatUp, bool doVerbosity = 0) {
    //Used for setting the statistical error of a central value histogram assuming one has a MCStat Shifted Up histogram a la the above function, SetMCStatSystHist
    int nBinsX = inputHistCV->GetNbinsX();
    int nBinsY = inputHistCV->GetNbinsY();
    int nBinsZ = inputHistCV->GetNbinsZ();
    
    int nBinsX_MCStatUp = inputHistMCStatUp->GetNbinsX();
    int nBinsY_MCStatUp = inputHistMCStatUp->GetNbinsY();
    int nBinsZ_MCStatUp = inputHistMCStatUp->GetNbinsZ();
    
    if (nBinsX != nBinsX_MCStatUp || nBinsY != nBinsY_MCStatUp || nBinsZ != nBinsZ_MCStatUp) {
        cout << "bins don't match! " << endl;
        cout << "nBinsX:nBinsX_MCStatUp " << nBinsX << ":" << nBinsX_MCStatUp << endl;
        cout << "nBinsY:nBinsY_MCStatUp " << nBinsY << ":" << nBinsY_MCStatUp << endl;
        cout << "nBinsZ:nBinsZ_MCStatUp " << nBinsZ << ":" << nBinsZ_MCStatUp << endl;
    }
    
    float currBinContent, currBinErrToSet;
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            for (int iBinZ = 1; iBinZ <= nBinsZ; ++iBinZ) {
                currBinContent = inputHistCV->GetBinContent(iBinX, iBinY, iBinZ);
                currBinErrToSet = inputHistMCStatUp->GetBinContent(iBinX, iBinY, iBinZ);
                if (currBinErrToSet > 2 * currBinContent) {
                    if (doVerbosity) {
                        cout << "going to correct currBinErrToSet so that it's not too large! This shouldn't affect the limit " << endl;
                    }
                    currBinErrToSet = 2 * currBinContent - 1E-4;
                }
                inputHistCV->SetBinError(iBinX, iBinY, iBinZ, currBinErrToSet - currBinContent);
                if (doVerbosity) {
                    cout << "currBinContent " << currBinContent << endl;
                    cout << "currBinErrToSet " << currBinErrToSet << endl;
                    cout << "resulting bin error " << inputHistCV->GetBinError(iBinX, iBinY, iBinZ) << endl;
                }
            }
        }
    }
}

inline void FixMCStatSystDownHistFromCVStatErr(TH1 * inputHistCV, TH1 * inputHistMCStatDown, bool doVerbosity = 0) {
    //Used for setting the statistical error of a central value histogram assuming one has a MCStat Shifted Up histogram a la the above function, SetMCStatSystHist
    int nBinsX = inputHistCV->GetNbinsX();
    int nBinsY = inputHistCV->GetNbinsY();
    int nBinsZ = inputHistCV->GetNbinsZ();
    
    int nBinsX_MCStatDown = inputHistMCStatDown->GetNbinsX();
    int nBinsY_MCStatDown = inputHistMCStatDown->GetNbinsY();
    int nBinsZ_MCStatDown = inputHistMCStatDown->GetNbinsZ();
    
    if (nBinsX != nBinsX_MCStatDown || nBinsY != nBinsY_MCStatDown || nBinsZ != nBinsZ_MCStatDown) {
        cout << "bins don't match! " << endl;
        cout << "nBinsX:nBinsX_MCStatDown " << nBinsX << ":" << nBinsX_MCStatDown << endl;
        cout << "nBinsY:nBinsY_MCStatDown " << nBinsY << ":" << nBinsY_MCStatDown << endl;
        cout << "nBinsZ:nBinsZ_MCStatDown " << nBinsZ << ":" << nBinsZ_MCStatDown << endl;
    }
    
    float currCVBinErr, currCVBinContent, currMCStatDownBinContent;
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            for (int iBinZ = 1; iBinZ <= nBinsZ; ++iBinZ) {
                currMCStatDownBinContent = inputHistMCStatDown->GetBinContent(iBinX, iBinY, iBinZ);
                currCVBinErr = inputHistCV->GetBinError(iBinX, iBinY, iBinZ);
                currCVBinContent = inputHistCV->GetBinContent(iBinX, iBinY, iBinZ);
                if (currCVBinContent - currCVBinErr > currMCStatDownBinContent) {
                    if (doVerbosity) {
                        cout << "going to correct currMCStatDownBinContent so that it's not spuriously 0! This shouldn't greatly affect the limit " << endl;
                        cout << "currCVBinContent - currCVBinErr " << currCVBinContent - currCVBinErr << endl;
                        cout << "currMCStatDownBinContent " << currMCStatDownBinContent << endl;
                    }
                    currMCStatDownBinContent = currCVBinContent - currCVBinErr;
                }
                inputHistMCStatDown->SetBinContent(iBinX, iBinY, iBinZ, currMCStatDownBinContent);
                if (doVerbosity) {
                    cout << "currCVBinContent " << currCVBinContent << endl;
                    cout << "currCVBinErr " << currCVBinErr << endl;
                    cout << "currMCStatDownBinContent " << currMCStatDownBinContent << endl;
                }
            }
        }
    }
}


inline float nDigits(float number, int digits) {
    return round(number * std::pow(10.,digits)) / std::pow(10.,digits);
}
inline float TruncateNum(float inNum, float LB, float UB, float safetyFactor) {
    float outNum;
    float average = (LB + UB) / 2;
    if (inNum > LB && inNum < UB) {
        if (inNum > average) {
            outNum = UB + safetyFactor;
        }
        else {
            outNum = LB - safetyFactor;
        }
    }
    else {
        outNum = inNum;
    }
    return outNum;
}
/*
inline void PoissonError(float binCount, float &errUp, float &errDown) {
    //taken from discussion on https://twiki.cern.ch/twiki/bin/viewauth/CMS/PoissonErrorBars
    const double alpha = 1 - 0.6827;
    double L =  (N==0) ? 0  : (ROOT::Math::gamma_quantile(alpha/2,N,1.));
    double U =  ROOT::Math::gamma_quantile_c(alpha/2,N+1,1) ;
}
*/
inline void relErrMaker( TH1F * &inputErrHist) {
    int NBins = inputErrHist->GetNbinsX();
    float binRelErr;
    for (int i = 1; i < NBins+1; ++i) {
        binRelErr = inputErrHist->GetBinError(i)/inputErrHist->GetBinContent(i);
        inputErrHist->SetBinError(i, binRelErr);
    }
}
inline void relErrSet(TH1F * inputRelErrHist, TH1F * inputErrHist) {
    int NBins = inputErrHist->GetNbinsX();
    for (int i = 1; i<NBins+1; ++i) {
        inputErrHist->SetBinError(i, inputErrHist->GetBinContent(i)*inputRelErrHist->GetBinError(i));
    }
}
inline TH1F * FracRatioHist(TH1F * hist1, TH1F * hist2, TString numString, TString denomString, bool doAbsRatio, TString fracratioBaseName, float fracRatioYAxisRange) {
    TString fracratioName = hist1->GetName();
    fracratioName += fracratioBaseName;
    TString fracRatioTitle = numString;
    fracRatioTitle += "/";
    fracRatioTitle += denomString;
    TH1F * fracRatioHist;
    if (doAbsRatio) {
        fracRatioHist = (TH1F *) hist1->Clone(fracratioName);
        fracRatioHist->Divide(fracRatioHist, hist2, 1, 1, "");
        HistAxisAttSet(fracRatioHist->GetYaxis(), fracRatioTitle, .15, .54, .14, .011, 1.0 - fracRatioYAxisRange, 1.0 + fracRatioYAxisRange);
        
    }
    else {
        fracRatioHist = (TH1F *) hist2->Clone(fracratioName);
        fracRatioHist->Add(hist1, -1);
        fracRatioHist->Divide(fracRatioHist, hist1, 1, 1, "");
        HistAxisAttSet(fracRatioHist->GetYaxis(), fracRatioTitle, .15, .54, .14, .011, -1.0 * fracRatioYAxisRange, 1.0 * fracRatioYAxisRange);
    }
    return fracRatioHist;
}
inline TH1F * FracRatioHist(TH1F * hist1, TH1F * hist2, TString numString, TString denomString, float axisLB, float axisUB, bool doAbsRatio, TString fracratioBaseName) {
    TString fracratioName = fracratioBaseName;
    TString fracRatioTitle = numString;
    fracRatioTitle += denomString;
    TH1F * fracRatioHist;
    if (doAbsRatio) {
        fracRatioHist = (TH1F *) hist1->Clone(fracratioName);
        fracRatioHist->Divide(fracRatioHist, hist2, 1, 1, "");
    }
    else {
        fracRatioHist = (TH1F *) hist2->Clone(fracratioName);
        fracRatioHist->Add(hist1, -1);
        fracRatioHist->Divide(fracRatioHist, hist1, 1, 1, "");
    }
    HistAxisAttSet(fracRatioHist->GetYaxis(), fracRatioTitle, .15, .54, .14, .011, axisLB, axisUB);
    return fracRatioHist;
}
inline void FixPad(TPad * &inputPad, int whichPad, TCanvas * &inputCanvas) {
    //    cout << "inputPad " << inputPad << endl;
    float m = 1.3;
    float n_ = 1;
    //    cout << "m " << m << endl;
    Double_t e_ = 15*m; //15
    Double_t k_ = 7*m; //15
    Double_t t_ = 30*m; //30
    Double_t b_ = 50*m; //80
    Double_t g_ = 87*m; //87
    Double_t d_ = 30*m; //30
    Double_t h_ = 350*m; //400
    Double_t w_ = 350*m; //350
    Double_t hl_ = 70*m;
    
    Double_t ww_ = g_ + w_ + e_ ;
    Double_t W_ = g_ + n_*w_ + 2*(n_-1)*e_ + d_;
    Double_t H_ = t_ + h_ + b_ + hl_ +2*k_ ;
    
    Double_t xlow_= 0;
    Double_t ylow_=0.;
    Double_t xup_=0;
    Double_t yup_=1.;
    inputCanvas->SetWindowSize(W_, H_);
    //    cout << "whichPad == " << whichPad << endl;
    if (whichPad == 1) {
        TString padname_("padhigh_");
        padname_ +=inputCanvas->GetTitle();
        xup_ = xlow_ + ww_ /W_;
        yup_ = 1.;
        ylow_ = (hl_ + b_ + k_ ) /H_;
        inputPad->SetPad(padname_, padname_, xlow_, ylow_, xup_, yup_, kWhite, 0, 0);
        xlow_ += (w_ + 2*e_)/W_;
        inputPad->SetLeftMargin(  g_/ww_ );
        inputPad->SetRightMargin( e_/ww_ );
        inputPad->SetTopMargin(  t_/H_ );
        inputPad->SetBottomMargin( k_/H_ );
        inputPad->SetFillColor(0);
        inputPad->SetTickx(1);
        inputPad->SetTicky(1);
        inputPad->SetFrameFillStyle(0);
        inputPad->SetFrameLineWidth(2);
        inputPad->SetFrameBorderMode(0);
    }
    else {
        //        cout << "trying some ish" << endl;
        TString padname_("padlow_");
        padname_ +=inputCanvas->GetTitle();
        xup_ = xlow_ + ww_ /W_;
        yup_ = (hl_ + b_ + k_ ) /H_;
        ylow_ = 0;
        inputPad->SetPad(padname_, padname_, xlow_, ylow_, xup_, yup_, kWhite, 0, 0);
        xlow_ += (w_ + 2*e_)/W_;
        inputPad->SetLeftMargin(  g_/ww_ );
        inputPad->SetRightMargin( e_/ww_ );
        inputPad->SetTopMargin( k_/H_ );
        inputPad->SetBottomMargin( b_ /(hl_ + b_ + k_ ) );
        inputPad->SetFillColor(0);
        inputPad->SetTickx(1);
        inputPad->SetTicky(1);
        inputPad->SetFrameFillStyle(0);
        inputPad->SetFrameLineWidth(2);
        inputPad->SetFrameBorderMode(0);
    }
}
inline void FixPadSingle(TPad * &inputPad, TCanvas * &inputCanvas) {
    //    cout << "inputPad " << inputPad << endl;
    Double_t padLeftMargin = 0.17;
    Double_t padRightMargin = 0.03;
    Double_t padTopMargin = 0.03;
    Double_t padBottomMargin = 0.11;
    Double_t canvWidth = 800.;
    Double_t canvHeight = 800.;
    
    inputCanvas->SetWindowSize(canvWidth, canvHeight);
    TString padname_ = "Pad_";
    padname_ += inputCanvas->GetTitle();
    Double_t xLow = 0.;
    Double_t xUp = 1.;
    Double_t yLow = 0.;
    Double_t yUp = 1.;
    Color_t borderColor = kWhite;
    Short_t borderSize = 0;
    Short_t borderMode = 0;
    
    inputPad->SetPad(padname_, padname_, xLow, yLow, xUp, yUp, borderColor, borderSize, borderMode);
    inputPad->SetLeftMargin(padLeftMargin);
    inputPad->SetRightMargin(padRightMargin);
    inputPad->SetBottomMargin(padBottomMargin);
    inputPad->SetTopMargin(padTopMargin);
    inputPad->SetFillColor(0);
    inputPad->SetTickx(1);
    inputPad->SetTicky(1);
    inputPad->SetFrameFillStyle(0);
    inputPad->SetFrameLineWidth(2);
    inputPad->SetFrameBorderMode(0);
}
inline void FixPadTriple(TPad * &inputPad, int whichPad, TCanvas * &inputCanvas) {
    
    Double_t canvWidth = 800.;
    Double_t canvHeight = 700.;
    
    inputCanvas->SetWindowSize(canvWidth, canvHeight);
    
    TString arrPadName[3] = {"padhigh_", "padmed_", "padlow_"};
    Double_t xLowPads[3] = {0.01, 0.01, 0.01};
//    Double_t yLowPads[3] = {2./3, 1./3, 0.037};
    Double_t yLowPads[3] = {2./3, 1./3, 0.01};
    Double_t xUpPads[3] = {0.99, 0.99, 0.99};
    Double_t yUpPads[3] = {0.99, 2./3 - .01, 1./3 + .015};
    
    Double_t leftMarginPads[3] = {0.09, 0.09, 0.09};
    Double_t rightMarginPads[3] = {0.033, 0.033, 0.033};
    Double_t topMarginPads[3] = {0.047, 0.0, 0.00};
    Double_t botMarginPads[3] = {0.147, 0.147, 0.33};

    TString padname_ = arrPadName[whichPad - 1];
    padname_ += inputCanvas->GetTitle();
    Color_t borderColor = kWhite;
    Short_t borderSize = 0;
    Short_t borderMode = 0;
    
    inputPad->SetPad(padname_, padname_, xLowPads[whichPad - 1], yLowPads[whichPad - 1], xUpPads[whichPad - 1], yUpPads[whichPad - 1], borderColor, borderSize, borderMode);
    
    inputPad->SetLeftMargin(leftMarginPads[whichPad - 1]);
    inputPad->SetRightMargin(rightMarginPads[whichPad - 1]);
    inputPad->SetTopMargin(topMarginPads[whichPad - 1]);
    inputPad->SetBottomMargin(botMarginPads[whichPad - 1]);
    inputPad->SetFillColor(0);
    inputPad->SetTickx(1);
    inputPad->SetTicky(1);
    inputPad->SetFrameFillStyle(0);
    inputPad->SetFrameLineWidth(2);
    inputPad->SetFrameBorderMode(0);
}

inline float dPhi(float phi1, float phi2) {
    float result = phi1-phi2;
    while (result >= TMath::Pi()) result -= 2*TMath::Pi();
    while (result < -1*TMath::Pi()) result += 2*TMath::Pi();
    return fabs(result);
}
inline float deltaR(float eta1, float phi1, float eta2, float phi2) {
    float dphi = dPhi(phi1,phi2);
    float deta = eta1-eta2;
    float result = dphi*dphi+deta*deta;
    result = sqrt(result);
    return result;
}

inline double dPhi(double phi1, double phi2) {
    double result = phi1-phi2;
    while (result >= TMath::Pi()) result -= 2*TMath::Pi();
    while (result < -1*TMath::Pi()) result += 2*TMath::Pi();
    return fabs(result);
}
inline double dPhi(TLorentzVector * inputVec1, TLorentzVector * inputVec2) {
    return dPhi(inputVec1->Phi(), inputVec2->Phi());
}

inline double deltaR(double eta1, double phi1, double eta2, double phi2) {
    double dphi = dPhi(phi1,phi2);
    double deta = eta1-eta2;
    double result = dphi*dphi+deta*deta;
    result = sqrt(result);
    return result;
}

inline Double_t deltaR(TLorentzVector * inputVec1, TLorentzVector * inputVec2) {
    Double_t DeltaPhi = dPhi(inputVec1->Phi(), inputVec2->Phi());
    Double_t DeltaEta = inputVec1->Eta() - inputVec2->Eta();
    Double_t result = DeltaEta * DeltaEta + DeltaPhi * DeltaPhi;
    result = sqrt(result);
    return result;
}

inline float GetMT(TLorentzVector * inputVec1, TLorentzVector * inputVec2) {
    float mass1 = inputVec1->M();
    float mass2 = inputVec2->M();
    float Et1 = inputVec1->Et();
    float Et2 = inputVec2->Et();
    float Pt1 = inputVec1->Pt();
    float Pt2 = inputVec2->Pt();
    float Angle = dPhi(inputVec1, inputVec2);
    float MTSq = mass1 * mass1 + mass2 * mass2 + 2 * (Et1 * Et2 - Pt1*Pt2*TMath::Cos(Angle));
    return TMath::Sqrt(MTSq);
}

inline void DefaultLorentzVector(TLorentzVector * inputVec) {
    inputVec->SetPtEtaPhiE(1E-8, 1E-8, 1E-8, 1E-8);
}
inline void PrintLorentzVector(TLorentzVector * inputVec) {
    cout << "vec Pt " << inputVec->Pt() << endl;
    cout << "vec Eta " << inputVec->Eta() << endl;
    cout << "vec Phi " << inputVec->Phi() << endl;
    cout << "vec En " << inputVec->E() << endl;
}

inline void HistogramUnderflowOverflow(TH1F * inputHist, bool doUnderflow, bool doOverflow) {
    float newHistErr;
    int NBins = inputHist->GetNbinsX();
    if (doUnderflow) {
        inputHist->SetBinContent(1, inputHist->GetBinContent(1) + inputHist->GetBinContent(0));
        newHistErr = TMath::Sqrt(inputHist->GetBinError(1)*inputHist->GetBinError(1) + inputHist->GetBinError(0)*inputHist->GetBinError(0));
        inputHist->SetBinError(1, newHistErr);
        inputHist->SetBinContent(0, 0);
        inputHist->SetBinError(0, 0);
    }
    if (doOverflow) {
        inputHist->SetBinContent(NBins, inputHist->GetBinContent(NBins) + inputHist->GetBinContent(NBins+1));
        newHistErr = TMath::Sqrt(inputHist->GetBinError(NBins)*inputHist->GetBinError(NBins) + inputHist->GetBinError(NBins+1)*inputHist->GetBinError(NBins+1));
        inputHist->SetBinError(NBins, newHistErr);
        inputHist->SetBinContent(NBins + 1, 0);
        inputHist->SetBinError(NBins + 1, 0);
    }
}

inline void RebinnedHistUnderOverflow(TH1F * inputHist, int whichBin, int whichDir) {
    float newHistErr;
    double integral, integralErr;
    int NBins = inputHist->GetNbinsX();
    if (whichDir < 0) {
        integral = inputHist->IntegralAndError(0, whichBin - 1, integralErr);
        inputHist->SetBinContent(whichBin, inputHist->GetBinContent(whichBin) + integral);
        newHistErr = TMath::Sqrt(inputHist->GetBinError(whichBin)*inputHist->GetBinError(whichBin) + integralErr * integralErr);
        inputHist->SetBinError(whichBin, newHistErr);
        for (int iBin = 0; iBin <= whichBin -1; ++iBin) {
            inputHist->SetBinContent(iBin, 0);
            inputHist->SetBinError(iBin, 0);
        }
    }
    else if (whichDir > 0) {
        integral = inputHist->IntegralAndError(whichBin + 1, NBins + 1, integralErr);
        inputHist->SetBinContent(whichBin, inputHist->GetBinContent(whichBin) + integral);
        newHistErr = TMath::Sqrt(inputHist->GetBinError(whichBin) * inputHist->GetBinError(whichBin) + integralErr * integralErr);
        inputHist->SetBinError(whichBin, newHistErr);
        for (int iBin = whichBin + 1; iBin <= NBins + 1; ++iBin) {
            inputHist->SetBinContent(iBin, 0);
            inputHist->SetBinError(iBin, 0);
        }
    }
}

inline void BadBinCheck(int &checkInt, int inputAxisNBins) {
    if (checkInt == 0) {
        cout << "problem with information entropy, homeslice! Input value is too low! Setting it to be right at histogram edge" << endl;
        checkInt = 1;
    }
    else if (checkInt == inputAxisNBins + 1) {
        cout << "problem with information entropy, homeslice! Input value is too high! Setting it to be right at histogram edge" << endl;
        checkInt = inputAxisNBins;
    }
}
inline void BinFinder(int * address, TH1 * hist, float x, float y, float z, bool doBadBinCheck, bool doVerbosity = 0) {
    TAxis * xAxis = hist->GetXaxis();
    TAxis * yAxis, * zAxis;
    address[0] = xAxis->FindBin(x);
    if (doBadBinCheck) {
        BadBinCheck(address[0], xAxis->GetNbins());
    }
    if (doVerbosity) {
        cout << "for hist " << hist->GetName() << endl;
        cout << "for x, " << x << " address[0] is " << address[0] << endl;
    }
    yAxis = hist->GetYaxis();
    if (yAxis->GetNbins() != 1) {
        address[1] = yAxis->FindBin(y);
        if (doBadBinCheck) {
            BadBinCheck(address[1], yAxis->GetNbins());
        }
        if (doVerbosity) {
            cout << "for y, " << y << " address[1] is " << address[1] << endl;
        }
    }
    zAxis = hist->GetZaxis();
    if (zAxis->GetNbins() != 1) {
        address[2] = zAxis->FindBin(z);
        if (doBadBinCheck) {
            BadBinCheck(address[2], zAxis->GetNbins());
        }
        if (doVerbosity) {
            cout << "for z, " << z << " address[2] is " << address[2] << endl;
        }
    }
}
inline void PassFailCut1D(TH1 * inHist, double &numPassCut, double &numPassCutErr, double &numFailCut, double &numFailCutErr, int * binAddreses) {
    double totalEventsErr;
    double totalEvents = inHist->IntegralAndError(1, inHist->GetNbinsX() + 1, totalEventsErr);

    numPassCut = inHist->IntegralAndError(binAddreses[0], inHist->GetNbinsX() + 1, numPassCutErr);
    numFailCut = totalEvents - numPassCut;
    numFailCutErr = TMath::Sqrt(totalEventsErr*totalEventsErr - numPassCutErr * numPassCutErr);
}
inline void PassFailANDCut2D(TH1 * inHist, double &numPassCut, double &numPassCutErr, double &numFailCut, double &numFailCutErr, int * binAddreses) {
    double totalEventsErr;
    double totalEvents = ((TH2 *) inHist)->IntegralAndError(1, inHist->GetNbinsX() + 1, 1, inHist->GetNbinsY() + 1, totalEventsErr);
    numPassCut = ((TH2 *) inHist)->IntegralAndError(binAddreses[0], inHist->GetNbinsX() + 1, binAddreses[1], inHist->GetNbinsY() + 1, numPassCutErr);
    numFailCut = totalEvents - numPassCut;
    numFailCutErr = TMath::Sqrt(totalEventsErr*totalEventsErr - numPassCutErr * numPassCutErr);
}
inline void PassFailANDCut3D(TH1 * inHist, double &numPassCut, double &numPassCutErr, double &numFailCut, double &numFailCutErr, int * binAddreses) {
    double totalEventsErr;
    double totalEvents = ((TH3 *) inHist)->IntegralAndError(1, inHist->GetNbinsX() + 1, 1, inHist->GetNbinsY() + 1, 1, inHist->GetNbinsZ() + 1, totalEventsErr);
    numPassCut = ((TH3 *) inHist)->IntegralAndError(binAddreses[0], inHist->GetNbinsX() + 1, binAddreses[1], inHist->GetNbinsY() + 1, binAddreses[2], inHist->GetNbinsZ() + 1, numPassCutErr);
    numFailCut = totalEvents - numPassCut;
    numFailCutErr = TMath::Sqrt(totalEventsErr*totalEventsErr - numPassCutErr * numPassCutErr);
}
inline void PassFailORCut2D(TH1 * inHist, double &numPassCut, double &numPassCutErr, double &numFailCut, double &numFailCutErr, int * binAddreses) {
    double totalEventsErr;
    double totalEvents = ((TH2 *) inHist)->IntegralAndError(1, inHist->GetNbinsX() + 1, 1, inHist->GetNbinsY() + 1, totalEventsErr);

    if (binAddreses[0] == 1 || binAddreses[1] == 1) {
        numFailCut = 0;
        numFailCutErr = 0;
    }
    else {
        numFailCut = ((TH2 *) inHist)->IntegralAndError(1, binAddreses[0] - 1, 1, binAddreses[1] - 1, numFailCutErr);
    }
    numPassCut = totalEvents - numFailCut;
    numPassCutErr = TMath::Sqrt(totalEventsErr*totalEventsErr - numFailCutErr * numFailCutErr);
}
inline void PassFailORCut3D(TH1 * inHist, double &numPassCut, double &numPassCutErr, double &numFailCut, double &numFailCutErr, int * binAddreses) {
    double totalEventsErr;
    double totalEvents = ((TH3 *) inHist)->IntegralAndError(1, inHist->GetNbinsX() + 1, 1, inHist->GetNbinsY() + 1, 1, inHist->GetNbinsZ() + 1, totalEventsErr);
    if (binAddreses[0] == 1 || binAddreses[1] == 1 || binAddreses[2] == 1) {
        numFailCut = 0;
        numFailCutErr = 0;
    }
    else {
        numFailCut = ((TH3 *) inHist)->IntegralAndError(1, binAddreses[0] - 1, 1, binAddreses[1] - 1, 1, binAddreses[2] - 1, numFailCutErr);
    }
    numPassCut = totalEvents - numFailCut;
    numPassCutErr = TMath::Sqrt(totalEventsErr*totalEventsErr - numFailCutErr * numFailCutErr);
}

inline void PassFailCut(TH1 * inHist, double &numPassCut, double &numPassCutErr, double &numFailCut, double &numFailCutErr, int * binAddreses, int whichIntType, bool doVerbosity = 0) {
    //whichIntType
    //0 is AND
    //1 is OR
    //2 is XOR -- not implemented yet though
    
    int numDims = 1;
    if (inHist->GetNbinsY() > 1) {
        numDims = (inHist->GetNbinsZ() > 1) ? 3 : 2;
    }
    switch (numDims) {
        case 1:
            PassFailCut1D(inHist, numPassCut, numPassCutErr, numFailCut, numFailCutErr, binAddreses);
            break;
        case 2:
            switch (whichIntType) {
                case 0:
                    PassFailANDCut2D(inHist, numPassCut, numPassCutErr, numFailCut, numFailCutErr, binAddreses);
                    break;
                case 1:
                    PassFailORCut2D(inHist, numPassCut, numPassCutErr, numFailCut, numFailCutErr, binAddreses);
                    break;
                default:
                    cout << "whichIntType needs to be 0 or 1: " << whichIntType << endl;
                    break;
            }
            break;
        case 3:
            switch (whichIntType) {
                case 0:
                    PassFailANDCut3D(inHist, numPassCut, numPassCutErr, numFailCut, numFailCutErr, binAddreses);
                    break;
                case 1:
                    PassFailORCut3D(inHist, numPassCut, numPassCutErr, numFailCut, numFailCutErr, binAddreses);
                    break;
                default:
                    cout << "whichIntType needs to be 0 or 1: " << whichIntType << endl;
                    break;
            }
            break;
        default:
            break;
    }
}

inline void PassFailCuts(vector<double> * vecIntegrals, vector<double> * vecIntegralErrors, int * binAddreses, TH1 * inHist, int whichIntType, bool doVerbosity = 0) {
    //it seems that ROOT is intelligent about bin errors and so I don't need to worry about setting the bin errors by hand
    double numPassCut, numFailCut;
    double numPassCutErr, numFailCutErr;
    PassFailCut(inHist, numPassCut, numPassCutErr, numFailCut, numFailCutErr, binAddreses, whichIntType);
    if (doVerbosity) {
        cout << "doing cut type of whichIntType " << whichIntType << endl;
        cout << "numPass Cut " << numPassCut << " +- " << numPassCutErr << endl;
        cout << "numFail Cut " << numFailCut << " +- " << numFailCutErr << endl;
    }
    vecIntegrals->push_back(numFailCut);
    vecIntegrals->push_back(numPassCut);
    
    vecIntegralErrors->push_back(numFailCutErr);
    vecIntegralErrors->push_back(numPassCutErr);
}
/*
inline void PassFailCuts(vector<double> * integrals, vector<double> * integralErrors, int * binAddreses, TH1 * hist, bool doVerbosity = 0) {
    double numPassCut, numFailCut;
    double numPassCutErr, numFailCutErr;
    numPassCut = hist->IntegralAndError(binAddreses[0], hist->GetNbinsX() + 1, numPassCutErr);
    if (binAddreses[0] == 1) {
        numFailCut = 0;
        numFailCutErr = 0;
    }
    else {
        numFailCut = hist->IntegralAndError(1, binAddreses[0] - 1, numFailCutErr);
    }
    if (doVerbosity) {
        cout << "numPass Cut " << numPassCut << endl;
        cout << "numFail Cut " << numFailCut << endl;
    }
    integrals->push_back(numFailCut);
    integrals->push_back(numPassCut);
    integralErrors->push_back(numFailCutErr);
    integralErrors->push_back(numPassCutErr);
}
inline void PassFailCuts(vector<double> * integrals, vector<double> * integralErrors, int * binAddreses, TH2 * hist, bool doVerbosity = 0) {
    double numPassCut, numFailCut;
    double numPassCutErr, numFailCutErr;
    numPassCut = hist->IntegralAndError(binAddreses[0], hist->GetNbinsX() + 1, binAddreses[1], hist->GetNbinsY() + 1, numPassCutErr);
    if (binAddreses[0] == 1 && binAddreses[1] == 1) {
        numFailCut = 0;
        numFailCutErr = 0;
    }
    else {
        numFailCut = hist->IntegralAndError(1, binAddreses[0] - 1, 1, binAddreses[1] - 1, numFailCutErr);
    }
    if (doVerbosity) {
        cout << "numPass Cut " << numPassCut << endl;
        cout << "numFail Cut " << numFailCut << endl;
    }
    integrals->push_back(numFailCut);
    integrals->push_back(numPassCut);
    integralErrors->push_back(numFailCutErr);
    integralErrors->push_back(numPassCutErr);
}
inline void PassFailCuts(vector<double> * integrals, vector<double> * integralErrors, int * binAddreses, TH3 * hist) {
    double numPassCut, numFailCut;
    double numPassCutErr, numFailCutErr;
    numPassCut = hist->IntegralAndError(binAddreses[0], hist->GetNbinsX() + 1, binAddreses[1], hist->GetNbinsY() + 1, binAddreses[2], hist->GetNbinsZ() + 1, numPassCutErr);
    if (binAddreses[0] == 1 && binAddreses[1] == 1 && binAddreses[2] == 1) {
        numFailCut = 0;
        numFailCutErr = 0;
    }
    else {
        numFailCut = hist->IntegralAndError(1, binAddreses[0] - 1, 1, binAddreses[1] - 1, 1, binAddreses[2] - 1, numFailCutErr);
    }
    integrals->push_back(numFailCut);
    integrals->push_back(numPassCut);
    integralErrors->push_back(numFailCutErr);
    integralErrors->push_back(numPassCutErr);
}
*/
inline Double_t ElectronEffectiveArea(float elecEta) {
    // calculate the effective area of the electron based upon its eta
    
    //should check numbers here:
    //http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h?revision=1.3&view=markup
    Double_t Aeff = 0.;
    if (fabs(elecEta) < 1.0 )         Aeff = 0.13; // +/- 0.001
    else if (fabs(elecEta) < 1.479)    Aeff = 0.14; // +/- 0.002
    else if (fabs(elecEta) < 2.0)      Aeff = 0.07; // +/- 0.001
    else if (fabs(elecEta) < 2.2)      Aeff = 0.09; // +/- 0.001
    else if (fabs(elecEta) < 2.3)      Aeff = 0.11; // +/- 0.002
    else if (fabs(elecEta) < 2.4)      Aeff = 0.11; // +/- 0.003
    else if (fabs(elecEta) > 2.4)      Aeff = 0.14; // +/- 0.004
    
    return Aeff;
}
inline TLorentzVector LeptonScaleSystShift(TLorentzVector inputLepVec, int inputLepPDGID, int shiftDirection) {
    // Shift Leptons within energy scales
    // Note that these numbers might need to be update
    float barrelEtaEnd = 1.4442;
    //float endcapEtaStart = 1.566;
    float electronESEB = 0.006; float electronESEE = 0.015;
    float muonES = 0.002;
    float scaleToUse = 0;
    TLorentzVector outShiftVec = inputLepVec;
    if (abs(inputLepPDGID) == 11) {
        scaleToUse = ((inputLepVec.Eta() < barrelEtaEnd) ? electronESEB : electronESEE);
    }
    else if (abs(inputLepPDGID) == 13) {
        scaleToUse = muonES;
    }
    else {
        cout << "wtf?! not dealing with electron or muon" << endl;
    }
    outShiftVec *= (1 + shiftDirection * scaleToUse);
    return outShiftVec;
}

/*
inline TH1F * PassCutHisto(TH1 * inputHist, vector<int> * values, vector<TString> * names, TString histNameAppend, TString SystAppendName = "", int levelVerbosity = 0) {
    //TH1F * PassCutHisto(TH1 * inputHist, vector<int> * values, vector<TString> * names, TString SystAppendName = "", int levelVerbosity = 0) {
    /// Function to make a custom "pass/fail histogram "
    // inputs are the following
    // vector of integer values which are the cut points
    // vector of names for the variable that's being cut -- c.f. integer cut values
    // appendName for what systematic if any the histogram comes from
    
    unsigned int numVals = values->size();
    if (levelVerbosity) {
        cout << "in PassCutHisto: number of values to cut on " << numVals << endl;
    }
    if (numVals == 0) {
        cout << "issue with values vector: it's size 0!" << endl;
    }
    else {
        if (numVals != names->size()) {
            cout << "issue with vector sizes " << endl;
            cout << "values->size() " << numVals << endl;
            cout << "names->size() " << names->size() << endl;
        }
    }
    
    TString baseHistName = "h_PassFail_";
    TString baseAxisName = "Pass/Fail ";
    for (unsigned int iCut = 0; iCut < values->size(); ++iCut) {
        baseHistName += names->at(iCut);
        baseHistName += "Cut";
        baseHistName += values->at(iCut);
        if (iCut != values->size()) {
            baseHistName += "_";
        }
        
        baseAxisName += names->at(iCut);
        baseAxisName += " > ";
        baseAxisName += values->at(iCut);
        if (iCut != values->size() - 1) {
            baseAxisName += ", ";
        }
    }
    baseHistName += SystAppendName;
    TH1F * outHist = new TH1F(baseHistName + histNameAppend, baseAxisName, 2, -0.5, 1.5);
    if (levelVerbosity) {
        cout << "outHist name " << outHist->GetName() << endl;
    }
    int binAddreses[3] = {1, 1, 1};
    float x = (float) values->at(0);
    float y = numVals > 1 ? (float) values->at(1) : 1e99;
    float z = numVals > 2 ? (float) values->at(2) : 1e99;
    if (levelVerbosity) {
        cout << "x:y:z = " << x << ":" << y << ":" << z << endl;
        cout << "inputHist name " << inputHist->GetName() << endl;
    }
    BinFinder(binAddreses, inputHist, x, y, z);
    vector<double> vecIntegrals;
    
    vector<double> vecIntegralErrors;
    if (numVals > 1) {
        if (numVals > 2) {
            PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, (TH3 *) inputHist);
        }
        else {
            PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, (TH2 *) inputHist);
        }
    }
    else {
        PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist);
    }
    
    outHist->SetBinContent(1, vecIntegrals[0]);
    outHist->SetBinError(1, vecIntegralErrors[0]);
    outHist->SetBinContent(2, vecIntegrals[1]);
    outHist->SetBinError(2, vecIntegralErrors[1]);
    outHist->GetXaxis()->SetBinLabel(1, "Failed Cut");
    outHist->GetXaxis()->SetBinLabel(2, "Passed Cut");
    return outHist;
}
*/


inline TH1F * PassCutHisto(TH1 * inputHist, vector<int> * values, vector<TString> * names, TString histNameAppend, int whichIntType, TString SystAppendName = "", int levelVerbosity = 0) {
    //TH1F * PassCutHisto(TH1 * inputHist, vector<int> * values, vector<TString> * names, TString SystAppendName = "", int levelVerbosity = 0) {
    /// Function to make a custom "pass/fail histogram "
    // inputs are the following
    // vector of integer values which are the cut points
    // vector of names for the variable that's being cut -- c.f. integer cut values
    // appendName for what systematic if any the histogram comes from
    
    unsigned int numVals = values->size();
    if (levelVerbosity) {
        cout << "in PassCutHisto: number of values to cut on " << numVals << endl;
    }
    if (numVals == 0) {
        cout << "issue with values vector: it's size 0!" << endl;
    }
    else {
        if (numVals != names->size()) {
            cout << "issue with vector sizes " << endl;
            cout << "values->size() " << numVals << endl;
            cout << "names->size() " << names->size() << endl;
        }
    }
    
    TString baseHistName = "h_PassFail_";
    TString baseAxisName = "Pass/Fail ";
    for (unsigned int iCut = 0; iCut < values->size(); ++iCut) {
        baseHistName += names->at(iCut);
        baseHistName += "Cut";
        baseHistName += values->at(iCut);
        if (iCut != values->size()) {
            baseHistName += "_";
        }
        
        baseAxisName += names->at(iCut);
        baseAxisName += " > ";
        baseAxisName += values->at(iCut);
        if (iCut != values->size() - 1) {
            baseAxisName += ", ";
        }
    }
    baseHistName += SystAppendName;
    TH1F * outHist = new TH1F(baseHistName + histNameAppend, baseAxisName, 2, -0.5, 1.5);
    if (levelVerbosity) {
        cout << "outHist name " << outHist->GetName() << endl;
    }
    int binAddreses[3] = {1, 1, 1};
    float x = (float) values->at(0);
    float y = numVals > 1 ? (float) values->at(1) : 1e99;
    float z = numVals > 2 ? (float) values->at(2) : 1e99;
    if (levelVerbosity) {
        cout << "x:y:z = " << x << ":" << y << ":" << z << endl;
        cout << "inputHist name " << inputHist->GetName() << endl;
    }
    BinFinder(binAddreses, inputHist, x, y, z, true, levelVerbosity);
    vector<double> vecIntegrals;
    vector<double> vecIntegralErrors;
    PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntType, levelVerbosity);
    outHist->SetBinContent(1, vecIntegrals[0]);
    outHist->SetBinError(1, vecIntegralErrors[0]);
    
    outHist->SetBinContent(2, vecIntegrals[1]);
    outHist->SetBinError(2, vecIntegralErrors[1]);
    
    outHist->GetXaxis()->SetBinLabel(1, "Failed Cut");
    outHist->GetXaxis()->SetBinLabel(2, "Passed Cut");
    return outHist;
}

inline TH1F * LogYHist(TH1F * inputHist, bool doVerbosity) {
    int nBins = inputHist->GetNbinsX();
    TH1F * outHist =  (TH1F *) inputHist->Clone(inputHist->GetName() + TString("_LogY"));
    float currBinContent, currBinErr;
    float x, y, yErr;
    for (int iBin = 1; iBin <= nBins; ++iBin) {
        x = inputHist->GetXaxis()->GetBinCenter(iBin);
        currBinContent = inputHist->GetBinContent(iBin);
        currBinErr = inputHist->GetBinError(iBin);
        if (currBinContent > 0) {
            y = TMath::Log(currBinContent);
            yErr = TMath::Abs(y * currBinErr / currBinContent);
            outHist->SetBinContent(iBin, y);
            outHist->SetBinError(iBin, yErr);
            if (doVerbosity) {
                cout << "x " << x << endl;
                cout << "currBinContent " << currBinContent << endl;
                cout << "currBinErr " << currBinErr << endl;
                cout << "y " << y << endl;
                cout << "yErr " << yErr << endl;
            }
        }
    }
    return outHist;
}

inline TGraphErrors LogYGraph(TH1F * inputHist, bool doVerbosity) {
    int nBins = inputHist->GetNbinsX();
    TGraphErrors outGraph(nBins);
    float currBinContent, currBinErr;
    float x, y, yErr;
    for (int iBin = 0; iBin < nBins; ++iBin) {
        x = inputHist->GetXaxis()->GetBinCenter(iBin + 1);
        currBinContent = inputHist->GetBinContent(iBin + 1);
        currBinErr = inputHist->GetBinError(iBin + 1);
        if (currBinContent > 0) {
            y = TMath::Log(currBinContent);
            yErr = TMath::Abs(y * currBinErr / currBinContent);
            outGraph.SetPoint(iBin, x, y);
            outGraph.SetPointError(iBin, 0, yErr);
            if (doVerbosity) {
                cout << "x " << x << endl;
                cout << "currBinContent " << currBinContent << endl;
                cout << "currBinErr " << currBinErr << endl;
                cout << "y " << y << endl;
                cout << "yErr " << yErr << endl;
            }
        }
    }
    return outGraph;
}

inline TGraph LogLogGraph(TH1F * inputHist) {
    int nBins = inputHist->GetNbinsX();
    TGraph outGraph(nBins);
    float x, y; //, yErr; -- not using y-error for now
    for (int iBin = 0; iBin < nBins; ++iBin) {
        x = TMath::Log(inputHist->GetXaxis()->GetBinLowEdge(iBin + 1));
        y = TMath::Log(inputHist->GetBinContent(iBin + 1));
        outGraph.SetPoint(iBin, x, y);
    }
    return outGraph;
}

inline TGraph ResidualsGraph(TGraph * inputGraph, TF1 * inputTF1) {
    TGraph outGraph(*inputGraph);
    int nPoints = inputGraph->GetN();
    double x, y;
    double funcVal, residual;
    for (int iPoint = 0; iPoint < nPoints; ++iPoint) {
        inputGraph->GetPoint(iPoint, x, y);
        funcVal = inputTF1->Eval(x);
        residual = y - funcVal;
        outGraph.SetPoint(iPoint, x, residual);
    }
    return outGraph;
}
inline void UnLogHist(TF1 * inputTF1, TH1F * inputHistToSet, bool isLogX = true, bool isLogY = true) {
    float xBinCent, yBinVal;
    for (int iBin = 1; iBin <= inputHistToSet->GetNbinsX(); ++iBin) {
        xBinCent = inputHistToSet->GetXaxis()->GetBinCenter(iBin);
        if (isLogX) {
            xBinCent = TMath::Log(xBinCent);
        }
        yBinVal = inputTF1->Eval(xBinCent);
        if (isLogY) {
            yBinVal = TMath::Exp(yBinVal);
        }
        inputHistToSet->SetBinContent(iBin, yBinVal);
    }
}

inline void UnLogXAxis(TGraph * inputGraph) {
    int nPoints = inputGraph->GetN();
    double x, y;
    for (int iPoint = 0; iPoint < nPoints; ++iPoint) {
        inputGraph->GetPoint(iPoint, x, y);
        inputGraph->SetPoint(iPoint, TMath::Exp(x), y);
    }
}

inline TGraph * DivideGraph(TGraph * inputGraph1, TGraph * inputGraph2, TString outName, TString yAxis) {
    TGraph * outGraph = (TGraph *) inputGraph1->Clone(outName);
    outGraph->GetYaxis()->SetTitle(yAxis);
    double x1, y1;
    double x2, y2;
    for (int iPoint = 0; iPoint < inputGraph1->GetN(); ++iPoint) {
        inputGraph1->GetPoint(iPoint, x1, y1);
        inputGraph2->GetPoint(iPoint, x2, y2);
        if (abs(x1 - x2) >= 1E-3) {
            cout << "difference between x1 and x2 is too large for iPoint " << iPoint << endl;
            cout << "difference is " << abs(x1 - x2) << endl;
        }
        outGraph->SetPoint(iPoint, x1, y1/y2);
    }
    return outGraph;
}

inline void HistDivBinContent(TH1 * inputTH1) {
    float binTotalVolume;
    TAxis * xAxis = inputTH1->GetXaxis();
    TAxis * yAxis = inputTH1->GetYaxis();
    TAxis * zAxis = inputTH1->GetZaxis();
    float currBinContent, currBinErr;
    for (int iBinX = 1; iBinX <= xAxis->GetNbins(); ++iBinX) {
        for (int iBinY = 1; iBinY <= yAxis->GetNbins(); ++iBinY) {
            for (int iBinZ = 1; iBinZ <= zAxis->GetNbins(); ++iBinZ) {
                binTotalVolume = xAxis->GetBinWidth(iBinX) * yAxis->GetBinWidth(iBinY) * zAxis->GetBinWidth(iBinZ);
                currBinContent = inputTH1->GetBinContent(iBinX, iBinY, iBinZ);
                currBinErr = inputTH1->GetBinError(iBinX, iBinY, iBinZ);
                inputTH1->SetBinContent(iBinX, iBinY, iBinZ, currBinContent / binTotalVolume);
                inputTH1->SetBinError(iBinX, iBinY, iBinZ, currBinErr / binTotalVolume);
            }
        }
    }
}




inline float ScaleBackCalcBasic(TFile * inputFile, bool doVerbosity = false, int whichSyst = 0, vector<TString> * vecSystString = 0) {
    TString mcplot = "h_nVtx_inclusive";
    TString mcplot_preRW = "h_BasicWeightIntegral_inclusive";
    
    TString fileName = inputFile->GetName();
    TString badSysts[2] = {"JetSmear", "UncES"};
    bool isBadSyst = false;
    if (whichSyst != 0) {
        for (int iBadSyst = 0; iBadSyst < 3; ++iBadSyst) {
            if (vecSystString->at(abs(whichSyst) - 1).Contains(badSysts[iBadSyst])) {
                isBadSyst = true;
            }
        }
        if (vecSystString->at(abs(whichSyst) - 1).Contains("genStop")) {
            if ((fileName.Contains("T2tt") || fileName.Contains("T2bw"))) {
                mcplot += vecSystString->at(abs(whichSyst) - 1) + TString("Shift");
                mcplot_preRW += vecSystString->at(abs(whichSyst) - 1) + TString("Shift");
                if (whichSyst > 0) {
                    mcplot += "Up";
                    mcplot_preRW += "Up";
                }
                else {
                    mcplot += "Down";
                    mcplot_preRW += "Down";
                }
            }
        }
        else {
            if (!isBadSyst) {
                mcplot += vecSystString->at(abs(whichSyst) - 1) + TString("Shift");
                mcplot_preRW += vecSystString->at(abs(whichSyst) - 1) + TString("Shift");
                if (whichSyst > 0) {
                    mcplot += "Up";
                    mcplot_preRW += "Up";
                }
                else {
                    mcplot += "Down";
                    mcplot_preRW += "Down";
                }
            }
        }
    }
    if (doVerbosity) {
        cout << "from file " << fileName << endl;
        cout << "mcplot to grab " << mcplot << endl;
        cout << "mcplot_preRW to grab " << mcplot_preRW << endl;
    }
    TH1F * nVtxOrigHist;
    TH1F * nVtxNewHist;
    int NBinsX, NBinsXNew;
    float scaleBack;
    nVtxOrigHist = (TH1F*) inputFile->Get(mcplot_preRW);
    nVtxNewHist = (TH1F*) inputFile->Get(mcplot);
    NBinsX = nVtxOrigHist->GetNbinsX();
    NBinsXNew = nVtxNewHist->GetNbinsX();
    scaleBack = (float) nVtxOrigHist->Integral(1, NBinsX + 1) / nVtxNewHist->Integral(1, NBinsXNew + 1);
    return scaleBack;
}

void ZeroOutNegHistBins(TH1 * inputHist) {
    int NBinsX = inputHist->GetNbinsX();
    int NBinsY = inputHist->GetNbinsY();
    int NBinsZ = inputHist->GetNbinsZ();
    for (int iBinX = 0; iBinX <= NBinsX + 1; ++iBinX) {
        for (int iBinY = 0; iBinY <= NBinsY + 1; ++iBinY) {
            for (int iBinZ = 0; iBinZ <= NBinsZ + 1; ++iBinZ) {
                if (inputHist->GetBinContent(iBinX, iBinY, iBinZ) < 0) {
                    inputHist->SetBinContent(iBinX, iBinY, iBinZ, 0);
                    inputHist->SetBinError(iBinX, iBinY, iBinZ, 0);
                }
            }
        }
    }
}


TH1F * CompareHists(TH2F * histCoarseBin, TH2F * histFineBin) {
    
    TH1F * testDiff = new TH1F("testDiff", ";#frac{Value Coarse - Value Fine}{Value Coarse}; Number of bins", 100, -1, 1);
    
    int nBinsX = histCoarseBin->GetNbinsX();
    int nBinsY = histCoarseBin->GetNbinsY();
    
    float currBinCenterX, currBinCenterY;
    float currFineBinCenterX, currFineBinCenterY;
    
    int currBinX_FineHist;
    int currBinY_FineHist;
    
    float currBinContent_Coarse, currBinContent_Fine;
    float currDiff;
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        currBinCenterX = histCoarseBin->GetXaxis()->GetBinCenter(iBinX);
        currBinX_FineHist = histFineBin->GetXaxis()->FindBin(currBinCenterX);
        currFineBinCenterX = histFineBin->GetXaxis()->GetBinCenter(currBinX_FineHist);
        if (abs(currFineBinCenterX - currBinCenterX) > 1E-2) {
            cout << "something wrong with their X bin difference!" << currFineBinCenterX - currBinX_FineHist << endl;
        }
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            currBinContent_Coarse = histCoarseBin->GetBinContent(iBinX, iBinY);
            
            currBinCenterY = histCoarseBin->GetYaxis()->GetBinCenter(iBinY);
            currBinY_FineHist = histFineBin->GetYaxis()->FindBin(currBinCenterY);
            currFineBinCenterY = histFineBin->GetYaxis()->GetBinCenter(currBinY_FineHist);
            if (abs(currFineBinCenterY - currBinCenterY) > 1E-2) {
                cout << "something wrong with their Y bin difference!" << currFineBinCenterY - currBinY_FineHist << endl;
            }

            
            
            currBinContent_Fine = histFineBin->GetBinContent(currBinX_FineHist, currBinY_FineHist);
            
            currDiff = currBinContent_Coarse - currBinContent_Fine;
            
            cout << "for mStop:mLSP of " << currBinCenterX << ":" << currBinCenterY << endl;
            cout << "diff of values is " << currDiff << endl;
            testDiff->Fill(currDiff/currBinContent_Coarse);
        }
    }
    return testDiff;
}

int FindBinInHistRange(TAxis * inputAxis, float inputXVal) {
    int nBins = inputAxis->GetNbins();
    int currBin = inputAxis->FindBin(inputXVal);
    
    if (currBin < 1) currBin = 1;
    if (currBin > nBins) currBin = nBins;
    
    return currBin;
}

void SetHistogramData(TH1 * inputHist, TString &histName, vector<int> * inVecNumBinsAxis, vector<TAxis *> * inVecHistAxes, vector<float> * inVecAxisBinWidths) {
    //Function to automatically set-up commonly used axis parameters
    
    inVecNumBinsAxis->resize(0);
    inVecHistAxes->resize(0);
    inVecAxisBinWidths->resize(0);

    histName = inputHist->GetName();
    
    TAxis * xAxis = inputHist->GetXaxis();
    TAxis * yAxis = inputHist->GetYaxis();
    TAxis * zAxis = inputHist->GetZaxis();
    
    inVecNumBinsAxis->push_back(xAxis->GetNbins());
    inVecNumBinsAxis->push_back(yAxis->GetNbins());
    inVecNumBinsAxis->push_back(zAxis->GetNbins());
    
    inVecAxisBinWidths->push_back(xAxis->GetBinWidth(1));
    inVecAxisBinWidths->push_back(yAxis->GetBinWidth(1));
    inVecAxisBinWidths->push_back(zAxis->GetBinWidth(1));
    
    inVecHistAxes->push_back(xAxis);
    inVecHistAxes->push_back(yAxis);
    inVecHistAxes->push_back(zAxis);
}


TString ConditionalStringReplacement(TString inputString, TString searchString, TString replaceString) {
    TString outString = inputString;
    int strIndex = inputString.Index(searchString);
    if (strIndex != -1) {
        int sizeSearch = searchString.Length();
        outString.Replace(strIndex, sizeSearch, replaceString);
    }
    return outString;
}

TH1 * LoadMultipleHistogram(vector<TFile *> * vecInputFiles, TString grabHistName, int minIndex, int maxIndex, TString addName) {
    TH1 * currHist = NULL, * outHist = NULL;
    TString currHistName;
    for (int iFile = minIndex; iFile < maxIndex; ++iFile) {
        currHist = (TH1 *) vecInputFiles->at(iFile)->Get(grabHistName);
        currHistName = currHist->GetName();
        if (outHist == NULL) {
            outHist = (TH1 *) currHist->Clone(currHistName + addName);
        }
        else {
            outHist->Add(currHist);
        }
    }
    return outHist;
}

void CloseMultipleFiles(vector<TFile *> * vecInputFiles) {
    unsigned int numFiles = vecInputFiles->size();
    for (unsigned int iFile = 0; iFile < numFiles; ++iFile) {
        if (vecInputFiles->at(iFile)) {
            cout << "going to close " << vecInputFiles->at(iFile)->GetName() << endl;
            vecInputFiles->at(iFile)->Close();
        }
    }
}

void DeleteVector(vector<TObject *> * inVecTObjects) {
    unsigned int numObjs = inVecTObjects->size();
    for (unsigned int iObj = 0; iObj < numObjs; ++iObj) {
        inVecTObjects->at(iObj)->Delete();
    }
    inVecTObjects->clear();
}

void DeleteVector(vector<TH2F *> * inVecTObjects) {
    unsigned int numObjs = inVecTObjects->size();
    for (unsigned int iObj = 0; iObj < numObjs; ++iObj) {
        inVecTObjects->at(iObj)->Delete();
    }
    inVecTObjects->clear();
}

void SmoothNearestNeighbor() {
    
}


