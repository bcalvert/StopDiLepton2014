#include <iostream>
#include "TMath.h"
#include "../HeaderFiles/StopPlottingHeaderFiles.h"
/*
#include "../HeaderFiles/BasicFunctions.h"
#include "../HeaderFiles/HistogramSystematics2.h"
#include "../HeaderFiles/HistogramSystematicsStructs.h"
#include "../HeaderFiles/StopStructDefinitions.h"
#include "../HeaderFiles/StopFunctionDefinitions_Plots.h"
#include "../HeaderFiles/GeneralPlotDrawingStructs.h"
#include "../HeaderFiles/GeneralPlotDrawingFunctions.h"
#include "../HeaderFiles/PlotDisplayStructs.h"
#include "../HeaderFiles/GeneralPlotDrawingFunctions_Set2.h"
#include "../HeaderFiles/StopPlotStructs_PlottingMacro.h"
#include "../HeaderFiles/StopPlotSetup_FileSetup.h"
#include "../HeaderFiles/StopPlotSetup_CustomPlots.h"
#include "../HeaderFiles/StopPlotStructs.h"
#include "../HeaderFiles/StopPlotSetup_ISPIFunctions_Set2.h"
#include "../HeaderFiles/StopPlotSetup.h"
*/

#include "TH1F.h"
#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TRint.h"
#include <vector>
#include <cmath>
using namespace std;
int main( int argc, char* argv[]) {
    gROOT->ProcessLine("#include <vector>");
    // Style things
    gStyle->SetErrorX(0.5); // set X uncertainty to 1/2 of a bin
    gStyle->SetTitleFont(42);        
//    gStyle->SetLegendFont(42);
    
    //./StopPlotsSpectraShowNewDESY_wSyst -b -q wChan 4 doPURW wTTbarGen 2 useDDEst wNTuple 0 RP.SLS.doReReco JsSm 0 doAbsRatio doSignal T2tt T2tt_FineBin 330 30 -1
    // ./StopPlotsSpectraShowNewDESY_wSyst -b -q wChan 9 doPURW wTTbarGen 2 useDDEst wNTuple 0 RP.SLS.doReReco doAbsRatio doSignal T2tt T2tt_FineBin 330 30 -1 ReleaseTheKraken multiChan testOutZMass0BJetsMETCut
    
    TRint theApp("App", &argc, argv);
    Bool_t retVal = kTRUE;
    
    RunParams RP;
    WeightCalculators WC;
    PlotMakingStructs PMS;
    AncillaryDrawingVariables ADV;    
    GlobalHistPlotInfo GHPI;
    HistDisplayParams HDP;

    SetupPlotRunning_Part1(argc, argv, &RP, &WC, &PMS, &ADV, &GHPI, &HDP);
    bool doVerb = RP.HPM.doVerbosity;
    
    HistogramDisplayStructs HDS_Data;
    HistogramDisplayStructs HDS_MC;
    SetupPlotRunning_DataMC(&RP, &WC, &PMS, &HDS_Data, &HDS_MC, doVerb);
    
    HistogramDisplayStructs HDS_Signal;
    if (RP.SLS.doSignal) {
        SetupPlotRunning_Signal(&RP, &WC, &PMS, &HDS_Signal, doVerb);
    }
    
    TString multiChannelCompOutputName;    
    RP.SLS.SetupMultiHist_MultiChanLoading(&PMS.multiHistListIDs, PMS.vecVecHistTtoUse[0], &PMS.multiChannelIDs, PMS.subSampVec, multiChannelCompOutputName);

    //Stuff related to drawing the histograms  
    
    
    
    TH1F * h_FracratioComp;
    unsigned int num1DPlots = PMS.vecVecHistTtoUse[0]->size();
    unsigned int num2DPlots = PMS.vecVecHistTtoUse[1]->size(); 
    unsigned int num3DPlots = PMS.vecVecHistTtoUse[2]->size();
    TString baseSaveNameLocation("../Plots/");
        
    float XaxisLegendPos = 0.7;
    float YaxisLegendStart = 0.83;
    float YAxisLB = 0.2;
//    float YAxisLB = 0.01; ////SEARCH FOR LB
    float YAxisUBBase = 2E5;
    float YAxisUB, histMax;
    bool  logYPad1;
    
    TString canvName, systCanvName;
    logYPad1 = true;
    
    
    vector<HistoStrings> vecHistStrContainer;
    HistoStrings currHistStrContainer;
    int indexToUse;
    unsigned int size = RP.SLS.multiHistList ? PMS.multiHistListIDs.size() : num1DPlots;
    if (doVerb) {
        cout << "size -- i.e. number of hists to make " << size << endl;
    }
    if (RP.HPM.doOneDee) {
        for (unsigned int iSpecHist = 0; iSpecHist < size; ++iSpecHist) {
            if (doVerb) {
                cout << "iSpecHist " << iSpecHist << endl;
            }
            indexToUse = RP.SLS.multiHistList ? PMS.multiHistListIDs[iSpecHist] : iSpecHist;
            if (doVerb) {
                cout << "indexToUse " << indexToUse << endl;
                cout << "size " << PMS.vecVecHistTtoUse[0]->size() << endl;
            }
            currHistStrContainer.SetBaseStuff(&PMS.vecVecHistTtoUse[0]->at(indexToUse), &RP.SLS);
            if (doVerb) {
                currHistStrContainer.PrintInfo();
            }
            if (RP.SLS.multiChannelAdd) {
                if (doVerb) cout << "multiChannelCompOutputName " << multiChannelCompOutputName << endl;
                currHistStrContainer.AddToNames(multiChannelCompOutputName, PMS.subSampVec, &PMS.multiChannelIDs);
            }
            else {
                if (doVerb) {
                    cout << "PMS.subSampVec size " << PMS.subSampVec->size() << endl;
                }
                if (RP.SLS.whichChanName.Length() != 0) {
                    currHistStrContainer.AddToNames(RP.SLS.whichChanName);
                }
                else {
                    currHistStrContainer.AddToNames(&PMS.subSampVec->at(RP.SLS.whichChan));
                }
                if (doVerb) {
                    cout << "post sub samp add " << endl;
                    currHistStrContainer.PrintInfo();
                }
            }
            currHistStrContainer.canvName += RP.GSI.saveName;
            currHistStrContainer.doSystCurrPlot = (RP.SLS.doSyst && PMS.vecVecHistTtoUse[0]->at(indexToUse).xAxis.doAxisSyst);
            vecHistStrContainer.push_back(currHistStrContainer);
        }
    }
                                                
    if (RP.HPM.doOneDee) {
        for (unsigned int iHist = 0; iHist < vecHistStrContainer.size(); ++iHist) {
            if (doVerb) cout << "Doing " << vecHistStrContainer[iHist].plotVarName << endl;
            PMS.NewCanvas(vecHistStrContainer[iHist].canvName);
            cout << "vecHistStrContainer[iHist].mcStackName " << vecHistStrContainer[iHist].mcStackName << endl;
            PMS.NewMCStack(vecHistStrContainer[iHist].mcStackName);
            //grab the histograms and do necessary projections
            if (RP.SLS.multiChannelAdd) {
                HDS_Data.GrabCentralValues(&vecHistStrContainer[iHist].vecMultiChannelAddStrings_Data, doVerb);
                HDS_MC.GrabCentralValues(&vecHistStrContainer[iHist].vecMultiChannelAddStrings_MC, doVerb);
            }
            else {
                HDS_Data.GrabCentralValues(vecHistStrContainer[iHist].nameToGrabData, doVerb);
                HDS_MC.GrabCentralValues(vecHistStrContainer[iHist].nameToGrabMC, doVerb);
            }
            
            if (RP.SLS.doSyst) {
                if (RP.SLS.multiChannelAdd) {
                    HDS_MC.GrabSystValues(&vecHistStrContainer[iHist].vecMultiChannelAddStrings_MC, &PMS.vecSystNames, doVerb);
                }
                else {
                    HDS_MC.GrabSystValues(vecHistStrContainer[iHist].nameToGrabMC, &PMS.vecSystNames, doVerb);
                }
            }
            
            
            HDS_Data.DoProjection(&PMS.vecIndMCParams, &RP.API, &HDP, "DataComp", false, doVerb);
            HDS_MC.DoProjection(&PMS.vecIndMCParams, &RP.API, &HDP, "MCComp", false, doVerb);
            HDS_MC.AddToStack(PMS.indMCStack);
            
            h_FracratioComp = GHPI.FracRatioHist_GHPI(HDS_Data.compSamp.first.grabbedHist_TH1F, HDS_MC.compSamp.first.grabbedHist_TH1F, RP.GHS.doAbsRatio, "ratioComp");
            ADV.SetPlots(PMS.indMCStack, h_FracratioComp, HDS_MC.compSamp.first.grabbedHist_TH1F);
            
            if (RP.SLS.doSyst) {
                HDS_MC.CalculateSystsComp(kGray + 1, RP.GHS.doAbsRatio, GHPI.fracRatioADP.axisRangePart1, GHPI.fracRatioADP.axisRangePart2, false, RP.SLS.SmearedPlots, RP.HPM.doStopXSec, doVerb);
            }
            
            
            if (RP.SLS.doSignal) {
                if (RP.SLS.multiChannelAdd) {
                    HDS_Signal.GrabCentralValues(&vecHistStrContainer[iHist].vecMultiChannelAddStrings_MC, doVerb);
                }
                else {
                    HDS_Signal.GrabCentralValues(vecHistStrContainer[iHist].nameToGrabMC, doVerb);
                }
                
                if (RP.SLS.doSyst) {
                    if (RP.SLS.multiChannelAdd) {
                        HDS_Signal.GrabSystValues(&vecHistStrContainer[iHist].vecMultiChannelAddStrings_MC, &PMS.vecSystNames, doVerb);
                    }
                    else {
                        HDS_Signal.GrabSystValues(vecHistStrContainer[iHist].nameToGrabMC, &PMS.vecSystNames, doVerb);
                    }
                }                
                HDS_Signal.DoProjection(&PMS.vecIndMCParams, &RP.API, &HDP, "SignalComp", false, doVerb);                
                if (RP.SLS.doSyst) {
                    if (doVerb) {
                        cout << "about to calc Signal systs " << endl;
                    }
                    HDS_Signal.CalculateSystsComp(kMagenta, RP.GHS.doAbsRatio, GHPI.fracRatioADP.axisRangePart1, GHPI.fracRatioADP.axisRangePart2, false, RP.SLS.SmearedPlots, RP.HPM.doStopXSec, doVerb);                    
                    // false here is for no Sym Error
                }
            }
            
            

            histMax = TMath::Max(HDS_Data.compSamp.first.grabbedHist_TH1F->GetMaximum(), HDS_MC.compSamp.first.grabbedHist_TH1F->GetMaximum());
            YAxisUB = (YAxisUBBase > 1) ? histMax * 2.5E2 : histMax * 2.5;
            ADV.yADPs[0].SetRanges(YAxisLB, YAxisUB);
            ADV.doLogYTopPad = logYPad1;
            ADV.BaseBounds.SetBounds(XaxisLegendPos, YaxisLegendStart, XaxisLegendPos, YaxisLegendStart);
            SpectrumDraw(PMS.plottingCanvas, &HDS_Data, &HDS_MC, &ADV, RP.GHS.doStats, &GHPI, RP.GHS.doSmartFracRatio, RP.GHS.showLegend, doVerb);
                        
            cout << "Integral Data " << HDS_Data.compSamp.first.grabbedHist_TH1F->Integral() << endl;
            cout << "Integral MC " << HDS_MC.compSamp.first.grabbedHist_TH1F->Integral() << endl;
//            SpectrumDraw(PMS.plottingCanvas, &HDS_Data, &HDS_MC, h_FracratioComp, h_ErrComp, PMS.indMCStack, XaxisLegendPos[iHist], YaxisLegendStart[iHist], YAxisLB, YAxisUB, logYPad1, RP.GHS.doStats, "", WC.intLumi, leg, RP.GHS.doSmartFracRatio, RP.GHS.showLegend);
            
            /*
             if (RP.SLS.doSignal) {
             SpectrumDraw_AddSignal(PMS.plottingCanvas, vecStop1DCentValHists, mcLegendsSignal, leg, RP.GHS.showLegend);
             }
             */                            
            
            
            
            if (RP.SLS.doSignal) {
                SpectrumDraw_AddSignal(PMS.plottingCanvas, &HDS_Signal, &ADV.topLeg, RP.GHS.showLegend);
            }
            
            
            
            PMS.plottingCanvas->SaveAs(baseSaveNameLocation + vecHistStrContainer[iHist].canvName + TString(".pdf"));
            if (RP.GSI.saveDotCFile) PMS.plottingCanvas->SaveAs(baseSaveNameLocation + vecHistStrContainer[iHist].canvName + TString(".C"));
            


            
            if (vecHistStrContainer[iHist].doSystCurrPlot) {
                for (unsigned int iSyst = 0; iSyst < HDS_MC.compSamp.first.indSSI.SystPlusStatErrorGraph.size(); ++iSyst) {
                    systCanvName = vecHistStrContainer[iHist].canvName + HDS_MC.compSamp.first.indSSI.SystName[iSyst];
                    PMS.NewCanvas(systCanvName);
                    
                    if (doVerb) {
                        cout << "printing out systematics for iSyst " << iSyst << endl;
                        HDS_MC.compSamp.first.indSSI.PrintVec(iSyst);
                    }
                    ADV.SetErrGraph(0, HDS_MC.compSamp.first.indSSI.SystPlusStatErrorGraph[iSyst]);
                    ADV.SetFracRatioGraph(HDS_MC.compSamp.first.indSSI.fracRatioSystVec[iSyst]);
                    SpectrumDrawSyst(PMS.plottingCanvas, &HDS_Data, &HDS_MC, &ADV, RP.GHS.doStats, &GHPI, RP.GHS.doSmartFracRatio, RP.GHS.showLegend, RP.GHS.doSmoothSyst, doVerb);
                    if (RP.SLS.doSignal) {
                        SpectrumDraw_AddSignal(PMS.plottingCanvas, &HDS_Signal, &ADV.topLeg, RP.GHS.showLegend, iSyst);
                    }
                    PMS.plottingCanvas->SaveAs(baseSaveNameLocation + systCanvName + TString(".pdf"));
                    if (RP.GSI.saveDotCFile) PMS.plottingCanvas->SaveAs(baseSaveNameLocation + systCanvName + TString(".C"));
                }
            }                        
            
            /*
             if (doSystCurrPlot) {
             
             SystGraphMakers(h_MCComp, mcCompHist1DSystVec, errCompSpecSource, errCompSpecSource_pStat, fracRatioSystVec, systCanvNameVec, kGray + 1, plotVarName, doAbsRatio, fracRatioYAxisRange, false, false, RP.SLS.SmearedPlots);
             for (unsigned int iSyst = 0; iSyst < systCanvNameVec->size(); ++iSyst) {
             systCanvName = canvName + systCanvNameVec->at(iSyst);
             PMS.plottingCanvas = new TCanvas(systCanvName, systCanvName, wtopx, wtopy, W_, H_);
             SpectrumDrawSyst(PMS.plottingCanvas, h_DataComp, dataLegendComp, h_MCComp, PMS.indMCStack, errCompSpecSource_pStat->at(iSyst), errCompSpecSource->at(iSyst), h_FracratioComp, fracRatioSystVec->at(iSyst), XaxisLegendPos[k], YaxisLegendStart[k], YAxisLB, YAxisUB, logYPad1, mcLegends, mcCompHist1DCentValVecSorted, RP.GHS.doStats, "", WC.intLumi, leg, RP.GHS.doSmartFracRatio, false, h_FracratioComp_Other, RP.GHS.showLegend, RP.GHS.doSmoothSyst);
             
             if (RP.SLS.doSignal) {
             SpectrumDrawSyst_AddSignal(PMS.plottingCanvas, vecStop1DCentValHists, vecErrSigCompVecSpecSource_pStat, iSyst, mcLegendsSignal, leg, RP.GHS.showLegend);
             }
             PMS.plottingCanvas->SaveAs(baseSaveNameLocation + systCanvName + TString(".pdf"));
             if (RP.GSI.saveDotCFile) PMS.plottingCanvas->SaveAs(baseSaveNameLocation + systCanvName + TString(".C"));
             }
             }
             
             
             if (RP.SLS.doSignal) {
             if (multiChannelAdd) {
             HDS_Signal.GrabCentralValues(&multiChannelAddStrings);
             HDS_Signal.GrabSystValues(&multiChannelAddStrings, &PMS.vecSystNames);
             }
             else {
             HDS_Signal.GrabCentralValues(dataplot);
             HDS_Signal.GrabSystValues(dataplot, &PMS.vecSystNames);
             }
             
             HDS_Signal.DoProjection(&PMS.vecIndMCParams, &RP.API, &HDP, false);
             
             vecStop1DCentValHists = new vector<TH1F *>;
             vecErrSigCompVecSpecSource = new vector<vector<TGraphAsymmErrors *> *>;
             vecErrSigCompVecSpecSource_pStat = new vector<vector<TGraphAsymmErrors *> *>;
             for (unsigned int iSigPoints = 0; iSigPoints < RP.SLS.vecStopMassGrab->size(); ++iSigPoints) {
             vecCurrSignalSystTH1Hists = new vector<TH1 *>;
             vecCurrStop1DSystHists = new vector<TH1F *>;
             vecCurrSigCompSpecSource = new vector<TGraphAsymmErrors *>;
             vecCurrSigCompSpecSource_pStat = new vector<TGraphAsymmErrors *>;
             HistogramVecGrabber_Signal(inputFilesSignal, signalSkimScaleVec, iSigPoints, currSignalCentValTH1Hist, mcplot, vecCurrSignalSystTH1Hists, systVec, subSampName, doOverflow[k], doUnderflow[k], doSystCurrPlot, false);
             HistogramAdderSignal(currSignalCentValTH1Hist, currSignal1DCentValHist, RBNX[k], 1, 1, -1, -1, -1, -1, "", doOverflow[k], doUnderflow[k], "");
             HistMainAttSet(currSignal1DCentValHist, kWhite, 0, mcColorsSignal->at(iSigPoints), 2, kWhite, 0, 0, mcStylesSignal->at(iSigPoints));
             vecStop1DCentValHists->push_back(currSignal1DCentValHist);                    
             if (doSystCurrPlot) {
             HistogramProjectorSyst(vecCurrSignalSystTH1Hists, vecCurrStop1DSystHists, RBNX[k], 1, 1, -1, -1, -1, -1, "", doOverflow[k], doUnderflow[k], "");
             
             SystGraphMakers(currSignal1DCentValHist, vecCurrStop1DSystHists, vecCurrSigCompSpecSource, vecCurrSigCompSpecSource_pStat, fracRatioSystVec, systCanvNameVec, mcColorsSignal->at(iSigPoints), plotVarName, doAbsRatio, fracRatioYAxisRange, false, true, RP.SLS.SmearedPlots);
             vecErrSigCompVecSpecSource->push_back(vecCurrSigCompSpecSource);
             vecErrSigCompVecSpecSource_pStat->push_back(vecCurrSigCompSpecSource_pStat);
             }
             
             }            
             }
             */
        }
    }    
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
