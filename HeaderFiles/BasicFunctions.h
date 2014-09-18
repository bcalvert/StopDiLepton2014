#include <cmath>
#include "TCanvas.h"
#include "TH1F.h"
#include "TString.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "./HistogramStyleFunctions.h"
#include "TGraph.h"
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
    }
    if (doOverflow) {
        inputHist->SetBinContent(NBins, inputHist->GetBinContent(NBins) + inputHist->GetBinContent(NBins+1));
        newHistErr = TMath::Sqrt(inputHist->GetBinError(NBins)*inputHist->GetBinError(NBins) + inputHist->GetBinError(NBins+1)*inputHist->GetBinError(NBins+1));
        inputHist->SetBinError(NBins, newHistErr);
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
inline void BinFinder(int * address, TH1 * hist, float x, float y, float z) {
    TAxis * xAxis = hist->GetXaxis();
    TAxis * yAxis, * zAxis;
    address[0] = xAxis->FindBin(x);
    BadBinCheck(address[0], xAxis->GetNbins());
    cout << "for hist " << hist->GetName() << endl;
    cout << "for x, " << x << " address[0] is " << address[0] << endl;
    yAxis = hist->GetYaxis();
    if (yAxis->GetNbins() != 1) {
        address[1] = yAxis->FindBin(y);
        BadBinCheck(address[1], yAxis->GetNbins());
        cout << "for y, " << x << " address[1] is " << address[1] << endl;
    }
    zAxis = hist->GetZaxis();
    if (zAxis->GetNbins() != 1) {
        address[2] = zAxis->FindBin(z);
        BadBinCheck(address[2], zAxis->GetNbins());
    }
}
inline void PassFailCuts(vector<double> * integrals, vector<double> * integralErrors, int * binAddreses, TH1 * hist) {
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
    cout << "numPass Cut " << numPassCut << endl;
    cout << "numFail Cut " << numFailCut << endl;
    integrals->push_back(numFailCut);
    integrals->push_back(numPassCut);
    integralErrors->push_back(numFailCutErr);
    integralErrors->push_back(numPassCutErr);
}
inline void PassFailCuts(vector<double> * integrals, vector<double> * integralErrors, int * binAddreses, TH2 * hist) {
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
    cout << "numPass Cut " << numPassCut << endl;
    cout << "numFail Cut " << numFailCut << endl;
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
    float barrelEtaEnd = 1.4442; float endcapEtaStart = 1.566;
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
    vector<double> integrals;
    vector<double> integralErrors;
    if (numVals > 1) {
        if (numVals > 2) {
            PassFailCuts(&integrals, &integralErrors, binAddreses, (TH3 *) inputHist);
        }
        else {
            PassFailCuts(&integrals, &integralErrors, binAddreses, (TH2 *) inputHist);
        }
    }
    else {
        PassFailCuts(&integrals, &integralErrors, binAddreses, inputHist);
    }
    outHist->SetBinContent(1, integrals[0]);
    outHist->SetBinError(1, integralErrors[0]);
    outHist->SetBinContent(2, integrals[1]);
    outHist->SetBinError(2, integralErrors[1]);
    outHist->GetXaxis()->SetBinLabel(1, "Failed Cut");
    outHist->GetXaxis()->SetBinLabel(2, "Passed Cut");
    return outHist;
}


inline TGraph LogLogGraph(TH1F * inputHist) {
    int nBins = inputHist->GetNbinsX();
    TGraph outGraph(nBins);
    float x, y;
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