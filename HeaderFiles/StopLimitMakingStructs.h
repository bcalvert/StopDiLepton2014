#include "TH2F.h"
#include "TLegend.h"
//#include "TH2C.h"
#include <vector>
#include "LimitScriptHeaderFiles/LimitScripts_ParametersStruct.h"
#include "LimitScriptHeaderFiles/LimitScripts_SUSYStruct.h"

//Structures used when showing/making exclusion limit plots -- need to work on documentation

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
    TH2F * h_SigStrengthLimitHist, * h_SigExclHist, * h_XSecLimHist;
    vector<TH2F *> vecMT2CutHists;
    
    TGraph * limContour;
    
    TString nameLPS; //base name added into histograms
    
    int numMT2Hists;
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
    void SetHists(TString inputFile, int binSize, bool doVerb = 0) {
        numMT2Hists = 2;
        
        TString addHistNameSigStrength[3] = {"", "OneSigUp", "OneSigDown"};
        TString addHistNameMT2[2] = {"_BestMT2llCut", "_BestMT2lblbCut"};
        vector<TString> vecHistNames_MT2Cut(0);
        
        if (doVerb) {
            cout << "nameLPS " << nameLPS << endl;
        }
        
        TString baseHistName = "h_SigStrengthLimitHist";
        baseHistName += nameLPS;
        
        for (int iHist = 0; iHist < 2; ++iHist) {
            vecHistNames_MT2Cut.push_back(baseHistName + addHistNameMT2[iHist]);
        }
        LimitHist(h_SigStrengthLimitHist, &vecMT2CutHists, inputFile, baseHistName, &vecHistNames_MT2Cut, binSize, doVerb);
    }
    
    void MakeXSectionLimitHists(TH1F * histXSection) {
        h_XSecLimHist = (TH2F *) h_SigStrengthLimitHist->Clone(h_SigStrengthLimitHist->GetName() + TString("_XSecLimit"));

        int nBinsX = h_XSecLimHist->GetNbinsX();
        int nBinsY = h_XSecLimHist->GetNbinsY();
        float massStop, currXSection, currSigStrengthLim;
        for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
            massStop = h_SigStrengthLimitHist->GetXaxis()->GetBinCenter(iBinX);
            currXSection = histXSection->GetBinContent(histXSection->GetXaxis()->FindBin(massStop));
            for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
                currSigStrengthLim = h_XSecLimHist->GetBinContent(iBinX, iBinY);
                currSigStrengthLim *= currXSection;
                h_XSecLimHist->SetBinContent(iBinX, iBinY, currSigStrengthLim);
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
    void WriteToFile(TFile * outFile, int whichHist, bool createSigExclStuff = false) {
        outFile->cd();
        TH2F * histToWrite;
        if (whichHist < 4) {
            switch (whichHist) {
                case 1:
                    histToWrite = h_SigStrengthLimitHist;
                    if (createSigExclStuff) {
                        TH2F * h_SigStrengthLimitHistNoTruncate = (TH2F *) h_SigStrengthLimitHist->Clone(h_SigStrengthLimitHist->GetName() + TString("_NoTruncation"));
                        ZeroNonExclPoints(h_SigStrengthLimitHist);
                        h_SigStrengthLimitHist->GetZaxis()->SetRangeUser(0, 1.0);
                        
                        TH2F * h_SigStrengthLimitHist_StatErrShiftUp = ExclConsistencyHist(h_SigStrengthLimitHist, 1);
                        TH2F * h_SigStrengthLimitHist_StatErrShiftDown = ExclConsistencyHist(h_SigStrengthLimitHist, -1);
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

        TString basePathCardSuper = BaseStringLimit_NameOrDir(inLPCSuper, inSUSYT2LPSuper, 0) + baseDirSuper;
        TString baseNameCardSuper = BaseStringLimit_NameOrDir(inLPCSuper, inSUSYT2LPSuper, 2);
        fileNameSuper = basePathCardSuper + TString("Combined") + baseNameCardSuper + TString(".txt");
        if (doVerb) {
            cout << "fileNameSuper " << fileNameSuper << endl;
        }
        LPS_Super.nameLPS = "LPS_Super" + baseNameCardSuper;
        
        TString basePathCardSub = BaseStringLimit_NameOrDir(inLPCSub, inSUSYT2LPSub, 0) + baseDirSub;
        TString baseNameCardSub = BaseStringLimit_NameOrDir(inLPCSub, inSUSYT2LPSub, 2);
        fileNameSub = basePathCardSub + TString("Combined") + baseNameCardSub + TString(".txt");
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

struct LimitScenarioContainer {
    TFile * outFile;
    TString nameOutFile;
    TString nameOutPath;
    
    vector<TString> vecInputFileNames_Exp;
    TString inputFileName_Obs;
    
    vector<TString> vecGraphNames_Exp;
    vector<TString> vecGraphNames_Obs;
    
    vector<LimitPlotShower> vecLPSExpected;
    vector<LimitPlotShower> vecLPSObserved;
    void DefaultVarVals(LimitParametersContainer * inLPC, SUSYT2LimitParams * inSUSYT2LP) {
        nameOutPath = "ContourFiles/";
        nameOutFile = "h_SigStrength";
        
        const int numLPS = 3;
        vecLPSExpected.resize(numLPS);
        vecLPSObserved.resize(numLPS);
        vecInputFileNames_Exp.resize(numLPS);
        
        TString arrStringGraphNameEnd[numLPS] = {"", "M", "P"};
        vecGraphNames_Exp.resize(numLPS);
        vecGraphNames_Obs.resize(numLPS);
        TString baseNameGraph = "graph_smoothed_";
        for (int iSig = 0; iSig < numLPS; ++iSig) {
            vecGraphNames_Exp[iSig] = baseNameGraph + TString("Exp") + arrStringGraphNameEnd[iSig];
            vecGraphNames_Obs[iSig] = baseNameGraph + TString("Obs") + arrStringGraphNameEnd[iSig];
        }
        
        TString baseNameLPS_Exp = "_LPS_Exp";
        TString baseNameLPS_Obs = "_LPS_Obs";
        TString addNameLPS[numLPS] = {"", "_OneSigUp", "_OneSigDown"};
        for (unsigned int iSig = 0; iSig < numLPS; ++iSig) {
            vecLPSExpected[iSig].nameLPS = baseNameLPS_Exp + addNameLPS[iSig];
            vecLPSObserved[iSig].nameLPS = baseNameLPS_Obs + addNameLPS[iSig];
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
        TString basePathLimCard = BaseStringLimit_NameOrDir(inLPC, inSUSYT2LP, 0) + TString("CombinedLimits/");
        
        inputFileName_Obs = basePathLimCard + TString("Combined") + vecBaseNameLimCard[0] + TString(".txt");
        for (int iExp = 0; iExp < numLPS; ++iExp) {
            vecInputFileNames_Exp[iExp] = basePathLimCard + TString("Combined") + vecBaseNameLimCard[iExp + 1] + TString(".txt");
        }
    }
    void SetOutfile(LimitParametersContainer * inLPC, SUSYT2LimitParams * inSUSYT2LP) {
        System_MakeDir(nameOutPath);
        
        inLPC->isExpLim = 1;
        inLPC->SetStrings();
        nameOutFile += BaseStringLimit_NameOrDir(inLPC, inSUSYT2LP, 2);
        nameOutFile.Replace(nameOutFile.Index("Exp_"), 4, "");
        nameOutFile += ".root";
        outFile = new TFile(nameOutPath + nameOutFile, "RECREATE");
        cout << "Saving histograms to " << nameOutFile << endl;
    }
    void SetHists(TH1F * inputHistXSection, SUSYMassLimitParams * inSUSYMLP, int whichAddtlPlots, bool doVerb = 0) {
        unsigned int numLPS = vecLPSExpected.size();
        for (unsigned int iSig = 0; iSig < numLPS; ++iSig) {
            if (doVerb) {
                cout << "iSig " << iSig << endl;
                cout << "about to set Expected hist" << endl;
            }
            vecLPSExpected[iSig].SetHists(vecInputFileNames_Exp[iSig], inSUSYMLP->binSize, doVerb);
            if (doVerb) {
                cout << "about to set Observed hist" << endl;
            }
            vecLPSObserved[iSig].SetHists(inputFileName_Obs, inSUSYMLP->binSize, doVerb);
        }
        if (doVerb) {
            cout << "about to scale up Observed hist" << endl;
        }
        vecLPSObserved[1].ScaleSigStrengthHist(inputHistXSection, 1);
        if (doVerb) {
            cout << "about to scale down Observed hist" << endl;
        }
        vecLPSObserved[2].ScaleSigStrengthHist(inputHistXSection, -1);
        if (doVerb) {
            cout << "about to make Expected XSection Limit Hist" << endl;
        }
        vecLPSExpected[0].MakeXSectionLimitHists(inputHistXSection);
        if (whichAddtlPlots == 0) {
            for (unsigned int iSig = 0; iSig < numLPS; ++iSig) {
                if (doVerb) {
                    cout << "iSig " << iSig << endl;
                    cout << "about to set Expected Limit Contour" << endl;
                }
                vecLPSExpected[iSig].SetOutContour(vecGraphNames_Exp[iSig], doVerb);
                if (doVerb) {
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
    void WriteFile() {
        for (unsigned int iSig = 0; iSig < vecLPSExpected.size(); ++iSig) {
            vecLPSExpected[iSig].WriteToFile(outFile, 1, true);
            vecLPSObserved[iSig].WriteToFile(outFile, 1, true);
            
            vecLPSExpected[iSig].WriteToFile(outFile, 4);
            vecLPSObserved[iSig].WriteToFile(outFile, 4);
        }
        vecLPSExpected[0].WriteToFile(outFile, 3);
        outFile->Write();
        outFile->Close();
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
        }
    }
};