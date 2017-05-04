#ifndef SignalYieldMaker_IndivPoint_cc
#define SignalYieldMaker_IndivPoint_cc
void SignalYieldMaker_IndivPoint::DoSignalContamination_SpecHist(TH1 * histSignal, TH1 * histTTBar, int numDims, int specSigContam = 0, bool doVerb = false) {
    //Function for changing the histogram histSignal to include signal contamination in the ttbar contribution -- this changes the
    //total hypothesis S+B by changing the S
    //specSigContam = 0 is basic one where you calculate the signal contamination using central value
    //specSigContam = 1 is sig contam accounting for the signal MC statistics error in contorl region
    //specSigContam = 2 is sig contam accounting for the TTBar MC statistics error in control region
    //specSigContam = 3 is sig contam accounting for the TTBar MC statistics error in signal region
    float yieldControl_TTBar = histTTBar->GetBinContent(1, 1, 1);
    float errControl_TTBar = histTTBar->GetBinError(1, 1, 1);
    
    float yieldControl_Signal = histSignal->GetBinContent(1, 1, 1);
    float errControl_Signal = histSignal->GetBinError(1, 1, 1);
    
    float SF = yieldControl_Signal / yieldControl_TTBar;
    float SF_Err_TTCont = SF * (errControl_TTBar / yieldControl_TTBar);
    float SF_Err_SigCont = SF * errControl_Signal / yieldControl_TTBar;
    float SF_Err_TTSig = 0.0;
    
    if (doVerb) {
        cout << "yieldControl_TTBar " << yieldControl_TTBar << endl;
        cout << "errControl_TTBar " << errControl_TTBar << endl;
        cout << "yieldControl_Signal " << yieldControl_Signal << endl;
        cout << "errControl_Signal " << errControl_Signal << endl;
        cout << "for histSignal " << histSignal->GetName() << endl;
        cout << "for histTTBar " << histTTBar->GetName() << endl;
        cout << "SF " << SF << endl;
        if (specSigContam != 0) {
            cout << "SF_Err_TTCont " << SF_Err_TTCont << endl;
            cout << "SF_Err_SigCont " << SF_Err_SigCont << endl;
        }
    }
    
    int nBinsX = histSignal->GetNbinsX();
    int nBinsY = histSignal->GetNbinsY();
    int nBinsZ = histSignal->GetNbinsZ();
    
    int startIndexX = 0;
    int endIndexX = nBinsX + 1;
    
    int startIndexY = numDims > 1 ? 0 : 1;
    int endIndexY = numDims > 1 ? nBinsY + 1 : 1;
    
    int startIndexZ = numDims > 2 ? 0 : 1;
    int endIndexZ = numDims > 2 ? nBinsZ + 1 : 1;
    
    float currBinSig, currBinTTBar;
    float newBinSig;
    float currBinErrSig, currBinErrTTBar;
    
    float currCorrFactor, currCorrFactorErr;
    for (int iBinX = startIndexX; iBinX <= endIndexX; ++iBinX) {
        for (int iBinY = startIndexY; iBinY <= endIndexY; ++iBinY) {
            for (int iBinZ = startIndexZ; iBinZ <= endIndexZ; ++iBinZ) {
                currBinSig = histSignal->GetBinContent(iBinX, iBinY, iBinZ);
                currBinErrSig = histSignal->GetBinError(iBinX, iBinY, iBinZ);
                
                currBinTTBar = histTTBar->GetBinContent(iBinX, iBinY, iBinZ);
                currBinErrTTBar = histTTBar->GetBinError(iBinX, iBinY, iBinZ);
                
                currCorrFactor = SF * currBinTTBar;
                if (doVerb) {
                    cout << "{iX,iY,iZ} = {" << iBinX << ":" << iBinY << ":" << iBinZ << "}" << endl;
                    cout << "currBinSig " << currBinSig << endl;
                    cout << "currBinTTBar " << currBinTTBar << endl;
                    cout << "currCorrFactor " << currCorrFactor << endl;
                }
                if (specSigContam == 0) {
                    currCorrFactorErr = 0.0;
                }
                else if (abs(specSigContam) == 1) {
                    currCorrFactorErr = currBinTTBar * SF_Err_SigCont;
                }
                else if (abs(specSigContam) == 2) {
                    currCorrFactorErr = currBinTTBar * SF_Err_TTCont;
                }
                else if (abs(specSigContam) == 3) {
                    SF_Err_TTSig = SF * currBinErrTTBar / yieldControl_TTBar;
                    currCorrFactorErr = currBinTTBar * SF_Err_TTSig;
                    if (doVerb) {
                        cout << "SF_Err_TTSig " << SF_Err_TTSig << endl;
                    }
                }
                if (doVerb && specSigContam != 0) {
                    cout << "currCorrFactorErr " << currCorrFactorErr << endl;
                }
                currCorrFactor += TMath::Sign(currCorrFactorErr, (float) specSigContam);
                newBinSig = currBinSig - currCorrFactor;
                if (newBinSig < 0) newBinSig = 0;
                histSignal->SetBinContent(iBinX, iBinY, iBinZ, newBinSig);
                if (currBinSig > 0) {
                    histSignal->SetBinError(iBinX, iBinY, iBinZ, currBinErrSig * newBinSig/currBinSig);
                }
                if (doVerb) {
                    cout << "currBinSig " << currBinSig << endl;
                    cout << "newBinSig " << newBinSig << endl;
                }
            }
        }
    }
}
void SignalYieldMaker_IndivPoint::DoSignalContamination(TFile * inputFileTTBarShapes, bool doVerb = false) {
    //1) Implements the signal contamination from the MT2 control region and the TTBar values therein
    //2) Adds in three statistical systematic uncertainties
    //2a) TTBar Stat. Uncertainty in the TTBar Control Region
    //2b) TTBar Stat. Uncertainty in each Signal bin
    //2c) Signal Stat. Uncertainty in the TTBar Control Region
    
    TString nameTTBarHist = "TTBar";
    
    TH1 * histToUseSig_CV;
    TH1 * histToUseTTBar_CV = (TH1 *) inputFileTTBarShapes->Get(nameTTBarHist);
    
    TH1 * histToUseSig_SystVarUp, * histToUseSig_SystVarDown;
    TH1 * histToUseTTBar_SystVarUp, * histToUseTTBar_SystVarDown;
    
    TString nameSigSyst_SigStatControl = "_SigStatControl";
    TString nameSigSyst_TTBarStatCont = "_TTBarStatCont";
    TString nameSigSyst_TTBarMCStat = "_TTBarMCStat";
    
    //Do CV
    if (numDims == 1) {
        histToUseSig_CV = outMT2Hist_CV_1D;
    }
    else if (numDims == 2) {
        histToUseSig_CV = outMT2Hist_CV_2D;
    }
    else if (numDims == 3) {
        histToUseSig_CV = outMT2Hist_CV_3D;
    }
    
    //Clone the CV hists into versions to make sig contamination stat. err shifts
    histSig_SigStatControlUp = (TH1 *) histToUseSig_CV->Clone(histToUseSig_CV->GetName() + nameSigSyst_SigStatControl + TString("Up"));
    histSig_SigStatControlDown = (TH1 *) histToUseSig_CV->Clone(histToUseSig_CV->GetName() + nameSigSyst_SigStatControl + TString("Down"));
    
    histSig_TTBarStatContUp = (TH1 *) histToUseSig_CV->Clone(histToUseSig_CV->GetName() + nameSigSyst_TTBarStatCont + TString("Up"));
    histSig_TTBarStatContDown = (TH1 *) histToUseSig_CV->Clone(histToUseSig_CV->GetName() + nameSigSyst_TTBarStatCont + TString("Down"));
    
    histSig_TTBarMCStatUp = (TH1 *) histToUseSig_CV->Clone(histToUseSig_CV->GetName() + nameSigSyst_TTBarMCStat + TString("Up"));
    histSig_TTBarMCStatDown = (TH1 *) histToUseSig_CV->Clone(histToUseSig_CV->GetName() + nameSigSyst_TTBarMCStat + TString("Down"));
    
    //Signal contamination for central value
    DoSignalContamination_SpecHist(histToUseSig_CV, histToUseTTBar_CV, numDims, 0, doVerb);
    
    //Signal contamination for central value
    
    //specSigContam = 1 is sig contam accounting for the signal MC statistics error in contorl region
    //specSigContam = 2 is sig contam accounting for the TTBar MC statistics error in control region
    //specSigContam = 3 is sig contam accounting for the TTBar MC statistics error in signal region
    DoSignalContamination_SpecHist(histSig_SigStatControlUp, histToUseTTBar_CV, numDims, 1, doVerb);
    DoSignalContamination_SpecHist(histSig_SigStatControlDown, histToUseTTBar_CV, numDims, -1, doVerb);
    
    DoSignalContamination_SpecHist(histSig_TTBarStatContUp, histToUseTTBar_CV, numDims, 2, doVerb);
    DoSignalContamination_SpecHist(histSig_TTBarStatContDown, histToUseTTBar_CV, numDims, -2, doVerb);
    
    DoSignalContamination_SpecHist(histSig_TTBarMCStatUp, histToUseTTBar_CV, numDims, 3, doVerb);
    DoSignalContamination_SpecHist(histSig_TTBarMCStatDown, histToUseTTBar_CV, numDims, -3, doVerb);
    
    //write the control region stat. syst. histograms
    /*
     outFile->cd();
     histSig_SigStatControlUp->Write();
     histSig_SigStatControlDown->Write();
     histSig_TTBarStatContUp->Write();
     histSig_TTBarStatContDown->Write();
     histSig_TTBarMCStatUp->Write();
     histSig_TTBarMCStatDown->Write();
     */
    
    for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
        //Set Syst Var hists for Signal
        if (numDims == 1) {
            histToUseSig_SystVarUp = vecOutMT2Hist_1D_SystVarUp[iSyst];
            histToUseSig_SystVarDown = vecOutMT2Hist_1D_SystVarDown[iSyst];
        }
        else if (numDims == 2) {
            histToUseSig_SystVarUp = vecOutMT2Hist_2D_SystVarUp[iSyst];
            histToUseSig_SystVarDown = vecOutMT2Hist_2D_SystVarDown[iSyst];
        }
        else if (numDims == 3) {
            histToUseSig_SystVarUp = vecOutMT2Hist_3D_SystVarUp[iSyst];
            histToUseSig_SystVarDown = vecOutMT2Hist_3D_SystVarDown[iSyst];
        }
        histToUseTTBar_SystVarUp = (TH1 *) inputFileTTBarShapes->Get(nameTTBarHist + vecSystNames[iSyst] + TString("Up"));
        histToUseTTBar_SystVarDown = (TH1 *) inputFileTTBarShapes->Get(nameTTBarHist + vecSystNames[iSyst] + TString("Down"));
        
        if (!histToUseTTBar_SystVarUp) {
            if (doVerb) {
                cout << "for syst " << vecSystNames[iSyst] << " no corresponding TTBar syst up variation " << endl;
            }
            histToUseTTBar_SystVarUp = histToUseTTBar_CV;
        }
        if (!histToUseTTBar_SystVarDown) {
            if (doVerb) {
                cout << "for syst " << vecSystNames[iSyst] << " no corresponding TTBar syst down variation " << endl;
            }
            histToUseTTBar_SystVarDown = histToUseTTBar_CV;
        }
        DoSignalContamination_SpecHist(histToUseSig_SystVarUp, histToUseTTBar_SystVarUp, numDims, 0, doVerb);
        DoSignalContamination_SpecHist(histToUseSig_SystVarDown, histToUseTTBar_SystVarDown, numDims, 0, doVerb);
    }
}
void SignalYieldMaker_IndivPoint::SetAxes() {
    for (unsigned int iMT2 = 0; iMT2 < vecWhichMT2.size(); ++iMT2) {
        switch (vecWhichMT2[iMT2]) {
            case 0:
                //MT2ll axis
                if (numDims == 1) {
                    AddInMT2llBounds_Control(&vecAxes, &vecNumBinsAxes);
                }
                else if (numDims == 2) {
                    AddInMT2llBounds2DShape(&vecAxes, &vecNumBinsAxes);
                }
                else if (numDims == 3) {
                    AddInMT2llBounds3DShape(&vecAxes, &vecNumBinsAxes);
                }
                break;
            case 1:
                //MT2lblb axis
                if (numDims == 1) {
                    AddInMT2lblbBounds_Control(&vecAxes, &vecNumBinsAxes);
                }
                else if (numDims == 2) {
                    AddInMT2lblbBounds2DShape(&vecAxes, &vecNumBinsAxes);
                }
                else if (numDims == 3) {
                    AddInMT2lblbBounds3DShape(&vecAxes, &vecNumBinsAxes);
                }
                break;
            case 2:
                //MT2bb_ZMET axis
                if (numDims == 1) {
                    AddInMT2bb_ZMETBounds_Control(&vecAxes, &vecNumBinsAxes);
                }
                else if (numDims == 2) {
                    AddInMT2bb_ZMETBounds2DShape(&vecAxes, &vecNumBinsAxes);
                }
                else if (numDims == 3) {
                    AddInMT2bb_ZMETBounds3DShape(&vecAxes, &vecNumBinsAxes);
                }
                break;
            default:
                cout << "vecWhichMT2[iMT2 = " << iMT2 << "] needs to be 0, 1, or 2, right now it is " << vecWhichMT2[iMT2] << endl;
                break;
        }
    }
}
void SignalYieldMaker_IndivPoint::DefaultVals(int inNumDims = 2) {
    numDims = inNumDims;
    
    //initialize the input directories to the correct area
    /*
     inBaseDir = "EfficiencyMaps/T2tt/PerPol0/SignalYields/SignalRegion/";
     outBaseDir = "EfficiencyMaps/T2tt/PerPol0/";
     */
    inBaseDir = "T2tt/PerPol0/SignalYields/SignalRegion/";
    outBaseDir = "T2tt/PerPol0/";
    
    
    //default is MT2ll vs. MT2lblb shape
    TString arrNameMT2[3] = {"MT2ll", "MT2lblb", "MT2bb_ZMET"};
    TString arrAxisNameMT2[3] = {"M_{T2}(ll) [GeV]", "M_{T2}(lb)(lb) [GeV]", "M_{T2}^{#slash{W}}(bb) [GeV]"};
    
    vecWhichMT2.resize(0);
    vecWhichMT2.push_back(0);
    if (numDims > 1) {
        vecWhichMT2.push_back(1);
        if (numDims > 2) {
            vecWhichMT2.push_back(2);
        }
    }
    
    vecMT2Name.resize(0);
    axisString = "";
    
    for (unsigned int iAxis = 0; iAxis < vecWhichMT2.size(); ++iAxis) {
        vecMT2Name.push_back(arrNameMT2[vecWhichMT2[iAxis]]);
        
        axisString += ";";
        axisString += arrAxisNameMT2[vecWhichMT2[iAxis]];
    }
    
    vecAxes.resize(0);
    vecNumBinsAxes.resize(0);
    
    SetAxes();
}
void SignalYieldMaker_IndivPoint::SetStringSignal(SUSYT2LimitParams * inSUSYT2LP, SUSYMassLimitParams * inSUSYMLP, LimitParametersContainer * inLPC, bool doSigContam = 0) {
    //        inBaseDir = "EfficiencyMaps/" + inSUSYT2LP->SavingName(true) + TString("/SignalYields/SignalRegion/");
    //        outBaseDir = "EfficiencyMaps/" + inSUSYT2LP->SavingName(true) + TString("/SignalYieldMT2Histograms/") + inSUSYMLP->SavingName(true);
    inBaseDir = inSUSYT2LP->SavingName(true) + TString("/SignalYields/SignalRegion/");
    outBaseDir = inSUSYT2LP->SavingName(true) + TString("/SignalYieldMT2Histograms/") + inSUSYMLP->SavingName(true);
    
    
    System_MakeDir(outBaseDir);
    
    outFileName = "h_" + inSUSYMLP->SavingName(false) + inSUSYT2LP->SavingName(false);
    if (inLPC->typeDilepChannel > -1) outFileName += inLPC->strDilepChannel;
    outFileName += inLPC->strFullSel;
    outFileName += "_" + vecMT2Name[0];
    if (vecMT2Name.size() > 1) {
        outFileName += "_vs_" + vecMT2Name[1];
        if (vecMT2Name.size() > 2) {
            outFileName += "_vs_" + vecMT2Name[2];
        }
    }
    outHistBaseName = outFileName;
    outFileName += doSigContam ? "_WithSigContam" : "_NoSigContam";
    outFileName += ".root";
    
    baseNameInFile = "h_SigYield_" + inSUSYT2LP->SavingName(false);
    
    if (inLPC->typeDilepChannel > -1) baseNameInFile += inLPC->strDilepChannel;
    baseNameInFile += inLPC->strFullSel;
    baseNameInFile += TString("_") + vecMT2Name[0] + TString("FPATSYAND");
    if (vecMT2Name.size() > 1) {
        baseNameInFile += "_" + vecMT2Name[1] + "SPATSY";
        if (vecMT2Name.size() > 2) {
            baseNameInFile += "AND_" + vecMT2Name[2] + "TPATSY";
        }
    }
    else {
        baseNameInFile += "_MT2lblb0";
    }
}
void SignalYieldMaker_IndivPoint::InitializeVecs() {
    vecSystNames.clear();
    SetSystVec(&vecSystNames);
    vecSystNames.push_back("_PDF");
    unsigned int numSysts = vecSystNames.size();
    
    vecCurrInHist_SystVarUp.resize(numSysts);
    vecCurrInHist_SystVarDown.resize(numSysts);
    
    vecOutMT2Hist_1D_SystVarUp.resize(numSysts);
    vecOutMT2Hist_1D_SystVarDown.resize(numSysts);
    vecOutMT2Hist_2D_SystVarUp.resize(numSysts);
    vecOutMT2Hist_2D_SystVarDown.resize(numSysts);
    vecOutMT2Hist_3D_SystVarUp.resize(numSysts);
    vecOutMT2Hist_3D_SystVarDown.resize(numSysts);
    
}

void SignalYieldMaker_IndivPoint::FixMCStats(bool doVerb = 0) {
    TH1 * histCV, * histMCStatUp, * histMCStatDown;
    int indexMCStat = -1;
    for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
        if (vecSystNames[iSyst].Contains("MCStat")) {
            indexMCStat = iSyst;
        }
    }
    if (indexMCStat == -1) {
        cout << "failed to find MCStat index " << endl;
    }
    if (numDims == 1) {
        histCV = outMT2Hist_CV_1D;
        histMCStatUp = vecOutMT2Hist_1D_SystVarUp[indexMCStat];
        histMCStatDown = vecOutMT2Hist_1D_SystVarDown[indexMCStat];
    }
    else if (numDims == 2) {
        histCV = outMT2Hist_CV_2D;
        histMCStatUp = vecOutMT2Hist_2D_SystVarUp[indexMCStat];
        histMCStatDown = vecOutMT2Hist_2D_SystVarDown[indexMCStat];
    }
    else if (numDims == 3) {
        histCV = outMT2Hist_CV_3D;
        histMCStatUp = vecOutMT2Hist_3D_SystVarUp[indexMCStat];
        histMCStatDown = vecOutMT2Hist_3D_SystVarDown[indexMCStat];
    }
    
    if (doVerb) {
        cout << "about to fix the stat error for CV histogram using MCStatUp " << endl;
    }
    SetHistStatErrFromMCStatSyst(histCV, histMCStatUp, doVerb);
    if (doVerb) {
        cout << "about to fix the bin content for MCStatDown using CV histogram error " << endl;
    }
    FixMCStatSystDownHistFromCVStatErr(histCV, histMCStatDown, doVerb);
}
void SignalYieldMaker_IndivPoint::SetYieldInputHist(float intForCut, vector<int> * inVecBins_OutHist, TH1 * histToSet, bool doVerb = 0) {
    int nBinsX = histToSet->GetNbinsX();
    int nBinsY = histToSet->GetNbinsY();
    int nBinsZ = histToSet->GetNbinsZ();
    
    float totIntegral = 0.0;
    
    TString nameClass = histToSet->ClassName();
    if (nameClass.Contains("TH2")) {
        totIntegral = ((TH2 *) histToSet)->Integral(inVecBins_OutHist->at(0), nBinsX + 1, inVecBins_OutHist->at(1), nBinsY + 1);
    }
    else if (nameClass.Contains("TH3")) {
        totIntegral = ((TH3 *) histToSet)->Integral(inVecBins_OutHist->at(0), nBinsX + 1, inVecBins_OutHist->at(1), nBinsY + 1, inVecBins_OutHist->at(2), nBinsZ + 1);
    }
    else {
        totIntegral = histToSet->Integral(inVecBins_OutHist->at(0), nBinsX + 1);
    }
    
    if (doVerb) {
        cout << "going to be grabbing the total integral by integrating from bin " << inVecBins_OutHist->at(0) << " to " << nBinsX + 1 << " in the x direction " << endl;
        if (nameClass.Contains("TH2")) {
            cout << "as well as grabbing the total integral by integrating from bin " << inVecBins_OutHist->at(1) << " to " << nBinsY + 1 << " in the y direction " << endl;
        }
        else if (nameClass.Contains("TH3")) {
            cout << "as well as grabbing the total integral by integrating from bin " << inVecBins_OutHist->at(1) << " to " << nBinsY + 1 << " in the y direction " << endl;
            cout << "as well as grabbing the total integral by integrating from bin " << inVecBins_OutHist->at(2) << " to " << nBinsZ + 1 << " in the z direction " << endl;
        }
    }
    
    float checkVal = 0.0;
    float valToInput = TMath::Max(checkVal, intForCut - totIntegral); //in case the smoothing made the total integral too large
    if (doVerb) {
        cout << "totIntegral " << totIntegral << endl;
        cout << "intForCut " << intForCut << endl;
        cout << "valToInput " << valToInput << endl;
    }
    if (doVerb) {
        cout << "going to set the bin content at binX = " << inVecBins_OutHist->at(0) << endl;
        cout << "going to set the bin content at binY = " << inVecBins_OutHist->at(1) << endl;
        cout << "going to set the bin content at binZ = " << inVecBins_OutHist->at(2) << endl;
    }
    histToSet->SetBinContent(inVecBins_OutHist->at(0), inVecBins_OutHist->at(1), inVecBins_OutHist->at(2), valToInput);
}
void SignalYieldMaker_IndivPoint::SetYieldInputFile(massCombo * inMassPoint, TFile * inCurrFile, TString currBaseHistGrabName, vector<int> * inVecBins_OutHist, bool doVerb = 0) {
    int binX_InHist, binY_InHist;
    int numSysts = vecSystNames.size();
    
    //Current yield for histogram passing cuts
    float currCVIntegral;
    vector<float> currIntegral_SystVarUp;
    currIntegral_SystVarUp.resize(numSysts);
    vector<float> currIntegral_SystVarDown;
    currIntegral_SystVarDown.resize(numSysts);
    
    currInHist = (TH2F *) inCurrFile->Get(currBaseHistGrabName);
    if (doVerb) {
        cout << "in " << inCurrFile->GetName() << endl;
        cout << "currBaseHistGrabName " << currBaseHistGrabName << endl;
    }
    binX_InHist = currInHist->GetXaxis()->FindBin(inMassPoint->first);
    binY_InHist = currInHist->GetYaxis()->FindBin(inMassPoint->second);
    
    
    TH1 * histToSet = 0;
    
    //Grab CV Integral and plug into CV Hist
    if (doVerb) {
        cout << "currInHist name " << currInHist->GetName() << endl;
    }
    currCVIntegral = currInHist->GetBinContent(binX_InHist, binY_InHist);
    if (numDims == 1) {
        histToSet = outMT2Hist_CV_1D;
    }
    else if (numDims == 2) {
        histToSet = outMT2Hist_CV_2D;
    }
    else {
        histToSet = outMT2Hist_CV_3D;
    }
    SetYieldInputHist(currCVIntegral, inVecBins_OutHist, histToSet, doVerb);
    if (doVerb) {
        cout << "currCVIntegral " << currCVIntegral << endl;
    }
    for (unsigned int iSyst = 0; iSyst < currIntegral_SystVarUp.size(); ++iSyst) {
        /*
         if (doVerb) {
         cout << "iSyst " << iSyst << endl;
         cout << "going to try to grab " << currBaseHistGrabName + vecSystNames[iSyst] + TString("Up") << endl;
         cout << "going to try to grab " << currBaseHistGrabName + vecSystNames[iSyst] + TString("Down") << endl;
         }
         */
        vecCurrInHist_SystVarUp[iSyst] = (TH2F *) inCurrFile->Get(currBaseHistGrabName + vecSystNames[iSyst] + TString("Up"));
        currIntegral_SystVarUp[iSyst] = vecCurrInHist_SystVarUp[iSyst]->GetBinContent(binX_InHist, binY_InHist);
        if (numDims == 1) {
            histToSet = vecOutMT2Hist_1D_SystVarUp[iSyst];
        }
        else if (numDims == 2) {
            histToSet = vecOutMT2Hist_2D_SystVarUp[iSyst];
        }
        else {
            histToSet = vecOutMT2Hist_3D_SystVarUp[iSyst];
        }
        SetYieldInputHist(currIntegral_SystVarUp[iSyst], inVecBins_OutHist, histToSet);
        
        vecCurrInHist_SystVarDown[iSyst] = (TH2F *) inCurrFile->Get(currBaseHistGrabName + vecSystNames[iSyst] + TString("Down"));
        currIntegral_SystVarDown[iSyst] = vecCurrInHist_SystVarDown[iSyst]->GetBinContent(binX_InHist, binY_InHist);
        if (numDims == 1) {
            histToSet = vecOutMT2Hist_1D_SystVarDown[iSyst];
        }
        else if (numDims == 2) {
            histToSet = vecOutMT2Hist_2D_SystVarDown[iSyst];
        }
        else {
            histToSet = vecOutMT2Hist_3D_SystVarDown[iSyst];
        }
        SetYieldInputHist(currIntegral_SystVarDown[iSyst], inVecBins_OutHist, histToSet);
    }
}
void SignalYieldMaker_IndivPoint::IterateOverCuts(massCombo * inMassPoint, bool doVerb = 0) {
    TString repFPat, repSPat, repTPat;
    
    vector<unsigned int> vecSizes(0);
    for (unsigned int iAxis = 0; iAxis < vecAxes.size(); ++iAxis) {
        vecSizes.push_back(vecAxes[iAxis].size());
        if (doVerb) {
            cout << "vecSizes[iAxis = " << iAxis << "] is " << vecSizes[iAxis] << endl;
        }
    }
    vector<int> vecBins;
    vecBins.resize(3);
    
    TString currGrabName;
    TFile * currFile;
    
    int subFactor = 1; //how far from edge to cut
    
    for (unsigned int iCutX = 0; iCutX < vecSizes[0] - subFactor; ++iCutX) {
        vecBins[0] = vecSizes[0] - subFactor - (iCutX);
        if (doVerb) {
            cout << "iCutX: " << iCutX << ", vecBins[0] = " << vecBins[0] << endl;
        }
        if (vecSizes.size() > 1) {
            for (unsigned int iCutY = 0; iCutY < vecSizes[1] - subFactor; ++iCutY) {
                vecBins[1] = vecSizes[1] - subFactor - (iCutY);
                if (doVerb) {
                    cout << "iCutY: " << iCutY << ", vecBins[1] = " << vecBins[1] << endl;
                }
                if (vecSizes.size() > 2) {
                    for (unsigned int iCutZ = 0; iCutZ < vecSizes[2] - subFactor; ++iCutZ) {
                        vecBins[2] = vecSizes[2] - subFactor - iCutZ;
                        if (doVerb) {
                            cout << "iCutZ: " << iCutZ << ", vecBins[2] = " << vecBins[2] << endl;
                        }
                        currGrabName = baseNameInFile;
                        repFPat = "";
                        if (doVerb) {
                            cout << "vecAxes[0][vecBins[0] - 1] " << vecAxes[0][vecBins[0] - 1] << endl;
                        }
                        repFPat += ModFive(vecAxes[0][vecBins[0] - 1]);
                        currGrabName.Replace(currGrabName.Index("FPATSY"), 6, repFPat);
                        
                        if (doVerb) {
                            cout << "vecAxes[1][vecBins[1] - 1] " << vecAxes[1][vecBins[1] - 1] << endl;
                        }
                        repSPat = "";
                        repSPat += ModFive(vecAxes[1][vecBins[1] - 1]);
                        currGrabName.Replace(currGrabName.Index("SPATSY"), 6, repSPat);
                        
                        if (doVerb) {
                            cout << "vecAxes[2][vecBins[2] - 1] " << vecAxes[2][vecBins[2] - 1] << endl;
                        }
                        repTPat = "";
                        repTPat += ModFive(vecAxes[2][vecBins[2] - 1]);
                        currGrabName.Replace(currGrabName.Index("TPATSY"), 6, repTPat);
                        
                        currFile = TFile::Open(inBaseDir + currGrabName + TString(".root"));
                        SetYieldInputFile(inMassPoint, currFile, currGrabName, &vecBins, doVerb);
                        currFile->Close();
                    }
                }
                else {
                    currGrabName = baseNameInFile;
                    repFPat = "";
                    if (doVerb) {
                        cout << "vecAxes[0][vecBins[0] - 1] " << vecAxes[0][vecBins[0] - 1] << endl;
                    }
                    repFPat += ModFive(vecAxes[0][vecBins[0] - 1]);
                    currGrabName.Replace(currGrabName.Index("FPATSY"), 6, repFPat);
                    
                    vecBins[2] = 1;
                    
                    if (doVerb) {
                        cout << "vecAxes[1][vecBins[1] - 1] " << vecAxes[1][vecBins[1] - 1] << endl;
                    }
                    repSPat = "";
                    repSPat += ModFive(vecAxes[1][vecBins[1] - 1]);
                    currGrabName.Replace(currGrabName.Index("SPATSY"), 6, repSPat);
                    currFile = TFile::Open(inBaseDir + currGrabName + TString(".root"));
                    SetYieldInputFile(inMassPoint, currFile, currGrabName, &vecBins, doVerb);
                    currFile->Close();
                }
            }
        }
        else {
            vecBins[1] = 1;
            vecBins[2] = 1;
            currGrabName = baseNameInFile;
            repFPat = "";
            if (doVerb) {
                cout << "vecAxes[0][vecBins[0] - 1] " << vecAxes[0][vecBins[0] - 1] << endl;
            }
            repFPat += ModFive(vecAxes[0][vecBins[0] - 1]);
            currGrabName.Replace(currGrabName.Index("FPATSY"), 6, repFPat);
            currFile = TFile::Open(inBaseDir + currGrabName + TString(".root"));
            SetYieldInputFile(inMassPoint, currFile, currGrabName, &vecBins, doVerb);
            currFile->Close();
        }
    }
}
void SignalYieldMaker_IndivPoint::InitializeHistsandOutfile(bool doVerb = 0) {
    outFile = new TFile(outBaseDir + outFileName, "RECREATE");
    if (doVerb) {
        cout << "Base hist name " << outHistBaseName << endl;
        cout << "saving to " << outFile->GetName() << endl;
    }
    if (numDims == 1) {
        outMT2Hist_CV_1D = new TH1F(outHistBaseName, axisString, vecNumBinsAxes[0], &vecAxes[0][0]);
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            vecOutMT2Hist_1D_SystVarUp[iSyst] = new TH1F(outHistBaseName + vecSystNames[iSyst] + TString("Up"), axisString, vecNumBinsAxes[0], &vecAxes[0][0]);
            vecOutMT2Hist_1D_SystVarDown[iSyst] = new TH1F(outHistBaseName + vecSystNames[iSyst] + TString("Down"), axisString, vecNumBinsAxes[0], &vecAxes[0][0]);
        }
    }
    else if (numDims == 2) {
        outMT2Hist_CV_2D = new TH2F(outHistBaseName, axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0]);
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            vecOutMT2Hist_2D_SystVarUp[iSyst] = new TH2F(outHistBaseName + vecSystNames[iSyst] + TString("Up"), axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0]);
            vecOutMT2Hist_2D_SystVarDown[iSyst] = new TH2F(outHistBaseName + vecSystNames[iSyst] + TString("Down"), axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0]);
        }
    }
    else if (numDims == 3) {
        outMT2Hist_CV_3D = new TH3F(outHistBaseName, axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0], vecNumBinsAxes[2], &vecAxes[2][0]);
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            vecOutMT2Hist_3D_SystVarUp[iSyst] = new TH3F(outHistBaseName + vecSystNames[iSyst] + TString("Up"), axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0], vecNumBinsAxes[2], &vecAxes[2][0]);
            vecOutMT2Hist_3D_SystVarDown[iSyst] = new TH3F(outHistBaseName + vecSystNames[iSyst] + TString("Down"), axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0], vecNumBinsAxes[2], &vecAxes[2][0]);
        }
    }
}
void SignalYieldMaker_IndivPoint::WriteFile(bool doSigContam) {
    outFile->cd();
    if (doSigContam) {
        histSig_SigStatControlUp->Write();
        histSig_SigStatControlDown->Write();
        histSig_TTBarStatContUp->Write();
        histSig_TTBarStatContDown->Write();
        histSig_TTBarMCStatUp->Write();
        histSig_TTBarMCStatDown->Write();
    }
    outFile->Write();
    outFile->Close();
}
#endif