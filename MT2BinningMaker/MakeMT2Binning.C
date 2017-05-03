#include <iostream>
#include "TTree.h"
#include "TMath.h"
#include "TEventList.h"
#include "TF1.h"
#include "TH1F.h"

#include "TGraph.h"
#include "TLegend.h"

#include "TFile.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TRint.h"
#include "TPaletteAxis.h"
#include <vector>
#include <cmath>
using namespace std;

#include "../HeaderFiles/BasicFunctions.h"
#include "../HeaderFiles/BasicFunctions_Math.h"
#include "../HeaderFiles/BasicFunctions_HistogramMaking.h"
#include "../HeaderFiles/BasicFunctions_SystemCommands.h"
#include "../HeaderFiles/HistogramSystematics2.h"
#include "../HeaderFiles/GeneralPlotDrawingStructs.h"
#include "../HeaderFiles/LimitScriptHeaderFiles/LimitScripts_ParametersStruct.h"
#include "../HeaderFiles/interpolate.h"

//SUSY functions/structures//
#include "../HeaderFiles/SUSYFunctions/SUSYFunctions_CompositeHeaderFiles.h"

#include "../HeaderFiles/LimitStructs/ScenarioDrawingParams.h"
#include "../HeaderFiles/LimitStructs/ScenarioDrawingParams.cc"
#include "../HeaderFiles/LimitScriptHeaderFiles/LimitScripts_SUSYStruct.h"
#include "../HeaderFiles/LimitStructs/SUSYT2LimitParams.h"
#include "../HeaderFiles/LimitStructs/SUSYT2LimitParams.cc"
#include "../HeaderFiles/GraphStyleFunctions.h"

#include "../HeaderFiles/LimitScriptHeaderFiles/LimitFunctions_Saving.h"
#include "../HeaderFiles/LimitScriptHeaderFiles/YieldCalculationFunctions.h"
#include "../HeaderFiles/LimitScriptHeaderFiles/CardCreationFunctions.h"
#include "../HeaderFiles/LimitScriptHeaderFiles/LimitRunningFunctions.h"
#include "../HeaderFiles/LimitScriptHeaderFiles/LimitCombinationFunctions.h"
#include "../HeaderFiles/StopLimitMakingFunctions.h"
#include "../HeaderFiles/SmoothingFunctions_Structs/TwoDeeGaussianSmoothingHeaderFiles.h"
#include "../HeaderFiles/StopLimitMakingStructs.h"
#include "../HeaderFiles/SmoothingFunctions_Structs/LimitScenarioSmoother_Optimizer.h"
#include "../HeaderFiles/SmoothingFunctions_Structs/LimitScenarioSmoother_Optimizer.cc"

#include "../HeaderFiles/ROCCurveFunctions/ROCCurveSavingFunctions_General.h"
#include "../HeaderFiles/ROCCurveFunctions/ROCCurveSavingFunctions_Specific.h"
#include "../HeaderFiles/ROCCurveFunctions/StopFunctions_ROCCurves.h"
#include "../HeaderFiles/ROCCurveFunctions/StopROCCurveFunctions.h"
#include "../HeaderFiles/PlotStrings.h"
#include "../HeaderFiles/ROCCurveFunctions/StopStructs_ROCCurves.h"
#include "../HeaderFiles/ROCCurveFunctions/ROCCurveMakingFunctions.h"
#include "../HeaderFiles/TreeLevelPlotting/MappingFunctions.h"
#include "../HeaderFiles/TreeLevelPlotting/TreeLevelDrawingManager.h"
#include "../HeaderFiles/TreeLevelPlotting/BinningFunctions.h"
#include "../HeaderFiles/PlotStrings_DrawCommands.h"

#include "../HeaderFiles/MathFunctions_Structs/BinCombiner.h"


typedef pair<int, int> vPP;

void FullSequence(vector<TGraph *> * inVecEffCurves, vector<ROCDrawingParams> * inVecROCDPs, TH1F * inputHistBackground, bool isPunzi, vPP * deltaM = NULL, bool doVerb = false) {
    if (doVerb) {
        cout << "size inVecEffCurves " << inVecEffCurves->size() << endl;
        cout << "size inVecROCDPs " << inVecROCDPs->size() << endl;
    }
    vector<TGraph *> tempVecEffCurves;
    vector<TString> tempLegStrings;
    for (unsigned int iCanv = 0; iCanv < inVecROCDPs->size(); ++iCanv) {
        tempVecEffCurves.resize(0);
        tempLegStrings.resize(0);
        if (doVerb) {
            cout << "Number of curves to draw for inVecROCDPs->at(iCanv) " << inVecROCDPs->at(iCanv).vecIndices.size() << endl;
        }
        for (unsigned int iEff = 0; iEff < inVecROCDPs->at(iCanv).vecIndices.size(); ++iEff) {
            if (doVerb) {
                cout << "grabbing eff curve from " << inVecROCDPs->at(iCanv).vecIndices[iEff] << endl;
            }
            tempVecEffCurves.push_back(inVecEffCurves->at(inVecROCDPs->at(iCanv).vecIndices[iEff]));
            GraphMainAttSet(inVecEffCurves->at(inVecROCDPs->at(iCanv).vecIndices[iEff]), kWhite, 1001, inVecROCDPs->at(iCanv).vecROCCurvesColor[iEff], 1, 2, inVecROCDPs->at(iCanv).vecROCCurvesColor[iEff], inVecROCDPs->at(iCanv).vecROCCurvesStyle[iEff], 1);
            tempLegStrings.push_back(inVecROCDPs->at(iCanv).vecLegStrings[inVecROCDPs->at(iCanv).vecIndices[iEff]]);
        }
        inVecROCDPs->at(iCanv).DrawOnCanvas(&tempVecEffCurves, inputHistBackground, isPunzi, deltaM);
        FinalDrawGraph(inVecROCDPs->at(iCanv).vecDrawCanvas[isPunzi], &tempVecEffCurves, &tempLegStrings, isPunzi);
    }
}


int main( int argc, char* argv[]) {
    
    gROOT->ProcessLine("#include <vector>");
    // Style things
    gStyle->SetErrorX(0.5); // set X uncertainty to 1/2 of a bin
    gStyle->SetTitleFont(42);
    
    TRint theApp("App", &argc, argv);
    Bool_t retVal = kTRUE;
    
    
    TString baseDirNonTTbar = "../PlotMakingCode/SkimmingMacro/SpecialSkims_2/";
    TString strNameTree = "OviSkimTree";
    
    
    //TLDMs for the backgrounds
    int whichROCType = 0;
    int whichMT2Var = 2;
    
    int whichSpecialBinning = 1;
    
    int whichSel = 2;
    vPP mStop(-1, 0);
    vPP deltaM(-1, 0);
    
    int iDM = 0;
    int iSig = 0;
    bool makeBkgs = true;
    int doVerb = 0;
    bool doDraw = false;
    bool doLog = false;
    int wPunzi = 0;
    int wChanToDraw = 0;
    
    bool tryHack = false;
    
    SUSYT2LimitParams SUSYT2LP;
    SUSYT2LP.DefaultVals();
    SUSYT2LP.SetParamsFromCommandLine(argc, argv);
    SUSYT2LP.SetStrings();
    
    LimitParametersContainer BasicLP;
    BasicLP.DefaultVals();
    BasicLP.SetParamsFromCommandLine(argc, argv);
    BasicLP.SetStrings();
    
    BinCombiner comboBinMaker;
    comboBinMaker.DefaultVarVals();
    comboBinMaker.minThreshBkg = 0.25;
    
    BinCombiner comboBinMaker_SplitDilep;
    comboBinMaker_SplitDilep.DefaultVarVals();
    comboBinMaker_SplitDilep.minThreshBkg = 0.25;
    
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"iSig", 4) == 0) {
            iSig = strtol(argv[k+1], NULL, 10);
        }
        else if (strncmp (argv[k],"-wSel", 5) == 0) {
            whichSel = strtol(argv[k+1], NULL, 10);
        }
        else if (strncmp (argv[k],"-wPunzi", 7) == 0) {
            wPunzi = strtol(argv[k+1], NULL, 10);
        }
        else if (strncmp (argv[k],"tryHack", 7) == 0) {
            tryHack = true;
        }
        else if (strncmp (argv[k],"doVerb", 6) == 0) {
            doVerb = strtol(argv[k+1], NULL, 10);
        }
        else if (strncmp (argv[k],"whichBin", 8) == 0) {
            whichSpecialBinning = strtol(argv[k+1], NULL, 10);
        }
        else if (strncmp (argv[k],"readBkgs", 8) == 0) {
            makeBkgs = false;
        }
        else if (strncmp (argv[k],"mStop", 5) == 0) {
            mStop.first = strtol(argv[k + 1], NULL, 10);
            mStop.second = strtol(argv[k + 2], NULL, 10);
        }
        else if (strncmp (argv[k],"deltaM", 6) == 0) {
            deltaM.first = strtol(argv[k + 1], NULL, 10);
            deltaM.second = strtol(argv[k + 1], NULL, 10);
        }
        else if (strncmp (argv[k],"dM", 2) == 0) {
            iDM = strtol(argv[k + 1], NULL, 10);
            deltaM.first = DeltaMValue(iDM);
            deltaM.second = DeltaMPrecision(strtol(argv[k + 2], NULL, 10));
        }
        else if (strncmp (argv[k],"doDraw", 6) == 0) {
            doDraw = true;
            wChanToDraw = strtol(argv[k + 1], NULL, 10);
        }
    }
    
    
    
    vPP * mStopCanv = mStop.first >= 0 ? &mStop : NULL;
    vPP * deltaMCanv = deltaM.first >= 0 ? &deltaM : NULL;
    
    if (doDraw) makeBkgs = false;
    
    
    
    
    const int numBkg = 4;
    TString arrNameBkg[numBkg] = {"TTbar", "DY", "Other", "Fake"};
    
    vector<TreeLevelDrawingManager> vecTLDM_Bkg(numBkg);
    for (int iBkg = 0; iBkg < numBkg; ++iBkg) {
        vecTLDM_Bkg[iBkg].SetUpMaps();
        vecTLDM_Bkg[iBkg].SetName(arrNameBkg[iBkg]);
        vecTLDM_Bkg[iBkg].SetThings();
        vecTLDM_Bkg[iBkg].SetROCCurve(whichSel, whichROCType, whichMT2Var);
    }            
    TreeLevelDrawingManager TLDM_Bkg_Comp;
    
    TLDM_Bkg_Comp.SetName("CompBkg");
    TLDM_Bkg_Comp.ROCCP = vecTLDM_Bkg[0].ROCCP;
    TLDM_Bkg_Comp.SetChannelNames();
    TLDM_Bkg_Comp.SetUpPunzi();
    TLDM_Bkg_Comp.SetNameGraphs(doVerb);
    
    
    
    const int numSig = 6;
    TString arrNameSig[numSig] = {"TTbar", "T2tt", "T2bwx0p25", "T2bwx0p50", "T2bwx0p75", "T2tb"};
    
    TString sigString = arrNameSig[iSig] + TString("_dM");
    
    TString appendString_OutName = sigString;
    appendString_OutName += iDM;
    appendString_OutName += "_vs";
    
    TString appendString_InName = TLDM_Bkg_Comp.nameTLDM;
    appendString_OutName += appendString_InName;
    TString outName = TLDM_Bkg_Comp.ROCCP.GetOutFileNameBase(true, appendString_OutName);
    TString inName = TLDM_Bkg_Comp.ROCCP.GetOutFileNameBase(false, appendString_InName);
    cout << "inName " << inName << endl;
    cout << "outName " << outName << endl;
    
    TreeLevelDrawingManager TLDM_Sig;
    TLDM_Sig.SetUpMaps(SUSYT2LP.indexPol, SUSYT2LP.indexPol);
    TLDM_Sig.SetName(arrNameSig[iSig]);
    TString cutCommand_SUSYMass = "";
    if (mStop.first > 0 || deltaM.first >= 0) {
        if (mStop.first >= 0) {
            TString addMStopCut = MStopCutString(&mStop);
            cutCommand_SUSYMass += addMStopCut;
        }
        
        if (deltaM.first >= 0) {
            TString addDeltaMCut = DeltaMCutString(&deltaM);
            cutCommand_SUSYMass += addDeltaMCut;
        }
    }
    TLDM_Sig.SetThings();
    cout << "cutCommand_SUSYMass " << cutCommand_SUSYMass << endl;
    TLDM_Sig.strAddCut = cutCommand_SUSYMass;
    TLDM_Sig.SetROCCurve(whichSel, whichROCType, whichMT2Var);
    if (!makeBkgs) {
        TLDM_Sig.OpenFile(baseDirNonTTbar, strNameTree);
    }
    else {
        for (int iBkg = 0; iBkg < numBkg; ++iBkg) {
            vecTLDM_Bkg[iBkg].OpenFile(baseDirNonTTbar, strNameTree);
        }
    }
    
    
    
    unsigned int numChannels = vecTLDM_Bkg[0].numChannels;
    //Better binning
    vector< vector<Double_t> > vecVecBinEdges;
    vector<int> vecNumBins;
    //Defining the better binning
    AddInMT2llBounds3DShapeTwo(&vecVecBinEdges, &vecNumBins);
    AddInMT2lblbBounds3DShapeThree(&vecVecBinEdges, &vecNumBins);
    AddInMT2bb_ZMETBounds3DShape(&vecVecBinEdges, &vecNumBins);
    //Histograms for the better binning
    
    vector<TH3F *> vecTH3sSignal_BetterBin(numChannels);
    vector<TString> vecNameTH3sSignal_BetterBin(numChannels);
    vector< vector<TH3F *> > vecVecTH3sBackgrounds_BetterBin(numChannels);
    vector< vector<TString> > vecVecNameTH3sBackgrounds_BetterBin(numChannels);
    for (unsigned int iChan = 0; iChan < numChannels; ++iChan) {
        vecVecTH3sBackgrounds_BetterBin[iChan].resize(numBkg);
        vecVecNameTH3sBackgrounds_BetterBin[iChan].resize(numBkg);
    }
    
    
    TString nameOutFile = "Maps/h_OptBinMap_";
    
    nameOutFile += BaseStringLimit_NameOrDir(&BasicLP, &SUSYT2LP, 2);
    nameOutFile.Replace(nameOutFile.Index("Exp_"), 4, "");
    nameOutFile += "_dM";
    nameOutFile += iDM;
    nameOutFile += ".root";
    
    
    cout << "nameOutFile " << nameOutFile << endl;
    
    if (!makeBkgs && !tryHack) {
        if (doVerb) {
            cout << "loading backgrounds " << endl;
        }
        TLDM_Bkg_Comp.ReadFromFile(inName);
    }
    
    vector<TH1 *> vecHist_Sig(4), vecHist_Bkg(4);
    TString baseNameOptHist = "h_OptMap";
    vector<TString> vecNameOptHistMaps(4);
    if (!makeBkgs) {
        if (doVerb) {
            cout << "calculating total number for signal " << endl;
        }
        DrawCommands sigDC;
        sigDC.DefaultVarVals();
        sigDC.SetVecs();
        sigDC.SetAsTTbar();
        TLDM_Sig.GetTotalEventHist(DrawCommandTwo(&sigDC), doVerb);
        TLDM_Sig.AddToHistNames("Temp");
        for (unsigned int iChan = 0; iChan < numChannels; ++iChan) {
            TH3F * hTemp = (TH3F *) TLDM_Sig.vecFileHists[iChan];
            DoOverflow(hTemp);
            
            vecNameTH3sSignal_BetterBin[iChan] = TLDM_Sig.vecFileHists[iChan]->GetName();
            vecNameTH3sSignal_BetterBin[iChan] += "_BetterBin";
            
//            vecTH3sSignal_BetterBin[iChan] = new TH3F(vecNameTH3sSignal_BetterBin[iChan], TString(";") + axisStringMT2Var(2) + TString(";") + axisStringMT2Var(1) + TString(";") + axisStringMT2Var(0), vecNumBins[2], &vecVecBinEdges[2][0], vecNumBins[1], &vecVecBinEdges[1][0], vecNumBins[0], &vecVecBinEdges[0][0]);
            vecTH3sSignal_BetterBin[iChan] = new TH3F(vecNameTH3sSignal_BetterBin[iChan], TString(";") + axisStringMT2Var(0) + TString(";") + axisStringMT2Var(1) + TString(";") + axisStringMT2Var(2), vecNumBins[0], &vecVecBinEdges[0][0], vecNumBins[1], &vecVecBinEdges[1][0], vecNumBins[2], &vecVecBinEdges[2][0]);
            CopyToBetterBinnedHist(hTemp, vecTH3sSignal_BetterBin[iChan]);
            hTemp = vecTH3sSignal_BetterBin[iChan];
            ZeroOutNegHistBins(hTemp);
            hTemp->SetName(TLDM_Sig.nameTLDM + TLDM_Sig.vecChanNames[iChan]);
            TLDM_Sig.vecFileHists[iChan] = hTemp;
            
            
            vecHist_Sig[iChan] = (TH1 *) TLDM_Sig.vecFileHists[iChan];
            if (vecHist_Sig[3] == NULL) {
                vecHist_Sig[3] = (TH1 *)((TH3F *) TLDM_Sig.vecFileHists[iChan]->Clone(TLDM_Sig.nameTLDM + TString("_Comp")));
            }
            else {
                vecHist_Sig[3]->Add(TLDM_Sig.vecFileHists[iChan]);
            }
            if (tryHack) {
                TString baseDir = "ActualBkgShape/";
                TString nameTTbarHist = "TTBar";
                TString nameNonTTbarHist = "NonTTBar";
                TString nameTTbarFile = "TTBar";
                TString nameNonTTbarFile = "NonTTBar";
                TFile * currTTbarFile, * currNonTTbarFile;
                if (iChan != 3) {
                    nameTTbarFile += BasicLP.strFullSel;
                    nameTTbarFile += BasicLP.strSimFakes;

                    nameTTbarFile += TLDM_Sig.vecChanNames[iChan];
                    nameTTbarFile += "_ThreeDee.root";
                    currTTbarFile = TFile::Open(baseDir + nameTTbarFile);
                    
                    nameNonTTbarFile += BasicLP.strFullSel;
                    nameNonTTbarFile += BasicLP.strSimFakes;
                    nameNonTTbarFile += TLDM_Sig.vecChanNames[iChan];
                    nameNonTTbarFile += "_ThreeDee.root";
                    currNonTTbarFile = TFile::Open(baseDir + nameNonTTbarFile);
                    
                    cout << "currTTbarFile " << currTTbarFile->GetName() << endl;
                    cout << "currNonTTbarFile " << currNonTTbarFile->GetName() << endl;
                    
                    cout << "nameTTbarHist " << nameTTbarFile << endl;
                    cout << "nameNonTTbarHist " << nameNonTTbarFile << endl;
                    vecHist_Bkg[iChan] = (TH1 *) ((TH3F *) currTTbarFile->Get(nameTTbarHist));
                    vecHist_Bkg[iChan]->Add((TH3F *) currNonTTbarFile->Get(nameNonTTbarHist));
                    
                    if (vecHist_Bkg[3] == NULL) {
                        vecNameOptHistMaps[3] = baseNameOptHist + TString("_Comp");
                        vecHist_Bkg[3] = (TH1 *) ((TH3F *) vecHist_Bkg[iChan]->Clone(TLDM_Bkg_Comp.nameTLDM + TString("_Comp")));
                    }
                    else {
                        vecHist_Bkg[3]->Add(vecHist_Bkg[iChan]);
                    }
                    
                }
            }
            else {
                vecHist_Bkg[iChan] = (TH1 *) TLDM_Bkg_Comp.vecFileHists[iChan];
                if (vecHist_Bkg[3] == NULL) {
                    vecNameOptHistMaps[3] = baseNameOptHist + TString("_Comp");
                    vecHist_Bkg[3] = (TH1 *) ((TH3F *) TLDM_Bkg_Comp.vecFileHists[iChan]->Clone(TLDM_Bkg_Comp.nameTLDM + TString("_Comp")));
                }
                else {
                    vecHist_Bkg[3]->Add(TLDM_Bkg_Comp.vecFileHists[iChan]);
                }
            }
            vecNameOptHistMaps[iChan] = baseNameOptHist + TLDM_Sig.vecChanNames[iChan];
        }
        
        comboBinMaker.MakeComboMap(&vecHist_Sig, &vecHist_Bkg, &vecNameOptHistMaps, 3, doVerb);
        //comboBinMaker_SplitDilep.MakeComboMap(&vecHist_Sig, &vecHist_Bkg, &vecNameOptHistMaps, 3, doVerb);
        
    }
    else {
        vector<DrawCommands> vecDCs(numBkg);
        vector<TString> vecDrawStrings(numBkg);
        for (int iBkg = 0; iBkg < numBkg; ++iBkg) {
            vecDCs[iBkg].DefaultVarVals();
        }
        vecDCs[0].SetAsTTbar();
        vecDCs[1].SetAsDY();
        vecDCs[2].SetAsOther();
        vecDCs[3].SetAsFake();
        for (int iBkg = 0; iBkg < numBkg; ++iBkg) {
            vecDCs[iBkg].SetVecs();
            vecDrawStrings[iBkg] = DrawCommandTwo(&vecDCs[iBkg]);
        }
        
        for (int iBkg = 0; iBkg < numBkg; ++iBkg) {
            cout << "*****************************************" << endl;
            cout << "calculating total number for " << vecTLDM_Bkg[iBkg].nameTLDM << endl;
            cout << "*****************************************" << endl;
            vecTLDM_Bkg[iBkg].GetTotalEventHist(vecDrawStrings[iBkg], doVerb);
            vecTLDM_Bkg[iBkg].AddToHistNames("Temp");
            
            for (unsigned int iChan = 0; iChan < numChannels; ++iChan) {
                TH3F * hTemp = (TH3F *) vecTLDM_Bkg[iBkg].vecFileHists[iChan];
                cout << "iChan pre overflow " << hTemp->Integral() << endl;
                DoOverflow(hTemp);
                cout << "iChan post overflow " << hTemp->Integral() << endl;
                vecVecNameTH3sBackgrounds_BetterBin[iChan][iBkg] = vecTLDM_Bkg[iBkg].vecFileHists[iChan]->GetName();
                vecVecNameTH3sBackgrounds_BetterBin[iChan][iBkg] += "_BetterBin";
                vecVecTH3sBackgrounds_BetterBin[iChan][iBkg] = new TH3F(vecVecNameTH3sBackgrounds_BetterBin[iChan][iBkg], TString(";") + axisStringMT2Var(0) + TString(";") + axisStringMT2Var(1) + TString(";") + axisStringMT2Var(2), vecNumBins[0], &vecVecBinEdges[0][0], vecNumBins[1], &vecVecBinEdges[1][0], vecNumBins[2], &vecVecBinEdges[2][0]);
                CopyToBetterBinnedHist(hTemp, vecVecTH3sBackgrounds_BetterBin[iChan][iBkg]);
                hTemp = vecVecTH3sBackgrounds_BetterBin[iChan][iBkg];
                cout << "iChan post copy " << hTemp->Integral() << endl;
                ZeroOutNegHistBins(hTemp);
                cout << "iChan post zero-ing out " << hTemp->Integral() << endl;
                hTemp->SetName(vecTLDM_Bkg[iBkg].nameTLDM + vecTLDM_Bkg[iBkg].vecChanNames[iChan]);
                vecTLDM_Bkg[iBkg].vecFileHists[iChan] = hTemp;
                if (doVerb) {
                    cout << "total number of events " << vecTLDM_Bkg[iBkg].vecFileHists[iChan]->Integral() << endl;
                }
            }
        }
        MakeCompTLDM(&TLDM_Bkg_Comp, &vecTLDM_Bkg);
    }
    
    TFile * outFile = NULL;
    TString nameToWrite = makeBkgs ? inName : nameOutFile;
    outFile = TFile::Open(nameToWrite, "RECREATE");
    if (makeBkgs) {
        TLDM_Bkg_Comp.WriteToFile(outFile, makeBkgs, doVerb);
    }
    else {
        //comboBinMaker_SplitDilep.WriteToFile(outFile, 0, 2);
        comboBinMaker.WriteToFile(outFile, 0, 3);
    }
    if (doVerb) {
        cout << "going to write the output file now " << endl;
    }
    outFile->Write();
    outFile->Close();
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
