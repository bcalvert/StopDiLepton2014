// Structs used as part of displaying histograms -- fairly general set of structures!

using namespace std;
typedef vector<TH1F *> histProjVec;
typedef pair<int, int> intBounds;
typedef pair<TString, intBounds> indMCParams;


typedef struct AxisProjInfo {
    int numDims, whichAxisToProjTo, whichAxisForDist, axisProjLB, axisProjUB;
    int whichAxisToMix;
    TString nameBase;
    vector<int> vecAxesProjBounds;
    float nRMS;
    int RMSWidth;
    void DefaultVarVals() {
        numDims = 1;
        whichAxisToProjTo = 1;
        whichAxisForDist = 1;
        axisProjLB = 4;
        axisProjUB = 20;
        // For HLT paths defined above 1: 40 GeV, 2: 60 GeV, 3: 90 GeV, 4: 100 GeV, 5: 120 GeV, 6: 150 GeV, 7: 165 GeV, 8: 200 GeV, 9: 250 GeV, 10: 300 GeV, 11: 400 GeV, 12: 500 GeV, 13: 650 GeV, 14: 800 GeV, 15: 1000 GeV
        whichAxisToMix = 6 - (whichAxisForDist + whichAxisToProjTo);
        nRMS = -1;
        RMSWidth = 200;
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
        TString optProj = patsyName;
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
                        patsy_1D = (TH1F *) patsy_2D->ProjectionX(patsyName, vecAxesProjBounds[0], vecAxesProjBounds[1]);
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
                        optProj += "_x";
                        patsy_3D->GetYaxis()->SetRange(vecAxesProjBounds[0], vecAxesProjBounds[1]);
                        patsy_3D->GetZaxis()->SetRange(vecAxesProjBounds[2], vecAxesProjBounds[3]);
                        break;
                    case 2:
                        optProj += "_y";
                        patsy_3D->GetXaxis()->SetRange(vecAxesProjBounds[0], vecAxesProjBounds[1]);
                        patsy_3D->GetZaxis()->SetRange(vecAxesProjBounds[2], vecAxesProjBounds[3]);
                        break;
                    case 3:
                        optProj += "_z";
                        patsy_3D->GetXaxis()->SetRange(vecAxesProjBounds[0], vecAxesProjBounds[1]);
                        patsy_3D->GetYaxis()->SetRange(vecAxesProjBounds[2], vecAxesProjBounds[3]);                    
                        break;
                    default:
                        break; 
                }
                patsy_1D = (TH1F *) patsy_3D->Project3D(optProj);        
                break;
            default:
                cout << "some bugs, homie! pick correct num. of dims!" << endl;
                break;
        }
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
        return projHist;
    }
} AxisProjInfo;

typedef struct HistDisplayParams {
    int RBNX;
    bool doOverflow, doUnderflow;
    TString addName;
    void SetVals(int inRBNX = 1, bool inDoOverflow = false, bool inDoUnderflow = false, TString inAddName = "") {
        RBNX = inRBNX;
        doOverflow = inDoOverflow;
        doUnderflow = inDoUnderflow;
        addName = inAddName;
    }
} HistDisplayParams;

typedef struct IndSamplePlotInfo {
    TFile * inputFile;
    TString nameISPI;
    float weight_CentVal;
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
    void DefaultWeights(int numSystVars = 0) {        
        weight_CentVal = 1.0;
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
    
    
    // Dealing with the name and sample type
    void SetSampleType(bool doVerbosity = false) {
        const int numVV = 5;
        TString VVNames[numVV] = {"WW", "WZ", "ZZ", "ZG", "WG"};
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
    void SetSystHistAsClone(int iSyst, TString systName) {
        systName += "Shift";
        vecGrabbedHist_SystVarUp[iSyst] = (TH1 *) grabbedHist->Clone(grabbedHist->GetName() + systName + TString("Up_Patsy_") + nameISPI);
        vecGrabbedHist_SystVarDown[iSyst] = (TH1 *) grabbedHist->Clone(grabbedHist->GetName() + systName + TString("Down_Patsy_") + nameISPI);
    }
    void GrabHist(TString grabName, bool doVerbosity = false) {
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
        for (unsigned int iSamp = 0; iSamp < vecGrabNames->size(); ++iSamp) {
            if (doVerbosity) {
                cout << "for file " << inputFile->GetName() << endl;
                cout << "currently trying to grab histogram " << vecGrabNames->at(iSamp) << endl;
                if (inputFile->Get(vecGrabNames->at(iSamp)) != NULL) {
                    cout << "histo is available to grab " << endl;
                    cout << "weight that it will be scaled by " << weight_CentVal << endl;
                }
            }
            if (currHist == NULL) {
                currHist = (TH1 *) inputFile->Get(vecGrabNames->at(iSamp));
            }
            else {
                currHist->Add((TH1 *) inputFile->Get(vecGrabNames->at(iSamp)));
            }
        }
        grabbedHist = currHist;
        if (weight_CentVal != 1.0) {
            grabbedHist->Scale(weight_CentVal);
        }
    }
    void GrabHistSyst(TString baseHistGrabName, vector<TString> * vecSystNameAppends, bool doVerbosity = false) {
        TString grabName;
        for (unsigned int iSyst = 0; iSyst < vecSystNameAppends->size(); ++iSyst) {
            grabName = baseHistGrabName;
            grabName += vecSystNameAppends->at(iSyst);
            grabName += "Shift";
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
    void GrabHistSyst(vector<TString> * vecBaseHistGrabNames, vector<TString> * vecSystNameAppends, bool doVerbosity = false) {
        TH1 * currHistSystUp, * currHistSystDown;
        TString grabName;
        for (unsigned int iSyst = 0; iSyst < vecSystNameAppends->size(); ++iSyst) {
            if (vecShouldGrabSyst[iSyst]) {
                currHistSystUp = 0;
                currHistSystDown = 0;
                for (unsigned int iSamp = 0; iSamp < vecBaseHistGrabNames->size(); ++iSamp) {
                    grabName = vecBaseHistGrabNames->at(iSamp);
                    grabName += vecSystNameAppends->at(iSyst);
                    grabName += "Shift";
                    
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
        // projections aren't done before adding things together ISPIs
        if (doVerbosity) {
            if (vecGrabbedHist_SystVarUp.size() > 0) {                
                cout << "about to try cloning syst vars " << endl;
            }
        }
        for (unsigned int iSyst = 0; iSyst < vecGrabbedHist_SystVarUp.size(); ++iSyst) {
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
        grabbedHist_TH1F = inAPI->DoProjection_Agglomerate(grabbedHist, inHDP->addName);
        grabbedHist_TH1F->RebinX(inHDP->RBNX);
        HistogramUnderflowOverflow(grabbedHist_TH1F, inHDP->doUnderflow, inHDP->doOverflow);
        for (unsigned int iSyst = 0; iSyst < vecGrabbedHist_SystVarUp.size(); ++iSyst) {        
            vecGrabbedHist_SystVarUp_TH1F[iSyst] = inAPI->DoProjection_Agglomerate(vecGrabbedHist_SystVarUp[iSyst], inHDP->addName);
            vecGrabbedHist_SystVarUp_TH1F[iSyst]->RebinX(inHDP->RBNX);
            HistogramUnderflowOverflow(vecGrabbedHist_SystVarUp_TH1F[iSyst], inHDP->doUnderflow, inHDP->doOverflow);
            
            vecGrabbedHist_SystVarDown_TH1F[iSyst] = inAPI->DoProjection_Agglomerate(vecGrabbedHist_SystVarDown[iSyst], inHDP->addName);
            vecGrabbedHist_SystVarDown_TH1F[iSyst]->RebinX(inHDP->RBNX);
            HistogramUnderflowOverflow(vecGrabbedHist_SystVarDown_TH1F[iSyst], inHDP->doUnderflow, inHDP->doOverflow);
        }
    }
    void SetTH1F_PassCut(vector<int> * vecCutValues, vector<TString> * vecCutVarNames, vector<TString> * vecSystNameAppends, bool doSyst = true, int levelVerbosity = 0) {
        if (doSyst && vecSystNameAppends->size() != vecGrabbedHist_SystVarUp.size()) {
            cout << "something funky in that syst name and syst TH1 vecs don't have same size" << endl;
        }
        TString systName = "";
        if (levelVerbosity) {
            cout << "about to run the PassCutHisto! " << endl;
        }
        grabbedHist_TH1F = PassCutHisto(grabbedHist, vecCutValues, vecCutVarNames, nameISPI, systName, levelVerbosity);
        if (doSyst) {
            for (unsigned int iSyst = 0; iSyst < vecSystNameAppends->size(); ++iSyst) {
                systName = "";
                systName += vecSystNameAppends->at(iSyst);
                systName += "Shift";
                
                vecGrabbedHist_SystVarUp_TH1F[iSyst] = PassCutHisto(vecGrabbedHist_SystVarUp[iSyst], vecCutValues, vecCutVarNames, nameISPI, systName + TString("Up"), levelVerbosity);
                vecGrabbedHist_SystVarDown_TH1F[iSyst] = PassCutHisto(vecGrabbedHist_SystVarDown[iSyst], vecCutValues, vecCutVarNames, nameISPI, systName + TString("Down"), levelVerbosity);
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
    
    ////////////////////
    /// yield functions
    void SetStatPlusSystErrorOnYields(int whichBin = 2, bool justStat = false, bool noSystPlusStat = true, bool doSymErr = false) {
        indSSI.SetStatPlusSystErrorYields(grabbedHist_TH1F, whichBin, nameISPI, justStat, noSystPlusStat, doSymErr);
    }
    void SetStatPlusSystErrorOnYields(TString inNameISPI, int whichBin = 2, bool justStat = false, bool noSystPlusStat = true, bool doSymErr = false) {
        indSSI.SetStatPlusSystErrorYields(grabbedHist_TH1F, whichBin, inNameISPI, justStat, noSystPlusStat, doSymErr);
    }
    void PrintOutYieldInfo(bool justStat = true, bool noSystPlusStat = true, bool printSysLim = false, bool printAveSys = false) {
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
    }
} IndSampleDisplayParams;
typedef pair<IndSamplePlotInfo, IndSampleDisplayParams> SampDisplay;
#include "./StopPlotSetup_ISPIFunctions_Set1.h"



#include "./StopPlotSetup_ISDPFunctions.h"

typedef struct HistogramDisplayStructs {
    vector<IndSamplePlotInfo> vecISPI_asLoaded;
    SampDisplay compSamp;
    vector<SampDisplay> vecSampDisplay_IndMC; //c.f. typedef at top of header file
    
    void PrintCompSystHistNames() {
        for (unsigned int iSyst = 0; iSyst < compSamp.first.vecGrabbedHist_SystVarUp_TH1F.size(); ++iSyst) {
            cout << "upSyst name " << compSamp.first.vecGrabbedHist_SystVarUp_TH1F[iSyst]->GetName() << endl;
            cout << "downSyst name " << compSamp.first.vecGrabbedHist_SystVarDown_TH1F[iSyst]->GetName() << endl;
        }
    }
    void DefaultVarVals() {
        vecISPI_asLoaded.resize(0);
        vecSampDisplay_IndMC.resize(0);        
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
    void GrabSystValues(TString baseHistGrabName, vector<TString> * vecSystNameAppends, bool doVerbosity = false) {
        for (unsigned int iISPI = 0; iISPI < vecISPI_asLoaded.size(); ++iISPI) {
            vecISPI_asLoaded[iISPI].GrabHistSyst(baseHistGrabName, vecSystNameAppends, doVerbosity);
        }
    }
    void GrabSystValues(vector<TString> * vecBaseHistGrabNames, vector<TString> * vecSystNameAppends, bool doVerbosity = false) {
        for (unsigned int iISPI = 0; iISPI < vecISPI_asLoaded.size(); ++iISPI) {
            vecISPI_asLoaded[iISPI].GrabHistSyst(vecBaseHistGrabNames, vecSystNameAppends, doVerbosity);
        }
    }
    void DoProjection(vector<indMCParams> * vecIndMCParams, AxisProjInfo * inAPI, HistDisplayParams * inHDP, TString compName, bool sortByInt = false, bool doVerbosity = false) {
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
            SortIndMC(sortByInt);
        }
        else {
            HistogramAdderProjector(inAPI, inHDP, &vecISPI_asLoaded, &compSamp.first, compName, doProj, doVerbosity); 
        }
        HistMainAttSet(compSamp.first.grabbedHist_TH1F, &compSamp.second);
    }
    ////////////////////////////////////////////////////////////////////////
    /// Functions related to yield calculations ///
    void DoPassCutHisto(vector<int> * vecCutValues, vector<TString> * vecCutVarNames, TString compName, vector<indMCParams> * vecIndMCParams, vector<TString> * vecSystNameAppends, bool doSyst = true, int levelVerbosity = 0) {
        if (levelVerbosity) {
            cout << "about to run SetTH1F_PassCut on compSamp" << endl;
        }
        if (vecSampDisplay_IndMC.size() > 0) {
            HistogramAdder(&vecISPI_asLoaded, &compSamp.first, compName, levelVerbosity, &vecSampDisplay_IndMC, vecIndMCParams);
        }
        else {
            HistogramAdder(&vecISPI_asLoaded, &compSamp.first, compName, levelVerbosity);
        }
        compSamp.first.SetTH1F_PassCut(vecCutValues, vecCutVarNames, vecSystNameAppends, doSyst, levelVerbosity);
        for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
            if (levelVerbosity) {
                cout << "about to run SetTH1F_PassCut on vecSampDisplay_IndMC[iIndMC] for iIndMC = " << iIndMC << endl;
            }
            vecSampDisplay_IndMC[iIndMC].first.SetTH1F_PassCut(vecCutValues, vecCutVarNames, vecSystNameAppends, doSyst, levelVerbosity);
        }
    }
    void SetSSI_YieldErrors(bool doIndMC = false, int whichBin = 2, bool justStat = false, bool noSystPlusStat = true, bool doSymErr = false) {
        compSamp.first.SetStatPlusSystErrorOnYields(whichBin, justStat, noSystPlusStat, doSymErr);
        if (doIndMC) {
            for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
//                vecSampDisplay_IndMC[iIndMC].first.SetStatPlusSystErrorOnYields(vecSampDisplay_IndMC[iIndMC].second.legendEntry, whichBin, justStat, noSystPlusStat, doSymErr);
                vecSampDisplay_IndMC[iIndMC].first.SetStatPlusSystErrorOnYields(whichBin, justStat, noSystPlusStat, doSymErr);
            }
        }
    }
    void PrintSSI_YieldInfo(bool doIndMC = false, bool justStat = true, bool noSystPlusStat = true, bool printSysLim = false, bool printAveSys = false) {
        compSamp.first.PrintOutYieldInfo(justStat, noSystPlusStat, printSysLim, printAveSys);
        if (doIndMC) {
            for (unsigned int iIndMC = 0; iIndMC < vecSampDisplay_IndMC.size(); ++iIndMC) {
                vecSampDisplay_IndMC[iIndMC].first.PrintOutYieldInfo(justStat, noSystPlusStat, printSysLim, printAveSys);
            }
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
                lineDS.SetParams(kMagenta, 1, 2, -1);
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
        for (int iIndMC = 0; iIndMC < numIndMC; ++iIndMC) {
            fillDS.SetParams(inVecMCColors->at(iIndMC), IndMCFillStyle, IndMCFillWidth, IndMCFillSize);
            lineDS.SetParams(inVecMCColors->at(iIndMC), IndMCLineStyle, IndMCLineWidth, IndMCLineSize);
            vecSampDisplay_IndMC[iIndMC].second.SetParams(inVecMCLegEntries->at(iIndMC), fillDS, lineDS, markerDS, inVecMCSortParams->at(iIndMC));
        }
    }
    void SortIndMC(bool sortByInt = false) {
        if (sortByInt) {
            sort(vecSampDisplay_IndMC.begin(), vecSampDisplay_IndMC.end(), greater<SampDisplay>());
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
    void CalculateSysts_IndMC(Color_t colorErrGraph, bool doAbsRatio, float fracRatioYAxisLB, float fracRatioYAxisUB, bool doSymErr, bool doSmear, bool doStopXSec, bool doVerbosity = false) {
        bool doFracRatio = false;
        if (doVerbosity) {
            cout << "about to calculate systs in compSamp " << endl;
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
} HistogramDisplayStructs;

typedef struct GlobalHistPlotInfo {
    vector<LatexString> vecLatexStrings;
    AxisDisplayParams fracRatioADP;
    void DefaultVarVals() {
        vecLatexStrings.resize(4); //TopCMSPreliminary String is index 0, Integrated Lumi string is index 1, Gen cut string is index 2, MET String is index 3
        fracRatioADP.DefaultVarVals();
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
    void SetLatexStringVec(float intLumi, int typeMET = 0, TString extraMETText = "", TString genCutUsed = "", bool isPrelim = true) {
        SetCMSPreliminaryLatex(&vecLatexStrings[0], isPrelim);
        SetLumiLatex(&vecLatexStrings[1], intLumi);
        SetGenCutLatex(&vecLatexStrings[2],  genCutUsed);
        SetMETLatexString(&vecLatexStrings[3], typeMET, extraMETText);
    }
    void DrawLatexStrings(TCanvas * inputCanvas, int whichPad = 1, bool doVerbosity = false, int begIndex = 0, int endIndex = -1) {
        int startIndex = begIndex;
        int finalIndex = endIndex > -1 ? endIndex : (int) vecLatexStrings.size() - 1;
        for (unsigned int iLS = startIndex; iLS <= finalIndex; ++iLS) {
            if (doVerbosity) {
                cout << "iLS = " << iLS << ", and text is " << vecLatexStrings[iLS].text << endl;                
            }
            vecLatexStrings[iLS].DrawCanvas(inputCanvas, whichPad, doVerbosity);
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
