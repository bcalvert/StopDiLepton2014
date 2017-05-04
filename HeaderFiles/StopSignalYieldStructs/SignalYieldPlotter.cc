#ifndef SignalYieldPlotter_cc
#define SignalYieldPlotter_cc
void SignalYieldPlotter::SetLatex() {
//    xLatex = 0.1675;
//    yLatex = 0.785;
    xLatex = 0.155;
    yLatex = 0.83;

    sizeLatex = 0.06;
    strT2.SetNDC();
    strT2.SetTextSize(sizeLatex);
    strT2_SecondTitle.SetNDC();
    strT2_SecondTitle.SetTextSize(sizeLatex);
}

void SignalYieldPlotter::CallLatexSedScript(bool doVerb = false) {
    TString outDir = "LatexFigs/";
    TString baseFigName="LatexFigs/fig_sig_EffMapPATSY.tex";
    TString outFigName="LatexFigs/fig_sig_";
    outFigName += strFileName;
    outFigName += ".tex";
    System_CopyFile(baseFigName, outFigName, doVerb);
    
    TString strFileNameToReplace = "EffMapFilename";
    TString strFileDirToReplace = "EffMapDir";
    TString strEffStringToReplace = "EffMapString";
    TString strT2SMSToReplace = "T2String";
    TString strMT2CutToReplace = !whichPlotType ? " to pass the cut, MT2Cut" : "MT2Cut";
    
    TString strDoubleHash = "##";
    TString strSlash = "\\\\";
    
    System_SedInFile(outFigName, strFileNameToReplace, strFileName, doVerb);
    System_SedInFile(outFigName, strFileDirToReplace, strFileDir, doVerb);
    System_SedInFile(outFigName, strEffStringToReplace, strInFile, doVerb);
    System_SedInFile(outFigName, strT2SMSToReplace, strT2SMS, doVerb);
    if (whichPlotType) {
        System_SedInFile(outFigName, strMT2CutToReplace, strMT2Cut, doVerb);
    }
    else {
        System_SedInFile(outFigName, strMT2CutToReplace, "", doVerb);
    }
    System_SedMultipleInFile(outFigName, strDoubleHash, strSlash, doVerb);
}
void SignalYieldPlotter::SetStringsOutLatex(CoarseMapMaker * inCMM, SUSYT2LimitParams * inSUSYT2LP) {
    TString strLeptEffSF = "estimated efficiency of the generator-level lepton filter when applied to the";
    TString strCoarseSigEff = "signal efficiency";
    TString strSmoothSigEff = "smoothed signal efficiency";
    TString strSignalYield = "smoothed signal yield";
    TString strSignalContam = "relative signal contamination";
    TString arrStr[4] = {strLeptEffSF, strCoarseSigEff, strSmoothSigEff, strSignalYield};
    
    strInFile = arrStr[whichPlotType];
    if (whichPlotType == 3 && inCMM->doControl) {
        strInFile = strSignalContam;
        
        TString strToReplaceCanvName = "SigYield";
        int lengthStrToReplaceCanvName = strToReplaceCanvName.Length();
        TString strReplaceCanvName = "SigContam";
        int lengthStrReplaceCanvName = strReplaceCanvName.Length();
        
        baseCanvName.Replace(baseCanvName.Index(strToReplaceCanvName), lengthStrToReplaceCanvName, strReplaceCanvName, lengthStrReplaceCanvName);
    }
    
    strMT2Cut = TString("$") + inCMM->GetMT2LatexString() + TString("$");
    
    nameEffMap = baseCanvName;
    nameEffMap.Replace(0, 2, "");
    strFileName = nameEffMap + canvAddName;
    strFileDir = nameEffMap;
    strT2SMS = inSUSYT2LP->LatexString();
}
void SignalYieldPlotter::SetCanvas(int whichSyst = 0) {
    TString fullCanvName = baseCanvName + canvAddName;
    int canvWidth = 1600;
    int canvHeight = 1200;
    
    float canvTopMargin = 0.081;
    /*
     TCanvas * canvToUse;
     if (whichSyst == 0) {
     canvToUse = canvCV;
     }
     else {
     if (makeFullSystCombo) {
     fullCanvName += "_FullSyst";
     }
     else {
     fullCanvName += vecSystNames[abs(whichSyst) - 1];
     }
     if (whichSyst > 0) {
     fullCanvName += "Up";
     if (doRatio) fullCanvName += "_RatioCV";
     canvToUse = tempCanv_SystUp;
     }
     else {
     fullCanvName += "Down";
     if (doRatio) fullCanvName += "_RatioCV";
     canvToUse = tempCanv_SystDown;
     }
     }
     canvToUse = new TCanvas(fullCanvName, fullCanvName, canvWidth, canvHeight);
     canvToUse->SetTopMargin(canvTopMargin);
     if (whichPlotType >= 2) {
     canvToUse->SetLogz();
     }
     */
    
    if (whichSyst == 0) {
        canvCV = new TCanvas(fullCanvName, baseCanvName + canvAddName, canvWidth, canvHeight);
        canvCV->SetTopMargin(canvTopMargin);
        if (whichPlotType >= 2) {
	  //            canvCV->SetLogz();
        }
    }
    else {
        if (makeFullSystCombo) {
            fullCanvName += "_FullSyst";
        }
        else {
            fullCanvName += vecSystNames[abs(whichSyst) - 1];
        }
        if (whichSyst > 0) {
            fullCanvName += "Up";
            if (doRatio) fullCanvName += "_RatioCV";
            tempCanv_SystUp = new TCanvas(fullCanvName, fullCanvName, canvWidth, canvHeight);
            tempCanv_SystUp->SetTopMargin(canvTopMargin);
            /*
             if (whichPlotType >= 2) {
             tempCanv_SystUp->SetLogz();
             }
             */
        }
        else {
            fullCanvName += "Down";
            if (doRatio) fullCanvName += "_RatioCV";
            tempCanv_SystDown = new TCanvas(fullCanvName, fullCanvName, canvWidth, canvHeight);
            tempCanv_SystDown->SetTopMargin(canvTopMargin);
            /*
             if (whichPlotType >= 2) {
             tempCanv_SystDown->SetLogz();
             }
             */
        }
    }
}
void SignalYieldPlotter::SaveCanvas(int whichSyst = 0) {
    TCanvas * canvToSave;
    if (whichSyst == 0) {
        canvToSave = canvCV;
    }
    else if (whichSyst > 0) {
        canvToSave = tempCanv_SystUp;
    }
    else if (whichSyst < 0) {
        canvToSave = tempCanv_SystDown;
    }
    canvToSave->SaveAs(baseCanvDir + canvToSave->GetName() + TString(".pdf"));
    canvToSave->SaveAs(baseCanvDir + canvToSave->GetName() + TString(".C"));
}
void SignalYieldPlotter::SetHist(TH2F * inHist, TString titleString) {
    inHist->GetXaxis()->SetTitle("m_{#tilde{t}_{1}} [GeV]");
    inHist->GetYaxis()->SetTitle("m_{#tilde{#chi}^{0}_{1}} [GeV]");
    inHist->GetYaxis()->SetTitleOffset(1.0);
    inHist->SetTitle(titleString);
    gStyle->SetTitleFontSize(0.037);
    gStyle->SetTitleX(0.11);
    gStyle->SetTitleY(0.981);
    gStyle->SetTitleH(0.035);
}
void SignalYieldPlotter::DrawHist(LeptFiltSignalAssociator * inLFSC, CoarseMapMaker * inCMM, SmoothMapMaker * inSMM, SignalYieldMaker * inSYM, SUSYT2LimitParams * inSUSYT2LP, bool doVerb = false) {
    //For showing Signal Contamination
    if (whichPlotType == 3 && inSYM->doSigContam && inCMM->doControl) {
        inSYM->NormalizeSigYieldByTTBar();
        TString strToReplace = "Signal Yield";
        int lengthStrToReplace = strToReplace.Length();
        TString strReplace = "Sig. Contam. Frac.";
        int lengthStrReplace = strReplace.Length();
        
        baseCanvTitle.Replace(baseCanvTitle.Index(strToReplace), lengthStrToReplace, strReplace, lengthStrReplace);
        
        TString strToReplaceCanvName = "SigYield";
        int lengthStrToReplaceCanvName = strToReplaceCanvName.Length();
        TString strReplaceCanvName = "SigContam";
        int lengthStrReplaceCanvName = strReplaceCanvName.Length();
        
        baseCanvName.Replace(baseCanvName.Index(strToReplaceCanvName), lengthStrToReplaceCanvName, strReplaceCanvName, lengthStrReplaceCanvName);
    }
    
    TH2F * histCVToDraw;
    vector<TH2F *> * vecHistToDraw_SystUp = NULL, * vecHistToDraw_SystDown = NULL;
    
    switch (whichPlotType) {
        case 0:
            histCVToDraw = inLFSC->outHist;
            break;
        case 1:
            histCVToDraw = inCMM->outHist_CV;
            vecHistToDraw_SystUp = &inCMM->vecOutHist_SystVarUp;
            vecHistToDraw_SystDown = &inCMM->vecOutHist_SystVarDown;
            break;
        case 2:
            histCVToDraw = inSMM->histCV;
            vecHistToDraw_SystUp = &inSMM->vecHist_SystVarUp;
            vecHistToDraw_SystDown = &inSMM->vecHist_SystVarDown;
            break;
        case 3:
            histCVToDraw = inSYM->outYieldHist_CV;
            vecHistToDraw_SystUp = &inSYM->vecOutYieldHist_SystVarUp;
            vecHistToDraw_SystDown = &inSYM->vecOutYieldHist_SystVarDown;
            break;
        default:
            cout << "whichPlotType must be between 0 through 3 -- it is " << whichPlotType <<  endl;
            break;
    }
    
    float currMin, currMax;
    float minLeptFilt = 0.4;
    float maxLeptFilt = 0.65;
    
    //Do Central Value
    //Set-up Canvas
    if (doVerb) {
        cout << "vecHistToDraw_SystUp size " << vecHistToDraw_SystUp->size() << endl;
        cout << "vecHistToDraw_SystDown size " << vecHistToDraw_SystDown->size() << endl;
    }
    
    inSUSYT2LP->SetStrings(histCVToDraw->GetXaxis()->GetBinWidth(1), histCVToDraw->GetXaxis()->GetXmax(), histCVToDraw->GetYaxis()->GetXmax(), true);
    
    if (drawSysts && whichPlotType != 0) {
        TH2F * ratioHistSyst;
        if (makeFullSystCombo) {
            TH2F * ratioHistSystUp, * ratioHistSystDown;
            vector<TH2F *> vecRatioHistSystUp(0);
            vector<TH2F *> vecRatioHistSystDown(0);
            
            for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                if (vecSystNames[iSyst].Contains("CT10") || vecSystNames[iSyst].Contains("MSTW") || vecSystNames[iSyst].Contains("NNPDF")) {
                    continue;
                }
                else {
                    vecRatioHistSystUp.push_back(SystRatioHist(histCVToDraw, vecHistToDraw_SystUp->at(iSyst)));
                    vecRatioHistSystDown.push_back(SystRatioHist(histCVToDraw, vecHistToDraw_SystDown->at(iSyst)));
                }
            }
            MakeComboSystHist(ratioHistSystUp, ratioHistSystDown, histCVToDraw, &vecRatioHistSystUp, &vecRatioHistSystDown, doVerb);
            
            
            SetCanvas(1);
            if (doVerb) {
                cout << "Drawing FullSyst Up shift canvas" << endl;
            }
            SetHist(ratioHistSystUp, baseCanvTitle);
            FindExtremaBelowDiagonal(ratioHistSystUp, currMin, currMax);
            ratioHistSystUp->GetXaxis()->SetRangeUser(100, 800);
            inSUSYT2LP->SetStrings(ratioHistSystUp->GetXaxis()->GetBinWidth(1), ratioHistSystUp->GetXaxis()->GetXmax(), ratioHistSystUp->GetYaxis()->GetXmax(), true);
            ratioHistSystUp->GetZaxis()->SetRangeUser(0.99, 1.25);
            ratioHistSystUp->Draw("colz");
	    TString tempTitle = ratioHistSystUp->GetTitle(); 
	    tempTitle.Replace(tempTitle.Index("Signal Yield"), 12, "Relative Systematic Uncertainty");
	    ratioHistSystUp->SetTitle(tempTitle);
	    inSUSYT2LP->currSDP.DrawOnCanvas(tempCanv_SystUp, true, doVerb);
            strT2.DrawLatex(xLatex, yLatex, inSUSYT2LP->TitleString());
            SaveCanvas(1);
            
            SetCanvas(-1);
            if (doVerb) {
                cout << "Drawing FullSyst Down shift canvas" << endl;
            }
            SetHist(ratioHistSystDown, baseCanvTitle);
            FindExtremaBelowDiagonal(ratioHistSystDown, currMin, currMax);
            ratioHistSystDown->GetXaxis()->SetRangeUser(100, 800);
            inSUSYT2LP->SetStrings(ratioHistSystDown->GetXaxis()->GetBinWidth(1), ratioHistSystDown->GetXaxis()->GetXmax(), ratioHistSystDown->GetYaxis()->GetXmax(), true);
            ratioHistSystDown->GetZaxis()->SetRangeUser(0.75, 1.01);
            ratioHistSystDown->Draw("colz");
            strT2.DrawLatex(xLatex, yLatex, inSUSYT2LP->TitleString());
            SaveCanvas(-1);
        }
        else {
            for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                //Do Syst Shift Up
                if (doVerb) {
                    cout << "setting Up shift canvas for iSyst = " << iSyst << endl;
                }
                SetCanvas(iSyst + 1);
                if (doVerb) {
                    cout << "Drawing Up shift canvas for iSyst = " << iSyst << endl;
                }
                if (doRatio) {
                    if (doVerb) {
                        cout << "histCVToDraw: " << histCVToDraw << endl;
                        cout << "histCVToDraw Name: " << histCVToDraw->GetName() << endl;
                        cout << "vecHistToDraw_SystUp->at(iSyst): " << vecHistToDraw_SystUp->at(iSyst) << endl;
                        cout << "vecHistToDraw_SystUp->at(iSyst) Name: " << vecHistToDraw_SystUp->at(iSyst)->GetName() << endl;
                    }
                    ratioHistSyst = SystRatioHist(histCVToDraw, vecHistToDraw_SystUp->at(iSyst));
                    SetHist(ratioHistSyst, baseCanvTitle);
                    FindExtremaBelowDiagonal(ratioHistSyst, currMin, currMax);
                    ratioHistSyst->GetZaxis()->SetRangeUser(currMin, currMax);
                    ratioHistSyst->GetXaxis()->SetRangeUser(100, 800);
                    inSUSYT2LP->SetStrings(ratioHistSyst->GetXaxis()->GetBinWidth(1), ratioHistSyst->GetXaxis()->GetXmax(), ratioHistSyst->GetYaxis()->GetXmax(), true);
                    ratioHistSyst->GetZaxis()->SetRangeUser(0.99, 1.25);
                    ratioHistSyst->Draw("colz");
                }
                else {
                    SetHist(vecHistToDraw_SystUp->at(iSyst), baseCanvTitle);
                    vecHistToDraw_SystUp->at(iSyst)->Draw("colz");
                }
                strT2.DrawLatex(xLatex, yLatex, inSUSYT2LP->TitleString());
                inSUSYT2LP->currSDP.DrawOnCanvas(tempCanv_SystUp, true, doVerb);
                SaveCanvas(iSyst + 1);
                
                //Do Syst Shift Down
                if (doVerb) {
                    cout << "setting Down shift canvas for iSyst = " << iSyst << endl;
                }
                SetCanvas(-1 * (iSyst + 1));
                if (doVerb) {
                    cout << "Drawing Down shift canvas for iSyst = " << iSyst << endl;
                }
                if (doRatio) {
                    if (doVerb) {
                        cout << "histCVToDraw: " << histCVToDraw << endl;
                        cout << "histCVToDraw Name: " << histCVToDraw->GetName() << endl;
                        cout << "vecHistToDraw_SystDown->at(iSyst): " << vecHistToDraw_SystDown->at(iSyst) << endl;
                        cout << "vecHistToDraw_SystDown->at(iSyst) Name: " << vecHistToDraw_SystDown->at(iSyst)->GetName() << endl;
                    }
                    ratioHistSyst = SystRatioHist(histCVToDraw, vecHistToDraw_SystDown->at(iSyst));
                    SetHist(ratioHistSyst, baseCanvTitle);
                    FindExtremaBelowDiagonal(ratioHistSyst, currMin, currMax);
                    ratioHistSyst->GetZaxis()->SetRangeUser(currMin, currMax);
                    ratioHistSyst->GetXaxis()->SetRangeUser(100, 800);
                    inSUSYT2LP->SetStrings(ratioHistSyst->GetXaxis()->GetBinWidth(1), ratioHistSyst->GetXaxis()->GetXmax(), ratioHistSyst->GetYaxis()->GetXmax(), true);
                    ratioHistSyst->GetZaxis()->SetRangeUser(0.75, 1.01);
                    ratioHistSyst->Draw("colz");
                }
                else {
                    SetHist(vecHistToDraw_SystDown->at(iSyst), baseCanvTitle);
                    vecHistToDraw_SystDown->at(iSyst)->Draw("colz");
                }
                strT2.DrawLatex(xLatex, yLatex, inSUSYT2LP->TitleString());
                inSUSYT2LP->currSDP.DrawOnCanvas(tempCanv_SystDown, true, doVerb);
                SaveCanvas(-1 * (iSyst + 1));
            }
        }
    }
    
    
    SetCanvas(0);
    SetHist(histCVToDraw, baseCanvTitle);
    if (whichPlotType == 0) {
        histCVToDraw->GetZaxis()->SetRangeUser(minLeptFilt, maxLeptFilt);
    }
    if (whichPlotType == 3 && inSYM->doSigContam && inCMM->doControl) {
        Size_t markerSize = 0.8;
        histCVToDraw->SetMarkerColor(kWhite);
        histCVToDraw->SetMarkerSize(markerSize);
        histCVToDraw->SetFillColor(kWhite);
        gStyle->SetPaintTextFormat("4.2f");
        histCVToDraw->Draw("colz text");
        histCVToDraw->GetXaxis()->SetRangeUser(100, 450);
        histCVToDraw->GetYaxis()->SetRangeUser(0, 350);
        inSUSYT2LP->SetStrings(histCVToDraw->GetXaxis()->GetBinWidth(1), 450 + histCVToDraw->GetXaxis()->GetBinWidth(1),  350 + histCVToDraw->GetYaxis()->GetBinWidth(1), true);
    }
    else if (whichPlotType == 1 || whichPlotType == 2) {
        MakePercentageEfficiencyHist(histCVToDraw);
        histCVToDraw->GetZaxis()->SetTitle("Efficiency (%)");
        histCVToDraw->Draw("colz");
        gPad->Update();
        TPaletteAxis * palette = (TPaletteAxis*)(histCVToDraw->GetListOfFunctions()->FindObject("palette"));
        /*
         palette->SetX1NDC(1.-0.18);
         palette->SetY1NDC(0.14);
         palette->SetX2NDC(1.-0.13);
         palette->SetY2NDC(1.-0.08);
         */
        
        palette->SetX2NDC(palette->GetX1NDC() + 0.025);
        palette->SetTitleOffset(0.7);
        palette->SetLabelFont(42);
        palette->SetLabelSize(0.035);
    }
    else {
        histCVToDraw->Draw("colz");
    }
    strT2.DrawLatex(xLatex, yLatex, inSUSYT2LP->TitleString());
    inSUSYT2LP->currSDP.DrawOnCanvas(canvCV, true, doVerb);
    SaveCanvas(0);
    
    
}
void SignalYieldPlotter::DrawHist(SignalYieldMaker * inSYM, CoarseMapMaker * inCMM, bool doVerb = false) {
    if (!drawSysts) return;
    TString canvPDFName[2] = {"_PDFEnvelope", "_PDFEnvelopeBookKeep"};
    TString fullCanvName;
    TCanvas * c1;
    int canvWidth = 1600;
    int canvHeight = 1200;
    TH2F * ratioHistSyst;
    float currMin, currMax;
    for (unsigned int iPDF = 1; iPDF < 3; ++iPDF) {
        if (doVerb) {
            cout << "Setting Up shift canvas for iPDF = " << iPDF << endl;
        }
        fullCanvName = baseCanvName + canvAddName;
        fullCanvName += canvPDFName[iPDF - 1];
        fullCanvName += "Up";
        if (doRatio && iPDF == 1) fullCanvName += "_RatioCV";
        c1 = new TCanvas(fullCanvName, fullCanvName, canvWidth, canvHeight);
        if (doVerb) {
            cout << "Drawing Up shift canvas for iPDF = " << iPDF << endl;
        }
        if (doRatio && iPDF == 1) {
            ratioHistSyst = SystRatioHist(inSYM->outYieldHist_CV, inSYM->vecOutYieldHist_SystVarUp[vecSystFinalNames.size() - 1 + iPDF]);
            SetHist(ratioHistSyst, baseCanvTitle);
            FindExtremaBelowDiagonal(ratioHistSyst, currMin, currMax);
            ratioHistSyst->GetZaxis()->SetRangeUser(currMin, currMax);
            ratioHistSyst->GetXaxis()->SetRangeUser(100, 800);
            ratioHistSyst->GetZaxis()->SetRangeUser(0.99, 1.25);
            ratioHistSyst->Draw("colz");
        }
        else {
            SetHist(inSYM->vecOutYieldHist_SystVarUp[vecSystFinalNames.size() - 1 + iPDF], baseCanvTitle);
            inSYM->vecOutYieldHist_SystVarUp[vecSystFinalNames.size() - 1 + iPDF]->Draw("colz");
        }
        c1->SaveAs(baseCanvDir + fullCanvName + TString(".pdf"));
        c1->SaveAs(baseCanvDir + fullCanvName + TString(".C"));
        
        if (doVerb) {
            cout << "Setting Down shift canvas for iPDF = " << iPDF << endl;
        }
        fullCanvName = baseCanvName + canvAddName;
        fullCanvName += canvPDFName[iPDF - 1];
        fullCanvName += "Down";
        if (doRatio && iPDF == 1) fullCanvName += "_RatioCV";
        c1 = new TCanvas(fullCanvName, fullCanvName, canvWidth, canvHeight);
        if (doVerb) {
            cout << "Drawing Down shift canvas for iPDF = " << iPDF << endl;
        }
        if (doRatio && iPDF == 1) {
            ratioHistSyst = SystRatioHist(inSYM->outYieldHist_CV, inSYM->vecOutYieldHist_SystVarDown[vecSystFinalNames.size() - 1 + iPDF]);
            SetHist(ratioHistSyst, baseCanvTitle);
            FindExtremaBelowDiagonal(ratioHistSyst, currMin, currMax);
            ratioHistSyst->GetZaxis()->SetRangeUser(currMin, currMax);
            ratioHistSyst->GetXaxis()->SetRangeUser(100, 800);
            ratioHistSyst->GetZaxis()->SetRangeUser(0.75, 1.01);
            ratioHistSyst->Draw("colz");
        }
        else {
            SetHist(inSYM->vecOutYieldHist_SystVarDown[vecSystFinalNames.size() - 1 + iPDF], baseCanvTitle);
            inSYM->vecOutYieldHist_SystVarDown[vecSystFinalNames.size() - 1 + iPDF]->Draw("colz");
        }
        c1->SaveAs(baseCanvDir + fullCanvName + TString(".pdf"));
        c1->SaveAs(baseCanvDir + fullCanvName + TString(".C"));
    }
}

void SignalYieldPlotter::DefaultVals(int inWPT) {
    doDraw = true;
    showYields = true;
    callLatexSedScript = false;
    doAverage = false;
    doRatio = true;
    drawSysts = false;
    makeFullSystCombo = true;
    whichPlotType = inWPT;
    
    TString baseBaseCanvName = "c_";
    
    const int numTypes = 4;
    TString baseCanvNameArr[numTypes] = {"LeptonFilterSF", "CoarseSigEff", "SmoothSigEff", "SigYield"};
    TString baseCanvTitleArr[numTypes] = {"Est. Lept. Filt. Eff.", "Coarse Signal Eff. for ", "Smoothed Signal Eff. for ", "Signal Yield for "};
    
    baseCanvName = baseBaseCanvName;
    baseCanvName += baseCanvNameArr[whichPlotType];
    
    baseCanvTitle = baseCanvTitleArr[whichPlotType];
    
    baseCanvDir = TString("SigEffPlots/") + baseCanvNameArr[inWPT] + TString("/");
    
    SetLatex();
}
void SignalYieldPlotter::SetCanvasAddName(SUSYT2LimitParams * inSUSYT2LP, CoarseMapMaker * inCMM = NULL) {
    canvAddName = "_" + inSUSYT2LP->SavingName(false, inSUSYT2LP->typeT2 == 2);
    cout << "canvAddName " << canvAddName << endl;
    canvAddName.Replace(canvAddName.Index("."), 1, "p", 1); //get rid of . in canvas name
    cout << "canvAddName " << canvAddName << endl;
    if (inCMM) {
        canvAddName += inCMM->GetMT2String();
    }
}
void SignalYieldPlotter::InitializeVecs(int whichT2) {
    bool includePDF = whichT2 < 2;
    vecSystNames.clear();
    SetSystVec(&vecSystNames, includePDF);
    
    vecSystFinalNames.clear();
    SetSystVecFinal(&vecSystFinalNames, includePDF);
    
    unsigned int numSysts = vecSystFinalNames.size();
    
    vecCanv_SystUp.resize(numSysts);
    vecCanv_SystDown.resize(numSysts);
}

void SignalYieldPlotter::Draw(LeptFiltSignalAssociator * inLFSC, CoarseMapMaker * inCMM, SmoothMapMaker * inSMM, SignalYieldMaker * inSYM, SUSYT2LimitParams * inSUSYT2LP, bool doVerb = false) {
    if (whichPlotType == 0) {
        SetCanvasAddName(inSUSYT2LP);
        
    }
    else {
        baseCanvTitle += inCMM->GetMT2TitleString();
        SetCanvasAddName(inSUSYT2LP, inCMM);
    }
    DrawHist(inLFSC, inCMM, inSMM, inSYM, inSUSYT2LP, doVerb);
    if (whichPlotType == 3) {
        DrawHist(inSYM, inCMM, doVerb);
    }
}

void SignalYieldPlotter::SetParamsFromCommandLine(int argc, char* argv[]) {
    for (int iSignalEffDrawParam = 0; iSignalEffDrawParam < argc; ++iSignalEffDrawParam) {
        if (strncmp (argv[iSignalEffDrawParam],"noDraw", 6) == 0) {
            doDraw = 0;
        }
        else if (strncmp (argv[iSignalEffDrawParam],"callLatex", 9) == 0) {
            callLatexSedScript = 1;
        }
        else if (strncmp (argv[iSignalEffDrawParam],"doSyst", 6) == 0) {
            drawSysts = 1;
        }
        else if (strncmp (argv[iSignalEffDrawParam],"doIndSyst", 9) == 0) {
            makeFullSystCombo = 0;
        }
    }
}
#endif
