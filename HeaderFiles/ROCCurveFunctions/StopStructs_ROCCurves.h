#include "TH1F.h"
#include "TPad.h"

typedef pair<float, float> knobPunzi;

struct ROCCurveParams {
    int minCut, maxCut, cutStepSize;
    vector<int> vecCutVals;
    
    //string for saving the ROC/Punzi Curves;
    TString strROCSave, strDraw;
    //strings for making the ROC/Punzi Curves
    TString strVarCut, strCutCommand;
    //int for number of BJets cut
    int numBJetsCut;
    //string for inner axis labels
    TString strXAxisSave;
    
    vector<knobPunzi> vecKnobPunzis;
    vector<TString> vecPostPunziNames;
    
    TString strDirSave;
    
    void SetDirString() {
        TString strUnder = "_";
        TString strSlash = "/";
        TString baseName = "ROCCurves/";
        baseName += strDraw + strSlash;
        baseName += strROCSave + strSlash;
        System_MakeDir(baseName);
        strDirSave = baseName;
    }
    
    TString GetOutFileNameBase(bool isROCCurve, TString addString) {
        TString strUnder = "_";
        TString baseName = strDirSave;
        baseName += isROCCurve ? "ROCCurves" : "Yields";
        TString appendName = strUnder + strDraw + strUnder + strROCSave;
        
        TString outFileName = baseName + appendName + addString;
        outFileName += ".root";
        return outFileName;
    }
    void SetAsROCCurve(bool isSim, int whichDraw, int whichROCType, int whichROC) {
        strDraw = ROCDrawString(whichDraw);
        strCutCommand = ROCCutString(isSim, whichDraw);
        
        strVarCut = ROCMainVarCutString(isSim, whichROCType, whichROC);
        strXAxisSave = PunziAxisString(whichROCType, whichROC);
        strROCSave = ROCSaveString(whichROCType, whichROC);
        SetMainVarCutValsVec(&vecCutVals, whichROCType, whichROC);
        
        numBJetsCut = NumBJetsCut(whichDraw);
        
        SetDirString();
    }
    
    void SetUpPunziKnobs() {
        vecKnobPunzis.resize(0);
        vecKnobPunzis.push_back(knobPunzi(5.0, 2.0));
        vecKnobPunzis.push_back(knobPunzi(2.0, 5.0));
        vecKnobPunzis.push_back(knobPunzi(2.0, 2.0));
        vecKnobPunzis.push_back(knobPunzi(2.0, 0.0));
    }
    void SetUpPunziNames() {
        unsigned int numPunziKnobs = vecKnobPunzis.size();
        vecPostPunziNames.resize(numPunziKnobs);
        for (unsigned int iPunzi = 0; iPunzi < numPunziKnobs; ++iPunzi) {
            vecPostPunziNames[iPunzi] = PostPunziName(&vecKnobPunzis[iPunzi]);
        }
    }
    void SetUpPunzi() {
        SetUpPunziKnobs();
        SetUpPunziNames();
    }
};

struct ROCDrawingParams {
    vector<int> vecIndices;
    
    vector<Color_t> vecROCCurvesColor;
    vector<Style_t> vecROCCurvesStyle;
    vector<TString> vecLegStrings;
    
    int whichT2SMS, addtlParam;
    TString strLatexSignal;
    
    vector<TCanvas *> vecDrawCanvas;
    vector<TString> vecNameCanvas;
    vector<TString> vecTLatexCuts;
    TString canvNameAppend;
    //Drawing
    void SetTLatexCuts(int whichSel) {
        FillLatexCutString(whichSel, &vecTLatexCuts);
    }
    void SetCanvasName(int whichCanv, TString inputName) {
        vecNameCanvas[whichCanv] = inputName + canvNameAppend;
    }
    void SetCanvas(int whichCanv, bool doLog) {
        int singCanvWidth = 1200;
        int singCanvHeight = 800;
        vecDrawCanvas[whichCanv] = new TCanvas(vecNameCanvas[whichCanv], vecNameCanvas[whichCanv], singCanvWidth, singCanvHeight);
        vecDrawCanvas[whichCanv]->SetLogy(doLog);
        vecDrawCanvas[whichCanv]->SetGridy(1);
        vecDrawCanvas[whichCanv]->SetLogx(doLog);
        vecDrawCanvas[whichCanv]->SetGridx(1);
    }
    void SetCanvasVec() {
        vecDrawCanvas.resize(2);
        vecNameCanvas.resize(2);
    }
    
    void DrawAdditionalThings(TPad * inputPad, TH1F * inputHistBackground, int numDiv, int whichCanv) {
        if (whichCanv > 0) {
            inputPad->cd();
            float rightMax = inputHistBackground->GetMaximum();
            Float_t scale = inputPad->GetUymax() / rightMax;
            inputHistBackground->Scale(scale);
            inputHistBackground->Draw("hist same");
            TGaxis * axis = new TGaxis(inputPad->GetUxmax(), inputPad->GetUymin(), inputPad->GetUxmax(), inputPad->GetUymax(), 0, rightMax, numDiv, "+L");
            axis->SetTitle("Number of Background Events Passing Cut");
            axis->SetTitleOffset(1.5);
            axis->SetLineColor(kRed);
            axis->SetLabelColor(kRed);
            axis->SetTextColor(kRed);
            axis->Draw();
        }
    }
    void DrawStringsOnCanvas(TCanvas * inputCanv, vPP * deltaM = NULL) {
        cout << "whichT2SMS " << whichT2SMS << endl;
        strLatexSignal = T2String(whichT2SMS, addtlParam);
        cout << "about to draw strings " << endl;
        DrawStrings(inputCanv, 1, strLatexSignal, deltaM);
        double xValStart = 0.583;
        double yValStart = 0.84;
        float size = 0.05;
        float stepSize = 0.07;
        TLatex textLatexCut; textLatexCut.SetNDC();
        textLatexCut.SetTextSize(size);
        inputCanv->cd();
        for (unsigned int iLatex = 0; iLatex < vecTLatexCuts.size(); ++iLatex) {
            textLatexCut.DrawLatex(xValStart, yValStart - iLatex * stepSize, vecTLatexCuts[iLatex]);
        }
    }
    
    void DrawOnCanvas(vector<TGraph *> * inputGraphVec, TH1F * inputHistBackground, int whichCanv, vPP * deltaM = NULL) {
        TPad * currPad = (TPad *) vecDrawCanvas[whichCanv]->cd(1);
        vecDrawCanvas[whichCanv]->cd();
        
        currPad->cd();
        inputGraphVec->at(0)->Draw("ACP");
        int numDiv = inputGraphVec->at(0)->GetHistogram()->GetYaxis()->GetNdivisions();
        currPad->Update();
        
        TAxis * yAxis = inputGraphVec->at(0)->GetYaxis();
        float absMin = 0;
        float absMax = 1.0;
        if (whichCanv) {
            float currMin, currMax;
            absMin = inputGraphVec->at(0)->GetHistogram()->GetMinimum();
            absMax = inputGraphVec->at(0)->GetHistogram()->GetMaximum();
            for (unsigned int iGraph = 1; iGraph < inputGraphVec->size(); ++iGraph) {
                currMin = inputGraphVec->at(iGraph)->GetHistogram()->GetMinimum();
                currMax = inputGraphVec->at(iGraph)->GetHistogram()->GetMaximum();
                absMin = TMath::Min(absMin, currMin);
                absMax = TMath::Max(absMax, currMax);
            }
            /*
            float maxHist = inputHistBackground->GetMaximum();
            double xZero, yZero;
            inputGraphVec->at(inputGraphVec->size() - 1)->GetPoint(1, xZero, yZero);
            inputHistBackground->Scale(yZero / maxHist);
            */
            TPad * overlay = new TPad("overlay","",0,0,1,1);
            overlay->SetFillStyle(4000);
            overlay->SetFillColor(0);
            overlay->SetFrameFillStyle(4000);
            overlay->Draw();
            overlay->cd();
            TH1F * hframe = overlay->DrawFrame(currPad->GetUxmin(), currPad->GetUymin(), currPad->GetUxmax(), currPad->GetUymax());
            hframe->GetXaxis()->SetLabelOffset(99);
            hframe->GetYaxis()->SetLabelOffset(99);
            //overlay->SetLogy();
            DrawAdditionalThings(overlay, inputHistBackground, numDiv, whichCanv);
        }
        else {
            float thresh = 2.5E-3;
            absMin = FirstPointDeltX(inputGraphVec->at(0), thresh);
        }
        
        currPad->cd();
        
        yAxis->SetTitleOffset(1.0);
        yAxis->SetRangeUser(absMin, absMax);
        
        for (unsigned int iGraph = 1; iGraph < inputGraphVec->size(); ++iGraph) {
            inputGraphVec->at(iGraph)->Draw("PC same");
        }
        cout << "about to draw strings in ROCDP" << endl;
        DrawStringsOnCanvas(vecDrawCanvas[whichCanv], deltaM);
        
        vecDrawCanvas[whichCanv]->Update();
        vecDrawCanvas[whichCanv]->Modified();
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
        yAxis->SetRangeUser(absMin, absMax * 1.5);
        
        for (unsigned int iGraph = 1; iGraph < inputGraphVec->size(); ++iGraph) {
            inputGraphVec->at(iGraph)->Draw("PC same");
        }
        cout << "about to draw strings in ROCDP" << endl;
        DrawStringsOnCanvas(inputCanv, deltaM);
        
        inputCanv->Update();
        inputCanv->Modified();
    }
    
    
    
    
    
    
    
    ///Setting up parameters
    void SetLegendStrings_SigCheck() {
        vecLegStrings.resize(0);
        SetLegendStringsROCCurves_DY(&vecLegStrings);
    }
    void SetLegendStrings_SigCheckMT2() {
        vecLegStrings.resize(0);
        vecLegStrings.push_back("T2tt");
        vecLegStrings.push_back("T2bw: x = 0.25");
        vecLegStrings.push_back("T2bw: x = 0.50");
        vecLegStrings.push_back("T2bw: x = 0.75");
    }
    void SetLegendStrings_WCheck() {
        vecLegStrings.resize(0);
        SetLegendStringsROCCurves_MT2lblb(&vecLegStrings);
    }
    void SetLegendStrings_dMCheck() {
        vecLegStrings.resize(0);
        
        int numDM = 4;
        vector<vPP> vecDeltaM(numDM);
        for (int iDM = 0; iDM < numDM; ++iDM) {
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
        cout << "fileName for T2 params " << fileName << endl;
        if (fileName.Contains("T2")) {
            if (!fileName.Contains("T2bw")) {
                whichT2SMS = 1;
                if (!fileName.Contains("T2tt")) {
                    whichT2SMS = 3;
                }
            }
            else {
                whichT2SMS = 0;
                if (fileName.Contains("75")) {
                    addtlParam = 75;
                }
                else if (fileName.Contains("50")) {
                    addtlParam = 50;
                }
                else if (fileName.Contains("25")) {
                    addtlParam = 25;
                }
            }
        }
        else {
            whichT2SMS = -1;
        }
    }
    void SetCanvName_SigCheck() {
        canvNameAppend += "DYKillerCompare";
    }
    void SetCanvName_SigCheckMT2() {
        canvNameAppend += "T2SMSCompare";
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




