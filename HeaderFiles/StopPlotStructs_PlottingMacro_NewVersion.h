// Structs used as part of setting up the plotting macros

using namespace std;
typedef vector<TH1F *> histProjVec;
typedef pair<int, int> intBounds;
typedef pair<TString, intBounds> indMCParams;
typedef struct SampLoadSettings {
    //// Struct that contains all the settings that affect which files are loaded, what kind of plots are loaded, etc.
    
    SampMakingVariables SMV;
    
    bool noType0;            // for grabbing version of MET with no Type 0 corrections
    bool doNonSig;          // For whether or not to grab SM background and data (used in MT2CutYieldCalc.C)
    bool doSignal;          // For whether or not to grab a signal point
    bool multSigPts;          // For whether or not to show multiple different signal points (As of 8/5/13 this functionality hasn't been added in yet)
    TString typeSMS;         // Which type of SMS to grab -- either "T2tt" or "T2bw" (as of 8/5/13) only has T2tt FineBin
    TString prefixT2tt;      // prefix for which kind of T2tt to grab
    vector<int> vecStopMassGrab;      // vector to hold the list of Stop masses to grab
    vector<int> vecChi0MassGrab;      // vector to hold the list of Chi0 masses to grab
    vector<double> vecCharginoMassGrab;   // vector to hold the list of Chargino masses to grab
    
    
    TString customPath;
    
    bool multiChannelAdd;
    TString multiChannelSetupFile;
    
    TString multiHistSetupFile;
    bool multiHistList;
    bool readHistListFile;
    TString indHistName;
    
    bool justMT2Plots;
    
    
    //   int  versNumber     = 1; //nominally was for subLepPt > 20 cut version, but this is deprecated for now
    
    //// histogram affecting things
    int whichChan;          //which "channel" to run on, this doesn't just mean ee, emu, or mumu, but can mean events in the full cut sequence, events in the ZMass window but otherwise full cut sequence, etc.
    bool SmearedPlots;
    int  JetsSmeared;
    
    void DefaultVarVals() {
        SMV.DefaultVarVals();        
        
        whichChan = 0;
        noType0 = 0;
        doNonSig = 0;
        doSignal = 0;
        multSigPts = 0;
        typeSMS = "";
        prefixT2tt = "";
        doPURW = 1;
        doSyst = 1;
        
        
        multiHistList = 0;
        readHistListFile = 0;
        indHistName = "";
        
        multiChannelAdd = 0;
        multiChannelSetupFile = "";
        
        
        multiHistSetupFile = "";
        
        justMT2Plots   = 0;
        
        customPath = "";
    }
    TString SignalString(int sigIndex) {
        // return the appropriate name for the signal file
        
        bool isT2tt = prefixT2tt.Contains("T2tt");
        TString sigString = "";
        sigString += prefixT2tt;
        sigString += "/";
        sigString += prefixT2tt;
        sigString += "_SignalStop_";
        sigString += vecStopMassGrab[sigIndex];
        sigString += "_Chi0_";
        sigString += vecChi0MassGrab[sigIndex];
        if (!isT2tt) {
            sigString += "_Chargino";
            sigString += vecCharginoMassGrab[sigIndex];
        }
        return sigString;
    }
    TString SignalLegendString(int sigIndex) {
        // return the legend entry string for the signal given the input parameters 
        TString legEntry;
        TString stringModelBase = "";
        TString stringStopMass = "M_{#tilde{t}}: ";
        TString stringChi0Mass = "M_{#chi_{0}}: ";
        //    TString stringCharginoMass = "M_{#chi^{#pm}}: ";
        TString stringCharginoMass = "#chi_{1}^{#pm} x ";
        legEntry = stringModelBase + stringStopMass;
        legEntry += vecStopMassGrab[sigIndex];
        legEntry += TString(", ");
        legEntry += stringChi0Mass;
        legEntry += vecChi0MassGrab[sigIndex];
        if (typeSMS.Contains("T2bw")) {
            legEntry += TString(", ");
            legEntry += stringCharginoMass;
            legEntry += vecCharginoMassGrab[sigIndex];
        }
        return legEntry;
    }
    TString SampleStringEnd(IndSamplePlotInfo * inISPI) {
        // how to end the input file based on what kind of sample you're grabbing
        TString outString = inISPI->nameISPI;
        outString += SMV.SampString(inISPI->sampleType);
        return outString;
    }
    void SetMultiChannelAddList(vector<int> * vecMultiChannelListIDs, vector<SampleT> * inputSubSampVec, TString &multiChannelCompOutputName) {
        // used for setting up the multi-channel combo histograms
        
        char Buffer[500];
        ifstream * multiChannelStream;        
        multiChannelStream = new ifstream(multiChannelSetupFile);
        if (!(multiChannelStream->eof())) {
            multiChannelStream->getline(Buffer, 500);
            cout << " first line " << TString(string(Buffer)) << endl;
            multiChannelCompOutputName = TString(string(Buffer));
            while (!(multiChannelStream->eof())) {
                multiChannelStream->getline(Buffer, 500);
                cout << " next line " << TString(string(Buffer)) << endl;
                if (TString(string(Buffer)).EqualTo("")) {
                    continue;
                }
                else {
                    vecMultiChannelListIDs->push_back(ChannelID(inputSubSampVec, TString(Buffer)));
                }
            }
        }
        else {
            cout << "Input MultiChannelAdd file is EMPTY!!! " << endl;
        }
    }    
    void SetMultiHistList(vector<int> * vecMultiHistListIDs, vector<HistogramT> * inHistTVec) {
        // used for setting up a list of multiple histograms one wishes to make at once
        char Buffer[500];
        ifstream * multiChannelStream;
        if (readHistListFile) {
            multiChannelStream = new ifstream(multiHistSetupFile);
            if (!(multiChannelStream->eof())) {
                multiChannelStream->getline(Buffer, 500);
                cout << " Category of plots being made " << TString(string(Buffer)) << endl;
                while (!(multiChannelStream->eof())) {
                    multiChannelStream->getline(Buffer, 500);
                    cout << " next line " << TString(string(Buffer)) << endl;
                    if (TString(string(Buffer)).EqualTo("")) {
                        continue;
                    }
                    else {
                        vecMultiHistListIDs->push_back(HistID(inHistTVec, TString(Buffer)));
                    }
                }
            }
            else {
                cout << "Input MultiHistAdd file is EMPTY!!! " << endl;
            }
        }
        else {            
            vecMultiHistListIDs->push_back(HistID(inHistTVec, indHistName));
        }
    }
    void SetupMultiHist_MultiChanLoading(vector<int> * vecMultiHistListIDs, vector<HistogramT> * inHistTVec, vector<int> * vecMultiChannelListIDs, vector<SampleT> * inputSubSampVec, TString &multiChannelCompOutputName) {
        // handle both multihist list setting and multichannel adding
        
        if (multiHistList) {
            SetMultiHistList(vecMultiHistListIDs, inHistTVec);
        }
        if (multiChannelAdd) {
            SetMultiChannelAddList(vecMultiChannelListIDs, inputSubSampVec, multiChannelCompOutputName);
        }
    }
    
    
    
    
    void SetupMultiHistList_MultiChanList_MT2CutYield(vector<TString> * vecMultiHistNames, bool calcByHand, vector<float> * vecMT2llCut, vector<float> * vecMT2lbCut) {
        // set up multi hist and multi-channel adding in the context of calculating MT2 yields
        TString plotGrabBaseName = SmearedPlots ? "h_Smear" : "h_";
        vector<TString> addString(0);
        for (unsigned int iCut = 0; iCut < vecMT2llCut->size(); ++iCut) {
            addString.push_back("");
        }
        if (calcByHand) {
            plotGrabBaseName += "MT2ll";
            plotGrabBaseName += vecMT2lbCut->size() > 0 ? "_vs_MT2lb" : "";
        }
        else {
            plotGrabBaseName += "MT2ll";
            for (unsigned int iCut = 0; iCut < vecMT2llCut->size(); ++iCut) {
                addString[iCut] += (int) vecMT2llCut->at(iCut);
            }
        }
        for (unsigned int iCut = 0; iCut < addString.size(); ++iCut) {
            vecMultiHistNames->push_back(plotGrabBaseName + addString[iCut]);
        }              
    }    
} SampLoadSettings;
typedef struct HistPlotMaking{
    // struck that contains variables that affect what histograms are made and any special modifications thereof
    
    
    bool addThings;          // Add together similar kinds of events (for aesthetic reasons) like VV backgrounds -- (6/25/13) don't turn off for now haven't validated code fully when not adding
    bool calcTTBarNorm;         // calculate TTBar normalization by utilizing integral to data - (other backgrounds) for MT2ll < 80 in the "Full Cut region"
    bool useDDEstimate;         // whether or not to use data-driven estimates for appropriate backgrounds -- (3/28/14) this has now been split into two classes, ttbar normalization and DY normalization
    bool useDDEstimate_TTBar, useDDEstimate_DY;
    int versDDEst_DY;
    
    
    bool doOneDee;          // Plots 1D histograms, as in the 1D HistTs defined in StopFunctionDefinitions_v2.h
    bool doTwoDee;          // Plots 2D histograms, as in the 2D HistTs defined in StopFunctionDefinitions_v2.h -- note, this involves making projections and what-not so there (as of 7/10/13) isn't actual plotting of 2D histograms
    bool doThreeDee;          // Plots 3D histograms, as in the 3D HistTs defined in StopFunctionDefinitions_v2.h -- note, this involves making projections and what-not so there (as of 7/10/13) isn't actual plotting of 3D histograms
    bool doIsoPlots;          // Plots the electron isolation variables to check what is the deal with them for Oviedo vs. DESY
    bool reportChi2;          // nominally, reports data/MC chi2 value in the control region
    bool doCDF;          // nominally, creates Cumulative Distribution Functions for MC and Data -- (6/25/13) haven't tested yet
    bool doVerbosity;
    
    
    bool doFOM, doOneDeeFOM, doTwoDeeFOM, printFOMInfo;
    int  typeFOM;
    TString FOMString;
    bool doYieldV1;
    bool doYieldV2;
    
    
    bool dispIndSource;    
    int  MT2llaxisCut;
    int  MT2lbaxisCut;
    bool tryCalcPassByHand;
    float inputMT2llCut;
    bool  cutMT2lb;
    float inputMT2lbCut;
    
    
    void DefaultVarVals() {
        addThings     = 1;
        calcTTBarNorm = 0;
        useDDEstimate = 0;
        useDDEstimate_TTBar = 0;
        useDDEstimate_DY = 0;
        versDDEst_DY = -1;
        doOneDee      = 1;
        doTwoDee      = 0;
        doThreeDee    = 0;
        doIsoPlots    = 0;
        reportChi2    = 0;
        doCDF         = 0;
        
        doVerbosity = 0;
        
        doFOM           = 0;
        typeFOM         = 0;
        FOMString       = "";
        doOneDeeFOM     = 0;
        doTwoDeeFOM     = 0;
        doYieldV1       = 1;
        doYieldV2       = 0;
        printFOMInfo    = 0;
        
        dispIndSource = 0;
        
        MT2llaxisCut       = 80;
        MT2lbaxisCut       = 170;
        tryCalcPassByHand  = 0;
        inputMT2llCut     = 80;
        cutMT2lb        = 0;
        inputMT2lbCut   = 170;
    }
    
    void SetCutValues(vector<float> * vecMT2llCut, vector<float> * vecMT2lbCut) {
        float MT2llCutsArray[5] = {80.01, 90.01, 100.01, 110.01, 120.01};
        if (tryCalcPassByHand) {
            vecMT2llCut->push_back(inputMT2llCut);
            if (cutMT2lb) {
                vecMT2lbCut->push_back(inputMT2lbCut);
            }
        }
        else {
            for (int iMT2ll = 0; iMT2ll < 5; ++iMT2ll) {
                vecMT2llCut->push_back(MT2llCutsArray[iMT2ll]);
            }
        }
    }        
} HistPlotMaking;

typedef struct GlobalHistSettings {
    // global settings applied to the plotting of histograms
    
    bool doStats;          // show statistics for data and mc spectra on plots -- e.g. mean and RMS -- (6/25/13) haven't tested yet to see how positioning looks
    bool doAbsRatio;          // Affects what gets shown in fractional ratio plots -- 0: (MC - Data) / Data, 1: Data/MC
    bool doSmartFracRatio;    // If on, makes it so that the ratio plot centers itself to include the ratio plot (i.e. matters when the ratio plot is going out of the nominal axis
    bool doSmoothSyst;
    bool showLegend;
    void DefaultVarVals() {
        doSmartFracRatio = 0;
        doStats         = 0;
        doAbsRatio      = 1;
        showLegend      = 1;
        doSmoothSyst    = 0;
    }
} GlobalHistSettings;

typedef struct GlobalSaveInfo {
    // global variables used as part of saving outputs
    
    TString saveName;
    bool saveDotCFile; // If on, saves a .C version of every plot that is made
    bool makePlots;
    void DefaultVarVals() {
        saveDotCFile = 0;
        makePlots = 1;
    }
    void SetSaveName(SampLoadSettings * inSLS, GlobalHistSettings * inGHS, HistPlotMaking * inHPM) {
        TString TTBarGenName[3] = {"_madgraph", "_mcatnlo", "_powheg"};
        TString nameNTuple = "_Ovi";
        TString stringDDEstimate = (inHPM->useDDEstimate) ? "_wDDEst" : "";
        TString stringExcSamp = (inSLS->doExcSamps) ? "_ExcSamps" : "";
        TString stringSignal = "";
        if (inSLS->doSignal) {
            stringSignal = "_wSignal";
            if (inSLS->multSigPts) {
                stringSignal += "MultSigPts";
            }
            else {
                stringSignal += "_Stop_";
                stringSignal += inSLS->vecStopMassGrab[0];
                stringSignal += "_Chi0_";
                stringSignal += inSLS->vecChi0MassGrab[0];
                if (inSLS->typeSMS.Contains("T2bw")) {
                    stringSignal += "_Chargino_";
                    stringSignal += inSLS->vecCharginoMassGrab[0];
                    stringSignal.Replace(stringSignal.First("."), 1, "pt");
                }
            }
        }   
        
        saveName = TTBarGenName[inSLS->whichTTbarGen];        
        saveName += nameNTuple;
        saveName += stringDDEstimate;
        saveName += stringExcSamp;
        saveName += stringSignal;
        if (inSLS->doReReco) saveName += "_ReReco";
        if (inGHS->doAbsRatio) saveName += "_AbsRatio";        
        if (inSLS->noType0) saveName += "_noType0";
        if (!inGHS->showLegend) saveName += "_noLegend";
        if (inSLS->SmearedPlots) saveName += "_SmearedMET";
        if (inGHS->doSmoothSyst) saveName += "_SmoothedSysts";
        if (inSLS->useUnblindedData) saveName += "_UnblindedData";
        if (inSLS->noGenRecRW) saveName += "_noGenRecRW";        
    }
} GlobalSaveInfo;

typedef struct RunParams {
    // General overarching structure that contains all the various running parameter structs
    
    GlobalSaveInfo GSI;
    SampLoadSettings SLS;
    AxisProjInfo API;
    GlobalHistSettings GHS;
    HistPlotMaking HPM;
    void DefaultVarVals() {
        GSI.DefaultVarVals();
        SLS.DefaultVarVals();
        HPM.DefaultVarVals();
        API.DefaultVarVals();
        GHS.DefaultVarVals();   
    }
    void SetGSISaveName() {
        GSI.SetSaveName(&SLS, &GHS, &HPM);
    }
    void SetVals(int argc, char * argv[]) {
        for (int k = 0; k < argc; ++k) {
            cout << "argv[k] for k = " << k << " is: " << argv[k] << endl;
            if (strncmp (argv[k],"wChan", 5) == 0) {
                SLS.whichChan = strtol(argv[k+1], NULL, 10 );
            }
            else if (strncmp (argv[k],"-p", 2) == 0) {
                SLS.customPath = TString(argv[k+1]);
            }
            else if (strncmp (argv[k],"noType0", 7) == 0) {
                SLS.noType0 = 1;
            }  
            else if (strncmp (argv[k],"doSignal", 8) == 0) {
                SLS.doSignal = 1;
                cout << "NB: Format for post command line arguments is TypeSMS, prefix for the file to grab, StopMassToGrab, Chi0MassToGrab, CharginoMassToGrab " << endl;
                SLS.typeSMS = TString(argv[k+1]);
                SLS.prefixT2tt = TString(argv[k+2]);
                SLS.vecStopMassGrab.push_back(strtol(argv[k+3], NULL, 10 ));
                SLS.vecChi0MassGrab.push_back(strtol(argv[k+4], NULL, 10 ));            
                SLS.vecCharginoMassGrab.push_back(strtod(argv[k+5], NULL));
                //            vecCharginoMassGrab->push_back(strtol(argv[k+5], NULL, 10 ));
            }
            else if (strncmp (argv[k],"jMT2P", 5) == 0) {
                SLS.justMT2Plots = 1;
            }
            else if (strncmp (argv[k],"multiChan", 9) == 0) {
                SLS.multiChannelAdd = 1;
                SLS.multiChannelSetupFile = TString(argv[k+1]);
                SLS.multiChannelSetupFile += ".txt";
            }
            else if (strncmp (argv[k],"specHist", 8) == 0) {            
                SLS.multiHistList = 1;
                SLS.readHistListFile = 0;
                SLS.indHistName = TString(argv[k+1]);
            }
            else if (strncmp (argv[k],"listHist", 8) == 0) {            
                SLS.multiHistList = 1;
                SLS.readHistListFile = 1;
                SLS.multiHistSetupFile = TString(argv[k+1]);
                SLS.multiHistSetupFile += ".txt";
            }
            /*
             else if (strncmp (argv[k],"versNum", 7) == 0) {
             versNumber = strtol(argv[k+1], NULL, 10 );
             }   
             */
            else if (strncmp (argv[k],"doNonSig", 8) == 0) {
                SLS.doNonSig = 1;
            } 
            else if (strncmp (argv[k],"noAdd", 5) == 0) {
                HPM.addThings = 0;
            }
            else if (strncmp (argv[k],"doVerbosity", 11) == 0) {
                HPM.doVerbosity = 1;
            }
            else if (strncmp (argv[k],"calcTTBarNorm", 13) == 0) {
                HPM.calcTTBarNorm = 1;
            }
            else if (strncmp (argv[k],"useDDEst", 8) == 0) {
                HPM.useDDEstimate = 1;
                HPM.useDDEstimate_TTBar = 1;
                HPM.useDDEstimate_DY = 1;
                HPM.versDDEst_DY = 1;
            }
            else if (strncmp (argv[k],"useTTBarDDEst", 13) == 0) {
                HPM.useDDEstimate = 1;
                HPM.useDDEstimate_TTBar = 1;
            }
            else if (strncmp (argv[k],"useDYDDEst", 10) == 0) {
                HPM.useDDEstimate = 1;
                HPM.useDDEstimate_DY = 1;
                HPM.versDDEst_DY = strtol(argv[k+1], NULL, 10 );
            }
            else if (strncmp (argv[k],"noOneDee", 8) == 0) {
                HPM.doOneDee = 0;
            }
            else if (strncmp (argv[k],"doTwoDee", 6) == 0) {
                HPM.doTwoDee = 1;
            }
            else if (strncmp (argv[k],"doThreeDee", 8) == 0) {
                HPM.doThreeDee = 1;
            }
            else if (strncmp (argv[k],"doIsoPlots", 10) == 0) {
                HPM.doIsoPlots = 1;
            }
            else if (strncmp (argv[k],"Chi2", 4) == 0) {
                HPM.reportChi2 = 1;
            }
            else if (strncmp (argv[k],"doFOM", 5) == 0) {
                HPM.doFOM = 1;
                HPM.typeFOM = strtol(argv[k+1], NULL, 10 );
                HPM.FOMString = HPM.typeFOM == 0 ? "S/sqrt(S+B)" : "S/sqrt(B)";
            }      
            else if (strncmp (argv[k],"doOneDeeFOM", 11) == 0) {
                HPM.doOneDeeFOM = 1;
            }       
            else if (strncmp (argv[k],"doTwoDeeFOM", 11) == 0) {
                HPM.doTwoDeeFOM = 1;
            }
            else if (strncmp (argv[k],"doYieldV2", 9) == 0) {
                HPM.doYieldV1 = 0;
                HPM.doYieldV2 = 1;
            }
            else if (strncmp (argv[k],"tryCPBH", 7) == 0) {
                HPM.tryCalcPassByHand = 1;
            }
            else if (strncmp (argv[k],"inputMT2ll", 13) == 0) {
                HPM.inputMT2llCut = strtol(argv[k+1], NULL, 10 );
            }
            else if (strncmp (argv[k],"cutMT2lb", 10) == 0) {
                HPM.cutMT2lb = 1;
                HPM.inputMT2lbCut = strtol(argv[k+1], NULL, 10 );
            }
            else if (strncmp (argv[k],"DIS", 3) == 0) {
                HPM.dispIndSource = 1;
            }
            else if (strncmp (argv[k],"pFOMI", 5) == 0) {
                HPM.printFOMInfo = 1;
            }
            else if (strncmp (argv[k],"MT2AxisCuts", 11) == 0) {
                HPM.MT2llaxisCut = strtol(argv[k+1], NULL, 10 );
                HPM.MT2lbaxisCut = strtol(argv[k+2], NULL, 10 );
            }
            else if (strncmp (argv[k],"noLeg", 5) == 0) {
                GHS.showLegend = 0;
            }
            else if (strncmp (argv[k],"noAbsRatio", 10) == 0) {
                GHS.doAbsRatio = 0;
            }
            else if (strncmp (argv[k],"doSFR", 5) == 0) {
                GHS.doSmartFracRatio = 1;
            }
            else if (strncmp (argv[k],"doStats", 7) == 0) {
                GHS.doStats = 1;
            }
            else if (strncmp (argv[k],"sDCF", 4) == 0) {
                GSI.saveDotCFile = 1;
            }
            else if (strncmp (argv[k],"noPlots", 7) == 0) {
                GSI.makePlots = 0;
            }
        }
        SLS.SMV.SetVals(argc, argv);
    }
} RunParams;