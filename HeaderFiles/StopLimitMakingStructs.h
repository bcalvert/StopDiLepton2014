#include "TH2F.h"
#include "TLegend.h"
#include "TROOT.h"
//#include "TH2C.h"
#include <vector>
#include "LimitScriptHeaderFiles/LimitScripts_ParametersStruct.h"
#include "LimitScriptHeaderFiles/LimitScripts_SUSYStruct.h"
#include "TwoDeeGaussian.h"
#include "../LimitSetting/ContourMaking/LocalAverage.C"

#include "Math/QuantFuncMathCore.h"
/*
#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"
#include "RooStats/HypoTestResult.h"
#include "RooStats/RooStatsUtils.h"
*/

//Structures used when showing/making exclusion limit plots -- need to work on documentation

inline Double_t PValueToSignificance(Double_t pvalue){
    return ROOT::Math::normal_quantile_c(pvalue,1);
}

struct Point {
    float xCoord, yCoord;
};
struct Cluster {
    //struct to contain cluster of points used to make TGraph Contour limit plots
    bool isSorted;
    vector<Point> vecPoints;
    void Sort() {
        
    }
};
struct LimitPlotShower {
    //h_SigStrengthLimitHist: TH2F containing the calculated limit on signal strength
    //h_SigExclHist: Boolean version of h_SigStrengthLimitHist where I require the value to be < 1
    //h_XSecLimHist: TH2F containing the calculated limit on the cross-section;
    //h_SignifHist: TH2F containing the calculated significance of the excesses;
    //h_PValueHist: TH2F containing the calculated p-value of the null-hypothesis;
    TH2F * h_SigStrengthLimitHist, * h_SigExclHist, * h_XSecLimHist;
    TH2F * h_SignifHist, * h_PValueHist;
    vector<TH2F *> vecMT2CutHists;
    
    TGraph * limContour;
    
    TString nameLPS; //base name added into histograms
    
    int numMT2Hists;
    float GetXSecLimMax() {
        return h_XSecLimHist->GetMaximum();
    }
    TCanvas * DrawHist(int whichHist, bool doZeroNonExcl, int drawOption, TString addName = "") {
        TH2F * histToDraw = NULL;
        switch (whichHist) {
            case 0:
                histToDraw = h_SigStrengthLimitHist;
                break;
            case 1:
                histToDraw = h_XSecLimHist;
                break;
            case 2:
                histToDraw = h_XSecLimHist;
                break;
            default:
                break;
        }
        
        cout << "doZeroNonExcl " << doZeroNonExcl << endl;
        if (doZeroNonExcl && whichHist == 0) {
            cout << "doZeroNonExcl 2 " << doZeroNonExcl << endl;
            ZeroNonExclPoints(histToDraw, 1, false);
        }
        
        TString canvName = histToDraw->GetName();
        canvName.Replace(canvName.Index("h_"), 2, "c_");
        TCanvas * outCanv = new TCanvas(canvName, canvName);
        if (drawOption == 0) {
            histToDraw->Draw("colz");
        }
        else {
            outCanv->cd()->SetPhi(250);
            histToDraw->Draw("lego");
        }
        return outCanv;
    }
    void SetRangeUser(bool isStandard, int whichHist, float zLB = 0.0, float zUB = 1.5) {
        TH2F * histToSet = NULL;
        switch (whichHist) {
            case 0:
                histToSet = h_SigStrengthLimitHist;
                break;
            case 1:
                histToSet = h_SigExclHist;
                break;
            case 2:
                histToSet = h_XSecLimHist;
                break;
            default:
                break;
        }
        if (isStandard) histToSet->GetYaxis()->SetRangeUser(0, 400);
        histToSet->GetZaxis()->SetRangeUser(zLB, zUB);
    }
    void ReverseConstDeltaMHists() {
        TH2F * currReverseDeltaMHist;
        currReverseDeltaMHist = OutputHistReverseConstDeltM(h_SigStrengthLimitHist, 0);
        h_SigStrengthLimitHist = currReverseDeltaMHist;
        currReverseDeltaMHist = OutputHistReverseConstDeltM(h_XSecLimHist, 0);
        h_XSecLimHist = currReverseDeltaMHist;
        
        for (unsigned int iMT2 = 0; iMT2 < vecMT2CutHists.size(); ++iMT2) {
            currReverseDeltaMHist = OutputHistReverseConstDeltM(vecMT2CutHists[iMT2], 0);
            vecMT2CutHists[iMT2] = currReverseDeltaMHist;
        }
    }
    
    void MakeConstDeltaMHists_Vers2(int whichChange, TString addName = "") {
        /*
        TAxis * yAxis = h_SigStrengthLimitHist->GetYaxis();
        TAxis * xAxis = h_SigStrengthLimitHist->GetXaxis();
        typedef pair<int, int> axisRange;
        int minLSP = (int) (yAxis->GetBinCenter(1) + 0.5);
        int maxLSP = (int) (yAxis->GetBinCenter(h_SigStrengthLimitHist->GetNbinsY()) + 0.5);
        int minStop = (int) (xAxis->GetBinCenter(1) + 0.5);
         int maxStop = (int) (xAxis->GetBinCenter(h_SigStrengthLimitHist->GetNbinsX()) + 0.5);
         cout << "minLSP " << minLSP << endl;
         cout << "maxLSP " << maxLSP << endl;
         cout << "minStop " << minStop << endl;
         cout << "maxStop " << maxStop << endl;
         
         axisRange axLSP(minLSP, maxLSP);
         axisRange axStop(minStop, maxStop);
         */
        axisRange axLSP(0, 400);
        axisRange axStop(100, 800);
        TH2F * currConstDeltaMHist;
        if (h_SigStrengthLimitHist) {
            currConstDeltaMHist = OutputHistDeltaMChanges(h_SigStrengthLimitHist, 0, whichChange, &axLSP, &axStop, TString("_Test") + addName);
            h_SigStrengthLimitHist = currConstDeltaMHist;
        }
        if (h_XSecLimHist) {
            currConstDeltaMHist = OutputHistDeltaMChanges(h_XSecLimHist, 0, whichChange, &axLSP, &axStop, TString("_Test") + addName);
            h_XSecLimHist = currConstDeltaMHist;
        }
        
        for (unsigned int iMT2 = 0; iMT2 < vecMT2CutHists.size(); ++iMT2) {
            if (vecMT2CutHists[iMT2]) {
                currConstDeltaMHist = OutputHistDeltaMChanges(vecMT2CutHists[iMT2], 0, whichChange, &axLSP, &axStop, TString("_Test") + addName);
                vecMT2CutHists[iMT2] = currConstDeltaMHist;
            }
        }
        
        if (h_SignifHist) {
            currConstDeltaMHist = OutputHistDeltaMChanges(h_SignifHist, 0, whichChange, &axLSP, &axStop, TString("_Test") + addName);
            h_SignifHist = currConstDeltaMHist;
        }
        
        if (h_PValueHist) {
            currConstDeltaMHist = OutputHistDeltaMChanges(h_PValueHist, 0, whichChange, &axLSP, &axStop, TString("_Test") + addName);
            h_PValueHist = currConstDeltaMHist;
        }
    }
    void MakeConstDeltaMHists(TString addName) {
        TH2F * currConstDeltaMHist;
        
        if (h_SigStrengthLimitHist) {
            currConstDeltaMHist = OutputHistConstDeltM(h_SigStrengthLimitHist, addName, 0);
            h_SigStrengthLimitHist = currConstDeltaMHist;
        }
        
        if (h_XSecLimHist) {
            currConstDeltaMHist = OutputHistConstDeltM(h_XSecLimHist, addName, 0);
            h_XSecLimHist = currConstDeltaMHist;
        }
        
        for (unsigned int iMT2 = 0; iMT2 < vecMT2CutHists.size(); ++iMT2) {
            if (vecMT2CutHists[iMT2]) {
                currConstDeltaMHist = OutputHistConstDeltM(vecMT2CutHists[iMT2], addName, 0);
                vecMT2CutHists[iMT2] = currConstDeltaMHist;
            }
        }
        
        if (h_SignifHist) {
            currConstDeltaMHist = OutputHistConstDeltM(h_SignifHist, addName, 0);
            h_SignifHist = currConstDeltaMHist;
        }
        
        if (h_PValueHist) {
            currConstDeltaMHist = OutputHistConstDeltM(h_PValueHist, addName, 0);
            h_PValueHist = currConstDeltaMHist;
        }
    }
    
    void DrawLimitAlongDeltaM(bool doLog) {
        TString baseCanvName = TString("c_LimConstDeltaM_") + nameLPS + TString("_DeltM");
        vector<TCanvas *> vecCanv;
        vector<TGraphErrors *> vecGraphs;
        TString currCanvName;
        int nBinsX = h_SigStrengthLimitHist->GetNbinsX();
        vecCanv.resize(nBinsX);
        vecGraphs.resize(nBinsX);
        float currMassStop;
        for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
            currMassStop = h_SigStrengthLimitHist->GetXaxis()->GetBinCenter(iBinX);
            currCanvName = baseCanvName;
            currCanvName += h_SigStrengthLimitHist->GetXaxis()->GetBinCenter(iBinX);
            vecCanv[iBinX - 1] = new TCanvas(currCanvName, currCanvName, 800, 800);
            vecGraphs[iBinX - 1] = GrabDeltaMLimit(h_SigStrengthLimitHist, currMassStop, doLog);
            vecGraphs[iBinX - 1]->Draw("AP");
        }
    }
    void SetOutContour(TString nameContour, bool doVerb = 0) {
        if (doVerb) {
            cout << "going to set out contour for nameContour = " << nameContour << endl;
        }
        float limThresh = 1E-6;
        
        TString tempName = h_SigStrengthLimitHist->GetName();
        tempName += "_TempClone";
        
        int nBinsX = h_SigStrengthLimitHist->GetNbinsX();
        int nBinsY = h_SigStrengthLimitHist->GetNbinsY();
        float lowStop = h_SigStrengthLimitHist->GetXaxis()->GetXmin();
        float highStop = h_SigStrengthLimitHist->GetXaxis()->GetXmax();
        float lowLSP = h_SigStrengthLimitHist->GetYaxis()->GetXmin();
        float highLSP = h_SigStrengthLimitHist->GetYaxis()->GetXmax();
        
        TH2F * tempHist = new TH2F(tempName, "", nBinsX, lowStop, highStop, nBinsY, lowLSP, highLSP);
        
        for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
            float currStopMass = h_SigStrengthLimitHist->GetXaxis()->GetBinCenter(iBinX);
            for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
                float currLSPMass = h_SigStrengthLimitHist->GetYaxis()->GetBinCenter(iBinY);
                float limit = h_SigStrengthLimitHist->GetBinContent(iBinX, iBinY);
                if (limit < limThresh) continue;
                tempHist->Fill(currStopMass, currLSPMass, 1./limit);
            }
        }
        
        TCanvas* c = new TCanvas(TString("c_") + tempName,"Contour List",0,0,600,600);
        
        tempHist->SetContour(3);
        tempHist->SetContourLevel(0, 0); //value for your first level
        tempHist->SetContourLevel(1, 1.); //non-existing high level
        tempHist->SetContourLevel(2, 1e9); //non-existing high level
        tempHist->Draw("CONT Z LIST");
        c->Update();
        
        TObjArray *conts = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
        TList* contLevel = NULL;
        TGraph* curv     = NULL;
        TGraph* gc       = NULL;
        Int_t TotalConts = conts->GetSize();
        Double_t x0, y0, z0;
        
        for(int i = 0; i < TotalConts; i++){
            contLevel = (TList*)conts->At(i);
            curv = (TGraph*)contLevel->First();
            for(int j = 0; j < contLevel->GetSize(); j++){
                curv->GetPoint(0, x0, y0);
                limContour = (TGraph*)curv->Clone();
            }
        }
        if (limContour) {
            limContour->SetName(nameContour);
        }
        
        delete tempHist;
    }

    void SetSigExclHists(bool doVerb = 0) {
        h_SigExclHist = (TH2F *) h_SigStrengthLimitHist->Clone(h_SigStrengthLimitHist->GetName() + TString("_Excl"));
        SetHistExclusion_2DSUSY(h_SigExclHist, doVerb);
    }
    void ScaleSigStrengthHist(TH1F * inputHistXSection, int whichDir) {
        int nBinsX = h_SigStrengthLimitHist->GetNbinsX();
        int nBinsY = h_SigStrengthLimitHist->GetNbinsY();
        
        float massStop;
        float currXSection, currXSectionErr;
        float currBinContent;
        for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
            massStop = h_SigStrengthLimitHist->GetXaxis()->GetBinCenter(iBinX);
            currXSection = inputHistXSection->GetBinContent(inputHistXSection->GetXaxis()->FindBin(massStop));
            currXSectionErr = inputHistXSection->GetBinError(inputHistXSection->GetXaxis()->FindBin(massStop));
            for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
                currBinContent = h_SigStrengthLimitHist->GetBinContent(iBinX, iBinY);
                currBinContent *= currXSection;
                if (whichDir < 0) {
                    currBinContent /= (currXSection + currXSectionErr);
                }
                else if (whichDir > 0) {
                    currBinContent /= (currXSection - currXSectionErr);
                }
                h_SigStrengthLimitHist->SetBinContent(iBinX, iBinY, currBinContent);
            }
        }
    }
    TString GetHistName(int whichHist) {
        TString baseHistName = "h_SigStrengthLimitHist";
        baseHistName += nameLPS;
        if (whichHist == 1) {
            return baseHistName + TString("_Excl");
        }
        else if (whichHist == 2) {
            return baseHistName + TString("_XSecLimit");
        }
        else if (whichHist == 3) {
            baseHistName = "h_Signif";
            baseHistName += nameLPS;
            return baseHistName;
        }
        else if (whichHist == 4) {
            baseHistName = "h_PValue";
            baseHistName += nameLPS;
            return baseHistName;
        }
        return baseHistName;
    }
    
    void LoadSignifHists(TFile * inputFile, TString addName = "", bool doVerb = false) {
        TString nameSignifHist = GetHistName(3) + addName;
        h_SignifHist = (TH2F *) inputFile->Get(nameSignifHist);
        if (doVerb) {
            cout << "tried to load " << nameSignifHist << "? " << (h_SignifHist != NULL) << endl;
        }
        TString namePValueHist = GetHistName(4) + addName;
        h_PValueHist = (TH2F *) inputFile->Get(namePValueHist);
        if (doVerb) {
            cout << "tried to load " << namePValueHist << "? " << (h_PValueHist != NULL) << endl;
        }
    }
    
    void LoadHists(TFile * inputFile, bool getNoTruncate, TString addName = "", bool doVerb = false) {
        TString nameSigStrengthLimitHist = GetHistName(0) + addName;
        if (getNoTruncate) nameSigStrengthLimitHist += "_NoTruncation";
        h_SigStrengthLimitHist = (TH2F *) inputFile->Get(nameSigStrengthLimitHist);
        if (doVerb) {
            cout << "tried to load " << nameSigStrengthLimitHist << "? " << (h_SigStrengthLimitHist != NULL) << endl;
        }
        TString nameSigExclHist = GetHistName(1) + addName;
//        if (getNoTruncate) nameSigExclHist += "_NoTruncation";
        h_SigExclHist = (TH2F *) inputFile->Get(nameSigExclHist);
        if (doVerb) {
            cout << "tried to load " << nameSigExclHist << "? " << (h_SigExclHist != NULL) << endl;
        }
        TString nameXSecLimHist = GetHistName(2) + addName;
//        if (getNoTruncate) nameXSecLimHist += "_NoTruncation";
        h_XSecLimHist = (TH2F *) inputFile->Get(nameXSecLimHist);
        if (doVerb) {
            cout << "tried to load " << nameXSecLimHist << "? " << (h_XSecLimHist != NULL) << endl;
        }
    }
    
    void SetHistNamesForSaving(TString addName = "") {
        if (h_SigStrengthLimitHist) h_SigStrengthLimitHist->SetName(GetHistName(0) + addName);
        if (h_SigExclHist) h_SigExclHist->SetName(GetHistName(1) + addName);
        if (h_XSecLimHist) h_XSecLimHist->SetName(GetHistName(2) + addName);
        if (h_SignifHist) h_SignifHist->SetName(GetHistName(3) + addName);
        if (h_PValueHist) h_PValueHist->SetName(GetHistName(4) + addName);
    }
    
    void SetSigExclHists(bool doVerb = 0) {
        TString nameSigExcl = GetHistName(1);
//        h_SigExclHist = (TH2F *) h_SigStrengthLimitHist->Clone(h_SigStrengthLimitHist->GetName() + TString("_Excl"));
        h_SigExclHist = (TH2F *) h_SigStrengthLimitHist->Clone(nameSigExcl);
        SetHistExclusion_2DSUSY(h_SigExclHist, doVerb);
    }
    void SetSignifHists(TString inputFileSignif, TString inputFilePValue, int binSize, bool doVerb = 0) {
        TString addHistNameSigStrength[3] = {"", "OneSigUp", "OneSigDown"};
        if (doVerb) {
            cout << "nameLPS " << nameLPS << endl;
        }
        
        TString baseHistName = GetHistName(4);
        SignifHist(h_PValueHist, baseHistName, inputFilePValue, NULL, binSize, doVerb);
        h_PValueHist->GetYaxis()->SetRangeUser(0 - (binSize * 0.5), 400);
        baseHistName = GetHistName(3);
        SignifHist(h_SignifHist, baseHistName, inputFileSignif, h_PValueHist, binSize, doVerb);
        h_SignifHist->GetYaxis()->SetRangeUser(0 - (binSize * 0.5), 400);
    }
    void SetSignifFromPValue() {
        int nBinsX = h_PValueHist->GetNbinsX();
        int nBinsY = h_PValueHist->GetNbinsY();
        
        float currPValue, currSignif;
        float currPValueUp;
        for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
            for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
                if (iBinX < iBinY) continue;
                currPValue = h_PValueHist->GetBinContent(iBinX, iBinY);
                currPValueUp = currPValue - h_PValueHist->GetBinError(iBinX, iBinY);
                currSignif = PValueToSignificance(currPValue);
                h_SignifHist->SetBinContent(iBinX, iBinY, currSignif);
                h_SignifHist->SetBinError(iBinX, iBinY, PValueToSignificance(currPValueUp));
            }
        }
    }
    void SetHists(TString inputFile, int binSize, bool doVerb = 0) {
        numMT2Hists = 2;
        
        TString addHistNameSigStrength[3] = {"", "OneSigUp", "OneSigDown"};
        TString addHistNameMT2[2] = {"_BestMT2llCut", "_BestMT2lblbCut"};
        vector<TString> vecHistNames_MT2Cut(0);
        
        if (doVerb) {
            cout << "nameLPS " << nameLPS << endl;
        }
        
        TString baseHistName = GetHistName(0);  // "h_SigStrengthLimitHist";
        //baseHistName += nameLPS;
        
        for (int iHist = 0; iHist < 2; ++iHist) {
            vecHistNames_MT2Cut.push_back(baseHistName + addHistNameMT2[iHist]);
        }
        LimitHist(h_SigStrengthLimitHist, &vecMT2CutHists, inputFile, baseHistName, &vecHistNames_MT2Cut, binSize, doVerb);
        h_SigStrengthLimitHist->GetYaxis()->SetRangeUser(0 - binSize * 0.5, 400);
    }
    
    void MakeXSectionLimitHists(TH1F * histXSection, TString addName = "") {
        TString nameSigXSec = GetHistName(2);
//        h_XSecLimHist = (TH2F *) h_SigStrengthLimitHist->Clone(h_SigStrengthLimitHist->GetName() + TString("_XSecLimit"));
        h_XSecLimHist = (TH2F *) h_SigStrengthLimitHist->Clone(nameSigXSec + addName);
        
        int nBinsX = h_XSecLimHist->GetNbinsX();
        int nBinsY = h_XSecLimHist->GetNbinsY();
        float massStop, currXSection, currSigStrengthLim;
        for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
            massStop = h_SigStrengthLimitHist->GetXaxis()->GetBinCenter(iBinX);
            currXSection = histXSection->GetBinContent(histXSection->GetXaxis()->FindBin(massStop));
            for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
                currSigStrengthLim = h_SigStrengthLimitHist->GetBinContent(iBinX, iBinY);
                currSigStrengthLim *= currXSection;
                h_XSecLimHist->SetBinContent(iBinX, iBinY, currSigStrengthLim);
            }
        }
    }
    void MakeLimitStrengthHist(TH1F * histXSection, TString addName) {
        h_SigStrengthLimitHist->SetName(h_SigStrengthLimitHist->GetName() + addName);
        int nBinsX = h_XSecLimHist->GetNbinsX();
        int nBinsY = h_XSecLimHist->GetNbinsY();
        float massStop, currXSection, currXSecLim, currSigStrengthLim;
        for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
            massStop = h_SigStrengthLimitHist->GetXaxis()->GetBinCenter(iBinX);
            currXSection = histXSection->GetBinContent(histXSection->GetXaxis()->FindBin(massStop));
            for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
                //NonConstDeltaM first
                currXSecLim = h_XSecLimHist->GetBinContent(iBinX, iBinY);
                currSigStrengthLim = currXSecLim / currXSection;
                h_SigStrengthLimitHist->SetBinContent(iBinX, iBinY, currSigStrengthLim);
            }
        }
    }
    void DrawMT2Hists(vector<TCanvas *> * inVecCanvases, TString canvNameAppend, TString outPath) {
        unsigned int numCanv = inVecCanvases->size();
        
        if ((int) numCanv != numMT2Hists) {
            cout << "numCanv doesn't match numMT2Hists!" << endl;
        }
        gStyle->SetPaintTextFormat("4.0f");
        for (int iHist = 0; iHist < numMT2Hists; ++iHist) {
            SetStyleHist(vecMT2CutHists[iHist], -1);
        }
        for (unsigned int iCanv = 0; iCanv < numCanv; ++iCanv) {
            inVecCanvases->at(iCanv)->cd();
            vecMT2CutHists[iCanv]->GetZaxis()->SetRangeUser(80, 120);
            vecMT2CutHists[iCanv]->Draw("colz text");
            inVecCanvases->at(iCanv)->SaveAs(outPath + inVecCanvases->at(iCanv)->GetName() + canvNameAppend);
        }
    }
    void DrawSigHists(TString canvName, TString canvNameAppend, TString outPath, int whichHist) {
        //whichHist: 1 -- draw SigStrength, 2 -- draw SigExcl, 3 -- draw XSec Limit
        TCanvas * outCanv = new TCanvas(canvName, canvName, 1600, 1200);
        switch (whichHist) {
            case 1:
                float maxSigStrength;
                maxSigStrength = h_SigStrengthLimitHist->GetMaximum();
                maxSigStrength = TMath::Min((float) 25, maxSigStrength);
                h_SigStrengthLimitHist->GetZaxis()->SetRangeUser(0.0, maxSigStrength);
                h_SigStrengthLimitHist->Draw("colz");
                break;
            case 2:
                h_SigExclHist->SetContour(2);
                h_SigExclHist->Draw("cont4 list");
                break;
            case 3:
                h_XSecLimHist->Draw("colz");
                break;
        }
        outCanv->SaveAs(outPath + outCanv->GetName() + canvNameAppend);
    }
    void WriteSignifToFile(TFile * outFile, bool doVerb = false) {
        outFile->cd();
        TH2F * histToWrite = h_SignifHist;
        if (histToWrite != NULL) {
            if (doVerb) {
                cout << "histToWrite name " << histToWrite->GetName() << endl;
            }
            histToWrite->Write();
        }
        else {
            cout << "histToWrite is NULL!" << endl;
        }
        histToWrite = h_PValueHist;
        if (histToWrite != NULL) {
            if (doVerb) {
                cout << "histToWrite name " << histToWrite->GetName() << endl;
            }
            histToWrite->Write();
        }
        else {
            cout << "histToWrite is NULL!" << endl;
        }
    }
    void WriteToFile(TFile * outFile, int whichHist, bool createSigExclStuff = false, bool doVerb = false) {
        outFile->cd();
        TH2F * histToWrite;
        if (whichHist < 4) {
            switch (whichHist) {
                case 1:
                    histToWrite = h_SigStrengthLimitHist;
                    if (createSigExclStuff) {
                        TH2F * h_SigStrengthLimitHistNoTruncate = (TH2F *) histToWrite->Clone(histToWrite->GetName() + TString("_NoTruncation"));
                        ZeroNonExclPoints(histToWrite, 1, false);
                        histToWrite->GetZaxis()->SetRangeUser(0, 1.0);
                        
                        TH2F * h_SigStrengthLimitHist_StatErrShiftUp = ExclConsistencyHist(histToWrite, 1);
                        TH2F * h_SigStrengthLimitHist_StatErrShiftDown = ExclConsistencyHist(histToWrite, -1);
                        //                        h_SigStrengthLimitHist_StatErrShiftUp->Write();
                        //                        h_SigStrengthLimitHist_StatErrShiftDown->Write();
                    }
                    break;
                case 2:
                    histToWrite = h_SigExclHist;
                    break;
                case 3:
                    histToWrite = h_XSecLimHist;
                    break;
                default:
                    break;
            }
            
            if (histToWrite != NULL) {
                if (doVerb) {
                    cout << "whichHist " << whichHist << endl;
                    cout << "histToWrite name " << histToWrite->GetName() << endl;
                }
                histToWrite->Write();
            }
            else {
                cout << "histToWrite is NULL!" << endl;
            }
        }
        else {
            if (limContour) {
                limContour->Write();
            }
            else {
                cout << "no limContour! " << endl;
            }
        }
    }
};

struct LimitPlotComparer {
    //h_LimitRatioHist: TH2F containing the ratio of limits on signal strength
    //Note this doesn't make sense unless it is specifically made in reference to another LimitPlotShower
    //The appends, "All", "BothExcl", and "SupExcl" refer to which points make it into the histogram
    //"All" -- all points
    //"BothExcl" -- only points that are excluded by both LPSs
    //"SupExcl" -- only points that are excluded by the "superior" LPS
    //"SubExcl" -- only points that are excluded by the "sub" LPS
    TH2F * h_LimitRatioHistAll;
    TH2F * h_LimitRatioHistBothExcl;
    TH2F * h_LimitRatioHistSupExcl;
    TH2F * h_LimitRatioHistSubExcl;
    
    TH2F * h_LimitRatioConsistencyHistAll;
    TH2F * h_LimitRatioConsistencyHistBothExcl;
    TH2F * h_LimitRatioConsistencyHistSupExcl;
    TH2F * h_LimitRatioConsistencyHistSubExcl;
    
    
    TString nameLPC;
    
    TString nameDispAna1, nameDispAna2;
    
    
    TString baseDirSuper, baseDirSub;
    TString fileNameSuper, fileNameSub;
    
    LimitPlotShower LPS_Super, LPS_Sub;
    
    bool showMT2;
    
    void DefaultVarVals() {
        showMT2 = 0;
        baseDirSuper = TString("CombinedLimits/");
        baseDirSub = TString("CombinedLimits/");
    }
    
    void SetFileNames(LimitParametersContainer * inLPCSuper, LimitParametersContainer * inLPCSub, SUSYT2LimitParams * inSUSYT2LPSuper, SUSYT2LimitParams * inSUSYT2LPSub, bool doVerb = 0) {
        TString addName = "";
        if (showMT2) addName = "Best";
        TString basePathCardSuper = BaseStringLimit_NameOrDir(inLPCSuper, inSUSYT2LPSuper, 0) + baseDirSuper;
        TString baseNameCardSuper = BaseStringLimit_NameOrDir(inLPCSuper, inSUSYT2LPSuper, 2);
        fileNameSuper = basePathCardSuper + TString("Combined") + addName + baseNameCardSuper + TString(".txt");
        if (doVerb) {
            cout << "fileNameSuper " << fileNameSuper << endl;
        }
        LPS_Super.nameLPS = "LPS_Super" + baseNameCardSuper;
        
        TString basePathCardSub = BaseStringLimit_NameOrDir(inLPCSub, inSUSYT2LPSub, 0) + baseDirSub;
        TString baseNameCardSub = BaseStringLimit_NameOrDir(inLPCSub, inSUSYT2LPSub, 2);
        fileNameSub = basePathCardSub + TString("Combined") + addName + baseNameCardSub + TString(".txt");
        if (doVerb) {
            cout << "fileNameSub " << fileNameSub << endl;
        }
        LPS_Sub.nameLPS = "LPS_Sub" + baseNameCardSub;
    }
    void SetHists(SUSYMassLimitParams * inSUSYMLP, bool doVerb = 0) {
        LPS_Super.SetHists(fileNameSuper, inSUSYMLP->binSize, doVerb);
        LPS_Sub.SetHists(fileNameSub, inSUSYMLP->binSize, doVerb);
    }
    
    void SetRatioHistVecs(bool doVerb = 0) {
        nameLPC = LPS_Sub.nameLPS + TString("Div") + LPS_Super.nameLPS;
        TString nameCompare = "h_Ratio";
        nameCompare += nameLPC;
        
        h_LimitRatioHistAll = LimitRatioHist_vers2(LPS_Super.h_SigStrengthLimitHist, LPS_Sub.h_SigStrengthLimitHist, "", false, -1, doVerb);
        h_LimitRatioHistBothExcl = LimitRatioHist_vers2(LPS_Super.h_SigStrengthLimitHist, LPS_Sub.h_SigStrengthLimitHist, "_BothExcl", false, 0, doVerb);
        h_LimitRatioHistSupExcl = LimitRatioHist_vers2(LPS_Super.h_SigStrengthLimitHist, LPS_Sub.h_SigStrengthLimitHist, "_OnlySuperExcl", false, 1, doVerb);
        h_LimitRatioHistSubExcl = LimitRatioHist_vers2(LPS_Super.h_SigStrengthLimitHist, LPS_Sub.h_SigStrengthLimitHist, "_OnlySubExcl", false, 2, doVerb);
        
        h_LimitRatioConsistencyHistAll = LimitRatioHist_vers2(LPS_Super.h_SigStrengthLimitHist, LPS_Sub.h_SigStrengthLimitHist, "", true, -1, doVerb);
        h_LimitRatioConsistencyHistBothExcl = LimitRatioHist_vers2(LPS_Super.h_SigStrengthLimitHist, LPS_Sub.h_SigStrengthLimitHist, "_BothExcl", true, 0, doVerb);
        h_LimitRatioConsistencyHistSupExcl = LimitRatioHist_vers2(LPS_Super.h_SigStrengthLimitHist, LPS_Sub.h_SigStrengthLimitHist, "_OnlySuperExcl", true, 1, doVerb);
        h_LimitRatioConsistencyHistSubExcl = LimitRatioHist_vers2(LPS_Super.h_SigStrengthLimitHist, LPS_Sub.h_SigStrengthLimitHist, "_OnlySubExcl", true, 2, doVerb);
    }
    
    void DrawRatioHists(TString canvName, TString canvNameAppend, TString outPath, int whichHist, bool doVerb = 0) {
        //whichHist: 1 -- Ratio CV hist, 2 -- Ratio Consistency Hist
        TCanvas * outCanv = new TCanvas(canvName, canvName, 1600, 1200);
        gStyle->SetPaintTextFormat("4.2f");
        float maxRatio;
        
        TH2F * h_LimRatioToUse_All, * h_LimRatioToUse_BothExcl, * h_LimRatioToUse_SupExcl, * h_LimRatioToUse_SubExcl;
        
        if (whichHist == 1) {
            h_LimRatioToUse_All = h_LimitRatioHistAll;
            h_LimRatioToUse_BothExcl = h_LimitRatioHistBothExcl;
            h_LimRatioToUse_SupExcl = h_LimitRatioHistSupExcl;
            h_LimRatioToUse_SubExcl = h_LimitRatioHistSubExcl;
        }
        else if (whichHist == 2) {
            h_LimRatioToUse_All = h_LimitRatioConsistencyHistAll;
            h_LimRatioToUse_BothExcl = h_LimitRatioConsistencyHistBothExcl;
            h_LimRatioToUse_SupExcl = h_LimitRatioConsistencyHistSupExcl;
            h_LimRatioToUse_SubExcl = h_LimitRatioConsistencyHistSubExcl;
        }
        SetStyleHist(h_LimRatioToUse_All, -1);
        SetStyleHist(h_LimRatioToUse_BothExcl, 0);
        SetStyleHist(h_LimRatioToUse_SupExcl, 1);
        SetStyleHist(h_LimRatioToUse_SubExcl, 2);
        
        if (doVerb) {
            cout << "About to draw the ratio hists " << endl;
        }
        maxRatio = h_LimRatioToUse_All->GetMaximum();
        maxRatio = TMath::Min((float)100, maxRatio);
        h_LimRatioToUse_All->GetZaxis()->SetRangeUser(0.0, maxRatio);
        h_LimRatioToUse_BothExcl->GetZaxis()->SetRangeUser(0.0, maxRatio);
        h_LimRatioToUse_SupExcl->GetZaxis()->SetRangeUser(0.0, maxRatio);
        h_LimRatioToUse_SubExcl->GetZaxis()->SetRangeUser(0.0, maxRatio);
        /*
         switch (whichHist) {
         case 1:
         maxRatio = h_LimRatioToUse_All->GetMaximum();
         maxRatio = TMath::Min((float)100, maxRatio);
         h_LimRatioToUse_All->GetZaxis()->SetRangeUser(0.0, maxRatio);
         h_LimRatioToUse_BothExcl->GetZaxis()->SetRangeUser(0.0, maxRatio);
         h_LimRatioToUse_SupExcl->GetZaxis()->SetRangeUser(0.0, maxRatio);
         h_LimRatioToUse_SubExcl->GetZaxis()->SetRangeUser(0.0, maxRatio);
         break;
         case 2:
         maxRatio = h_LimRatioToUse_All->GetMaximum();
         maxRatio = TMath::Min((float)100, maxRatio);
         h_LimRatioToUse_All->SetMinimum(0.0);
         h_LimRatioToUse_All->Print();
         cout << "h_LimRatioToUse_All minimum " << h_LimRatioToUse_All->GetMinimum() << endl;
         h_LimRatioToUse_BothExcl->SetMinimum(0.0);
         h_LimRatioToUse_SupExcl->SetMinimum(0.0);
         h_LimRatioToUse_SubExcl->SetMinimum(0.0);
         
         break;
         }
         */
        
        SetBlackPlot(outCanv, h_LimRatioToUse_All);
        h_LimRatioToUse_All->Draw("text");
        h_LimRatioToUse_BothExcl->Draw("text same");
        h_LimRatioToUse_SupExcl->Draw("text same");
        h_LimRatioToUse_SubExcl->Draw("text same");
        /*
         if (whichHist == 1) {
         h_LimRatioToUse_All->Draw("text");
         h_LimRatioToUse_BothExcl->Draw("text same");
         h_LimRatioToUse_SupExcl->Draw("text same");
         h_LimRatioToUse_SubExcl->Draw("text same");
         }
         
         else {
         h_LimRatioToUse_All->Draw("colz text");
         h_LimRatioToUse_BothExcl->Draw("colz text same");
         h_LimRatioToUse_SupExcl->Draw("colz text same");
         h_LimRatioToUse_SubExcl->Draw("colz text same");
         }
         */
        TLatex drawText;
        drawText.SetNDC();
        drawText.SetTextColor(kWhite);
        TString textToDraw = "#frac{UB #sigma(";
        textToDraw += nameDispAna1;
        textToDraw += ")}{UB #sigma(";
        textToDraw += nameDispAna2;
        textToDraw += ")}";
        outCanv->cd();
        drawText.DrawLatex(0.2, 0.75, textToDraw);
        //stuff for legend
        TString titleNeither = "Neither Excludes";
        TString titleBoth = "Both Exclude";
        TString titleAna1 = nameDispAna1;
        titleAna1 += " Excludes";
        TString titleAna2 = nameDispAna2;
        titleAna2 += " Excludes";
        cout << "titleAna1 " << titleAna1 << endl;
        cout << "titleAna2 " << titleAna2 << endl;
        TLegend legRatio(0.15, 0.5, 0.4, 0.7);
        legRatio.SetX1NDC(0.15);
        legRatio.SetX2NDC(0.4);
        legRatio.SetY1NDC(0.5);
        legRatio.SetY2NDC(0.7);
        legRatio.SetTextColor(kWhite);
        legRatio.AddEntry(h_LimRatioToUse_All, titleNeither, "f");
        legRatio.AddEntry(h_LimRatioToUse_BothExcl, titleBoth, "f");
        legRatio.AddEntry(h_LimRatioToUse_SupExcl, titleAna1, "f");
        legRatio.AddEntry(h_LimRatioToUse_SubExcl, titleAna2, "f");
        outCanv->cd();
        legRatio.Draw("same");
        outCanv->SaveAs(outPath + outCanv->GetName() + canvNameAppend);
    }
    
    
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        for (int iLPCParam = 0; iLPCParam < argc; ++iLPCParam) {
            if (strncmp (argv[iLPCParam],"nameTopAna", 10) == 0) {
                nameDispAna1 = TString(argv[iLPCParam + 1]);
            }
            else if (strncmp (argv[iLPCParam],"nameBotAna", 10) == 0) {
                nameDispAna2 = TString(argv[iLPCParam + 1]);
            }
            else if (strncmp (argv[iLPCParam],"SM", 2) == 0) {
                showMT2 = 1;
            }
            else if (strncmp (argv[iLPCParam],"-supDir", 7) == 0) {
                baseDirSuper = TString(argv[iLPCParam + 1]);
            }
            else if (strncmp (argv[iLPCParam],"-subDir", 7) == 0) {
                baseDirSub = TString(argv[iLPCParam + 1]);
            }
        }
    }
};

struct SignifScenarioContainer {
    TFile * outFile;
    TString addName;
    
    TString basePathSignif;
    
    TString nameOutFile;
    TString nameOutPath;
    
    bool doZeroNonExcl;
    
    vector<TString> vecInputSignifFileNames_Exp;
    vector<TString> vecInputPValueFileNames_Exp;
    TString inputSignifFileName_Obs;
    TString inputPValueFileName_Obs;
    
    vector<TString> vecGraphNames_Exp;
    vector<TString> vecGraphNames_Obs;
    
    vector<LimitPlotShower> vecLPSExpected;
    vector<LimitPlotShower> vecLPSObserved;
    
    unsigned int numLPSExp, numLPSObs;
    
    TCanvas * DrawHist(bool doObs, int whichSig, int whichHist, int drawOption, TString secondAddName = "") {
        TCanvas * outCanv;
        if (doObs) {
            outCanv = vecLPSObserved[whichSig].DrawHist(whichHist, doZeroNonExcl, drawOption, addName + secondAddName);
        }
        else {
            outCanv = vecLPSExpected[whichSig].DrawHist(whichHist, doZeroNonExcl, drawOption, addName + secondAddName);
        }
        return outCanv;
    }
    void DefaultVarVals(LimitParametersContainer * inLPC, SUSYT2LimitParams * inSUSYT2LP) {
        
        basePathSignif = "CombinedSignifs/";
        nameOutPath = "ContourFiles/";
        nameOutFile = "h_Signif";
        
        addName = "";
        
        numLPSExp = 3;
        numLPSObs = 1;
        
        vecLPSExpected.resize(numLPSExp);
        vecLPSObserved.resize(numLPSObs);
        vecInputSignifFileNames_Exp.resize(numLPSExp);
        vecInputPValueFileNames_Exp.resize(numLPSExp);
        
        TString arrStringGraphNameEnd[3] = {"", "M", "P"};
        vecGraphNames_Exp.resize(numLPSExp);
        vecGraphNames_Obs.resize(numLPSObs);
        TString baseNameGraph = "graph_smoothed_";
        for (unsigned int iSigExp = 0; iSigExp < numLPSExp; ++iSigExp) {
            vecGraphNames_Exp[iSigExp] = baseNameGraph + TString("Exp") + arrStringGraphNameEnd[iSigExp];
        }
        for (unsigned int iSigObs = 0; iSigObs < numLPSObs; ++iSigObs) {
            vecGraphNames_Obs[iSigObs] = baseNameGraph + TString("Obs") + arrStringGraphNameEnd[iSigObs];
        }
        
        
        TString baseNameLPS_Exp = "_LPS_Exp";
        TString baseNameLPS_Obs = "_LPS_Obs";
        TString addNameLPS[3] = {"", "_OneSigUp", "_OneSigDown"};
        for (unsigned int iSigExp = 0; iSigExp < numLPSExp; ++iSigExp) {
            vecLPSExpected[iSigExp].nameLPS = baseNameLPS_Exp + addNameLPS[iSigExp];
        }
        for (unsigned int iSigObs = 0; iSigObs < numLPSObs; ++iSigObs) {
            vecLPSObserved[iSigObs].nameLPS = baseNameLPS_Obs + addNameLPS[iSigObs];
        }
        
        vector<TString> vecBaseNameSignifCard(4);
        vector<TString> vecBaseNamePValueCard(4);
        vector<LimitParametersContainer> vecLPC(4);
        for (unsigned int iLPC = 0; iLPC < vecLPC.size(); ++iLPC) {
            if (iLPC > 0) {
                vecLPC[iLPC] = vecLPC[0];
            }
            else {
                vecLPC[iLPC] = *inLPC;
            }
            if (inLPC->useAsymLims && inLPC->isSignificance && iLPC > 0) {
	      cout << "hack" << endl;
                vecLPC[iLPC].isExpLim = 1;
            }
            else {
                vecLPC[iLPC].isExpLim = iLPC;
            }
            vecLPC[iLPC].SetStrings();
            
            vecBaseNameSignifCard[iLPC] = BaseStringLimit_NameOrDir(&vecLPC[iLPC], inSUSYT2LP, 2);
            vecBaseNamePValueCard[iLPC] = BaseStringLimit_NameOrDir(&vecLPC[iLPC], inSUSYT2LP, 2);
            vecBaseNamePValueCard[iLPC].Replace(vecBaseNamePValueCard[iLPC].Index("Signif"), 6, "PValue");
        }
        TString basePathSignifCard = BaseStringLimit_NameOrDir(inLPC, inSUSYT2LP, 0) + basePathSignif;
        
        inputSignifFileName_Obs = basePathSignifCard + TString("Combined") + vecBaseNameSignifCard[0] + TString(".txt");
        inputPValueFileName_Obs = basePathSignifCard + TString("Combined") + vecBaseNamePValueCard[0] + TString(".txt");
        for (unsigned int iSigExp = 0; iSigExp < numLPSExp; ++iSigExp) {
            vecInputSignifFileNames_Exp[iSigExp] = basePathSignifCard + TString("Combined") + vecBaseNameSignifCard[iSigExp + 1] + TString(".txt");
            vecInputPValueFileNames_Exp[iSigExp] = basePathSignifCard + TString("Combined") + vecBaseNamePValueCard[iSigExp + 1] + TString(".txt");
        }
    }
    void SetOutfile(LimitParametersContainer * inLPC, SUSYT2LimitParams * inSUSYT2LP, bool makeFile) {
        System_MakeDir(nameOutPath);
        
        inLPC->isExpLim = 1;
        inLPC->SetStrings();
        nameOutFile += BaseStringLimit_NameOrDir(inLPC, inSUSYT2LP, 2);
        nameOutFile.Replace(nameOutFile.Index("Exp_"), 4, "");
        nameOutFile += ".root";
        if (makeFile) {
            outFile = new TFile(nameOutPath + nameOutFile, "RECREATE");
            cout << "Saving histograms to " << nameOutPath + nameOutFile << endl;
        }
        else {
            outFile = TFile::Open(nameOutPath + nameOutFile);
            cout << "Loading histograms from " << nameOutPath + nameOutFile << endl;
        }
    }
    void SetHists(SUSYMassLimitParams * inSUSYMLP, bool doVerb = 0) {
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            if (doVerb) {
                cout << "iSig " << iSig << endl;
                cout << "about to set Expected hist" << endl;
            }
            vecLPSExpected[iSig].SetSignifHists(vecInputSignifFileNames_Exp[iSig], vecInputPValueFileNames_Exp[iSig], inSUSYMLP->binSize, doVerb);
        }
        for (unsigned int iSig = 0; iSig < numLPSObs; ++iSig) {
            if (doVerb) {
                cout << "iSig " << iSig << endl;
                cout << "about to set Observed hist" << endl;
            }
            vecLPSObserved[iSig].SetSignifHists(inputSignifFileName_Obs, inputPValueFileName_Obs, inSUSYMLP->binSize, doVerb);
        }
    }
    void SetHistsAsConstDeltaM(bool doVerb) {
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            if (doVerb) {
                cout << "doing const Delta M for vecLPSExpected[" << iSig <<"]" << endl;
            }
            vecLPSExpected[iSig].MakeConstDeltaMHists("");
        }
        for (unsigned int iSig = 0; iSig < numLPSObs; ++iSig) {
            if (doVerb) {
                cout << "doing const Delta M for vecLPSObserved[" << iSig <<"]" << endl;
            }
            vecLPSObserved[iSig].MakeConstDeltaMHists("");
        }
    }
    void SetInputSignifVector(TH2F * &inputSignifHist, vector<TH2F *> * inVecInputExp, vector<TH2F *> * inVecInputObs) {
        inVecInputExp->resize(0);
        inVecInputObs->resize(0);
        inputSignifHist = (TH2F *) vecLPSObserved[0].h_SignifHist->Clone();
        for (unsigned int iSigExp = 0; iSigExp < numLPSExp; ++iSigExp) {
            inVecInputExp->push_back((TH2F *) vecLPSExpected[iSigExp].h_SignifHist->Clone());
        }
        for (unsigned int iSigObs = 0; iSigObs < numLPSObs; ++iSigObs) {
            inVecInputObs->push_back((TH2F *) vecLPSObserved[iSigObs].h_SignifHist->Clone());
        }
    }
    void SmoothSignifConstDeltM_Optimal(Smoother * inSmoother, bool doVerbosity = false) {
        TH2F * currSmoothHist;
        for (unsigned int iSigExp = 0; iSigExp < numLPSExp; ++iSigExp) {
            vecLPSExpected[iSigExp].MakeConstDeltaMHists_Vers2(1);
            currSmoothHist = inSmoother->GenerateOptimalSmoothedMap(vecLPSExpected[iSigExp].h_PValueHist, doVerbosity);
            vecLPSExpected[iSigExp].h_PValueHist = (TH2F *) currSmoothHist->Clone(currSmoothHist->GetName());
            vecLPSExpected[iSigExp].SetSignifFromPValue();
            vecLPSExpected[iSigExp].MakeConstDeltaMHists_Vers2(-1);
        }
        for (unsigned int iSigObs = 0; iSigObs < numLPSObs; ++iSigObs) {
            vecLPSObserved[iSigObs].MakeConstDeltaMHists_Vers2(1);
            currSmoothHist = inSmoother->GenerateOptimalSmoothedMap(vecLPSObserved[iSigObs].h_PValueHist, doVerbosity);
            vecLPSObserved[iSigObs].h_PValueHist = (TH2F *) currSmoothHist->Clone(currSmoothHist->GetName());
            vecLPSObserved[iSigObs].SetSignifFromPValue();
            vecLPSObserved[iSigObs].MakeConstDeltaMHists_Vers2(-1);
        }
    }
    void SmoothSigStrengthConstDeltM(TH2F * inputKernel, TString addName, TString addNameSmoothing) {
        float thresh = 1E-4;
        TH2F * currSmoothHist;
        bool doLogNormal = false;
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            vecLPSExpected[iSig].MakeConstDeltaMHists(addName);
            cout << "about to smooth the signal strength limit in vecLPSExpected[" << iSig << "]" << endl;
            currSmoothHist = KernelAveragedHistogram(vecLPSExpected[iSig].h_SigStrengthLimitHist, inputKernel, doLogNormal, addNameSmoothing, thresh);
            //            cout << "currSmoothHist name " << currSmoothHist->GetName() << endl;
            vecLPSExpected[iSig].h_SigStrengthLimitHist = currSmoothHist;
            cout << "vecLPSExpected[iSig].h_SigStrengthLimitHist name " << vecLPSExpected[iSig].h_SigStrengthLimitHist->GetName() << endl;
            vecLPSExpected[iSig].ReverseConstDeltaMHists();
            cout << "vecLPSExpected[iSig].h_SigStrengthLimitHist name " << vecLPSExpected[iSig].h_SigStrengthLimitHist->GetName() << endl;
        }
        if (numLPSObs > 0) {
            vecLPSObserved[0].MakeConstDeltaMHists(addName);
            cout << "about to smooth the observed signal strength limit " << endl;
            currSmoothHist = KernelAveragedHistogram(vecLPSObserved[0].h_SigStrengthLimitHist, inputKernel, doLogNormal, addNameSmoothing, thresh);
            vecLPSObserved[0].h_SigStrengthLimitHist = currSmoothHist;
            vecLPSObserved[0].ReverseConstDeltaMHists();
            if (numLPSObs > 1) {
                vecLPSObserved[1].h_SigStrengthLimitHist = vecLPSObserved[0].h_SigStrengthLimitHist;
                if (numLPSObs > 2) {
                    vecLPSObserved[2].h_SigStrengthLimitHist = vecLPSObserved[0].h_SigStrengthLimitHist;
                }
            }
        }
    }
    void SmoothXSectionConstDeltM(TH2F * inputKernel, TString addName, TString addNameSmoothing) {
        float thresh = 1E-4;
        TH2F * currSmoothHist;
        bool doLogNormal = true;
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            vecLPSExpected[iSig].MakeConstDeltaMHists(addName);
            cout << "about to smooth the xsection limit in vecLPSExpected[" << iSig << "]" << endl;
            currSmoothHist = KernelAveragedHistogram(vecLPSExpected[iSig].h_XSecLimHist, inputKernel, doLogNormal, addNameSmoothing, thresh);
            //            cout << "currSmoothHist name " << currSmoothHist->GetName() << endl;
            vecLPSExpected[iSig].h_XSecLimHist = currSmoothHist;
            cout << "vecLPSExpected[iSig].h_XSecLimHist name " << vecLPSExpected[iSig].h_XSecLimHist->GetName() << endl;
            vecLPSExpected[iSig].ReverseConstDeltaMHists();
            cout << "vecLPSExpected[iSig].h_XSecLimHist name " << vecLPSExpected[iSig].h_XSecLimHist->GetName() << endl;
        }
        if (numLPSObs > 0) {
            vecLPSObserved[0].MakeConstDeltaMHists(addName);
            cout << "about to smooth the observed xsection limit " << endl;
            currSmoothHist = KernelAveragedHistogram(vecLPSObserved[0].h_XSecLimHist, inputKernel, doLogNormal, addNameSmoothing, thresh);
            vecLPSObserved[0].h_XSecLimHist = currSmoothHist;
            vecLPSObserved[0].ReverseConstDeltaMHists();
            if (numLPSObs > 1) {
                vecLPSObserved[1].h_XSecLimHist = vecLPSObserved[0].h_XSecLimHist;
                if (numLPSObs > 2) {
                    vecLPSObserved[2].h_XSecLimHist = vecLPSObserved[0].h_XSecLimHist;
                }
            }
        }
    }
    void LoadSignifHists(TFile * inputFile, bool doVerb = false) {
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            vecLPSExpected[iSig].LoadSignifHists(inputFile, addName, doVerb);
        }
        for (unsigned int iSig = 0; iSig < numLPSObs; ++iSig) {
            vecLPSObserved[iSig].LoadSignifHists(inputFile, addName, doVerb);
        }
    }
    void WriteFile(bool writeContours, TFile * extOutFile = NULL, bool closeFile = true, bool doVerb = false) {
        TFile * fileToWriteTo = extOutFile ? extOutFile : outFile;
        if (doVerb) {
            cout << "addName " << endl;
        }
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            vecLPSExpected[iSig].SetHistNamesForSaving(addName);
            vecLPSExpected[iSig].WriteSignifToFile(fileToWriteTo, doVerb);
        }
        for (unsigned int iSig = 0; iSig < numLPSObs; ++iSig) {
            vecLPSObserved[iSig].SetHistNamesForSaving(addName);
            vecLPSObserved[iSig].WriteSignifToFile(fileToWriteTo, doVerb);
        }
        if (closeFile) {
            fileToWriteTo->Write();
            fileToWriteTo->Close();
        }
    }
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        for (int iSSCParam = 0; iSSCParam < argc; ++iSSCParam) {
            if (strncmp (argv[iSSCParam],"-inExp", 6) == 0) {
                cout << "format is ExpectedCV, ExpectedOneSigUp, ExpectedOneSigDown" << endl;
                vecInputSignifFileNames_Exp.resize(0);
                for (int iSig = 1; iSig <= 3; ++iSig) {
                    vecInputSignifFileNames_Exp.push_back(TString(argv[iSSCParam + iSig]));
                }
            }
            else if (strncmp (argv[iSSCParam],"-inObs", 6) == 0) {
                inputSignifFileName_Obs = TString(argv[iSSCParam + 1]);
                inputPValueFileName_Obs = TString(argv[iSSCParam + 2]);
            }
            else if (strncmp (argv[iSSCParam],"-sigPath", 8) == 0) {
                basePathSignif = TString(argv[iSSCParam + 1]);
                basePathSignif += "/";
            }
        }
    }
};

struct LimitScenarioContainer {
    TFile * outFile;
    TString addName;
    
    TString basePathLimit;
    
    TString nameOutFile;
    TString nameOutPath;
    
    bool doZeroNonExcl;
    
    vector<TString> vecInputFileNames_Exp;
    TString inputFileName_Obs;
    
    vector<TString> vecGraphNames_Exp;
    vector<TString> vecGraphNames_Obs;
    
    vector<LimitPlotShower> vecLPSExpected;
    vector<LimitPlotShower> vecLPSObserved;
    
    unsigned int numLPSExp, numLPSObs;
    
    float GetMaxXSecLimit(bool doObs, int whichSig) {
        if (doObs) {
            return vecLPSObserved[whichSig].GetXSecLimMax();
        }
        else {
            return vecLPSExpected[whichSig].GetXSecLimMax();
        }
    }
    
    void SetInputVector(TH2F * &inputHistXSection, vector<TH2F *> * inVecInputExp, vector<TH2F *> * inVecInputObs) {
        inVecInputExp->resize(0);
        inVecInputObs->resize(0);
        inputHistXSection = (TH2F *) vecLPSExpected[0].h_XSecLimHist->Clone();
        for (unsigned int iSigExp = 0; iSigExp < numLPSExp; ++iSigExp) {
            inVecInputExp->push_back((TH2F *) vecLPSExpected[iSigExp].h_SigStrengthLimitHist->Clone());
        }
        for (unsigned int iSigObs = 0; iSigObs < numLPSObs; ++iSigObs) {
            inVecInputObs->push_back((TH2F *) vecLPSObserved[iSigObs].h_SigStrengthLimitHist->Clone());
        }
    }
    
    TCanvas * DrawHist(bool doObs, int whichSig, int whichHist, int drawOption, TString secondAddName = "") {
        TCanvas * outCanv;
        if (doObs) {
            outCanv = vecLPSObserved[whichSig].DrawHist(whichHist, doZeroNonExcl, drawOption, addName + secondAddName);
        }
        else {
            outCanv = vecLPSExpected[whichSig].DrawHist(whichHist, doZeroNonExcl, drawOption, addName + secondAddName);
        }
        return outCanv;
    }
    void PrepHists(bool isStandard, bool doObs, int whichSig, float maxXSecLimit, float maxExcl = 1.0) {
        if (doObs) {
            vecLPSObserved[whichSig].SetRangeUser(isStandard, 0, 1E-3, maxExcl);
            vecLPSObserved[whichSig].SetRangeUser(isStandard, 2, 1E-3, maxXSecLimit);
        }
        else {
            vecLPSExpected[whichSig].SetRangeUser(isStandard, 0, 1E-3, maxExcl);
            vecLPSExpected[whichSig].SetRangeUser(isStandard, 2, 1E-3, maxXSecLimit);
        }
    }
    void DefaultVarVals(LimitParametersContainer * inLPC, SUSYT2LimitParams * inSUSYT2LP, bool checkNominality = true) {
        
        basePathLimit = "CombinedLimits/";
        nameOutPath = "ContourFiles/";
        nameOutFile = "h_SigStrength";
        
        addName = "";
        
        numLPSExp = 3;
        numLPSObs = 3;
        
        if (checkNominality) {
            bool doAll = inSUSYT2LP->IsNominalScenario();
            
            if (!doAll) {
                numLPSExp = 1;
                numLPSObs = 3;
            }
        }
        
        vecLPSExpected.resize(numLPSExp);
        vecLPSObserved.resize(numLPSObs);
        vecInputFileNames_Exp.resize(numLPSExp);
        
        TString arrStringGraphNameEnd[3] = {"", "M", "P"};
        vecGraphNames_Exp.resize(numLPSExp);
        vecGraphNames_Obs.resize(numLPSObs);
        TString baseNameGraph = "graph_smoothed_";
        for (unsigned int iSigExp = 0; iSigExp < numLPSExp; ++iSigExp) {
            vecGraphNames_Exp[iSigExp] = baseNameGraph + TString("Exp") + arrStringGraphNameEnd[iSigExp];
        }
        for (unsigned int iSigObs = 0; iSigObs < numLPSObs; ++iSigObs) {
            vecGraphNames_Obs[iSigObs] = baseNameGraph + TString("Obs") + arrStringGraphNameEnd[iSigObs];
        }

        
        TString baseNameLPS_Exp = "_LPS_Exp";
        TString baseNameLPS_Obs = "_LPS_Obs";
        TString addNameLPS[3] = {"", "_OneSigUp", "_OneSigDown"};
        for (unsigned int iSigExp = 0; iSigExp < numLPSExp; ++iSigExp) {
            vecLPSExpected[iSigExp].nameLPS = baseNameLPS_Exp + addNameLPS[iSigExp];
        }
        for (unsigned int iSigObs = 0; iSigObs < numLPSObs; ++iSigObs) {
            vecLPSObserved[iSigObs].nameLPS = baseNameLPS_Obs + addNameLPS[iSigObs];
        }
        
        vector<TString> vecBaseNameLimCard(4);
        vector<LimitParametersContainer> vecLPC(4);
        for (unsigned int iLPC = 0; iLPC < vecLPC.size(); ++iLPC) {
            if (iLPC > 0) {
                vecLPC[iLPC] = vecLPC[0];
            }
            else {
                vecLPC[iLPC] = *inLPC;
            }
            vecLPC[iLPC].isExpLim = iLPC;
            vecLPC[iLPC].SetStrings();
            
            vecBaseNameLimCard[iLPC] = BaseStringLimit_NameOrDir(&vecLPC[iLPC], inSUSYT2LP, 2);
        }
        TString basePathLimCard = BaseStringLimit_NameOrDir(inLPC, inSUSYT2LP, 0) + basePathLimit;
        
        inputFileName_Obs = basePathLimCard + TString("Combined") + vecBaseNameLimCard[0] + TString(".txt");
        for (unsigned int iSigExp = 0; iSigExp < numLPSExp; ++iSigExp) {
            vecInputFileNames_Exp[iSigExp] = basePathLimCard + TString("Combined") + vecBaseNameLimCard[iSigExp + 1] + TString(".txt");
        }
    }
    void SetOutfile(LimitParametersContainer * inLPC, SUSYT2LimitParams * inSUSYT2LP, bool makeFile) {
        System_MakeDir(nameOutPath);
        
        inLPC->isExpLim = 1;
        inLPC->SetStrings();
        nameOutFile += BaseStringLimit_NameOrDir(inLPC, inSUSYT2LP, 2);
        nameOutFile.Replace(nameOutFile.Index("Exp_"), 4, "");
        nameOutFile += ".root";
        if (makeFile) {
            outFile = new TFile(nameOutPath + nameOutFile, "RECREATE");
            cout << "Saving histograms to " << nameOutPath + nameOutFile << endl;
        }
        else {
            outFile = TFile::Open(nameOutPath + nameOutFile);
            cout << "Loading histograms from " << nameOutPath + nameOutFile << endl;
        }
    }
    void SetHists(TH1F * inputHistXSection, SUSYMassLimitParams * inSUSYMLP, int whichAddtlPlots, bool doZeroNonExclPoints, bool doVerb = 0) {
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            if (doVerb) {
                cout << "iSig " << iSig << endl;
                cout << "about to set Expected hist" << endl;
            }
            vecLPSExpected[iSig].SetHists(vecInputFileNames_Exp[iSig], inSUSYMLP->binSize, doVerb);
        }
        for (unsigned int iSig = 0; iSig < numLPSObs; ++iSig) {
            if (doVerb) {
                cout << "iSig " << iSig << endl;
                cout << "about to set Observed hist" << endl;
            }
            vecLPSObserved[iSig].SetHists(inputFileName_Obs, inSUSYMLP->binSize, doVerb);
        }
        
        if (doZeroNonExclPoints) {
            if (doVerb) {
                cout << "about to zero out non-excluded points" << endl;
            }
            for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
                ZeroNonExclPoints(vecLPSExpected[iSig].h_SigStrengthLimitHist, 1E3, false);
            }
            for (unsigned int iSig = 0; iSig < numLPSObs; ++iSig) {
                ZeroNonExclPoints(vecLPSObserved[iSig].h_SigStrengthLimitHist, 1E3, false);
            }
        }
        
        if (numLPSObs > 0) {
            if (doVerb) {
                cout << "about to scale up Observed hist" << endl;
            }
            vecLPSObserved[1].ScaleSigStrengthHist(inputHistXSection, 1);
            if (doVerb) {
                cout << "about to scale down Observed hist" << endl;
            }
            vecLPSObserved[2].ScaleSigStrengthHist(inputHistXSection, -1);
            
        }
        if (doVerb) {
            cout << "about to make XSection Limit Hist" << endl;
        }
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            vecLPSExpected[iSig].MakeXSectionLimitHists(inputHistXSection);
        }
        for (unsigned int iSig = 0; iSig < numLPSObs; ++iSig) {
            vecLPSObserved[iSig].MakeXSectionLimitHists(inputHistXSection);
        }
        if (whichAddtlPlots >= 0) {
            if (whichAddtlPlots == 0) {
                for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
                    if (doVerb) {
                        cout << "iSig " << iSig << endl;
                        cout << "about to set Expected Limit Contour" << endl;
                    }
                    vecLPSExpected[iSig].SetOutContour(vecGraphNames_Exp[iSig], doVerb);
                }
                for (unsigned int iSig = 0; iSig < numLPSObs; ++iSig) {
                    if (doVerb) {
                        cout << "iSig " << iSig << endl;
                        cout << "about to set Observed Limit Contour" << endl;
                    }
                    vecLPSObserved[iSig].SetOutContour(vecGraphNames_Obs[iSig], doVerb);
                }
            }
            else {
                if (doVerb) {
                    cout << "going to try drawing 1D lim graphs" << endl;
                }
                vecLPSExpected[0].DrawLimitAlongDeltaM(false);
                TCanvas * test = new TCanvas("test", "test", 800, 800);
                vecLPSExpected[0].h_SigStrengthLimitHist->Draw("colz");
            }
        }
    }
    void CheckObsScaling() {
        cout << "bin 12:2 " << endl;
        for (unsigned int iSig = 0; iSig < numLPSObs; ++iSig) {
            cout << "Obs -- " << iSig << ": " << vecLPSObserved[iSig].h_SigStrengthLimitHist->GetBinContent(12,2) << endl;
        }
    }
    void MakeXSectionLimitHists(TH1F * inputHistXSection, TString limAddName) {
        CheckObsScaling();
        TString currName = vecLPSObserved[0].h_SigStrengthLimitHist->GetName();
        TString addNameLPS[3] = {"", "_OneSigUp", "_OneSigDown"};
        if (numLPSObs > 0) {
            vecLPSObserved[1].h_SigStrengthLimitHist->SetName(currName + addNameLPS[1] + limAddName);
            vecLPSObserved[2].h_SigStrengthLimitHist->SetName(currName + addNameLPS[2] + limAddName);
            vecLPSObserved[1].ScaleSigStrengthHist(inputHistXSection, 1);
            vecLPSObserved[2].ScaleSigStrengthHist(inputHistXSection, -1);
        }
        CheckObsScaling();
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            vecLPSExpected[iSig].MakeXSectionLimitHists(inputHistXSection, limAddName);
        }
        for (unsigned int iSig = 0; iSig < numLPSObs; ++iSig) {
            vecLPSObserved[iSig].MakeXSectionLimitHists(inputHistXSection, limAddName);
        }
    }
    void MakeLimitStrengthHists(TH1F * inputHistXSection, TString limAddName) {
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            vecLPSExpected[iSig].MakeLimitStrengthHist(inputHistXSection, limAddName);
        }
        for (unsigned int iSig = 0; iSig < numLPSObs; ++iSig) {
            vecLPSObserved[iSig].MakeLimitStrengthHist(inputHistXSection, limAddName);
        }
        if (numLPSObs > 0) {
            vecLPSObserved[1].ScaleSigStrengthHist(inputHistXSection, 1);
            vecLPSObserved[2].ScaleSigStrengthHist(inputHistXSection, -1);
        }
    }
    void SetHistsAsConstDeltaM(bool doVerb) {
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            if (doVerb) {
                cout << "doing const Delta M for vecLPSExpected[" << iSig <<"]" << endl;
            }
            vecLPSExpected[iSig].MakeConstDeltaMHists("");
        }
        for (unsigned int iSig = 0; iSig < numLPSObs; ++iSig) {
            if (doVerb) {
                cout << "doing const Delta M for vecLPSObserved[" << iSig <<"]" << endl;
            }
            vecLPSObserved[iSig].MakeConstDeltaMHists("");
        }
    }
    void SmoothSigStrengthConstDeltM_Optimal(Smoother * inSmoother, bool doVerbosity = false) {
        TH2F * currSmoothHist;
        for (unsigned int iSigExp = 0; iSigExp < numLPSExp; ++iSigExp) {
            vecLPSExpected[iSigExp].MakeConstDeltaMHists_Vers2(1);
            currSmoothHist = inSmoother->GenerateOptimalSmoothedMap(vecLPSExpected[iSigExp].h_SigStrengthLimitHist, doVerbosity);
            vecLPSExpected[iSigExp].h_SigStrengthLimitHist = (TH2F *) currSmoothHist->Clone(currSmoothHist->GetName());
            vecLPSExpected[iSigExp].MakeConstDeltaMHists_Vers2(-1);
        }
        for (unsigned int iSigObs = 0; iSigObs < numLPSObs; ++iSigObs) {
            TString baseName = vecLPSObserved[0].h_SigStrengthLimitHist->GetName();
            int lengthBaseName = baseName.Length();
            if (iSigObs == 0) {
                vecLPSObserved[iSigObs].MakeConstDeltaMHists_Vers2(1);
                currSmoothHist = inSmoother->GenerateOptimalSmoothedMap(vecLPSObserved[iSigObs].h_SigStrengthLimitHist, doVerbosity);
                vecLPSObserved[iSigObs].h_SigStrengthLimitHist = (TH2F *) currSmoothHist->Clone(currSmoothHist->GetName());
                vecLPSObserved[iSigObs].MakeConstDeltaMHists_Vers2(-1);
            }
            else {
                TString currName = vecLPSObserved[0].h_SigStrengthLimitHist->GetName();
                currName.Replace(currName.Index(baseName), lengthBaseName, "");
                TString currName2 = vecLPSObserved[iSigObs].h_SigStrengthLimitHist->GetName();
                vecLPSObserved[iSigObs].h_SigStrengthLimitHist = (TH2F *) vecLPSObserved[0].h_SigStrengthLimitHist->Clone(currName2 + currName);
            }
        }
    }
    void SmoothSigStrengthConstDeltM(TH2F * inputKernel, TString addName, TString addNameSmoothing) {
        float thresh = 1E-4;
        TH2F * currSmoothHist;
        bool doLogNormal = false;
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            vecLPSExpected[iSig].MakeConstDeltaMHists(addName);
            cout << "about to smooth the signal strength limit in vecLPSExpected[" << iSig << "]" << endl;
            currSmoothHist = KernelAveragedHistogram(vecLPSExpected[iSig].h_SigStrengthLimitHist, inputKernel, doLogNormal, addNameSmoothing, thresh);
            //            cout << "currSmoothHist name " << currSmoothHist->GetName() << endl;
            vecLPSExpected[iSig].h_SigStrengthLimitHist = currSmoothHist;
            cout << "vecLPSExpected[iSig].h_SigStrengthLimitHist name " << vecLPSExpected[iSig].h_SigStrengthLimitHist->GetName() << endl;
            vecLPSExpected[iSig].ReverseConstDeltaMHists();
            cout << "vecLPSExpected[iSig].h_SigStrengthLimitHist name " << vecLPSExpected[iSig].h_SigStrengthLimitHist->GetName() << endl;
        }
        if (numLPSObs > 0) {
            vecLPSObserved[0].MakeConstDeltaMHists(addName);
            cout << "about to smooth the observed signal strength limit " << endl;
            currSmoothHist = KernelAveragedHistogram(vecLPSObserved[0].h_SigStrengthLimitHist, inputKernel, doLogNormal, addNameSmoothing, thresh);
            vecLPSObserved[0].h_SigStrengthLimitHist = currSmoothHist;
            vecLPSObserved[0].ReverseConstDeltaMHists();
            if (numLPSObs > 1) {
                vecLPSObserved[1].h_SigStrengthLimitHist = vecLPSObserved[0].h_SigStrengthLimitHist;
                if (numLPSObs > 2) {
                    vecLPSObserved[2].h_SigStrengthLimitHist = vecLPSObserved[0].h_SigStrengthLimitHist;
                }
            }
        }
    }
    void SmoothXSectionConstDeltM(TH2F * inputKernel, TString addName, TString addNameSmoothing) {
        float thresh = 1E-4;
        TH2F * currSmoothHist;
        bool doLogNormal = true;
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            vecLPSExpected[iSig].MakeConstDeltaMHists(addName);
            cout << "about to smooth the xsection limit in vecLPSExpected[" << iSig << "]" << endl;
            currSmoothHist = KernelAveragedHistogram(vecLPSExpected[iSig].h_XSecLimHist, inputKernel, doLogNormal, addNameSmoothing, thresh);
//            cout << "currSmoothHist name " << currSmoothHist->GetName() << endl;
            vecLPSExpected[iSig].h_XSecLimHist = currSmoothHist;
            cout << "vecLPSExpected[iSig].h_XSecLimHist name " << vecLPSExpected[iSig].h_XSecLimHist->GetName() << endl;
            vecLPSExpected[iSig].ReverseConstDeltaMHists();
            cout << "vecLPSExpected[iSig].h_XSecLimHist name " << vecLPSExpected[iSig].h_XSecLimHist->GetName() << endl;
        }
        if (numLPSObs > 0) {
            vecLPSObserved[0].MakeConstDeltaMHists(addName);
            cout << "about to smooth the observed xsection limit " << endl;
            currSmoothHist = KernelAveragedHistogram(vecLPSObserved[0].h_XSecLimHist, inputKernel, doLogNormal, addNameSmoothing, thresh);
            vecLPSObserved[0].h_XSecLimHist = currSmoothHist;
            vecLPSObserved[0].ReverseConstDeltaMHists();
            if (numLPSObs > 1) {
                vecLPSObserved[1].h_XSecLimHist = vecLPSObserved[0].h_XSecLimHist;
                if (numLPSObs > 2) {
                    vecLPSObserved[2].h_XSecLimHist = vecLPSObserved[0].h_XSecLimHist;
                }
            }
        }
    }
    void LoadHists(TFile * inputFile, bool getNoTruncate, bool doVerb = false) {
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            vecLPSExpected[iSig].LoadHists(inputFile, getNoTruncate, addName, doVerb);
        }
        for (unsigned int iSig = 0; iSig < numLPSObs; ++iSig) {
            vecLPSObserved[iSig].LoadHists(inputFile, getNoTruncate, addName, doVerb);
        }
    }
    void WriteFile(bool writeContours, TFile * extOutFile = NULL, bool closeFile = true, bool doVerb = false) {
        TFile * fileToWriteTo = extOutFile ? extOutFile : outFile;
        if (doVerb) {
            cout << "addName " << endl;
        }
        for (unsigned int iSig = 0; iSig < numLPSExp; ++iSig) {
            vecLPSExpected[iSig].SetHistNamesForSaving(addName);
            vecLPSExpected[iSig].WriteToFile(fileToWriteTo, 1, true, doVerb);
            vecLPSExpected[iSig].WriteToFile(fileToWriteTo, 3, false, doVerb);
            if (writeContours) {
                vecLPSExpected[iSig].WriteToFile(fileToWriteTo, 4, false, doVerb);
            }
        }
        for (unsigned int iSig = 0; iSig < numLPSObs; ++iSig) {
            vecLPSObserved[iSig].SetHistNamesForSaving(addName);
            vecLPSObserved[iSig].WriteToFile(fileToWriteTo, 1, true, doVerb);
            vecLPSObserved[iSig].WriteToFile(fileToWriteTo, 3, false, doVerb);
            if (writeContours) {
                vecLPSObserved[iSig].WriteToFile(fileToWriteTo, 4, false, doVerb);
            }
        }
        if (closeFile) {
            fileToWriteTo->Write();
            fileToWriteTo->Close();
        }
    }
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        for (int iLSCParam = 0; iLSCParam < argc; ++iLSCParam) {
            if (strncmp (argv[iLSCParam],"-inExp", 6) == 0) {
                cout << "format is ExpectedCV, ExpectedOneSigUp, ExpectedOneSigDown" << endl;
                vecInputFileNames_Exp.resize(0);
                for (int iSig = 1; iSig <= 3; ++iSig) {
                    vecInputFileNames_Exp.push_back(TString(argv[iLSCParam + iSig]));
                }
            }
            else if (strncmp (argv[iLSCParam],"-inObs", 6) == 0) {
                inputFileName_Obs = TString(argv[iLSCParam + 1]);
            }
            else if (strncmp (argv[iLSCParam],"-limPath", 8) == 0) {
                basePathLimit = TString(argv[iLSCParam + 1]);
                basePathLimit += "/";
            }
        }
    }
};

struct LimitScenarioSmoother_Back {
    LimitScenarioContainer LSC_Unsmoothed;
    vector<LimitScenarioContainer> vecLSC_Smoothed;
    
    vector<float> vecSigX, vecSigY;
    vector<TString> vecAddName, vecSmoothName;
    vector<TH2F *> vecKernels;
    
    void PrepHists(bool isStandard, bool doObs, int whichSig, float maxExcl = 1.0) {
        vector<float> vecMaxXSecLimit;
        vecMaxXSecLimit.push_back(LSC_Unsmoothed.GetMaxXSecLimit(doObs, whichSig));
        for (unsigned int iSig = 0; iSig < vecLSC_Smoothed.size(); ++iSig) {
            vecMaxXSecLimit.push_back(vecLSC_Smoothed[iSig].GetMaxXSecLimit(doObs, whichSig));
        }
        sort(vecMaxXSecLimit.begin(), vecMaxXSecLimit.end());
        float maxXSecLimit = vecMaxXSecLimit[vecMaxXSecLimit.size() - 1];
        LSC_Unsmoothed.PrepHists(isStandard, doObs, whichSig, maxXSecLimit, maxExcl);
        for (unsigned int iSig = 0; iSig < vecLSC_Smoothed.size(); ++iSig) {
            vecLSC_Smoothed[iSig].PrepHists(isStandard, doObs, whichSig, maxXSecLimit, maxExcl);
        }
    }
    
    void SetZeroNonExcl(bool whichVal) {
        LSC_Unsmoothed.doZeroNonExcl = whichVal;
        for (unsigned int iSig = 0; iSig < vecLSC_Smoothed.size(); ++iSig) {
            vecLSC_Smoothed[iSig].doZeroNonExcl = whichVal;
        }
    }
    
    void DrawLatex(TCanvas * inputCanvas, int whichSmooth) {
        TString strSmooth = !whichSmooth ? "No Smoothing" : "Gaussian Kernel: ";
        if (whichSmooth) {
            strSmooth += "(#sigma_{x}:#sigma_{y}) [GeV] = (";
            strSmooth += vecSigX[whichSmooth - 1];
            strSmooth += ":";
            strSmooth += vecSigY[whichSmooth - 1];
            strSmooth += ")";
        }
        inputCanvas->cd();
        TLatex a;
        a.SetNDC();
        a.SetTextSize(0.03);
        a.DrawLatex(0.15, 0.88, strSmooth.Data());
    }
    void DrawHist(bool doObs, int whichSig, int whichHist, int whichSmooth, int drawOption, TString addNameCanv) {
        TCanvas * outCanv;
        if (whichSmooth) {
            outCanv = vecLSC_Smoothed[whichSmooth - 1].DrawHist(doObs, whichSig, whichHist, drawOption, vecSmoothName[whichSmooth - 1]);
        }
        else {
            outCanv = LSC_Unsmoothed.DrawHist(doObs, whichSig, whichHist, drawOption, "");
        }
        DrawLatex(outCanv, whichSmooth);
        outCanv->SaveAs(outCanv->GetName() + addNameCanv + TString(".pdf"));
    }
    
    void SetHistsAsConstDeltaM(bool doVerb) {
        if (doVerb) {
            cout << "doing ConstDeltaM for unsmoothed " << endl;
        }
        LSC_Unsmoothed.SetHistsAsConstDeltaM(doVerb);
        for (unsigned int iSig = 0; iSig < vecLSC_Smoothed.size(); ++iSig) {
            if (doVerb) {
                cout << "doing ConstDeltaM for vecLSC_Smoothed[" << iSig << "]" << endl;
            }
            vecLSC_Smoothed[iSig].SetHistsAsConstDeltaM(doVerb);
        }
    }
    
    void LoadHists(TFile * inputFile, bool getNoTruncate, bool doVerb = false) {
        LSC_Unsmoothed.LoadHists(inputFile, getNoTruncate, doVerb);
        for (unsigned int iSig = 0; iSig < vecLSC_Smoothed.size(); ++iSig) {
            vecLSC_Smoothed[iSig].LoadHists(inputFile, getNoTruncate, doVerb);
        }
    }
    
    void DefaultVarVals() {
        
        const int numSigs = 20;
        //float sigXVals[numSigs] = {15, 25, 25, 45, 45, 35, 10, 15, 15, 0.5};
        //float sigYVals[numSigs] = {15, 15, 10, 25, 10,  5,  5, 10,  5, 0.5};
        float sigXVals[numSigs] = {15, 25, 25, 45, 45, 35, 10, 15, 15, 0.5, 15, 25, 25, 45, 45, 35, 10, 15, 15, 0.5};
        float sigYVals[numSigs] = {15, 15, 10, 25, 10,  5,  5, 10,  5, 0.5, 15, 15, 10, 25, 10,  5,  5, 10,  5, 0.5};
        
        TString nameAdd[numSigs];
        TString nameSmooth[numSigs];
        for (int iAddName = 0; iAddName < numSigs; ++iAddName) {
            nameAdd[iAddName] = "_Smooth";
            if (iAddName < numSigs / 2) nameAdd[iAddName] += "XSec";
            nameAdd[iAddName] += iAddName + 1;
            
            nameSmooth[iAddName] = "_2D";
            if (iAddName < numSigs / 2) nameSmooth[iAddName] += "XSec";
            nameSmooth[iAddName] += "X";
            nameSmooth[iAddName] += sigXVals[iAddName];
            nameSmooth[iAddName] += "Y";
            nameSmooth[iAddName] += sigYVals[iAddName];
        }
        //TString nameSmooth[numSigs] = {"_2DBoth15", "_2DX25Y15", "_2DX25Y10", "_2DX45Y25", "_2DX45Y10", "_2DX10Y5", "_2DX15Y10", "_2DX15Y5", "_NoSmooth"};
         
        /*
        const int numSigs = 1;
        float sigXVals[numSigs] = {45};
        float sigYVals[numSigs] = {25};
        TString nameAdd[numSigs] = {"_Smooth4"};
        TString nameSmooth[numSigs] = {"_2DHighXMedY"};
        */
        vecSigX.resize(0);
        vecSigX.assign(sigXVals, sigXVals + numSigs);
        vecSigY.resize(0);
        vecSigY.assign(sigYVals, sigYVals + numSigs);
        
        vecAddName.resize(0);
        vecAddName.assign(nameAdd, nameAdd + numSigs);
        
        vecSmoothName.resize(0);
        vecSmoothName.assign(nameSmooth, nameSmooth + numSigs);
        
        for (int iSig = 0; iSig < numSigs; ++iSig) {
            vecKernels.push_back(TwoDeeGaussianKernel(vecSigX[iSig], vecSigY[iSig]));
        }
        vecLSC_Smoothed.resize(numSigs);
    }
    void SetLSCVec(TH1F * inputHistXSection, bool doSmoothing = true) {
        for (unsigned int iLSC = 0; iLSC < vecLSC_Smoothed.size(); ++iLSC) {
            cout << "iLSC " << iLSC << endl;
            vecLSC_Smoothed[iLSC] = LSC_Unsmoothed;
            vecLSC_Smoothed[iLSC].addName = vecSmoothName[iLSC];
            if (doSmoothing) {
                if (iLSC >= vecLSC_Smoothed.size() / 2) {
                    vecLSC_Smoothed[iLSC].SmoothSigStrengthConstDeltM(vecKernels[iLSC], vecAddName[iLSC], vecSmoothName[iLSC]);
                    vecLSC_Smoothed[iLSC].MakeXSectionLimitHists(inputHistXSection, vecAddName[iLSC] + vecSmoothName[iLSC]);
                }
                else {
                    vecLSC_Smoothed[iLSC].SmoothXSectionConstDeltM(vecKernels[iLSC], vecAddName[iLSC], vecSmoothName[iLSC]);
                    vecLSC_Smoothed[iLSC].MakeLimitStrengthHists(inputHistXSection, vecAddName[iLSC] + vecSmoothName[iLSC]);
                }
            }
        }
    }
    void WriteFile(TFile * extOutFile = NULL, bool doVerb = false) {
        for (unsigned int iLSC = 0; iLSC < vecLSC_Smoothed.size(); ++iLSC) {
            vecLSC_Smoothed[iLSC].WriteFile(false, extOutFile, false, doVerb);
        }
        LSC_Unsmoothed.WriteFile(false, extOutFile, true, doVerb);
    }
    
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        /*
        for (int iLSSParam = 0; iLSSParam < argc; ++iLSSParam) {
            if (strncmp (argv[iLSSParam],"-sigX", 5) == 0) {
                vecInputFileNames_Exp.resize(0);
                for (int iSig = 1; iSig <= 3; ++iSig) {
                    vecInputFileNames_Exp.push_back(TString(argv[iLSSParam + iSig]));
                }
            }
            else if (strncmp (argv[iLSSParam],"-inObs", 6) == 0) {
                inputFileName_Obs = TString(argv[iLSSParam + 1]);
            }
        }
        */
    }
};



