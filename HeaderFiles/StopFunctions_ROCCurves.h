#include <iostream>
#include "TTree.h"
#include "TMath.h"
#include "TEventList.h"
#include "TF1.h"

#include "TGraph.h"
#include "TLegend.h"

#include "TFile.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TRint.h"
#include <vector>
#include <cmath>

#include "TString.h"

//Functions used for plotting/setting-up ROC and/or Punzi curves -- need to work on this


void BaseCanvasSetup(TCanvas * inputCanvas, bool logYPad1) {
    inputCanvas->Divide(1, 2);
    TPad * Pad1 = (TPad *) inputCanvas->cd(1);
    FixPad(Pad1, 1, inputCanvas);
    Pad1->SetLogy(logYPad1);
    Pad1->SetGridy(1);
    TPad * botPad = (TPad *) inputCanvas->cd(2);
    FixPad(botPad, 2, inputCanvas);
    botPad->SetGridy(1);
}

TString JointString(bool doAND, vector<TString> * inVecString) {
    TString outStr = doAND ? "AND: " : "OR: ";
    outStr += inVecString->at(0);
    for (int iString = 1; iString < (int) inVecString->size(); ++iString) {
        outStr += ", ";
        outStr += inVecString->at(iString);
    }
    return outStr;
}

void SetCanvases(vector<TCanvas *> * inVecCanvas, vector<TString> * inVecCanvasNames, bool dualPad, bool doLog) {
    
    int singCanvSize = 800;
    
    for (unsigned int iCanv = 0; iCanv < inVecCanvas->size(); ++iCanv) {
        if (dualPad) {
            inVecCanvas->at(iCanv) = new TCanvas(inVecCanvasNames->at(iCanv), inVecCanvasNames->at(iCanv));
            BaseCanvasSetup(inVecCanvas->at(iCanv), doLog);
        }
        else {
            inVecCanvas->at(iCanv) = new TCanvas(inVecCanvasNames->at(iCanv), inVecCanvasNames->at(iCanv), singCanvSize, singCanvSize);
            inVecCanvas->at(iCanv)->SetLogy(doLog);
            inVecCanvas->at(iCanv)->SetGridy(1);
            inVecCanvas->at(iCanv)->SetLogx(doLog);
            inVecCanvas->at(iCanv)->SetGridx(1);
        }
    }
}

float FirstPointDeltX(TGraph * inROCCurve, float thresh) {
    //compute the first point where the difference in x values between
    //successive cuts is smaller than a threshold and return the y-value at the point
    //(used for setting maxima of plots to be well defined)
    
    //note code assumes monotonicity in the x curve structure
    //(i.e. cardinal ordering of x points)
    
    double x1, x2;
    double y1, y2;
    int nPoints = inROCCurve->GetN();
//    int endPoint = nPoints - 1;
    for (int iPoint = 0; iPoint < nPoints - 1; ++iPoint) {
        inROCCurve->GetPoint(iPoint, x1, y1);
        inROCCurve->GetPoint(iPoint+1, x2, y2);
        if (x2 - x1 < thresh) {
            /*
            cout << "iPoint " << iPoint << endl;
            cout << "x2 " << x2 << endl;
            cout << "x1 " << x1 << endl;
            cout << "y2 " << y2 << endl;
            cout << "y1 " << y1 << endl;
            */
            break;
        }
    }
    return y2;
}

int WhatTypeOfPunzi(TGraph * inPunzi) {
    double x1, x2, x3;
    double y1, y2, y3;
    int nPoints = inPunzi->GetN();
    inPunzi->GetPoint(0, x1, y1);
    inPunzi->GetPoint(nPoints/2, x2, y2);
    inPunzi->GetPoint(nPoints - 1, x3, y3);
    
    int outType = 0;
    if (y1 > y3) {
        if (y2 > y3) {
            //stays upward
            outType = 0;
        }
        else {
            //slopes downward
            outType = 3;
        }
    }
    else {
        outType = 2;
    }
    return outType;
}

void FinalDrawGraph(TCanvas * inputCanvas, vector<TGraph *> * inputGraphVec, vector<TString> * inVecLegEntries, TString latexStringSig, bool isEff) {
    inputCanvas->cd(1);
    double xposLatex, yposLatex;
    xposLatex = 0.1;
    yposLatex = 0.9;
    
    double x1, x2, y1, y2;
    
    x1 = 0.1;
    x2 = 0.70;
    y1 = 0.15;
    y2 = 0.4;
    
    double x1Punzi[4] = {0.1, 0.35, 0.1, 0.35};
    double x2Punzi[4] = {0.6, 0.85, 0.6, 0.85};
    double y1Punzi[4] = {0.15, 0.15, 0.6, 0.6};
    double y2Punzi[4] = {0.4, 0.4, 0.85, 0.85};
    
    if (!isEff) {
        int typePunzi;
        typePunzi = WhatTypeOfPunzi(inputGraphVec->at(0));
        x1 = x1Punzi[typePunzi];
        x2 = x2Punzi[typePunzi];
        y1 = y1Punzi[typePunzi];
        y2 = y2Punzi[typePunzi];
    }
    
    TLegend * leg = new TLegend(x1, y1, x2, y2);
    for (unsigned int iGraph = 0; iGraph < inputGraphVec->size(); ++iGraph) {
        leg->AddEntry(inputGraphVec->at(iGraph), inVecLegEntries->at(iGraph), "p");
    }
    leg->Draw("same");
    
    TLatex sigString;
    sigString.SetNDC();
    sigString.SetTextSize(0.04);
    sigString.DrawLatex(xposLatex, yposLatex, latexStringSig);
    
//    inputCanvas->SaveAs(inputCanvas->GetName() + TString(".C"));
    inputCanvas->SaveAs(inputCanvas->GetName() + TString(".pdf"));
}

void InitialDrawGraphsROCCurve(TCanvas * inputCanvas, vector<TGraph *> * inputGraphVec) {
    inputCanvas->cd();
    inputGraphVec->at(0)->Draw("ACP");
    
    TAxis * yAxis = inputGraphVec->at(0)->GetYaxis();
    
    float thresh = 2.5E-3;
    float goodY = FirstPointDeltX(inputGraphVec->at(0), thresh);
    //        cout << "goodY " << goodY << endl;
    yAxis->SetRangeUser(goodY, 1.0);
    
    yAxis->SetTitleOffset(1.0);
    for (int iGraph = 1; iGraph < (int) inputGraphVec->size(); ++iGraph) {
        inputGraphVec->at(iGraph)->Draw("PC same");
    }
}

void InitialDrawGraphsPunzi(TCanvas * inputCanvas, vector<TGraph *> * inputGraphVec, vector<TGraph *> * inputRatioGraphVec) {
    inputCanvas->cd(1);
    inputGraphVec->at(0)->Draw("ACP");
    
    TAxis * yAxis = inputGraphVec->at(0)->GetYaxis();
    
    yAxis->SetTitleOffset(1.0);
    for (int iGraph = 1; iGraph < (int) inputGraphVec->size(); ++iGraph) {
        inputGraphVec->at(iGraph)->Draw("PC same");
    }
    /*
    inputCanvas->cd(2);
    
    inputRatioGraphVec->at(0)->Draw("ACP");
    for (int iGraph = 1; iGraph < (int) inputRatioGraphVec->size(); ++iGraph) {
        inputRatioGraphVec->at(iGraph)->Draw("PC same");
    }
     */
}

/*
void InitialDrawGraphsPunzi(TCanvas * inputCanvas, vector<TGraph *> * inputGraphVec, vector<TGraph *> * inputRatioGraphVec) {
    inputCanvas->cd(1);
    inputGraphVec->at(0)->Draw("ACP");
    
    TAxis * yAxis = inputGraphVec->at(0)->GetYaxis();
    
    yAxis->SetTitleOffset(1.0);
    for (int iGraph = 1; iGraph < (int) inputGraphVec->size(); ++iGraph) {
        inputGraphVec->at(iGraph)->Draw("PC same");
    }
    
    inputCanvas->cd(2);
    
    inputRatioGraphVec->at(0)->Draw("ACP");
    for (int iGraph = 1; iGraph < (int) inputRatioGraphVec->size(); ++iGraph) {
        inputRatioGraphVec->at(iGraph)->Draw("PC same");
    }
}
*/

TString PunziGraphName(TString preString, TString postString, float nSigmaA, float nSigmaB, int wCheck = -1) {
    TString outNamePunziGraphMT2lblb = preString;
    outNamePunziGraphMT2lblb += "SigmaA";
    outNamePunziGraphMT2lblb += nSigmaA;
    outNamePunziGraphMT2lblb += "SigmaB";
    outNamePunziGraphMT2lblb += nSigmaB;
    outNamePunziGraphMT2lblb += postString;
    if (wCheck > -1) {
        outNamePunziGraphMT2lblb += wCheck;
    }
    return outNamePunziGraphMT2lblb;
}

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

float GetSFBkg(int whichBkg) {
    int L_Data = 19656;
    //0 -- TTBar Comp
    const int numBkgs = 2;
    float xSec[numBkgs] = {245.8, 1.};
    float numEvents[numBkgs] = {28150723., 1.};
    return L_Data * xSec[whichBkg] / numEvents[whichBkg];
}

void SetColorsROCCurves(vector<Color_t> * inVecColor) {
    inVecColor->push_back(kBlack);
    inVecColor->push_back(kRed);
    inVecColor->push_back(kBlue);
    inVecColor->push_back(kGreen + 1);
    inVecColor->push_back(kYellow + 2);
}

void SetStyleROCCurves(vector<Style_t> * inVecStyle) {
    inVecStyle->push_back(2);
    inVecStyle->push_back(20);
    inVecStyle->push_back(21);
    inVecStyle->push_back(22);
    inVecStyle->push_back(29);
}


void SetCanvasNamesROCCurves_DY(vector<TString> * inVecString, TString addString = "") {
    TString strBase = "c_ROCCurves_";
    strBase += addString;
    TString strDY = strBase + TString("DYSelectors");
    
    inVecString->push_back(strDY);
}

void SetCanvasNamesPunziCurves_DY(vector<TString> * inVecString, TString addString = "") {
    TString strBase = "c_PunziCurvesSelector_DY";
    strBase += addString;
    TString strDY = strBase + TString("DYKillers");
    
    inVecString->push_back(strDY);
}

void SetIndicesROCCurves_DY(vector< vector<int> > * inVecVecInt) {
    //group all ROC curves together
    
    vector<int> vecIndex_DYKillers(0);
    vecIndex_DYKillers.push_back(0);
    vecIndex_DYKillers.push_back(1);
    vecIndex_DYKillers.push_back(2);
    vecIndex_DYKillers.push_back(3);
    vecIndex_DYKillers.push_back(4);
    inVecVecInt->push_back(vecIndex_DYKillers);
}

TString SetFileString_DY(int whichSig) {
    const int numSigs = 9;
    TString strSigs[numSigs] = {"TTBar", "T2ttSuperHighMass", "T2ttHighMass", "T2ttLowMass", "T2bwHighMassx0.75", "T2bwHighMassx0.25", "T2bwLowMassx0.75", "T2bwLowMassx0.50", "T2bwLowMassx0.25"};
    return strSigs[whichSig];
}


void SetLegendStringsROCCurves_DY(vector<TString> * inVecString) {
    
    TString xAxisCutStringMET = "SF #slash{E}_{T} cut";
    TString xAxisCutStringMETSig1DHack = "SF #slash{E}^{2}_{T}/#sigma^{2}_{#slash{E}_{T}} cut";
    TString xAxisCutStringMETSig2DHack = "SF #slash{E}^{2}_{T}/#sigma^{2}_{#slash{E}_{T}} cut";
    TString xAxisCutStringMETSig2DTrue = "SF #slash{E}_{T} significance cut";
    TString xAxisCutStringMETSig2DTrue_2 = "#slash{E}_{T} significance cut";
    
    inVecString->push_back(xAxisCutStringMET);
    inVecString->push_back(xAxisCutStringMETSig1DHack);
    inVecString->push_back(xAxisCutStringMETSig2DHack);
    inVecString->push_back(xAxisCutStringMETSig2DTrue);
    inVecString->push_back(xAxisCutStringMETSig2DTrue_2);
}


void SetCanvasNamesROCCurves_MT2lblb(vector<TString> * inVecString, TString addString = "") {
    TString strBase = "c_ROCCurvesMT2lblbSelector_";
    strBase += addString;
    TString strSing = strBase + TString("SingleVars");
    TString strMinMT2AND = strBase + TString("MinMT2AND");
    TString strMinMT2OR = strBase + TString("MinMT2OR");
    TString strTriple = strBase + TString("Triple");
    TString strMinMaxMass_MassDiff_ANDOR = strBase + TString("MinMassDiffMaxMass_ANDOR");
    TString strMinDiffTopMass_MassDiff_ANDOR = strBase + TString("MinDiffTopMassMaxMass_ANDOR");
    
    inVecString->push_back(strSing);
    inVecString->push_back(strMinMT2AND);
    inVecString->push_back(strMinMT2OR);
    inVecString->push_back(strTriple);
    inVecString->push_back(strMinMaxMass_MassDiff_ANDOR);
    inVecString->push_back(strMinDiffTopMass_MassDiff_ANDOR);
}

void SetCanvasNamesPunziCurves_MT2lblb(vector<TString> * inVecString, TString addString = "") {
    TString strBase = "c_PunziCurvesMT2lblbSelector_";
    strBase += addString;
    TString strSing = strBase + TString("SingleVars");
    TString strMinMT2AND = strBase + TString("MinMT2AND");
    TString strMinMT2OR = strBase + TString("MinMT2OR");
    TString strTriple = strBase + TString("Triple");
    TString strMinMaxMass_MassDiff_ANDOR = strBase + TString("MinMassDiffMaxMass_ANDOR");
    TString strMinDiffTopMass_MassDiff_ANDOR = strBase + TString("MinDiffTopMassMaxMass_ANDOR");
    
    inVecString->push_back(strSing);
    inVecString->push_back(strMinMT2AND);
    inVecString->push_back(strMinMT2OR);
    inVecString->push_back(strTriple);
    inVecString->push_back(strMinMaxMass_MassDiff_ANDOR);
    inVecString->push_back(strMinDiffTopMass_MassDiff_ANDOR);
}
void SetIndicesROCCurves_MT2lblb(vector< vector<int> > * inVecVecInt) {
    //group single guys together
    //0, 1, 2, 11
    
    //group duals minMT2 ANDs together
    //0, 3, 5, 15
    
    //group duals minMT2 ORs together
    //0, 4, 6, 14
    
    //group triple ANDs and ORs
    //0, 9, 10
    
    //group ANDs and ORs of minMaxMass and minMassDiff
    //0, 7, 8
    
    //group ANDs and ORs of minAbsDiffMassTop and minMassDiff
    //0, 12, 13
    
    
    vector<int> vecIndex_SinglePlots(0);
    vecIndex_SinglePlots.push_back(0);
    vecIndex_SinglePlots.push_back(1);
    vecIndex_SinglePlots.push_back(2);
    vecIndex_SinglePlots.push_back(11);
    inVecVecInt->push_back(vecIndex_SinglePlots);
    
    vector<int> vecIndex_MinMT2_ANDPlots(0);
    vecIndex_MinMT2_ANDPlots.push_back(0);
    vecIndex_MinMT2_ANDPlots.push_back(3);
    vecIndex_MinMT2_ANDPlots.push_back(5);
    vecIndex_MinMT2_ANDPlots.push_back(15);
    inVecVecInt->push_back(vecIndex_MinMT2_ANDPlots);
    
    vector<int> vecIndex_MinMT2_ORPlots(0);
    vecIndex_MinMT2_ORPlots.push_back(0);
    vecIndex_MinMT2_ORPlots.push_back(4);
    vecIndex_MinMT2_ORPlots.push_back(6);
    vecIndex_MinMT2_ORPlots.push_back(14);
    inVecVecInt->push_back(vecIndex_MinMT2_ORPlots);
    
    vector<int> vecIndex_Triple_AND_ORPlots(0);
    vecIndex_Triple_AND_ORPlots.push_back(0);
    vecIndex_Triple_AND_ORPlots.push_back(9);
    vecIndex_Triple_AND_ORPlots.push_back(10);
    inVecVecInt->push_back(vecIndex_Triple_AND_ORPlots);

    vector<int> vecIndex_MinMaxMass_MinMassDiff_AND_OR_Plots(0);
    vecIndex_MinMaxMass_MinMassDiff_AND_OR_Plots.push_back(0);
    vecIndex_MinMaxMass_MinMassDiff_AND_OR_Plots.push_back(7);
    vecIndex_MinMaxMass_MinMassDiff_AND_OR_Plots.push_back(8);
    inVecVecInt->push_back(vecIndex_MinMaxMass_MinMassDiff_AND_OR_Plots);
    
    vector<int> vecIndex_MinAbsTopDiff_MinMassDiff_AND_OR_Plots(0);
    vecIndex_MinAbsTopDiff_MinMassDiff_AND_OR_Plots.push_back(0);
    vecIndex_MinAbsTopDiff_MinMassDiff_AND_OR_Plots.push_back(12);
    vecIndex_MinAbsTopDiff_MinMassDiff_AND_OR_Plots.push_back(13);
    inVecVecInt->push_back(vecIndex_MinAbsTopDiff_MinMassDiff_AND_OR_Plots);
}

TString SetFileString_MT2lblb(int whichSig) {
    const int numSigs = 8;
    TString strSigs[numSigs] = {"T2ttSuperHighMass", "T2ttHighMass", "T2ttLowMass", "T2bwHighMassx0.75", "T2bwHighMassx0.25", "T2bwLowMassx0.75", "T2bwLowMassx0.50", "T2bwLowMassx0.25"};
    return strSigs[whichSig];
}

TString SetTLatexSignal(int whichSig, bool isT2tt) {
    TString stringStopMass = "M_{#tilde{t}}: ";
    TString stringLSPMass = "M_{#chi_{0}}: ";
    //    TString stringCharginoMass = "M_{#chi^{#pm}}: ";
    TString stringCharginoMass = "#chi_{1}^{#pm} x ";
    
    TString stringT2tt = "T2tt: ";
    TString stringT2bw = "T2bw: ";
    
    const int numSigs = 8;
    TString superHighStopMassT2tt = "925-1000";
    TString highStopMassT2tt = "675-800";
    TString lowStopMassT2tt = "150-250";
    TString highStopMassT2bw = "500-650";
    TString lowStopMassT2bw = "125-475";
    
    TString superHighLSPMassT2tt = "0-900";
    TString highLSPMassT2tt = "0-700";
    TString lowLSPMassT2tt = "0-100";
    TString highLSPMassT2bw = "150-300";
    TString lowLSPMassT2bw = "25-300";
    
    vector<TString> vecStrStopMass;
    vecStrStopMass.push_back(superHighStopMassT2tt);
    vecStrStopMass.push_back(highStopMassT2tt);
    vecStrStopMass.push_back(lowStopMassT2tt);
    vecStrStopMass.push_back(highStopMassT2bw);
    vecStrStopMass.push_back(highStopMassT2bw);
    vecStrStopMass.push_back(lowStopMassT2bw);
    vecStrStopMass.push_back(lowStopMassT2bw);
    vecStrStopMass.push_back(lowStopMassT2bw);
    
    vector<TString> vecStrLSPMass;
    vecStrLSPMass.push_back(superHighLSPMassT2tt);
    vecStrLSPMass.push_back(highLSPMassT2tt);
    vecStrLSPMass.push_back(lowLSPMassT2tt);
    vecStrLSPMass.push_back(highLSPMassT2bw);
    vecStrLSPMass.push_back(highLSPMassT2bw);
    vecStrLSPMass.push_back(lowLSPMassT2bw);
    vecStrLSPMass.push_back(lowLSPMassT2bw);
    vecStrLSPMass.push_back(lowLSPMassT2bw);
    
    TString outStr = isT2tt ? stringT2tt : stringT2bw;
    outStr += stringStopMass + vecStrStopMass[whichSig];
    outStr += ", ";
    outStr += stringLSPMass + vecStrLSPMass[whichSig];
    if (!isT2tt) {
        outStr += ", ";
        outStr += stringCharginoMass;
        if (whichSig == 3 || whichSig == 5) {
            outStr += "0.75";
        }
        else if (whichSig == 6) {
            outStr += "0.50";
        }
        else if (whichSig == 4 || whichSig == 7) {
            outStr += "0.25";
        }
        else {
            cout << "whichSig is out of wack " << endl;
        }
    }
    return outStr;
}

void SetLegendStringsROCCurves_MT2lblb(vector<TString> * inVecString) {
    TString strMinMT2 = "Min. M_{T2}(lb)(lb)";
    TString strMinMaxMass = "Min. Max(m_{bl,1}, m_{bl,2})";
    TString strMinMassDiff = "Min. |m_{bl,1} - m_{bl,2}|";
    TString strMinMinAbsTopMassDiff = "Min. Min(#Delta(m_{bl,1}, m_{bl,2}))";
    
    vector<TString> vecStr_MinMT2_MinMaxMass(0);
    vecStr_MinMT2_MinMaxMass.push_back(strMinMT2);
    vecStr_MinMT2_MinMaxMass.push_back(strMinMaxMass);
    
    vector<TString> vecStr_MinMT2_MinMassDiff(0);
    vecStr_MinMT2_MinMassDiff.push_back(strMinMT2);
    vecStr_MinMT2_MinMassDiff.push_back(strMinMassDiff);
    
    vector<TString> vecStr_MinMaxMass_MinMassDiff(0);
    vecStr_MinMaxMass_MinMassDiff.push_back(strMinMaxMass);
    vecStr_MinMaxMass_MinMassDiff.push_back(strMinMassDiff);

    vector<TString> vecStr_MinMT2_MinMaxMass_MinMassDiff(0);
    vecStr_MinMT2_MinMaxMass_MinMassDiff.push_back(strMinMT2);
    vecStr_MinMT2_MinMaxMass_MinMassDiff.push_back(strMinMaxMass);
    vecStr_MinMT2_MinMaxMass_MinMassDiff.push_back(strMinMassDiff);

    vector<TString> vecStr_MinMT2_MinTopMassDiff(0);
    vecStr_MinMT2_MinTopMassDiff.push_back(strMinMT2);
    vecStr_MinMT2_MinTopMassDiff.push_back(strMinMinAbsTopMassDiff);

    vector<TString> vecStr_MinMassDiff_MinTopMassDiff(0);
    vecStr_MinMassDiff_MinTopMassDiff.push_back(strMinMinAbsTopMassDiff);
    vecStr_MinMassDiff_MinTopMassDiff.push_back(strMinMassDiff);
    
    inVecString->push_back(strMinMT2);
    inVecString->push_back(strMinMaxMass);
    inVecString->push_back(strMinMassDiff);
    inVecString->push_back(JointString(true, &vecStr_MinMT2_MinMaxMass));
    inVecString->push_back(JointString(false, &vecStr_MinMT2_MinMaxMass));
    inVecString->push_back(JointString(true, &vecStr_MinMT2_MinMassDiff));
    inVecString->push_back(JointString(false, &vecStr_MinMT2_MinMassDiff));
    inVecString->push_back(JointString(true, &vecStr_MinMaxMass_MinMassDiff));
    inVecString->push_back(JointString(false, &vecStr_MinMaxMass_MinMassDiff));
    inVecString->push_back(JointString(true, &vecStr_MinMT2_MinMaxMass_MinMassDiff));
    inVecString->push_back(JointString(false, &vecStr_MinMT2_MinMaxMass_MinMassDiff));
    inVecString->push_back(strMinMinAbsTopMassDiff);
    inVecString->push_back(JointString(false, &vecStr_MinMassDiff_MinTopMassDiff));
    inVecString->push_back(JointString(true, &vecStr_MinMassDiff_MinTopMassDiff));
    inVecString->push_back(JointString(false, &vecStr_MinMT2_MinTopMassDiff));
    inVecString->push_back(JointString(true, &vecStr_MinMT2_MinTopMassDiff));
}