#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <map>
using namespace std;

typedef map<TString, TString> mapStrToStr;
typedef map<TString, bool> mapStrToBool;


struct TreeLevelDrawingManager {
    TString nameTLDM;
    
    //Strings for saving/loading
    TString strFileName;
    
    //Strings for drawing
    TString strWeight, strWeight_Addtl;
    TString strWeight_PUWeight;
    TString strAddCut;
    //floats for drawing
    vector<float> vecWeightPU;
    
    //Maps containing the relevant info
    mapStrToStr mapWeights, mapWeights_Addtl, mapWeights_PU;
    mapStrToStr mapFiles;
    mapStrToBool mapIsSim, mapIsSig;
    
    //File management
    TFile * inputFile;
    TTree * inputTree;
    bool isSim, isSig;
    
    
    ////Things for managing efficiency/yield calculations
    ROCCurveParams ROCCP;
    
    vector<TGraph *> vecROCCurves;
    vector<TString> vecNameROCCurves;
    
    
    vector< vector<TGraph *> > vecVecPunziGraphs;
    vector< vector<TString> > vecVecPunziGraphNames;
    
    unsigned int numChannels, numPunzi;
    vector<TString> vecChanNames;
    vector<TH1 *> vecFileHists;
    vector<TH1F *> vecFileHists_Projection;
    //Normalizing number of events
    float totalEvents;
    vector<float> vecTotalEvents_Channel;
    //Numbers for efficiencies
    vector< vector<float> > vecVecNumEventsPassingCut, vecVecEffPassCut;
    float currNumEvents_Cut, currEfficiency_Cut, currYield_Cut;
    
    void SetName(TString inputName) {
        nameTLDM = inputName;
    }
    
    void SetUpMaps(int indexPolT2tt = -1, int indexPolT2bw = 4) {
        SetFileNameMap(mapFiles);
        SetIsSimMap(mapIsSim);
        SetIsSigMap(mapIsSig);
        SetWeightMap(mapWeights, indexPolT2tt, indexPolT2bw);
        SetBareBonesWeightMap(mapWeights_PU);
        SetWeightMap_Additional(mapWeights_Addtl);
    }
    void SetChannelNames() {
        vecChanNames.resize(0);
        vecChanNames.push_back("_MuMu");
        vecChanNames.push_back("_EE");
        vecChanNames.push_back("_EMu");
        numChannels = vecChanNames.size();
    }
    void SetStrings() {
        strAddCut = "";
        strFileName = SetTString(&mapFiles, nameTLDM);
        strWeight = SetTString(&mapWeights, nameTLDM);
        strWeight_PUWeight = SetTString(&mapWeights_PU, nameTLDM);
        SetChannelNames();
    }
    void SetBools() {
        isSim = SetBool(&mapIsSim, nameTLDM);
        isSig = SetBool(&mapIsSig, nameTLDM);
    }
    void SetThings() {
        SetStrings();
        SetBools();
        if (isSim && !isSig) {
            strAddCut = " && (TLep0GM_PromptLepton && TLep1GM_PromptLepton)";
        }
    }
    void SetUpPunzi() {
        ROCCP.SetUpPunzi();
        numPunzi = ROCCP.vecKnobPunzis.size();
        cout << "numPunzi " << numPunzi << endl;
    }
    void SetROCCurve(int whichDraw, int whichROCType, int whichROC) {
        ROCCP.SetAsROCCurve(isSim, whichDraw, whichROCType, whichROC);
        SetUpPunzi();
    }
    void OpenFile(TString baseDirFile, TString inputTreeName) {
        TString openName = baseDirFile + strFileName + TString(".root");
        inputFile = TFile::Open(openName);
        cout << "going to try and open " << openName << endl;
        if (!inputFile) {
            cout << "couldn't find " << baseDirFile + strFileName + TString(".root") << endl;
        }
        else {
            inputTree = (TTree *) inputFile->Get(inputTreeName);
        }
    }
    
    void SetNameGraphs(bool doVerb = false, TString addName = "") {
        TString fullAddName = ROCCP.strDraw + ROCCP.strROCSave + addName;
        if (doVerb) {
            cout << "fullAddName " << fullAddName << endl;
            cout << "numPunzi " << numPunzi << endl;
        }
        vecVecPunziGraphNames.resize(numChannels + 1);
        vecNameROCCurves.resize(numChannels + 1);
        for (unsigned int iChan = 0; iChan < numChannels + 1; ++iChan) {
            vecVecPunziGraphNames[iChan].resize(numPunzi);
            for (unsigned int iPunzi = 0; iPunzi < numPunzi; ++iPunzi) {
                if (doVerb) {
                    cout << "iChan " << iChan << endl;
                    cout << "iPunzi " << iPunzi << endl;
                }
                vecVecPunziGraphNames[iChan][iPunzi] = "PunziCurve_";
                vecVecPunziGraphNames[iChan][iPunzi] += ROCCP.vecPostPunziNames[iPunzi];
                vecVecPunziGraphNames[iChan][iPunzi] += fullAddName;
                vecVecPunziGraphNames[iChan][iPunzi] += (iChan < numChannels) ? vecChanNames[iChan]  : "_CompDilep";
                if (doVerb) {
                    cout << "vecVecPunziGraphNames[iChan][iPunzi] " << vecVecPunziGraphNames[iChan][iPunzi] << endl;
                }
            }
            vecNameROCCurves[iChan] = "EffCurve_";
            vecNameROCCurves[iChan] += fullAddName;
            vecNameROCCurves[iChan] += (iChan < numChannels) ? vecChanNames[iChan]  : "_CompDilep";
            if (doVerb) {
                cout << "vecNameROCCurves[iChan] " << vecNameROCCurves[iChan] << endl;
            }
        }
    }
    
    void GetWeightPU(bool doVerb = false) {
        vecWeightPU.resize(numChannels);
        
        TString baseDilep = "TDoEvent && ((TPassDoubleMu && TChannel == 0) || (TPassDoubleEl && TChannel == 1) || (TPassElMu && TChannel == 2))";
        
        baseDilep += strAddCut;
        
        vector<TString> vecChanCutStr(numChannels);
        TString cutStringBase = " && TChannel == ";
        for (unsigned int iChan = 0; iChan < numChannels; ++iChan) {
            if (!isSim || strWeight_PUWeight.Length() == 0) {
                vecWeightPU[iChan] = 1.0;
            }
            else {
                vecChanCutStr[iChan] = cutStringBase;
                vecChanCutStr[iChan] += iChan;
                vecWeightPU[iChan] = GetNPass(inputTree, ROCCP.strDraw, strWeight_PUWeight + EncapsulateString(baseDilep + vecChanCutStr[iChan]), doVerb) / GetNPass(inputTree, ROCCP.strDraw, strWeight + EncapsulateString(baseDilep + vecChanCutStr[iChan]), doVerb);
                cout << "vecWeightPU[" << iChan << "] " << vecWeightPU[iChan] << endl;
            }
        }
    }
    void CalculateTotalEvents() {
        numChannels = vecFileHists_Projection.size();
        vecTotalEvents_Channel.resize(numChannels);
        totalEvents = 0;
        float totalEvents_Channel;
        for (unsigned int iChan = 0; iChan < numChannels; ++iChan) {
            totalEvents_Channel = vecFileHists_Projection[iChan]->Integral(0, vecFileHists_Projection[iChan]->GetNbinsX() + 1);
            cout << "totalEvents_Channel " << totalEvents_Channel << endl;
            vecTotalEvents_Channel[iChan] = totalEvents_Channel;
            totalEvents += totalEvents_Channel;
        }
        cout << "total number of events: " << totalEvents << endl;
    }
    void CheckProjHists() {
        for (unsigned int iChan = 0; iChan < numChannels; ++iChan) {
            cout << "iChan " << iChan << endl;
            cout << "hist " << vecFileHists_Projection[iChan] << endl;
            cout << "name of hist " << vecFileHists_Projection[iChan]->GetName() << endl;
        }
    }
    void DoProjection(int binAxis1, int binAxis2, int projAxis, TString addString, bool doVerb) {
        vecFileHists_Projection.resize(numChannels);
        for (unsigned int iChan = 0; iChan < numChannels; ++iChan) {
            if (vecFileHists[iChan]->InheritsFrom("TH1F")) {
                vecFileHists_Projection[iChan] = (TH1F*) vecFileHists[iChan];
            }
            else {
                TString projName = vecFileHists[iChan]->GetName();
                projName += addString;
                switch (projAxis) {
                    case 1:
                        //project onto X
                        vecFileHists_Projection[iChan] = (TH1F *) ((TH3F *) vecFileHists[iChan])->ProjectionX(projName, binAxis1, binAxis1, binAxis2, binAxis2);
                        if (doVerb) {
                            cout << "vecFileHists_Projection[iChan] name " << vecFileHists_Projection[iChan]->GetName() << endl;
                        }
                        break;
                    case 2:
                        //project onto Y
                        vecFileHists_Projection[iChan] = (TH1F *) ((TH3F *) vecFileHists[iChan])->ProjectionY(projName, binAxis1, binAxis1, binAxis2, binAxis2);
                        break;
                    case 3:
                        //project onto Z
                        vecFileHists_Projection[iChan] = (TH1F *) ((TH3F *) vecFileHists[iChan])->ProjectionZ(projName, binAxis1, binAxis1, binAxis2, binAxis2);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    void GetTotalEventHist(TString inDrawCommand = "", bool doVerb = false) {
        GetWeightPU(doVerb);
        
        vecFileHists.resize(numChannels);

        
        vector<TString> vecChanCutStr(numChannels);
        TString cutStringBase = "TChannel == ";
        totalEvents = 0.0;
        for (unsigned int iChan = 0; iChan < numChannels; ++iChan) {
            vecChanCutStr[iChan] = cutStringBase;
            vecChanCutStr[iChan] += iChan;
            vecChanCutStr[iChan] += strAddCut;
            vector<TString> vecAddtlCutStrings(0);
            vecAddtlCutStrings.push_back(vecChanCutStr[iChan]);
            
            TString searchStringAddtlWeight = nameTLDM;
            searchStringAddtlWeight += "_";
            searchStringAddtlWeight += iChan;
            searchStringAddtlWeight += "_";
            searchStringAddtlWeight += ROCCP.numBJetsCut;
            if (isSig || !isSim) {
                strWeight_Addtl = "";
            }
            else {
                strWeight_Addtl = SetTString(&mapWeights_Addtl, searchStringAddtlWeight);
            }
            if (inDrawCommand.Length() > 0) {
                TH3F * tempHist = GetTotalHistogram_TH3(inputTree, inDrawCommand, &ROCCP, &vecAddtlCutStrings, strWeight + strWeight_Addtl);
                vecFileHists[iChan] = (TH1 *) tempHist->Clone(nameTLDM + vecChanNames[iChan]);                
            }
            else {
                TH1F * tempHist = GetTotalHistogram(inputTree, &ROCCP, &vecAddtlCutStrings, strWeight + strWeight_Addtl);
                vecFileHists[iChan] = (TH1 *) tempHist->Clone(nameTLDM + vecChanNames[iChan]);
            }
            vecFileHists[iChan]->Scale(vecWeightPU[iChan]);            
        }
    }
    void AddToHistNames(TString addName) {
        TString currHistName;
        for (unsigned int iChan = 0; iChan < numChannels; ++iChan) {
            currHistName = vecFileHists[iChan]->GetName();
            currHistName += addName;
            vecFileHists[iChan]->SetName(currHistName);
        }
    }
    
    void CalculateROCNumbers(bool onlyCutSF, bool doVerb = false) {
        unsigned int numCuts = ROCCP.vecCutVals.size();
        int currBinCut;
        
        vecVecNumEventsPassingCut.resize(numChannels + 1);
        vecVecEffPassCut.resize(numChannels + 1);        
        for (unsigned int iChan = 0; iChan < numChannels + 1; ++iChan) {
            vecVecNumEventsPassingCut[iChan].resize(numCuts);
            vecVecEffPassCut[iChan].resize(numCuts);
        }
        
        for (unsigned int iCut = 0; iCut < numCuts; ++iCut) {
            currNumEvents_Cut = 0;
            currBinCut = vecFileHists_Projection[0]->GetXaxis()->FindBin(ROCCP.vecCutVals[iCut]);
            
            float currNumEvents_Channel_Cut;
            int currBinToIntegrateFrom;
            
            for (unsigned int iChan = 0; iChan < numChannels; ++iChan) {
                currBinToIntegrateFrom = (onlyCutSF && iChan == 2) ? 0 : currBinCut;
                currNumEvents_Channel_Cut = vecFileHists_Projection[iChan]->Integral(currBinToIntegrateFrom, vecFileHists_Projection[iChan]->GetNbinsX() + 1);
                if (doVerb) {
                    cout << "for iChan = " << iChan << endl;
                    cout << "currBinToIntegrateFrom " << currBinToIntegrateFrom << endl;
                    cout << "currNumEvents_Channel_Cut " << currNumEvents_Channel_Cut << endl;
                }
                currNumEvents_Cut += currNumEvents_Channel_Cut;
                vecVecNumEventsPassingCut[iChan][iCut] += currNumEvents_Channel_Cut;
                vecVecEffPassCut[iChan][iCut] = vecTotalEvents_Channel[iChan] > 0 ? currNumEvents_Channel_Cut / vecTotalEvents_Channel[iChan] : 0;
            }
            vecVecNumEventsPassingCut[numChannels][iCut] = currNumEvents_Cut;
            currEfficiency_Cut = totalEvents > 0 ? currNumEvents_Cut / totalEvents : 0;
            if (doVerb) {
                cout << "currNumEvents_Cut " << currNumEvents_Cut << endl;
                cout << "totalEvents " << totalEvents << endl;
                cout << "currEfficiency_Cut " << currEfficiency_Cut << endl;
            }
            vecVecEffPassCut[numChannels][iCut] = currEfficiency_Cut;
        }
    }
    void ReadFromFile(TString inputNameFile, bool readGraphs = false) {
        TFile * inFile = TFile::Open(inputNameFile);
        if (!inFile) {
            cout << "couldn't find " << inputNameFile << endl;
        }
        
        SetChannelNames();
        SetUpPunzi();
        
        unsigned int numChanToGrab = numChannels;
        if (readGraphs) ++numChanToGrab;
        
        vecFileHists.resize(numChannels);
        vecROCCurves.resize(numChannels + 1);
        vecVecPunziGraphs.resize(numChannels + 1);
        for (unsigned int iChan = 0; iChan < numChanToGrab; ++iChan) {
            vecVecPunziGraphs[iChan].resize(numPunzi);
        }
        
        for (unsigned int iChan = 0; iChan < numChanToGrab; ++iChan) {
            if (readGraphs) {
                vecROCCurves[iChan] = (TGraph *) inFile->Get(vecNameROCCurves[iChan]);
                if (!vecROCCurves[iChan]) {
                    cout << "from the file " << inputNameFile << endl;
                    cout << "failed to grab " << vecROCCurves[iChan] << endl;
                }
                for (unsigned int iPunzi = 0; iPunzi < numPunzi; ++iPunzi) {
                    vecVecPunziGraphs[iChan][iPunzi] = (TGraph *) inFile->Get(vecVecPunziGraphNames[iChan][iPunzi]);
                    if (!vecVecPunziGraphs[iChan][iPunzi]) {
                        cout << "from the file " << inputNameFile << endl;
                        cout << "failed to grab " << vecVecPunziGraphNames[iChan][iPunzi] << endl;
                    }
                }
            }
            else {
                cout << "nameTLDM + vecChanNames[iChan] " << nameTLDM + vecChanNames[iChan] << endl;
                vecFileHists[iChan] = (TH1F *) inFile->Get(nameTLDM + vecChanNames[iChan]);
            }
        }
    }
    void GrabBackgroundProjections(TString inputNameFile, TString addGrabName) {
        TFile * inFile = TFile::Open(inputNameFile);
        vecFileHists_Projection.resize(numChannels);
        for (unsigned int iChan = 0; iChan < numChannels; ++iChan) {
            vecFileHists_Projection[iChan] = (TH1F *) inFile->Get(nameTLDM + vecChanNames[iChan] + addGrabName);
        }
    }
    void WriteToFile(TFile * outFile, bool makeBkgs, bool doVerb = false) {
        outFile->cd();
        unsigned int numChan = numChannels;
        if (doVerb) {
            cout << "numChan " << numChan << endl;
            cout << "numPunzi " << numPunzi << endl;
        }
        if (!makeBkgs) numChan += 1;
        for (unsigned int iChan = 0; iChan < numChan; ++iChan) {
            if (makeBkgs) {
                if (doVerb) {
                    cout << "Writing the output histograms" << endl;
                }
                vecFileHists[iChan]->Write();
            }
            else {
                
                vecROCCurves[iChan]->Write();
                if (doVerb) {
                    cout << "wrote the ROC Curve: " << vecROCCurves[iChan]->GetName() << ", now going to write Punzi curves " << endl;
                }
                for (unsigned int iPunzi = 0; iPunzi < numPunzi; ++iPunzi) {
                    vecVecPunziGraphs[iChan][iPunzi]->Write();
                    if (doVerb) {
                        cout << "wrote the Punzi Curve: " << vecVecPunziGraphs[iChan][iPunzi]->GetName() << endl;
                    }
                }
                if (doVerb) {
                    cout << "wrote the Punzi curves, now going to write the background projections" << endl;
                    cout << "does it exist? " << vecFileHists_Projection[iChan] << endl;
                    cout << "does it exist? " << vecFileHists_Projection[iChan]->GetName() << endl;
                }
                if (iChan < numChannels) {
                    vecFileHists_Projection[iChan]->Write();
                }
            }
        }
    }
};

void SetROCCurve(TreeLevelDrawingManager * inTLDM_Bkg, TreeLevelDrawingManager * inTLDM_Sig, bool doVerb = false) {
    unsigned int numChannels = inTLDM_Bkg->numChannels;
    inTLDM_Bkg->vecROCCurves.resize(numChannels + 1);
    for (unsigned int iChan = 0; iChan < numChannels + 1; ++iChan) {
        inTLDM_Bkg->vecROCCurves[iChan] = EffCurve(&inTLDM_Bkg->vecVecEffPassCut[iChan], &inTLDM_Sig->vecVecEffPassCut[iChan], inTLDM_Bkg->vecNameROCCurves[iChan]);   
    }
}

void SetPunzi(TreeLevelDrawingManager * inTLDM_Bkg, TreeLevelDrawingManager * inTLDM_Sig, float scaleFactor, float minBkg = 1.0, bool doVerb = false) {
    unsigned int numChannels = inTLDM_Bkg->numChannels;
    unsigned int numPunzi = inTLDM_Bkg->numPunzi;
    unsigned int numCuts = inTLDM_Bkg->vecVecEffPassCut[0].size();
    inTLDM_Bkg->vecVecPunziGraphs.resize(numChannels + 1);    
    for (unsigned int iChan = 0; iChan < numChannels + 1; ++iChan) {
        inTLDM_Bkg->vecVecPunziGraphs[iChan].resize(numPunzi);
    }    
    vector<float> vecCurrPunziParameter(0);

    for (unsigned int iChan = 0; iChan < numChannels + 1; ++iChan) {
        for (unsigned int iPunzi = 0; iPunzi < numPunzi; ++iPunzi) {            
            vecCurrPunziParameter.clear();
            vecCurrPunziParameter.resize(numCuts);
            for (unsigned int iCut = 0; iCut < numCuts; ++iCut) {
                if (inTLDM_Bkg->vecVecNumEventsPassingCut[iChan][iCut] < minBkg) {
                    vecCurrPunziParameter[iCut] = 0.0;
                }
                else {
                    vecCurrPunziParameter[iCut] = (scaleFactor * GetPunziParam(inTLDM_Sig->vecVecEffPassCut[iChan][iCut], inTLDM_Bkg->vecVecNumEventsPassingCut[iChan][iCut], inTLDM_Bkg->ROCCP.vecKnobPunzis[iPunzi].first, inTLDM_Bkg->ROCCP.vecKnobPunzis[iPunzi].second, doVerb));
                }
            }
            inTLDM_Bkg->vecVecPunziGraphs[iChan][iPunzi] = PunziCurve(&vecCurrPunziParameter, &inTLDM_Bkg->ROCCP.vecCutVals, inTLDM_Bkg->vecVecPunziGraphNames[iChan][iPunzi], inTLDM_Bkg->ROCCP.strXAxisSave, scaleFactor);
        }
    }
}

void MakeCompTLDM(TreeLevelDrawingManager * inTLDM, vector<TreeLevelDrawingManager> * inVecTLDM) {
    unsigned int numTLDM = inVecTLDM->size();
    unsigned int numChannels = inVecTLDM->at(0).vecFileHists.size();
    inTLDM->vecFileHists.resize(numChannels);
    for (unsigned int iChan = 0; iChan < numChannels; ++iChan) {
        TString nameHist = inVecTLDM->at(0).vecFileHists[iChan]->GetName();
        int indexName = nameHist.Index(inVecTLDM->at(0).nameTLDM);
        int lengthName = inVecTLDM->at(0).nameTLDM.Length();
        nameHist.Replace(indexName, lengthName, inTLDM->nameTLDM);
        inTLDM->vecFileHists[iChan] = (TH1 *) inVecTLDM->at(0).vecFileHists[iChan]->Clone(nameHist);
        
        for (unsigned int iTLDM = 1; iTLDM < numTLDM; ++iTLDM) {
            inTLDM->vecFileHists[iChan]->Add(inVecTLDM->at(iTLDM).vecFileHists[iChan]);
        }
    }
}
