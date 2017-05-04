// Structs used as part of setting up the plotting macros

using namespace std;
typedef vector<TH1F *> histProjVec;
typedef pair<int, int> intBounds;
typedef pair<TString, intBounds> indMCParams;
typedef struct SampLoadSettings {
    //// Struct that contains all the settings that affect which files are loaded, what kind of plots are loaded, etc.
    

    int whichTTbarGen;          // 0 is Madgraph, 1 is MC@NLO, 2 is Powheg (default is 2)
    bool doExcSamps;          // For grabbing exclusive (DY + N Jets, TTBar Decay modes) or inclusive samples (As of 8/5/13, only applies to Oviedo)
    bool noGenRecRW;
    bool noType0;            // for grabbing version of MET with no Type 0 corrections
    int whichSSType;
    int whichDilepType;
    bool doDropFakes;
    bool doNonSig;          // For whether or not to grab SM background and data (used in MT2CutYieldCalc.C)
    bool doSignal;          // For whether or not to grab a signal point
    bool multSigPts;          // For whether or not to show multiple different signal points (As of 8/5/13 this functionality hasn't been added in yet)
    TString typeSMS;         // Which type of SMS to grab -- either "T2tt" or "T2bw" (as of 8/5/13) only has T2tt FineBin
    TString prefixT2tt;      // prefix for which kind of T2tt to grab
    vector<int> vecStopMassGrab;      // vector to hold the list of Stop masses to grab
    vector<int> vecChi0MassGrab;      // vector to hold the list of Chi0 masses to grab
    vector<double> vecCharginoMassGrab;   // vector to hold the list of Chargino masses to grab
    vector<int> vecPercentPolGrab;      // vector to hold the list of Chi0 masses to grab
    bool doPURW;          // grab the nVtx reweighted MC files
    bool doSyst;          // look at systematics plots -- (6/25/13) don't turn off for now haven't validated code fully when not making systematicslumi
    TString customPath, customPathSignalFile, customPathSignalNorm;
    
    bool multiChannelAdd;
    TString multiChannelSetupFile;
    
    TString multiHistSetupFile;
    bool multiHistList;
    bool readHistListFile;
    TString indHistName;
    
    int numDims;
    bool doOneDeeHack;
    
    bool doReReco;
    bool useUnblindedData;
    bool justMT2Plots;
    
    
    //   int  versNumber     = 1; //nominally was for subLepPt > 20 cut version, but this is deprecated for now
    
    //// histogram affecting things
    int whichChan;          //which "channel" to run on, this doesn't just mean ee, emu, or mumu, but can mean events in the full cut sequence, events in the ZMass window but otherwise full cut sequence, etc.
    TString whichChanName;
    bool SmearedPlots;
    int  JetsSmeared;
    
    void SetUpSLSForPlots(int inWSS) {
        useUnblindedData = 1;
        whichTTbarGen = 2;
        doDropFakes = 1;
        noGenRecRW = 1;
        SmearedPlots = 1;
        JetsSmeared = 0;
        whichSSType = inWSS;
    }
    
    void DefaultVarVals() {
        whichChan = 0;
        whichChanName = "";
        whichTTbarGen = 2;
        doExcSamps = 1;
        noGenRecRW = 0;
        noType0 = 0;
        doDropFakes = 0;
        doNonSig = 0;
        doSignal = 0;
        multSigPts = 0;
        typeSMS = "";
        prefixT2tt = "";
        doPURW = 1;
        doSyst = 1;
        
        whichSSType = -1;
        whichDilepType = -1;
        
        multiHistList = 0;
        readHistListFile = 0;
        indHistName = "";
        
        multiChannelAdd = 0;
        multiChannelSetupFile = "";
        
        numDims = -1;
        doOneDeeHack = true;
        
        multiHistSetupFile = "";
        
        doReReco       = 1;
        useUnblindedData = 0;
        justMT2Plots   = 0;
        
        SmearedPlots   = 0;
        JetsSmeared    = 0;
        
        customPath = "";
        customPathSignalFile = "../RootFiles/Signal/Dir_";
        customPathSignalNorm = "../PlotMakingCode/Histos_";
    }
    
    void PrintSignalInfo() {
        cout << "prefixT2tt " << prefixT2tt << endl;
        cout << "size of vecs: " << endl;
        cout << "vecStopMassGrab: " << vecStopMassGrab.size() << endl;
        cout << "vecChi0MassGrab: " << vecChi0MassGrab.size() << endl;
        cout << "vecPercentPolGrab: " << vecPercentPolGrab.size() << endl;
        cout << "vecCharginoMassGrab: " << vecCharginoMassGrab.size() << endl;
        for (unsigned int iSig = 0; iSig < vecStopMassGrab.size(); ++iSig) {
            cout << "contents of vecs at iSig = " << iSig << ": " << endl;
            cout << "vecStopMassGrab: " << vecStopMassGrab[iSig] << endl;
            cout << "vecChi0MassGrab: " << vecChi0MassGrab[iSig] << endl;
            cout << "vecPercentPolGrab: " << vecPercentPolGrab[iSig] << endl;
            cout << "vecCharginoMassGrab: " << vecCharginoMassGrab[iSig] << endl;
        }
    }
    
    TString SignalShapeString() {
        TString strDim[4] = {"", "OneDee", "TwoDee", "ThreeDee"};
        if (numDims > -1) {
            return strDim[numDims];
        }
        return "";
    }
    TString ShapeNameAppend() {
        TString strInit = SignalShapeString();
        TString strEnd = doOneDeeHack ? "ToOneDee.root" : ".root";
        return strInit + strEnd;
    }
    TString NameSigFile(int sigIndex, bool doVerbosity = false) {
        bool isT2tt = prefixT2tt.Contains("T2tt");
        TString strSMS = "h_Stop";
        strSMS += vecStopMassGrab[sigIndex];
        strSMS += "_LSP";
        strSMS += vecChi0MassGrab[sigIndex];
        strSMS += isT2tt ? "T2tt" : "T2bw";
        if (isT2tt) {
            strSMS += "PerPol";
            strSMS += vecPercentPolGrab[sigIndex];
        }
        else {
            strSMS += "CharginoFrac0.";
            strSMS += vecCharginoMassGrab[sigIndex];
            strSMS += "_SCharSW";
        }
        return strSMS;
    }
    TString PathStringFile(int sigIndex) {
        bool isT2tt = prefixT2tt.Contains("T2tt");
        TString strSMS = isT2tt ? "T2tt/" : "T2bw/";
        if (isT2tt) {
            strSMS += "PerPol";
            strSMS += vecPercentPolGrab[sigIndex];
        }
        else {
            strSMS += "CharFrac0.";
            strSMS += vecCharginoMassGrab[sigIndex];
            strSMS += "/SCharSW/";
        }
        strSMS += "/";
        strSMS += "Stop";
        strSMS += vecStopMassGrab[sigIndex];
        strSMS += "/";
        strSMS += "LSP";
        strSMS += vecChi0MassGrab[sigIndex];
        strSMS += "/";
        
        return strSMS;
    }
    TString FullPathFile(int whichKind, int sigIndex = 0) {
        TString baseString = "../LimitSetting/ShapeAnalysis/";
        TString arrStrKind[3] = {"Data/", "Backgrounds/", "Signal/"};
        TString sigString = "";
        if (whichKind == 2) {
            //running on signal so grab more
            sigString = PathStringFile(sigIndex);
        }
        
        return baseString + arrStrKind[whichKind] + sigString;
    }
    
    TString TTBarString() {
        // return the appropriate ttbar generator name for the input file
        TString appendNamesTTBarGens[4] = {"_Madgraph_Exclusive", "_MCatNLO", "_Powheg", "_Madgraph"};
        return appendNamesTTBarGens[whichTTbarGen];
    }
    TString SignalStringForFile(int sigIndex) {
        bool isT2tt = prefixT2tt.Contains("T2tt");
        TString strSMS = isT2tt ? "_T2tt" : "_T2bw";
        TString sigString = strSMS;
        sigString += "_Stop";
        sigString += vecStopMassGrab[sigIndex];
        sigString += "_Chi0";
        sigString += vecChi0MassGrab[sigIndex];
        if (!isT2tt) {
            sigString += "_CharFrac";
            sigString += vecCharginoMassGrab[sigIndex];
        }
        else {
            sigString += "_PerPol";
            sigString += vecPercentPolGrab[sigIndex];
        }
        return sigString;
    }
    TString SignalString(int sigIndex) {
        // return the appropriate name for the signal file
        
        TString stringSSType = "";
        if (whichSSType > -1) {
            stringSSType = "_wSS";
            stringSSType += whichSSType;
        }
        
        bool isT2tt = prefixT2tt.Contains("T2tt");
        TString sigString = "";
        sigString += prefixT2tt;
        sigString += "/Tree_";
        sigString += prefixT2tt;
        sigString += "_Oviedo_SkimOutput_Oviedo";
        sigString += stringSSType;
        sigString += "_SignalStop";
        //        sigString += "_SignalStop_";
        sigString += vecStopMassGrab[sigIndex];
        //        sigString += "_Chi0_";
        sigString += "_Chi0";
        sigString += vecChi0MassGrab[sigIndex];
        if (!isT2tt) {
            sigString += "_Chargino";
            sigString += vecCharginoMassGrab[sigIndex];
        }
        sigString += "_PercentPol";
        sigString += vecPercentPolGrab[sigIndex];
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
    TString SampleStringDYLoading() {
        TString MCAppendString = "";
        MCAppendString += doExcSamps ? "_Exclusive" : "";
        TString stringSSType = "";
        if (whichSSType > -1) {
            stringSSType = "_wSS";
            stringSSType += whichSSType;
        }
        MCAppendString += "_Oviedo";
        MCAppendString += !doPURW ? "_noPURW" : "";
        MCAppendString += doSyst ? "" : "_noSyst";
        MCAppendString += stringSSType;
        MCAppendString += "_PATSY";
        MCAppendString += noGenRecRW ? "_noGenRecRW" : "";
        MCAppendString += doDropFakes ? "_noFakes" : "";
        MCAppendString += "Haddplots.root";
        return MCAppendString;
    }
    TString SampleStringScaleFacSaving() {
        TString MCAppendString = "";
        MCAppendString += doExcSamps ? "_Exclusive" : "";
        MCAppendString += !doPURW ? "_noPURW" : "";
        MCAppendString += noGenRecRW ? "_noGenRecRW" : "";
        MCAppendString += doDropFakes ? "_noFakes" : "";
        MCAppendString += ".root";
        return MCAppendString;
    }
    TString SampleStringEnd(IndSamplePlotInfo * inISPI) {
        // how to end the input file based on what kind of sample you're grabbing
        TString outString = inISPI->nameISPI;
        TString MCAppendString = "";
        switch (inISPI->sampleType) {
            case 1:
                outString += TTBarString();
                break;
            case 2:
                MCAppendString += doExcSamps ? "_Exclusive" : "";
                break;                
        }
        
        TString stringSSType = "";
        if (whichSSType > -1) {
            stringSSType = "_wSS";
            stringSSType += whichSSType;
        }
        
        TString strDiLep[4] = {"_MuMu", "_EE", "_EMu", "_SF"};
        TString strDiLepType = "";

        
        MCAppendString += "_Oviedo";
        MCAppendString += !doPURW ? "_noPURW" : "";
        MCAppendString += doSyst ? "" : "_noSyst";
        MCAppendString += stringSSType;
        if (whichDilepType > -1) {
            MCAppendString += strDiLep[whichDilepType];
        }
        MCAppendString += noGenRecRW ? "_noGenRecRW" : "";
        MCAppendString += doDropFakes ? "_noFakes" : "";
        if (inISPI->sampleType > 0) {
            MCAppendString += "Haddplots.root";                       
            outString += MCAppendString;
        }
        else if (inISPI->sampleType == 0) {

            outString += "_OviedoHaddplots";
            outString += useUnblindedData ? "_NOTBLIND" : "";
            outString += ".root";
        }
        else { //is signal
            outString += "_Output.root";
        }
        
        if (inISPI->sampleType == 100) {
            outString = inISPI->nameISPI;
            if (whichDilepType > -1) {
                outString += strDiLep[whichDilepType];
            }
            outString += "_OviedoHaddplots.root";
        }
        
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
    
    
    void SetupMultiHistList_MultiChanList_MT2Saving(vector<TString> * vecMultiHistNames, int levelVerbosity = 0) {
        // set up multi hist and multi-channel adding in the context of calculating MT2 yields
        TString plotGrabBaseName = SmearedPlots ? "h_Smear" : "h_";
        plotGrabBaseName += "MT2ll_vs_MT2lblb_vs_MT2bb_ZMET";
        vecMultiHistNames->push_back(plotGrabBaseName);
        if (levelVerbosity) {
            for (int iHist = 0; iHist < (int) vecMultiHistNames->size(); ++iHist) {
                cout << "MT2 Saving Hist name for iHist = " << iHist << " is " << vecMultiHistNames->at(iHist) << endl;
            }
        }
    }
    
    void SetupMultiHistList_MultiChanList_MT2CutYield(vector<TString> * vecMultiHistNames, bool calcByHand, vector<float> * vecMT2llCut, vector<float> * vecMT2lbCut, int levelVerbosity = 0) {
        // set up multi hist and multi-channel adding in the context of calculating MT2 yields
        TString plotGrabBaseName = SmearedPlots ? "h_Smear" : "h_";
        vector<TString> addString(0);
        for (unsigned int iCut = 0; iCut < vecMT2llCut->size(); ++iCut) {
            addString.push_back("");
        }
        if (calcByHand) {
            plotGrabBaseName += "MT2ll";
            plotGrabBaseName += vecMT2lbCut->size() > 0 ? "_vs_MT2lblb" : "";
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
        if (levelVerbosity) {
            for (int iHist = 0; iHist < (int) vecMultiHistNames->size(); ++iHist) {
                cout << "MT2CutYield Hist name for iHist = " << iHist << " is " << vecMultiHistNames->at(iHist) << endl;
            }
        }
    }
    
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        for (int iSLSParam = 0; iSLSParam < argc; ++iSLSParam) {
            if (strncmp (argv[iSLSParam],"wChan", 5) == 0) {
                whichChan = strtol(argv[iSLSParam + 1], NULL, 10 );
            }
            else if (strncmp (argv[iSLSParam],"nameWChan", 9) == 0) {
                whichChanName = TString(argv[iSLSParam + 1]);
            }
            else if (strncmp (argv[iSLSParam],"-p", 2) == 0) {
                customPath = TString(argv[iSLSParam + 1]);
            }
            else if (strncmp (argv[iSLSParam],"wTTbarGen", 9) == 0) {
                whichTTbarGen = strtol(argv[iSLSParam + 1], NULL, 10 );
            }
            else if (strncmp (argv[iSLSParam],"whichSS", 7) == 0) {
                whichSSType = strtol(argv[iSLSParam + 1], NULL, 10 );
            }
            else if (strncmp (argv[iSLSParam],"whichDL", 7) == 0) {
                whichDilepType = strtol(argv[iSLSParam + 1], NULL, 10 );
            }
            else if (strncmp (argv[iSLSParam],"noType0", 7) == 0) {
                noType0 = 1;
            }
            else if (strncmp (argv[iSLSParam],"noFakes", 7) == 0) {
                doDropFakes = 1;
            }
            else if (strncmp (argv[iSLSParam],"noExcSamps", 10) == 0) {
                doExcSamps = 0;
            }
            else if (strncmp (argv[iSLSParam],"noGenRecRW", 10) == 0) {
                noGenRecRW = 1;
            }
            else if (strncmp (argv[iSLSParam],"doSignal", 8) == 0) {
                doSignal = 1;
                //                cout << "NB: Format for post command line arguments is TypeSMS, prefix for the file to grab, StopMassToGrab, Chi0MassToGrab, CharginoMassToGrab " << endl;
                typeSMS = TString(argv[iSLSParam + 1]);
                prefixT2tt = TString(argv[iSLSParam + 2]);
                vecStopMassGrab.push_back(strtol(argv[iSLSParam + 3], NULL, 10 ));
                vecChi0MassGrab.push_back(strtol(argv[iSLSParam + 4], NULL, 10 ));
                vecCharginoMassGrab.push_back(strtod(argv[iSLSParam + 5], NULL));
                vecPercentPolGrab.push_back(strtol(argv[iSLSParam + 6], NULL, 10 ));
                //            vecCharginoMassGrab->push_back(strtol(argv[iSLSParam + 5], NULL, 10 ));
            }
            else if (strncmp (argv[iSLSParam],"pathSigFile", 11) == 0) {
                customPathSignalFile = TString(argv[iSLSParam + 1]);
                //                cout << "setting sig file path to " << customPathSignalFile << endl;
            }
            else if (strncmp (argv[iSLSParam],"pathSigNorm", 11) == 0) {
                customPathSignalNorm = TString(argv[iSLSParam + 1]);
                //                cout << "setting sig norm file path to " << customPathSignalNorm << endl;
            }
            else if (strncmp (argv[iSLSParam],"ReleaseTheKraken", 16) == 0) {
                useUnblindedData = 1;
                cout << "RELEASING THE KRAKEN!!! " << endl;
                cout << "http://www.youtube.com/watch?v=gb2zIR2rvRQ " << endl;
            }
            else if (strncmp (argv[iSLSParam],"jMT2P", 5) == 0) {
                justMT2Plots = 1;
            }
            else if (strncmp (argv[iSLSParam],"noPURW", 6) == 0) {
                doPURW = 0;
            }
            else if (strncmp (argv[iSLSParam],"noSyst", 6) == 0) {
                doSyst = 0;
            }
            else if (strncmp (argv[iSLSParam],"JsSm", 4) == 0) {
                SmearedPlots = 1;
                JetsSmeared = strtol(argv[iSLSParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iSLSParam],"multiChan", 9) == 0) {
                multiChannelAdd = 1;
                multiChannelSetupFile = TString(argv[iSLSParam + 1]);
                multiChannelSetupFile += ".txt";
            }
            else if (strncmp (argv[iSLSParam],"specHist", 8) == 0) {
                multiHistList = 1;
                readHistListFile = 0;
                indHistName = TString(argv[iSLSParam + 1]);
            }
            else if (strncmp (argv[iSLSParam],"listHist", 8) == 0) {
                multiHistList = 1;
                readHistListFile = 1;
                multiHistSetupFile = TString(argv[iSLSParam + 1]);
                multiHistSetupFile += ".txt";
            }
            else if (strncmp (argv[iSLSParam],"noReReco", 8) == 0) {
                doReReco = 0;
            }
            else if (strncmp (argv[iSLSParam],"doNonSig", 8) == 0) {
                doNonSig = 1;
            }
            else if (strncmp (argv[iSLSParam],"numDims", 7) == 0) {
                numDims = strtol(argv[iSLSParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iSLSParam],"noHack", 6) == 0) {
                doOneDeeHack = false;
            }
        }
    }
} SampLoadSettings;
typedef struct HistPlotMaking{
    // struck that contains variables that affect what histograms are made and any special modifications thereof
    
    
    bool addThings;          // Add together similar kinds of events (for aesthetic reasons) like VV backgrounds -- (6/25/13) don't turn off for now haven't validated code fully when not adding
    bool calcTTBarNorm;         // calculate TTBar normalization by utilizing integral to data - (other backgrounds) for MT2ll < 80 in the "Full Cut region"
    bool useDDEstimate;         // whether or not to use data-driven estimates for appropriate backgrounds -- (3/28/14) this has now been split into two classes, ttbar normalization and DY normalization
    bool useDDEstimate_TTBar, useDDEstimate_DY;
    int versDDEst_TTBar, versDDEst_DY;
    
    
    bool doOneDee;          // Plots 1D histograms, as in the 1D HistTs defined in StopFunctionDefinitions_v2.h
    bool doTwoDee;          // Plots 2D histograms, as in the 2D HistTs defined in StopFunctionDefinitions_v2.h -- note, this involves making projections and what-not so there (as of 7/10/13) isn't actual plotting of 2D histograms
    bool doThreeDee;          // Plots 3D histograms, as in the 3D HistTs defined in StopFunctionDefinitions_v2.h -- note, this involves making projections and what-not so there (as of 7/10/13) isn't actual plotting of 3D histograms
    
    TString strAxis1, strAxis2;
    
    
    bool doIsoPlots;          // Plots the electron isolation variables to check what is the deal with them for Oviedo vs. DESY
    bool reportChi2;          // nominally, reports data/MC chi2 value in the control region
    bool doCDF;          // nominally, creates Cumulative Distribution Functions for MC and Data -- (6/25/13) haven't tested yet
    bool doVerbosity;
    
    
    bool doFOM, doOneDeeFOM, doTwoDeeFOM, printFOMInfo;
    int  typeFOM;
    TString FOMString;
    bool doYieldV1;
    bool doYieldV2;
    int printSysLim;
    bool printAveSyst;
    bool doStopXSec;
    
    bool dispIndSource;    
    int  MT2llaxisCut;
    int  MT2lbaxisCut;
    bool tryCalcPassByHand;
    float inputMT2llCut;
    bool  cutMT2lb;
    float inputMT2lbCut;
    int whichIntType;
    bool doControl;
    
    bool doSaveHistograms;
    int  saveHistoDimension;
    
    TString saveLocation;
    
    TString unitAxis;
    
    float scaleFactor;
    
    void SetUpHPMForPlots() {
        useDDEstimate = 1;
        useDDEstimate_TTBar = 1;
        versDDEst_TTBar = 0;
        useDDEstimate_DY = 1;
        versDDEst_DY = 4;
        
        unitAxis = "GeV / c^{2}";
    }
    
    void DefaultVarVals() {
        scaleFactor = 1.0;
        addThings     = 1;
        calcTTBarNorm = 0;
        useDDEstimate = 0;
        useDDEstimate_TTBar = 0;
        useDDEstimate_DY = 0;
        versDDEst_TTBar = -1;
        versDDEst_DY = -1;
        doOneDee      = 1;
        doTwoDee      = 0;
        doThreeDee    = 0;
        strAxis1      = "";
        strAxis2      = "";
        doIsoPlots    = 0;
        reportChi2    = 0;
        doCDF         = 0;
        
        doSaveHistograms = false;
        saveHistoDimension = -1;
        
        doVerbosity = 0;
        
        doFOM           = 0;
        typeFOM         = 0;
        FOMString       = "";
        doOneDeeFOM     = 0;
        doTwoDeeFOM     = 0;
        doYieldV1       = 1;
        doYieldV2       = 0;
        printFOMInfo    = 0;
        printSysLim     = 0;
        printAveSyst    = 0;
        doStopXSec      = 1;
        
        dispIndSource = 0;
        
        MT2llaxisCut       = 80;
        MT2lbaxisCut       = 170;
        tryCalcPassByHand  = 0;
        inputMT2llCut     = 80;
        cutMT2lb        = 0;
        inputMT2lbCut   = 170;
        whichIntType = 0;
        doControl = 0;
        
        unitAxis = "GeV";
        
        saveLocation = "../Plots/";
    }
    
    void SetCutValues(vector<float> * vecMT2llCut, vector<float> * vecMT2lbCut, int levelVerbosity = 0) {
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
        if (levelVerbosity) {
            for (int iMT2ll = 0; iMT2ll < (int) vecMT2llCut->size(); ++iMT2ll) {
                cout << "cut for iMT2ll = " << iMT2ll << " is " << vecMT2llCut->at(iMT2ll) << endl;
            }
            for (int iMT2lb = 0; iMT2lb < (int) vecMT2lbCut->size(); ++iMT2lb) {
                cout << "cut for iMT2lb = " << iMT2lb << " is " << vecMT2lbCut->at(iMT2lb) << endl;
            }
        }
    }        
} HistPlotMaking;

typedef struct GlobalHistSettings {
    // global settings applied to the plotting of histograms
    
    bool doZeroNegBins;
    bool doStats;          // show statistics for data and mc spectra on plots -- e.g. mean and RMS -- (6/25/13) haven't tested yet to see how positioning looks
    bool doAbsRatio;          // Affects what gets shown in fractional ratio plots -- 0: (MC - Data) / Data, 1: Data/MC
    bool doSmartFracRatio;    // If on, makes it so that the ratio plot centers itself to include the ratio plot (i.e. matters when the ratio plot is going out of the nominal axis
    bool doSmoothSyst;
    bool showLegend;
    
    float fracAbsRatioLB, fracAbsRatioUB;
    float fracRelRatioLB, fracRelRatioUB;
    
    void SetUpGHSForPlots() {
        fracAbsRatioLB = 0.5;
        fracAbsRatioUB = 1.6;
    }
    
    void DefaultVarVals() {
        doZeroNegBins = 1;
        
        doSmartFracRatio = 0;
        doStats         = 0;
        doAbsRatio      = 1;
        showLegend      = 1;
        doSmoothSyst    = 0;
        
        fracAbsRatioLB = 1 - 0.23;
        fracAbsRatioUB = 1 + 0.23;
        
        fracRelRatioLB = -0.26;
        fracRelRatioUB = 0.26;
    }
    float ReturnFracRatioBound(int whichBound) {
        float outBound;
        if (whichBound == 1) {
            outBound = doAbsRatio ? fracAbsRatioLB : fracRelRatioLB;
        }
        else {
            outBound = doAbsRatio ? fracAbsRatioUB : fracRelRatioUB;
        }
        return outBound;
    }
} GlobalHistSettings;

typedef struct GlobalSaveInfo {
    // global variables used as part of saving outputs
    
    TString saveName;
    bool saveDotCFile; // If on, saves a .C version of every plot that is made
    bool makePlots;
    int typeSystPlotToSave; // 0: saves individual systematics plots (plus no Syst plot), 1: saves Full Systematics plot plus breakdown plot
    void DefaultVarVals() {
        saveDotCFile = 0;
        makePlots = 1;
        typeSystPlotToSave = 1;
    }
    void SetSaveName(SampLoadSettings * inSLS, GlobalHistSettings * inGHS, HistPlotMaking * inHPM) {
        TString TTBarGenName[4] = {"_madgraph_exclusive", "_mcatnlo", "_powheg", "_madgraph"};
        TString nameNTuple = "_Ovi";
        TString nameDiLep[3] = {"_MuMu", "_EE", "_EMu"};
        
        TString nameSignalShape[4] = {"", "_OneDeeShape", "_TwoDeeShape", "_ThreeDeeShape"};
        TString stringSigShape = inSLS->SignalShapeString();
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
                stringSignal += "_PerPol_";
                stringSignal += inSLS->vecPercentPolGrab[0];
            }
        }
        
        
        saveName = TTBarGenName[inSLS->whichTTbarGen];
        if (inSLS->whichDilepType > -1) {
            saveName += nameDiLep[inSLS->whichDilepType];
        }
        saveName += nameNTuple;
        saveName += stringSigShape;
        saveName += stringDDEstimate;
        saveName += stringExcSamp;
        saveName += stringSignal;
        if (inSLS->doReReco) saveName += "_ReReco";
        if (inGHS->doAbsRatio) saveName += "_AbsRatio";
        if (!inGHS->doZeroNegBins) saveName += "_noZNB";
        if (inSLS->noType0) saveName += "_noType0";
        if (!inGHS->showLegend) saveName += "_noLegend";
        if (inSLS->SmearedPlots) saveName += "_SmearedMET";
        if (inGHS->doSmoothSyst) saveName += "_SmoothedSysts";
        if (inSLS->useUnblindedData) saveName += "_UnblindedData";
        if (inSLS->noGenRecRW) saveName += "_noGenRecRW";
        if (!inSLS->doDropFakes) saveName += "_keepFakes";
    }
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        for (int iGSIParam = 0; iGSIParam < argc; ++iGSIParam) {
            
            if (strncmp (argv[iGSIParam],"sDCF", 4) == 0) {
                saveDotCFile = 1;
            }
            else if (strncmp (argv[iGSIParam],"noPlots", 7) == 0) {
                makePlots = 0;
            }
            else if (strncmp (argv[iGSIParam],"typeSystPlot", 12) == 0) {
                typeSystPlotToSave = strtol(argv[iGSIParam + 1], NULL, 10);
            }
        }
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
        SLS.SetParamsFromCommandLine(argc, argv);
        GSI.SetParamsFromCommandLine(argc, argv);
        for (int k = 0; k < argc; ++k) {
            cout << "argv[k] for k = " << k << " is: " << argv[k] << endl;
            if (strncmp (argv[k],"makePlots_Paper", 15) == 0) {
                int inWSS = 3;
                SLS.SetUpSLSForPlots(inWSS);
                HPM.SetUpHPMForPlots();
                GHS.SetUpGHSForPlots();
            }
            else if (strncmp (argv[k],"noAdd", 5) == 0) {
                HPM.addThings = 0;
            }
            else if (strncmp (argv[k],"psl", 3) == 0) {
                cout << "argc " << argc << endl;
                cout << "k " << k << endl;
                cout << "k + 1 " << k + 1 << endl;
                cout << "argv[k + 1] " << argv[k] << endl;
                cout << "argv[k + 1] " << argv[k + 1] << endl;
                cout << "TString(argv[k + 1]) " << TString(string(argv[k + 1])) << endl;
                HPM.saveLocation = TString(argv[k + 1]);
                HPM.saveLocation += "/";
            }
            else if (strncmp (argv[k],"doVerbosity", 11) == 0) {
                HPM.doVerbosity = 1;
                cout << "Verbosity is turned on!" << endl;
            }
            else if (strncmp (argv[k],"calcTTBarNorm", 13) == 0) {
                HPM.calcTTBarNorm = 1;
            }
            else if (strncmp (argv[k],"scaleFactor", 11) == 0) {
                HPM.scaleFactor = strtod(argv[k + 1], NULL);
            }
            else if (strncmp (argv[k],"useDDEst", 8) == 0) {
                HPM.useDDEstimate = 1;
                HPM.useDDEstimate_TTBar = 1;
                HPM.versDDEst_TTBar = 0;
                HPM.useDDEstimate_DY = 1;
                HPM.versDDEst_DY = 4;
            }
            else if (strncmp (argv[k],"useTTBarDDEst", 13) == 0) {
                HPM.useDDEstimate = 1;
                HPM.useDDEstimate_TTBar = 1;
                HPM.versDDEst_TTBar = strtol(argv[k+1], NULL, 10 );
            }
            else if (strncmp (argv[k],"useDYDDEst", 10) == 0) {
                HPM.useDDEstimate = 1;
                HPM.useDDEstimate_DY = 1;
                HPM.versDDEst_DY = strtol(argv[k+1], NULL, 10 );
            }
            else if (strncmp (argv[k],"calcTTBarDDEst", 11) == 0) {
                HPM.versDDEst_TTBar = strtol(argv[k+1], NULL, 10 );
            }
            else if (strncmp (argv[k],"calcDYDDEst", 11) == 0) {
                HPM.versDDEst_DY = strtol(argv[k+1], NULL, 10 );
            }
            else if (strncmp (argv[k],"noOneDee", 8) == 0) {
                HPM.doOneDee = 0;
            }
            else if (strncmp (argv[k],"doTwoDee", 8) == 0) {
                HPM.doTwoDee = 1;
                HPM.doOneDee = 0;
                cout << "Specify which axis to project to, followed by the LB and UB bins for the other axis, i.e...." << endl;
                cout << "doTwoDee 1 2 3 to project to x Axis ('1') using bin 2-3 along the y-axis ('2' and '3') and use STRING as the axis variable" << endl;
                API.numDims  = 2;
                API.whichAxisToProjTo = strtol(argv[k+1], NULL, 10 );
                API.SetAxesProjBounds_Agglomerate(strtol(argv[k+2], NULL, 10 ), strtol(argv[k+3], NULL, 10 ), -1, -1);
            }
            else if (strncmp (argv[k],"doThreeDee", 10) == 0) {
                HPM.doThreeDee = 1;
                HPM.doTwoDee = 0;
                HPM.doOneDee = 0;
                cout << "Specify which axis to project to, followed by the LB and UB bins for the other two axes, i.e...." << endl;
                cout << "doThreeDee 3 2 3 1 3 to project to z Axis (first '3') using bin 2-3 along the x-axis (the subsequent '2' and '3') ";
                cout << "and bins 1-3 of the y-axis (the subsequent '1' and '3')"<< endl;
                API.numDims  = 3;
                API.whichAxisToProjTo = strtol(argv[k+1], NULL, 10 );
                API.SetAxesProjBounds_Agglomerate(strtol(argv[k+2], NULL, 10 ), strtol(argv[k+3], NULL, 10 ), strtol(argv[k+4], NULL, 10 ), strtol(argv[k+5], NULL, 10 ));
            }
            else if (strncmp (argv[k],"axisRange1D", 11) == 0) {
                API.axis1DLB = strtol(argv[k+1], NULL, 10 );
                API.axis1DUB = strtol(argv[k+2], NULL, 10 );
            }
            else if (strncmp (argv[k],"setAxesNames", 12) == 0) {
                HPM.strAxis1 = TString(argv[k+1]);
                HPM.strAxis2 = TString(argv[k+2]);
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
            else if (strncmp (argv[k],"doSave", 6) == 0) {
                HPM.doYieldV1 = 0;
                HPM.doSaveHistograms = true;
                HPM.saveHistoDimension = strtol(argv[k+1], NULL, 10 );
            }
            else if (strncmp (argv[k],"printAveSystLim", 12) == 0) {
                HPM.printAveSyst = 1;
                HPM.printSysLim = 1;
                HPM.doStopXSec = 0;
            }
            else if (strncmp (argv[k],"printSystLim", 12) == 0) {
                HPM.printSysLim = 1;
                HPM.doStopXSec = 0;
            }
            else if (strncmp (argv[k],"printPerSystLim", 15) == 0) {
                HPM.printSysLim = 2;
                HPM.doStopXSec = 0;
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
            else if (strncmp (argv[k],"wInTy", 5) == 0) {
                HPM.whichIntType = strtol(argv[k+1], NULL, 10 );
            }
            else if (strncmp (argv[k],"doControl", 9) == 0) {
                HPM.doControl = 1;
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
            else if (strncmp (argv[k],"doSpecBound", 11) == 0) {
                GHS.fracAbsRatioLB = 0.5;
                GHS.fracAbsRatioUB = 1.6;
            }
            else if (strncmp (argv[k],"doStats", 7) == 0) {
                GHS.doStats = 1;
            }
            else if (strncmp (argv[k],"keepNBs", 7) == 0) {
                GHS.doZeroNegBins = 0;
            }
        }    
    }
} RunParams;
