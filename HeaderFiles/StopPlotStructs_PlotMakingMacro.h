typedef struct SignalRunSettings {
    // settings that will depend upon command line settings
    bool isSignal;
    int   grabStopMass, grabChi0Mass;
    float grabCharginoMassFrac;
    int stopPolPercentRight;
    
    bool doMassCutSkim;
    float genStopMassCut, genChi0MassCut, genCharginoMassCut;
    
    // settings that won't be changed period or won't change from command line settings
    float weightPol;
    float   massDiffThresh;
    float CharginoMassFracDiffThresh;
    bool isT2tt, isT2ttFineBin, isTightBin;
    
    TString stringSRS;
    
    void DefaultVarVals() {
        isSignal = 0;
        
        grabStopMass = -1;
        grabChi0Mass = -1;
        grabCharginoMassFrac = 0.0; 
        stopPolPercentRight = -1;
        
        doMassCutSkim = 0;
        genStopMassCut = 0;
        genChi0MassCut = 0;
        genCharginoMassCut = 0;
        
        weightPol = 1.0;
        massDiffThresh = 5;
        CharginoMassFracDiffThresh = 0.125;
        isT2tt = false;
        isT2ttFineBin = false;
        isTightBin = false;
        
        stringSRS = "";
    }
    
    void SetSRSString(int whichType) {
        stringSRS = "";
        if (whichType == 2) {
            if (isSignal) {
                stringSRS += "_SignalStop";
                stringSRS += grabStopMass;
                stringSRS += "_Chi0";
                stringSRS += grabChi0Mass;
                if (grabCharginoMassFrac >= 0) {                        
                    stringSRS += "_Chargino";
                    stringSRS += grabCharginoMassFrac;
                }
                stringSRS += "_PercentPol";
                stringSRS += stopPolPercentRight;
            }
        }
    }
} SignalRunSettings;

typedef struct RunRangeLimits {
    // settings that change what portion of the data to run on
    bool doSpecRun, doSpecRunEvent;
    int  whichRun, whichEvent;
    int  nEvents;
    bool isLimStats;
    int startPoint;
    
    TString stringRRL;
    void DefaultVarVals() {
        doSpecRun       = 0;
        doSpecRunEvent  = 0;
        whichRun        = -1;
        whichEvent      = -1;
        nEvents         = -1;     // limits total number of events one is running on
        isLimStats      = 0;
        
        startPoint = 0;
        stringRRL = "";
    }
    void SetRRLString(int whichType) {
        stringRRL = "";
        if (doSpecRun) {
            stringRRL += "_specRun";
        }
        else if (doSpecRunEvent) {
            stringRRL += "_specRunEvent";
        }        
        if (isLimStats) {
            stringRRL += "_isLimStats_";
            stringRRL += nEvents;
        }
    }
} RunRangeLimits;

typedef struct PlotSaveInfoVars {
    bool printEventNum;
    bool doVerbosity, doVerbosity_Plots;
    int  levelJetVerbosity, levelLepVerbosity;
    bool grabOutDir;
    TString outputSavePathString;
    
    TString fInName;
    
    bool useOldNTuple;
    bool useSpecialMET;

    bool makeGenJetCheck;
    void DefaultVarVals() {
        grabOutDir      = 0;      // whether or not to use the file: "outputSavePath.txt" for where to save output
        outputSavePathString = "outputSavePath";
        doVerbosity     = 0;      // prints a lot of debug info if turned on
        doVerbosity_Plots = 0;    // prints a lot of debug info if turned on for plots in particular
        levelLepVerbosity  = 0;
        levelJetVerbosity  = 0;
        
        printEventNum = 0;
        
        useOldNTuple = 1;
        useSpecialMET = 0;

	makeGenJetCheck = 0;
    }
} PlotSaveInfoVars;

typedef struct SampMakingVariables {
    // but also contains parameters about running on the event -- doing PURW-ing?, doing Data?, etc.
    
    TString stringSMV;

    //// general settings that will vary
    int whichDiLepType;

    // plot making settings
    int subLepPtCut;
    
    
    bool  doData, doBlindData; // data specific booleans
    
    //general booleans that should basically always be on -- not necessarily MC or data specific
    bool doPURW, doPhiCorr;
      
    ////MC specific variables
    // MC specific booleans
    bool doBookSyst, doGenRecoil, doOfficialSmear, doDropFakes;
    int typeJetSmear;
    bool SmearedPlots;
    int  whichTTbarGen;
    bool doExcSamps;
    
    bool keepLooseLeps; // boolean for whether or not to keep the loose leptons when running on data
    bool estFakeLep; // boolean for whether or not to run the fake Lepton estimation (for plot making purposes)
    
    void DefaultVarVals() {
        whichTTbarGen = 2; // 0 is Madgraph, 1 is MC@NLO, 2 is Powheg (default is 2)
        doExcSamps = 1; // For grabbing exclusive (DY + N Jets, TTBar Decay modes) or inclusive samples (As of 8/5/13, only applies to Oviedo)
        
        doData = 0;     // Whether you're running on data or not
        doBlindData = 1;
        
        doBookSyst = 1; // used for deciding whether or not to book systematics
        doGenRecoil = 1;
        doOfficialSmear = 0;
        doDropFakes = 0;
        typeJetSmear = 1;
        SmearedPlots = 0;
        
        keepLooseLeps = 0;
        estFakeLep    = 0;
        
        whichDiLepType = -1;
        
        subLepPtCut      = 10;    // Sets the pT cut used for subLepPtCut

        
        doPURW = 1;     // run pile up reweighting
        doPhiCorr = 1;  // whether to do the MetPhi asymmetry correction
        
        stringSMV = "";
    }
    void SetSMVString(int whichType) {
        TString stringDiLep[3] = {"_MuMu", "_EE", "_EMu"};
        
        stringSMV = "";        
        if (whichType == 2) {
            if (doData && !estFakeLep) {
                stringSMV += doBlindData ? "MT2Leq80" : "_NOTBLIND";
            }
        }
        stringSMV += "_Oviedo";
        if (!doPURW && !doData) {
            stringSMV += "_NoPURW";   
        }
        if (typeJetSmear != 1) {
            stringSMV += "_JetsSmeared_";
        }
        if (keepLooseLeps && !estFakeLep) {
            stringSMV += "_LooseLeps";
        }
        if (whichType == 2) {
            if (!doBookSyst && (!doData || (doData && estFakeLep))) stringSMV += "_noSyst";
            if (doOfficialSmear) stringSMV += "_OfficialSmear";
            if (abs(subLepPtCut - 10.) > 1E-3) {
                stringSMV += "_subLepPtCut";
                stringSMV += subLepPtCut;
            }
            if (whichDiLepType > -1) {
                stringSMV += stringDiLep[whichDiLepType];
            }
            if (!doGenRecoil) {
                stringSMV += "_noGenRecRW";
            }
            if (doDropFakes) {
                stringSMV += "_noFakes";
            }
            if (estFakeLep) {
                stringSMV += "_FakeLepDD";
            }
        }
    }
    TString SampString(int sampType) {
        TString outString = "";
        if (sampType == 0) {
            outString += "_OviedoHaddplots";
            outString += !doBlindData ? "_NOTBLIND" : "";
            outString += ".root";
        }
        else if (sampType < 0) {
             outString += "_Output.root";
        }
        else {
            switch(sampType) {
                case 1:
                    outString += TTBarString();
                    break;
                case 2:
                    outString += doExcSamps ? "_Exclusive" : "";
                    break;
                default:
                    break;
            }
            outString += !doPURW ? "_noPURW" : "";
            outString += doBookSyst ? "" : "_noSyst";
            outString += doGenRecoil ? "" : "_noGenRecRW";
            outString += "Haddplots.root";
        }
        return outString;
    }
    TString TTBarString() {
        // return the appropriate ttbar generator name for the input file
        TString appendNamesTTBarGens[3] = {"_Madspin", "_MCatNLO", "_Powheg"};
        return appendNamesTTBarGens[whichTTbarGen];
    }
    TString TTBarSaveString() {
        return "FIXME";
    }
    void SetVals(int argc, char * argv[]) {        
        for (int k = 0; k < argc; ++k) {
            if (strncmp (argv[k],"typeJetSmear", 12) == 0) {
                typeJetSmear = strtol(argv[k+1], NULL, 10);
            }
            else if (strncmp (argv[k],"JsSm", 4) == 0) {
                SmearedPlots = 1;
                typeJetSmear = strtol(argv[k+1], NULL, 10);
            }
            else if (strncmp (argv[k],"noPhiCorr", 9) == 0) {
                doPhiCorr = 0;   
            }        
            else if (strncmp (argv[k],"doOfficialSmear", 9) == 0) {
                doOfficialSmear = 1;
            }
            else if (strncmp (argv[k],"wTTbarGen", 9) == 0) {
                whichTTbarGen = strtol(argv[k+1], NULL, 10 );
            }           
            else if (strncmp (argv[k],"noExcSamps", 10) == 0) {
                doExcSamps = 0;
            }
            else if (strncmp (argv[k],"noFakes", 7) == 0) {
                doDropFakes = 1;
            }
            else if (strncmp (argv[k],"noPURW", 6) == 0) {
                doPURW = 0;           
            }
            else if (strncmp (argv[k],"noBookSyst", 10) == 0) {
                doBookSyst = 0;
            }
            else if (strncmp (argv[k],"noGenRecRW", 10) == 0) {
//                cout << "genRecoil is on! " << endl;
                doGenRecoil = 0;
            }
            else if (strncmp (argv[k],"typeDiLep", 9) == 0) {
                whichDiLepType = strtol(argv[k+1], NULL, 10);
            }
            else if (strncmp (argv[k],"ReleaseTheKraken", 16) == 0) {
                doBlindData = 0;
                cout << "RELEASING THE KRAKEN!!! " << endl;
                cout << "http://www.youtube.com/watch?v=gb2zIR2rvRQ " << endl;
            }       
            else if (strncmp (argv[k],"subLepPt", 8) == 0) {
                subLepPtCut = strtol(argv[k+1], NULL, 10);   
            }
            else if (strncmp (argv[k],"keepLL", 6) == 0) {
                keepLooseLeps = 1;
            }
            else if (strncmp (argv[k],"doFakeLep", 9) == 0) {
                estFakeLep = 1;
                keepLooseLeps = 1;
                doBlindData = 0;
                doBookSyst = 1;
            }
        }
    }
} SampMakingVariables;

typedef struct PlotMakingRunParams {
    SampMakingVariables SMV;
    
    RunRangeLimits RRL;
    SignalRunSettings SRS;
    // settings that affect cout statements and/or where the results are saved
    PlotSaveInfoVars PSIV;
    
    void DefaultVarVals() {
        PSIV.DefaultVarVals();
        RRL.DefaultVarVals();
        SRS.DefaultVarVals();
        SMV.DefaultVarVals();
    }    
    void SetVals(int argc, char * argv[]) {        
        for (int k = 0; k < argc; ++k) {
            cout << "argv[k] for k = " << k << " is: " << argv[k] << endl;
            
            if (strncmp (argv[k],"-i", 2) == 0) { 
                PSIV.fInName = TString(argv[k+1]);
                if (PSIV.fInName.Contains("MuEG") || PSIV.fInName.Contains("DoubleMu") || PSIV.fInName.Contains("DoubleEl") || PSIV.fInName.Contains("run2012")) {
                    cout << "Running on Data" << endl;
                    SMV.doData = 1;
                    SMV.doBookSyst = 0;
                }                
            }
            else if (strncmp (argv[k],"doVerbosity_Skim", 16) == 0) {
                PSIV.doVerbosity = 1;
                PSIV.levelLepVerbosity = strtol(argv[k+1], NULL, 10);
                PSIV.levelJetVerbosity = strtol(argv[k+2], NULL, 10);
            }
            else if (strncmp (argv[k],"-sInd", 5) == 0) {
                RRL.startPoint = strtol(argv[k+1], NULL, 10);           
            }
            else if (strncmp (argv[k],"pEvNum", 6) == 0) {
                PSIV.printEventNum = 1;
            }
            else if (strncmp (argv[k],"doMassCut", 9) == 0) {
                SRS.doMassCutSkim = 1;
                SRS.genStopMassCut = strtol(argv[k+1], NULL, 10);   
                SRS.genChi0MassCut = strtol(argv[k+2], NULL, 10);   
                SRS.genCharginoMassCut = strtol(argv[k+3], NULL, 10);   
            } 
            else if (strncmp (argv[k],"doVerbosity", 11) == 0) {
                PSIV.doVerbosity = 1;   
            }
            else if (strncmp (argv[k],"doPlotVerbosity", 15) == 0) {
                PSIV.doVerbosity_Plots = 1;   
            }
            else if (strncmp (argv[k],"gOutDir", 7) == 0) {
                PSIV.grabOutDir = 1;
            }        
            else if (strncmp (argv[k], "OutDirName", 10) == 0) {
                PSIV.outputSavePathString = TString(argv[k+1]);
            }
            else if (strncmp (argv[k],"doSpecEventRun", 14) == 0) {
                RRL.doSpecRunEvent = 1;
                RRL.doSpecRun = 0;
                RRL.whichRun = strtol(argv[k+1], NULL, 10);
                RRL.whichEvent = strtol(argv[k+2], NULL, 10);
            }
            else if (strncmp (argv[k],"doSpecRun", 9) == 0) {
                RRL.doSpecRun = 1;
                RRL.whichRun = strtol(argv[k+1], NULL, 10);
            }
            else if (strncmp (argv[k],"isSig_Skim", 10) == 0) {
                SRS.isSignal = 1;               
            }
            else if (strncmp (argv[k],"isTightBin", 10) == 0) {
                SRS.isTightBin = 1;
            }
            else if (strncmp (argv[k],"isSig", 5) == 0) {
                SRS.isSignal = 1;
                SRS.grabStopMass = strtol(argv[k+1], NULL, 10);
                SRS.grabChi0Mass = strtol(argv[k+2], NULL, 10);
                SRS.grabCharginoMassFrac = strtod(argv[k+3], NULL);
                SRS.stopPolPercentRight = strtol(argv[k+4], NULL, 10);
                cout << "Looking for StopMass: " << SRS.grabStopMass << ", Chi0Mass: " << SRS.grabChi0Mass << ", and CharginoMassFrac: " << SRS.grabCharginoMassFrac << endl;
                cout << "as well, looking for stop polarization " << SRS.stopPolPercentRight << endl;
            }
            else if (strncmp (argv[k],"limStats", 8) == 0) {
                RRL.isLimStats = 1;
                RRL.nEvents = strtol(argv[k+1], NULL, 10);   
            }
            else if (strncmp (argv[k],"useNew", 6) == 0) {
                PSIV.useOldNTuple = 0;
            }
            else if (strncmp (argv[k],"useSpecMET", 10) == 0) {
                PSIV.useSpecialMET = 1;
            }
	    else if (strncmp (argv[k],"checkDeltEn", 11) == 0) {
	      PSIV.makeGenJetCheck = 1;
            }
        }
        SMV.SetVals(argc, argv);
    }
    void SetStrings(int whichType) {
        SMV.SetSMVString(whichType);
        RRL.SetRRLString(whichType);
        SRS.SetSRSString(whichType);
    }
    TString OutStringAdd() {
        TString outString;
        outString += SMV.stringSMV;
        outString += RRL.stringRRL;
        outString += SRS.stringSRS;
        return outString;
    }
} PlotMakingRunParams;
