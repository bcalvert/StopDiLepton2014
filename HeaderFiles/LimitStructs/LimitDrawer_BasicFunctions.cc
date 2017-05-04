double GetNDCX(double x) {
    gPad->Update();//this is necessary!
    double NDCX = (x - gPad->GetX1())/(gPad->GetX2()-gPad->GetX1());
    return NDCX;
}

double GetNDCY(double y) {
    gPad->Update();//this is necessary!
    cout << "y " << y << endl;
    cout << "gPad->GetY1() " << gPad->GetY1() << endl;
    cout << "gPad->GetY2() " << gPad->GetY2() << endl;
    double NDCY = (y - gPad->GetY1())/(gPad->GetY2()-gPad->GetY1());
    cout << "NDCY " << NDCY << endl;
    return NDCY;
}

TH1F * OneDeeSignifDist(TH2F * inputObsSignif) {
    TH1F * outHist = new TH1F("h_OneDeeSignifDist", ";Observed #it{Local} Significance (#sigma);No. of Mass Points with Obs. Sig.", 60, -2, 4);

    //Set up histogram data
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputObsSignif, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        for (int iBinY = 1; iBinY <= vecInputHistAxisNBins[1]; ++iBinY) {
            if (iBinX < iBinY) continue;
            outHist->Fill(inputObsSignif->GetBinContent(iBinX, iBinY));
        }
    }
    return outHist;
}


bool SplitHistAlongDeltaM(TH2F * inputHist, int hackDeltaM, int whichDir) {
    //Set up histogram data
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    float currMassX, currMassY;
    float currDeltaM;
    float currBinWidth;
    float currBinContent;
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        currMassX = vecInputHistAxes[0]->GetBinCenter(iBinX);
        currBinWidth = vecInputHistAxes[0]->GetBinWidth(iBinX);
        for (int iBinY = 1; iBinY <= vecInputHistAxisNBins[1]; ++iBinY) {
            currMassY = vecInputHistAxes[1]->GetBinCenter(iBinY);
            currDeltaM = currMassX - currMassY;
            currBinContent = inputHist->GetBinContent(iBinX, iBinY);
            if (TMath::Abs(currDeltaM - hackDeltaM) < currBinWidth / 2) {
                if (currBinContent < 1.0 && currBinContent > 1E-6) {
                    cout << "not going to split -- this histogram has real sensitivity in the delta M region of choice!" << endl;
                    cout << "currBinContent at iBinX:iBinY " << iBinX << ":" << iBinY << " is " << currBinContent << endl;
                    cout << "currDeltaM " << currDeltaM << endl;
                    return false;
                }
            }
            if (whichDir > 0) {
                if (currDeltaM > hackDeltaM) continue;
            }
            else if (whichDir < 0) {
                if (currDeltaM < hackDeltaM) continue;
            }
            inputHist->SetBinContent(iBinX, iBinY, 0);
            inputHist->SetBinError(iBinX, iBinY, 0);
        }
    }
    return true;
}

void SetContourVecVec(vector< vector<TGraph *> > * inVecVecContours, vector<TH2F *> * inVecSigStrengthHists, vector<GraphParams> * inVecGPs, int minNumPoints, float desiredExclusion, bool doComplement = false, int hackDeltaMRegion = -1, int startIndexToAdd = 0, bool doVerb = false) {
    unsigned int numHists = inVecSigStrengthHists->size();
    unsigned int currNumContours;
    TH2F * currHist;
    TString currHistName;
    vector<TH2F *> vecOfCurrHist; //used for splitting along diagonals of no sensitivity -- i.e. deltaM = top mass for T2tt

    bool shouldSplit;
    
    for (unsigned int iHist = 0; iHist < numHists; ++iHist) {
        currHist = inVecSigStrengthHists->at(iHist);
        currHistName = currHist->GetName();
        shouldSplit = hackDeltaMRegion > -1;
        if (shouldSplit) {
            TH2F * test = (TH2F *) currHist->Clone(currHistName + "_TestSplit");
            shouldSplit = SplitHistAlongDeltaM(test, hackDeltaMRegion, 1);
        }
        if (shouldSplit) {
            vecOfCurrHist.clear();
            vecOfCurrHist.push_back((TH2F *) currHist->Clone(currHistName + "_ForwardDeltaMSplit"));
            vecOfCurrHist.push_back((TH2F *) currHist->Clone(currHistName + "_BackwardDeltaMSplit"));
            
            SplitHistAlongDeltaM(vecOfCurrHist[0], hackDeltaMRegion, 1);
            SplitHistAlongDeltaM(vecOfCurrHist[1], hackDeltaMRegion, -1);
            ReturnTGraphVector(&inVecVecContours->at(iHist + startIndexToAdd), vecOfCurrHist[0], minNumPoints, desiredExclusion, doComplement, true, doVerb);
            ReturnTGraphVector(&inVecVecContours->at(iHist + startIndexToAdd), vecOfCurrHist[1], minNumPoints, desiredExclusion, doComplement, true, doVerb);
        }
        else {
            ReturnTGraphVector(&inVecVecContours->at(iHist + startIndexToAdd), inVecSigStrengthHists->at(iHist), minNumPoints, desiredExclusion, doComplement, false, doVerb);
        }
        
        currNumContours = inVecVecContours->at(iHist + startIndexToAdd).size();
        if (doVerb) {
            cout << "for iHist = " << iHist + startIndexToAdd << ", I found " << currNumContours << " contours " << endl;
        }
        for (unsigned iContour = 0; iContour < currNumContours; ++iContour) {
            inVecGPs->at(iHist + startIndexToAdd).SetInputTGraph(inVecVecContours->at(iHist + startIndexToAdd)[iContour]);
        }
    }
}

void LimitDrawer::DoInterpolation() {
    TString currHistName;
    vector<TH2F *> vecCurrInterpHist;
    TH2F * currHistToInterpolate;
    string dirToRW = "SW";

    
    currHistToInterpolate = h_XSecLimitMap;
    currHistName = currHistToInterpolate->GetName();
    SetInterpolatedHistVec(currHistToInterpolate, &vecCurrInterpHist, dirToRW, numInterpolate);
    if (numInterpolate > 0) {
        h_XSecLimitMap->Delete();
        h_XSecLimitMap = (TH2F *) vecCurrInterpHist[numInterpolate - 1]->Clone(currHistName);
    }
    
    SetInterpolatedHistVec_InputVec(&vecSigStrengthLimitHists_Obs, dirToRW, numInterpolate);
    SetInterpolatedHistVec_InputVec(&vecSigStrengthLimitHists_Exp, dirToRW, numInterpolate);
    SetInterpolatedHistVec_InputVec(&vecSigStrengthLimitHists_Extra, dirToRW, numInterpolate);
}
void LimitDrawer::SaveCanvas() {
    dirToSaveTo += "/";
    cout << "saving to the dir: " << dirToSaveTo << endl;
    canvDrawParams.canvName.Replace(canvDrawParams.canvName.Index("0."), 2, "0p");
    drawCanv->SaveAs(dirToSaveTo + canvDrawParams.canvName + TString(".pdf"));
}
void LimitDrawer::SaveLimitInfo(){
    
}

void LimitDrawer::SetParamsFromCommandLine(int argc, char * argv[]) {
    for (int iLimitDrawerParams = 0; iLimitDrawerParams < argc; ++iLimitDrawerParams) {
        if (strncmp (argv[iLimitDrawerParams],"doInterp", 8) == 0) {
            runInterpolation = true;
            numInterpolate = strtol(argv[iLimitDrawerParams + 1], NULL, 10);
        }
        else if (strncmp (argv[iLimitDrawerParams],"minNumPts", 9) == 0) {
            minNumPoints = strtol(argv[iLimitDrawerParams + 1], NULL, 10);
        }
        else if (strncmp (argv[iLimitDrawerParams],"saveDir", 7) == 0) {
            dirToSaveTo = TString(argv[iLimitDrawerParams + 1]);
        }
        else if (strncmp (argv[iLimitDrawerParams],"doCoupling", 10) == 0) {
            isBasicExclusion = false;
        }
        else if (strncmp (argv[iLimitDrawerParams],"grabSmooth", 10) == 0) {
            grabSmoothedCurves = true;
        }
        else if (strncmp (argv[iLimitDrawerParams],"tryGlobal", 9) == 0) {
            tryGlobal = true;
            fudgeFactorEuler = strtod(argv[iLimitDrawerParams + 1], NULL);
            InitializeEuler();
        }
    }
    SUSYT2LPs.FullSequence(argc, argv);
    if (runInterpolation) {
        float currBinWidth = 25. / (TMath::Power(2, numInterpolate));
        SUSYT2LPs.SetStrings(currBinWidth);
    }
}
void LimitDrawer::CorrectToGlobalSignificance() {
    CorrectSignificance(h_XSecLimitMap, inputFuncEuler, fudgeFactorEuler);
    
    for (int iObs = 0; iObs < numObs; ++iObs) {
        CorrectSignificance(vecSigStrengthLimitHists_Obs[iObs], inputFuncEuler, fudgeFactorEuler);
    }
    
    for (int iExp = 0; iExp < numExp; ++iExp) {
        CorrectSignificance(vecSigStrengthLimitHists_Exp[iExp], inputFuncEuler, fudgeFactorEuler);
    }
    
    for (int iExtra = 0; iExtra < numExtra; ++iExtra) {
        CorrectSignificance(vecSigStrengthLimitHists_Extra[iExtra], inputFuncEuler, fudgeFactorEuler);
    }
}
void LimitDrawer::InitializeEuler() {
    inputFuncEuler = new TF1("f_Euler", "exp(- (x * x) / 2) * ([0] + x * [1])", 0, 40);
    inputFuncEuler->SetParameter(0, 9.54872e+00);
    inputFuncEuler->SetParameter(1, 3.26214e+00);
    
    inputFuncEuler->SetParError(0, 2.85848e-01);
    inputFuncEuler->SetParError(1, 2.72687e-01);
}
void LimitDrawer::DefaultVals(LimitParametersContainer * inLPC, bool isHack = false) {
    canvDrawParams.DefaultVals();
    
    
    tryGlobal = false;
    fudgeFactorEuler = 0.25;
    dirToSaveTo = "ContourPlots";
    
    isBasicExclusion = true;
    
    runInterpolation = false;
    numInterpolate = 1;
    
    nameSmoothedAddition = "_2DGaussSmoothed_OptKernel";
    grabSmoothedCurves = false;
    
    xyStartVals = pairFloats(11., 0.5);
    xyStartVals_ExtraTop = pairFloats(60., 0.6);
    xyStartVals_ExtraBot = pairFloats(60., 0.1);
    
    minNumPoints = 5;
    desiredExclusion = 1.0;
    
    numObs = 0;
    numExp = 0;
    numExtra = 0;
    
    vecAnaCMSString.clear();
    vecAnaIntLumi.clear();
    
    if (isHack) {
        vecAnaCMSString.push_back("");
        vecAnaIntLumi.push_back(19.5);
    }
    else {
        inLPC->FillCMSVectors(&vecAnaIntLumi, &vecAnaCMSString);
    }
    
    beamEnergy = 8;
    textPrelim = " #it{Preliminary}";
    
    vecAddName.resize(0);
    vecAddName.push_back("");
    vecAddName.push_back("_OneSigUp");
    vecAddName.push_back("_OneSigDown");
    
    //    nameFile = "../ContourMaking/ContourFiles/h_SigStrengthLimit_FullFreq_LepCombo_BaseFullSel_WithSigContam_Shape_ThreeDee_T2bw_CharFrac0.75_SCharSW.root";
    //    nameFile = "../ContourMaking/ContourFiles/h_SigStrengthLimit_FullFreq_DiLep_BaseFullSel_WithSigContam_Shape_ThreeDee_T2bw_CharFrac0.25_SCharSW.root";
    nameFile = "../ContourMaking/ContourFiles/h_SigStrengthLimit_FullFreq_DiLep_BaseFullSel_WithSigContam_Shape_ThreeDee_T2tt_PerPol0.root";
    inFile = TFile::Open(nameFile);
    
    baseHistName = "h_SigStrengthLimitHist_LPS_";
    
    nameXSecMap = baseHistName + "Exp_XSecLimit";
}

void LimitDrawer::SetSignifVals() {
    baseHistName = "h_Signif_LPS_";
    nameXSecMap = baseHistName + "Obs";
}

void LimitDrawer::SetLoadingInformation(TFile * inputFile, TString inputBaseHistName = "", bool isSignif = false) {
    inFile = inputFile;
    if (inputBaseHistName.Length() != 0) {
        baseHistName = inputBaseHistName;
        if (isSignif) {
            nameXSecMap = baseHistName + "Obs_Signif";
        }
        else {
            nameXSecMap = baseHistName + "Exp_XSecLimit";
        }
    }
}

void LimitDrawer::GrabXSecLimitHist(bool doVerb = false) {
    TString grabNameXSecMap = nameXSecMap ;
    grabNameXSecMap += grabSmoothedCurves ? nameSmoothedAddition : "";
    if (doVerb) {
        cout << "currently looking in the file " << inFile->GetName() << endl;
        cout << "looking for the histogram " << grabNameXSecMap << endl;
    }
    h_XSecLimitMap = (TH2F *) inFile->Get(grabNameXSecMap);
}

void LimitDrawer::GrabExtraSignalStrengthHists(bool doVerb = false) {
    if (doVerb) {
        cout << "currently looking in the file " << inFile->GetName() << endl;
    }
    
    for (int iExtra = 0; iExtra < numExtra; ++iExtra) {
        if (doVerb) {
            cout << "for iExtra = " << iExtra << ", I'm looking for the histogram " << vecHistNames_Extra[iExtra] << endl;
        }
        vecSigStrengthLimitHists_Extra[iExtra] = (TH2F *) vecCouplingFiles[iExtra]->Get(vecHistNames_Extra[iExtra]);
    }
}

void LimitDrawer::GrabSignalStrengthHists(bool doVerb = false) {
    if (doVerb) {
        cout << "currently looking in the file " << inFile->GetName() << endl;
    }
    
    
    for (int iObs = 0; iObs < numObs; ++iObs) {
        if (doVerb) {
            cout << "for iObs = " << iObs << ", I'm looking for the histogram " << vecHistNames_Obs[iObs] << endl;
        }
        vecSigStrengthLimitHists_Obs[iObs] = (TH2F *) inFile->Get(vecHistNames_Obs[iObs]);
    }
    
    for (int iExp = 0; iExp < numExp; ++iExp) {
        if (doVerb) {
            cout << "for iExp = " << iExp << ", I'm looking for the histogram " << vecHistNames_Exp[iExp] << endl;
        }
        vecSigStrengthLimitHists_Exp[iExp] = (TH2F *) inFile->Get(vecHistNames_Exp[iExp]);
    }
    
    GrabExtraSignalStrengthHists(doVerb);
    /*
    for (int iExtra = 0; iExtra < numExtra; ++iExtra) {
        if (doVerb) {
            cout << "for iExtra = " << iExtra << ", I'm looking for the histogram " << vecHistNames_Extra[iExtra] << endl;
        }
        vecSigStrengthLimitHists_Extra[iExtra] = (TH2F *) vecCouplingFiles[iExtra]->Get(vecHistNames_Extra[iExtra]);
    }
    */
}

void LimitDrawer::MakeContourVectors(bool doVerb = false, bool isSignif = false) {
    if (isSignif) {
        for (int iCont = 0; iCont < vecVecContours_Exp.size() / 3; ++iCont) {
            cout << "about to make the contours for the observed" << endl;
            SetContourVecVec(&vecVecContours_Obs, &vecSigStrengthLimitHists_Obs, &vecGPs_Obs, minNumPoints, 2.0 + iCont, true, -1, 3 * iCont, doVerb);
            cout << "about to make the contours for the expected " << endl;
            SetContourVecVec(&vecVecContours_Exp, &vecSigStrengthLimitHists_Exp, &vecGPs_Exp, minNumPoints, 2.0 + iCont, true, -1, 3 * iCont, doVerb);
            cout << "about to make the contours for the extra " << endl;
            SetContourVecVec(&vecVecContours_Extra, &vecSigStrengthLimitHists_Extra, &vecGPs_Extra, minNumPoints, 2.0 + iCont, true, -1, 3 * iCont, doVerb);
        }
    }
    else {
        cout << "about to make the contours for the observed" << endl;
        SetContourVecVec(&vecVecContours_Obs, &vecSigStrengthLimitHists_Obs, &vecGPs_Obs, minNumPoints, desiredExclusion, false, SUSYT2LPs.currSDP.hackDeltaMRegion, 0, doVerb);
        cout << "about to make the contours for the expected " << endl;
        SetContourVecVec(&vecVecContours_Exp, &vecSigStrengthLimitHists_Exp, &vecGPs_Exp, minNumPoints, desiredExclusion, false, SUSYT2LPs.currSDP.hackDeltaMRegion, 0, doVerb);
        cout << "about to make the contours for the extra " << endl;
        SetContourVecVec(&vecVecContours_Extra, &vecSigStrengthLimitHists_Extra, &vecGPs_Extra, minNumPoints, desiredExclusion, false, SUSYT2LPs.currSDP.hackDeltaMRegion, 0, doVerb);
    }
}

void LimitDrawer::SetCanvasName(TString inputString) {
    if (tryGlobal) {
        inputString += "_Global";
    }
    if (grabSmoothedCurves) {
        inputString += "_Smoothed";
    }
    canvDrawParams.canvName = "c_" + inputString;
    canvDrawParams.canvTitle = "c_" + inputString;
}

void LimitDrawer::FullSetupSequence_BasicSignif(bool alreadyHaveHists, bool doVerb = false) {
    if (!alreadyHaveHists) {
        GrabXSecLimitHist(doVerb);
    }
    canvDrawParams.SetAsLimitCanvas(SUSYT2LPs.currSDP.labelXaxis, SUSYT2LPs.currSDP.labelYaxis, "", true);
    //canvDrawParams.SetEmptyHistogram(h_XSecLimitMap);
    InitializeCanvas();
    SetBasicParamsSignif();
    SetVectorSizesSignif();
    DefaultSigHistNames();
    SetSignifGraphParams();
    SetText_All(doVerb, true);
    
    ///Grab histograms
    if (!alreadyHaveHists) {
        GrabSignalStrengthHists(doVerb);
    }
    if (tryGlobal) {
        CorrectToGlobalSignificance();
    }
    MakeContourVectors(doVerb, true);
    if (runInterpolation) {
        DoInterpolation();
    }
}

void LimitDrawer::FullSetupSequence_BasicExclusion(bool alreadyHaveHists, bool doVerb = false) {
    if (!alreadyHaveHists) {
        GrabXSecLimitHist(doVerb);
    }
    canvDrawParams.SetAsLimitCanvas(SUSYT2LPs.currSDP.labelXaxis, SUSYT2LPs.currSDP.labelYaxis, "", false);
    //canvDrawParams.SetEmptyHistogram(h_XSecLimitMap);
    InitializeCanvas();
    SetBasicParams();
    SetVectorSizes();
    DefaultSigHistNames();
    SetGraphParams();
    SetText_All(doVerb, false);
    
    ///Grab histograms
    if (!alreadyHaveHists) {
        GrabSignalStrengthHists(doVerb);
    }
    MakeContourVectors(doVerb);
    if (runInterpolation) {
        DoInterpolation();
    }
}

void LimitDrawer::FullSetupSequence_Coupling_Hack(bool doVerb = false) {
    canvDrawParams.SetAsLimitCanvas(SUSYT2LPs.currSDP.labelXaxis, SUSYT2LPs.currSDP.labelYaxis, "", false);
    //canvDrawParams.SetEmptyHistogram(h_XSecLimitMap);
    InitializeCanvas();
    SetGraphParams();
    SetText_All(doVerb, false);
    
    ///Grab histograms
    GrabExtraSignalStrengthHists(doVerb);
    MakeContourVectors(doVerb);
    if (runInterpolation) {
        DoInterpolation();
    }
}

void LimitDrawer::FullSetupSequence_Coupling(vector<SUSYT2LimitParams> * inVecSUSYT2LP, vector<TFile *> * inVecTFile, bool doVerb = false) {
    GrabXSecLimitHist(doVerb);
    canvDrawParams.SetAsLimitCanvas(SUSYT2LPs.currSDP.labelXaxis, SUSYT2LPs.currSDP.labelYaxis, "", false);
    //canvDrawParams.SetEmptyHistogram(h_XSecLimitMap);
    InitializeCanvas();
    SetCouplingParams(inVecSUSYT2LP, inVecTFile);
    SetVectorSizes();
    DefaultSigHistNames();
    SetGraphParams();
    SetText_All(doVerb, false);
    
    ///Grab histograms
    GrabSignalStrengthHists(doVerb);
    MakeContourVectors(doVerb);
    if (runInterpolation) {
        DoInterpolation();
    }
}

void LimitDrawer::FullDrawSequence(bool doVerb = false, bool isSignif = false) {
    if (doVerb) {
        cout << "drawing the empty histogram" << endl;
    }
    //canvDrawParams.DrawEmptyHisto(drawCanv);
    if (doVerb) {
        cout << "drawing the cross-section map " << endl;
    }
    SetHistoParams(isSignif);
    //h_XSecLimitMap->Draw("colz");
    DrawContours(doVerb);
    DrawText_All(doVerb, isSignif);
    
    if (isSignif) {
        TCanvas * c1 = new TCanvas("balh", "balh");
        h_SigDist->Draw("e1");
    }
}


void LimitDrawer::InitializeCanvas() {
    drawCanv = new TCanvas(canvDrawParams.canvName, canvDrawParams.canvTitle, canvDrawParams.canvWidth, canvDrawParams.canvHeight);
    canvDrawParams.SetCanvas(drawCanv);
}

void LimitDrawer::SetText_PaletteLabel(bool isSignif = false) {
    TString strPaletteLabel = "95% CL upper limit on cross section (pb)";
    if (isSignif) {
        strPaletteLabel = "Observed #it{Local} Significance (#sigma)";
        if (tryGlobal) {
            strPaletteLabel = "Observed Est. #it{Global} Significance (#sigma)";
        }
    }
    
    float sizePaletteLabel = 0.045;
    int alignPaletteLabel = 12;
    float posX_PaletteLabel = 0.98;
    float posY_PaletteLabel = 0.15;
    float anglePaletteLabel = 90;
    //float anglePaletteLabel = 0;
    int fontPaletteLabel = 42;
    
    latexPaletteLabel.SetLatex(true, sizePaletteLabel, alignPaletteLabel, posX_PaletteLabel, posY_PaletteLabel, strPaletteLabel);
    latexPaletteLabel.straightGhetto.textFont = fontPaletteLabel;
    latexPaletteLabel.straightGhetto.textAngle = anglePaletteLabel;
}

void LimitDrawer::SetText_Ana(LimitParametersContainer * inLPC, bool doVerb = false) {
    
    vector<TString> vecStrLatexAnalysis = inLPC->GetLatexAnaString();
    
    float sizeAna = 0.038;
    int alignAna = 13;
    //    float posX_Ana = 0.22;
    float posX_Ana = 0.554;

    if (!isBasicExclusion) {
        posX_Ana = 0.2366;
    }
    /*
    if (!isBasicExclusion) {
        float minX = SUSYT2LPs.currSDP.bounds_SUSYMasses.xLB;
        float xRange = SUSYT2LPs.currSDP.bounds_SUSYMasses.GetXRange();
        posX_Ana = minX + xyStartVals.first * xRange / 100;
    }
    */
    
    float posY_Ana = 0.76;
    float stepsizeY_Ana = 0.05;
    int fontAna = 42;
    
    vecLatexAna.resize(vecStrLatexAnalysis.size());
    for (unsigned int iAna = 0; iAna < vecStrLatexAnalysis.size(); ++iAna) {
        vecLatexAna[iAna].SetLatex(true, sizeAna, alignAna, posX_Ana, posY_Ana - iAna * stepsizeY_Ana, vecStrLatexAnalysis[iAna]);
        vecLatexAna[iAna].straightGhetto.textFont = fontAna;
    }
}

void LimitDrawer::SetText_CMS(bool doVerb = false) {
    char buf[300];
    
    TString strCMS = "#bf{CMS}";
    strCMS += textPrelim;

    TString preStringAna = ",";
    for (unsigned int iAna = 0; iAna < vecAnaIntLumi.size(); ++iAna) {
        if (iAna > 0) {
            preStringAna = " +";
        }
        sprintf(buf,"%s %0.1f fb^{-1} %s", preStringAna.Data(), vecAnaIntLumi[iAna], vecAnaCMSString[iAna].Data());
        strCMS += TString(string(buf));
    }
    sprintf(buf, ", #sqrt{s} = %0.0f TeV", beamEnergy);
    strCMS += TString(string(buf));
    
    
    if (doVerb) {
        cout << "strCMS " << strCMS << endl;
    }
    float sizeCMS = 0.038;
    int alignCMS = 13;
//    float posX_CMS = 0.22;
    float posX_CMS = 0.1585;
    float posY_CMS = 0.98;
    int fontCMS = 42;
    
    latexCMS.SetLatex(true, sizeCMS, alignCMS, posX_CMS, posY_CMS, strCMS);
    latexCMS.straightGhetto.textFont = fontCMS;
}

void LimitDrawer::SetText_LegHack(bool doVerb = false, bool isSignif = false) {
    float minX = SUSYT2LPs.currSDP.bounds_SUSYMasses.xLB;
    //float maxY = SUSYT2LPs.currSDP.bounds_SUSYMasses.yUB;
    
    float xRange = SUSYT2LPs.currSDP.bounds_SUSYMasses.GetXRange();
    //float yRange = SUSYT2LPs.currSDP.bounds_SUSYMasses.GetYRange();
    
    double x, yMax, yMax75;
    graph_WhiteBackground.GetPoint(1, x, yMax);
    graph_WhiteBackground.GetPoint(2, x, yMax75);
    
    //float Ymax = SUSYT2LPs.currSDP.bounds_SUSYMasses.yUB + (SUSYT2LPs.currSDP.binWidth / 2);
    
    
    float sizeLegendHack = 0.038;
    int alignLegendHack = 13;
    int fontLegendHack = 42;
    
    float posX_LegendHack = minX + xyStartVals.first * xRange / 100;
    
//    int numHacks = isSignif ? 3 : 2;
    int numHacks = isSignif ? 2 : 2;
    vecLatexLegendHack.resize(numHacks);
    vector<float> vecPosY_LegendHack(numHacks);
    vector<TString> vecStr_LegendHack(numHacks);

    if (isSignif) {
        for (int iHack = 0; iHack < numHacks; ++iHack) {
            TString baseString = "";
            baseString += (iHack + 1) * 2;
            baseString += "#sigma 0.5 (#pm0.34) Exp. Sig.";
            vecStr_LegendHack[iHack] =  baseString;
            vecPosY_LegendHack[iHack] = yMax75 + (xyStartVals.second - (iHack * 0.25)) * (yMax - yMax75);
        }
    }
    else {
        vecStr_LegendHack[0] = "Observed #pm 1 #sigma_{theory}";
        vecPosY_LegendHack[0] = yMax75 + xyStartVals.second * (yMax - yMax75);
        vecStr_LegendHack[1] = "Expected #pm 1 #sigma_{experiment}";
        vecPosY_LegendHack[1] = yMax75 + (xyStartVals.second - 0.25) * (yMax - yMax75);
    }
    
    if (!isBasicExclusion) {
        numHacks = 1;
        vecLatexLegendHack.resize(numHacks);
    }
    
    /*
    if (doVerb) {
        cout << "strObsHack " << strObsHack << endl;
        cout << "strExpHack " << strExpHack << endl;
    }
     */
    for (int iHack = 0; iHack < numHacks; ++iHack) {
        vecLatexLegendHack[iHack].SetLatex(false, sizeLegendHack, alignLegendHack, posX_LegendHack, vecPosY_LegendHack[iHack], vecStr_LegendHack[iHack]);
        vecLatexLegendHack[iHack].straightGhetto.textFont = fontLegendHack;
    }
    
    if (numExtra > 0) {
        if (numExtra > 2) {
            sizeLegendHack = 0.025;
            xyStartVals_ExtraTop = pairFloats(75., 0.9);
            xyStartVals_ExtraBot = pairFloats(75., 0.05);
        }
        float posX_ExtraLegendHack = minX + xyStartVals_ExtraTop.first * xRange / 100;
        float minY_ExtraLegendHack = yMax75 + xyStartVals_ExtraBot.second * (yMax - yMax75);
        float maxY_ExtraLegendHack = yMax75 + xyStartVals_ExtraTop.second * (yMax - yMax75);
        LatexString currExtraLS;
        float currY_ExtraLegendHack = 0.0;
        for (int iExtra = 0; iExtra < numExtra; ++iExtra) {
            currY_ExtraLegendHack = minY_ExtraLegendHack * (1 + ((iExtra + 1) * 1.0/numExtra) * (maxY_ExtraLegendHack/minY_ExtraLegendHack - 1));
            cout << "currY_ExtraLegendHack " << currY_ExtraLegendHack << endl;
            currExtraLS.SetLatex(false, sizeLegendHack, alignLegendHack, posX_ExtraLegendHack, currY_ExtraLegendHack, vecStr_ExtraLegendHack[iExtra]);
            currExtraLS.straightGhetto.textFont = fontLegendHack;
            vecLatexLegendHack.push_back(currExtraLS);
        }
    }
}
/*
 void LimitDrawer::SetText_Model(bool doVerb = false) {
 char * buf[300];
 
 TString labelModel = "T2tt";
 
 TString strModel = labelModel + TString(" #scale[0.7]{ }");
 if (doVerb) {
 cout << "strModel " << strModel << endl;
 }
 
 float sizeModel = 0.040;
 int alignModel = 13;
 float posX_Model = 0.16;
 float posY_Model = 0.90;
 int fontModel = 42;
 
 latexModel.SetLatex(true, sizeModel, alignModel, posX_Model, posY_Model, strModel);
 latexModel.straightGhetto.textFont = fontModel;
 }
 */

void LimitDrawer::SetText_WhiteBackground(bool doVerb = false) {
    float Xmin = SUSYT2LPs.currSDP.bounds_SUSYMasses.xLB - (SUSYT2LPs.currSDP.binWidth / 2);
    float Xmax = SUSYT2LPs.currSDP.bounds_SUSYMasses.xUB + (SUSYT2LPs.currSDP.binWidth / 2);
    
    //float Ymin = SUSYT2LPs.currSDP.bounds_SUSYMasses.yLB - (SUSYT2LPs.currSDP.binWidth / 2);
    float Ymax = SUSYT2LPs.currSDP.bounds_SUSYMasses.yUB + (SUSYT2LPs.currSDP.binWidth / 2);
//    float Ymax = SUSYT2LPs.currSDP.bounds_SUSYMasses.yUB;
    
    //    cout << "Ymax " << Ymax << endl;
    
    float yMax75 = (Ymax - (SUSYT2LPs.currSDP.binWidth / 2)) * 0.75;
    cout << "yMax75 " << yMax75 << endl;
    
    params_WhiteBackgroundGraph.SetAsWhiteBackgroundGraphParams();
    graph_WhiteBackground.Set(5);
    
    params_WhiteBackgroundGraph.SetInputTGraph(&graph_WhiteBackground);
    graph_WhiteBackground.SetPoint(0, Xmin, Ymax);
    graph_WhiteBackground.SetPoint(1, Xmax, Ymax);
    graph_WhiteBackground.SetPoint(2, Xmax, yMax75);
    graph_WhiteBackground.SetPoint(3, Xmin, yMax75);
    graph_WhiteBackground.SetPoint(4, Xmin, Ymax);
}

void LimitDrawer::SetText_All(bool doVerb = false, bool isSignif = false) {
    SetText_WhiteBackground(doVerb);
    SetText_CMS(doVerb);
    //SetText_Model(doVerb);
    SetText_PaletteLabel(isSignif);
    SetText_LegHack(doVerb, isSignif);
}

void LimitDrawer::SetCouplingParams(vector<SUSYT2LimitParams> * inVecSUSYT2LP, vector<TFile *> * inVecTFile) {
    numObs = 3;
    numExp = 0;
    numExtra = inVecSUSYT2LP->size();
    vecCouplingFiles.clear();
    vecStr_ExtraLegendHack.clear();
    for (int iExtra = 0; iExtra < numExtra; ++iExtra) {
        vecCouplingFiles.push_back(inVecTFile->at(iExtra));
        vecStr_ExtraLegendHack.push_back(inVecSUSYT2LP->at(iExtra).currSDP.strLegend);
    }
}

void LimitDrawer::SetBasicParams() {
    numObs = 3;
    numExp = 3;
    numExtra = 0;
}

void LimitDrawer::SetBasicParamsSignif() {
    numObs = 0;
    numExp = 3;
    numExtra = 0;
}


void LimitDrawer::SetHistoParams(bool isSignif = false) {
    h_XSecLimitMap->GetXaxis()->SetTitle(SUSYT2LPs.currSDP.labelXaxis);
    h_XSecLimitMap->GetXaxis()->SetNdivisions(505);
    
    h_XSecLimitMap->GetYaxis()->SetTitle(SUSYT2LPs.currSDP.labelYaxis);
    h_XSecLimitMap->GetYaxis()->SetNdivisions(505);
//    h_XSecLimitMap->GetYaxis()->SetRangeUser(SUSYT2LPs.currSDP.bounds_SUSYMasses.yLB, SUSYT2LPs.currSDP.bounds_SUSYMasses.yUB);
    h_XSecLimitMap->GetYaxis()->SetRangeUser(0, SUSYT2LPs.currSDP.bounds_SUSYMasses.yUB);
    
    h_XSecLimitMap->GetZaxis()->SetLabelFont(42);
    h_XSecLimitMap->GetZaxis()->SetTitleFont(42);
    h_XSecLimitMap->GetZaxis()->SetLabelSize(0.035);
    h_XSecLimitMap->GetZaxis()->SetTitleSize(0.035);
    if (isSignif) {
        maxSigma = h_XSecLimitMap->GetMaximum();
        cout << "###########################" << endl;
        cout << "###########################" << endl;
        cout << "max signif. " << maxSigma << endl;
        cout << "###########################" << endl;
        cout << "###########################" << endl;
        h_SigDist = OneDeeSignifDist(h_XSecLimitMap);
//        h_XSecLimitMap->GetZaxis()->SetRangeUser(-1,3.5);
        h_XSecLimitMap->GetZaxis()->SetRangeUser(-1,4.0);
    }
    else {
        h_XSecLimitMap->GetZaxis()->SetRangeUser(10e-4,100);
    }
    
    int NRGBs = 5;
    int NCont = 255;
    
    double arrayStops[5] = {0.00, 0.34, 0.61, 0.84, 1.00};
    double arrayReds[5] = {0.50, 0.50, 1.00, 1.00, 1.00};
    double arrayGreens[5] = {0.50, 1.00, 1.00, 0.60, 0.50};
    double arrayBlues[5] = {1.00, 1.00, 0.50, 0.40, 0.50};
    
    //int colorTable =
    TColor::CreateGradientColorTable(NRGBs, arrayStops, arrayReds, arrayGreens, arrayBlues, NCont);
    gStyle->SetNumberContours(NCont);
    
    drawCanv->cd();
    h_XSecLimitMap->Draw("colz");
    
    gPad->Update();
    TPaletteAxis * palette = (TPaletteAxis*)(h_XSecLimitMap->GetListOfFunctions()->FindObject("palette"));
    palette->SetX1NDC(1.-0.18);
    palette->SetY1NDC(0.14);
    palette->SetX2NDC(1.-0.13);
    palette->SetY2NDC(1.-0.08);
    palette->SetLabelFont(42);
    palette->SetLabelSize(0.035);
}



void LimitDrawer::DrawText_WhiteBackground() {
    //cout << "drawing the white background graph " << endl;
    drawCanv->cd();
    graph_WhiteBackground.Draw("FSAME");
    graph_WhiteBackground.Draw("LSAME");
    gPad->Update();
    gPad->Modified();
}

void LimitDrawer::DrawText_Ana(bool doVerb = false) {
    if (doVerb) {
        cout << "drawing the Analysis Latex text " << endl;
    }
    for (unsigned int iAna = 0; iAna < vecLatexAna.size(); ++iAna) {
        vecLatexAna[iAna].DrawCanvas(drawCanv, 1, doVerb);
    }
    gPad->Update();
    gPad->Modified();
}

void LimitDrawer::DrawText_CMS(bool doVerb = false) {
    if (doVerb) {
        cout << "drawing the CMS Latex text " << endl;
    }
    latexCMS.DrawCanvas(drawCanv, 1, doVerb);
    gPad->Update();
    gPad->Modified();
}
/*
 void LimitDrawer::DrawText_Model(bool doVerb = false) {
 cout << "drawing the SUSY Model Latex text " << endl;
 
 latexModel.DrawCanvas(drawCanv, 1, doVerb);
 }
 */

void LimitDrawer::DrawText_PaletteLabel(bool doVerb = false) {
    if (doVerb) {
        cout << "drawing the Pallete Label Latex text " << endl;
    }
    latexPaletteLabel.DrawCanvas(drawCanv, 1, doVerb);
    gPad->Update();
    gPad->Modified();
}

void LimitDrawer::DrawText_All(bool doVerb = false, bool isSignif = false) {
    if (doVerb) {
        cout << "drawing all the text " << endl;
        
        cout << "redrawing the canvas axis" << endl;
    }
    drawCanv->RedrawAxis();
    DrawText_WhiteBackground();
    DrawText_CMS(doVerb);
    DrawText_Ana(doVerb);
    SUSYT2LPs.DrawOnCanvas(drawCanv, true, doVerb);
    //DrawText_Model(doVerb);
    DrawText_PaletteLabel(doVerb);
    DrawLegend_BasicExclusion(doVerb, isSignif);
    
    
    if (isSignif) {
        int x = h_XSecLimitMap->FindFirstBinAbove(0.98 * maxSigma, 1);
        int y = h_XSecLimitMap->FindFirstBinAbove(0.98 * maxSigma, 2);
        int maxStop = (int) (0.5 + h_XSecLimitMap->GetXaxis()->GetBinCenter(x));
        int maxLSP = (0.5 + h_XSecLimitMap->GetYaxis()->GetBinCenter(y));

        char buf[100];
        sprintf(buf, "Max. Obs. #sigma = %0.2f", maxSigma);
        
        TString strSigma = TString(buf);
        
        float xPos = 0.168;
        
        TString massString = "(m_{#tilde{t}_{1}}, m_{#tilde{#chi}^{0}_{1}}) = ";
        massString += "(";
        massString += maxStop;
        massString += ":";
        massString += maxLSP;
        massString += ")";
        TLatex latexMaxSigma;
        latexMaxSigma.SetNDC();
        latexMaxSigma.SetTextSize(0.028);
        latexMaxSigma.DrawLatex(xPos, 0.676, strSigma);
        
        TLatex latexMassString = latexMaxSigma;
        latexMassString.SetNDC();
        latexMassString.DrawLatex(xPos, 0.64, massString);
        /*
        TString massString = "(m_{#tilde{t}_{1}}, m_{#tilde{chi}^{0}_{1}}) [GeV]";
        TString massString2 = "(";
        massString2 += maxStop;
        massString2 += ":";
        massString2 += maxLSP;
        massString2 += ")";
        TLatex latexMaxSigma;
        latexMaxSigma.SetNDC();
        latexMaxSigma.SetTextSize(0.028);
        latexMaxSigma.DrawLatex(0.172, 0.676, strSigma);
        
        TLatex latexMassString = latexMaxSigma;
        latexMassString.DrawLatex(0.172, 0.606, massString);
        latexMassString.DrawLatex(0.172, 0.536, massString2);
         */
    }
}

void LimitDrawer::DrawContours(bool doVerb = false) {
    if (doVerb) {
        cout << "about to draw the contours " << endl;
    }
    drawCanv->cd();
    
    unsigned int currNumContours;
    for (unsigned int iExp = 0; iExp < vecVecContours_Exp.size(); ++iExp) {
        currNumContours = vecVecContours_Exp[iExp].size();
        if (doVerb) {
            cout << "for iExp = " << iExp << ", I found " << currNumContours << " contours " << endl;
        }
        for (unsigned iContour = 0; iContour < currNumContours; ++iContour) {
            vecVecContours_Exp[iExp][iContour]->Draw("L same");
        }
    }
    for (unsigned int iExtra = 0; iExtra < vecVecContours_Extra.size(); ++iExtra) {
        currNumContours = vecVecContours_Extra[iExtra].size();
        if (doVerb) {
            cout << "for iExtra = " << iExtra << ", I found " << currNumContours << " contours " << endl;
        }
        for (unsigned iContour = 0; iContour < currNumContours; ++iContour) {
            vecVecContours_Extra[iExtra][iContour]->Draw("L same");
        }
    }
    for (unsigned int iObs = 0; iObs < vecVecContours_Obs.size(); ++iObs) {
        currNumContours = vecVecContours_Obs[iObs].size();
        if (doVerb) {
            cout << "for iObs = " << iObs << ", I found " << currNumContours << " contours " << endl;
        }
        for (unsigned iContour = 0; iContour < currNumContours; ++iContour) {
            vecVecContours_Obs[iObs][iContour]->Draw("L same");
        }
    }
}

void LimitDrawer::DrawLegend_BasicExclusion(bool doVerb = false, bool isSignif = false) {
    if (doVerb) {
        cout << "going to draw the Legend hack " << endl;
    }
    float minX = SUSYT2LPs.currSDP.bounds_SUSYMasses.xLB;
    //float maxY = SUSYT2LPs.currSDP.bounds_SUSYMasses.yUB;
    
    float xRange = SUSYT2LPs.currSDP.bounds_SUSYMasses.GetXRange();
    //float yRange = SUSYT2LPs.currSDP.bounds_SUSYMasses.GetYRange();
    
    pairFloats xPair(minX, xRange);

    
    double x, yMax, yMax75;
    graph_WhiteBackground.GetPoint(1, x, yMax);
    graph_WhiteBackground.GetPoint(2, x, yMax75);
    
    pairFloats yPair(yMax75, yMax);
    pairFloats yPairTry2(h_XSecLimitMap->GetYaxis()->GetXmin(), yMax);
    

    pairFloats xyStartVals_Obs = xyStartVals;
    for (int iObs = 0; iObs < numObs; ++iObs) {
        vecGPs_Obs[iObs].SetInputTGraphForLegendHack(&vecLegendHackGraphs_Obs[iObs], &xyStartVals_Obs, &xPair, &yPair, true, iObs, doVerb);
    }
    
    pairFloats xyStartVals_Exp = xyStartVals;
    if (numObs > 0) {
        xyStartVals_Exp.second -= 0.25;
    }
    
    for (int iExp = 0; iExp < numExp; ++iExp) {
        vecGPs_Exp[iExp].SetInputTGraphForLegendHack(&vecLegendHackGraphs_Exp[iExp], &xyStartVals_Exp, &xPair, &yPair, false, iExp, doVerb);
    }
    if (isSignif) {
        cout << "vecLegendHackGraphs_Exp.size() " << vecLegendHackGraphs_Exp.size() << endl;
        for (unsigned int iExp = numExp; iExp < vecLegendHackGraphs_Exp.size(); ++iExp) {
            if (iExp % 3 == 0) {
                xyStartVals_Exp.second -= 0.25;
            }
            vecGPs_Exp[iExp].SetInputTGraphForLegendHack(&vecLegendHackGraphs_Exp[iExp], &xyStartVals_Exp, &xPair, &yPair, false, iExp % 3, doVerb);
        }
    }
    
    
    pairFloats xyStartVals_ExtraToUse;
    
    float minY_ExtraLegendHack = yMax75 + xyStartVals_ExtraBot.second * (yMax - yMax75);
    float maxY_ExtraLegendHack = yMax75 + xyStartVals_ExtraTop.second * (yMax - yMax75);

    for (int iExtra = 0; iExtra < numExtra; ++iExtra) {
        xyStartVals_ExtraToUse = xyStartVals_ExtraTop;
        float currY_ExtraLegendHack = minY_ExtraLegendHack * (1 + ((iExtra + 1) * 1.0/numExtra) * (maxY_ExtraLegendHack/minY_ExtraLegendHack - 1));
        /*
         float currY_ExtraLegendHack = minY_ExtraLegendHack * (1 + ((iExtra + 1) * 1.0/numExtra) * (maxY_ExtraLegendHack/minY_ExtraLegendHack - 1));
         xyStartVals_ExtraToUse.second = GetNDCY(currY_ExtraLegendHack);
         */
        //xyStartVals_ExtraToUse.second = xyStartVals_ExtraBot.second * (1 + (1 + ((iExtra + 1) * 1.0/numExtra) * (xyStartVals_ExtraTop.second/xyStartVals_ExtraBot.second - 1)));
        if (numExtra > 2) {
            xyStartVals_ExtraToUse.second = xyStartVals_ExtraBot.second * (currY_ExtraLegendHack/maxY_ExtraLegendHack) * (1 + (1 + ((iExtra + 1) * 1.0/numExtra) * (xyStartVals_ExtraTop.second/xyStartVals_ExtraBot.second - 1)));
        }
        else {
            xyStartVals_ExtraToUse.second = xyStartVals_ExtraBot.second * (0.8) * (1 + (1 + ((iExtra + 1) * 1.0/numExtra) * (xyStartVals_ExtraTop.second/xyStartVals_ExtraBot.second - 1)));
        }
//        xyStartVals_ExtraToUse.second = xyStartVals_ExtraBot.second * (yMax75/yMax) * (1 + (1 + ((iExtra + 1) * 1.0/numExtra) * (xyStartVals_ExtraTop.second/xyStartVals_ExtraBot.second - 1)));
//        xyStartVals_ExtraToUse.second = GetNDCY(minY_ExtraLegendHack) * (1 + (1 + ((iExtra + 1) * 1.0/numExtra) * (xyStartVals_ExtraTop.second/xyStartVals_ExtraBot.second - 1)));
//        xyStartVals_ExtraToUse.second = minY_ExtraLegendHack * (1 + (1 + ((iExtra + 1) * 1.0/numExtra) * (xyStartVals_ExtraTop.second/xyStartVals_ExtraBot.second - 1)));
        cout << "iExtra " << iExtra << endl;
        cout << "xyStartVals_ExtraToUse.second " << xyStartVals_ExtraToUse.second << endl;
//        vecGPs_Extra[iExtra].SetInputTGraphForLegendHack(&vecLegendHackGraphs_Extra[iExtra], &xyStartVals_ExtraToUse, &xPair, &yPair, true, iExtra, doVerb);
//        vecGPs_Extra[iExtra].SetInputTGraphForLegendHack(&vecLegendHackGraphs_Extra[iExtra], &xyStartVals_ExtraToUse, &xPair, &yPair, true, iExtra, doVerb);
        vecGPs_Extra[iExtra].SetInputTGraphForLegendHack(&vecLegendHackGraphs_Extra[iExtra], &xyStartVals_ExtraToUse, &xPair, &yPair, true, 1, doVerb);
    }
    
    
    
    for (unsigned int iHack = 0; iHack < vecLatexLegendHack.size(); ++iHack) {
        vecLatexLegendHack[iHack].DrawCanvas(drawCanv, 1, doVerb);
    }
    
    drawCanv->cd();
    for (int iObs = 0; iObs < numObs; ++iObs) {
        if (doVerb) {
            cout << "about to draw hack for iObs " << iObs << endl;
            cout << "vecLegendHackGraphs_Obs name " << vecLegendHackGraphs_Obs[iObs].GetName() << endl;
        }
        vecLegendHackGraphs_Obs[iObs].Draw("L same");
        /*
        gPad->Update();
        gPad->Modified();
         */
    }
    
    for (int iExp = 0; iExp < numExp; ++iExp) {
        if (doVerb) {
            cout << "about to draw hack for iExp " << iExp << endl;
            cout << "vecLegendHackGraphs_Exp name " << vecLegendHackGraphs_Exp[iExp].GetName() << endl;
        }
        vecLegendHackGraphs_Exp[iExp].Draw("L same");
        /*
         gPad->Update();
         gPad->Modified();
         */
    }
    if (isSignif) {
        for (unsigned int iExp = numExp; iExp < vecLegendHackGraphs_Exp.size(); ++iExp) {
            if (doVerb) {
                cout << "about to draw hack for iExp " << iExp << endl;
                cout << "vecLegendHackGraphs_Exp name " << vecLegendHackGraphs_Exp[iExp].GetName() << endl;
            }
            vecLegendHackGraphs_Exp[iExp].Draw("L same");
            /*
             gPad->Update();
             gPad->Modified();
             */
        }
    }
    
    for (int iExtra = 0; iExtra < numExtra; ++iExtra) {
        if (doVerb) {
            cout << "about to draw hack for iExtra " << iExtra << endl;
            cout << "vecLegendHackGraphs_Extra name " << vecLegendHackGraphs_Extra[iExtra].GetName() << endl;
        }
        vecLegendHackGraphs_Extra[iExtra].Draw("L same");
        /*
         gPad->Update();
         gPad->Modified();
         */
    }
    
    gPad->Update();
    gPad->Modified();
}

void LimitDrawer::SetVectorSizes() {
    vecGPs_Obs.resize(numObs);
    vecGPs_Exp.resize(numExp);
    vecGPs_Extra.resize(numExtra);
    
    vecLegendHackGraphs_Obs.resize(numObs);
    vecLegendHackGraphs_Exp.resize(numExp);
    vecLegendHackGraphs_Extra.resize(numExtra);
    
    vecHistNames_Obs.resize(numObs);
    vecHistNames_Exp.resize(numExp);
    vecHistNames_Extra.resize(numExtra);
    
    vecSigStrengthLimitHists_Obs.resize(numObs);
    vecSigStrengthLimitHists_Exp.resize(numExp);
    vecSigStrengthLimitHists_Extra.resize(numExtra);
    
    vecVecContours_Obs.resize(numObs);
    vecVecContours_Exp.resize(numExp);
    vecVecContours_Extra.resize(numExtra);
}

void LimitDrawer::SetVectorSizesSignif() {
    vecGPs_Obs.resize(numObs);
    vecGPs_Exp.resize(numExp * 2);
    vecGPs_Extra.resize(numExtra);
    
    vecLegendHackGraphs_Obs.resize(numObs);
    vecLegendHackGraphs_Exp.resize(numExp * 2);
    vecLegendHackGraphs_Extra.resize(numExtra);
    
    vecHistNames_Obs.resize(numObs);
    vecHistNames_Exp.resize(numExp);
    vecHistNames_Extra.resize(numExtra);
    
    vecSigStrengthLimitHists_Obs.resize(numObs);
    vecSigStrengthLimitHists_Exp.resize(numExp);
    vecSigStrengthLimitHists_Extra.resize(numExtra);
    
    vecVecContours_Obs.resize(numObs);
    vecVecContours_Exp.resize(numExp * 2);
    vecVecContours_Extra.resize(numExtra);
}

void LimitDrawer::DefaultSigHistNames() {
    for (int iObs = 0; iObs < numObs; ++iObs) {
        vecHistNames_Obs[iObs] = baseHistName + TString("Obs") + vecAddName[iObs];
        if (grabSmoothedCurves) vecHistNames_Obs[iObs] += nameSmoothedAddition;
    }
    for (int iExp = 0; iExp < numExp; ++iExp) {
        vecHistNames_Exp[iExp] = baseHistName + TString("Exp") + vecAddName[iExp];
        if (grabSmoothedCurves) vecHistNames_Exp[iExp] += nameSmoothedAddition;
    }
    for (int iExtra = 0; iExtra < numExtra; ++iExtra) {
        vecHistNames_Extra[iExtra] = baseHistName + TString("Obs");
        if (grabSmoothedCurves) vecHistNames_Extra[iExtra] += nameSmoothedAddition;
    }
}

void LimitDrawer::SetSignifGraphParams() {
    cout << "setting up the vectors of graph params" << endl;
    
    
    for (int iObs = 0; iObs < numObs; ++iObs) {
        vecGPs_Obs[iObs].SetAsBasicLimitContour(true, iObs);
    }
    
    for (int iExp = 0; iExp < numExp; ++iExp) {
        vecGPs_Exp[iExp].SetAsBasicLimitContour(0, iExp);
        vecGPs_Exp[iExp + 3].SetAsBasicSignifContour(1, iExp);
        //vecGPs_Exp[iExp + 6].SetAsBasicSignifContour(2, iExp);
    }
    
    for (int iExtra = 0; iExtra < numExtra; ++iExtra) {
        vecGPs_Extra[iExtra].SetAsAdditionalLimitContour(iExtra);
    }
}
void LimitDrawer::SetGraphParams() {
    cout << "setting up the vectors of graph params" << endl;
    
    
    for (int iObs = 0; iObs < numObs; ++iObs) {
        vecGPs_Obs[iObs].SetAsBasicLimitContour(true, iObs);
    }
    
    for (int iExp = 0; iExp < numExp; ++iExp) {
        vecGPs_Exp[iExp].SetAsBasicLimitContour(false, iExp);
    }
    
    for (int iExtra = 0; iExtra < numExtra; ++iExtra) {
        vecGPs_Extra[iExtra].SetAsAdditionalLimitContour(iExtra);
    }
}



