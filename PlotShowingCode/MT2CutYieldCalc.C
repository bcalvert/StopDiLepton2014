#include <iostream>
#include "TMath.h"
#include "TLorentzVector.h"
#include "TTree.h"

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
#include "TH1D.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TLegend.h"
//#include "TH3F.h"
//#include "TH3D.h"
//#include "TF1.h"
#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TRint.h"
//#include "TFitResult.h"
#include "TGraphAsymmErrors.h"
#include <vector>
#include <cmath>

using namespace std;
int main( int argc, char* argv[]) {
    
    
    gROOT->ProcessLine("#include <vector>");
    // Style things
    gStyle->SetErrorX(0.5); // set X uncertainty to 1/2 of a bin
    gStyle->SetTitleFont(42);        
    //    gStyle->SetLegendFont(42);
    
    //./MT2CutYieldCalc_Test doPURW wTTbarGen 2 wNTuple 0 doReReco doNonSig JsSm 0 ReleaseTheKraken useTTBarDDEst
    
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
    if (RP.SLS.doNonSig) {
        SetupPlotRunning_DataMC(&RP, &WC, &PMS, &HDS_Data, &HDS_MC, doVerb);
    }
    
    HistogramDisplayStructs HDS_Signal;
    if (RP.SLS.doSignal) {
        SetupPlotRunning_Signal(&RP, &WC, &PMS, &HDS_Signal, doVerb);
    }
    
    TString channelName = "_FullCut";
    PMS.SetYieldHistNames(&RP.SLS, &RP.HPM, doVerb);
    //Stuff related to drawing the histograms
    
    ValError statErrData;
    SampleSystematicsInfo fullErrMCComp;
    SampleSystematicsInfo fullErrIndIndMC;
    vector<SampleSystematicsInfo> fullErrIndMC;
    vector<SampleSystematicsInfo> fullErrIndMCSort;
    SampleSystematicsInfo fullErrSigPoints;
    vector<ValError> vecStatErrData;
    vector<SampleSystematicsInfo> vecFullErrMCComp;
    vector< vector<SampleSystematicsInfo> > vecFullErrIndMC;
    vector<SampleSystematicsInfo> vecFullErrSigPoints;
    
    TString saveNameAddition("../Plots/");
    
    
    vector<HistoStrings> vecHistStrContainer;
    HistoStrings currHistStrContainer;
    for (unsigned int iHist = 0; iHist < PMS.vecHistNamesForYieldCalc.size(); ++iHist) {
        currHistStrContainer.SetBaseStuff(PMS.vecHistNamesForYieldCalc[iHist], &RP.SLS);
        currHistStrContainer.AddToNames(channelName);
        vecHistStrContainer.push_back(currHistStrContainer);
    }
    
    if (RP.HPM.doYieldV1) {
        for (unsigned int iHist = 0; iHist < vecHistStrContainer.size(); ++iHist) {
            if (doVerb) cout << "Doing " << vecHistStrContainer[iHist].plotVarName << endl;
            if (RP.SLS.doNonSig) {
                HDS_Data.GrabCentralValues(vecHistStrContainer[iHist].nameToGrabData, doVerb);
                HDS_MC.GrabCentralValues(vecHistStrContainer[iHist].nameToGrabMC, doVerb);
                
                if (RP.SLS.doSyst) {
                    HDS_MC.GrabSystValues(vecHistStrContainer[iHist].nameToGrabMC, &PMS.vecSystNames, doVerb);
                }
                if (!RP.HPM.tryCalcPassByHand) {
                    HDS_Data.DoProjection(&PMS.vecIndMCParams, &RP.API, &HDP, "DataComp", false, doVerb);
                    HDS_MC.DoProjection(&PMS.vecIndMCParams, &RP.API, &HDP, "MCComp", false, doVerb);
                }
                else {
                    PMS.DoPassCut(&HDS_Data, "DataComp", &PMS.vecIndMCParams, 0, false, doVerb);
                    PMS.DoPassCut(&HDS_MC, "MCComp", &PMS.vecIndMCParams, 0, RP.SLS.doSyst, doVerb);
                    // note the 0 here means first set of input values, the false means no Syst since Data
                }
                
                cout << "test 1 " << HDS_MC.compSamp.first.grabbedHist->GetBinContent(2) << endl;
                cout << "test " << HDS_MC.compSamp.first.grabbedHist_TH1F->GetBinContent(2) << endl;
                
                if (RP.SLS.doSyst) {
                    HDS_MC.CalculateSystsComp(kGray + 1, RP.GHS.doAbsRatio, GHPI.fracRatioADP.axisRangePart1, GHPI.fracRatioADP.axisRangePart2, false, RP.SLS.SmearedPlots, RP.HPM.doStopXSec, doVerb);
                    HDS_MC.CalculateSysts_IndMC(kGray + 1, RP.GHS.doAbsRatio, GHPI.fracRatioADP.axisRangePart1, GHPI.fracRatioADP.axisRangePart2, false, RP.SLS.SmearedPlots, RP.HPM.doStopXSec, doVerb);
                    // false here is for no Sym Error
                }
                
                cout << "about to set individual ssi infor " << endl;
                //SetSSI_YieldErrors(bool doIndMC = false, int whichBin = 2, bool justStat = false, bool noSystPlusStat = true, bool doSymErr = false)
                HDS_Data.SetSSI_YieldErrors(false, 2, true, true, false);
                HDS_MC.SetSSI_YieldErrors(true, 2, !RP.SLS.doSyst, true, false);
                cout << "about to print individual SSI info " << endl;
                //PrintSSI_YieldInfo(bool doIndMC = false, bool justStat = true, bool noSystPlusStat = true)
                HDS_Data.PrintSSI_YieldInfo(false, true, true);
                HDS_MC.PrintSSI_YieldInfo(true, !RP.SLS.doSyst, true);
            }
            
            if (RP.SLS.doSignal) {
                HDS_Signal.GrabCentralValues(vecHistStrContainer[iHist].nameToGrabMC, doVerb);
                
                if (RP.SLS.doSyst) {
                    HDS_Signal.GrabSystValues(vecHistStrContainer[iHist].nameToGrabMC, &PMS.vecSystNames, doVerb);
                }
                if (!RP.HPM.tryCalcPassByHand) {
                    HDS_Signal.DoProjection(&PMS.vecIndMCParams, &RP.API, &HDP, "Signal", false, doVerb);
                }
                else {
                    PMS.DoPassCut(&HDS_Signal, "Signal", &PMS.vecIndMCParams, 0, RP.SLS.doSyst);
                }
                if (RP.SLS.doSyst) {
                    HDS_Signal.CalculateSystsComp(kGray + 1, RP.GHS.doAbsRatio, GHPI.fracRatioADP.axisRangePart1, GHPI.fracRatioADP.axisRangePart2, false, RP.SLS.SmearedPlots, RP.HPM.doStopXSec, doVerb);                    
                    // false here is for no Sym Error
                }
                
                HDS_Signal.SetSSI_YieldErrors(false, 2, !RP.SLS.doSyst, true, false);
                cout << "about to print Signal info " << endl;
                HDS_Signal.PrintSSI_YieldInfo(false, !RP.SLS.doSyst, true, RP.HPM.printSysLim, RP.HPM.printAveSyst);
            }
            
        }
    }               
    /*
    subSampName = subSampVec->at(grabChan[RP.SLS.whichChan]).histNameSuffix;
    TCanvas * c_Var;

    
    TString canvNameBase, canvName;
    const Int_t numCanvs = 14;
    TString canvNameAdd[numCanvs] = {"_CentVal", "_LepESShiftUp", "_LepESShiftDown", "_JetESShiftUp", "_JetESShiftDown", "_UncESShiftUp", "_UncESShiftDown", "_LepEffSFShiftUp", "_LepEffSFShiftDown", "_genTopRW", "_genStopXSecShiftUp", "_genStopXSecShiftDown", "_BTagSFShiftUp", "_BTagSFShiftDown"};    
    vector<TH2F *> vecMCComp(numCanvs);
    vector<TH2F *> vecSig(numCanvs);
    vector<TH2F *> vecFOMHists(numCanvs);
    
    vector<TH1F *> vecMCComp1D(numCanvs);
    vector<TH1F *> vecSig1D(numCanvs);
    vector<TH1F *> vecFOMHists1D(numCanvs);
    TString SigStringTitle;
    if (RP.HPM.doFOM) {
        SigStringTitle = HPI.mcLegendsSignal->at(0);
        if (RP.HPM.doOneDeeFOM) {
            plotGrabBaseName = RP.SLS.SmearedPlots ? "h_SmearMT2ll" : "h_MT2ll";
            if (noType0) {
                plotGrabBaseName += "_noType0";
            }
            plotMCGrabName = plotGrabBaseName;
            plotSystGrabName = plotMCGrabName;
            plotDataGrabName = plotMCGrabName;
            plotDataGrabName += subSampName;
            if (RP.SLS.SmearedPlots) {
                plotDataGrabName.Replace(2,5,"");
            }
            canvNameBase = "c_FOM_OneDee";
            mcHistTH1Vec = new vector<TH1 *>;
            mcHistSystTH1Vec = new vector<TH1 *>;
            vecCurrSignalSystTH1Hists = new vector<TH1 *>;        
            HistogramVecGrabberCentValGrab(inputFiles, false, mcHistTH1Vec, nVtxBackScaleVec, RecoilBackScaleVecCV, plotMCGrabName, subSampName, RP.HPM.useDDEstimate, TTBarSF, scaleLumi);
            vecMCComp1D[0] = HistogramAdderMCOneDee(mcHistTH1Vec);
            HistogramVecGrabber_Signal(inputFilesSignal, signalSkimScaleVec, 0, currSignalCentValTH1Hist, plotSystGrabName, vecCurrSignalSystTH1Hists, systVec, subSampName, false, false, RP.SLS.doSyst, false);
            vecSig1D[0] = (TH1F*) currSignalCentValTH1Hist;
            //        cout << "vecMCComp[0] ";
            //        cout << vecMCComp[0]->GetName() << " integral " << vecMCComp[0]->Integral() << endl;
            //        cout << "vecSig[0] ";
            //        cout << vecSig[0]->GetName() << " integral " << vecSig[0]->Integral() << endl;
            vecFOMHists1D[0] = FOMHist(vecMCComp1D[0], vecSig1D[0], SigStringTitle, RP.HPM.typeFOM, 0);
            if (RP.GSI.makePlots) {
                canvName = canvNameBase;
                canvName += canvNameAdd[0];
                canvName += RP.GSI.saveName;
                c_Var = new TCanvas(canvName, canvName, 0, 0, 700, 700);
                //            vecFOMHists1D[0]->Draw("e1");
                vecFOMHists1D[0]->Draw("hist");
                c_Var->SaveAs(saveNameAddition + canvName + TString(".pdf"));
                if (RP.GSI.saveDotCFile) c_Var->SaveAs(saveNameAddition + canvName + TString(".C"));
            }
            if (RP.HPM.printFOMInfo) {
                
                int XBinMaxVal, YBinMaxVal, ZBinMaxVal;
                TAxis * xAxis = vecFOMHists1D[0]->GetXaxis();
                int MinXBin = xAxis->FindBin(RP.HPM.MT2llaxisCut);
                int MaxXBin = xAxis->GetNbins();
                xAxis->SetRange(MinXBin, MaxXBin);
                vecFOMHists1D[0]->GetMaximumBin(XBinMaxVal, YBinMaxVal, ZBinMaxVal);
                cout << "Printing FOM Info " << endl;
                if (RP.SLS.SmearedPlots) {
                    cout << "In the range of SMEARED MT2ll = " << xAxis->GetBinLowEdge(MinXBin) << " to " << xAxis->GetBinUpEdge(MaxXBin) << endl;
                    cout << "Max F.O.M., using F.O.M. of " << RP.HPM.FOMString << " is " << vecFOMHists1D[0]->GetBinContent(XBinMaxVal, YBinMaxVal) << " and is found cutting at MT2ll = " << xAxis->GetBinLowEdge(XBinMaxVal) << endl;
                }
                else {
                    cout << "In the range of MT2ll = " << xAxis->GetBinLowEdge(MinXBin) << " to " << xAxis->GetBinUpEdge(MaxXBin) << endl;
                    cout << "Max F.O.M., using F.O.M. of " << RP.HPM.FOMString << " is " << vecFOMHists1D[0]->GetBinContent(XBinMaxVal, YBinMaxVal) << " and is found cutting at MT2ll = " << xAxis->GetBinLowEdge(XBinMaxVal) << endl;
                }
            }
            else {
                if (RP.SLS.doSyst) {
                    HistogramVecGrabberSystGrab(inputFiles, mcHistTH1Vec, mcHistSystTH1Vec, nVtxBackScaleVec, RecoilBackScaleVecCV, RecoilBackScaleVecDoubleCorr, plotSystGrabName, subSampName, systVec, RP.HPM.useDDEstimate, TTBarSF, scaleLumi, false, RP.SLS.whichNTuple);
                    for (int iSyst = 0; iSyst < numCanvs - 1; ++iSyst) {
                        if (canvNameAdd[iSyst + 1].Contains("genStop")) {
                            vecMCComp1D[iSyst + 1] = HistogramAdderMCOneDee(mcHistTH1Vec);
                        }
                        else {
                            vecMCComp1D[iSyst + 1] = SystHistFinderOneDee(mcHistSystTH1Vec, canvNameAdd[iSyst + 1]);
                        }
                        cout << "vecMCComp[iSyst + 1] " << vecMCComp1D[iSyst + 1]->GetName() << " Integral " << vecMCComp1D[iSyst + 1]->Integral() << endl;
                        cout << "vecMCComp[iSyst + 1] " << vecMCComp1D[iSyst + 1]->GetName() << " Mean " << vecMCComp1D[iSyst + 1]->GetMean(1) << endl;
                        vecSig1D[iSyst + 1] = SystHistFinderOneDee(vecCurrSignalSystTH1Hists, canvNameAdd[iSyst + 1]);
                        cout << "vecSig[iSyst + 1] " << vecSig1D[iSyst + 1]->GetName() << " Integral " << vecSig1D[iSyst + 1]->Integral() << endl;
                        cout << "vecSig[iSyst + 1] " << vecSig1D[iSyst + 1]->GetName() << " Mean " << vecSig1D[iSyst + 1]->GetMean(1) << endl;
                        vecFOMHists1D[iSyst + 1] = FOMHist(vecMCComp1D[iSyst + 1], vecSig1D[iSyst + 1], SigStringTitle, RP.HPM.typeFOM, iSyst + 1);
                        if (RP.GSI.makePlots) {
                            canvName = canvNameBase;
                            canvName += canvNameAdd[iSyst + 1];
                            canvName += RP.GSI.saveName;
                            c_Var = new TCanvas(canvName, canvName, 0, 0, 700, 700);
                            //                    vecFOMHists1D[iSyst + 1]->Draw("e1"); 
                            vecFOMHists1D[iSyst + 1]->Draw("hist");
                            c_Var->SaveAs(saveNameAddition + canvName + TString(".pdf"));
                            if (RP.GSI.saveDotCFile) c_Var->SaveAs(saveNameAddition + canvName + TString(".C"));
                        }
                    }
                } 
            }
        }
        if (RP.HPM.doTwoDeeFOM) {
            plotGrabBaseName = RP.SLS.SmearedPlots ? "h_SmearMT2ll_vs_MT2lb" : "h_MT2ll_vs_MT2lb";
            if (noType0) {
                plotGrabBaseName += "_noType0";
            }
            canvNameBase = "c_FOM_TwoDee";
            plotMCGrabName = plotGrabBaseName;
            plotSystGrabName = plotMCGrabName;
            plotDataGrabName = plotMCGrabName;
            plotDataGrabName += subSampName;
            mcHistTH1Vec = new vector<TH1 *>;
            mcHistSystTH1Vec = new vector<TH1 *>;
            vecCurrSignalSystTH1Hists = new vector<TH1 *>;        
            HistogramVecGrabberCentValGrab(inputFiles, false, mcHistTH1Vec, nVtxBackScaleVec, RecoilBackScaleVecCV, plotMCGrabName, subSampName, RP.HPM.useDDEstimate, TTBarSF, scaleLumi);
            vecMCComp[0] = HistogramAdderMCTwoDee(mcHistTH1Vec);
            HistogramVecGrabber_Signal(inputFilesSignal, signalSkimScaleVec, 0, currSignalCentValTH1Hist, plotSystGrabName, vecCurrSignalSystTH1Hists, systVec, subSampName, false, false, RP.SLS.doSyst, false);
            vecSig[0] = (TH2F*) currSignalCentValTH1Hist;
            //        cout << "vecMCComp[0] ";
            //        cout << vecMCComp[0]->GetName() << " integral " << vecMCComp[0]->Integral() << endl;
            //        cout << "vecSig[0] ";
            //        cout << vecSig[0]->GetName() << " integral " << vecSig[0]->Integral() << endl;
            vecFOMHists[0] = FOMHist(vecMCComp[0], vecSig[0], SigStringTitle, RP.HPM.typeFOM, 0);
            if (RP.GSI.makePlots) {
                canvName = canvNameBase;
                canvName += canvNameAdd[0];
                canvName += RP.GSI.saveName;
                c_Var = new TCanvas(canvName, canvName, 0, 0, 700, 700);
                vecFOMHists[0]->Draw("colz");
                c_Var->SaveAs(saveNameAddition + canvName + TString(".pdf"));
                if (RP.GSI.saveDotCFile) c_Var->SaveAs(saveNameAddition + canvName + TString(".C"));
            }
            if (RP.HPM.printFOMInfo) {
                int XBinMaxVal, YBinMaxVal, ZBinMaxVal;
//                cout << " test " << endl;
                TAxis * xAxis = vecFOMHists[0]->GetXaxis();
//                cout << " test2 " << xAxis->GetName() << endl;
                int MinXBin = xAxis->FindBin(RP.HPM.MT2llaxisCut);
                int MaxXBin = xAxis->GetNbins();
                TAxis * yAxis = vecFOMHists[0]->GetYaxis();
                int MinYBin = yAxis->FindBin(RP.HPM.MT2lbaxisCut);
//                int MaxYBin = yAxis->GetNbins();
                int MaxYBin = RP.SLS.typeSMS.Contains("T2tt") ? yAxis->FindBin(200) : yAxis->FindBin(250);
                xAxis->SetRange(MinXBin, MaxXBin);
                yAxis->SetRange(MinYBin, MaxYBin);
                vecFOMHists[0]->GetMaximumBin(XBinMaxVal, YBinMaxVal, ZBinMaxVal);
                cout << "Printing FOM Info " << endl;
                cout << "In the range of MT2ll = " << xAxis->GetBinLowEdge(MinXBin) << " to " << xAxis->GetBinUpEdge(MaxXBin) << endl;
                cout << "In the range of MT2lb = " << yAxis->GetBinLowEdge(MinYBin) << " to " << yAxis->GetBinUpEdge(MaxYBin) << endl;

                cout << "Max F.O.M., using F.O.M. of " << RP.HPM.FOMString << " is " << vecFOMHists[0]->GetBinContent(XBinMaxVal, YBinMaxVal) << " and is found cutting at MT2ll = " << xAxis->GetBinLowEdge(XBinMaxVal) << " and MT2lblb = " << yAxis->GetBinLowEdge(YBinMaxVal) << endl;
            }
            else {
                if (RP.SLS.doSyst) {
                    HistogramVecGrabberSystGrab(inputFiles, mcHistTH1Vec, mcHistSystTH1Vec, nVtxBackScaleVec, RecoilBackScaleVecCV, RecoilBackScaleVecDoubleCorr, plotSystGrabName, subSampName, systVec, RP.HPM.useDDEstimate, TTBarSF, scaleLumi, false, RP.SLS.whichNTuple);
                    for (int iSyst = 0; iSyst < numCanvs -1; ++iSyst) {
                        if (canvNameAdd[iSyst + 1].Contains("genStop")) {
                            vecMCComp[iSyst + 1] = HistogramAdderMCTwoDee(mcHistTH1Vec);
                        }
                        else {
                            vecMCComp[iSyst + 1] = SystHistFinderTwoDee(mcHistSystTH1Vec, canvNameAdd[iSyst + 1]);
                        }
                        cout << "vecMCComp[iSyst + 1] " << vecMCComp[iSyst + 1]->GetName() << " Integral " << vecMCComp[iSyst + 1]->Integral() << endl;
                        cout << "vecMCComp[iSyst + 1] " << vecMCComp[iSyst + 1]->GetName() << " Mean " << vecMCComp[iSyst + 1]->GetMean(1) << endl;
                        vecSig[iSyst + 1] = SystHistFinderTwoDee(vecCurrSignalSystTH1Hists, canvNameAdd[iSyst + 1]);
                        cout << "vecSig[iSyst + 1] " << vecSig[iSyst + 1]->GetName() << " Integral " << vecSig[iSyst + 1]->Integral() << endl;
                        cout << "vecSig[iSyst + 1] " << vecSig[iSyst + 1]->GetName() << " Mean " << vecSig[iSyst + 1]->GetMean(1) << endl;
                        vecFOMHists[iSyst + 1] = FOMHist(vecMCComp[iSyst + 1], vecSig[iSyst + 1], SigStringTitle, RP.HPM.typeFOM, iSyst + 1);
                        if (RP.GSI.makePlots) {
                            canvName = canvNameBase;
                            canvName += canvNameAdd[iSyst + 1];
                            canvName += RP.GSI.saveName;
                            c_Var = new TCanvas(canvName, canvName, 0, 0, 700, 700);
                            vecFOMHists[iSyst + 1]->Draw("colz");
                            c_Var->SaveAs(saveNameAddition + canvName + TString(".pdf"));
                            if (RP.GSI.saveDotCFile) c_Var->SaveAs(saveNameAddition + canvName + TString(".C"));
                        }
                    }
                }
            }
        }
    }
    */
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
