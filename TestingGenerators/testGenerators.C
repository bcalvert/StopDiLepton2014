#include "TString.h"
#include "TH3F.h"
#include <iostream>
#include "TTree.h"
#include "TMath.h"
#include "TFile.h"
#include "TH3F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TRint.h"
#include <vector>
#include <cmath>
#include <numeric>
#include "TGraphAsymmErrors.h"
#include "TFitResult.h"
#include "TLine.h"
#include "TLegend.h"

#include "../HeaderFiles/BasicFunctions.h"
#include "../HeaderFiles/BasicFunctions_Math.h"
#include "../HeaderFiles/BasicFunctions_HistogramMaking.h"
#include "../HeaderFiles/BasicFunctions_SystemCommands.h"
#include "../HeaderFiles/HistogramSystematics2.h"
#include "../HeaderFiles/HistogramSystematicsStructs.h"

using namespace std;

float GetTotalError(float inputErrorSystUp, float inputErrorSystDown, float inputStatError) {
    float totalSystError = TMath::Power(inputErrorSystDown, 2);
    totalSystError += TMath::Power(inputErrorSystUp, 2);
    totalSystError /= 2;
    totalSystError += TMath::Power(inputStatError, 2);
    return TMath::Sqrt(totalSystError);
}
/*
inline void HistMainAttSet(TH1 * inputHist, Color_t FillColor = kBlack, Style_t FillStyle = 1001, Color_t LineColor = kBlack, Width_t LineWidth = 2, Color_t MarkerColor = kBlack, Style_t MarkerStyle = 20, Size_t MarkerSize = 1, Style_t LineStyle = 1) {
    inputHist->SetFillColor(FillColor);
    inputHist->SetFillStyle(FillStyle);
    inputHist->SetLineColor(LineColor);
    inputHist->SetLineWidth(LineWidth);
    inputHist->SetMarkerColor(MarkerColor);
    inputHist->SetMarkerStyle(MarkerStyle);
    inputHist->SetMarkerSize(MarkerSize);
    inputHist->SetLineStyle(LineStyle);
}

void pointSystErr(Double_t centVal, Double_t shiftUp, Double_t shiftDown, Double_t &errUp, Double_t &errDown, bool doSymError = false, bool doVerb = false) {
    Double_t diffShiftUp, diffShiftDown;
    Double_t errSum;
    int whichErrCase = 0;
    // +(-) 3 for if shift up is greater (smaller) than cent Val; likewise for shift down but with +(-) 1 instead;
    // +4 (-4): both more (less) than cent val
    // +2 (-2): upErr more, downErr less (upErr less, downErr more)
    if (doVerb) {
        std::cout << "shiftUp " << shiftUp << std::endl;
        std::cout << "shiftDown " << shiftDown << std::endl;
    }
    whichErrCase += (shiftUp > centVal) ? 3 : 0;
    whichErrCase += (shiftUp < centVal) ? -3 : 0;
    whichErrCase += (shiftDown > centVal) ? 1 : 0;
    whichErrCase += (shiftDown < centVal) ? -1 : 0;
    diffShiftUp = centVal - shiftUp;
    diffShiftDown = centVal - shiftDown;
    //    std::cout << "justOneSideErr " << justOneSideErr << std::endl;
    //    std::cout << "whichErrCase " << whichErrCase << std::endl;
    errSum = TMath::Sqrt(0.5*(diffShiftDown*diffShiftDown + diffShiftUp*diffShiftUp));
    if (doSymError) {
        errUp = errSum;
        errDown = errSum;
    }
    else {
        switch (whichErrCase) {
            case -4:
                errUp = 0;
                errDown = errSum;
                break;
            case -3:
                errUp = 0;
                errDown = abs(diffShiftUp);
                break;
            case -2:
                errUp = abs(diffShiftDown);
                errDown = abs(diffShiftUp);
                break;
            case -1:
                errUp = 0;
                errDown = abs(shiftDown);
            case 0:
                errUp = 0;
                errDown = 0;
                break;
            case 1:
                errUp = abs(shiftDown);
                errDown = 0;
            case 2:
                errUp = abs(diffShiftUp);
                errDown = abs(diffShiftDown);
                break;
            case 3:
                errUp = abs(diffShiftUp);
                errDown = 0;
                break;
            case 4:
                errUp = errSum;
                errDown = 0;
                break;
            default:
                break;
        }
    }
    if (doVerb) {
        std::cout << "whichErrCase " << whichErrCase << std::endl;
        std::cout << "errUp " << errUp << std::endl;
        std::cout << "errDown " << errDown << std::endl;
    }
}
*/

int main( int argc, char* argv[]){
    gROOT->ForceStyle();
    gROOT->ProcessLine("#include <vector>");
    // Style things
    gStyle->SetErrorX(0.5); // set X uncertainty to 1/2 of a bin
    gStyle->SetTitleFont(42);
    
    
    TRint theApp("App", &argc, argv);
    Bool_t retVal = kTRUE;
    
    
    bool doVerb = 0;
    int whichGenFile0 = 1, whichGenFile1 = 2;
    
    bool useFakes = false;
    int whichChan = 3;
    
    float scaleGenDiff = 1.0;
    double sigma = 1.0;
    for (int iGenCompareParam = 0; iGenCompareParam < argc; ++iGenCompareParam) {
        if (strncmp (argv[iGenCompareParam],"doVerb", 6) == 0) {
            doVerb = 1;
        }
        else if (strncmp (argv[iGenCompareParam],"firstTTbarGen", 13) == 0) {
            whichGenFile0 = strtol(argv[iGenCompareParam + 1], NULL, 10 );
        }
        else if (strncmp (argv[iGenCompareParam],"secondTTbarGen", 14) == 0) {
            whichGenFile1 = strtol(argv[iGenCompareParam + 1], NULL, 10 );
        }
        else if (strncmp (argv[iGenCompareParam],"useFakes", 8) == 0) {
            useFakes = true;
        }
        else if (strncmp (argv[iGenCompareParam],"whichChan", 9) == 0) {
            whichChan = strtol(argv[iGenCompareParam + 1], NULL, 10 );
        }
        else if (strncmp (argv[iGenCompareParam],"scaleGen", 8) == 0) {
            scaleGenDiff = strtod(argv[iGenCompareParam + 1], NULL);
        }
        else if (strncmp (argv[iGenCompareParam],"sigma", 5) == 0) {
            sigma = strtod(argv[iGenCompareParam + 1], NULL);
        }
    }
    
    float xValLatexDilep = 0.186;
    float yValLatexDilep = 0.77;
    
    float xValLatexGenerator = 0.16;
    float yValLatexGenerator = 0.86;
    
    TString appendNamesTTBarGens[4] = {"Madgraph_Exclusive", "MCatNLO", "Powheg", "Madgraph"};
    TString legendNameTTBarGens[4] = {"Exc. Madgraph", "MC@NLO", "Powheg", "Madgraph"};
    TString nameChannel[4] = {"_MuMu", "_EE", "_EMu", ""};
    
    TString arrDilepLatex[3] = {"#mu#mu", "ee", "e#mu"};
    TString strLatexDilep = whichChan != 3 ? arrDilepLatex[whichChan] : "";
    TString strLatexGenerator = TString("#frac{") + legendNameTTBarGens[whichGenFile0] + TString("}{") + legendNameTTBarGens[whichGenFile1] + TString("}");
    TLatex b;
    b.SetNDC();
    b.SetTextSize(0.09);
    
    TLatex c;
    c.SetNDC();
    c.SetTextSize(0.038);

    
    TString baseCanvNameRatio = "c_TTBarGenCompare_";
    TString baseCanvNameRatioSignif = "c_TTBarGenShapeCompareSignif_";
    TString baseCanvNameShapeCorr = "c_TTBarGenShapeUncert_";
    TString canvAddName = appendNamesTTBarGens[whichGenFile0] + TString("Div") + appendNamesTTBarGens[whichGenFile1] + nameChannel[whichChan];
    
    TString outFileName = "h_TTBarGenShapeUncert_" + canvAddName;
    
    TString baseHistName = "h_SmearMT2ll_vs_MT2lblb_vs_MT2bb_ZMET_FullCut_LowBLepMass";
    TString baseFileName = "../RootFiles/MC/TTBarComp_";
    TString baseFileNameEnd = "_Oviedo_wSS0" + nameChannel[whichChan] + TString("_noGenRecRW");
    if (!useFakes) baseFileNameEnd += "_noFakes";
    baseFileNameEnd += "Haddplots";
    
    TString nameFile0 = baseFileName + appendNamesTTBarGens[whichGenFile0] + baseFileNameEnd + TString(".root");
    TString nameFile1 = baseFileName + appendNamesTTBarGens[whichGenFile1] + baseFileNameEnd + TString(".root");
    
    TFile * _file0 = TFile::Open(nameFile0);
    TFile * _file1 = TFile::Open(nameFile1);
    
    TH3F * arrHistsFile0[3];
    TH3F * arrHistsFile1[3];
    TString addNameSyst[3] = {"", "_genRecoilRWShiftUp", "_genRecoilRWShiftDown"};
    double currControlBinFile0[3], currControlBinFile1[3];
    double currIntegral;

    for (int iSyst = 0; iSyst < 3; ++iSyst) {
        TString grabName = baseHistName + addNameSyst[iSyst];
        arrHistsFile0[iSyst] = (TH3F*) _file0->Get(grabName);
        
        currIntegral = arrHistsFile0[iSyst]->Integral();
        arrHistsFile0[iSyst]->Scale(1./currIntegral);
        currControlBinFile0[iSyst] = arrHistsFile0[iSyst]->GetBinContent(1,1,1);
        /*
        if (iSyst != 0) {
            arrHistsFile0[iSyst]->Scale(currControlBinFile0[0]/currControlBinFile0[iSyst]);
        }
         */
        
        arrHistsFile1[iSyst] = (TH3F*) _file1->Get(baseHistName + addNameSyst[iSyst]);
        arrHistsFile1[iSyst]->SetLineColor(kBlue);
        arrHistsFile1[iSyst]->SetFillColor(kBlue);
        arrHistsFile1[iSyst]->SetFillStyle(3001);
        arrHistsFile1[iSyst]->Scale(1./currIntegral);
        currControlBinFile1[iSyst] = arrHistsFile1[iSyst]->GetBinContent(1,1,1);
        arrHistsFile1[iSyst]->Scale(currControlBinFile0[iSyst]/currControlBinFile1[iSyst]);
        /*
        if (iSyst != 0) {
            arrHistsFile1[iSyst]->Scale(currControlBinFile1[0]/currControlBinFile1[iSyst]);
        }
         */
    }
    
    TH1F * h_OneDeeFile0 = new TH1F("h_OneDeeFile0", ";3D M_{T2} Shape Global Bin; Number of Events / Bin", 64, 0.5, 64.5); h_OneDeeFile0->Sumw2();
    TH1F * h_OneDeeFile1 = new TH1F("h_OneDeeFile1", ";3D M_{T2} Shape Global Bin; Number of Events / Bin", 64, 0.5, 64.5); h_OneDeeFile1->Sumw2();

    
    int binCounter = 0;
    double currBinFile0[3], currBinFile1[3];
    double currBinErrorFile0[3], currBinErrorFile1[3];
    double currRatio[3];
    double currRatioBinSystErrUp[64], currRatioBinSystErrDown[64];
    
    float currDiffShiftUpFile0, currDiffShiftUpFile1;
    float currDiffShiftDownFile0, currDiffShiftDownFile1;

    for (int iBinX = 1; iBinX <= 4; ++iBinX) {
        for (int iBinY = 1; iBinY <= 4; ++iBinY) {
            for (int iBinZ = 1; iBinZ <= 4; ++iBinZ) {
                binCounter++;
                for (int iSyst = 0; iSyst < 3; ++iSyst) {
                    currBinFile0[iSyst] = arrHistsFile0[iSyst]->GetBinContent(iBinX,iBinY,iBinZ);
                    currBinErrorFile0[iSyst] = arrHistsFile0[iSyst]->GetBinError(iBinX,iBinY,iBinZ);
                    currBinFile1[iSyst] = arrHistsFile1[iSyst]->GetBinContent(iBinX,iBinY,iBinZ);
                    currBinErrorFile1[iSyst] = arrHistsFile1[iSyst]->GetBinError(iBinX,iBinY,iBinZ);
                }
                h_OneDeeFile0->SetBinContent(binCounter, currBinFile0[0]);
                h_OneDeeFile0->SetBinError(binCounter, currBinErrorFile0[0]);
                h_OneDeeFile1->SetBinContent(binCounter, currBinFile1[0]);
                h_OneDeeFile1->SetBinError(binCounter, currBinErrorFile1[0]);
                
                h_OneDeeFile1->SetLineColor(kBlue);
                h_OneDeeFile1->SetFillColor(kBlue);
                h_OneDeeFile1->SetFillStyle(3001);
                
                currDiffShiftUpFile0 = scaleGenDiff * TMath::Abs(currBinFile0[1] - currBinFile0[0]);
                currDiffShiftUpFile1 = scaleGenDiff * TMath::Abs(currBinFile1[1] - currBinFile1[0]);
                
                currDiffShiftDownFile0 = scaleGenDiff * TMath::Abs(currBinFile0[2] - currBinFile0[0]);
                currDiffShiftDownFile1 = scaleGenDiff * TMath::Abs(currBinFile1[2] - currBinFile1[0]);
                if (doVerb) {
                    cout << "binCounter " << binCounter << endl;
                    cout << "currBinFile0[0] " << currBinFile0[0] << endl;
                    cout << "currDiffShiftUp " << TMath::Max(currDiffShiftUpFile0, currDiffShiftUpFile1) << endl;
                    cout << "currDiffShiftDown " << TMath::Max(currDiffShiftDownFile0, currDiffShiftDownFile1) << endl;
                }
                
//                pointSystErr(currBinFile0[0]/currBinFile1[0], currBinFile0[1]/currBinFile1[1], currBinFile0[2]/currBinFile1[2], currRatioBinSystErrUp[binCounter - 1], currRatioBinSystErrDown[binCounter - 1], false);
////                pointSystErr(currBinFile0[0]/currBinFile1[0], currBinFile0[1]/currBinFile1[0], currBinFile0[2]/currBinFile1[0], currRatioBinSystErrUp[binCounter - 1], currRatioBinSystErrDown[binCounter - 1], false);
                pointSystErr(currBinFile0[0], currBinFile0[0] + TMath::Max(currDiffShiftUpFile0, currDiffShiftUpFile1), currBinFile0[0] - TMath::Max(currDiffShiftDownFile0, currDiffShiftDownFile1), currRatioBinSystErrUp[binCounter - 1], currRatioBinSystErrDown[binCounter - 1], false);
            }
        }
    }
    TCanvas * c1 = new TCanvas("c_Test","c_Test");
    h_OneDeeFile0->Draw("e1");
    h_OneDeeFile1->Draw("hist same");
    
//    TH1F * h_OneDeeRatio = new TH1F("h_OneDeeRatio", ";3D M_{T2} Shape Global Bin; File0/MC@NLO", 64, 0.5, 64.5); h_OneDeeRatio->Sumw2;
    TH1F * h_OneDeeRatio = (TH1F *) h_OneDeeFile0->Clone("h_OneDeeRatio");
    h_OneDeeRatio->Divide(h_OneDeeFile0, h_OneDeeFile1);
    
    int nBinsX = h_OneDeeRatio->GetNbinsX();
    TGraphAsymmErrors * outGraph = new TGraphAsymmErrors(64);
    Double_t x;
    
    float currUpErr, currDownErr;
    
    
    TH1F * h_OneDeeRatio_Signif = (TH1F *) h_OneDeeRatio->Clone("h_OneDeeRatio_Signif");
    
    float currTotalError, currSignif;
    for (int iPoint = 1; iPoint <= nBinsX; ++iPoint) {
        x = (Double_t) h_OneDeeRatio->GetBinCenter(iPoint);
//        outGraph->SetPoint(iPoint, x, 1);
        
        currRatio[0] = h_OneDeeRatio->GetBinContent(iPoint);
        
        outGraph->SetPoint(iPoint, x, currRatio[0]);
        float binWidth = (Double_t) h_OneDeeRatio->GetXaxis()->GetBinWidth(iPoint);
        outGraph->SetPointEXlow(iPoint, binWidth/2.);
        outGraph->SetPointEXhigh(iPoint, binWidth/2.);
        /*
        cout << "iPoint " << iPoint << endl;
        cout << "currRatioBinSystErrUp[iPoint - 1] " << currRatioBinSystErrUp[iPoint - 1] << endl;
        */
        if (currRatio[0] < 1E-9) {
            currUpErr = 0;
            currDownErr = 0;
        }
        else {
            if (doVerb) {
                cout << "iPoint " << iPoint << endl;
                cout << "h_OneDeeFile0->GetBinContent(iPoint) " << h_OneDeeFile0->GetBinContent(iPoint) << endl;
                cout << "currRatioBinSystErrUp[iPoint - 1] " << currRatioBinSystErrUp[iPoint - 1] << endl;
                cout << "currRatioBinSystErrDown[iPoint - 1] " << currRatioBinSystErrDown[iPoint - 1] << endl;
            }
            currUpErr = currRatioBinSystErrUp[iPoint - 1] / h_OneDeeFile0->GetBinContent(iPoint);
            currDownErr = currRatioBinSystErrDown[iPoint - 1] / h_OneDeeFile0->GetBinContent(iPoint);
        }
        
        outGraph->SetPointEYhigh(iPoint, currUpErr);
        outGraph->SetPointEYlow(iPoint, currDownErr);
        
        currTotalError = GetTotalError(currUpErr, currDownErr, h_OneDeeRatio->GetBinError(iPoint));
        currSignif = (currRatio[0] - 1) / currTotalError;
        h_OneDeeRatio_Signif->SetBinContent(iPoint, currSignif);
    }
    outGraph->SetFillColor(kRed - 2);
    outGraph->SetFillStyle(3001);
    
    TString canvName_Ratio = baseCanvNameRatio + canvAddName;
    TCanvas * c_Ratio = new TCanvas(canvName_Ratio, canvName_Ratio);
    h_OneDeeRatio->Draw("e1");
    
    
    TGraphAsymmErrors * fracRatioDrawGraph = ClonePoints(h_OneDeeRatio, false, true);
    fracRatioDrawGraph->SetLineColor(kBlack);
    HistToGraphCopyAttributes(h_OneDeeRatio, fracRatioDrawGraph);
    
    TString nameYaxisRatio = legendNameTTBarGens[whichGenFile0];
    nameYaxisRatio += "/";
    nameYaxisRatio += legendNameTTBarGens[whichGenFile1];
    
    h_OneDeeRatio->GetYaxis()->SetTitle(nameYaxisRatio);
    h_OneDeeRatio->GetYaxis()->SetRangeUser(0.15, 1.85);
    fracRatioDrawGraph->Draw("p0 same");
    outGraph->Draw("2 same");
    
    TAxis * XAxis = h_OneDeeRatio->GetXaxis();
    float xMin = XAxis->GetBinLowEdge(XAxis->GetFirst());
    float xMax = XAxis->GetBinUpEdge(XAxis->GetLast());
    TLine * line = new TLine(xMin, 1, xMax, 1);
    line->SetLineColor(kRed+1);
    line->SetLineStyle(7);
    line->SetLineWidth(2);
    line->Draw("same");
    b.DrawLatex(xValLatexDilep, yValLatexDilep, strLatexDilep);
    
//    TLegend * legendRatio = new TLegend(0.65, 0.65, 0.8, 0.8);
    TLegend * legendRatio = new TLegend(0.47, 0.8, 0.86, 0.87);
    legendRatio->AddEntry(outGraph, "Gen-ISR Uncertainty", "f");
    legendRatio->Draw("same");
    c_Ratio->SaveAs(canvName_Ratio + TString(".pdf"));
    
    
    TString canvName_RatioSignif = baseCanvNameRatioSignif + canvAddName;
    TCanvas * c_RatioSignif = new TCanvas(canvName_RatioSignif, canvName_RatioSignif);
    Color_t fillColorSignif = kBlue;
    Color_t lineColorSignif = kRed;
    Style_t styleMarker = 0;
    
    h_OneDeeRatio_Signif = SignifHist(h_OneDeeRatio, outGraph);
    
    HistMainAttSet(h_OneDeeRatio_Signif, fillColorSignif, 3001, lineColorSignif, 2, kBlack, 20, styleMarker);
    
    h_OneDeeRatio_Signif->Draw("hist");
    h_OneDeeRatio_Signif->GetYaxis()->SetRangeUser(-4, 4);
    
    TLine * lineOnePlus = new TLine(xMin, 1, xMax, 1);
    lineOnePlus->SetLineColor(kRed+1);
    lineOnePlus->SetLineStyle(7);
    lineOnePlus->SetLineWidth(2);
    lineOnePlus->Draw("same");
    
    TLine * lineOneMinus = new TLine(xMin, -1, xMax, -1);
    lineOneMinus->SetLineColor(kRed+1);
    lineOneMinus->SetLineStyle(7);
    lineOneMinus->SetLineWidth(2);
    lineOneMinus->Draw("same");
    

    b.DrawLatex(xValLatexDilep, yValLatexDilep, strLatexDilep);
    c.DrawLatex(xValLatexGenerator, yValLatexGenerator, strLatexGenerator);
    c_RatioSignif->SaveAs(canvName_RatioSignif + TString(".pdf"));

    
    TString canvName_ShapeCorr = baseCanvNameShapeCorr + canvAddName;
    TCanvas * c_ShapeCorr = new TCanvas(canvName_ShapeCorr, canvName_ShapeCorr);
    TH1F * h_OneDeeRatio_Corrected = CorrectedRatioHist(h_OneDeeRatio, h_OneDeeRatio_Signif, sigma);
    
    HistMainAttSet(h_OneDeeRatio_Corrected, fillColorSignif, 3001, lineColorSignif, 2, kBlack, 20, styleMarker);
    nameYaxisRatio = "Relative Shape Uncertainty";
    h_OneDeeRatio_Corrected->GetYaxis()->SetTitle(nameYaxisRatio);
    h_OneDeeRatio_Corrected->GetYaxis()->SetRangeUser(0.0, 1.5);
    
    h_OneDeeRatio_Corrected->Draw("hist");
    b.DrawLatex(xValLatexDilep, yValLatexDilep, strLatexDilep);
    c.DrawLatex(xValLatexGenerator, yValLatexGenerator, strLatexGenerator);
    c_ShapeCorr->SaveAs(canvName_ShapeCorr + TString(".pdf"));
    

    TString canvName_RatioSignifCorr = baseCanvNameRatioSignif + canvAddName + TString("_Corr");
    TCanvas * c_RatioSignifCorr = new TCanvas(canvName_RatioSignifCorr, canvName_RatioSignifCorr);
    
    TH1F * h_OneDeeRatio_Corr_Signif = SignifHist(h_OneDeeRatio, outGraph, h_OneDeeRatio_Corrected);
    
    HistMainAttSet(h_OneDeeRatio_Corr_Signif, fillColorSignif, 3001, lineColorSignif, 2, kBlack, 20, styleMarker);
    
    h_OneDeeRatio_Corr_Signif->Draw("hist");
    h_OneDeeRatio_Corr_Signif->GetYaxis()->SetRangeUser(-4, 4);
    h_OneDeeRatio_Corr_Signif->GetYaxis()->SetTitle("Post-Correction Significance");
    lineOnePlus->Draw("same");
    lineOneMinus->Draw("same");
    b.DrawLatex(xValLatexDilep, yValLatexDilep, strLatexDilep);
    c.DrawLatex(xValLatexGenerator, yValLatexGenerator, strLatexGenerator);
    c_RatioSignifCorr->SaveAs(canvName_RatioSignifCorr + TString(".pdf"));
    
    
    TFile * outFile = new TFile(outFileName + TString(".root"), "RECREATE");
    outFile->cd();
    h_OneDeeRatio_Corrected->Write();
    outFile->Write();
    outFile->Close();
    theApp.Run(retVal);
}
