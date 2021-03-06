// Structs used as part of displaying histograms -- fairly general set of structures!

using namespace std;
typedef vector<TH1F *> histProjVec;
typedef pair<int, int> intBounds;
typedef pair<TString, intBounds> indMCParams;


float GetNDCX(float x) {
    gPad->Update();//this is necessary!
    float NDCX = (x - gPad->GetX1())/(gPad->GetX2()-gPad->GetX1());
    return NDCX;
}

float GetNDCY(float y) {
    gPad->Update();//this is necessary!
    float NDCY = (y - gPad->GetY1())/(gPad->GetY2()-gPad->GetY1());
    return NDCY;
}


void DoOverflow(TH1 * inputHist) {
    int nBinsX = inputHist->GetNbinsX();
    int nBinsY = inputHist->GetNbinsY();
    int nBinsZ = inputHist->GetNbinsZ();
    
    float currBinContent, currBinContentToAddTo;
    
    int currBinToAddToX, currBinToAddToY, currBinToAddToZ;
    bool isOverflowX = false;
    bool isOverflowY = false;
    bool isOverflowZ = false;
    for (int iBinX = 0; iBinX <= nBinsX + 1; ++iBinX) {
        isOverflowX = false;
        currBinToAddToX = iBinX;
        if (currBinToAddToX == 0) {
            isOverflowX = true;
            currBinToAddToX = 1;
        }
        else if (currBinToAddToX > nBinsX) {
            isOverflowX = true;
            currBinToAddToX = nBinsX;
        }
        for (int iBinY = 0; iBinY <= nBinsY + 1; ++iBinY) {
            isOverflowY = false;
            currBinToAddToY = iBinY;
            if (currBinToAddToY == 0) {
                isOverflowY = true;
                currBinToAddToY = 1;
            }
            else if (currBinToAddToY > nBinsY) {
                isOverflowY = true;
                currBinToAddToY = nBinsY;
            }
            for (int iBinZ = 0; iBinZ <= nBinsZ + 1; ++iBinZ) {
                isOverflowZ = false;
                currBinToAddToZ = iBinZ;
                if (currBinToAddToZ == 0) {
                    isOverflowZ = true;
                    currBinToAddToZ = 1;
                }
                else if (currBinToAddToZ > nBinsZ) {
                    isOverflowZ = true;
                    currBinToAddToZ = nBinsZ;
                }
                if (!(isOverflowX || isOverflowY || isOverflowZ)) continue;
                currBinContent = inputHist->GetBinContent(iBinX, iBinY, iBinZ);
                currBinContentToAddTo = inputHist->GetBinContent(currBinToAddToX, currBinToAddToY, currBinToAddToZ);
                
                inputHist->SetBinContent(iBinX, iBinY, iBinZ, 0);
                inputHist->SetBinContent(currBinToAddToX, currBinToAddToY, currBinToAddToZ, currBinContentToAddTo + currBinContent);
            }
        }
    }
}



TString CorrGrabName(TString grabName, int sampleType) {
    TString outString = grabName;
    if ((sampleType == 0 || sampleType == 100) && outString.Contains("Smear")) {
        outString.Replace(outString.Index("Smear"), 5, "");
    }
    return outString;
}

typedef struct AxisProjInfo {
    int numDims, whichAxisToProjTo, whichAxisForDist, axisProjLB, axisProjUB;
    int whichAxisToMix;
    int axis1DLB, axis1DUB;
    TString nameBase;
    vector<int> vecAxesProjBounds;
    float nRMS;
    int RMSWidth;
    void SetUp3DSig(int inWAtPt, int whichBin) {
        whichAxisToProjTo = inWAtPt;
        vecAxesProjBounds.resize(4);
        for (int iBin = 0; iBin < 4; ++iBin) {
            vecAxesProjBounds[iBin] = whichBin;
        }
    }
    TString Get3DSigCanvNameAPI() {
        TString outString = "_3DSig_WAtPt";
        outString += whichAxisToProjTo;
        outString += "_ProjLB";
        outString += vecAxesProjBounds[0];
        outString += "_ProjUB";
        outString += vecAxesProjBounds[1];
        return outString;
    }
    TString GetCanvNameAPI() {
        TString outString = "numDims";
        outString += numDims;
        if (numDims > 1) {
        outString += "_WAtPt";
        outString += whichAxisToProjTo;
            outString += "_ProjAxis1LB";
            outString += vecAxesProjBounds[0];
            outString += "_ProjAxis1UB";
            outString += vecAxesProjBounds[1];
            if (numDims > 2) {
                outString += "_ProjAxis2LB";
                outString += vecAxesProjBounds[2];
                outString += "_ProjAxis2UB";
                outString += vecAxesProjBounds[3];
            }
        }
        return outString;
    }
    void DefaultVarVals() {
        numDims = 1;
        whichAxisToProjTo = 1;
        whichAxisForDist = 1;
        axis1DLB = 1;
        axis1DUB = -1;
        axisProjLB = 4;
        axisProjUB = 20;
        // For HLT paths defined above 1: 40 GeV, 2: 60 GeV, 3: 90 GeV, 4: 100 GeV, 5: 120 GeV, 6: 150 GeV, 7: 165 GeV, 8: 200 GeV, 9: 250 GeV, 10: 300 GeV, 11: 400 GeV, 12: 500 GeV, 13: 650 GeV, 14: 800 GeV, 15: 1000 GeV
        whichAxisToMix = 6 - (whichAxisForDist + whichAxisToProjTo);
        nRMS = -1;
        RMSWidth = 200;
    }
    void ReRangeHist(TH1F * inputHist) {
        if (axis1DLB > 1 || axis1DUB > - 1) {
            if (axis1DUB > - 1) {
                inputHist->GetXaxis()->SetRange(axis1DLB, axis1DUB);
                RebinnedHistUnderOverflow(inputHist, axis1DUB, 1);
            }
            else {
                inputHist->GetXaxis()->SetRange(axis1DLB, inputHist->GetNbinsX());
            }
            if (axis1DLB > 1) {
                RebinnedHistUnderOverflow(inputHist, axis1DLB, -1);
            }
        }
    }
    void SetVars(int inND, int inWAtPt, int inWAfD, int inAxProjLB, int inAxProjUB) {
        if (vecAxesProjBounds.size() == 0) vecAxesProjBounds.resize(4);
        numDims = inND;
        whichAxisToProjTo = inWAtPt;
        whichAxisForDist = inWAfD;
        axisProjLB = inAxProjLB;
        axisProjUB = inAxProjUB;
        whichAxisToMix = 6 - (whichAxisForDist + whichAxisToProjTo);
    }
    void SetAxesProjBounds_Agglomerate(int axis1LB, int axis1UB, int axis2LB, int axis2UB) {
        if (vecAxesProjBounds.size() == 0) vecAxesProjBounds.resize(4);
        vecAxesProjBounds[0] = axis1LB;
        vecAxesProjBounds[1] = axis1UB;
        vecAxesProjBounds[2] = axis2LB;
        vecAxesProjBounds[3] = axis2UB;
    }
    TH1F * DoProjection_Agglomerate(TH1 * inputHist, TString addName = "") {
        TH1F * patsy_1D;
        TH2F * patsy_2D;
        TH3F * patsy_3D;
        TString patsyName = inputHist->GetName();
        patsyName += addName;        
//        TString optProj = patsyName;
        TString optProj = "";
        switch (numDims) {
            case 1:
                patsyName += "_TH1FCast";
                patsy_1D = (TH1F *) inputHist->Clone(patsyName);
                break;
            case 2:
                patsy_2D = (TH2F *) inputHist->Clone(patsyName);                
                patsyName += "_TH1FCast";
                switch (whichAxisToProjTo) {
                    case 1:
                        patsy_1D = (TH1F *) patsy_2D->ProjectionX(patsyName, vecAxesProjBounds[0], vecAxesProjBounds[1]);
                        break;
                    case 2:                      
                        patsy_1D = (TH1F *) patsy_2D->ProjectionY(patsyName, vecAxesProjBounds[0], vecAxesProjBounds[1]);
                        break;
                    default:
                        cout << "something weird going on: whichAxisToProjTo " << whichAxisToProjTo << endl;
                        break; 
                }
                break;
            case 3:
                patsy_3D = (TH3F *) inputHist->Clone(patsyName);
                switch (whichAxisToProjTo) {
                    case 1:
//                        optProj += "_x";
                        optProj += "x";
                        patsy_3D->GetYaxis()->SetRange(vecAxesProjBounds[0], vecAxesProjBounds[1]);
                        patsy_3D->GetZaxis()->SetRange(vecAxesProjBounds[2], vecAxesProjBounds[3]);
                        break;
                    case 2:
//                        optProj += "_y";
                        optProj += "y";
                        patsy_3D->GetXaxis()->SetRange(vecAxesProjBounds[0], vecAxesProjBounds[1]);
                        patsy_3D->GetZaxis()->SetRange(vecAxesProjBounds[2], vecAxesProjBounds[3]);
                        break;
                    case 3:
//                        optProj += "_z";
                        optProj += "z";
                        patsy_3D->GetXaxis()->SetRange(vecAxesProjBounds[0], vecAxesProjBounds[1]);
                        patsy_3D->GetYaxis()->SetRange(vecAxesProjBounds[2], vecAxesProjBounds[3]);                    
                        break;
                    default:
                        break; 
                }
                /*
                cout << "whichAxisToProjTo " << whichAxisToProjTo << endl;
                cout << "optProj " << optProj << endl;
                */
                patsy_1D = (TH1F *) patsy_3D->Project3D(optProj);        
                break;
            default:
                cout << "some bugs, homie! pick correct num. of dims!" << endl;
                break;
        }
        if (numDims > 1) {
            if (EqualBinWidths(patsy_1D)) {
                patsy_1D->GetYaxis()->SetTitle("Events / NUM GeV");
            }
            else {
                patsy_1D->GetYaxis()->SetTitle("Events / GeV");
            }
        }
        patsy_1D->SetTitle("");
        return patsy_1D;
    }
    void SetAxesProjBounds_IndDist(int &currIndex) {
        vecAxesProjBounds[0] = (whichAxisToMix > whichAxisForDist) ? currIndex : axisProjLB;
        vecAxesProjBounds[1] = (whichAxisToMix > whichAxisForDist) ? currIndex : axisProjUB;
        vecAxesProjBounds[2] = (whichAxisToMix > whichAxisForDist) ? axisProjLB : currIndex;
        vecAxesProjBounds[3] = (whichAxisToMix > whichAxisForDist) ? axisProjUB : currIndex;        
    }
    TH1F * DoProjection_IndDist(TH1 * inputHist, int currIndex, TString patsyName, TString name) {
        TH1F * projHist;
        TH2F * projPatsy2DHist;
        TH3F * projPatsy3DHist;
        //        Option_t optProj = name;
        TString optProj = name;
        //        Option_t optProj = "t";
        SetAxesProjBounds_IndDist(currIndex);
        if (numDims > 2) {
            projPatsy3DHist = (TH3F *) inputHist->Clone(patsyName);
            switch (whichAxisToProjTo) {
                case 1:
                    optProj += "_x";
                    projPatsy3DHist->GetYaxis()->SetRange(vecAxesProjBounds[0], vecAxesProjBounds[1]);
                    projPatsy3DHist->GetZaxis()->SetRange(vecAxesProjBounds[2], vecAxesProjBounds[3]);
                    break;
                case 2:
                    optProj += "_y";
                    projPatsy3DHist->GetXaxis()->SetRange(vecAxesProjBounds[0], vecAxesProjBounds[1]);
                    projPatsy3DHist->GetZaxis()->SetRange(vecAxesProjBounds[2], vecAxesProjBounds[3]);
                    break;
                case 3:
                    optProj += "_z";
                    projPatsy3DHist->GetXaxis()->SetRange(vecAxesProjBounds[0], vecAxesProjBounds[1]);
                    projPatsy3DHist->GetYaxis()->SetRange(vecAxesProjBounds[2], vecAxesProjBounds[3]);                    
                    break;
                default:
                    break; 
            }
            projHist = (TH1F *) projPatsy3DHist->Project3D(optProj);
        }
        else {
            projPatsy2DHist = (TH2F *) inputHist->Clone(patsyName);
            switch (whichAxisToProjTo) {
                case 1:
                    projHist = (TH1F *) projPatsy2DHist->ProjectionX(name, vecAxesProjBounds[0], vecAxesProjBounds[1]);
                    break;
                case 2:                      
                    projHist = (TH1F *) projPatsy2DHist->ProjectionY(name, vecAxesProjBounds[0], vecAxesProjBounds[1]);
                    break;
                default:
                    cout << "something weird going on: whichAxisToProjTo " << whichAxisToProjTo << endl;
                    break; 
            }
        }
        if (numDims > 1) {
            if (EqualBinWidths(projHist)) {
                projHist->GetYaxis()->SetTitle("Events / NUM GeV");
            }
            else {
                projHist->GetYaxis()->SetTitle("Events / GeV");
            }
        }
        return projHist;
    }
    
    TH2F * ThreeDeeToTwoDeeProjection(TH3F * inputHist, TString setName, bool doVerb = 0) {
        //Function that takes an input 3D histogram and, in the 2D plane of the 2 axes not projected with, projects the remaining axis
        //and calculates some statistical quantity with this projection, the idea being that you can use this to help visualize 3D distributions
        //still a little bit of a work in progress
        int nBinsX3D = inputHist->GetNbinsX();
        int nBinsY3D = inputHist->GetNbinsY();
        int nBinsZ3D = inputHist->GetNbinsZ();
        
        inputHist->GetXaxis()->SetRange(1, nBinsX3D);
        inputHist->GetYaxis()->SetRange(1, nBinsY3D);
        inputHist->GetZaxis()->SetRange(1, nBinsZ3D);

        if (whichAxisToProjTo == 1) {
            inputHist->GetXaxis()->SetRange(vecAxesProjBounds[0], vecAxesProjBounds[1]);
        }
        else if (whichAxisToProjTo == 2) {
            inputHist->GetYaxis()->SetRange(vecAxesProjBounds[0], vecAxesProjBounds[1]);
        }
        else if (whichAxisToProjTo == 3) {
            inputHist->GetZaxis()->SetRange(vecAxesProjBounds[0], vecAxesProjBounds[1]);
        }
        
        TString optProjString = "p_";
        switch (whichAxisToProjTo) {
            case 1:
                optProjString += "zy";
                break;
            case 2:
                optProjString += "zx";
                break;
            case 3:
                optProjString += "yx";
                break;
            default:
                break;
        }
        if (doVerb) {
            cout << "inputHist name " << inputHist->GetName() << endl;
            cout << "optProjString " << optProjString << endl;
        }
        TH2F * outHist = (TH2F *) inputHist->Project3D(optProjString);
        outHist->SetName(setName);
        return outHist;
    }
    
    
    TString GetProjectionString(TH1 * inputHist, TString axisVar, int whichAxisInt) {
        TAxis * specAxis;
        
        int whichAxisToGrabStringFrom = -1;
        switch (whichAxisToProjTo) {
            case 1:
                whichAxisToGrabStringFrom = whichAxisInt + 1;
                break;
            case 2:
                whichAxisToGrabStringFrom = whichAxisInt == 2 ? 3 : 1;
                break;
            case 3:
                whichAxisToGrabStringFrom = whichAxisInt;
                break;
            default:
                break;
        }
        switch (whichAxisToGrabStringFrom) {
            case 1:
                specAxis = inputHist->GetXaxis();
                break;
            case 2:
                specAxis = inputHist->GetYaxis();
                break;
            case 3:
                specAxis = inputHist->GetZaxis();
                break;
            default:
                cout << "whichAxisIntOne must be 1, 2, or 3: it is " << whichAxisInt << endl;
                break;
        }
        int binLB = whichAxisInt == 1 ? vecAxesProjBounds[0] : vecAxesProjBounds[2];
        int binUB = whichAxisInt == 1 ? vecAxesProjBounds[1] : vecAxesProjBounds[3];
        cout << "for grabbing string," << endl;
        cout << "binLB " << binLB << endl;
        cout << "binUB " << binLB << endl;
        int axisLB = TMath::Nint(specAxis->GetBinLowEdge(binLB));
        int axisUB = TMath::Nint(specAxis->GetBinUpEdge(binUB));
        
        TString outString = "";
        
        if (binUB <= specAxis->GetNbins()) {
            outString += axisLB;
            outString += " GeV #leq ";
            outString += axisVar;
            outString += " #leq ";
            outString += axisUB;
            outString += " GeV";
        }
        else {
            outString += axisVar;
            outString += " #geq ";
            outString += axisLB;
            outString += " GeV";
        }
        return outString;
    }
} AxisProjInfo;

typedef struct HistDisplayParams {
    int RBNX;
    bool doOverflow, doUnderflow;
    TString addName;
    int whichIndMCSort;
    int typeBinLabel;
    TString xAxisName;
    void SetVals(int inRBNX = 1, bool inDoOverflow = false, bool inDoUnderflow = false, TString inAddName = "") {
        RBNX = inRBNX;
        doOverflow = inDoOverflow;
        doUnderflow = inDoUnderflow;
        addName = inAddName;
    }
    void DefaultVarVals() {
        RBNX = 1;
        doOverflow = true;
        doUnderflow = true;
        addName = "";
        whichIndMCSort = 1;
        typeBinLabel = -1;
	xAxisName = "";
    }
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        for (int iHistDispParam = 0; iHistDispParam < argc; ++iHistDispParam) {
            if (strncmp (argv[iHistDispParam],"makePlots_Paper", 15) == 0) {
                whichIndMCSort = strtol(argv[iHistDispParam + 1], NULL, 10 );
            }
            else if (strncmp (argv[iHistDispParam],"nRBNX_HDP", 9) == 0) {
                RBNX = strtol(argv[iHistDispParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iHistDispParam],"noOF_HDP", 8) == 0) {
                doOverflow = false;
            }
            else if (strncmp (argv[iHistDispParam],"noUF_HDP", 8) == 0) {
                doUnderflow = false;
            }
            else if (strncmp (argv[iHistDispParam],"xName", 5) == 0) {
	        xAxisName = TString(argv[iHistDispParam + 1]);
            }
            else if (strncmp (argv[iHistDispParam],"binLabelType", 12) == 0) {
                typeBinLabel = strtol(argv[iHistDispParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iHistDispParam],"addName_HDP", 11) == 0) {
                addName = TString(argv[iHistDispParam + 1]);
            }
            else if (strncmp (argv[iHistDispParam],"WhIM_HDP", 8) == 0) {
                whichIndMCSort = strtol(argv[iHistDispParam + 1], NULL, 10);
            }
        }
    }
} HistDisplayParams;

typedef struct IndSamplePlotInfo {
    TFile * inputFile;
    TString nameISPI;
    float weight_CentVal;
    float weight_CentVal_StatErr;
    vector<float> weight_SystVarUp;
    vector<float> weight_SystVarDown;
    TH1 * grabbedHist;
    TH1F * grabbedHist_TH1F;
    TString grabAppendName;
    TString sampAddName;
    vector<bool> vecShouldGrabSyst;
    vector<TH1 *> vecGrabbedHist_SystVarUp, vecGrabbedHist_SystVarDown;
    vector<TH1F *> vecGrabbedHist_SystVarUp_TH1F, vecGrabbedHist_SystVarDown_TH1F;
    histProjVec grabbedHistProjection;
    vector< histProjVec > vecGrabbedHistProjection_SystVarUp, vecGrabbedHistProjection_SystVarDown;
    
    int sampleType; // 0: Data, 1: TTbar, 2: ZDY, 3: EWK, 4: TTbar + Vec boson, 5: Other:, -1: Signal
    
    SampleSystematicsInfo indSSI;
    
    void ScaleHists(float scaleFactor) {
        grabbedHist_TH1F->Scale(scaleFactor);
        for (unsigned int iSyst = 0; iSyst < vecGrabbedHist_SystVarUp_TH1F.size(); ++iSyst) {
            vecGrabbedHist_SystVarUp_TH1F[iSyst]->Scale(scaleFactor);
            vecGrabbedHist_SystVarDown_TH1F[iSyst]->Scale(scaleFactor);
        }
    }
  void ChangeXAxisName(TString xAxisName) {
    grabbedHist_TH1F->GetXaxis()->SetTitle(xAxisName);
  }
    void ChangeAxisUnitVar(TString replaceString, TString strToReplace = "GeV") {
        TString xTitle = grabbedHist_TH1F->GetXaxis()->GetTitle();
        TString yTitle = grabbedHist_TH1F->GetYaxis()->GetTitle();

        int lengthStrToReplace = strToReplace.Length();
        int lengthReplaceString = replaceString.Length();
        
        xTitle.Replace(xTitle.Index(strToReplace), lengthStrToReplace, replaceString, lengthReplaceString);
        yTitle.Replace(yTitle.Index(strToReplace), lengthStrToReplace, replaceString, lengthReplaceString);
        
        grabbedHist_TH1F->GetXaxis()->SetTitle(xTitle);
        grabbedHist_TH1F->GetYaxis()->SetTitle(yTitle);
    }
    void DrawBinLabels1D(TPad * inputPad, float yAxisLowVal, TH1F * inputHist, int whichBinLabelType) {
        //whichBinLabelType: 0 is standard wide range GeV plot, 1 is object multiplicity, 2 is passes cut, 3 is deltaPhi
        inputPad->cd();
        
        if (whichBinLabelType == -1) return;
        TAxis * xAxis = inputHist->GetXaxis();
        xAxis->SetLabelOffset(1);
        inputPad->Update();
        inputPad->Modified();
        TAxis * yAxis = inputHist->GetYaxis();
        
        TLatex labelTex;
        //labelTex.SetNDC();
        float size = 0.15;
        labelTex.SetTextSize(size);
        float currNDCX, currNDCY;
        double currTextSize;
        
        int binLB = xAxis->GetFirst();
        int binUB = xAxis->GetLast();
        int binRange = binUB - binLB;
        
        int minBin = binLB;
        int maxBin = binUB;
        int binStep = 1;
        
        float bigBinThresh = 250;
        
        vector<int> vecModBins;
        
        
        int modFactorBigBin = 50;
        if (whichBinLabelType == 0) {
            if (xAxis->GetBinCenter(binUB) > bigBinThresh) {
                modFactorBigBin = 100;
            }
        }
        else if (whichBinLabelType == 1) {
            binStep = binRange > 4 ? 2 : 1;
            modFactorBigBin = 1;
        }
        else if (whichBinLabelType == 2) {
            xAxis->SetBinLabel(1, "Failed");
            xAxis->SetBinLabel(2, "Passed");
            
            labelTex.SetText(0, yAxisLowVal, "Failed");
            currTextSize = labelTex.GetXsize();
            labelTex.DrawLatex(0. - currTextSize/2, yAxisLowVal, "Failed");
            
            labelTex.SetText(1, yAxisLowVal, "Passed");
            currTextSize = labelTex.GetXsize();
            labelTex.DrawLatex(1. - currTextSize/2, yAxisLowVal, "Passed");
        }
        else if (whichBinLabelType == 3) {
	    float pi = 3.14159;
	    labelTex.DrawLatex(0., yAxisLowVal, "0");
	    labelTex.DrawLatex(pi/4, yAxisLowVal, "#frac{#pi}{4}");
	    labelTex.DrawLatex(pi/2, yAxisLowVal, "#frac{#pi}{2}");
	    labelTex.DrawLatex(pi * 0.75, yAxisLowVal, "#frac{3#pi}{4}");
	    labelTex.DrawLatex(pi, yAxisLowVal, "#pi");
	    labelTex.DrawLatex(0., yAxisLowVal, "0");
	    labelTex.DrawLatex(0., yAxisLowVal, "0");
	    labelTex.DrawLatex(0., yAxisLowVal, "0");
	  
            xAxis->SetBinLabel(xAxis->FindBin(0.), "0");
            xAxis->SetBinLabel(xAxis->FindBin(3.14159/4), "#frac{#pi}{4}");
            xAxis->SetBinLabel(xAxis->FindBin(3.14159/2), "#frac{#pi}{2}");
            xAxis->SetBinLabel(xAxis->FindBin(3.14159 * 0.75), "#frac{3#pi}{4}");
            xAxis->SetBinLabel(xAxis->FindBin(3.14159), "#pi");
        }
        else if ((whichBinLabelType / 10) % 4 == 0) {
            TString name = inputHist->GetName();
            if (whichBinLabelType == 40) {
                vecModBins.push_back(0);
                vecModBins.push_back(80);
                vecModBins.push_back(100);
                vecModBins.push_back(120);
                vecModBins.push_back(150);
            }
            else if (whichBinLabelType == 41) {
                vecModBins.push_back(0);
                vecModBins.push_back(80);
                vecModBins.push_back(130);
                vecModBins.push_back(170);
                vecModBins.push_back(210);
                vecModBins.push_back(250);
                vecModBins.push_back(300);
            }
            else if (whichBinLabelType == 42) {
                vecModBins.push_back(80);
                vecModBins.push_back(130);
                vecModBins.push_back(170);
                vecModBins.push_back(210);
                vecModBins.push_back(250);
                vecModBins.push_back(300);
            }
        }
        
        unsigned int numMod = vecModBins.size();
        
        int currBinValue;
        TString currBinLabel;
        if (whichBinLabelType < 2 || whichBinLabelType > 3) {
            for (int iBinX = minBin; iBinX <= maxBin; iBinX += binStep) {
                currBinValue = (int) (xAxis->GetBinLowEdge(iBinX) + 0.5);
                bool doBin = iBinX == minBin;
                
                if (whichBinLabelType < 2) {
                    doBin = iBinX == minBin || (currBinValue % modFactorBigBin == 0 && iBinX < maxBin - 1);
                }
                else {
                    for (unsigned int iMod = 0; iMod < numMod; ++iMod) {
//                        cout << "iMod " << iMod << endl;
//                        cout << "vecModBins[iMod] " << vecModBins[iMod] << endl;
//                        cout << "currBinValue / vecModBins[iMod] " << currBinValue / vecModBins[iMod] << endl;
                        if (currBinValue == vecModBins[iMod] && iBinX < maxBin - 1) doBin = true;
                    }
                }
                if (doBin) {
                    currBinLabel = "";
                    currBinLabel += currBinValue;
                    //currNDCX = GetNDCX((float) currBinValue);
                    //currNDCY = GetNDCY((float) yAxisLowVal);
                    labelTex.SetText(currBinValue, yAxisLowVal, currBinLabel);
                    currTextSize = labelTex.GetXsize();
                    labelTex.DrawLatex(currBinValue - currTextSize/2, yAxisLowVal, currBinLabel);
                    //labelTex.DrawLatex(currNDCX - size/2, currNDCY, currBinLabel);
                }
            }
//            currBinValue = (int) (xAxis->GetBinUpEdge(maxBin) + 0.5);
            currBinValue = (int) (xAxis->GetBinLowEdge(maxBin) + 0.5);
            cout << "currBinValue " << currBinValue << endl;
            currBinLabel = "";
            currBinLabel += "#geq ";
            currBinLabel += currBinValue;
            labelTex.SetText(currBinValue, yAxisLowVal, currBinLabel);
            currTextSize = labelTex.GetXsize();
            currBinValue = (int) (xAxis->GetBinLowEdge(maxBin) + 0.5);
            labelTex.DrawLatex(currBinValue - currTextSize/2, yAxisLowVal, currBinLabel);
//            currNDCX = GetNDCX((float) currBinValue);
//            currNDCY = GetNDCY((float) yAxisLowVal);
//            labelTex.DrawLatex(currNDCX - size/2, currNDCY, currBinLabel);
            //labelTex.DrawLatex(currBinValue, yAxisLowVal, currBinLabel);
        }
        gPad->Update();
        gPad->Modified();
    }
    void SetBinLabels1D(TH1F * inputHist, int whichBinLabelType) {
        //whichBinLabelType: 0 is standard wide range GeV plot, 1 is object multiplicity, 2 is passes cut, 3 is deltaPhi
        
        TAxis * xAxis = inputHist->GetXaxis();
        int binLB = xAxis->GetFirst();
        int binUB = xAxis->GetLast();
        int binRange = binUB - binLB;
        
        int minBin = binLB;
        int maxBin = binUB;
        int binStep = 1;
        
        float bigBinThresh = 250;
        
        
        int modFactorBigBin = 50;
        
        xAxis->SetLabelOffset(1);
        /*
        if (whichBinLabelType == 0) {
            if (xAxis->GetBinCenter(binUB) > bigBinThresh) {
                modFactorBigBin = 100;
            }
        }
        else if (whichBinLabelType == 1) {
            binStep = binRange > 4 ? 2 : 1;
            modFactorBigBin = 1;
        }
        else if (whichBinLabelType == 2) {
            xAxis->SetBinLabel(1, "Failed");
            xAxis->SetBinLabel(2, "Passed");
        }
        else if (whichBinLabelType == 3) {
            xAxis->SetBinLabel(xAxis->FindBin(0.), "0");
            xAxis->SetBinLabel(xAxis->FindBin(3.14159/4), "#frac{#pi}{4}");
            xAxis->SetBinLabel(xAxis->FindBin(3.14159/2), "#frac{#pi}{2}");
            xAxis->SetBinLabel(xAxis->FindBin(3.14159 * 0.75), "#frac{3#pi}{4}");
            xAxis->SetBinLabel(xAxis->FindBin(3.14159), "#pi");
        }
        
        int currBinValue;
        TString currBinLabel;
        if (whichBinLabelType < 2) {
            for (int iBinX = minBin; iBinX <= maxBin; iBinX += binStep) {
                currBinValue = (int) (xAxis->GetBinLowEdge(iBinX) + 0.5);
                if (iBinX == minBin || currBinValue % modFactorBigBin == 0) {
                    currBinLabel = "";
                    currBinLabel += currBinValue;
                    xAxis->SetBinLabel(iBinX, currBinLabel);
                }
            }
            currBinValue = (int) (xAxis->GetBinLowEdge(maxBin) + 0.5);
            currBinLabel = "";
            currBinLabel += "#geq ";
            currBinLabel += currBinValue;
            xAxis->SetBinLabel(maxBin, currBinLabel);
        }
        xAxis->LabelsOption("h");
         
         xAxis->SetLabelFont(62);
         xAxis->SetLabelSize(0.2);
         xAxis->SetLabelOffset(0.03);

         */
        if (whichBinLabelType == 2) {
            xAxis->SetBinLabel(1, "Failed");
            xAxis->SetBinLabel(2, "Passed");
            xAxis->LabelsOption("h");
            
            xAxis->SetLabelOffset(0.03);
        }
    }
    
    void SetBinLabels1D_AllHists(int whichBinLabelType) {
        SetBinLabels1D(grabbedHist_TH1F, whichBinLabelType);
        for (unsigned int iSyst = 0; iSyst < vecGrabbedHist_SystVarUp_TH1F.size(); ++iSyst) {
            SetBinLabels1D(vecGrabbedHist_SystVarUp_TH1F[iSyst], whichBinLabelType);
            SetBinLabels1D(vecGrabbedHist_SystVarDown_TH1F[iSyst], whichBinLabelType);
        }
    }
    void DefaultWeights(int numSystVars = 0) {
        weight_CentVal = 1.0;
        weight_CentVal_StatErr = 0.0;
        weight_SystVarUp.resize(numSystVars);
        weight_SystVarDown.resize(numSystVars);
        for (int iSyst = 0; iSyst < numSystVars; ++iSyst) {
            weight_SystVarUp[iSyst] = 1.0;
            weight_SystVarDown[iSyst] = 1.0;
        }
    }
    void PrintVals() {
        cout << "File name is " << inputFile->GetName() << endl;
        if (grabbedHist != NULL) cout << "current grabbed hist is " << grabbedHist->Integral() << endl;        
    }
    void PrintBinContent() {
        cout << "for ISPI: " << nameISPI << endl;
        PrintHistogram(grabbedHist, false, true);

        unsigned int numSysts = vecGrabbedHist_SystVarUp.size();
        
        for (unsigned int iSyst = 0; iSyst < numSysts; ++iSyst) {
            PrintHistogram(vecGrabbedHist_SystVarUp[iSyst], false, true, grabbedHist);
            PrintHistogram(vecGrabbedHist_SystVarDown[iSyst], false, true, grabbedHist);
        }
    }
    
    void ZeroOutNegBins() {
        ZeroOutNegHistBins(grabbedHist);
        for (unsigned int iSyst = 0; iSyst < vecGrabbedHist_SystVarUp.size(); ++iSyst) {
            ZeroOutNegHistBins(vecGrabbedHist_SystVarUp[iSyst]);
            ZeroOutNegHistBins(vecGrabbedHist_SystVarDown[iSyst]);
        }
    }
    void DivideHistogramBins(vector<TString> * inNamesToDivide, bool doTH1Fs = true) {
        bool doDivide = false;
        TString nameHist = grabbedHist->GetName();
        for (unsigned int iName = 0; iName < inNamesToDivide->size(); ++iName) {
            if (nameHist.Contains(inNamesToDivide->at(iName))) {
                doDivide = true;
            }
        }
        if (doDivide) {
            if (doTH1Fs) {
                HistDivBinContent(grabbedHist_TH1F);
                for (unsigned int iSyst = 0; iSyst < vecGrabbedHist_SystVarUp_TH1F.size(); ++iSyst) {
                    HistDivBinContent(vecGrabbedHist_SystVarUp_TH1F[iSyst]);
                    HistDivBinContent(vecGrabbedHist_SystVarDown_TH1F[iSyst]);
                }
            }
            else {
                HistDivBinContent(grabbedHist);
                for (unsigned int iSyst = 0; iSyst < vecGrabbedHist_SystVarUp_TH1F.size(); ++iSyst) {
                    HistDivBinContent(vecGrabbedHist_SystVarUp[iSyst]);
                    HistDivBinContent(vecGrabbedHist_SystVarDown[iSyst]);
                }
            }
        }
    }
    
    // Dealing with the name and sample type
    void SetSampleType(bool doVerbosity = false) {
        const int numVV = 7;
        TString VVNames[numVV] = {"WW", "WZ", "ZZ", "ZG", "WG", "VV", "VG"};
        bool isVV = false;
        if (nameISPI.Contains("Data")) {
            sampleType = 0;
        }
        else if (nameISPI.Contains("TTBar")) {
            if (nameISPI.Contains("VecBoson")) {
                sampleType = 4;
            }
            else {
                sampleType = 1;
            }
        }
        else if (nameISPI.Contains("ZDY")) {
            sampleType = 2;
        }
        else if (nameISPI.Contains("T2tt") || nameISPI.Contains("T2bw")) {
            sampleType = -1;
        }
        else if (nameISPI.Contains("FakeLep")) {
            sampleType = 100;
        }
        else {
            for (int iVV = 0; iVV < numVV; ++iVV) {
                if (nameISPI.EqualTo(VVNames[iVV])) isVV = true;
            }
            if (doVerbosity) cout << "nameISPI? " << nameISPI << "; contains a VV name " << isVV << endl;
            sampleType = isVV ? 3 : 5;
        }
    }
    
    void DefaultVarVals() {
        vecShouldGrabSyst.resize(0);
        vecGrabbedHist_SystVarUp.resize(0);
        vecGrabbedHist_SystVarDown.resize(0);
        vecGrabbedHist_SystVarUp_TH1F.resize(0);
        vecGrabbedHist_SystVarDown_TH1F.resize(0);
        vecGrabbedHistProjection_SystVarUp.resize(0);
        vecGrabbedHistProjection_SystVarDown.resize(0);
        indSSI.SetVecSizes(0);
    }
    
    void SetSystVecSizes(int size) {
        vecShouldGrabSyst.resize(size);
        vecGrabbedHist_SystVarUp.resize(size);
        vecGrabbedHist_SystVarDown.resize(size);
        vecGrabbedHist_SystVarUp_TH1F.resize(size);
        vecGrabbedHist_SystVarDown_TH1F.resize(size);
        vecGrabbedHistProjection_SystVarUp.resize(size);
        vecGrabbedHistProjection_SystVarDown.resize(size);
        indSSI.SetVecSizes(size);
    }
    void SetInputFile(TString fileName) {
        inputFile = TFile::Open(fileName);
    }
    void SetName(TString inName) {
        nameISPI = inName;
    }
    void SetSystHistAsClone(int iSyst, TString systName, bool isShape = false) {
        if (!isShape) {
            systName += "Shift";
        }
        vecGrabbedHist_SystVarUp[iSyst] = (TH1 *) grabbedHist->Clone(grabbedHist->GetName() + systName + TString("Up_Patsy_") + nameISPI);
        vecGrabbedHist_SystVarDown[iSyst] = (TH1 *) grabbedHist->Clone(grabbedHist->GetName() + systName + TString("Down_Patsy_") + nameISPI);
        
        if ((systName.Contains("TTBarNorm") && sampleType == 1) || (systName.Contains("DYNorm") && sampleType == 2) || (systName.Contains("EWKNorm") && sampleType == 3)) {
            /*
            cout << "going to do some scaling of " << nameISPI << " based upon the statError " << weight_CentVal_StatErr << endl;
            cout << "going to do some scaling of " << vecGrabbedHist_SystVarUp[iSyst]->GetName() << " with SF: " << 1 + weight_CentVal_StatErr << endl;
            cout << "going to do some scaling of " << vecGrabbedHist_SystVarDown[iSyst]->GetName() << " with SF: " << 1 - weight_CentVal_StatErr << endl;
             */
            vecGrabbedHist_SystVarUp[iSyst]->Scale(1 + weight_CentVal_StatErr);
            vecGrabbedHist_SystVarDown[iSyst]->Scale(1 - weight_CentVal_StatErr);
        }
        else {
            /*
            cout << "not going to scale " << vecGrabbedHist_SystVarUp[iSyst]->GetName() << " or " << vecGrabbedHist_SystVarDown[iSyst]->GetName() << endl;
             */
        }
    }
    void GrabHist(TString grabName, bool doVerbosity = false) {
        grabName = CorrGrabName(grabName, sampleType);
        if (doVerbosity) {
            cout << "for file " << inputFile->GetName() << endl;
            cout << "currently trying to grab histogram " << grabName << endl;
            if (inputFile->Get(grabName) != NULL) {
                cout << "histo is available to grab " << endl;
                cout << "weight that it will be scaled by " << weight_CentVal << endl;
            }
        }
        grabbedHist = (TH1 *) inputFile->Get(grabName);
        if (weight_CentVal != 1.0) {
            grabbedHist->Scale(weight_CentVal);
        }
        if (doVerbosity) {
            cout << "sanity check on bin 2 " << grabbedHist->GetBinContent(2) << endl;
        }
    }
    void GrabHist(vector<TString> * vecGrabNames, bool doVerbosity = false) {
        TH1 * currHist = 0;
        TString grabName = "";
        for (unsigned int iSamp = 0; iSamp < vecGrabNames->size(); ++iSamp) {
            grabName = CorrGrabName(vecGrabNames->at(iSamp), sampleType);
            if (doVerbosity) {
                cout << "for file " << inputFile->GetName() << endl;
                cout << "currently trying to grab histogram " << grabName << endl;
                if (inputFile->Get(grabName) != NULL) {
                    cout << "histo is available to grab " << endl;
                    cout << "weight that it will be scaled by " << weight_CentVal << endl;
                }
            }
            if (currHist == NULL) {
                currHist = (TH1 *) inputFile->Get(grabName);
            }
            else {
                currHist->Add((TH1 *) inputFile->Get(grabName));
            }
        }
        grabbedHist = currHist;
        if (weight_CentVal != 1.0) {
            grabbedHist->Scale(weight_CentVal);
        }
    }
    
    void GrabHistSystShape(TString baseHistGrabName, vector<TString> * vecSystNameAppends, TString addName = "", bool doVerbosity = false) {
        TString grabName;
        //would really like to make this function part of the other but stuck for now....
        for (unsigned int iSyst = 0; iSyst < vecSystNameAppends->size(); ++iSyst) {
            grabName = baseHistGrabName;
            grabName = CorrGrabName(grabName, sampleType);
            grabName += vecSystNameAppends->at(iSyst);
            if (vecShouldGrabSyst[iSyst]) {
                
                if (doVerbosity) {
                    cout << "for file " << inputFile->GetName() << endl;
                    cout << "for systematic " << vecSystNameAppends->at(iSyst) << endl;
                    cout << "currently trying to grab histogram " << grabName + TString("Up") + addName << endl;
                    if (inputFile->Get(grabName + TString("Up") + addName) != NULL) {
                        cout << "histo is available to grab " << endl;
                        cout << "weight ShiftUp " << weight_SystVarUp[iSyst] << endl;
                        cout << "weight ShiftDown " << weight_SystVarDown[iSyst] << endl;
                    }
                }
                
                vecGrabbedHist_SystVarUp[iSyst] = (TH1 *) inputFile->Get(grabName + TString("Up") + addName);
                vecGrabbedHist_SystVarDown[iSyst] = (TH1 *) inputFile->Get(grabName + TString("Down") + addName);
                if (weight_SystVarUp[iSyst] != 1.0) {
                    vecGrabbedHist_SystVarUp[iSyst]->Scale(weight_SystVarUp[iSyst]);
                }
                if (weight_SystVarDown[iSyst] != 1.0) {
                    vecGrabbedHist_SystVarDown[iSyst]->Scale(weight_SystVarDown[iSyst]);
                }
                /*
                 if (weight != 1.0) {
                 if (vecSystNameAppends->at(iSyst).Contains("genRecoilRW")) {
                 vecGrabbedHist_SystVarDown[iSyst]->Scale(weightRecoilDouble);
                 }
                 else {
                 vecGrabbedHist_SystVarDown[iSyst]->Scale(weight);
                 }
                 
                 }
                 */
            }
            else {
                if (doVerbosity) {
                    cout << "for file " << inputFile->GetName() << endl;
                    cout << "for systematic " << vecSystNameAppends->at(iSyst) << endl;
                    cout << "setting syst hists to central value " << endl;
                }
                SetSystHistAsClone(iSyst, vecSystNameAppends->at(iSyst), true);
            }
        }
    }
    
    
    
    void GrabHistSyst(TString baseHistGrabName, vector<TString> * vecSystNameAppends, bool doVerbosity = false, bool addShift = true) {
        TString grabName;
        for (unsigned int iSyst = 0; iSyst < vecSystNameAppends->size(); ++iSyst) {
            grabName = baseHistGrabName;
            grabName = CorrGrabName(grabName, sampleType);
            grabName += vecSystNameAppends->at(iSyst);
            if (addShift) {
                grabName += "Shift";
            }
            if (vecShouldGrabSyst[iSyst]) {
                
                if (doVerbosity) {
                    cout << "for file " << inputFile->GetName() << endl;
                    cout << "for systematic " << vecSystNameAppends->at(iSyst) << endl;
                    cout << "currently trying to grab histogram " << grabName << endl;
                    if (inputFile->Get(grabName + TString("Up")) != NULL) {
                        cout << "histo is available to grab " << endl;
                        cout << "weight ShiftUp " << weight_SystVarUp[iSyst] << endl;
                        cout << "weight ShiftDown " << weight_SystVarDown[iSyst] << endl;
                    }
                }
                
                vecGrabbedHist_SystVarUp[iSyst] = (TH1 *) inputFile->Get(grabName + TString("Up"));            
                vecGrabbedHist_SystVarDown[iSyst] = (TH1 *) inputFile->Get(grabName + TString("Down"));                
                if (weight_SystVarUp[iSyst] != 1.0) {
                    vecGrabbedHist_SystVarUp[iSyst]->Scale(weight_SystVarUp[iSyst]);
                }
                if (weight_SystVarDown[iSyst] != 1.0) {
                    vecGrabbedHist_SystVarDown[iSyst]->Scale(weight_SystVarDown[iSyst]);
                }
                    /*
                     if (weight != 1.0) {
                    if (vecSystNameAppends->at(iSyst).Contains("genRecoilRW")) {
                        vecGrabbedHist_SystVarDown[iSyst]->Scale(weightRecoilDouble);                        
                    }
                    else {
                        vecGrabbedHist_SystVarDown[iSyst]->Scale(weight);                        
                    }
                     
                     }
                     */
            }
            else {
                if (doVerbosity) {
                    cout << "for file " << inputFile->GetName() << endl;
                    cout << "for systematic " << vecSystNameAppends->at(iSyst) << endl;
                    cout << "setting syst hists to central value " << endl;
                }
                SetSystHistAsClone(iSyst, vecSystNameAppends->at(iSyst));
            }
        }
    }
    void GrabHistSyst(vector<TString> * vecBaseHistGrabNames, vector<TString> * vecSystNameAppends, bool doVerbosity = false, bool addShift = true) {
        TH1 * currHistSystUp, * currHistSystDown;
        TString grabName;
        for (unsigned int iSyst = 0; iSyst < vecSystNameAppends->size(); ++iSyst) {
            if (vecShouldGrabSyst[iSyst]) {
                currHistSystUp = 0;
                currHistSystDown = 0;
                for (unsigned int iSamp = 0; iSamp < vecBaseHistGrabNames->size(); ++iSamp) {
                    grabName = vecBaseHistGrabNames->at(iSamp);
                    grabName = CorrGrabName(grabName, sampleType);
                    grabName += vecSystNameAppends->at(iSyst);
                    if (addShift) {
                        grabName += "Shift";
                    }
                    
                    if (doVerbosity) {
                        cout << "for file " << inputFile->GetName() << endl;
                        cout << "for systematic " << vecSystNameAppends->at(iSyst) << endl;
                        cout << "currently trying to grab histogram " << grabName << endl;
                        if (inputFile->Get(grabName + TString("Up")) != NULL) {
                            cout << "histo is available to grab " << endl;
                            cout << "weight ShiftUp " << weight_SystVarUp[iSyst] << endl;
                            cout << "weight ShiftDown " << weight_SystVarDown[iSyst] << endl;
                        }
                    }
                    
                    if (currHistSystUp == NULL && currHistSystDown == NULL) {
                        currHistSystUp = (TH1 *) inputFile->Get(grabName + TString("Up"));
                        currHistSystDown = (TH1 *) inputFile->Get(grabName + TString("Down"));
                    }
                    else {
                        currHistSystUp->Add((TH1 *) inputFile->Get(grabName + TString("Up")));
                        currHistSystDown->Add((TH1 *) inputFile->Get(grabName + TString("Down")));
                    }
                }
                vecGrabbedHist_SystVarUp[iSyst] = currHistSystUp;
                vecGrabbedHist_SystVarDown[iSyst] = currHistSystDown;
                
                if (weight_SystVarUp[iSyst] != 1.0) {
                    vecGrabbedHist_SystVarUp[iSyst]->Scale(weight_SystVarUp[iSyst]);
                }
                if (weight_SystVarDown[iSyst] != 1.0) {
                    vecGrabbedHist_SystVarDown[iSyst]->Scale(weight_SystVarDown[iSyst]);
                }
                /*
                 if (weight != 1.0) {
                 if (vecSystNameAppends->at(iSyst).Contains("genRecoilRW")) {
                 vecGrabbedHist_SystVarDown[iSyst]->Scale(weightRecoilDouble);                        
                 }
                 else {
                 vecGrabbedHist_SystVarDown[iSyst]->Scale(weight);                        
                 }
                 
                 }
                 */
            }
            else {
                if (doVerbosity) {
                    cout << "for file " << inputFile->GetName() << endl;
                    cout << "for systematic " << vecSystNameAppends->at(iSyst) << endl;
                    cout << "setting syst hists to central value " << endl;
                }
                SetSystHistAsClone(iSyst, vecSystNameAppends->at(iSyst));
            }
        }
    }
    void SetVecSystSize(int size) {
        vecGrabbedHist_SystVarUp.resize(size);
        vecGrabbedHist_SystVarDown.resize(size);
        vecGrabbedHist_SystVarUp_TH1F.resize(size);
        vecGrabbedHist_SystVarDown_TH1F.resize(size);
        vecGrabbedHistProjection_SystVarUp.resize(size);
        vecGrabbedHistProjection_SystVarDown.resize(size);
        vecShouldGrabSyst.resize(size);
    }
    void CloneHists_NonTTBar(vector<IndSamplePlotInfo> * inVecISPI) {
        nameISPI = "NonTTBar";
        TString addName = "_";
        addName += nameISPI;
        grabbedHist = (TH1 *) inVecISPI->at(0).grabbedHist->Clone(inVecISPI->at(0).grabbedHist->GetName() + addName);
        grabbedHist_TH1F = (TH1F *) inVecISPI->at(0).grabbedHist_TH1F->Clone(inVecISPI->at(0).grabbedHist_TH1F->GetName() + addName);
        
        vecGrabbedHist_SystVarUp.resize(inVecISPI->at(0).vecGrabbedHist_SystVarUp.size());
        vecGrabbedHist_SystVarDown.resize(inVecISPI->at(0).vecGrabbedHist_SystVarDown.size());
        vecGrabbedHist_SystVarUp_TH1F.resize(inVecISPI->at(0).vecGrabbedHist_SystVarUp_TH1F.size());
        vecGrabbedHist_SystVarDown_TH1F.resize(inVecISPI->at(0).vecGrabbedHist_SystVarDown_TH1F.size());
        
        for (unsigned int iSyst = 0; iSyst < vecGrabbedHist_SystVarUp.size(); ++iSyst) {
            vecGrabbedHist_SystVarUp[iSyst] = (TH1 *) inVecISPI->at(0).vecGrabbedHist_SystVarUp[iSyst]->Clone(inVecISPI->at(0).vecGrabbedHist_SystVarUp[iSyst]->GetName() + addName);
            vecGrabbedHist_SystVarDown[iSyst] = (TH1 *) inVecISPI->at(0).vecGrabbedHist_SystVarDown[iSyst]->Clone(inVecISPI->at(0).vecGrabbedHist_SystVarDown[iSyst]->GetName() + addName);
            
            vecGrabbedHist_SystVarUp_TH1F[iSyst] = (TH1F *) inVecISPI->at(0).vecGrabbedHist_SystVarUp_TH1F[iSyst]->Clone(inVecISPI->at(0).vecGrabbedHist_SystVarUp_TH1F[iSyst]->GetName() + addName);
            vecGrabbedHist_SystVarDown_TH1F[iSyst] = (TH1F *) inVecISPI->at(0).vecGrabbedHist_SystVarDown_TH1F[iSyst]->Clone(inVecISPI->at(0).vecGrabbedHist_SystVarDown_TH1F[iSyst]->GetName() + addName);
        }
        
        for (unsigned int iVecIndMC = 1; iVecIndMC < inVecISPI->size(); ++iVecIndMC) {
            grabbedHist->Add(inVecISPI->at(iVecIndMC).grabbedHist);
            grabbedHist_TH1F->Add(inVecISPI->at(iVecIndMC).grabbedHist_TH1F);
            for (unsigned int iSyst = 0; iSyst < vecGrabbedHist_SystVarUp.size(); ++iSyst) {
                vecGrabbedHist_SystVarUp[iSyst]->Add(inVecISPI->at(iVecIndMC).vecGrabbedHist_SystVarUp[iSyst]);
                vecGrabbedHist_SystVarDown[iSyst]->Add(inVecISPI->at(iVecIndMC).vecGrabbedHist_SystVarDown[iSyst]);
                vecGrabbedHist_SystVarUp_TH1F[iSyst]->Add(inVecISPI->at(iVecIndMC).vecGrabbedHist_SystVarUp_TH1F[iSyst]);
                vecGrabbedHist_SystVarDown_TH1F[iSyst]->Add(inVecISPI->at(iVecIndMC).vecGrabbedHist_SystVarDown_TH1F[iSyst]);
            }
        }
    }
    void CloneHists(IndSamplePlotInfo * inISPI, bool setNameEqual = false, bool setSampTypeEqual = false, bool doVerbosity = false) {
        // function used as part of adding together ISPIs
        TString addName = "_";
        addName += nameISPI;
        
        // first equalize sizes of various vectors
        // basic systematics vec
        SetVecSystSize((int) inISPI->vecGrabbedHist_SystVarUp.size());
        
        // now add that ish
        if (doVerbosity) {
            cout << "name of grabbedHist for inISPI " << inISPI->nameISPI << endl;
            cout << inISPI->grabbedHist->GetName() << endl;
        }
        grabbedHist = (TH1 *) inISPI->grabbedHist->Clone(inISPI->grabbedHist->GetName() + addName);
        if (doVerbosity) {
            cout << "grabbedHist? " << grabbedHist << endl;
        }
        // projections aren't done before adding things together ISPIs
        if (doVerbosity) {
            if (vecGrabbedHist_SystVarUp.size() > 0) {                
                cout << "about to try cloning syst vars " << endl;
                cout << "vecGrabbedHist_SystVarUp.size() " << vecGrabbedHist_SystVarUp.size() << endl;
                cout << "inISPI->vecGrabbedHist_SystVarUp.size() " << inISPI->vecGrabbedHist_SystVarUp.size() << endl;
            }
        }
        for (unsigned int iSyst = 0; iSyst < vecGrabbedHist_SystVarUp.size(); ++iSyst) {
            if (doVerbosity) {
                cout << "iSyst " << iSyst << endl;
                cout << "inISPI->vecGrabbedHist_SystVarUp[iSyst] " << inISPI->vecGrabbedHist_SystVarUp[iSyst] << endl;
                cout << "inISPI->vecGrabbedHist_SystVarUp[iSyst]->GetName() " << inISPI->vecGrabbedHist_SystVarUp[iSyst]->GetName() << endl;
            }
            vecGrabbedHist_SystVarUp[iSyst] = (TH1 *) inISPI->vecGrabbedHist_SystVarUp[iSyst]->Clone(inISPI->vecGrabbedHist_SystVarUp[iSyst]->GetName() + addName);
            vecGrabbedHist_SystVarDown[iSyst] = (TH1 *) inISPI->vecGrabbedHist_SystVarDown[iSyst]->Clone(inISPI->vecGrabbedHist_SystVarDown[iSyst]->GetName() + addName);
        }
        
        if (setNameEqual) {
            if (doVerbosity) {
                cout << "setting name to be equal to inputISPI! " << inISPI->nameISPI << endl;
            }
            nameISPI = inISPI->nameISPI;
        }
        if (setSampTypeEqual) {
            if (doVerbosity) {
                cout << "setting sample type to be equal to inputISPI! " << inISPI->sampleType << endl;
            }
            sampleType = inISPI->sampleType;
        }
    }
    void AddHists(IndSamplePlotInfo * inISPI, bool checkSampleType = false, bool doVerbosity = false) {
        if (doVerbosity) cout << "about to add " << inISPI->grabbedHist->GetName() << " from " << inISPI->nameISPI << endl;
        grabbedHist->Add(inISPI->grabbedHist);
        if (doVerbosity) {
            if (vecGrabbedHist_SystVarUp.size() > 0) {
                cout << "going to do the same for the syst hists " << endl;
            }
        }
        for (unsigned int iSyst = 0; iSyst < vecGrabbedHist_SystVarUp.size(); ++iSyst) {
            vecGrabbedHist_SystVarUp[iSyst]->Add(inISPI->vecGrabbedHist_SystVarUp[iSyst]);
            vecGrabbedHist_SystVarDown[iSyst]->Add(inISPI->vecGrabbedHist_SystVarDown[iSyst]);
        }
        if (checkSampleType) {
            if (inISPI->sampleType == sampleType) {
                
            }
            else {
                if (doVerbosity)  {
                    cout << "sample types aren't the same -- one to add " << inISPI->sampleType << " and current one " << sampleType << endl;
                }
            }
        }
    }
    // stuff for histogram projection (for making final 1D output distributions)
    void SetTH1F_Projection(AxisProjInfo * inAPI, HistDisplayParams * inHDP, bool doVerbosity = false) {
        vector<TString> vecStringsToDivide;
        vecStringsToDivide.push_back("SignalShape");
        vecStringsToDivide.push_back("Coarse");
        
        grabbedHist_TH1F = inAPI->DoProjection_Agglomerate(grabbedHist, inHDP->addName);
        grabbedHist_TH1F->RebinX(inHDP->RBNX);
        HistogramUnderflowOverflow(grabbedHist_TH1F, inHDP->doUnderflow, inHDP->doOverflow);
        inAPI->ReRangeHist(grabbedHist_TH1F);
        for (unsigned int iSyst = 0; iSyst < vecGrabbedHist_SystVarUp.size(); ++iSyst) {
            vecGrabbedHist_SystVarUp_TH1F[iSyst] = inAPI->DoProjection_Agglomerate(vecGrabbedHist_SystVarUp[iSyst], inHDP->addName);
            vecGrabbedHist_SystVarUp_TH1F[iSyst]->RebinX(inHDP->RBNX);
            HistogramUnderflowOverflow(vecGrabbedHist_SystVarUp_TH1F[iSyst], inHDP->doUnderflow, inHDP->doOverflow);
            inAPI->ReRangeHist(vecGrabbedHist_SystVarUp_TH1F[iSyst]);
            
            vecGrabbedHist_SystVarDown_TH1F[iSyst] = inAPI->DoProjection_Agglomerate(vecGrabbedHist_SystVarDown[iSyst], inHDP->addName);
            vecGrabbedHist_SystVarDown_TH1F[iSyst]->RebinX(inHDP->RBNX);
            HistogramUnderflowOverflow(vecGrabbedHist_SystVarDown_TH1F[iSyst], inHDP->doUnderflow, inHDP->doOverflow);
            inAPI->ReRangeHist(vecGrabbedHist_SystVarDown_TH1F[iSyst]);
        }
        DivideHistogramBins(&vecStringsToDivide);
    }
    void SetTH1F_PassCut(vector<int> * vecCutValues, vector<TString> * vecCutVarNames, int whichIntType, vector<TString> * vecSystNameAppends, bool doSyst = true, int levelVerbosity = 0) {
        if (doSyst && vecSystNameAppends->size() != vecGrabbedHist_SystVarUp.size()) {
            cout << "something funky in that syst name and syst TH1 vecs don't have same size" << endl;
        }
        TString systName = "";
        if (levelVerbosity) {
            cout << "about to run the PassCutHisto! " << endl;
        }
        grabbedHist_TH1F = PassCutHisto(grabbedHist, vecCutValues, vecCutVarNames, nameISPI, whichIntType, systName, levelVerbosity);
        if (doSyst) {
            for (unsigned int iSyst = 0; iSyst < vecSystNameAppends->size(); ++iSyst) {
                systName = "";
                systName += vecSystNameAppends->at(iSyst);
                systName += "Shift";
                
                vecGrabbedHist_SystVarUp_TH1F[iSyst] = PassCutHisto(vecGrabbedHist_SystVarUp[iSyst], vecCutValues, vecCutVarNames, nameISPI, whichIntType, systName + TString("Up"), levelVerbosity);
                vecGrabbedHist_SystVarDown_TH1F[iSyst] = PassCutHisto(vecGrabbedHist_SystVarDown[iSyst], vecCutValues, vecCutVarNames, nameISPI, whichIntType, systName + TString("Down"), levelVerbosity);
            }
        }
    }
    // stuff for histogram projection (for mean/RMS)
    void SetVecGPHSize(int whichAxis) {
        int NBins;
        switch (whichAxis) {
            case 1:
                NBins = grabbedHist->GetNbinsX();
                break;
            case 2:
                NBins = grabbedHist->GetNbinsY();
                break;
            case 3:
                NBins = grabbedHist->GetNbinsZ();
                break;
        }
        grabbedHistProjection.resize(NBins);
        for (unsigned int iSyst = 0; iSyst < vecGrabbedHistProjection_SystVarUp.size(); ++iSyst) {            
            vecGrabbedHistProjection_SystVarUp[iSyst].resize(NBins);
            vecGrabbedHistProjection_SystVarDown[iSyst].resize(NBins);
        }
    }
    void SetOneDeeProjectionVec(AxisProjInfo * inAPI, TString systAddName = "") {
        SetVecGPHSize(inAPI->whichAxisForDist);
        TString name, patsyName;
        TString patsyNamebase = "patsy";
        TH1F * currProjHist1D;
        for (unsigned int iHist = 1; iHist < grabbedHistProjection.size(); ++iHist) {
            name = inAPI->nameBase;
            name += iHist;
            patsyName = patsyNamebase;
            patsyName += name;
            currProjHist1D = inAPI->DoProjection_IndDist(grabbedHist, iHist, patsyName, name);
            grabbedHistProjection[iHist-1] = currProjHist1D;
        }
    }
    void CalculateSysts(Color_t colorErrGraph, bool doAbsRatio, float fracRatioYAxisLB, float fracRatioYAxisUB, bool doSymErr, bool doFracRatio, bool doSmear, bool doStopXSec, bool doVerbosity = false) {
        if (doVerbosity) {
            cout << "sample type " << sampleType << endl;
        }
        indSSI.SetSystGraphs(grabbedHist_TH1F, &vecGrabbedHist_SystVarUp_TH1F, &vecGrabbedHist_SystVarDown_TH1F, colorErrGraph, doAbsRatio, fracRatioYAxisLB, fracRatioYAxisUB, doSymErr, doFracRatio, doSmear, sampleType == 3, doStopXSec, doVerbosity);
        
        if (doVerbosity) {
            cout << "quick sanity check " << endl;
            indSSI.PrintVec(0, doFracRatio);
        }
    }
    
    void CalculateSystsShapes(Color_t colorErrGraph, bool doAbsRatio, float fracRatioYAxisLB, float fracRatioYAxisUB, bool doSymErr, bool doFracRatio, bool doSmear, bool doStopXSec, bool doVerbosity = false) {
        if (doVerbosity) {
            cout << "sample type " << sampleType << endl;
        }
        indSSI.SetSystGraphsShapes(grabbedHist_TH1F, &vecGrabbedHist_SystVarUp_TH1F, &vecGrabbedHist_SystVarDown_TH1F, colorErrGraph, doAbsRatio, fracRatioYAxisLB, fracRatioYAxisUB, doSymErr, doFracRatio, doSmear, sampleType == 3, doStopXSec, doVerbosity);
        
        if (doVerbosity) {
            cout << "quick sanity check " << endl;
            indSSI.PrintVec(0, doFracRatio);
        }
    }
    
    ////////////////////
    /// yield functions
    void SetStatPlusSystErrorOnYields(int whichBin = 2, bool justStat = false, bool noSystPlusStat = true, bool doSymErr = false) {
        indSSI.SetStatPlusSystErrorYields(grabbedHist_TH1F, whichBin, nameISPI, justStat, noSystPlusStat, doSymErr);
    }
    void SetStatPlusSystErrorOnYields(TString inNameISPI, int whichBin = 2, bool justStat = false, bool noSystPlusStat = true, bool doSymErr = false) {
        indSSI.SetStatPlusSystErrorYields(grabbedHist_TH1F, whichBin, inNameISPI, justStat, noSystPlusStat, doSymErr);
    }
    void PrintOutYieldInfo(bool justStat = true, bool noSystPlusStat = true, int printSysLim = 0, bool printAveSys = false) {
        indSSI.PrintOut(justStat, noSystPlusStat, printSysLim, printAveSys);
    }
} IndSamplePlotInfo;

typedef struct DisplayStyle {
    Color_t color;
    Style_t style;
    Width_t width;
    Size_t  size;
    void SetParams(Color_t inColor, Style_t inStyle, Width_t inWidth, Size_t inSize) {
        color = inColor;
        style = inStyle;
        width = inWidth;
        size  = inSize;
    }
    void SetParams() {
        
    }
} DisplayStyle;

typedef struct IndSampleDisplayParams {
    TString legendEntry;
    DisplayStyle fillDS, lineDS, markerDS;
    int     sortParam; //fixed at 0 for comps, custom set for ind MCs
    void SetParams(TString inLE, DisplayStyle inFillDS, DisplayStyle inLineDS, DisplayStyle inMarkerDS, int inSP = 0) {
        legendEntry = inLE;
        fillDS = inFillDS;
        lineDS = inLineDS;
        markerDS = inMarkerDS;
        sortParam = inSP;
        /*
        cout << "legendEntry " << legendEntry << endl;
        cout << "sortParam " << sortParam << endl;
         */
    }
} IndSampleDisplayParams;
typedef pair<IndSamplePlotInfo, IndSampleDisplayParams> SampDisplay;
#include "./StopPlotSetup_ISPIFunctions_Set1.h"



#include "./StopPlotSetup_ISDPFunctions.h"

typedef struct HistogramDisplayStructs {
    vector<IndSamplePlotInfo> vecISPI_asLoaded;
    SampDisplay compSamp;
    vector<SampDisplay> vecSampDisplay_IndMC; //c.f. typedef at top of header file
    
    SampDisplay nonTTBarSD;
    
    bool doOffset;
    

  void ChangeXAxisName(TString xAxisName) {
    compSamp.first.ChangeXAxisName(xAxisName);
  }

    void PrintCompSystHistNames() {
        for (unsigned int iSyst = 0; iSyst < compSamp.first.vecGrabbedHist_SystVarUp_TH1F.size(); ++iSyst) {
            cout << "upSyst name " << compSamp.first.vecGrabbedHist_SystVarUp_TH1F[iSyst]->GetName() << endl;
            cout << "downSyst name " << compSamp.first.vecGrabbedHist_SystVarDown_TH1F[iSyst]->GetName() << endl;
        }
    }
    void DefaultVarVals() {
        doOffset = false;
        vecISPI_asLoaded.resize(0);
        vecSampDisplay_IndMC.resize(0);        
    }
    
    void ScaleISPIs(float scaleFactor) {
        compSamp.first.ScaleHists(scaleFactor);
        for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
            vecSampDisplay_IndMC[iIndMC].first.ScaleHists(scaleFactor);
        }
    }
    
    void ChangeAxisUnitVar(TString replaceString, TString strToReplace = "GeV") {
        compSamp.first.ChangeAxisUnitVar(replaceString, strToReplace);
        for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
            vecSampDisplay_IndMC[iIndMC].first.ChangeAxisUnitVar(replaceString, strToReplace);
        }
    }
    
    void SetBinLabels1D_AllHists(int whichBinLabelType) {
        compSamp.first.SetBinLabels1D_AllHists(whichBinLabelType);
    }
    void DoOverflowAllHists() {
        DoOverflow(compSamp.first.grabbedHist);
        for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
            DoOverflow(vecSampDisplay_IndMC[iIndMC].first.grabbedHist);
            for (unsigned int iSyst = 0; iSyst < vecSampDisplay_IndMC[iIndMC].first.vecGrabbedHist_SystVarUp.size(); ++iSyst) {
                DoOverflow(vecSampDisplay_IndMC[iIndMC].first.vecGrabbedHist_SystVarUp[iSyst]);
                DoOverflow(vecSampDisplay_IndMC[iIndMC].first.vecGrabbedHist_SystVarDown[iSyst]);
            }
        }
    }
    
    void WriteToFile(vector<TFile *> * vecOutFiles, vector<indMCParams> * inVecIndMCParams, vector<TString> * inVecSystNames, vector<int> * vecIndexMC = 0, bool doVerbosity = 0) {
        //if vecIndexMC is non-existent, vecOutFiles should be size 1
        if (!vecIndexMC) {
            vecOutFiles->at(0)->cd();
            TString nameHist = compSamp.first.grabbedHist->GetName();
            cout << "nameHist " << nameHist << endl;
            if (nameHist.Contains("Data")) {
                compSamp.first.grabbedHist->SetName("data_obs");
            }
            else {
                compSamp.first.grabbedHist->SetName("Signal");
            }
            compSamp.first.grabbedHist->Write();
            compSamp.first.PrintBinContent();
        }
        else {
            //Write NonTTBar Combined dude
            
            
            TString nonTTBarTrimName = "NonTTBar";
            unsigned int indexFirstNonTTBar = 1;
            
            vecOutFiles->at(vecIndexMC->at(indexFirstNonTTBar))->cd();
            TH1 * nonTTBarComboHist = (TH1 *)  vecSampDisplay_IndMC[indexFirstNonTTBar].first.grabbedHist->Clone(nonTTBarTrimName);

            vector<TH1 *> vecNonTTBarComboHist_SystVarUp(vecSampDisplay_IndMC[indexFirstNonTTBar].first.vecGrabbedHist_SystVarUp.size());
            vector<TH1 *> vecNonTTBarComboHist_SystVarDown(vecSampDisplay_IndMC[indexFirstNonTTBar].first.vecGrabbedHist_SystVarDown.size());
            
            for (unsigned int iSyst = 0; iSyst < vecNonTTBarComboHist_SystVarUp.size(); ++iSyst) {
                TString systName = inVecSystNames->at(iSyst);
                if (!systName.Contains("_")) {
                    systName = "_";
                    systName += inVecSystNames->at(iSyst);
                }
                vecNonTTBarComboHist_SystVarUp[iSyst] = (TH1 *) vecSampDisplay_IndMC[indexFirstNonTTBar].first.vecGrabbedHist_SystVarUp[iSyst]->Clone(nonTTBarTrimName + systName + TString("Up"));
                vecNonTTBarComboHist_SystVarDown[iSyst] = (TH1 *) vecSampDisplay_IndMC[indexFirstNonTTBar].first.vecGrabbedHist_SystVarDown[iSyst]->Clone(nonTTBarTrimName + systName + TString("Down"));
            }
            
            for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
                TString trimName = inVecIndMCParams->at(iIndMC).first;
                trimName.Replace(trimName.Index("_"), 1, "");
                vecOutFiles->at(vecIndexMC->at(iIndMC))->cd();
                
                if (iIndMC > indexFirstNonTTBar) {
                    nonTTBarComboHist->Add(vecSampDisplay_IndMC[iIndMC].first.grabbedHist);
                }
                
                vecSampDisplay_IndMC[iIndMC].first.grabbedHist->SetName(trimName);
                vecSampDisplay_IndMC[iIndMC].first.grabbedHist->Write();
                vecSampDisplay_IndMC[iIndMC].first.PrintBinContent();
                
                TH1 * indMCHist_MCStatUp  = (TH1 *) vecSampDisplay_IndMC[iIndMC].first.grabbedHist->Clone(vecSampDisplay_IndMC[iIndMC].first.grabbedHist->GetName() + TString("_") + trimName + TString("MCStatUp"));
                SetMCStatSystHist(indMCHist_MCStatUp, 1, doVerbosity);
                cout << "indMCHist_MCStatUp name " << indMCHist_MCStatUp->GetName() << endl;
                //indMCHist_MCStatUp->Write();
                TH1 * indMCHist_MCStatDown  = (TH1 *) vecSampDisplay_IndMC[iIndMC].first.grabbedHist->Clone(vecSampDisplay_IndMC[iIndMC].first.grabbedHist->GetName() + TString("_") + trimName + TString("MCStatDown"));
                SetMCStatSystHist(indMCHist_MCStatDown, -1, doVerbosity);
                cout << "indMCHist_MCStatDown name " << indMCHist_MCStatDown->GetName() << endl;
                //indMCHist_MCStatDown->Write();
                
                for (unsigned int iSyst = 0; iSyst < vecSampDisplay_IndMC[iIndMC].first.vecGrabbedHist_SystVarUp.size(); ++iSyst) {
                    TString systName = inVecSystNames->at(iSyst);
                    if (!systName.Contains("_")) {
                        systName = "_";
                        systName += inVecSystNames->at(iSyst);
                    }
                    cout << "writing " << systName << endl;
                    //systName.Replace(systName.Index("_"), 1, "");
                    vecSampDisplay_IndMC[iIndMC].first.vecGrabbedHist_SystVarUp[iSyst]->SetName(trimName + systName + TString("Up"));
                    vecSampDisplay_IndMC[iIndMC].first.vecGrabbedHist_SystVarUp[iSyst]->Write();
                    
                    vecSampDisplay_IndMC[iIndMC].first.vecGrabbedHist_SystVarDown[iSyst]->SetName(trimName + systName + TString("Down"));
                    vecSampDisplay_IndMC[iIndMC].first.vecGrabbedHist_SystVarDown[iSyst]->Write();
                    
                    if (iIndMC > indexFirstNonTTBar) {
                        vecNonTTBarComboHist_SystVarUp[iSyst]->Add(vecSampDisplay_IndMC[iIndMC].first.vecGrabbedHist_SystVarUp[iSyst]);
                        vecNonTTBarComboHist_SystVarDown[iSyst]->Add(vecSampDisplay_IndMC[iIndMC].first.vecGrabbedHist_SystVarDown[iSyst]);
                    }
                }
            }
            
            TH1 * nonTTBarComboHist_MCStatUp = (TH1 *) nonTTBarComboHist->Clone(nonTTBarTrimName + TString("_") + nonTTBarTrimName + TString("MCStatUp"));
            SetMCStatSystHist(nonTTBarComboHist_MCStatUp, 1, doVerbosity);
            TH1 * nonTTBarComboHist_MCStatDown = (TH1 *) nonTTBarComboHist->Clone(nonTTBarTrimName + TString("_") + nonTTBarTrimName + TString("MCStatDown"));
            SetMCStatSystHist(nonTTBarComboHist_MCStatDown, -1, doVerbosity);
            
            nonTTBarComboHist->Write();
            nonTTBarComboHist_MCStatUp->Write();
            nonTTBarComboHist_MCStatDown->Write();
            
            PrintHistogram(nonTTBarComboHist, false, true);
            
            for (unsigned int iSyst = 0; iSyst < vecNonTTBarComboHist_SystVarUp.size(); ++iSyst) {
                vecNonTTBarComboHist_SystVarUp[iSyst]->Write();
                vecNonTTBarComboHist_SystVarDown[iSyst]->Write();
                
                PrintHistogram(vecNonTTBarComboHist_SystVarUp[iSyst], false, true, nonTTBarComboHist);
                PrintHistogram(vecNonTTBarComboHist_SystVarDown[iSyst], false, true, nonTTBarComboHist);
            }
        }
    }
    void ZeroOutNegBins() {
        for (unsigned int iISPI = 0; iISPI < vecISPI_asLoaded.size(); ++iISPI) {
            vecISPI_asLoaded[iISPI].ZeroOutNegBins();
        }
    }
    void SetSystSize(int size = 0, bool doSyst = false) {
        if (doSyst) {
            for (unsigned int iISPI = 0; iISPI < vecISPI_asLoaded.size(); ++iISPI) {
                vecISPI_asLoaded[iISPI].SetSystVecSizes(size);                
            }
            compSamp.first.SetSystVecSizes(size);
            for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
                vecSampDisplay_IndMC[iIndMC].first.SetSystVecSizes(size);
            }
        }
        else {
            for (unsigned int iISPI = 0; iISPI < vecISPI_asLoaded.size(); ++iISPI) {
                vecISPI_asLoaded[iISPI].SetSystVecSizes(0);                
            }
            compSamp.first.SetSystVecSizes(size);
            for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
                vecSampDisplay_IndMC[iIndMC].first.SetSystVecSizes(0);
            }
        }
    }
    void SetBoolSysts(int iSyst, int iISPI, bool grabSyst) {
        vecISPI_asLoaded[iISPI].vecShouldGrabSyst[iSyst] = grabSyst;
    }
    void GrabCentralValues(TString grabName, bool doVerbosity = false) {
        for (unsigned int iISPI = 0; iISPI < vecISPI_asLoaded.size(); ++iISPI) {
            vecISPI_asLoaded[iISPI].GrabHist(grabName, doVerbosity);
        }
    }
    void GrabCentralValues(vector<TString> * vecGrabNames, bool doVerbosity = false) {
        for (unsigned int iISPI = 0; iISPI < vecISPI_asLoaded.size(); ++iISPI) {
            vecISPI_asLoaded[iISPI].GrabHist(vecGrabNames, doVerbosity);
        }
    }
    void GrabSystValues(TString baseHistGrabName, vector<TString> * vecSystNameAppends, bool doVerbosity = false, bool addShift = true) {
        for (unsigned int iISPI = 0; iISPI < vecISPI_asLoaded.size(); ++iISPI) {
            vecISPI_asLoaded[iISPI].GrabHistSyst(baseHistGrabName, vecSystNameAppends, doVerbosity, addShift);
        }
    }
    void GrabSystValues(vector<TString> * vecBaseHistGrabNames, vector<TString> * vecSystNameAppends, bool doVerbosity = false, bool addShift = true) {
        for (unsigned int iISPI = 0; iISPI < vecISPI_asLoaded.size(); ++iISPI) {
            vecISPI_asLoaded[iISPI].GrabHistSyst(vecBaseHistGrabNames, vecSystNameAppends, doVerbosity, addShift);
        }
    }
    void DoProjection(vector<indMCParams> * vecIndMCParams, AxisProjInfo * inAPI, HistDisplayParams * inHDP, TString compName, bool doVerbosity = false) {
        bool doProj = true;
        if (doVerbosity) {
            cout << "vecSampDisplay_IndMC size " << vecSampDisplay_IndMC.size() << endl;
        }
        if (vecSampDisplay_IndMC.size() > 0) {
            HistogramAdderProjector(inAPI, inHDP, &vecISPI_asLoaded, &compSamp.first, compName, doProj, doVerbosity, &vecSampDisplay_IndMC, vecIndMCParams);
            for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
                if (doVerbosity) {
                    cout << "setting parameters of individual hists " << endl;
                    cout << "iIndMC " << iIndMC << endl;
                }
                HistMainAttSet(vecSampDisplay_IndMC[iIndMC].first.grabbedHist_TH1F, &vecSampDisplay_IndMC[iIndMC].second);
            }
            SortIndMC(!inHDP->whichIndMCSort);
        }
        else {
            HistogramAdderProjector(inAPI, inHDP, &vecISPI_asLoaded, &compSamp.first, compName, doProj, doVerbosity); 
        }
        HistMainAttSet(compSamp.first.grabbedHist_TH1F, &compSamp.second);
    }
    ////////////////////////////////////////////////////////////////////////
    /// Functions related to yield calculations ///
    void SetUpHistosForSaving(TString compName, vector<indMCParams> * vecIndMCParams, int levelVerbosity = 0) {
        if (levelVerbosity) {
            cout << "about to Set up the histos for saving " << endl;
        }
        if (vecSampDisplay_IndMC.size() > 0) {
            HistogramAdder(&vecISPI_asLoaded, &compSamp.first, compName, levelVerbosity, &vecSampDisplay_IndMC, vecIndMCParams);
        }
        else {
            HistogramAdder(&vecISPI_asLoaded, &compSamp.first, compName, levelVerbosity);
        }
    }
    void DoPassCutHisto(vector<int> * vecCutValues, vector<TString> * vecCutVarNames, int whichIntType, TString compName, vector<indMCParams> * vecIndMCParams, vector<TString> * vecSystNameAppends, bool doSyst = true, int levelVerbosity = 0) {
        if (levelVerbosity) {
            cout << "about to run SetTH1F_PassCut on compSamp" << endl;
        }
        if (vecSampDisplay_IndMC.size() > 0) {
            HistogramAdder(&vecISPI_asLoaded, &compSamp.first, compName, levelVerbosity, &vecSampDisplay_IndMC, vecIndMCParams);
        }
        else {
            HistogramAdder(&vecISPI_asLoaded, &compSamp.first, compName, levelVerbosity);
        }
        compSamp.first.SetTH1F_PassCut(vecCutValues, vecCutVarNames, whichIntType, vecSystNameAppends, doSyst, levelVerbosity);
        for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
            if (levelVerbosity) {
                cout << "about to run SetTH1F_PassCut on vecSampDisplay_IndMC[iIndMC] for iIndMC = " << iIndMC << endl;
            }
            vecSampDisplay_IndMC[iIndMC].first.SetTH1F_PassCut(vecCutValues, vecCutVarNames, whichIntType, vecSystNameAppends, doSyst, levelVerbosity);
        }
    }
    void SetSSI_YieldErrors(bool doIndMC = false, int whichBin = 2, bool justStat = false, bool noSystPlusStat = true, bool doSymErr = false) {
        compSamp.first.SetStatPlusSystErrorOnYields(whichBin, justStat, noSystPlusStat, doSymErr);
        if (doIndMC) {
            for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
                //                vecSampDisplay_IndMC[iIndMC].first.SetStatPlusSystErrorOnYields(vecSampDisplay_IndMC[iIndMC].second.legendEntry, whichBin, justStat, noSystPlusStat, doSymErr);
                vecSampDisplay_IndMC[iIndMC].first.SetStatPlusSystErrorOnYields(whichBin, justStat, noSystPlusStat, doSymErr);
            }
            nonTTBarSD.first.SetStatPlusSystErrorOnYields(whichBin, justStat, noSystPlusStat, doSymErr);
        }
    }
    void PrintSSI_YieldInfo(bool doIndMC = false, bool justStat = true, bool noSystPlusStat = true, int printSysLim = 0, bool printAveSys = false) {
        compSamp.first.PrintOutYieldInfo(justStat, noSystPlusStat, printSysLim, printAveSys);
        
        bool hasTTBar = false;
        
        if (doIndMC) {
            for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
                if (vecSampDisplay_IndMC[iIndMC].first.nameISPI.Contains("TTBar")) hasTTBar = true;
                vecSampDisplay_IndMC[iIndMC].first.PrintOutYieldInfo(justStat, noSystPlusStat, printSysLim, printAveSys);
            }
            nonTTBarSD.first.PrintOutYieldInfo(justStat, noSystPlusStat, printSysLim, printAveSys);
        }
        
        if (hasTTBar) {
            SampleSystematicsInfo MCCompMinTTBar = SSISubtraction(&compSamp.first.indSSI, &vecSampDisplay_IndMC[0].first.indSSI, "TTBar");
            MCCompMinTTBar.PrintOut(justStat, noSystPlusStat, printSysLim, printAveSys);
        }
    }
    ////////////////////////////////////////////////////////////////////////
    void SetCompParams(TString compString, int HistoType = 0) {
        //HistoType: -1 is signal, 0 is data, 1 is MCComp
        DisplayStyle fillDS, lineDS, markerDS;
        switch (HistoType) {
            case -1:
                //Color_t signalColors[6] = {kMagenta, kMagenta + 1, kMagenta + 2, kViolet + 1, kViolet + 5, kViolet + 10};
                //    Style_t signalStyles[6] = {1, 2, 4, 8, 6, 10};
                fillDS.SetParams(kWhite, 0, -1, -1);
                lineDS.SetParams(kCyan, 1, 2, -1);
                markerDS.SetParams(kWhite, 0, -1, 0);
                break;
            case 0:
                fillDS.SetParams(kWhite, 0, -1, -1);
                lineDS.SetParams(kBlack, 1, 2, -1);
                markerDS.SetParams(kBlack, 20, -1, 0.9);
                break;
            case 1:
                fillDS.SetParams(kWhite, 0, -1, -1);
                lineDS.SetParams(kWhite, 1, 2, -1);
                markerDS.SetParams(kWhite, 20, -1, 0);
                break;
        }
        compSamp.second.SetParams(compString, fillDS, lineDS, markerDS, 0);
    }
    void SetIndMCParams(vector<TString> * inVecMCLegEntries, vector<Color_t> * inVecMCColors, vector<int> * inVecMCSortParams) {
        DisplayStyle fillDS, lineDS, markerDS;
        
        Style_t IndMCFillStyle = 1001;
        Width_t IndMCFillWidth = -1;
        Size_t IndMCFillSize = -1;
        
        Style_t IndMCLineStyle = 1;
        Width_t IndMCLineWidth = 2;
        Size_t IndMCLineSize = -1;
        markerDS.SetParams(kWhite, 20, -1, 0); // not using markerDS for individual MC
        
        unsigned int numIndMC = inVecMCLegEntries->size();
        vecSampDisplay_IndMC.resize(numIndMC);
        for (unsigned int iIndMC = 0; iIndMC < numIndMC; ++iIndMC) {
            fillDS.SetParams(inVecMCColors->at(iIndMC), IndMCFillStyle, IndMCFillWidth, IndMCFillSize);
            lineDS.SetParams(inVecMCColors->at(iIndMC), IndMCLineStyle, IndMCLineWidth, IndMCLineSize);
            vecSampDisplay_IndMC[iIndMC].second.SetParams(inVecMCLegEntries->at(iIndMC), fillDS, lineDS, markerDS, inVecMCSortParams->at(iIndMC));
        }
    }
    void SortIndMC(bool sortByInt = false) {
        if (sortByInt) {
            sort(vecSampDisplay_IndMC.begin(), vecSampDisplay_IndMC.end(), greater<SampDisplay>());
            for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
                cout << "Integral for sample: " << vecSampDisplay_IndMC[iIndMC].first.nameISPI << " is " << vecSampDisplay_IndMC[iIndMC].first.grabbedHist_TH1F->Integral() << endl;
            }
        }
        else {
            sort(vecSampDisplay_IndMC.begin(), vecSampDisplay_IndMC.end(), less<SampDisplay>());   
        }
    }
    
    void CalculateSystsComp(Color_t colorErrGraph, bool doAbsRatio, float fracRatioYAxisLB, float fracRatioYAxisUB, bool doSymErr, bool doSmear, bool doStopXSec, bool doVerbosity = false) {
        bool doFracRatio = true;
        if (doVerbosity) {
            cout << "about to calculate systs in compSamp " << endl;
        }
        compSamp.first.CalculateSysts(colorErrGraph, doAbsRatio, fracRatioYAxisLB, fracRatioYAxisUB, doSymErr, doFracRatio, doSmear, doStopXSec, doVerbosity);
    }
    void CalculateSystsCompShape(Color_t colorErrGraph, bool doAbsRatio, float fracRatioYAxisLB, float fracRatioYAxisUB, bool doSymErr, bool doSmear, bool doStopXSec, bool doVerbosity = false) {
        bool doFracRatio = true;
        if (doVerbosity) {
            cout << "about to calculate systs in compSamp " << endl;
        }
        compSamp.first.CalculateSystsShapes(colorErrGraph, doAbsRatio, fracRatioYAxisLB, fracRatioYAxisUB, doSymErr, doFracRatio, doSmear, doStopXSec, doVerbosity);
    }
    void CalculateSysts_IndMC(Color_t colorErrGraph, bool doAbsRatio, float fracRatioYAxisLB, float fracRatioYAxisUB, bool doSymErr, bool doSmear, bool doStopXSec, bool doVerbosity = false) {
        bool doFracRatio = false;
        if (doVerbosity) {
            cout << "about to calculate systs in compSamp " << endl;
        }
        if (vecSampDisplay_IndMC.size() > 0) {
            nonTTBarSD = vecSampDisplay_IndMC[0];
            vector<IndSamplePlotInfo> vecIndMCISPIs(0);
            for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
                vecIndMCISPIs.push_back(vecSampDisplay_IndMC[iIndMC].first);
            }
            nonTTBarSD.first.CloneHists_NonTTBar(&vecIndMCISPIs);
            nonTTBarSD.first.CalculateSysts(colorErrGraph, doAbsRatio, fracRatioYAxisLB, fracRatioYAxisUB, doSymErr, doFracRatio, doSmear, doStopXSec, doVerbosity);
        }
        for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
            vecSampDisplay_IndMC[iIndMC].first.CalculateSysts(colorErrGraph, doAbsRatio, fracRatioYAxisLB, fracRatioYAxisUB, doSymErr, doFracRatio, doSmear, doStopXSec, doVerbosity);
        }
    }
    void PrintIndSystInfo() {
        
    }
    void AddToStack(THStack * MCStack) {
        for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
            MCStack->Add(vecSampDisplay_IndMC[iIndMC].first.grabbedHist_TH1F);
        }
    }
    
    void AddSignalToStack(THStack * MCStack) {
        MCStack->Add(compSamp.first.grabbedHist_TH1F);
    }
    
    //Shape Spectra functions
    void SetInputFileShape(vector<TString> * inVecFileNames, TString basePath, TString nameAppend, int numSysts, bool doVerbosity = 0) {
        vecISPI_asLoaded.resize(inVecFileNames->size());
        TString fileName;
        TString grabName;
        for (unsigned int iISPI = 0; iISPI < vecISPI_asLoaded.size(); ++iISPI) {
            grabName = inVecFileNames->at(iISPI);
            if (doVerbosity) {
                cout << "grabName before changing " << grabName << endl;
            }
            if (!(grabName.Contains("TTBar") || grabName.Contains("Data") || grabName.Contains("Stop"))) {
                grabName = "NonTTBar";
            }
            if (doVerbosity) {
                cout << "iISPI " << iISPI << endl;
                cout << "basePath " << basePath << endl;
                cout << "grabName " << grabName << endl;
                cout << "nameAppend " << nameAppend << endl;
            }
            vecISPI_asLoaded[iISPI].SetInputFile(basePath + grabName + nameAppend);
            vecISPI_asLoaded[iISPI].SetName(inVecFileNames->at(iISPI));
            vecISPI_asLoaded[iISPI].SetSampleType(doVerbosity);
            vecISPI_asLoaded[iISPI].DefaultWeights(numSysts);
        }
    }
    void GrabCentralValuesShape(TString addName = "", bool doVerbosity = false) {
        TString grabName = "";
        for (unsigned int iISPI = 0; iISPI < vecISPI_asLoaded.size(); ++iISPI) {
            if (vecISPI_asLoaded[iISPI].nameISPI.Contains("Data")) {
                grabName = "data_obs" + addName;
            }
            else if (vecISPI_asLoaded[iISPI].nameISPI.Contains("T2tt") || vecISPI_asLoaded[iISPI].nameISPI.Contains("T2bw")) {
                grabName = "signal" + addName;
            }
            else {
                grabName = vecISPI_asLoaded[iISPI].nameISPI + addName;
            }
            if (doVerbosity) {
                cout << "for iISPI = " << iISPI << ", trying to grab " << grabName << endl;
            }
            vecISPI_asLoaded[iISPI].GrabHist(grabName, doVerbosity);
        }
    }
    void GrabSystValues(vector<TString> * vecSystNameAppends, TString addName = "", bool doVerbosity = false) {
        TString baseHistGrabName = "";
        for (unsigned int iISPI = 0; iISPI < vecISPI_asLoaded.size(); ++iISPI) {
            if (vecISPI_asLoaded[iISPI].nameISPI.Contains("Data")) {
                baseHistGrabName = "data_obs";
            }
            else if (vecISPI_asLoaded[iISPI].nameISPI.Contains("T2tt") || vecISPI_asLoaded[iISPI].nameISPI.Contains("T2bw")) {
                baseHistGrabName = "signal";
            }
            else {
                baseHistGrabName = vecISPI_asLoaded[iISPI].nameISPI;
            }
            if (doVerbosity) {
                cout << "for iISPI = " << iISPI << ", trying to grab " << baseHistGrabName << endl;
            }
            vecISPI_asLoaded[iISPI].GrabHistSystShape(baseHistGrabName, vecSystNameAppends, addName, doVerbosity);
        }
    }
    
} HistogramDisplayStructs;

typedef struct GlobalHistPlotInfo {
    vector<LatexString> vecLatexStrings;
    AxisDisplayParams fracRatioADP;
    TString strExtra;
    void DefaultVarVals() {
        vecLatexStrings.resize(5);
        //TopCMSPreliminary String is index 0, Integrated Lumi string is index 1
        //Gen cut string is index 2, MET String is index 3
        //Dilep String is index 4
        fracRatioADP.DefaultVarVals();
        strExtra = "";
    }
    void SetFracRatioADPNameRange(bool doAbsRatio, float fracRatioYAxisRangeLB, float fracRatioYAxisRangeUB) {
        TString fracRatioNumerName;
        TString fracRatioDenomName;
        fracRatioNumerName = (doAbsRatio) ? "Data/" : "(MC-Data)/";
        fracRatioDenomName = (doAbsRatio) ? "MC" : "Data"; 
        
        /*
            float   fracRatioYAxisRangeLB, fracRatioYAxisRangeUB;
        fracRatioYAxisRangeLB = (doAbsRatio) ? (1 - 0.23) : -0.26;
        fracRatioYAxisRangeUB = (doAbsRatio) ? (1 + 0.23) : 0.26;
        
        if (doSpecialRange) {
            fracRatioYAxisRangeUB = 1.6;
            fracRatioYAxisRangeLB = 0.5;
        }
        */
        fracRatioADP.SetStrings(fracRatioNumerName, fracRatioDenomName);
        fracRatioADP.SetRanges(fracRatioYAxisRangeLB, fracRatioYAxisRangeUB);
    }
    
    TH1F * FracRatioHist_GHPI(TH1F * topHist, TH1F * botHist, bool doAbsRatio, TString fracRatioName) {
        TH1F * outHist = FracRatioHist(topHist, botHist, fracRatioADP.axisStringPart1, fracRatioADP.axisStringPart2, fracRatioADP.axisRangePart1, fracRatioADP.axisRangePart2, doAbsRatio, fracRatioName);
        return outHist;
    }
    void SetLatexStringVec(float intLumi, int typeMET = 0, TString extraMETText = "", int typeDiLep = -1, TString genCutUsed = "", bool isPrelim = true) {
        SetCMSPreliminaryLatex(&vecLatexStrings[0], isPrelim);
	vecLatexStrings[0].straightGhetto.textFont = 42;
        SetLumiLatex(&vecLatexStrings[1], intLumi);
        SetGenCutLatex(&vecLatexStrings[2],  genCutUsed);
        if (strExtra.Length() > 0) {
            SetGeneralLatexString(&vecLatexStrings[3], strExtra);
        }
        else {
            SetMETLatexString(&vecLatexStrings[3], typeMET, extraMETText);
        }
        SetDilepLatexString(&vecLatexStrings[4], typeDiLep);
    }
    void AddProjectionLatexStrings(TString projString1 = "", TString projString2 = "") {
        if (vecLatexStrings.size() < 7) vecLatexStrings.resize(7);
        SetProjectionLatex(&vecLatexStrings[5], 0, projString1);
        SetProjectionLatex(&vecLatexStrings[6], 1, projString2);
    }
    void DrawLatexStrings(TCanvas * inputCanvas, int whichPad = 1, bool doVerbosity = false, int begIndex = 0, int endIndex = -1) {
        int startIndex = begIndex;
        int finalIndex = endIndex > -1 ? endIndex : (int) vecLatexStrings.size() - 1;
        for (int iLS = startIndex; iLS <= finalIndex; ++iLS) {
            if (doVerbosity) {
                cout << "iLS = " << iLS << ", and text is " << vecLatexStrings[iLS].text << endl;
            }
            vecLatexStrings[iLS].DrawCanvas(inputCanvas, whichPad, doVerbosity);
        }
    }
    void SetParamsFromCommandLine(int argc, char * argv[]) {
        for (int iGHPIParam = 0; iGHPIParam < argc; ++iGHPIParam) {
            if (strncmp (argv[iGHPIParam],"strExtra", 8) == 0) {
                strExtra = TString(argv[iGHPIParam + 1]);
                while (strExtra.Contains("SPACE")) {
                    strExtra.Replace(strExtra.Index("SPACE"), 5, " ");
                }
            }
        }
    }
} GlobalHistPlotInfo;

bool SortHists_FirstHistGreater(const SampDisplay &a, const SampDisplay &b, bool sortByInt = false) {
    if (sortByInt) {
        return a.first.grabbedHist_TH1F->Integral() > b.first.grabbedHist_TH1F->Integral();
    }
    else {
        return a.second.sortParam > b.second.sortParam;
    }
}
bool SortHists_FirstHistSmaller(const SampDisplay &a, const SampDisplay &b, bool sortByInt = false) {
    if (sortByInt) {
        return a.first.grabbedHist_TH1F->Integral() < b.first.grabbedHist_TH1F->Integral();
    }
    else {
        return a.second.sortParam < b.second.sortParam;
    }
}
inline bool operator<(const SampDisplay &a, const SampDisplay &b)
{
    return (a.second.sortParam < b.second.sortParam);
}

inline bool operator>(const SampDisplay &a, const SampDisplay &b)
{
    return a.first.grabbedHist_TH1F->Integral() < b.first.grabbedHist_TH1F->Integral();
}
