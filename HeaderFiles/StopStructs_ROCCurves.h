struct ROCCurveParams {
    int minCut, maxCut, cutStepSize;
    vector<int> vecCutVals;
    
    TString strROCSave, strXAxisSave, strVarCut, strDraw, strCutCommand;
    void SetAsROCCurve(bool isSim, int whichDraw) {
        const int numDraw = 25;
        
        TString arrStrDraw[numDraw];
        
        TString strDilep = "Dilep";
        TString stringJet = "Jet";
        TString stringJet2 = "Jet2BJet";
        TString stringLowBLepMass = "LowBLepMass";
        TString stringZMass = "ZMass";
        TString stringZMassMETSig1D = "ZMassMETSig1D";
        TString stringZMassMETSig2D = "ZMassMETSig2D";
        TString strMT2Tail = "MT2Tail";
        TString strMT2Control = "MT2Control";
        TString strMT2llTail = "MT2llTail";
        TString strMT2llControl = "MT2llControl";
        
        arrStrDraw[0] = strDilep;
        arrStrDraw[1] = strDilep + stringJet;
        arrStrDraw[2] = strDilep + stringJet + stringZMass;
        arrStrDraw[3] = strDilep + stringJet + stringZMass + strMT2llTail;
        arrStrDraw[4] = strDilep + stringJet + stringZMass + strMT2llControl;
        arrStrDraw[5] = strDilep + stringJet2;
        arrStrDraw[6] = strDilep + stringJet2 + stringZMass;
        arrStrDraw[7] = strDilep + stringJet2 + stringZMass + strMT2llTail;
        arrStrDraw[8] = strDilep + stringJet2 + stringZMass + strMT2llControl;
        arrStrDraw[9] = strDilep + stringJet + stringZMass + strMT2Tail;
        arrStrDraw[10] = strDilep + stringJet2 + stringZMass + strMT2Tail;
        arrStrDraw[11] = strDilep + stringJet + strMT2Tail;
        arrStrDraw[12] = strDilep + stringJet2 + strMT2Tail;
	arrStrDraw[13] = strDilep + stringJet + stringZMassMETSig1D;
	arrStrDraw[14] = strDilep + stringJet + stringZMassMETSig1D + strMT2llTail;
	arrStrDraw[15] = strDilep + stringJet + stringZMassMETSig1D + strMT2llControl;
	arrStrDraw[16] = strDilep + stringJet + stringZMassMETSig2D;
	arrStrDraw[17] = strDilep + stringJet + stringZMassMETSig2D + strMT2llTail;
	arrStrDraw[18] = strDilep + stringJet + stringZMassMETSig2D + strMT2llControl;
        arrStrDraw[19] = strDilep + stringJet + stringZMass + stringLowBLepMass;
        arrStrDraw[20] = strDilep + stringJet + stringZMass + stringLowBLepMass + strMT2llTail;
        arrStrDraw[21] = strDilep + stringJet + stringZMass + stringLowBLepMass + strMT2llControl;
        arrStrDraw[22] = strDilep + stringJet2 + stringZMass + stringLowBLepMass;
        arrStrDraw[23] = strDilep + stringJet2 + stringZMass + stringLowBLepMass + strMT2llTail;
        arrStrDraw[24] = strDilep + stringJet2 + stringZMass + stringLowBLepMass + strMT2llControl;
        
        strDraw = arrStrDraw[whichDraw];
        
        TString selDilep = "TDoEvent && ((TPassDoubleMu && TChannel == 0) || (TPassDoubleEl && TChannel == 1) || (TPassElMu && TChannel == 2))";
        TString diLepZMassCut = "(TChannel == 2 || (!TDiLepInZ && ";
        diLepZMassCut += isSim ? "TSmearMET_v2" : "TPFMET";
        diLepZMassCut += " >= 40 && (TChannel == 0 || TChannel == 1)))";
        TString diLepZMassMETSig1DCut = "(TChannel == 2 || (!TDiLepInZ && ";
	diLepZMassMETSig1DCut += isSim ? "TMath::Power(TSmearMET_v2/TMath::Sqrt(THT_AllJets), 2)" : "TMath::Power(TPFMET/TMath::Sqrt(THT_AllJets), 2)";
        diLepZMassMETSig1DCut += " >= 80 && (TChannel == 0 || TChannel == 1)))";
        TString diLepZMassMETSig2DCut = "(TChannel == 2 || (!TDiLepInZ && ";
	diLepZMassMETSig2DCut += isSim ? "TSmearMET_SigTrue_v2" : "TPFMET_SigTrue";
        diLepZMassMETSig2DCut += " >= 160 && (TChannel == 0 || TChannel == 1)))";
        TString jetCut = "TNJets > 1 && TNJetsBtag > 0";
        TString jetCut2 = "TNJets > 1 && TNJetsBtag > 1";
        TString lowBLepMassCut = "TMath::Max(TSmearMassBLep0_BLepsUsed_v2, TSmearMassBLep1_BLepsUsed_v2) < 200";
	if (!isSim) lowBLepMassCut = "TMath::Max(TPFMassBLep0_BLepsUsed, TPFMassBLep1_BLepsUsed) < 200";
        TString cutStringMT2Tail = "(TSmearMT2ll_v2 > 80 || TSmearMT2lblb_v2 > 170 || TSmearMT2bb_ZMET_v2 > 170)";
        TString cutStringMT2llTail = "(TSmearMT2ll_v2 > 80)";
        TString cutStringMT2llControl = "(TSmearMT2ll_v2 < 80)";
        if (!isSim) {
            cutStringMT2Tail = "(TPFMT2ll > 80 || TPFMT2lblb > 170 || TPFMT2bb_ZMET > 170)";
            cutStringMT2llTail = "(TPFMT2ll > 80)";
            cutStringMT2llControl = "(TPFMT2ll < 80)";
        }
        TString andCut = " && ";
        
        
        TString arrStrCut[numDraw];
        arrStrCut[0] = selDilep;
        arrStrCut[1] = selDilep + andCut + jetCut;
        arrStrCut[2] = selDilep + andCut + jetCut + andCut + diLepZMassCut;
        arrStrCut[3] = selDilep + andCut + jetCut + andCut + diLepZMassCut + andCut + cutStringMT2llTail;
        arrStrCut[4] = selDilep + andCut + jetCut + andCut + diLepZMassCut + andCut + cutStringMT2llControl;
        arrStrCut[5] = selDilep + andCut + jetCut2;
        arrStrCut[6] = selDilep + andCut + jetCut2 + andCut + diLepZMassCut;
        arrStrCut[7] = selDilep + andCut + jetCut2 + andCut + diLepZMassCut + andCut + cutStringMT2llTail;
        arrStrCut[8] = selDilep + andCut + jetCut2 + andCut + diLepZMassCut + andCut + cutStringMT2llControl;
        arrStrCut[9] = selDilep + andCut + jetCut + andCut + diLepZMassCut + andCut + cutStringMT2Tail;
        arrStrCut[10] = selDilep + andCut + jetCut2 + andCut + diLepZMassCut + andCut + cutStringMT2Tail;
        
        arrStrCut[11] = selDilep + andCut + jetCut + andCut + cutStringMT2Tail;
        arrStrCut[12] = selDilep + andCut + jetCut2 + andCut + cutStringMT2Tail;
        arrStrCut[13] = selDilep + andCut + jetCut + andCut + diLepZMassMETSig1DCut;
        arrStrCut[14] = selDilep + andCut + jetCut + andCut + diLepZMassMETSig1DCut + andCut + cutStringMT2llTail;
        arrStrCut[15] = selDilep + andCut + jetCut + andCut + diLepZMassMETSig1DCut + andCut + cutStringMT2llControl;
        arrStrCut[16] = selDilep + andCut + jetCut + andCut + diLepZMassMETSig2DCut;
        arrStrCut[17] = selDilep + andCut + jetCut + andCut + diLepZMassMETSig2DCut + andCut + cutStringMT2llTail;
        arrStrCut[18] = selDilep + andCut + jetCut + andCut + diLepZMassMETSig2DCut + andCut + cutStringMT2llControl;
        arrStrCut[19] = selDilep + andCut + jetCut + andCut + diLepZMassCut + andCut + lowBLepMassCut;
        arrStrCut[20] = selDilep + andCut + jetCut + andCut + diLepZMassCut + andCut + lowBLepMassCut + andCut + cutStringMT2llTail;
        arrStrCut[21] = selDilep + andCut + jetCut + andCut + diLepZMassCut + andCut + lowBLepMassCut + andCut + cutStringMT2llControl;
        arrStrCut[22] = selDilep + andCut + jetCut2 + andCut + diLepZMassCut + andCut + lowBLepMassCut;
        arrStrCut[23] = selDilep + andCut + jetCut2 + andCut + diLepZMassCut + andCut + lowBLepMassCut + andCut + cutStringMT2llTail;
        arrStrCut[24] = selDilep + andCut + jetCut2 + andCut + diLepZMassCut + andCut + lowBLepMassCut + andCut + cutStringMT2llControl;

        strCutCommand = arrStrCut[whichDraw];
        
    }
    void SetAsMT2lblbROCCurve(bool isSim, int whichDraw, int whichMT2) {
        const int numMT2 = 4;
        
        SetAsROCCurve(isSim, whichDraw);
        
        TString arrStrVarCut[numMT2] = {"TSmearMT2ll_v2", "TSmearMT2lblb_v2", "TSmearMT2bb_ZMET_v2", "TSmearMT2lblb_ZeroBLepMass_v2"};
        if (!isSim) {
            for (int iMT2 = 0; iMT2 < numMT2; ++iMT2) {
                int index = arrStrVarCut[iMT2].Index("Smear");
                arrStrVarCut[iMT2].Replace(index, 5, "PF");
                index = arrStrVarCut[iMT2].Index("_v2");
                arrStrVarCut[iMT2].Replace(index, 3, "");
            }
        }
        strVarCut = arrStrVarCut[whichMT2] + TString (" >= ");
        
        TString arrStrXAxisSave[numMT2] = {"M_{T2}(ll) Cut [GeV]   ", "M_{T2}(lb)(lb) Cut [GeV]   ", "M_{T2}^{#slash{W}}(bb) Cut [GeV]   ", "M_{T2}^{0lb}(lb)(lb) Cut [GeV]   "};
        strXAxisSave = arrStrXAxisSave[whichMT2];
    }
    void SetAsDYROCCurve(bool isSim, int whichDraw, int whichROC) {
        ///in order of variable
        ///SF MET, SF METSig 1D Hack, SF METSig 2D Hack, SF METSig True, ZMass Window
        int arrMinCut[5] = {0, 0, 0, 0, 0};
        int arrMaxCut[5] = {200, 200, 200, 200, 30};
        int arrCutStepSize[5] = {5, 2, 2, 2, 2};
        
        minCut = arrMinCut[whichROC];
        maxCut = arrMaxCut[whichROC];
        cutStepSize = arrCutStepSize[whichROC];
        
        vecCutVals.resize(0);
        for (int iCut = minCut; iCut <= maxCut; iCut += cutStepSize) {
            vecCutVals.push_back(iCut);
        }
        
        TString arrStrROCSave[5] = {"SFMETCut", "SFMETSig1DCut", "SFMETSig2DCut", "SFMETSig2DFullCut", "SFZMassCut"};
        strROCSave = arrStrROCSave[whichROC];
        
        
        TString arrStrXAxisSave[5] = {"SF #slash{E}_{T} cut", "SF #slash{E}^{2}_{T, 1D}/#sigma^{2}_{#slash{E}_{T, 1D}} cut", "SF #slash{E}^{2}_{T, 2D}/#sigma^{2}_{#slash{E}_{T, 2D}} cut", "SF #slash{E}_{T} significance cut", "SF M_{ll} cut"};
        TString arrStrVarCut[5] = {"TSmearMET_v2", "TMath::Power(TSmearMET_v2/TMath::Sqrt(THT_AllJets), 2)", "TMath::Power(TSmearUPerp_v2/THadResPerpGauss_AllJets,2)+TMath::Power((TSmearUPar_v2+TDiLepPt)/THadResParGauss_AllJets, 2)", "TSmearMET_SigTrue_v2", "TMath::Abs(TDiLepMass - ZMass)"};
        strXAxisSave = arrStrXAxisSave[whichROC];
        
        TString baseCutString = "(TChannel == 2 || ((TChannel == 0 || TChannel == 1) && ";

        strVarCut = baseCutString + arrStrVarCut[whichROC] + TString(" >= ");
        
        SetAsROCCurve(isSim, whichDraw);
    }
    
};




struct ROCDrawingParams {
    vector<int> vecIndices;
    TString canvNameAppend;
    
    vector<Color_t> vecROCCurvesColor;
    vector<Style_t> vecROCCurvesStyle;
    vector<TString> vecLegStrings;
    
    int whichT2SMS, addtlParam;
    TString strLatexSignal;
    

    //Drawing
    void DrawStringsOnCanvas(TCanvas * inputCanv, vPP * deltaM = NULL) {
        strLatexSignal = T2String(whichT2SMS, addtlParam);
        DrawStrings(inputCanv, 0, strLatexSignal, deltaM);
    }
    void DrawOnCanvas(TCanvas * inputCanv, vector<TGraph *> * inputGraphVec, bool isPunzi, vPP * deltaM = NULL) {
        TString canvName = inputCanv->GetName();
        inputCanv->SetName(canvName + canvNameAppend);
        inputCanv->cd();
        inputGraphVec->at(0)->Draw("ACP");
        
        TAxis * yAxis = inputGraphVec->at(0)->GetYaxis();
        
        
        float absMin = 0;
        float absMax = 1.0;
        if (isPunzi) {
            float currMin, currMax;
            absMin = inputGraphVec->at(0)->GetHistogram()->GetMinimum();
            absMax = inputGraphVec->at(0)->GetHistogram()->GetMaximum();
            for (unsigned int iGraph = 1; iGraph < inputGraphVec->size(); ++iGraph) {
                currMin = inputGraphVec->at(iGraph)->GetHistogram()->GetMinimum();
                currMax = inputGraphVec->at(iGraph)->GetHistogram()->GetMaximum();
                absMin = TMath::Min(absMin, currMin);
                absMax = TMath::Max(absMax, currMax);
            }
        }
        else {
            float thresh = 2.5E-3;
            absMin = FirstPointDeltX(inputGraphVec->at(0), thresh);
        }
        
        yAxis->SetTitleOffset(1.0);
        yAxis->SetRangeUser(absMin, absMax);
        
        for (unsigned int iGraph = 1; iGraph < inputGraphVec->size(); ++iGraph) {
            inputGraphVec->at(iGraph)->Draw("PC same");
        }
        
        DrawStringsOnCanvas(inputCanv, deltaM);
        
        inputCanv->Update();
        inputCanv->Modified();
    }
    
    
    
    ///Setting up parameters
    void SetLegendStrings_SigCheck() {
        vecLegStrings.resize(0);
        SetLegendStringsROCCurves_DY(&vecLegStrings);
    }
    void SetLegendStrings_WCheck() {
        vecLegStrings.resize(0);
        SetLegendStringsROCCurves_MT2lblb(&vecLegStrings);
    }
    void SetLegendStrings_dMCheck() {
        vecLegStrings.resize(0);
        
        int numDM = 4;
        vector<vPP> vecDeltaM(numDM);
        for (unsigned int iDM = 0; iDM < numDM; ++iDM) {
            vecDeltaM[iDM].first = DeltaMValue(iDM);
            vecDeltaM[iDM].second = DeltaMPrecision(0);
            vecLegStrings.push_back(DeltaMPlotString(&vecDeltaM[iDM], false));
        }
    }
    
    
    void SetColorsROCCurves() {
        vector<Color_t> vecColors{kBlack, kRed, kBlue, kGreen + 1, kYellow + 2};
        vecROCCurvesColor.resize(0);
        vecROCCurvesColor.insert(vecROCCurvesColor.end(), vecColors.begin(), vecColors.end());
    }
    
    void SetStyleROCCurves() {
        vector<Style_t> vecStyles{2, 20, 21, 22, 29};
        vecROCCurvesColor.resize(0);
        vecROCCurvesStyle.insert(vecROCCurvesStyle.end(), vecStyles.begin(), vecStyles.end());
    }
    
    
    void SetT2Params(TString fileName) {
        if (fileName.Contains("T2")) {
            if (!fileName.Contains("T2bw")) {
                whichT2SMS = 1;
                if (!fileName.Contains("T2tt")) {
                    whichT2SMS = 3;
                }
            }
            else {
                if (fileName.Contains("75")) {
                    addtlParam = 75;
                }
                else if (fileName.Contains("50")) {
                    addtlParam = 75;
                }
                else if (fileName.Contains("25")) {
                    addtlParam = 75;
                }
            }
        }
    }
    void SetCanvName_SigCheck() {
        canvNameAppend += "DYKillerCompare";
    }
    void SetCanvName_dMCheck() {
        canvNameAppend = "DeltaMCheck";
    }
    void SetCanvName_WCheck(int whichCheckSet) {
        TString strSing = TString("SingleVars");
        TString strMinMT2AND = TString("MinMT2AND");
        TString strMinMT2OR = TString("MinMT2OR");
        TString strTriple = TString("Triple");
        TString strMinMaxMass_MassDiff_ANDOR = TString("MinMassDiffMaxMass_ANDOR");
        TString strMinDiffTopMass_MassDiff_ANDOR = TString("MinDiffTopMassMaxMass_ANDOR");
        
        vector<TString> vecCanvNames{strSing, strMinMT2AND, strMinMT2OR, strTriple, strMinMaxMass_MassDiff_ANDOR, strMinDiffTopMass_MassDiff_ANDOR};
        canvNameAppend = vecCanvNames[whichCheckSet];
    }
    void SetIndices_SigCheck() {
        vector<int> vecIndices_Sig{0, 1, 2, 3};
        vecIndices.insert(vecIndices.begin(), vecIndices_Sig.begin(), vecIndices_Sig.end());
    }
    void SetIndices_dMCheck() {
        vector<int> vecIndices_dM{0, 1, 2, 3};
        vecIndices.insert(vecIndices.begin(), vecIndices_dM.begin(), vecIndices_dM.end());
    }
    void SetIndices_WCheck(int whichCheckSet) {
        vecIndices.resize(0);
        vecIndices.push_back(0);
        
        
        vector<int> vecIndices_SinglePlots{1, 2, 11};
        vector<int> vecIndices_MinMT2_ANDPlots{3, 5, 15};
        vector<int> vecIndices_MinMT2_ORPlots{4, 6, 14};
        vector<int> vecIndices_Triple_AND_ORPlots{9, 10};
        vector<int> vecIndices_MinMaxMass_MinMassDiff_AND_OR_Plots{7, 8};
        vector<int> vecIndices_MinAbsTopDiff_MinMassDiff_AND_OR_Plots{12, 13};
        
        vector<int> * vecIndicesToAssign;
        
        if (whichCheckSet == 0) {
            vecIndicesToAssign = &vecIndices_SinglePlots;
        }
        else if (whichCheckSet == 0) {
            vecIndicesToAssign = &vecIndices_MinMT2_ANDPlots;
        }
        else if (whichCheckSet == 0) {
            vecIndicesToAssign = &vecIndices_MinMT2_ORPlots;
        }
        else if (whichCheckSet == 0) {
            vecIndicesToAssign = &vecIndices_Triple_AND_ORPlots;
        }
        else if (whichCheckSet == 0) {
            vecIndicesToAssign = &vecIndices_MinMaxMass_MinMassDiff_AND_OR_Plots;
        }
        else if (whichCheckSet == 0) {
            vecIndicesToAssign = &vecIndices_MinAbsTopDiff_MinMassDiff_AND_OR_Plots;
        }
        vecIndices.insert(vecIndices.end(), vecIndicesToAssign->begin(), vecIndicesToAssign->end());
    }
    
    
};




