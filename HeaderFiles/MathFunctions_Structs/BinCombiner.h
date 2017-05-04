#ifndef BinCombiner_h
#define BinCombiner_h

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TString.h"
#include <iostream>
#include <vector>

#include "./PunziCheck.h"

using namespace std;


void TestHist(TH1 * inputHist, bool doVerb = false) {
    int currGlobalBin;
    int currBinMapBin;
    int currMapBinX, currMapBinY, currMapBinZ;
    int totalBins = 0, totalMappingBins = 0;
    cout << "inputHist name " << inputHist->GetName() << endl;
    for (int iBinX = inputHist->GetNbinsX(); iBinX > 0; --iBinX) {
        for (int iBinY = inputHist->GetNbinsY(); iBinY > 0; --iBinY) {
            for (int iBinZ = inputHist->GetNbinsZ(); iBinZ > 0; --iBinZ) {
                totalBins++;
                currGlobalBin = inputHist->GetBin(iBinX, iBinY, iBinZ);
                currBinMapBin = (int) (0.5 + inputHist->GetBinContent(iBinX, iBinY, iBinZ));
                inputHist->GetBinXYZ(currBinMapBin, currMapBinX, currMapBinY, currMapBinZ);
                cout << "currGlobalBin:currBinMapBin " << currGlobalBin << ":" << currBinMapBin << endl;
                cout << "initial bin triplet " << iBinX << ":" << iBinY << ":" << iBinZ << " -> " << currMapBinX << ":" << currMapBinY << ":" << currMapBinZ << endl;
                if (currGlobalBin != currBinMapBin) {
                    totalMappingBins++;
                    if (doVerb) {
                        cout << "iBinX:iBinY:iBinZ " << iBinX << ":" << iBinY << ":" << iBinZ << endl;
                        cout << "currMapBinX:currMapBinY:currMapBinZ " << currMapBinX << ":" << currMapBinY << ":" << currMapBinZ << endl;
                    }
                }
            }
        }
    }
    cout << "totalBins " << totalBins << endl;
    cout << "totalMappingBins " << totalMappingBins << endl;
}


void AddBins(TH1 * inputHist, int currBin, int currBestBin) {
    float currBinContent, currBinError;
    float currBestBinContent, currBestBinError;
    
    if (currBestBin != currBin) {
        currBinContent = inputHist->GetBinContent(currBin);
        currBinError = inputHist->GetBinError(currBin);

        currBestBinContent = inputHist->GetBinContent(currBestBin);
        currBestBinError = inputHist->GetBinError(currBestBin);
        
        inputHist->SetBinContent(currBin, 0);
        inputHist->SetBinError(currBin, 0);
        
        inputHist->SetBinContent(currBestBin, currBinContent + currBestBinContent);
        inputHist->SetBinError(currBestBin, TMath::Sqrt(TMath::Power(currBinError, 2) + TMath::Power(currBestBinError, 2)));
    }
}
bool JustDoBackgroundPunziSum(vector<BinPunziCheck> * inVecBinPC) {
    unsigned int numPunzi = inVecBinPC->size();
    float currPunzi;
    
    bool allZero = true;
    for (unsigned int iPunzi = 0; iPunzi < numPunzi; ++iPunzi) {
        currPunzi = inVecBinPC->at(iPunzi).punziParam;
        if (currPunzi > 0) allZero = false;
    }
    return allZero;
}

float GetPunziParamSum(vector<BinPunziCheck> * inVecBinPC, bool doBackground, bool doVerb = false) {
    unsigned int numPunzi = inVecBinPC->size();
    
    float currSum = 0.0;
    float currPunzi;
    
    for (unsigned int iPunzi = 0; iPunzi < numPunzi; ++iPunzi) {
        if (doBackground) {
            currPunzi = inVecBinPC->at(iPunzi).backYield;
        }
        else {
            currPunzi = inVecBinPC->at(iPunzi).punziParam;
        }
        currSum += currPunzi;
        if (doVerb) {
            cout << "in Punzi vector sum, for iPunzi " << iPunzi << endl;
            if (doBackground) {
                cout << "currBackground " << currPunzi << endl;
            }
            else {
                cout << "currPunzi " << currPunzi << endl;
            }
        }
    }
    return currSum;
}

struct BinCombiner {
    vector<TH1 *> vecOutComboMaps;
    
    int whichAlgorithm; //which algorithm to use to designate how to combine bins
    float minThreshBkg;
    float punziSigmaA, punziSigmaB;
    
    void WriteToFile(TFile * outFile, int minIndex = 0, int maxIndex = -1) {
        outFile->cd();
        unsigned int numMaps = vecOutComboMaps.size();
        
        if (maxIndex < 0) maxIndex = numMaps - 1;
        
        for (unsigned int iMap = minIndex; iMap <= maxIndex; ++iMap) {
            vecOutComboMaps[iMap]->Write();
        }
    }
    void DefaultVarVals() {
        vecOutComboMaps.resize(0);
        
        punziSigmaA = 5.0;
        punziSigmaB = 2.0;
        
        whichAlgorithm = 0;
        minThreshBkg = 0.5;
    }
    int RankPunziCheckers(vector<BinPunziCheck> * inVecBinPC, int currProblemGlobalBin, bool doVerb = false) {
        unsigned int numPunzi = inVecBinPC->size();
        
        vector<BinPunziCheck> tempVecBinPC(0);
        
        int whichIndexForPunziProblemBin = -1;
        
        for (unsigned int iPunzi = 0; iPunzi < numPunzi; ++iPunzi) {
            if (inVecBinPC->at(iPunzi).globalBin == currProblemGlobalBin) {
                whichIndexForPunziProblemBin = iPunzi;
                continue; //don't want to check with problem bin
            }
            tempVecBinPC.push_back(inVecBinPC->at(iPunzi));
        }
        
        float currPunziVecSum;
        float currPunziBestSum = -1;
        int currBestBin = -1;
        
        
        
        for (unsigned int iPunzi = 0; iPunzi < numPunzi - 1; ++iPunzi) {
            if (doVerb) {
                cout << "in Punzi vector ranker, for iPunzi " << iPunzi << endl;
            }
            BinPunziCheck tempBinPC = tempVecBinPC[iPunzi];
            tempVecBinPC[iPunzi].sigEff += inVecBinPC->at(whichIndexForPunziProblemBin).sigEff;
            tempVecBinPC[iPunzi].backYield += inVecBinPC->at(whichIndexForPunziProblemBin).backYield;
            tempVecBinPC[iPunzi].SetPunziParam(doVerb);
            currPunziVecSum = GetPunziParamSum(&tempVecBinPC, JustDoBackgroundPunziSum(&tempVecBinPC), doVerb);
            if (currPunziVecSum > currPunziBestSum) {
                if (doVerb) {
                    cout << "currPunziBestSum " << currPunziBestSum << endl;
                    cout << "currPunziVecSum " << currPunziVecSum << endl;
                    cout << "currGlobalBin Value " << tempVecBinPC[iPunzi].globalBin << endl;
                    cout << "currBestBin Value " << currBestBin << endl;
                }
                currPunziBestSum = currPunziVecSum;
                currBestBin = tempVecBinPC[iPunzi].globalBin;
            }
            
            tempVecBinPC[iPunzi] = tempBinPC;
        }
        
        return currBestBin;
    }
    int FindOptimalBin(TH1 * inputHistSig, TH1 * inputHistBkg, int currProblemBinX, int currProblemBinY, int currProblemBinZ, bool doVerb = false) {
        float totalIntegralSig = inputHistSig->Integral();
        if (doVerb) {
            cout << "totalIntegralSig " << totalIntegralSig << endl;
        }
        //need this for calculating signal efficiency
        
//        float currProblemBkgYield, currProblemSigYield, currProblemSig
        /*
        currProblemSigYield = inputHistSig->GetBinContent(currProblemBinX, currProblemBinY, currProblemBinZ);
        currProblemSigEff = currProblemSigYield / totalIntegralSig;
        currProblemBkgYield = inputHistBkg->GetBinContent(currProblemBinX, currProblemBinY, currProblemBinZ);
        */
        
        float currAddBkgYield, currAddSigYield, currAddSigEff;
        
        vector<BinPunziCheck> currVecBinPunziCheckers(0);
        
        int currProblemGlobalBin = inputHistSig->GetBin(currProblemBinX, currProblemBinY, currProblemBinZ);
        int currAddGlobalBin;
        if (doVerb) {
            cout << "currProblemBinX:currProblemBinY:currProblemBinZ " << currProblemBinX << ":" << currProblemBinY << ":" << currProblemBinZ << endl;
        }
        for (int addBinZ = currProblemBinZ; addBinZ >= TMath::Max(1, currProblemBinZ - 1); --addBinZ) {
            for (int addBinY = currProblemBinY; addBinY >= TMath::Max(1, currProblemBinY - 1); --addBinY) {
	      for (int addBinX = currProblemBinX; addBinX >= TMath::Max(1, currProblemBinX - 1); --addBinX) {
		//cout << "currProblemBinX + currProblemBinY + currProblemBinZ " << currProblemBinX + currProblemBinY + currProblemBinZ <<endl;
		//cout << "addBinX + addBinY + addBinZ " << addBinX + addBinY + addBinZ <<endl;
		//if (currProblemBinX + currProblemBinY + currProblemBinZ - addBinZ - addBinY - addBinZ > 1) continue;
                    currAddGlobalBin = inputHistSig->GetBin(addBinX, addBinY, addBinZ);
                    if (doVerb) {
                        cout << "addBinX:addBinY:addBinZ " << addBinX << ":" << addBinY << ":" << addBinZ << endl;
                        cout << "currAddGlobalBin " << currAddGlobalBin << endl;
                    }
                    
                    currAddBkgYield = inputHistBkg->GetBinContent(addBinX, addBinY, addBinZ);
                    currAddSigYield = inputHistSig->GetBinContent(addBinX, addBinY, addBinZ);
                    currAddSigEff = currAddSigYield / totalIntegralSig;
                    
                    BinPunziCheck currBinPC;
                    currBinPC.SetValues(currAddGlobalBin, punziSigmaA, punziSigmaB, currAddSigEff, currAddBkgYield, doVerb);
                    currVecBinPunziCheckers.push_back(currBinPC);
                }
            }
        }
        int bestGlobalBin = RankPunziCheckers(&currVecBinPunziCheckers, currProblemGlobalBin, doVerb);
        if (doVerb) {
            cout << "bestGlobalBin " << bestGlobalBin << endl;
        }
        return bestGlobalBin;
    }
    
    void MakeComboMap(vector<TH1 *> * vecInputHistSig, vector<TH1 *> * vecInputHistBkg, vector<TString> * vecInputComboMapNames, int numDims, bool doVerb = false) {
        unsigned int numSigHists = vecInputHistSig->size();
        unsigned int numBkgHists = vecInputHistBkg->size();
        if (numSigHists != numBkgHists) {
            cout << "numSigHists and numBkgHists don't match! numSigHists: " << numSigHists << ", numBkgHists: " << numBkgHists << endl;
        }
        
        TH1F * currHist1D = NULL;
        TH2F * currHist2D = NULL;
        TH3F * currHist3D = NULL;
        
        for (unsigned int iHist = 0; iHist < numSigHists; ++iHist) {
            TH1 * currHistSig = vecInputHistSig->at(iHist);
            TH1 * currHistBkg = vecInputHistBkg->at(iHist);
            
            if (numDims == 1) {
                currHist1D = (TH1F *) currHistSig->Clone(vecInputComboMapNames->at(iHist));
            }
            else if (numDims == 2) {
                currHist2D = (TH2F *) currHistSig->Clone(vecInputComboMapNames->at(iHist));
            }
            else if (numDims == 3) {
                currHist3D = (TH3F *) currHistSig->Clone(vecInputComboMapNames->at(iHist));
            }
            
            
            TString histNameSig;
            vector<int> vecInputSigHistAxisNBins;
            vector<TAxis *> vecInputSigHistAxes;
            vector<float> vecInputSigHistAxisBinWidths;
            SetHistogramData(currHistSig, histNameSig, &vecInputSigHistAxisNBins, &vecInputSigHistAxes, &vecInputSigHistAxisBinWidths);
            
            TString histNameBkg;
            vector<int> vecInputBkgHistAxisNBins;
            vector<TAxis *> vecInputBkgHistAxes;
            vector<float> vecInputBkgHistAxisBinWidths;
            SetHistogramData(currHistBkg, histNameBkg, &vecInputBkgHistAxisNBins, &vecInputBkgHistAxes, &vecInputBkgHistAxisBinWidths);
            
            for (int iAxis = 0; iAxis < 3; ++iAxis) {
                if (vecInputSigHistAxisNBins[iAxis] != vecInputBkgHistAxisNBins[iAxis]) {
                    cout << "number of axes for axis " << iAxis + 1 << " doesn't match" << endl;
                }
            }
            
            float currBkgYield;
            
            int currGlobalBin, currMapBin;
            
            for (int iBinZ = vecInputSigHistAxisNBins[2]; iBinZ > 0; --iBinZ) {
                for (int iBinY = vecInputSigHistAxisNBins[1]; iBinY > 0; --iBinY) {
                    for (int iBinX = vecInputSigHistAxisNBins[0]; iBinX > 0; --iBinX) {
                        currGlobalBin = currHistSig->GetBin(iBinX, iBinY, iBinZ);
                        
                        currBkgYield = currHistBkg->GetBinContent(iBinX, iBinY, iBinZ);
                        if (currBkgYield < minThreshBkg) {
                            if (doVerb) {
                                cout << "##########################" << endl;
                                cout << "GOING TO OPTIMIZE A BIN" << endl;
                                cout << "iBinX:iBinY:iBinZ " << iBinX << ":" << iBinY << ":" << iBinZ << endl;
                                cout << "currBkgYield " << currBkgYield << endl;
                                cout << "##########################" << endl;
                            }
                            //have to actually do something about it
                            currMapBin = FindOptimalBin(currHistSig, currHistBkg, iBinX, iBinY, iBinZ, doVerb);
                        }
                        else {
                            currMapBin = currGlobalBin;
                        }
                        if (currMapBin != currGlobalBin) {
                            AddBins(currHistSig, currGlobalBin, currMapBin);
                        }
                        currHist3D->SetBinContent(iBinX, iBinY, iBinZ, currMapBin);
                    }
                }
            }
            TestHist(currHist3D, doVerb);
            vecOutComboMaps.push_back((TH1 *)((TH3F*) currHist3D->Clone(vecInputComboMapNames->at(iHist))));
        }
    }
    
};
#endif
