#include "TString.h"
#include "TCanvas.h"
#include "TLatex.h"

#include <iostream>

using namespace std;

typedef pair<int, int> vPP;

struct DrawCommands {
    bool isSim;
    bool isSig;
    int whichPolIndex;
    
    int whichFile;
    int whichAxis;
    int whichDraw;
    
    //variables for the latex file generation
    vector< vector<TString> > vecVecSedLatexFileName_WhichFile, vecVecSedLatexPhysicsString;
    vector<TString> vecSedLatexFileName_WhichDraw;
    vector<TString> vecSedLatexDistributionString;
    TString fileNameMT2Corr;
    TString strAddtl;
    TString strNumber;
    //variables for the plot saving
    vector< vector<TString> > vecVecFileNames, vecVecCanvAddName, vecVecLatexString;
    vector<TString> vecStrLatexAnalysis;
    TString strCanvAdd_Ana;
    int whichVecIndex;
    
    void DrawOnCanvas(TCanvas * inputCanv) {
        float xValString = 0.04;
        float baseYValString = 0.86;
        float stepSizeYValString = 0.10;
        
        float size = 0.05;
        TLatex textStrAna;
        textStrAna.SetNDC();
        textStrAna.SetTextSize(size);
        inputCanv->cd();
        for (unsigned int iStr = 0; iStr < vecStrLatexAnalysis.size(); ++iStr) {
            textStrAna.DrawLatex(xValString, baseYValString - iStr * stepSizeYValString, vecStrLatexAnalysis[iStr]);
        }
    }
    
    void SetAnalysisLatexString(bool moreBJets, int whichMET) {
        
        
        strCanvAdd_Ana = "";
        vecStrLatexAnalysis.resize(0);
        vecStrLatexAnalysis.push_back("3D M_{T2} Shape Selection");
        if (moreBJets) {
            vecStrLatexAnalysis.push_back("2+ b-jets");
            strCanvAdd_Ana += "_2bjets";
        }
        if (whichMET == 1) {
            vecStrLatexAnalysis.push_back("#frac{#slash{E}^{2}_{T}}{H_{T}} #geq 80");
            strCanvAdd_Ana += "_METSig1D";
        }
        else if (whichMET == 2) {
            vecStrLatexAnalysis.push_back("#slash{E}_{T} sig. #geq 160");
            strCanvAdd_Ana += "_METSigTrue";
        }
    }
    void CallLatexSedScript(bool doVerb = false) {
        TString outDir = "TripleMT2Plots/LatexFigs/";
        TString baseSingleFigName="TripleMT2Plots/LatexFigs/fig_MT2corr_CorrPATSY.tex";
        TString base2DFigName="TripleMT2Plots/LatexFigs/fig_MT2corr_PATSY_2DDist.tex";
        TString base3DFigName="TripleMT2Plots/LatexFigs/fig_MT2corr_PATSY_3DInfo.tex";

        TString outSingleFigName="TripleMT2Plots/LatexFigs/fig_MT2corr_";
        outSingleFigName += fileNameMT2Corr;
        outSingleFigName += ".tex";
        System_CopyFile(baseSingleFigName, outSingleFigName, doVerb);
        
        TString out2DFigName="TripleMT2Plots/LatexFigs/fig_MT2corr_";
        out2DFigName += vecVecSedLatexFileName_WhichFile[whichVecIndex][whichFile];
        out2DFigName += "_2DDist.tex";
        System_CopyFile(base2DFigName, out2DFigName, doVerb);
        
        TString out3DFigName="TripleMT2Plots/LatexFigs/fig_MT2corr_";
        out3DFigName += vecVecSedLatexFileName_WhichFile[whichVecIndex][whichFile];
        out3DFigName += "_3DInfo.tex";
        System_CopyFile(base3DFigName, out3DFigName, doVerb);
        
        TString strDistributionStringToReplace = "DistributionString";
        TString strPhysicsStringToReplace = "PhysicsString";
        TString strAdditionalStringToReplace = "AdditionalString";
        TString strMT2CorrFilenameToReplace = "MT2CorrFilename";
        TString strNumberStringToReplace = "NumberString";
        TString strSampleStringToReplace = "SampleString";
        
        TString strDoubleHash = "##";
        TString strSlash = "\\\\";
        
        System_SedInFile(outSingleFigName, strDistributionStringToReplace, vecSedLatexDistributionString[whichDraw], doVerb);
        System_SedInFile(outSingleFigName, strPhysicsStringToReplace, vecVecSedLatexPhysicsString[whichVecIndex][whichFile], doVerb);
        System_SedInFile(outSingleFigName, strAdditionalStringToReplace, strAddtl, doVerb);
        System_SedInFile(outSingleFigName, strMT2CorrFilenameToReplace, fileNameMT2Corr, doVerb);
        
        System_SedInFile(out2DFigName, strNumberStringToReplace, strNumber, doVerb);
        System_SedInFile(out2DFigName, strPhysicsStringToReplace, vecVecSedLatexPhysicsString[whichVecIndex][whichFile], doVerb);
        System_SedInFile(out2DFigName, strAdditionalStringToReplace, strAddtl, doVerb);
        System_SedInFile(out2DFigName, strSampleStringToReplace, vecVecSedLatexFileName_WhichFile[whichVecIndex][whichFile], doVerb);
        
        System_SedInFile(out3DFigName, strNumberStringToReplace, strNumber, doVerb);
        System_SedInFile(out3DFigName, strPhysicsStringToReplace, vecVecSedLatexPhysicsString[whichVecIndex][whichFile], doVerb);
        System_SedInFile(out3DFigName, strAdditionalStringToReplace, strAddtl, doVerb);
        System_SedInFile(out3DFigName, strSampleStringToReplace, vecVecSedLatexFileName_WhichFile[whichVecIndex][whichFile], doVerb);
        
        

        System_SedMultipleInFile(outSingleFigName, strDoubleHash, strSlash, doVerb);
        System_SedMultipleInFile(out2DFigName, strDoubleHash, strSlash, doVerb);
        System_SedMultipleInFile(out3DFigName, strDoubleHash, strSlash, doVerb);
    }
    
    bool IsT2tb() {
        if (!(isSim && isSig)) {
            return false;
        }
        return whichFile == 1;
    }
    
    void SetInputSUSYT2LP(SUSYT2LimitParams * inSUSYT2LP) {
        inSUSYT2LP->indexPol = whichPolIndex;
        inSUSYT2LP->typeT2 = 0;
        inSUSYT2LP->charFrac = -1;
        switch (whichFile) {
            case 0:
                inSUSYT2LP->typeT2 = 1;
                break;
            case 1:
                inSUSYT2LP->typeT2 = 3;
                break;
            case 2:
                inSUSYT2LP->charFrac = 75;
                break;
            case 3:
                inSUSYT2LP->charFrac = 50;
                break;
            case 4:
                inSUSYT2LP->charFrac = 25;
                break;
            default:
                break;
        }
        inSUSYT2LP->SetStrings();
    }
    void SetSedLatexDistributionStringVec() {
        TString baseAxisString = "dependence of $##left##langle";
        TString endAxisString = "##right##rangle$ on ";
        TString arrAxisVarString[4] = {"", "##MTTwobbZMET", "##MTTwolblb", "##MTTwoll"};
        TString arrAxisOtherVarString[4] = {"", "##MTTwoll and ##MTTwolblb", "##MTTwoll and ##MTTwobbZMET", "##MTTwolblb and ##MTTwobbZMET"};
        
        TString baseDistString = "2D distribution of ";
        vector<TString> vecDistString{baseAxisString + arrAxisVarString[whichAxis] + endAxisString + arrAxisOtherVarString[whichAxis], baseDistString + arrAxisOtherVarString[1], baseDistString + arrAxisOtherVarString[2], baseDistString + arrAxisOtherVarString[3], baseDistString + TString("##MTTwoll and ##KTTwo")};
        vecSedLatexDistributionString.insert(vecSedLatexDistributionString.end(), vecDistString.begin(), vecDistString.end());
    }
    
    void SetNumberString() {
        strNumber = isSim ? "normalized fractional " : "";
    }
    void SetAdditionalString(vPP * massStop = NULL, vPP * DeltaM = NULL) {
        TString strWith = " with ";
        TString strAnd = " and ";
        strAddtl = "";
        if (isSig && (massStop || DeltaM)) {
            if (massStop) {
                strAddtl += strAddtl.Length() ? strAnd : strWith;
                strAddtl += MStopPlotString(massStop, true);
            }
            if (DeltaM) {
                strAddtl += strAddtl.Length() ? strAnd : strWith;
                strAddtl += DeltaMPlotString(DeltaM, true);
            }
        }
    }
    void UpdateSedLatexPhysics(SUSYT2LimitParams * inSUSYT2LP) {
        if (isSim && isSig) {
            vecVecSedLatexPhysicsString[whichVecIndex][whichFile] = inSUSYT2LP->LatexString();
        }
    }
    void SetSedLatexPhysicsStringVecs() {
        vector<TString> vecPhysicsStringData{"collision data", "estimated fake lepton"};
        vector<TString> vecPhysicsStringBkg{"##ttbar", "##Zll", "Other"};
        vector<TString> vecPhysicsStringSig{"T2tt", "T2tb", "T2bw0p75", "T2bw0p50", "T2bw0p25"};
        vecVecSedLatexPhysicsString.resize(0);
        vecVecSedLatexPhysicsString.push_back(vecPhysicsStringData);
        vecVecSedLatexPhysicsString.push_back(vecPhysicsStringBkg);
        vecVecSedLatexPhysicsString.push_back(vecPhysicsStringSig);
    }
    void SetSedLatex(SUSYT2LimitParams * inSUSYT2LP, vPP * massStop = NULL, vPP * DeltaM = NULL) {
        SetNumberString();
        SetAdditionalString(massStop, DeltaM);
        SetSedLatexDistributionStringVec();
        SetSedLatexPhysicsStringVecs();
        UpdateSedLatexPhysics(inSUSYT2LP);
    }
    void SetFileNameVecs() {
        vector<TString> vecFilesData{"Tree_Data_Bare_NoSyst", "Tree_Fake_Bare_NoSyst"};
        vector<TString> vecFilesBkg{"Tree_TTbar_Bare_NoSyst", "Tree_DY_Bare_NoSyst", "Tree_NonDY_NonTTbar_Bare_NoSyst"};
        vector<TString> vecFilesSig{"Tree_T2tt_Bare_NoSyst", "Tree_T2tb_Bare_NoSyst", "Tree_T2bwx0p75_Bare_NoSyst", "Tree_T2bwx0p50_Bare_NoSyst", "Tree_T2bwx0p25_Bare_NoSyst"};
        vecVecFileNames.resize(0);
        vecVecFileNames.push_back(vecFilesData);
        vecVecFileNames.push_back(vecFilesBkg);
        vecVecFileNames.push_back(vecFilesSig);
    }
    void SetCanvAddNameVecs() {
        vector<TString> vecCanvAddNameData{"Data", "FakeLep"};
        vector<TString> vecCanvAddNameBkg{"TTbar", "DY", "Other"};
        vector<TString> vecCanvAddNameSig{"T2tt", "T2tb", "T2bw_x0p75", "T2bw_x0p50", "T2bw_x0p25"};
        vecVecCanvAddName.resize(0);
        vecVecCanvAddName.push_back(vecCanvAddNameData);
        vecVecCanvAddName.push_back(vecCanvAddNameBkg);
        vecVecCanvAddName.push_back(vecCanvAddNameSig);
        vecVecSedLatexFileName_WhichFile = vecVecCanvAddName;
    }
    void SetLatexStringVecs() {
        vector<TString> vecLatexStringsData{"Data", "Fake Leptons"};
        vector<TString> vecLatexStringsBkg{"t#bar{t}", "Z#rightarrow l^{+}l^{-}", "Other"};
        vector<TString> vecLatexStringsSig{"T2tt", "T2tb", "T2bw, x = 0.25", "T2bw, x = 0.50", "T2bw, x = 0.75"};
        vecVecLatexString.resize(0);
        vecVecLatexString.push_back(vecLatexStringsData);
        vecVecLatexString.push_back(vecLatexStringsBkg);
        vecVecLatexString.push_back(vecLatexStringsSig);
    }
    void SetVecIndex() {
        whichVecIndex = 0;
        if (isSim) {
            whichVecIndex = isSig ? 2 : 1;
        }
    }
    void SetVecs() {
        SetVecIndex();
        SetFileNameVecs();
        SetCanvAddNameVecs();
        SetLatexStringVecs();
    }
    void UpdateLatexString(SUSYT2LimitParams * inSUSYT2LP) {
        cout << "test a " << endl;
        if (isSim && isSig) {
            SetInputSUSYT2LP(inSUSYT2LP);
            cout << "test b " << endl;
            vecVecLatexString[whichVecIndex][whichFile] = inSUSYT2LP->TitleString();
        }
    }
    void UpdateCanvAddNameString(SUSYT2LimitParams * inSUSYT2LP, vPP * massStop = NULL, vPP * DeltaM = NULL) {
        if (isSim && isSig) {
            vecVecCanvAddName[whichVecIndex][whichFile] = inSUSYT2LP->SavingName(false);
            if (massStop) {
                TString strStop = "_MStop";
                strStop += massStop->first;
                strStop += "pm";
                strStop += massStop->second;
                vecVecCanvAddName[whichVecIndex][whichFile] += strStop;
            }
            if (DeltaM) {
                TString strDeltaM = "_DeltaM";
                strDeltaM += DeltaM->first;
                strDeltaM += "pm";
                strDeltaM += DeltaM->second;
                vecVecCanvAddName[whichVecIndex][whichFile] += strDeltaM;
            }
            int indexDecimal = vecVecCanvAddName[whichVecIndex][whichFile].Index(".");
            if (indexDecimal > -1) {
                vecVecCanvAddName[whichVecIndex][whichFile].Replace(indexDecimal, 1, "p");
            }
            vecVecSedLatexFileName_WhichFile[whichVecIndex][whichFile] = vecVecCanvAddName[whichVecIndex][whichFile];
        }
    }
    TString StrFileName() {
        TString baseDir = "SpecialSkims_2/";
        return baseDir + vecVecFileNames[whichVecIndex][whichFile];
    }
    TString CanvName(vPP * massStop = NULL, vPP * DeltaM = NULL) {
        TString arrAxisString[4] = {"", "MeanMT2bb", "MeanMT2lblb", "MeanMT2ll"};
        
        TString arrCanvName[5] = {arrAxisString[whichAxis], "MT2llvsMT2lblb", "MT2llvsMT2bb", "MT2lblbvsMT2bb", "MT2llvsKT2"};
        TString canvName = "c_";
        canvName += arrCanvName[whichDraw];
        canvName += TString("_") + vecVecCanvAddName[whichVecIndex][whichFile];
        canvName += strCanvAdd_Ana;
        int indexDecimal = canvName.Index(".");
        if (indexDecimal > -1) {
            canvName.Replace(indexDecimal, 1, "p");
        }
        fileNameMT2Corr = canvName;
        fileNameMT2Corr.Replace(fileNameMT2Corr.Index("c_"), 2, "");
        return canvName;
    }
    TString LatexDrawString() {
        return vecVecLatexString[whichVecIndex][whichFile];
    }
    TString WeightString() {
        TString baseWeightData = whichFile > 0 ? "TEventFakeLepWeight * " : "";
        TString baseWeightMC = isSig ? "TPUWeight1D * " : "TWeightMC * TPUWeight1D * ";
        if (!isSig) {
            if (whichFile == 0) {
                baseWeightMC += "TGenWeight[0] * 1.03 * ";
            }
            else if (whichFile == 1) {
                baseWeightMC += " 1.45 * ";
            }
            baseWeightMC += " 19656 * 0.9 * ";
        }
        TString addWeightStop = "";
        if (whichPolIndex > -1) {
            addWeightStop= "TGenStopPolWeights[";
            addWeightStop += whichPolIndex;
            addWeightStop += "] * ";
        }
        
        TString strWeight;
        if (!isSim) {
            strWeight = baseWeightData;
        }
        else {
            strWeight = baseWeightMC;
            if (isSig) {
                strWeight += addWeightStop;
            }
        }
        return strWeight;
    }
    void DefaultVarVals() {
        isSim = true;
        isSig = false;
        whichFile = 0;
        whichPolIndex = -1;
        
        whichDraw = 0;
        whichAxis = 3;
        whichVecIndex = 0;
    }
    void SetAsFake() {
        whichFile = 1;
        isSim = false;
        isSig = false;
    }
    bool IsFake() {
        return (whichFile == 1 && !isSim && !isSig);
    }
    void SetAsTTbar() {
        whichFile = 0;
        isSim = true;
        isSig = false;
    }
    bool IsTTbar() {
        return (whichFile == 0 && isSim && !isSig);
    }
    void SetAsDY() {
        whichFile = 1;
        isSim = true;
        isSig = false;
    }
    bool IsDY() {
        return (whichFile == 1 && isSim && !isSig);
    }
    void SetAsOther() {
        whichFile = 2;
        isSim = true;
        isSig = false;
    }
    bool IsOther() {
        return (whichFile == 2 && isSim && !isSig);
    }
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        for (int iDCParam = 0; iDCParam < argc; ++iDCParam) {
            if (strncmp (argv[iDCParam],"-wDraw", 6) == 0) {
                whichDraw = strtol(argv[iDCParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iDCParam],"-wAxis", 6) == 0) {
                whichAxis = strtol(argv[iDCParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iDCParam],"-wFile", 6) == 0) {
                whichFile = strtol(argv[iDCParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iDCParam],"isSig", 5) == 0) {
                isSig = true;
            }
            else if (strncmp (argv[iDCParam],"-wPol", 5) == 0) {
                whichPolIndex = strtol(argv[iDCParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iDCParam],"isData", 6) == 0) {
                isSim = false;
            }
        }
    }
};

struct DrawParams {
    int nBins;
    float xAxisLB, xAxisUB;
    TString axisString;
    TString drawVar;
    
    void SetAxisString() {
        axisString = "";
        axisString += nBins;
        axisString += ", ";
        axisString += xAxisLB;
        axisString += ", ";
        axisString += xAxisUB;
    }
    
    void SetVals (int inputBins, float inputXLB, float inputXUB, bool isSim, int whichVar) {
        TString arrVarsSim[4] = {"TSmearMT2ll_v2", "TSmearMT2lblb_v2", "TSmearMT2bb_ZMET_v2", "TSmearMT2bb_ZMET_v2 - TSmearMT2lblb_v2"};
        TString arrVarsData[4] = {"TPFMT2ll", "TPFMT2lblb", "TPFMT2bb_ZMET", "TPFMT2bb_ZMET - TPFMT2lblb"};
        
        nBins = inputBins;
        xAxisLB = inputXLB;
        xAxisUB = inputXUB;
        SetAxisString();
        if (isSim) {
            drawVar = arrVarsSim[whichVar];
        }
        else {
            drawVar = arrVarsData[whichVar];
        }
    }
};

TString BaseCutCommand(bool isSim, bool cutZ, bool moreBJets, int whichMET, int whichChannel = -1, bool isT2tb = false) {
    TString strMET = "TSmearMET_v2 >= 40";
    if (whichMET == 1) {
        strMET = "TMath::Power(TSmearMET_v2/TMath::Sqrt(THT_AllJets), 2) >= 80";
    }
    else if (whichMET == 2) {
        strMET = "TSmearMET_SigTrue_v2 >= 160";
    }
    if (!isSim) {
        int strIndex = strMET.Index("Smear");
        strMET.Replace(strIndex, 5, "PF");
        strIndex = strMET.Index("_v2");
        strMET.Replace(strIndex, 3, "");
    }
    TString strZ = cutZ ? "!TDiLepInZ" : "";
    TString cutCommand = "(";
    if (whichChannel > -1) {
        cutCommand += "TChannel == ";
        cutCommand += whichChannel;
        cutCommand += " && ";
    }
    cutCommand += "TNJets > 1 && TNJetsBtag > ";
    cutCommand += moreBJets ? 1 : 0;
    cutCommand += " && ";
    if (isT2tb) {
        cutCommand += "((TGenChargino_0_Index > 0 || TGenChargino_1_Index > 0) && !(TGenChargino_0_Index > 0 && TGenChargino_1_Index > 0)) && ";
    }
    cutCommand += "TDoEvent && ((";
    cutCommand += strMET;
    if (strZ.Length() > 0) {
        cutCommand += " && ";
        cutCommand += strZ;
    }
    cutCommand += " && ((TPassDoubleMu && TChannel == 0) || (TPassDoubleEl && TChannel == 1))) || (TPassElMu && TChannel == 2))";
    
    TString lowBLepMassCut = "TMath::Max(TSmearMassBLep0_BLepsUsed_v2, TSmearMassBLep1_BLepsUsed_v2) < 200";
    if (!isSim) lowBLepMassCut = "TMath::Max(TPFMassBLep0_BLepsUsed, TPFMassBLep1_BLepsUsed) < 200";
    
    cutCommand += " && " + TString(lowBLepMassCut);
    cutCommand += ")";
    return cutCommand;
}
TString DrawCommand(vector<DrawParams> * inVecDPs) {
    
    TString strColon = ":";
    TString strComma = ",";
    TString str3DHist = ">>h3d";
    TString str2DHist = ">>h2d";
    
    unsigned int nDims = inVecDPs->size();
    
    TString drawCommand_Var = "";
    TString drawCommand_Hist = nDims > 2 ? ">>h3d(" : ">>h2d(";
    
    for (unsigned int iDim = 0; iDim < nDims; ++iDim) {
        drawCommand_Var += inVecDPs->at(nDims - 1 - iDim).drawVar;
        drawCommand_Hist += inVecDPs->at(iDim).axisString;
        if (iDim < nDims - 1) {
            drawCommand_Var += strColon;
            drawCommand_Hist += strComma;
        }
        else {
            drawCommand_Hist += ")";
        }
        
    }
    TString drawCommand = drawCommand_Var + drawCommand_Hist;
    
    return drawCommand;
}
TString DrawCommand(DrawCommands * inDC) {
    DrawParams dp_MT2ll, dp_MT2lblb, dp_MT2bb_ZMET, dp_KT2;
    if (inDC->whichDraw == 0) {
        dp_MT2ll.SetVals(200, 0, 200, inDC->isSim, 0);
        dp_MT2lblb.SetVals(500, 0, 500, inDC->isSim, 1);
        dp_MT2bb_ZMET.SetVals(420, 80, 500, inDC->isSim, 2);
        dp_KT2.SetVals(800, -300, 500, inDC->isSim, 3);
    }
    else {
        dp_MT2ll.SetVals(200, 0, 200, inDC->isSim, 0);
        dp_MT2lblb.SetVals(300, 0, 300, inDC->isSim, 1);
        dp_MT2bb_ZMET.SetVals(270, 80, 350, inDC->isSim, 2);
        dp_KT2.SetVals(800, -300, 500, inDC->isSim, 3);
    }
    
    vector<DrawParams> vecDP3D{dp_MT2bb_ZMET, dp_MT2lblb, dp_MT2ll};
    vector<DrawParams> vecDP2DA{dp_MT2lblb, dp_MT2ll};
    vector<DrawParams> vecDP2DB{dp_MT2bb_ZMET, dp_MT2ll};
    vector<DrawParams> vecDP2DC{dp_MT2bb_ZMET, dp_MT2lblb};
    vector<DrawParams> vecDP2DD{dp_KT2, dp_MT2ll};
    vector< vector<DrawParams> > vecVecDPs{vecDP3D, vecDP2DA, vecDP2DB, vecDP2DC, vecDP2DD};
    
    TString drawCommand = DrawCommand(&vecVecDPs[inDC->whichDraw]);
    
    return drawCommand;
}


TString DrawCommandTwo(DrawCommands * inDC) {
    DrawParams dp_MT2ll, dp_MT2lblb, dp_MT2bb_ZMET, dp_KT2;
    dp_MT2ll.SetVals(20, 0, 200, inDC->isSim, 0);
    dp_MT2lblb.SetVals(40, 0, 400, inDC->isSim, 1);
    dp_MT2bb_ZMET.SetVals(42, 80, 500, inDC->isSim, 2);
    dp_KT2.SetVals(80, -300, 500, inDC->isSim, 3);
    
//    vector<DrawParams> vecDP3D{dp_MT2bb_ZMET, dp_MT2lblb, dp_MT2ll};
    vector<DrawParams> vecDP3D{dp_MT2ll, dp_MT2lblb, dp_MT2bb_ZMET};
    vector<DrawParams> vecDP2DA{dp_MT2lblb, dp_MT2ll};
    vector<DrawParams> vecDP2DB{dp_MT2bb_ZMET, dp_MT2ll};
    vector<DrawParams> vecDP2DC{dp_MT2bb_ZMET, dp_MT2lblb};
    vector<DrawParams> vecDP2DD{dp_KT2, dp_MT2ll};
    vector< vector<DrawParams> > vecVecDPs{vecDP3D, vecDP2DA, vecDP2DB, vecDP2DC, vecDP2DD};
    
    TString drawCommand = DrawCommand(&vecVecDPs[inDC->whichDraw]);
    
    return drawCommand;
}


TString WeightStringSpecific(DrawCommands * inDC, bool moreBJets, int whichChannel = -1) {
    int lumi = 19656;
    vector<TString> vecWeightTTBar1B{"0.99", "1.13", "1.02"};
    vector<TString> vecWeightTTBar2B{"1.06", "1.18", "1.07"};
    vector<TString> vecWeightDY1B{"1.43", "1.46", "1.44"};
    vector<TString> vecWeightDY2B{"1.53", "1.48", "1.50"};
    vector<TString> * vecToUse;
    TString baseWeightString = inDC->WeightString();

    bool useExtraSF = false;
    if (inDC->isSim) {
        if (inDC->IsDY()) {
            useExtraSF = true;
            if (moreBJets) {
                vecToUse = &vecWeightDY2B;
            }
            else {
                vecToUse = &vecWeightDY1B;
            }
        }
        else if (inDC->IsTTbar()){
            useExtraSF = true;
            if (moreBJets) {
                vecToUse = &vecWeightTTBar2B;
            }
            else {
                vecToUse = &vecWeightTTBar1B;
            }
        }
        baseWeightString += lumi;
        baseWeightString += " * ";
        if (useExtraSF) {
            baseWeightString += vecToUse->at(whichChannel);
            baseWeightString += " * ";
        }
    }
    return baseWeightString;
}
