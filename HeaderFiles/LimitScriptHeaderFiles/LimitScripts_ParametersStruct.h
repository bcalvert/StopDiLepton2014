#ifndef LimitScripts_ParamStructs
#define LimitScripts_ParamStructs

#include "TString.h"

struct LimitParametersContainer {
    //parameters for which dilepton analysis it is
    int typeFullSel;
    int typeDilepAna; // 0 for CC, 1 for shape
    int typeDilepChannel; //-1 for all, 0 for MuMu, 1 for EE, 2 for EMu;
    int typeCut;
    int numDims;
    bool doMT2Scan;
    bool simFakes;
    bool splitDilepChannel;
    int fixDMBin;
    TString strFixBin;
    int numIterations;
    int whichTTbarGen;          // 0 is Madgraph, 1 is MC@NLO, 2 is Powheg (default is 2)
    bool usePowhegShapeSyst;
    
    bool grabOld;
    
    TString strFullSel, strDilepAnaType, strCutType, strDim, strDilepChannel, strSimFakes;
    TString strPowShape;
    
    //parameters of the actual limit running
    int typeLepComboAnaType;
    int useAsymLims;
    int isExpLim;
    bool useSigContam;
    bool isSignificance;
    bool hackLimSig;
    TString strLepComboAnaType, strAsympLims, strExpLim, strSigContam;
    bool doLocal;


    //basePath for single lepton cards from Michael
    TString basePathSingLep;
    
    //Combined together strings
    TString strParamAppend;
    TString strBaseNameCard, strBaseNameLim, strBaseNameSignificance;
    
    /*
    void SetSavingParamsStruct(SavingParams * inSP) {
       inSP->strFullSel = strFullSel;
        inSP->strDilepAnaType = strDilepAnaType;
        inSP->strCutType = strCutType;
        inSP->strDim = strDim;
        inSP->strDilepChannel = strDilepChannel;
        
        inSP->strLepComboAnaType = strLepComboAnaType;
        inSP->strAsympLims = strAsympLims;
        inSP->strExpLim = strExpLim;
        inSP->strSigContam = strSigContam;
        
        inSP->basePathSingLep = basePathSingLep;
        
        inSP->strParamAppend = strParamAppend;
        inSP->strBaseNameCard = strBaseNameCard;
        inSP->strBaseNameLim = strBaseNameLim;
    }
    */
    
    void DefaultVals() {
        typeFullSel = 0;
        typeDilepAna = 0;
        typeDilepChannel = -1;
        typeCut = 0;
        numDims = 0;
        doMT2Scan = true;
        simFakes = false;
        splitDilepChannel = false;
        grabOld = false;
        fixDMBin = -1;
        whichTTbarGen = 2;
        usePowhegShapeSyst = false;
        
        typeLepComboAnaType = 0;
        useAsymLims = 0;
        isExpLim = 1;
        useSigContam = 1;
        isSignificance = false;
        hackLimSig = false;
        doLocal = false;
        
	//        basePathSingLep = "/data/users/bcalvert/SingleLeptonCards/Iteration3_";
	//        basePathSingLep = "/data/users/bcalvert/SingleLeptonCards/Iteration4_";
	//        basePathSingLep = "/data/users/bcalvert/SingleLeptonCards/Iteration5_";
        basePathSingLep = "/data/users/bcalvert/SingleLeptonCards/Iteration6_";
        
        numIterations = 20;
    }
    
    void SetNumIterationsSignif(int massStop, int massLSP) {
        int deltaM = massStop - massLSP;
        
        if (isExpLim == 0) {
            if (massStop < 400) {
                numIterations = 50;
                if (deltaM < 250) {
                    numIterations = 100;
                    if (massLSP < 150) {
                        numIterations = 200;
                    }
                }
            }
        }
        else {
            if (massStop < 400 && massLSP < 150) {
                numIterations = 50;
            }
            if (massStop < 300 && massLSP < 100) {
                numIterations = 100;
                if (massLSP < 50) {
                    numIterations = 200;
                }
            }
        }
    }
    
    void FillCMSVectors(vector<float> * inVecLumis, vector<TString> * inVecAnaTitles) {
        if (typeLepComboAnaType == 1) {
            inVecLumis->push_back(19.5);
            inVecAnaTitles->push_back(" (1-L)");
        }
        else {
            inVecLumis->push_back(19.7);
            inVecAnaTitles->push_back(" (2-L)");
            if (typeLepComboAnaType == 2) {
                inVecLumis->push_back(19.5);
                inVecAnaTitles->push_back(" (1-L)");
            }
        }
    }
    
    void PrintStrings() {
        cout << "strFullSel: " << strFullSel << endl;
        cout << "strDilepChannel: " << strDilepChannel << endl;
        cout << "strDilepAnaType: " << strDilepAnaType << endl;
        cout << "strCutType: " << strCutType << endl;
        cout << "strDim: " << strDim << endl;
        cout << "strLepComboAnaType: " << strLepComboAnaType << endl;
        cout << "strAsympLims: " << strAsympLims << endl;
        cout << "strExpLim: " << strExpLim << endl;
        cout << "strSigContam: " << strSigContam << endl;
        cout << "strParamAppend: " << strParamAppend << endl;
        cout << "strBaseNameCard: " << strBaseNameCard << endl;
        cout << "strBaseNameLim: " << strBaseNameLim << endl;
        cout << "strBaseNameSignificance: " << strBaseNameSignificance << endl;
    }
    
    bool DoBest() {
        return typeLepComboAnaType == 0 && typeDilepAna == 0 && doMT2Scan && isExpLim == 1;
    }
    
    TString NameSingLepFile() {
        TString strUnder = "_";
        TString baseNameSLShape = "h_SingLepOneBinShape";
        TString strFullSel_SingLep = "_BaseFullSel";
        cout << "using a forced full selection string for single lepton" << endl;
        baseNameSLShape += strFullSel_SingLep + strSigContam + strUnder;
        return baseNameSLShape;
    }
    TString WhichLimitCondorString(bool isMT2Scan) {
        TString strUnder = "_";
        TString strWhichLimitBase = "";
        strWhichLimitBase = strLepComboAnaType + strDilepAnaType + strUnder;
        if (simFakes) strWhichLimitBase += strSimFakes + strUnder;
        if (typeDilepChannel > -1) strWhichLimitBase += strDilepChannel;
        if (isMT2Scan) {
            strWhichLimitBase += strCutType;
        }
        else {
            if (typeLepComboAnaType == 0) {
                strWhichLimitBase += TString("Best") + strCutType;
            }
            else {
                strWhichLimitBase += strCutType;
            }
        }
        return strWhichLimitBase;
    }
    
    TString BaseNameShapeCard() {
        TString strUnder = "_";
        TString baseNameShapeCard = "ShapeCard";
        baseNameShapeCard += typeLepComboAnaType > 0 ? "Combo" : "";
        baseNameShapeCard += "_NonTTBar";
        baseNameShapeCard += strDim + strUnder + "SIGNALPATSY";
        if (splitDilepChannel) {
            baseNameShapeCard += "_SplitDilep";
        }
        baseNameShapeCard += strSigContam;
        if (isSignificance) {
//            baseNameShapeCard += "_NoGhost";
        }
        if (usePowhegShapeSyst) {
            baseNameShapeCard += "_wGenShapeSyst";
        }
        baseNameShapeCard += TString(".txt");
        return baseNameShapeCard;
    }
    TString BestLimitString(int whichString) {
        //whichString -- 0: Limit card, 1: Limit file
        TString outString = "Best";
        outString += whichString ? strBaseNameLim : strBaseNameCard;
        return outString;
    }
    
    TString CondorBaseLogNameAppend() {
        TString strUnder = "_";
        TString strLim = isSignificance ? "Signif" : "Lim";
        TString outString = strLepComboAnaType + strFullSel + strExpLim + strAsympLims + strLim + strSigContam + strDilepAnaType + strSimFakes + strPowShape;
        if (typeDilepChannel > -1) outString += strDilepChannel;
        if (typeDilepAna > 0) outString += strDim;
        if (splitDilepChannel) {
            outString += "_SplitDilep";
        }
        outString += strFixBin;
        outString += strUnder;
        return outString;
    }
    
    
    //Functions for the dilepton analysis parameters
    
    vector<TString> GetLatexAnaString() {
        TString arrAnaType[3] = {"", "1-L", "1-L + "};
        TString arrDilepType[2] = {"2-L: C & C", "2-L: M_{T2} Shape"};
        TString baseStringAnaType = arrAnaType[typeLepComboAnaType];
        TString appendNamesTTBarGens[4] = {"Exc. MadGraph", "MC@NLO", "Powheg", "MadGraph"};
        if (typeLepComboAnaType != 1) {
//            baseStringAnaType += ", " + arrDilepType[typeDilepAna];
            baseStringAnaType += "" + arrDilepType[typeDilepAna];
        }
        
        TString arrDilepSel[8] = {"Shape", "Shape + 2 b", "Shape + #frac{#slash{E}_{T}}{#sqrt{H_{T}}}", "Shape + #slash{E}_{T} sig.", "Shape in ZCR", "Shape for 0 b", "Shape + 0b CR", "Shape + 0b & Z CRs"};
        TString stringDilepAnaSel = "";
        if (typeDilepAna > 0) {
            stringDilepAnaSel += "Sel: ";
            stringDilepAnaSel += arrDilepSel[typeFullSel];
        }
        
        vector<TString> outVecAnaStrings(0);
        outVecAnaStrings.push_back(baseStringAnaType);
        outVecAnaStrings.push_back(stringDilepAnaSel);
        if (whichTTbarGen != 2) {
            outVecAnaStrings.push_back(appendNamesTTBarGens[whichTTbarGen]);
        }
        if (simFakes) {
            outVecAnaStrings.push_back("w/ Sim. Fakes");
        }
        if (splitDilepChannel) {
            outVecAnaStrings.push_back("w/ Ind. Dilep Chan.");
        }
        if (usePowhegShapeSyst) {
            outVecAnaStrings.push_back("w/ t#bar{t} Gen-Shape Syst.");
        }
        return outVecAnaStrings;
    }
    TString SetHistAppendString_WhichFullSel() {
        TString baseString = "_FullCut";
        TString arrHistAppend_WhichFullSel[9] = {"", "2BJets", "METSig1D", "METSigTrue", "_inZ_ZCR", "_0BJets", "Bogus", "Bogus", "2BJets"};
        TString histAppendString = baseString + arrHistAppend_WhichFullSel[typeFullSel];
        if (typeFullSel == 4) {
            histAppendString.Replace(histAppendString.Index("_FullCut"), 8, "");
        }
        if (typeDilepAna > 0 && typeFullSel < 8) histAppendString += "_LowBLepMass";
        return histAppendString;
    }
    vector<TString> GetNamesDilepChannels() {
        vector<TString> vecDilepType{"_MuMu", "_EE", "_EMu"};
        return vecDilepType;
    }
    void SetSavingString_DileptonParams() {
        TString strUnder = "_";
        TString arrWhichFullSel[9] = {"BaseFullSel", "2BJetsFullSel", "METSig1DFullSel", "METSigTrueFullSel", "ZCR", "0BJetsFullSel", "FullBJetCombo", "FullCRCombo", "2BJetsFullSel"};
        
        TString arrShapeOrCC[2] = {"CC", "Shape"};
        TString arrANDOR[2] = {"CutAnd", "CutOr"};
        TString arrDim[4] = {"", "OneDee", "TwoDee", "ThreeDee"};
        
        vector<TString> vecDilepType = GetNamesDilepChannels();
        strFullSel = strUnder + arrWhichFullSel[typeFullSel];
        if (typeDilepAna > 0 && typeFullSel < 8) strFullSel += "_LowBLepMass";
        
        TString appendNamesTTBarGens[4] = {"_Madgraph_Exclusive", "_MCatNLO", "_Powheg", "_Madgraph"};
        if (whichTTbarGen != 2) {
            strFullSel += appendNamesTTBarGens[whichTTbarGen];
        }
        
        strDilepAnaType = strUnder + arrShapeOrCC[typeDilepAna];
        strCutType = strUnder + arrANDOR[typeCut];
        strDim = strUnder + arrDim[numDims];
        strDilepChannel = typeDilepChannel > -1 ? vecDilepType[typeDilepChannel] : "";
        strFixBin = "";
        if (fixDMBin > -1) {
            strFixBin = "_FixBin";
            strFixBin += fixDMBin;
        }
        strSimFakes = simFakes ? "_SimFakes" : "";
        
        strPowShape = usePowhegShapeSyst ? "_wGenShapeSyst" : "";
    }
    void SetDileptonParams(int whichFullSel, int whichDilepAna, int useOR, int whichDilepType, int shapeDims = 0) {
        typeFullSel = whichFullSel;
        typeDilepAna = whichDilepAna;
        typeDilepChannel = whichDilepType;
        typeCut = useOR;
        numDims = shapeDims;
        
        SetSavingString_DileptonParams();
    }
    
    void SetSavingString_LimitParams() {
        TString strUnder = "_";
        TString arrAsymp[2] = {"FullFreq", "Asymp"};
        TString arrWhichLepAnaType[3] = {"DiLep", "SingLep", "LepCombo"};
        TString arrExpLim[4] = {"Obs", "Exp", "ExpOneSigUp", "ExpOneSigDown"};
        TString arrSigContam[2] = {"NoSigContam", "WithSigContam"};
        
        strLepComboAnaType = strUnder + arrWhichLepAnaType[typeLepComboAnaType];
        strAsympLims = strUnder + arrAsymp[useAsymLims];
        strExpLim = strUnder + arrExpLim[isExpLim];
        strSigContam = strUnder + arrSigContam[useSigContam];
    }
    
    void SetLimitParams(int whichLepAnaType, bool doAsymp, int doExpLims, bool doSC) {
        typeLepComboAnaType = whichLepAnaType;
        useAsymLims = doAsymp;
        isExpLim = doExpLims;
        useSigContam = doSC;
        SetSavingString_LimitParams();
    }
    void SetAppendString() {
        TString strUnder = "_";
        TString strShape = typeDilepAna == 0 ? strCutType : strDim;
        
        strParamAppend = strLepComboAnaType + strFullSel + strSigContam + strDilepAnaType + strSimFakes;
        if (grabOld) {
            strParamAppend = strLepComboAnaType + TString("_BaseFullSel") + strSigContam + strDilepAnaType;
        }
        if (typeDilepChannel > -1) {
            strParamAppend += strDilepChannel;
        }
        if (splitDilepChannel) {
            strParamAppend += "_SplitDilep";
        }
        strParamAppend += strFixBin;
        strParamAppend += strShape;
        strParamAppend += strPowShape;
        strParamAppend += "_";
    }
    
    void SetNames_Card_Limit() {
        TString strUnder = "_";
        strBaseNameCard = "LimitCard";
        if (isSignificance || hackLimSig) {
            strBaseNameCard = "SignifCard";
        }
        strBaseNameLim = "Limit";
        strBaseNameSignificance = "Signif";
        
        strBaseNameCard += strParamAppend;
        
        strBaseNameLim += strExpLim + strAsympLims;
        strBaseNameLim += strParamAppend;
        if (hackLimSig) {
            strBaseNameLim += "HackLimSig_";
        }
        
        strBaseNameSignificance += strExpLim + strAsympLims;
        strBaseNameSignificance += strParamAppend;
    }
    
    void SetStrings() {
        SetSavingString_DileptonParams();
        SetSavingString_LimitParams();
        SetAppendString();
        SetNames_Card_Limit();
        basePathSingLep += useSigContam ? "WithSC/" : "WithoutSC/";
    }
    void SetAsSingLep() {
        isSignificance = 0;
        typeFullSel = 0;
        typeDilepChannel = -1;
        typeDilepAna = 0;
        typeLepComboAnaType = 1;
        fixDMBin = -1;
        splitDilepChannel = false;
        SetStrings();
    }
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        for (int iLimParam = 0; iLimParam < argc; ++iLimParam) {
            if (strncmp (argv[iLimParam],"-tSel", 5) == 0) {
                typeFullSel = strtol(argv[iLimParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iLimParam],"-tDilep", 7) == 0) {
                typeDilepAna = strtol(argv[iLimParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iLimParam],"simFakes", 8) == 0) {
                simFakes = true;
            }
            else if (strncmp (argv[iLimParam],"-wDilep", 7) == 0) {
                typeDilepChannel = strtol(argv[iLimParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iLimParam],"whichDL", 7) == 0) {
                typeDilepChannel = strtol(argv[iLimParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iLimParam],"-tCut", 5) == 0) {
                typeCut = strtol(argv[iLimParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iLimParam],"numDims", 7) == 0) {
                numDims = strtol(argv[iLimParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iLimParam],"-tCombo", 7) == 0) {
                typeLepComboAnaType = strtol(argv[iLimParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iLimParam],"-", 7) == 0) {
                typeLepComboAnaType = strtol(argv[iLimParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iLimParam],"doAsymp", 5) == 0) {
                useAsymLims = 1;
            }
            else if (strncmp (argv[iLimParam],"doObsLim", 8) == 0) {
                isExpLim = 0;
            }
            else if (strncmp (argv[iLimParam],"whichExp", 8) == 0) {
                isExpLim = strtol(argv[iLimParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iLimParam],"noSC", 4) == 0) {
                useSigContam = 0;
            }
            else if (strncmp (argv[iLimParam],"doSC", 4) == 0) {
                useSigContam = 1;
            }
            else if (strncmp (argv[iLimParam],"splitDilep", 10) == 0) {
                splitDilepChannel = true;
            }
            else if (strncmp (argv[iLimParam],"doSignif", 8) == 0) {
                isSignificance = true;
            }
            else if (strncmp (argv[iLimParam],"useSignifForLimit", 17) == 0) {
                hackLimSig = true;
            }
            else if (strncmp (argv[iLimParam],"doLocal", 7) == 0) {
                doLocal = true;
            }
            else if (strncmp (argv[iLimParam],"fixBin", 6) == 0) {
                fixDMBin = strtol(argv[iLimParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iLimParam],"grabOld", 7) == 0) {
                grabOld = true;
            }
            else if (strncmp (argv[iLimParam],"usePowShape", 11) == 0) {
                usePowhegShapeSyst = true;
            }
            else if (strncmp (argv[iLimParam],"wTTbarGen", 9) == 0) {
                whichTTbarGen = strtol(argv[iLimParam + 1], NULL, 10 );
            }
            else if (strncmp (argv[iLimParam],"noScan", 6) == 0) {
                doMT2Scan = false;
            }
            else if (strncmp (argv[iLimParam],"basePathSL", 10) == 0) {
                basePathSingLep = TString(argv[iLimParam + 1]);
            }
        }
    }
};
#endif
