// Functions used as part of filling histograms in the plot-making macro

using namespace std;

#include "./StopFunctions_PlotFillingShowing_Strings.h"
#include "./StopFunctions_PlotFillingShowing_HistogramBooking.h"
#include "./StopFunctions_PlotFillingShowing_VariableMapping.h"

inline void SetPassCutMap(passCutMap &inputCutMap, vector<SampleT> * subSampVec, EventStructPointerInfo * inESPI, int whichDiLepTypeGlobal, bool hasMoreThan2Leps, bool doVerbosity = false) {
    
    float cutMT2ll = 80.;
    float cutMT2bb_ZMET = 170.;
    float cutMT2lblb = 170;
    
    for (unsigned int iSS = 0; iSS < subSampVec->size(); ++iSS) {
        inputCutMap[subSampVec->at(iSS).histNameSuffix] = false;
        if (whichDiLepTypeGlobal > -1 && inESPI->addELI->EventDiLepType != whichDiLepTypeGlobal) {
            if (doVerbosity) {
                cout << "cutting this event variation because it is failing dilepton cut! " << endl;
                cout << "required type " << whichDiLepTypeGlobal << endl;
                cout << "current type " << inESPI->addELI->EventDiLepType << endl;
            }
            continue;
        }
        if (!inESPI->addELI->doEvent) {
            if (doVerbosity) {
                cout << "cutting this event variation because it is the 'Do Event' cut! " << endl;
            }
            continue;
        }                        
        if (doVerbosity) cout << "test " << endl;
        if (subSampVec->at(iSS).whichdiLepType >= 0 && inESPI->addELI->EventDiLepType != subSampVec->at(iSS).whichdiLepType) continue;
        if (doVerbosity) cout << "test 2 " << endl;
        if (inESPI->addEJI->EventNJets < subSampVec->at(iSS).cutNJets) continue;
        if (doVerbosity) cout << "test 3" << endl;
        if (inESPI->addEJI->EventNBtagJets < subSampVec->at(iSS).cutNBJets) continue;
        if (doVerbosity) cout << "test 4" << endl;
        if (!(inESPI->addELI->EventDiLepType == 2 && subSampVec->at(iSS).histNameSuffix.Contains("FullCut"))) {
            if (inESPI->addEMI->EventMET < subSampVec->at(iSS).cutMET) continue;
            //            if (subSampVec->at(iSS).doZVeto >= 0 && inESPI->addELI->EventDiLepinZMass == subSampVec->at(iSS).doZVeto) continue;
            if (!(subSampVec->at(iSS).doZVeto < 0 || inESPI->addELI->EventDiLepinZMass != subSampVec->at(iSS).doZVeto)) continue;
        }
        if (subSampVec->at(iSS).histNameSuffix.Contains("FullCut")) {
            if (doVerbosity) {
                cout << "inESPI->addEMI->MET_EMT2I.EventMT2ll " << inESPI->addEMI->MET_EMT2I.EventMT2ll << endl;
                cout << "inESPI->addEMI->MET_EMT2I.EventMT2lblb " << inESPI->addEMI->MET_EMT2I.EventMT2lblb << endl;
                cout << "inESPI->addEMI->MET_EMT2I.EventMT2bb_ZMET " << inESPI->addEMI->MET_EMT2I.EventMT2bb_ZMET << endl;
                cout << "subSampVec->at(iSS).histNameSuffix "<< subSampVec->at(iSS).histNameSuffix << endl;
            }
            
            if (subSampVec->at(iSS).histNameSuffix.Contains("TTBarControl")) {
                if (inESPI->addEMI->MET_EMT2I.EventMT2ll > cutMT2ll || inESPI->addEMI->MET_EMT2I.EventMT2lblb > cutMT2lblb || inESPI->addEMI->MET_EMT2I.EventMT2bb_ZMET > cutMT2bb_ZMET) {
                    if (doVerbosity) {
                        cout << "continuing because event is not in control region " << endl;
                    }
                    continue;
                }
            }
            else if (subSampVec->at(iSS).histNameSuffix.Contains("Signal")) {
                if (inESPI->addEMI->MET_EMT2I.EventMT2ll < cutMT2ll && inESPI->addEMI->MET_EMT2I.EventMT2lblb < cutMT2lblb && inESPI->addEMI->MET_EMT2I.EventMT2bb_ZMET < cutMT2bb_ZMET) {
                    if (doVerbosity) {
                        cout << "continuing because event is not in signal region " << endl;
                    }
                    continue;
                }
            }
            else if (subSampVec->at(iSS).histNameSuffix.Contains("METSig1D")) {
                float cutValMETSig1D = 80;
                
                float METSig1DHack_AllJets = TMath::Power(inESPI->addEMI->EventMET / TMath::Sqrt(inESPI->addEJI->EventHT_AllJets), 2);
                if (inESPI->addELI->EventDiLepType != 2 && METSig1DHack_AllJets < cutValMETSig1D) {
                    if (doVerbosity) {
                        cout << "continuing because event has low 1D MET Significance " << endl;
                        cout << "METSig1DHack_AllJets " << METSig1DHack_AllJets << endl;
                    }
                    continue;
                }
            }
            else if (subSampVec->at(iSS).histNameSuffix.Contains("METSig2D")) {
                float cutValMETSig2D = 160;
                //cout << "inESPI->addEDSI->DP_DiLepton.BVC.Vec_Pt " << inESPI->addEDSI->DP_DiLepton.BVC.Vec_Pt << endl;
                float resoMET2DHack_Par_AllJets = inESPI->addEJI->EventHadResParGauss_AllJets;
                float resoMET2DHack_Perp_AllJets = inESPI->addEJI->EventHadResPerpGauss_AllJets;
                
                float METSig2DHack_Par_AllJets = (inESPI->addEMI->MET_ERI.recoilUPar + inESPI->addEDSI->DP_DiLepton.BVC.Vec_Pt) / resoMET2DHack_Par_AllJets;
                float METSig2DHack_Perp_AllJets = inESPI->addEMI->MET_ERI.recoilUPerp / resoMET2DHack_Perp_AllJets;
                
                float METSig2DHack_AllJets = TMath::Power(METSig2DHack_Perp_AllJets, 2) + TMath::Power(METSig2DHack_Par_AllJets, 2);
                
                if (inESPI->addELI->EventDiLepType != 2 && METSig2DHack_AllJets < cutValMETSig2D) {
                    if (doVerbosity) {
                        cout << "continuing because event has low 2D MET Significance " << endl;
                        cout << "METSig2DHack_AllJets " << METSig2DHack_AllJets << endl;
                    }
                    continue;
                }
            }
            else if (subSampVec->at(iSS).histNameSuffix.Contains("METSigTrue")) {
                float cutValMETSig2DTrue = 160;
                float METSig2DTrue = inESPI->addEMI->EventMETSigTrue;
                
                if (inESPI->addELI->EventDiLepType != 2 && METSig2DTrue < cutValMETSig2DTrue) {
                    if (doVerbosity) {
                        cout << "continuing because event has low True MET Significance " << endl;
                        cout << "METSig2DTrue " << METSig2DTrue << endl;
                    }
                    continue;
                }
            }
        }
        if (doVerbosity) cout << "test 5" << endl;
        if (subSampVec->at(iSS).histNameSuffix.Contains("BothinBarrel")) {
            if (!DiLeptonGeometry(inESPI->addELI, 0)) continue;
        }
        if (doVerbosity) cout << "test 6" << endl;
        if (subSampVec->at(iSS).histNameSuffix.Contains("OneinBarrel")) { 
            if (!DiLeptonGeometry(inESPI->addELI, 1)) continue;
        }   
        if (doVerbosity) cout << "test 7" << endl;
        if (subSampVec->at(iSS).histNameSuffix.Contains("BothinEndcap")) {
            if (!DiLeptonGeometry(inESPI->addELI, 2)) continue;
        }
        if (doVerbosity) cout << "test 8" << endl;
        if (subSampVec->at(iSS).histNameSuffix.Contains("0BJets")) {
            if (inESPI->addEJI->EventNBtagJets > 0) {
                if (!(subSampVec->at(iSS).histNameSuffix.Contains("inZMass") && inESPI->addELI->EventDiLepinZMass)) continue;
            }
        }
        if (subSampVec->at(iSS).histNameSuffix.Contains("ZCR")) {
            if (inESPI->addEJI->EventNBtagJets > 0) continue;
        }
        if (subSampVec->at(iSS).histNameSuffix.Contains("LowBLepMass")) {
            if (inESPI->addEJI->EventNJets < 2) continue;
            if (inESPI->addEMI->MET_EMT2I.EventMassBLep0_BLepsUsed > 200 || inESPI->addEMI->MET_EMT2I.EventMassBLep1_BLepsUsed > 200) continue;
            if (subSampVec->at(iSS).histNameSuffix.Contains("LowBLepMass170")) {
                if (inESPI->addEMI->MET_EMT2I.EventMassBLep0_BLepsUsed > 170 || inESPI->addEMI->MET_EMT2I.EventMassBLep1_BLepsUsed > 170) continue;
            }
        }
        if (doVerbosity) cout << "test 9" << endl;
        if (subSampVec->at(iSS).histNameSuffix.Contains("_0Jets") && inESPI->addEJI->EventNJets != 0) continue;
        if (doVerbosity) cout << "test 10" << endl;
        if (subSampVec->at(iSS).histNameSuffix.Contains("_1Jet") && inESPI->addEJI->EventNJets != 1) continue;
        if (doVerbosity) cout << "test 11" << endl;
        if (subSampVec->at(iSS).histNameSuffix.Contains("FullCutBlind")) {
            if (inESPI->addEMI->MET_EMT2I.EventMT2ll > 80) continue;
        }
        if (doVerbosity) cout << "test 12" << endl;
        if (subSampVec->at(iSS).histNameSuffix.Contains("_FullCutLowMT2ll") && inESPI->addEMI->MET_EMT2I.EventMT2ll > 5) continue;
        if (doVerbosity) cout << "test 13" << endl;
        if (subSampVec->at(iSS).histNameSuffix.Contains("Only2Leps") && hasMoreThan2Leps) continue;
        if (doVerbosity) cout << "test 14" << endl;
        if (subSampVec->at(iSS).histNameSuffix.Contains("ISR")) {
            if (subSampVec->at(iSS).histNameSuffix.Contains("30") && !ISRJetCutFullSelection(inESPI->addEJI, 30)) continue;
            else if (subSampVec->at(iSS).histNameSuffix.Contains("100") && !ISRJetCutFullSelection(inESPI->addEJI, 100)) continue;
            else if (subSampVec->at(iSS).histNameSuffix.Contains("150") && !ISRJetCutFullSelection(inESPI->addEJI, 150)) continue;
            else if (subSampVec->at(iSS).histNameSuffix.Contains("200") && !ISRJetCutFullSelection(inESPI->addEJI, 200)) continue;
        }
        if (doVerbosity) cout << "test 15: passes!" << endl;
        inputCutMap[subSampVec->at(iSS).histNameSuffix] = true;
        if (doVerbosity) cout << "set inputCut Map to be true for S_Current = " << subSampVec->at(iSS).histNameSuffix << endl;
    }    
}
inline void HistogramFill(passCutMap * inputCutMap, StV_Map * inStVM, vector<SampleT> * subSampVec, vector<HistogramT> * HistTVec, HMap_1D * inputHMap1D, HMap_2D * inputHMap2D, HMap_3D * inputHMap3D, EventStructPointerInfo * inESPI, int numDims = 1, TString stringSystCheck = "", bool doVerbosity = false) {
    
    float fillWeight;
    StV_Map::iterator xIter;
    StV_Map::iterator yIter;
    StV_Map::iterator zIter;
    float MT2llCut = 80;
    float MT2lbCut = 170;
    
    bool doFill = false;      
    
    for (unsigned int iSS = 0; iSS < subSampVec->size(); ++iSS) {
        if (doVerbosity) {
            cout << "about to check if passes inputCutMap " << endl;
        }
        if ((*inputCutMap)[subSampVec->at(iSS).histNameSuffix]) {
            if (doVerbosity) {
                cout << "passed inputCut Map for S_Current = " << subSampVec->at(iSS).histNameSuffix << endl;
            }
            for (unsigned int iHT = 0; iHT < HistTVec->size(); ++iHT) {
                if (doVerbosity) {
                    cout << "current Syst to check against " << stringSystCheck << endl;
                }
                if (stringSystCheck.Length() != 0) {
                    if (!HistTVec->at(iHT).name.Contains(stringSystCheck)) continue;
                }
                xIter = inStVM->find(HistTVec->at(iHT).xAxis.axisVarKey);
                if (numDims > 1) {
                    yIter = inStVM->find(HistTVec->at(iHT).yAxis.axisVarKey);
                    if (numDims > 2) {
                        zIter = inStVM->find(HistTVec->at(iHT).zAxis.axisVarKey);
                    }
                }                
                if (doVerbosity) {
                    cout << "" << endl;
                    cout << "iSS " << iSS << endl;
                    cout << "iHT " << iHT << endl;
                    cout << "Hist name " << HistTVec->at(iHT).name << endl;
                    cout << "S_Current.histNameSuffix " << subSampVec->at(iSS).histNameSuffix << endl;
                    cout << "HistTVec->at(iHT).xAxis.axisVarKey " << HistTVec->at(iHT).xAxis.axisVarKey << endl;
                    if (numDims > 1) {
                        cout << "HistTVec->at(iHT).yAxis.axisVarKey " << HistTVec->at(iHT).yAxis.axisVarKey << endl;
                        if (numDims > 2) {
                            cout << "HistTVec->at(iHT).zAxis.axisVarKey " << HistTVec->at(iHT).zAxis.axisVarKey << endl;
                        }
                    }
                }
                switch (numDims) {
                    case 1:
                        doFill = xIter != inStVM->end();
                        break;
                    case 2:
                        doFill = xIter != inStVM->end() && yIter != inStVM->end();
                        break;
                    case 3:
                        doFill = xIter != inStVM->end() && yIter != inStVM->end() && zIter != inStVM->end();
                        break;
                    default:
                        cout << "num of Dims should be 1, 2, or 3 -- it is " << numDims << endl;
                        break;
                }
                if (doFill) {
                    if (doVerbosity) {
                        cout << "xIter first " << xIter->first << endl;
                        cout << "xIter second " << xIter->second << endl;
                        if (numDims > 1) {
                            cout << "yIter first " << yIter->first << endl;
                            cout << "yIter second " << yIter->second << endl;
                            if (numDims > 2) {
                                cout << "zIter first " << zIter->first << endl;
                                cout << "zIter second " << zIter->second << endl;
                            }
                        }
                    }
                    //Check to see if should be blinded data
                    if (subSampVec->at(iSS).blindDataChannel && inESPI->addBEI->blindData && inESPI->addBEI->doData) {
                        //cout << "going to check if blinded" << endl;
                        if (HistTVec->at(iHT).HistContainsStringAsAxisVar("MT2ll", numDims) && inESPI->addEMI->MET_EMT2I.EventMT2ll > MT2llCut) continue;
                        if (HistTVec->at(iHT).HistContainsStringAsAxisVar("MT2lb", numDims) && inESPI->addEMI->MET_EMT2I.EventMT2lblb > MT2lbCut) continue;                        
                    }
                    
                    if (!HistTVec->at(iHT).EventPassesHistSpecificCut(inStVM, doVerbosity)) continue;
                    
                    if (HistTVec->at(iHT).name.Contains("BasicWeight")) {
                        fillWeight = inESPI->addBEI->basicWeight;
                    }
                    else {
                        fillWeight = HistTVec->at(iHT).name.Contains("preRW") ? inESPI->addBEI->preNVtxRWWeight : inESPI->addBEI->weight;
                    }
                    //                    fillWeight = HistTVec->at(iHT).name.Contains("preRW") ? inESPI->addBEI->preNVtxRWWeight : inESPI->addBEI->weight;
                    if (HistTVec->at(iHT).name.Contains("h_ChannelCutFlow")) fillWeight = 1.;
                    switch (numDims) {
                        case 1:
                            (*inputHMap1D)[histKeyString(HistTVec->at(iHT), subSampVec->at(iSS).histNameSuffix)]->Fill(xIter->second, fillWeight);
                            break;
                        case 2:
                            (*inputHMap2D)[histKeyString(HistTVec->at(iHT), subSampVec->at(iSS).histNameSuffix)]->Fill(xIter->second, yIter->second, fillWeight);
                            break;
                        case 3:
                            (*inputHMap3D)[histKeyString(HistTVec->at(iHT), subSampVec->at(iSS).histNameSuffix)]->Fill(xIter->second, yIter->second, zIter->second, fillWeight);
                            break;
                        default:
                            cout << "num of Dims should be 1, 2, or 3 -- it is " << numDims << endl;
                            break;
                    }
                }
                else {
                    if (doVerbosity) {
                        cout << "printing why I failed! " << endl;
                        if (xIter != inStVM->end()) {
                            cout << "I was able to find the key: " << HistTVec->at(iHT).xAxis.axisVarKey << " in the map! " << endl;
                        }
                        else {
                            cout << "I couldn't find the key: " << HistTVec->at(iHT).xAxis.axisVarKey << " in the map! " << endl;   
                        }                    
                        if (numDims > 1) {
                            if (yIter != inStVM->end()) {
                                cout << "I was able to find the key: " << HistTVec->at(iHT).yAxis.axisVarKey << " in the map! " << endl;
                            }
                            else {
                                cout << "I couldn't find the key: " << HistTVec->at(iHT).yAxis.axisVarKey << " in the map! " << endl;   
                            }
                            if (numDims > 2) {
                                if (zIter != inStVM->end()) {
                                    cout << "I was able to find the key: " << HistTVec->at(iHT).zAxis.axisVarKey << " in the map! " << endl;
                                }
                                else {
                                    cout << "I couldn't find the key: " << HistTVec->at(iHT).zAxis.axisVarKey << " in the map! " << endl;   
                                }
                            }
                        }
                    }
                }
                if (doVerbosity) cout << "" << endl;
            }
        }
    }
}

inline void SetupMapsAndFillHistograms(StV_Map &inStVM, passCutMap &inPCM, vector<SampleT> * subSampVec, EventStructPointerInfo * inESPI, EventPileUpInfo * inEPI, vector< vector<HistogramT> *> * inVecVecHistT, HMap_1D * inputHMap1D, HMap_2D * inputHMap2D, HMap_3D * inputHMap3D, float METSig_Raw, int numSpaceDimensions, int systDimStop, int whichSyst = 0, bool isData = false, bool isSmear = false, int whichDiLepTypeGlobal = -1, bool doVerbosity = false) {
    bool hasMoreThan2Leps = inESPI->addELI->HasMoreThanNLeps();
    
    TString systStringCheck = "";    
    if (whichSyst != 0 && !isData) {
        systStringCheck = SystString_v4(whichSyst);
    }
    else if (whichSyst != 0 && isData) {
        systStringCheck = SystString_FakeLep(whichSyst);
    }
    
    /// Set up Event Variables Map
    SetStringKey_StFMap(inStVM, inESPI, inEPI, METSig_Raw);
    if (doVerbosity) {
        TString verboseString = isSmear ? "Smeared " : "";
        verboseString += whichSyst == 0 ? "centVal" : systStringCheck;
        cout << "going to print out the StVMap contents " << endl;
        for (StV_Map::iterator xIter = inStVM.begin(); xIter != inStVM.end(); ++xIter) {
            cout << verboseString << " xIter first " << xIter->first << endl;
            cout << verboseString << " xIter second " << xIter->second << endl;
        }
    }
    if (doVerbosity) {
        cout << "whichDiLepTypeGlobal? " << whichDiLepTypeGlobal << endl;
        cout << "event type: " << inESPI->addELI->EventDiLepType << endl;
    }
    /// Set up Event Passes Sub-cuts Map
    SetPassCutMap(inPCM, subSampVec, inESPI, whichDiLepTypeGlobal, hasMoreThan2Leps, doVerbosity);
    
    /// Fill histograms
    for (int iDim = 0; iDim < numSpaceDimensions; ++iDim) {
        if (whichSyst != 0 && iDim >= systDimStop) continue;
        HistogramFill(&inPCM, &inStVM, subSampVec, inVecVecHistT->at(iDim), inputHMap1D, inputHMap2D, inputHMap3D, inESPI, iDim + 1, systStringCheck, doVerbosity);
    }
}



TString SystString_PDF(int whichSyst) {
    int numCT10 = 25;
    int numMSTW = 23;
    int numNNPDF = 50;
    TString baseNameCT10 = "CT10EV";
    TString baseNameMSTW = "MSTWEV";
    TString baseNameNNPDF = "NNPDFEV";
    
    TString baseString = "";
    if (abs(whichSyst) == 1) {
        baseString = "genRecoilRW";
    }
    else if (abs(whichSyst) > 1 && abs(whichSyst) <= numCT10 + 1) {
        baseString = baseNameCT10;
        baseString += abs(whichSyst) - 1;
    }
    else if (abs(whichSyst) > 1 + numCT10 && abs(whichSyst) <= numCT10 + numMSTW + 1) {
        baseString = baseNameMSTW;
        baseString += abs(whichSyst) - 1 - numCT10;
    }
    else if (abs(whichSyst) > 1 + numCT10 + numMSTW && abs(whichSyst) <= numCT10 + numMSTW + numNNPDF + 1) {
        baseString = baseNameNNPDF;
        baseString += abs(whichSyst) - 1 - numCT10 - numMSTW;
    }
    if (whichSyst < 0) {
        baseString += "ShiftDown";
    }
    else {
        baseString += "ShiftUp";
    }
    return baseString;
}

inline void SetupMapsAndFillHistograms_PDF(StV_Map &inStVM, passCutMap &inPCM, vector<SampleT> * subSampVec, EventStructPointerInfo * inESPI, EventPileUpInfo * inEPI, vector< vector<HistogramT> *> * inVecVecHistT, HMap_1D * inputHMap1D, HMap_2D * inputHMap2D, HMap_3D * inputHMap3D, float METSig_Raw, int systDimStop, int whichSyst = 0, bool isData = false, bool isSmear = false, int whichDiLepTypeGlobal = -1, bool doVerbosity = false) {
    
    int numSpaceDimensions = 3;
    bool hasMoreThan2Leps = inESPI->addELI->HasMoreThanNLeps();
    
    TString systStringCheck = "";
    if (whichSyst != 0 && !isData) {
        systStringCheck = SystString_PDF(whichSyst);
    }
    
    /// Set up Event Variables Map
    SetStringKey_StFMap(inStVM, inESPI, inEPI, METSig_Raw);
    if (doVerbosity) {
        TString verboseString = isSmear ? "Smeared " : "";
        verboseString += whichSyst == 0 ? "centVal" : systStringCheck;
        cout << "going to print out the StVMap contents " << endl;
        for (StV_Map::iterator xIter = inStVM.begin(); xIter != inStVM.end(); ++xIter) {
            cout << verboseString << " xIter first " << xIter->first << endl;
            cout << verboseString << " xIter second " << xIter->second << endl;
        }
    }
    if (doVerbosity) {
        cout << "whichDiLepTypeGlobal? " << whichDiLepTypeGlobal << endl;
        cout << "event type: " << inESPI->addELI->EventDiLepType << endl;
    }
    /// Set up Event Passes Sub-cuts Map
    SetPassCutMap(inPCM, subSampVec, inESPI, whichDiLepTypeGlobal, hasMoreThan2Leps, doVerbosity);
    
    /// Fill histograms
    for (int iDim = 0; iDim < numSpaceDimensions; ++iDim) {
        if (whichSyst != 0 && iDim >= systDimStop) continue;
        HistogramFill(&inPCM, &inStVM, subSampVec, inVecVecHistT->at(iDim), inputHMap1D, inputHMap2D, inputHMap3D, inESPI, iDim + 1, systStringCheck, doVerbosity);
    }
}



