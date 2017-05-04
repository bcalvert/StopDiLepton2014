typedef pair<int, int> axisRange;

void RunLEEOnInputToyFile(LimitParametersContainer * inLPC, SUSYT2LimitParams * inSUSYT2LP, SUSYMassLimitParams * inSUSYMLP, TString nameCondorScript, int numToysPerFile, int whichToyIter, TString inputNameToysFile, MT2LimitParams * inMT2LP = 0) {
    
    TString pathLimCard = OutputPathCard(inLPC, inSUSYMLP, inSUSYT2LP, inMT2LP);
    TString baseNameLimCard = OutputBaseNameCard(inLPC, inSUSYMLP, inSUSYT2LP);
    TString nameLimCard = pathLimCard + baseNameLimCard;
    
    TString strBaseCommand = nameCondorScript;
    strBaseCommand += " combine -M  ProfileLikelihood --significance";
    
    TString strToys = "-t ";
    strToys += numToysPerFile;
    
    TString strToysFile = "--toysFile ";
    strToysFile += inputNameToysFile;
    
    TString strMass = "--mass ";
    strMass += inSUSYMLP->massStop;
    strMass += " --LSPmass ";
    strMass += inSUSYMLP->massLSP;
    
    TString strIter = "--wIter ";
    strIter += whichToyIter;
    
    
    TString strOutName = "-n ";
    strOutName += inSUSYT2LP->SavingName(false);
    strOutName += "_ToyIter";
    strOutName += whichToyIter;
    strOutName += inLPC->strFullSel;
    
    
    vector<TString> vecComArgs(0);
    vecComArgs.push_back(strToys);
    vecComArgs.push_back(strToysFile);
    vecComArgs.push_back(strMass);
    vecComArgs.push_back(strIter);
    vecComArgs.push_back(strOutName);
    vecComArgs.push_back(nameLimCard);
    
    System_GeneralCommand(strBaseCommand, &vecComArgs, true);
    System_GeneralCommand(strBaseCommand, &vecComArgs, false);
}


vector<int> CalculateEulerChar(TH2F * inHistMap, vector<float> * inVecThresholds, TString addName = "", bool doVerbosity = false) {
    //calculates the Euler Characteristic for a given 2D map,
    //needing as input the map as well as a vector of thresholds to apply to the map.
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inHistMap, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    int minStop = (int) (vecInputHistAxes[0]->GetBinCenter(1) + 0.5);
    int maxStop = (int) (vecInputHistAxes[0]->GetBinCenter(vecInputHistAxisNBins[0]) + 0.5);
    int minLSP = (int) (vecInputHistAxes[1]->GetBinCenter(1) + 0.5);
    int maxLSP = (int) (vecInputHistAxes[1]->GetBinCenter(vecInputHistAxisNBins[1]) + 0.5);
    
    axisRange axLSP(minLSP, maxLSP);
    axisRange axStop(minStop, maxStop);
 addName += "_ConstDeltaM";
    TH2F * currConstDeltaMHist = OutputHistDeltaMChanges(inHistMap, 0, 1, &axLSP, &axStop, addName);
    //TH2F * currConstDeltaMHist = (TH2F *) inHistMap->Clone(histName + "_Test");
    
    TString histName_ConstDeltaM;
    vector<int> vecInputHistAxisNBins_ConstDeltaM;
    vector<TAxis *> vecInputHistAxes_ConstDeltaM;
    vector<float> vecInputHistAxisBinWidths_ConstDeltaM;
    SetHistogramData(currConstDeltaMHist, histName_ConstDeltaM, &vecInputHistAxisNBins_ConstDeltaM, &vecInputHistAxes_ConstDeltaM, &vecInputHistAxisBinWidths_ConstDeltaM);
    
    unsigned int numThreshes = inVecThresholds->size();
    
    ///debugging draw canvases
    vector<TCanvas *> vecCanvases(numThreshes);
    vector<TH2F *> vecDrawHist(numThreshes);
    
    
    vector<int> vecSumVertices(numThreshes), vecSumEdges(numThreshes), vecSumFaces(numThreshes), vecEulerChars(numThreshes);
    
    int currVertices, currEdges, currFaces;
    
    int currMapEulerChar;

    float currBinContent, currBinContentPlusX, currBinContentPlusY, currBinContentPlusXPlusY;
    bool currPointIsEdgeX, currPointIsEdgeY;
    
    for (unsigned int iThresh = 0; iThresh < numThreshes; ++iThresh) {
        if (doVerbosity) {
            TString currCanvName = "c_TestCanv_ThreshIndex";
            currCanvName += iThresh;
            vecCanvases[iThresh] = new TCanvas(currCanvName, currCanvName);
            
            TString currDrawHistName = histName_ConstDeltaM + TString("_Draw");
            currDrawHistName += iThresh;
            vecDrawHist[iThresh] = (TH2F *) currConstDeltaMHist->Clone(currDrawHistName);
            vecDrawHist[iThresh]->GetZaxis()->SetRangeUser(inVecThresholds->at(iThresh), 30);
            vecDrawHist[iThresh]->Draw("colz");
        }
        currMapEulerChar = 0;
        for (int iBinX = 1; iBinX <= vecInputHistAxisNBins_ConstDeltaM[0]; ++iBinX) {
            for (int iBinY = 1; iBinY <= vecInputHistAxisNBins_ConstDeltaM[1]; ++iBinY) {
                //reset counts for current point
                currVertices = 0;
                currEdges = 0;
                currFaces = 0;
                
                
                currBinContent = currConstDeltaMHist->GetBinContent(iBinX, iBinY);
                //set the plus x,y guys to be far below 0 to expedite scanning
                currBinContentPlusX = -9999;
                currBinContentPlusY = -9999;
                currBinContentPlusXPlusY = -9999;
                
                
                currPointIsEdgeX = (iBinX == vecInputHistAxisNBins_ConstDeltaM[0]);
                currPointIsEdgeY = (iBinY == vecInputHistAxisNBins_ConstDeltaM[1]);
                
                if (currBinContent < inVecThresholds->at(iThresh)) {
                    continue;
                    //no addition of vertices, edges, nor faces to sum
                }
                else {
                    currVertices++;
                    //check for edges
                    if (!currPointIsEdgeX) {
                        currBinContentPlusX = currConstDeltaMHist->GetBinContent(iBinX + 1, iBinY);
                        if (currBinContentPlusX >= inVecThresholds->at(iThresh)) currEdges++;
                    }
                    if (!currPointIsEdgeY) {
                        currBinContentPlusY = currConstDeltaMHist->GetBinContent(iBinX, iBinY + 1);
                        if (currBinContentPlusY >= inVecThresholds->at(iThresh)) currEdges++;
                    }
                    if (currEdges == 2) {
                        currBinContentPlusXPlusY = currConstDeltaMHist->GetBinContent(iBinX + 1, iBinY + 1);
                        if (currBinContentPlusXPlusY >= inVecThresholds->at(iThresh)) currFaces++;
                    }
                }
                vecSumVertices[iThresh] += currVertices;
                vecSumEdges[iThresh] += currEdges;
                vecSumFaces[iThresh] += currFaces;
            }
        }
        currMapEulerChar = vecSumVertices[iThresh] - vecSumEdges[iThresh] + vecSumFaces[iThresh];
        if (doVerbosity) {
            cout << "for threshold " << inVecThresholds->at(iThresh) << endl;
            cout << "the current Euler Characteristic for the histogram " << histName_ConstDeltaM << " is " << currMapEulerChar << endl;
        }
        vecEulerChars[iThresh] = currMapEulerChar;
    }
    return vecEulerChars;
}


struct LEECalculator {
    int numToys, minIter, maxIter;
    
    TString nameTree;
    TString nameFile;
    TFile * inFile;
    TTree * inTree;
    
    double sigmaLimit;
    
    vector< vector<TH2F *> > vecVecHists;
    
    vector< vector<int> > vecVecEulers;
    
    void SetFileAndTree() {
        inFile = TFile::Open(nameFile);
        inTree = (TTree *) inFile->Get(nameTree);
    }
    
    void DefaultVals() {
        nameTree = "limit";
        nameFile = "Test.root";
        
        sigmaLimit = 0.2;
        numToys = 500;
        minIter = 1;
        maxIter = 1;
    }
    void SetVec(int whichDir = 0) {
        TString baseHistName = "h_SignifMap_wIter";
        TString addNameToy = "_wToy";
        TString currHistName;
        
        TString axisString = ";M_{#tilde{t}} [GeV];M_{#tilde{#chi}_{1}^{0}} [GeV]";
        //int numStop = 29;
        int numStop = 17;
        float minStop = 87.5;
        //float maxStop = 812.5;
        float maxStop = 512.5;
        //int numLSP = 9;
        int numLSP = 17;
        //int numLSP = 11;
        float minLSP = -12.5;
        float maxLSP = 412.5;
        //float maxLSP = 312.5;
        //float maxLSP = 262.5;
        //float maxLSP = 212.5;
        
        unsigned int currNumIters = 1 + maxIter - minIter;
        vecVecHists.resize(currNumIters);
        for (unsigned int iIter = 1; iIter <= currNumIters; ++iIter) {
            vecVecHists[iIter - 1].resize(numToys);
            for (int iToy = 1; iToy <= numToys; ++iToy) {
                currHistName = baseHistName;
                currHistName += minIter - 1 + iIter;
                currHistName += addNameToy;
                currHistName += iToy;
                if (whichDir > 0) {
                    currHistName += "_OneSigUp";
                }
                else if (whichDir < 0) {
                    currHistName += "_OneSigDown";
                }
                vecVecHists[iIter - 1][iToy - 1] = new TH2F(currHistName, axisString, numStop, minStop, maxStop, numLSP, minLSP, maxLSP);
            }
        }
    }
    void ProjectIntoTH2F(int whichDir = 0) {
        float limitMult = 1.0;

        if (whichDir > 0) {
            limitMult += sigmaLimit;
        }
        else if (whichDir < 0) {
            limitMult -= sigmaLimit;
        }
        
        TString currNameHist;

        TString varString = "mLSP:mStop";
        TString baseSelStringBegin = "limit * (";
        baseSelStringBegin += limitMult;
        baseSelStringBegin += ")* (iToy == ";
        cout << "baseSelStringBegin " << baseSelStringBegin << endl;
        TString baseSelStringMid = "&& wIter == ";
        
        TString currSelString;
        unsigned int currNumIters = vecVecHists.size();
        
        for (unsigned int iIter = 1; iIter <= currNumIters; ++iIter) {
            unsigned int currNumToys = vecVecHists[iIter - 1].size();
            for (unsigned int iToy = 1; iToy <= currNumToys; ++iToy) {
                //cout << "Setting up Hist for iIter " << iIter << ", iToy " << iToy << endl;
                currNameHist = vecVecHists[iIter - 1][iToy - 1]->GetName();
                currSelString = baseSelStringBegin;
                currSelString += iToy;
                currSelString += baseSelStringMid;
                currSelString += minIter - 1 + iIter;
                currSelString += ")";
                inTree->Project(currNameHist, varString, currSelString);
                if (iIter == 1 && iToy == 1) {
                    cout << "currNameHist " << currNameHist << endl;
                    cout << "varString " << varString << endl;
                    cout << "currSelString " << currSelString << endl;
                }
            }
        }
    }
    void RunEuler(vector<float> * inVecThresholds, TString addName = "", bool doVerbosity = false) {
        unsigned int currNumIters = vecVecHists.size();
        
        unsigned int numThreshes = inVecThresholds->size();
        vecVecEulers.resize(numThreshes);
        vector<int> currVecEuler;
        for (unsigned int iIter = 0; iIter < currNumIters; ++iIter) {
            unsigned int currNumToys = vecVecHists[iIter].size();
            for (unsigned int iToy = 0; iToy < currNumToys; ++iToy) {
                //                if (doVerbosity) {
                //cout << "calculating Euler for iIter " << iIter << ", iToy " << iToy << endl;
                //                }
                currVecEuler = CalculateEulerChar(vecVecHists[iIter][iToy], inVecThresholds, addName, doVerbosity);
                for (unsigned int iThresh = 0; iThresh < numThreshes; ++iThresh) {
                    vecVecEulers[iThresh].push_back(currVecEuler[iThresh]);
                }
            }
        }
    }
    
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        for (int iLEEParam = 0; iLEEParam < argc; ++iLEEParam) {
            if (strncmp (argv[iLEEParam],"-t", 2) == 0) {
                numToys = strtol(argv[iLEEParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iLEEParam],"minIter", 7) == 0) {
                minIter = strtol(argv[iLEEParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iLEEParam],"maxIter", 7) == 0) {
                maxIter = strtol(argv[iLEEParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iLEEParam],"nameFile", 8) == 0) {
                nameFile = TString(argv[iLEEParam + 1]);
                nameFile += ".root";
            }
            else if (strncmp (argv[iLEEParam],"sigmaLim", 8) == 0) {
                sigmaLimit = strtod(argv[iLEEParam + 1], NULL);
            }
        }
    }
};

