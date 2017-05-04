void SplitHistAlongDeltaM(TH2F * inputHist, int hackDeltaM, int whichDir) {
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
                    return;
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
}

void SetContourVecVec(vector< vector<TGraph *> > * inVecVecContours, vector<TH2F *> * inVecSigStrengthHists, vector<GraphParams> * inVecGPs, int minNumPoints, float desiredExclusion, int hackDeltaMRegion = -1, bool doVerb = false) {
    unsigned int numHists = inVecSigStrengthHists->size();
    unsigned int currNumContours;
    TH2F * currHist;
    TString currHistName;
    vector<TH2F *> vecOfCurrHist; //used for splitting along diagonals of no sensitivity -- i.e. deltaM = top mass for T2tt

    for (unsigned int iHist = 0; iHist < numHists; ++iHist) {
        currHist = inVecSigStrengthHists->at(iHist);
        currHistName = currHist->GetName();
        if (hackDeltaMRegion > -1) {
            vecOfCurrHist.clear();
            vecOfCurrHist.push_back((TH2F *) currHist->Clone(currHistName + "_ForwardDeltaMSplit"));
            vecOfCurrHist.push_back((TH2F *) currHist->Clone(currHistName + "_BackwardDeltaMSplit"));
            
            SplitHistAlongDeltaM(vecOfCurrHist[0], hackDeltaMRegion, 1);
            SplitHistAlongDeltaM(vecOfCurrHist[1], hackDeltaMRegion, -1);
            ReturnTGraphVector(&inVecVecContours->at(iHist), vecOfCurrHist[0], minNumPoints, desiredExclusion, true);
            ReturnTGraphVector(&inVecVecContours->at(iHist), vecOfCurrHist[1], minNumPoints, desiredExclusion, true);
        }
        else {
            ReturnTGraphVector(&inVecVecContours->at(iHist), inVecSigStrengthHists->at(iHist), minNumPoints, desiredExclusion);
        }
        
        currNumContours = inVecVecContours->at(iHist).size();
        if (doVerb) {
            cout << "for iHist = " << iHist << ", I found " << currNumContours << " contours " << endl;
        }
        for (unsigned iContour = 0; iContour < currNumContours; ++iContour) {
            inVecGPs->at(iHist).SetInputTGraph(inVecVecContours->at(iHist)[iContour]);
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
        else if (strncmp (argv[iLimitDrawerParams],"grabSmooth", 10) == 0) {
            grabSmoothedCurves = true;
        }
    }
    SUSYT2LPs.FullSequence(argc, argv);
    if (runInterpolation) {
        float currBinWidth = 25. / (TMath::Power(2, numInterpolate));
        SUSYT2LPs.SetStrings(currBinWidth);
    }
}

void LimitDrawer::DefaultVals() {
    canvDrawParams.DefaultVals();
    
    dirToSaveTo = "ContourPlots/";
    
    runInterpolation = false;
    numInterpolate = 1;
    
    nameSmoothedAddition = "_2DGaussSmoothed_OptKernel";
    grabSmoothedCurves = false;
    
    minNumPoints = 5;
    desiredExclusion = 1.0;
    
    numObs = 0;
    numExp = 0;
    numExtra = 0;
    
    intLumi = 19.7;
    beamEnergy = 8;
    textPrelim = " Preliminary";
    
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

void LimitDrawer::SetLoadingInformation(TFile * inputFile, TString inputBaseHistName = "") {
    inFile = inputFile;
    if (inputBaseHistName.Length() != 0) {
        baseHistName = inputBaseHistName;
        nameXSecMap = baseHistName + "Exp_XSecLimit";
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
}

void LimitDrawer::MakeContourVectors(bool doVerb = false) {
    cout << "about to make the contours " << endl;
    SetContourVecVec(&vecVecContours_Obs, &vecSigStrengthLimitHists_Obs, &vecGPs_Obs, minNumPoints, desiredExclusion, SUSYT2LPs.currSDP.hackDeltaMRegion, doVerb);
    SetContourVecVec(&vecVecContours_Exp, &vecSigStrengthLimitHists_Exp, &vecGPs_Exp, minNumPoints, desiredExclusion, SUSYT2LPs.currSDP.hackDeltaMRegion, doVerb);
    SetContourVecVec(&vecVecContours_Extra, &vecSigStrengthLimitHists_Extra, &vecGPs_Extra, minNumPoints, desiredExclusion, SUSYT2LPs.currSDP.hackDeltaMRegion, doVerb);
}

void LimitDrawer::SetCanvasName(TString inputString) {
    if (grabSmoothedCurves) {
        inputString += "_Smoothed";
    }
    canvDrawParams.canvName = "c_" + inputString;
    canvDrawParams.canvTitle = "c_" + inputString;
}

void LimitDrawer::FullSetupSequence_BasicExclusion_AlreadyHaveHists(bool doVerb = false) {
    canvDrawParams.SetAsLimitCanvas(SUSYT2LPs.currSDP.labelXaxis, SUSYT2LPs.currSDP.labelYaxis, "");
    canvDrawParams.SetEmptyHistogram(h_XSecLimitMap);
    InitializeCanvas();
    SetBasicParams();
    SetVectorSizes();
    DefaultSigHistNames();
    SetGraphParams_BasicExclusion();
    SetText_All(doVerb);
    
    ///Grab histograms
    MakeContourVectors(doVerb);
    if (runInterpolation) {
        DoInterpolation();
    }
}

void LimitDrawer::FullSetupSequence_BasicExclusion(bool doVerb = false) {
    GrabXSecLimitHist(doVerb);
    canvDrawParams.SetAsLimitCanvas(SUSYT2LPs.currSDP.labelXaxis, SUSYT2LPs.currSDP.labelYaxis, "");
    canvDrawParams.SetEmptyHistogram(h_XSecLimitMap);
    InitializeCanvas();
    SetBasicParams();
    SetVectorSizes();
    DefaultSigHistNames();
    SetGraphParams_BasicExclusion();
    SetText_All(doVerb);
    
    ///Grab histograms
    GrabSignalStrengthHists(doVerb);
    MakeContourVectors(doVerb);
    if (runInterpolation) {
        DoInterpolation();
    }
}

void LimitDrawer::FullDrawSequence(bool doVerb = false) {
    if (doVerb) {
        cout << "drawing the empty histogram" << endl;
    }
    canvDrawParams.DrawEmptyHisto(drawCanv);
    if (doVerb) {
        cout << "drawing the cross-section map " << endl;
    }
    SetHistoParams();
    //h_XSecLimitMap->Draw("colz");
    DrawContours(doVerb);
    DrawText_All(doVerb);
}


void LimitDrawer::InitializeCanvas() {
    drawCanv = new TCanvas(canvDrawParams.canvName, canvDrawParams.canvTitle, canvDrawParams.canvWidth, canvDrawParams.canvHeight);
    canvDrawParams.SetCanvas(drawCanv);
}

void LimitDrawer::SetText_PaletteLabel() {
    TString strPaletteLabel = "95% C.L. upper limit on cross section (pb)";
    
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

void LimitDrawer::SetText_CMS(bool doVerb = false) {
    char buf[300];
    sprintf(buf," %0.1f fb^{-1}, #sqrt{s} = %0.0f TeV", intLumi, beamEnergy);
    TString strCMS = "CMS";
    strCMS += textPrelim;
    strCMS += ", " + TString(string(buf));
    
    if (doVerb) {
        cout << "strCMS " << strCMS << endl;
    }
    float sizeCMS = 0.038;
    int alignCMS = 13;
    float posX_CMS = 0.22;
    float posY_CMS = 0.98;
    int fontCMS = 42;
    
    latexCMS.SetLatex(true, sizeCMS, alignCMS, posX_CMS, posY_CMS, strCMS);
    latexCMS.straightGhetto.textFont = fontCMS;
}

void LimitDrawer::SetText_LegHack(bool doVerb = false) {
    TString strObsHack = "Observed #pm 1 #sigma_{theory}";
    TString strExpHack = "Expected #pm 1 #sigma_{experiment}";
    
    if (doVerb) {
        cout << "strObsHack " << strObsHack << endl;
        cout << "strExpHack " << strExpHack << endl;
    }
    
    float sizeObsHack = 0.038;
    int alignObsHack = 13;
    
    float minX = SUSYT2LPs.currSDP.bounds_SUSYMasses.xLB;
    float maxY = SUSYT2LPs.currSDP.bounds_SUSYMasses.yUB;
    
    float xRange = SUSYT2LPs.currSDP.bounds_SUSYMasses.GetXRange();
    float yRange = SUSYT2LPs.currSDP.bounds_SUSYMasses.GetYRange();
    
    double x, yMax, yMax75;
    graph_WhiteBackground.GetPoint(1, x, yMax);
    graph_WhiteBackground.GetPoint(2, x, yMax75);
    
    float Ymax = SUSYT2LPs.currSDP.bounds_SUSYMasses.yUB + (SUSYT2LPs.currSDP.binWidth / 2);
    
    float posX_ObsHack = minX + 11 * xRange / 100;
//    float posY_ObsHack = maxY - 1.5 * yRange / 10;
    float posY_ObsHack = yMax75 + 0.5 * (yMax - yMax75);
    int fontObsHack = 42;
    latexObsHack.SetLatex(false, sizeObsHack, alignObsHack, posX_ObsHack, posY_ObsHack, strObsHack);
    latexObsHack.straightGhetto.textFont = fontObsHack;
    
    float sizeExpHack = 0.038;
    int alignExpHack = 13;
    float posX_ExpHack = minX + 11 * xRange / 100;
//    float posY_ExpHack = maxY - 2.15 * yRange / 10;
    float posY_ExpHack = yMax75 + 0.25 * (yMax - yMax75);
    int fontExpHack = 42;
    latexExpHack.SetLatex(false, sizeExpHack, alignExpHack, posX_ExpHack, posY_ExpHack, strExpHack);
    latexExpHack.straightGhetto.textFont = fontExpHack;
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
    
    float Ymin = SUSYT2LPs.currSDP.bounds_SUSYMasses.yLB - (SUSYT2LPs.currSDP.binWidth / 2);
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

void LimitDrawer::SetText_All(bool doVerb = false) {
    SetText_WhiteBackground(doVerb);
    SetText_CMS(doVerb);
    //SetText_Model(doVerb);
    SetText_PaletteLabel();
    SetText_LegHack(doVerb);
}


void LimitDrawer::SetBasicParams() {
    
    numObs = 3;
    numExp = 3;
    numExtra = 0;
    
}

void LimitDrawer::SetHistoParams() {
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
    h_XSecLimitMap->GetZaxis()->SetRangeUser(10e-4,100);
    
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

void LimitDrawer::DrawText_All(bool doVerb = false) {
    if (doVerb) {
        cout << "drawing all the text " << endl;
        
        cout << "redrawing the canvas axis" << endl;
    }
    drawCanv->RedrawAxis();
    DrawText_WhiteBackground();
    DrawText_CMS(doVerb);
    SUSYT2LPs.DrawOnCanvas(drawCanv, true, doVerb);
    //DrawText_Model(doVerb);
    DrawText_PaletteLabel(doVerb);
    DrawLegend_BasicExclusion(doVerb);
}

void LimitDrawer::DrawContours(bool doVerb = false) {
    if (doVerb) {
        cout << "about to draw the contours " << endl;
    }
    drawCanv->cd();
    
    unsigned int currNumContours;
    for (int iExp = 0; iExp < numExp; ++iExp) {
        currNumContours = vecVecContours_Exp[iExp].size();
        if (doVerb) {
            cout << "for iExp = " << iExp << ", I found " << currNumContours << " contours " << endl;
        }
        for (unsigned iContour = 0; iContour < currNumContours; ++iContour) {
            vecVecContours_Exp[iExp][iContour]->Draw("L same");
        }
    }
    for (int iExtra = 0; iExtra < numExtra; ++iExtra) {
        currNumContours = vecVecContours_Extra[iExtra].size();
        if (doVerb) {
            cout << "for iExtra = " << iExtra << ", I found " << currNumContours << " contours " << endl;
        }
        for (unsigned iContour = 0; iContour < currNumContours; ++iContour) {
            vecVecContours_Extra[iExtra][iContour]->Draw("L same");
        }
    }
    for (int iObs = 0; iObs < numObs; ++iObs) {
        currNumContours = vecVecContours_Obs[iObs].size();
        if (doVerb) {
            cout << "for iObs = " << iObs << ", I found " << currNumContours << " contours " << endl;
        }
        for (unsigned iContour = 0; iContour < currNumContours; ++iContour) {
            vecVecContours_Obs[iObs][iContour]->Draw("L same");
        }
    }
}

void LimitDrawer::DrawLegend_BasicExclusion(bool doVerb = false) {
    if (doVerb) {
        cout << "going to draw the Legend hack " << endl;
    }
    float minX = SUSYT2LPs.currSDP.bounds_SUSYMasses.xLB;
    float maxY = SUSYT2LPs.currSDP.bounds_SUSYMasses.yUB;
    
    float xRange = SUSYT2LPs.currSDP.bounds_SUSYMasses.GetXRange();
    float yRange = SUSYT2LPs.currSDP.bounds_SUSYMasses.GetYRange();
    
    pairFloats xPair(minX, xRange);

    
    double x, yMax, yMax75;
    graph_WhiteBackground.GetPoint(1, x, yMax);
    graph_WhiteBackground.GetPoint(2, x, yMax75);
    
    pairFloats yPair(yMax75, yMax);
    
    for (int iObs = 0; iObs < numObs; ++iObs) {
        vecGPs_Obs[iObs].SetInputTGraphForLegendHack(&vecLegendHackGraphs_Obs[iObs], &xPair, &yPair, true, iObs, doVerb);
    }
    
    for (int iExp = 0; iExp < numExp; ++iExp) {
        vecGPs_Exp[iExp].SetInputTGraphForLegendHack(&vecLegendHackGraphs_Exp[iExp], &xPair, &yPair, false, iExp, doVerb);
    }
    
    latexObsHack.DrawCanvas(drawCanv, 1, doVerb);
    latexExpHack.DrawCanvas(drawCanv, 1, doVerb);
    
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
    
    vecSigStrengthLimitHists_Obs.resize(numObs);
    vecSigStrengthLimitHists_Exp.resize(numExp);
    vecSigStrengthLimitHists_Extra.resize(numExtra);
    
    vecVecContours_Obs.resize(numObs);
    vecVecContours_Exp.resize(numExp);
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
}

void LimitDrawer::SetGraphParams_BasicExclusion() {
    cout << "setting up the vectors of graph params" << endl;
    
    
    for (int iObs = 0; iObs < numObs; ++iObs) {
        vecGPs_Obs[iObs].SetAsBasicLimitContour(true, iObs);
    }
    
    for (int iExp = 0; iExp < numExp; ++iExp) {
        vecGPs_Exp[iExp].SetAsBasicLimitContour(false, iExp);
    }
}



