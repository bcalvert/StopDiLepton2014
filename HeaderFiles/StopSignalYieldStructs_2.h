#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TString.h"
#include "TRandom.h"
#include <iostream>
#include "TDirectory.h"
#include "TKey.h"
#include <iostream>
#include <fstream>
//#include <vector>
#include <cmath>
#include <sstream>
using namespace std;
/*
 #include "./LimitScriptHeaderFiles/LimitScripts_ParametersStruct.h"
 #include "./LimitScriptHeaderFiles/LimitScripts_SUSYStruct.h"
 #include "./LimitScriptHeaderFiles/LimitFunctions_Saving.h"
 #include "./LimitScriptHeaderFiles/YieldCalculationFunctions.h"
 #include "./LimitScriptHeaderFiles/CardCreationFunctions.h"
 */

struct TripleBin {
    int binX, binY, binZ;
};

typedef pair<int, int> massCombo;

void SetTestEntries(TH1 * inputHist, float val) {
    //Sets up a test histogram with gaussians input values in each bin
    int nBinsX = inputHist->GetNbinsX();
    int nBinsY = inputHist->GetNbinsY();
    int nBinsZ = inputHist->GetNbinsZ();
    TRandom gSn;
    int globalBin;
    float globalBinContent;
    for (int iBinX = 0; iBinX <= nBinsX + 1; ++iBinX) {
        for (int iBinY = 0; iBinY <= nBinsY + 1; ++iBinY) {
            for (int iBinZ = 0; iBinZ <= nBinsZ + 1; ++iBinZ) {
                globalBin = inputHist->GetBin(iBinX, iBinY, iBinZ);
                globalBinContent = gSn.Gaus(val, 0.1 * val);
                inputHist->SetBinContent(globalBin, globalBinContent);
            }
        }
    }
}

vector<float> ReadSingLepFileFloats(TString fileName) {
    //Function used for reading in input values projected into a file
    //Similar one somewhere that should be consolidated
    
    ifstream inputFileStream(fileName);
    string line;
    vector<float> vecFloats;
    while (!(inputFileStream.eof())) {
        getline(inputFileStream, line);
        stringstream ss( line );
        float currField = 0.0;
        ss >> currField;
        vecFloats.push_back(currField);
    }
    return vecFloats;
}


/*
 struct SignalContaminationHandler {
 TString fullPathPlusNameControlFileSignal;
 TString fullPathPlusNameControlFileTTBar;
 
 TFile * outputFile;
 TFile * inputFileSignal;
 TFile * inputFileTTBar;
 
 
 vector<TString> vecSystNames_Sig;
 
 float yieldTTBar, yieldSignal;
 
 float SFLumi;
 
 vector<float> vecSigConSFTTBar;
 float SigConSFTTBar;
 
 void DefaultVals() {
 SFLumi = 0.026;
 cout << "Setting LumiSystUncert to be " << SFLumi << endl;
 }
 
 void GetYield() {
 yieldTTBar = inputControlHistTTBar->Integral();
 yieldSignal = inputControlHistSignal->Integral();
 }
 void SetSF() {
 vecSigConSFTTBar.resize(3);
 vecSigConSFTTBar[0] = 1 - ((1.0 + 0 * SFLumi) * yieldSignal)/yieldTTBar;
 vecSigConSFTTBar[1] = 1 - ((1.0 + 1 * SFLumi) * yieldSignal)/yieldTTBar;
 vecSigConSFTTBar[2] = 1 - ((1.0 -1 * SFLumi) * yieldSignal)/yieldTTBar;
 }
 void DoCalc(TH1 * controlTTBar, TH1 * controlSignal) {
 inputControlHistTTBar = controlTTBar;
 inputControlHistSignal = controlSignal;
 GetYield();
 SetSF();
 }
 float GetSF(int whichSyst = 0) {
 return vecSigConSFTTBar[whichSyst];
 SigConSFTTBar = 1 - ((1.0 + whichSyst * SFLumi) * yieldSignal)/yieldTTBar;
 return SigConSFTTBar;
 }
 };
 */

void AddBins(TH1 * inputHist, int currBin, int currBestBin, int isStatSyst = 0, TH1 * inputHist_CV = NULL) {
    float currBinContent, currBinError;
    float currBestBinContent, currBestBinError;
    
    if (currBestBin != currBin) {
        currBinContent = inputHist->GetBinContent(currBin);
        currBinError = inputHist->GetBinError(currBin);
        
        currBestBinContent = inputHist->GetBinContent(currBestBin);
        currBestBinError = inputHist->GetBinError(currBestBin);
        
        float newBinContent = -99;
        if (isStatSyst) {
            ///need to calculate relative systematic difference, add in quadrature and then rebuild
            currBinContent -= inputHist_CV->GetBinContent(currBin);
            currBestBinContent -= inputHist_CV->GetBinContent(currBestBin);
            
            newBinContent = TMath::Sqrt( TMath::Power(currBinContent, 2) + TMath::Power(currBestBinContent, 2));
        }
        else {
            newBinContent = currBinContent + currBestBinContent;
        }
        
        
        inputHist->SetBinContent(currBin, 0);
        inputHist->SetBinContent(currBestBin, newBinContent);
        
        inputHist->SetBinError(currBin, 0);
        inputHist->SetBinError(currBestBin, TMath::Sqrt(TMath::Power(currBinError, 2) + TMath::Power(currBestBinError, 2)));
    }
}



struct OneDeeHistMaker {
    //Structure that takes as input a TH1 from a specific file and either does a 1D snake chain projection or just copies it and saves it as something else
    //Used as part of the 3D MT2 shape analysis
    
    TH1F * outputHist_1D, * outputHist_1D_CompressedBin;
    
    TH2F * outputHist_2D;
    TH3F * outputHist_3D;
    
    TH1F * outputHist_1D_TTBarGenSystUp, * outputHist_1D_TTBarGenSystDown;
    TH1F * outputHist_1D_CompressedBin_TTBarGenSystUp, * outputHist_1D_CompressedBin_TTBarGenSystDown;
    
    int numDimsInput;
    bool makeOneDeeHack;
    
    void SetOutputHist(TH1 * inputHist, vector<int> * inVecBinMap, TString partOfHistNameToReplace, bool cutControl, vector<TripleBin> * inVecBinsToKill, TH1F * genShapeHist = NULL, TString strReplace = "", bool doVerb = 0) {
        numDimsInput = 1;
        TString className = inputHist->ClassName();
        if (className.Contains("TH2")) {
            numDimsInput = 2;
        }
        else if (className.Contains("TH3")) {
            numDimsInput = 3;
        }
        TString nameAppendArr[3] = {"One", "Two", "Three"};
        TString nameAppend = nameAppendArr[numDimsInput - 1];
        nameAppend += "DToOneD";
        
        if (doVerb) {
            cout << "inputHist name " << inputHist->GetName() << endl;
        }
        
        int nBinsX = inputHist->GetNbinsX();
        int nBinsY = inputHist->GetNbinsY();
        int nBinsZ = inputHist->GetNbinsZ();
        
        int totBins = (nBinsX);
        if (numDimsInput > 1) {
            totBins *= (nBinsY);
            if (numDimsInput > 2) {
                totBins *= (nBinsZ);
            }
        }
        
        //stuff for running the control checks
        float controlMT2ll = 80;
        float controlMT2lblb = 170;
        float controlMT2bb_ZMET = 170;
        float binCenterX, binCenterY, binCenterZ;
        bool isControlBin;
        
        TString outHistName = inputHist->GetName();
        int lengthReplace = partOfHistNameToReplace.Length();
        TString outHistWriteName;
        outHistName.Replace(outHistName.Index(partOfHistNameToReplace), lengthReplace, partOfHistNameToReplace + nameAppend);
        
        outHistWriteName = inputHist->GetName();
        if (strReplace.Length() != 0) {
            outHistWriteName.Replace(outHistWriteName.Index(partOfHistNameToReplace), lengthReplace, strReplace);
            if (doVerb) {
                cout << "replacing " << partOfHistNameToReplace << " with " << strReplace << endl;
                cout << "outHistWriteName " << outHistWriteName << endl;
            }
        }
        
        if (makeOneDeeHack) {
            int numCompressedBins = (int) inVecBinMap->size();
            int currCounterCompressedBin = 0;
            outputHist_1D = new TH1F(outHistName + TString("_FullBin"), ";Bin;Events / Bin", totBins, 0.5, (float) ((totBins)+0.5));
            outputHist_1D_CompressedBin = new TH1F(outHistName, ";Compressed Bin;Events / Bin", numCompressedBins, 0.5, (float) ((numCompressedBins)+0.5));
            
            if (genShapeHist && !(outHistName.Contains("Up") || outHistName.Contains("Down"))) {
                TString outHistNameTTBar = "TTBar";
                outputHist_1D_TTBarGenSystUp = new TH1F(outHistNameTTBar + TString("_PowhegShapeUp_FullBin"), ";Bin;Events / Bin", totBins, 0.5, (float) ((totBins)+0.5));
                outputHist_1D_TTBarGenSystDown = new TH1F(outHistNameTTBar + TString("_PowhegShapeDown_FullBin"), ";Bin;Events / Bin", totBins, 0.5, (float) ((totBins)+0.5));
                
                outputHist_1D_CompressedBin_TTBarGenSystUp = new TH1F(outHistNameTTBar + TString("_PowhegShapeUp"), ";Compressed Bin;Events / Bin", numCompressedBins, 0.5, (float) ((numCompressedBins)+0.5));
                outputHist_1D_CompressedBin_TTBarGenSystDown = new TH1F(outHistNameTTBar + TString("_PowhegShapeDown"), ";Compressed Bin;Events / Bin", numCompressedBins, 0.5, (float) ((numCompressedBins)+0.5));
                
                outputHist_1D_TTBarGenSystUp->SetFillStyle(3001);
                outputHist_1D_TTBarGenSystUp->SetFillColor(kRed - 2);
                outputHist_1D_TTBarGenSystDown->SetFillStyle(3001);
                outputHist_1D_TTBarGenSystDown->SetFillColor(kBlue - 2);
                outputHist_1D_CompressedBin_TTBarGenSystUp->SetFillStyle(3001);
                outputHist_1D_CompressedBin_TTBarGenSystUp->SetFillColor(kRed - 2);
                outputHist_1D_CompressedBin_TTBarGenSystDown->SetFillStyle(3001);
                outputHist_1D_CompressedBin_TTBarGenSystDown->SetFillColor(kBlue - 2);
            }
            else {
                outputHist_1D_TTBarGenSystUp = NULL;
                outputHist_1D_TTBarGenSystDown = NULL;
                outputHist_1D_CompressedBin_TTBarGenSystUp = NULL;
                outputHist_1D_CompressedBin_TTBarGenSystDown = NULL;
            }
            
            int binCounter = 0;
            
            int globalBin;
            float globalBinContent, globalBinError;
            
            float currGenShapeUncert;
            
            for (int iBinX = 1; iBinX <= inputHist->GetNbinsX(); ++iBinX) {
                binCenterX = inputHist->GetXaxis()->GetBinCenter(iBinX);
                for (int iBinY = 1; iBinY <= inputHist->GetNbinsY(); ++iBinY) {
                    binCenterY = inputHist->GetYaxis()->GetBinCenter(iBinY);
                    for (int iBinZ = 1; iBinZ <= inputHist->GetNbinsZ(); ++iBinZ) {
                        binCounter++;
                        binCenterZ = inputHist->GetZaxis()->GetBinCenter(iBinZ);
                        if (cutControl) {
                            if (numDimsInput == 1) {
                                isControlBin = binCenterX < controlMT2ll;
                            }
                            else if (numDimsInput == 2) {
                                isControlBin = binCenterX < controlMT2ll && binCenterY < controlMT2lblb;
                            }
                            else if (numDimsInput == 3) {
                                isControlBin = binCenterX < controlMT2ll && binCenterY < controlMT2lblb && binCenterZ < controlMT2bb_ZMET;
                            }
                        }
                        else {
                            isControlBin = false;
                        }
                        
                        for (unsigned int iTripleBin = 0; iTripleBin < inVecBinsToKill->size(); ++iTripleBin) {
                            if (iBinX == inVecBinsToKill->at(iTripleBin).binX && iBinY == inVecBinsToKill->at(iTripleBin).binY && iBinZ == inVecBinsToKill->at(iTripleBin).binZ) {
                                if (inVecBinMap->at(currCounterCompressedBin) == binCounter) {
                                    currCounterCompressedBin++;
                                }
                                continue;
                            }
                        }
                        
                        if (cutControl && isControlBin) {
                            if (doVerb) {
                                cout << "continuing because you're in control region, binX: " << binCenterX << " binY: " << binCenterY << " binZ: " << binCenterZ << endl;
                            }
                            if (inVecBinMap->at(currCounterCompressedBin) == binCounter) {
                                currCounterCompressedBin++;
                            }
                            continue;
                        }
                        globalBin = inputHist->GetBin(iBinX, iBinY, iBinZ);
                        globalBinContent = inputHist->GetBinContent(globalBin);
                        globalBinError = inputHist->GetBinError(globalBin);
                        currGenShapeUncert = genShapeHist ? genShapeHist->GetBinContent(binCounter) : 0.0;
                        if (doVerb) {
                            cout << "in hist " << inputHist->GetName() << ":";
                            cout << " for iBinX: " << iBinX << ", iBinY: " << iBinY << ", iBinZ: " << iBinZ << endl;
                            cout << ", global bin is " << globalBin << " with binContent " << globalBinContent << endl;
                            cout << ", current bin to fill is " << binCounter << " with binContent " << globalBinContent << endl;
                        }
                        outputHist_1D->SetBinContent(binCounter, globalBinContent);
                        outputHist_1D->SetBinError(binCounter, globalBinError);
                        
                        if (genShapeHist) {
                            outputHist_1D_TTBarGenSystUp->SetBinContent(binCounter, globalBinContent * (1 + currGenShapeUncert));
                            outputHist_1D_TTBarGenSystUp->SetBinError(binCounter, globalBinError);
                            outputHist_1D_TTBarGenSystDown->SetBinContent(binCounter, TMath::Max(0., globalBinContent * (1 - currGenShapeUncert)));
                            outputHist_1D_TTBarGenSystDown->SetBinError(binCounter, globalBinError);
                        }
                        /*
                         if (iBinX == 1) {
                         cout << " for iBinX:iBinY:iBinZ = " << iBinX << ": " << iBinY << ": " << iBinZ << endl;
                         cout << "binCounter " << binCounter << endl;
                         cout << "globalBinContent " << globalBinContent << endl;
                         }
                         if (iBinX == 4) {
                         cout << " for iBinX:iBinY:iBinZ = " << iBinX << ": " << iBinY << ": " << iBinZ << endl;
                         cout << "binCounter " << binCounter << endl;
                         cout << "globalBinContent " << globalBinContent << endl;
                         }
                         cout << "binCounter " << binCounter << endl;
                         cout << "currCounterCompressedBin " << currCounterCompressedBin << endl;
                         cout << "numCompressedBins " << numCompressedBins << endl;
                         */
                        if (currCounterCompressedBin < numCompressedBins) {
                            //cout << "inVecBinMap->at(currCounterCompressedBin) " << inVecBinMap->at(currCounterCompressedBin) << endl;
                            if (inVecBinMap->at(currCounterCompressedBin) == binCounter) {
                                currCounterCompressedBin++;
                                outputHist_1D_CompressedBin->SetBinContent(currCounterCompressedBin, globalBinContent);
                                outputHist_1D_CompressedBin->SetBinError(currCounterCompressedBin, globalBinError);
                                
                                if (genShapeHist) {
                                    outputHist_1D_CompressedBin_TTBarGenSystUp->SetBinContent(currCounterCompressedBin, globalBinContent * (1 + currGenShapeUncert));
                                    outputHist_1D_CompressedBin_TTBarGenSystUp->SetBinError(currCounterCompressedBin, globalBinError);
                                    outputHist_1D_CompressedBin_TTBarGenSystDown->SetBinContent(currCounterCompressedBin, TMath::Max(0., globalBinContent * (1 - currGenShapeUncert)));
                                    outputHist_1D_CompressedBin_TTBarGenSystDown->SetBinError(currCounterCompressedBin, globalBinError);
                                }
                            }
                        }
                    }
                }
            }
            outputHist_1D->SetName(outHistWriteName + TString("_FullBin"));
            outputHist_1D_CompressedBin->SetName(outHistWriteName);
        }
        else {
            if (numDimsInput == 1) {
                outputHist_1D = (TH1F *) inputHist->Clone("Temp");
                outputHist_1D->SetName(outHistWriteName);
            }
            else if (numDimsInput == 2) {
                outputHist_2D = (TH2F *) inputHist->Clone("Temp");
                outputHist_2D->SetName(outHistWriteName);
            }
            else if (numDimsInput == 3) {
                outputHist_3D = (TH3F *) inputHist->Clone("Temp");
                outputHist_3D->SetName(outHistWriteName);
            }
        }
        //HistogramUnderflowOverflow(outputHist_1D, true, true);
    }
    void WriteToFile(TFile * outputFile) {
        outputFile->cd();
        if (makeOneDeeHack) {
            if (!outputFile->Get(outputHist_1D->GetName())) {
                outputHist_1D->Write();
            }
            if (!outputFile->Get(outputHist_1D_CompressedBin->GetName())) {
                outputHist_1D_CompressedBin->Write();
            }
            
            if (outputHist_1D_TTBarGenSystUp || outputHist_1D_TTBarGenSystDown || outputHist_1D_CompressedBin_TTBarGenSystUp || outputHist_1D_CompressedBin_TTBarGenSystDown) {
                if (!outputFile->Get(outputHist_1D_TTBarGenSystUp->GetName())) {
                    cout << "writing " << outputHist_1D_TTBarGenSystUp->GetName() << endl;
                    outputHist_1D_TTBarGenSystUp->Write();
                }
                if (!outputFile->Get(outputHist_1D_TTBarGenSystDown->GetName())) {
                    outputHist_1D_TTBarGenSystDown->Write();
                }
                if (!outputFile->Get(outputHist_1D_CompressedBin_TTBarGenSystUp->GetName())) {
                    cout << "writing " << outputHist_1D_CompressedBin_TTBarGenSystUp->GetName() << endl;
                    outputHist_1D_CompressedBin_TTBarGenSystUp->Write();
                }
                if (!outputFile->Get(outputHist_1D_CompressedBin_TTBarGenSystDown->GetName())) {
                    outputHist_1D_CompressedBin_TTBarGenSystDown->Write();
                }
            }
        }
        else {
            if (numDimsInput == 1) {
                if (!outputFile->Get(outputHist_1D->GetName())) {
                    outputHist_1D->Write();
                }
            }
            else if (numDimsInput == 2) {
                if (!outputFile->Get(outputHist_2D->GetName())) {
                    outputHist_2D->Write();
                }
            }
            else if (numDimsInput == 3) {
                if (!outputFile->Get(outputHist_3D->GetName())) {
                    outputHist_3D->Write();
                }
            }
        }
    }
};

struct OneDeeHistController {
    //Controller structure that controls the OneDeeHistMaker doing projections
    //by passing it appropriate histograms and what not
    //also handles signal contamination
    
    OneDeeHistMaker ODHM;
    bool addTTBarSyst;
    
    vector<TripleBin> vecBinsToKill;
    
    TFile * inFile, * outFile;
    
    int numDims;
    TString strFileAppend;
    TString strFileLoad;
    
    bool cutControl;
    
    bool doSigContam;
    
    vector<int> vecWhichMT2;
    vector<TString> vecMT2Name;
    
    TString nameInFile, nameOutFile;
    TString baseDir;
    TString outDir;
    
    TString nameIntegralFile;
    
    
    TH1 * binMap;
    
    void CheckHistSyst(TH1F * inputHist, TH1 * inputHist_CV) {
        if (inputHist_CV && inputHist != inputHist_CV) {
            if (inputHist_CV->Integral() > 0) {
                if (inputHist->Integral() < 1E-6) {
                    bool hackedBin = false;
                    int binCounter = 0;
                    for (int iBinX = 1; iBinX <= inputHist_CV->GetNbinsX(); ++iBinX) {
                        for (int iBinY = 1; iBinY <= inputHist_CV->GetNbinsY(); ++iBinY) {
                            for (int iBinZ = 1; iBinZ <= inputHist_CV->GetNbinsZ(); ++iBinZ) {
                                binCounter++;
                                if (hackedBin) continue;
                                if (iBinX != 1 || iBinY != 1 || iBinZ != 1) {
                                    if (inputHist_CV->GetBinContent(iBinX, iBinY, iBinZ) > 0) {
                                        inputHist->SetBinContent(binCounter, TMath::Min(1E-4, inputHist_CV->GetBinContent(iBinX, iBinY, iBinZ)));
                                        hackedBin = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    void RunHistThroughBinMap(TH1 * inputHist, vector<int> * inVecBinMap, TH1 * inputHist_CV = NULL) {
        TString histName;
        vector<int> vecInputHistAxisNBins;
        vector<TAxis *> vecInputHistAxes;
        vector<float> vecInputHistAxisBinWidths;
        SetHistogramData(inputHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
        
        TString currHistName = inputHist->GetName();
        int isStatSyst = 0;
        if (currHistName.Contains("Stat")) {
            if (currHistName.Contains("Up")) {
                isStatSyst = 1;
            }
            else if (currHistName.Contains("Down")) {
                isStatSyst = -1;
            }
        }
        
        int currGlobalBin, currGlobalBestBin;
        
        
        //        vector< vector<int> > vecVecBinMap(vecInputHistAxisNBins[0] * vecInputHistAxisNBins[1] * vecInputHistAxisNBins[2]);
        inVecBinMap->resize(0);
        
        int binCounter = 0;
        /*
         for (int iBinX = vecInputHistAxisNBins[0]; iBinX > 0; --iBinX) {
         for (int iBinY = vecInputHistAxisNBins[1]; iBinY > 0; --iBinY) {
         for (int iBinZ = vecInputHistAxisNBins[2]; iBinZ > 0; --iBinZ) {
         */
        for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
            for (int iBinY = 1; iBinY <= vecInputHistAxisNBins[1]; ++iBinY) {
                for (int iBinZ = 1; iBinZ <= vecInputHistAxisNBins[2]; ++iBinZ) {
                    binCounter++;
                    currGlobalBin = inputHist->GetBin(iBinX, iBinY, iBinZ);
                    currGlobalBestBin = (int) (binMap->GetBinContent(iBinX, iBinY, iBinZ) + 0.5);
                    /*
                     cout << "binCounter " << binCounter << endl;
                     cout << "currGlobalBin " << currGlobalBin << endl;
                     cout << "currGlobalBestBin " << currGlobalBestBin << endl;
                     */
                    if (currGlobalBin == currGlobalBestBin) {
                        //cout << "found a match: binCounter " << binCounter << endl;
                        inVecBinMap->push_back(binCounter);
                    }
                }
            }
        }
        
        for (int iBinX = vecInputHistAxisNBins[0]; iBinX > 0; --iBinX) {
            for (int iBinY = vecInputHistAxisNBins[1]; iBinY > 0; --iBinY) {
                for (int iBinZ = vecInputHistAxisNBins[2]; iBinZ > 0; --iBinZ) {
                    
                    
                    
                    currGlobalBin = inputHist->GetBin(iBinX, iBinY, iBinZ);
                    currGlobalBestBin = (int) (binMap->GetBinContent(iBinX, iBinY, iBinZ) + 0.5);
                    /*
                     if (iBinX == 4) {
                     cout << " for iBinX:iBinY:iBinZ = " << iBinX << ": " << iBinY << ": " << iBinZ << endl;
                     cout << " currGlobalBin " << currGlobalBin << endl;
                     cout << " currGlobalBestBin " << currGlobalBestBin << endl;
                     }
                     */
                    AddBins(inputHist, currGlobalBin, currGlobalBestBin, isStatSyst, inputHist_CV);
                }
            }
        }
        /*
         for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
         for (int iBinY = 1; iBinY <= TMath::Min(iBinX, vecInputHistAxisNBins[1]); ++iBinY) {
         for (int iBinZ = 1; iBinZ <= TMath::Min(iBinX, vecInputHistAxisNBins[2]); ++iBinZ) {
         currGlobalBin = inputHist->GetBin(iBinX, iBinY, iBinZ);
         currGlobalBestBin = (int) (binMap->GetBinContent(iBinX, iBinY, iBinZ) + 0.5);
         AddBins(inputHist, currGlobalBin, currGlobalBestBin, isStatSyst, inputHist_CV);
         }
         }
         }
         */
    }
    
    void DefaultVals() {
        ODHM.makeOneDeeHack = true;
        numDims = 1;
        cutControl = false;
        doSigContam = false;
        addTTBarSyst = false;
        vecBinsToKill.resize(0);
    }
    void SetFileAppendString_SigSpecific(TString strSpecific) {
        TString arrFileAppend[3] = {"_OneDee", "_TwoDee", "_ThreeDee"};
        strFileAppend = strSpecific + arrFileAppend[numDims - 1];;
        strFileAppend += "ToOneDee";
    }
    void SetFileAppendString(TString inStrFileLoad = "") {
        TString arrFileAppend[3] = {"_OneDee", "_TwoDee", "_ThreeDee"};
        strFileLoad = inStrFileLoad + arrFileAppend[numDims - 1];
        cout << "strFileLoad " << strFileLoad << endl;
        strFileAppend = strFileLoad;
        if (ODHM.makeOneDeeHack) {
            strFileAppend += "ToOneDee";
        }
    }
    void SetMT2AxisString() {
        
        TString arrNameMT2[3] = {"MT2ll", "MT2lblb", "MT2bb_ZMET"};
        vecWhichMT2.resize(0);
        vecWhichMT2.push_back(0);
        if (numDims > 1) {
            vecWhichMT2.push_back(1);
            if (numDims > 2) {
                vecWhichMT2.push_back(2);
            }
        }
        
        vecMT2Name.resize(0);
        
        for (unsigned int iAxis = 0; iAxis < vecWhichMT2.size(); ++iAxis) {
            cout << "pushing back " << vecWhichMT2[iAxis] << " into the MT2 string " << endl;
            vecMT2Name.push_back(arrNameMT2[vecWhichMT2[iAxis]]);
        }
        
    }
    void SetBinsToKill(vector<int> * inVecBinX, vector<int> * inVecBinY, vector<int> * inVecBinZ) {
        vecBinsToKill.resize(0);
        TripleBin currTripBin;
        for (unsigned int iTripleBin = 0; iTripleBin < inVecBinX->size(); ++iTripleBin) {
            currTripBin.binX = inVecBinX->at(iTripleBin);
            currTripBin.binY = inVecBinY->at(iTripleBin);
            currTripBin.binZ = inVecBinZ->at(iTripleBin);
            vecBinsToKill.push_back(currTripBin);
        }
    }
    void SetStringsData() {
        baseDir = "Data/";
        outDir = "Data/";
        TString nameAppend = strFileLoad + ".root";
        nameInFile = "Data" + nameAppend;
        nameOutFile = TString("Data") + strFileAppend + TString(".root");
        if (vecBinsToKill.size() != 0) {
            nameOutFile = TString("Data") + strFileAppend;
            for (unsigned int iTripleBin = 0; iTripleBin < vecBinsToKill.size(); ++iTripleBin) {
                nameOutFile += "_Bin";
                nameOutFile += vecBinsToKill[iTripleBin].binX;
                nameOutFile += vecBinsToKill[iTripleBin].binY;
                nameOutFile += vecBinsToKill[iTripleBin].binZ;
            }
            nameOutFile += ".root";
        }
        nameIntegralFile = outDir + "ShapeInfo_Data" + strFileAppend + TString(".txt");
    }
    void SetStringsBkg(int whichBkgFile = 0) {
        TString arrFileAppendGhost[3] = {"OneDee", "TwoDee", "ThreeDee"};
        
        baseDir = "Backgrounds/";
        outDir = "Backgrounds/";
        TString arrFileName[3] = {"TTBar", "NonTTBar", "Ghost"};
        nameInFile = arrFileName[whichBkgFile];
        if (whichBkgFile == 2) {
            nameInFile += arrFileAppendGhost[numDims - 1];
        }
        else {
            nameInFile += strFileLoad;
        }
        nameInFile += ".root";
        if (nameInFile.BeginsWith("TTBar")) {
            addTTBarSyst = true;
        }
        else {
            addTTBarSyst = false;
        }
        cout << "going to try to open " << nameInFile << endl;
        
        nameOutFile = arrFileName[whichBkgFile];
        nameOutFile += strFileAppend + TString(".root");
        if (vecBinsToKill.size() != 0) {
            nameOutFile = arrFileName[whichBkgFile] + strFileAppend;
            for (unsigned int iTripleBin = 0; iTripleBin < vecBinsToKill.size(); ++iTripleBin) {
                nameOutFile += "_Bin";
                nameOutFile += vecBinsToKill[iTripleBin].binX;
                nameOutFile += vecBinsToKill[iTripleBin].binY;
                nameOutFile += vecBinsToKill[iTripleBin].binZ;
            }
            nameOutFile += ".root";
        }
        nameIntegralFile = outDir + "ShapeInfo_MC";
        nameIntegralFile += whichBkgFile;
        nameIntegralFile += TString("") + strFileAppend + TString(".txt");
    }
    void SetStringsSignal(SUSYT2LimitParams * inSUSYT2LP, SUSYMassLimitParams * inSUSYMLP,  LimitParametersContainer * inLPC) {
        TString strUnder = "_";
        
        baseDir = "../../EfficiencyMaps/" + inSUSYT2LP->SavingName(true) + TString("/SignalYieldMT2Histograms/") + inSUSYMLP->SavingName(true);
        outDir = "Signal/" + inSUSYT2LP->SavingName(true) + inSUSYMLP->SavingName(true);
        
        System_MakeDir(outDir);
        
        nameInFile = "h_" + inSUSYMLP->SavingName(false) + inSUSYT2LP->SavingName(false);
        //        nameInFile += TString("_") + inLPC->strFullSel;
        if (inLPC->typeDilepChannel > -1) nameInFile += inLPC->strDilepChannel;
        nameInFile += inLPC->strFullSel;
        nameInFile += "_" + vecMT2Name[0];
        if (vecMT2Name.size() > 1) {
            nameInFile += "_vs_" + vecMT2Name[1];
            if (vecMT2Name.size() > 2) {
                nameInFile += "_vs_" + vecMT2Name[2];
            }
        }
        nameInFile += doSigContam ? "_WithSigContam" : "_NoSigContam";
        nameInFile += ".root";
        
        TString appendNamesTTBarGens[4] = {"_Madgraph_Exclusive", "_MCatNLO", "_Powheg", "_Madgraph"};
        for (int iString = 0; iString < 4; ++iString) {
            int index = nameInFile.Index(appendNamesTTBarGens[iString]);
            int length = 0;
            if (index != -1) {
                length = appendNamesTTBarGens[iString].Length();
                nameInFile.Replace(index, length, "");
            }
        }
        
        
        nameOutFile = "h_" + inSUSYMLP->SavingName(false) + inSUSYT2LP->SavingName(false);
        if (inLPC->typeDilepChannel > -1) nameOutFile += inLPC->strDilepChannel;
        nameOutFile += inLPC->strFullSel;
        nameOutFile += strUnder + strFileAppend;
        nameOutFile += doSigContam ? "_WithSigContam" : "_NoSigContam";
        if (vecBinsToKill.size() != 0) {
            for (unsigned int iTripleBin = 0; iTripleBin < vecBinsToKill.size(); ++iTripleBin) {
                nameOutFile += "_Bin";
                nameOutFile += vecBinsToKill[iTripleBin].binX;
                nameOutFile += vecBinsToKill[iTripleBin].binY;
                nameOutFile += vecBinsToKill[iTripleBin].binZ;
            }
        }
        nameOutFile += ".root";
        
        cout << "going to save to " << outDir + nameOutFile << endl;
        //        "ShapeInfo_Data" + strFileAppend + TString(".txt");
        //        nameIntegralFile = outDir + "ShapeInfo_Signal" + inLPC->strFullSel + TString("_") + strFileAppend;
        nameIntegralFile = outDir + "ShapeInfo_Signal" + strFileAppend;
        nameIntegralFile += doSigContam ? "_WithSigContam" : "_NoSigContam";
        nameIntegralFile += TString(".txt");
    }
    void LoadHist(TString strNameToSet = "", bool doVerb = 0) {
        outFile = TFile::Open(outDir + nameOutFile);
        if (doVerb) {
            cout << "reading from " << outFile->GetName() << endl;
        }
        ODHM.outputHist_1D = (TH1F *) outFile->Get(strNameToSet);
    }
    
    void IterateOverFileHists(TString strNameToSet = "", bool doVerb = 0) {
        inFile = TFile::Open(baseDir + nameInFile);
        if (doVerb) {
            cout << "inFile name " << inFile->GetName() << endl;
        }
        outFile = new TFile(outDir + nameOutFile, "RECREATE");
        if (doVerb) {
            cout << "outFile name " << outFile->GetName() << endl;
        }
        TString path( (char*)strstr( inFile->GetPath(), ":" ) );
        path.Remove( 0, 2 );
        inFile->cd( path );
        TH1 * currHistToRebin;
        
        TDirectory * current_sourcedir = gDirectory;
        TKey * key;
        
        vector<TString> vecCurrGoodStrings(0);
        TString currName;
        TIter grabCVKey( current_sourcedir->GetListOfKeys() );
        TH1 * currHistCV;
        TH1F * currHistCV_Rebinned;
        while ( (key = (TKey *) grabCVKey())) {
            currName = key->GetName();
            if (!currName.Contains("Up") && !currName.Contains("Down")) {
                vecCurrGoodStrings.push_back(currName);
                currHistCV = (TH1 *) current_sourcedir->Get(currName);
            }
        }
        
        
        TIter nextkey( current_sourcedir->GetListOfKeys() );
        TString currNamePartToReplace;
        bool foundBaseString;
        int indexBaseString;
        bool isCVHist;
        char intBuffer[300];
        while ( (key = (TKey *) nextkey())) {
            foundBaseString = false;
            currName = key->GetName();
            isCVHist = false;
            indexBaseString = -1;
            if (!currName.Contains("Up") && !currName.Contains("Down")) {
                isCVHist = true;
            }
            else {
                isCVHist = false;
            }
            for (unsigned int iHist = 0; iHist < vecCurrGoodStrings.size(); ++iHist) {
                if (currName.Contains(vecCurrGoodStrings[iHist])) {
                    foundBaseString = true;
                    indexBaseString = iHist;
                }
            }
            if (!foundBaseString) {
                cout << "couldn't find base string! " << currName;
            }
            currHistToRebin = (TH1 *) current_sourcedir->Get(key->GetName());
            
            vector<int> vecBinMap;
            RunHistThroughBinMap(currHistToRebin, &vecBinMap, currHistCV);
            
            if (isCVHist && addTTBarSyst && ODHM.makeOneDeeHack) {
                TString nameFileHist = "Backgrounds/TTBarGenShapeUncert/h_TTBarGenShapeUncert_MCatNLODivPowheg";
                if (nameInFile.Contains("EMu")) {
                    nameFileHist += "_EMu";
                }
                else if (nameInFile.Contains("EE")) {
                    nameFileHist += "_EE";
                }
                else if (nameInFile.Contains("MuMu")) {
                    nameFileHist += "_MuMu";
                }
                nameFileHist += ".root";
                
                TFile * inFileTTBarGenSyst = TFile::Open(nameFileHist);
                
                TString nameHist = "h_OneDeeRatio_Corrected";
                TH1F * systHist = (TH1F *) inFileTTBarGenSyst->Get(nameHist);
                
                cout << "found systHist " << nameHist << " from file " << nameFileHist << endl;
                ODHM.SetOutputHist(currHistToRebin, &vecBinMap, vecCurrGoodStrings[indexBaseString], cutControl, &vecBinsToKill,  systHist, strNameToSet, doVerb);
            }
            else {
                ODHM.SetOutputHist(currHistToRebin, &vecBinMap, vecCurrGoodStrings[indexBaseString], cutControl, &vecBinsToKill, NULL, strNameToSet, doVerb);
            }
            if (ODHM.makeOneDeeHack) {
                CheckHistSyst(ODHM.outputHist_1D, currHistCV);
                TString name = ODHM.outputHist_1D->GetName();
                //		if (name.Contains("SignalMCStatDown") && (ODHM.outputHist_1D->Integral() < 1E-6)) {
                // ODHM.outputHist_1D->SetBinContent(2, 1E-6);
                //}
                if (name.Contains("SignalMCStatDown")) {
                    ODHM.outputHist_1D->SetBinContent(2, TMath::Max(1E-6,ODHM.outputHist_1D->GetBinContent(2)));
                    ODHM.outputHist_1D_CompressedBin->SetBinContent(2, TMath::Max(1E-6,ODHM.outputHist_1D_CompressedBin->GetBinContent(2)));
                }
            }
            if (isCVHist) {
                if (ODHM.makeOneDeeHack) {
                    sprintf(intBuffer,"Integral: %0.3f", ODHM.outputHist_1D->Integral());
                }
                else {
                    if (numDims == 1) {
                        sprintf(intBuffer,"Integral: %0.3f", ODHM.outputHist_1D->Integral());
                    }
                    else if (numDims == 2) {
                        sprintf(intBuffer,"Integral: %0.3f", ODHM.outputHist_2D->Integral());
                    }
                    else if (numDims == 3) {
                        sprintf(intBuffer,"Integral: %0.3f", ODHM.outputHist_3D->Integral());
                    }
                }
                TString cmdPrintYield = "echo ";
                vector<TString> vecComArgs(0);
                vecComArgs.push_back("Name:");
                vecComArgs.push_back(key->GetName() + TString(","));
                vecComArgs.push_back(intBuffer);
                vecComArgs.push_back(">> " + nameIntegralFile);
                System_RemoveFile(nameIntegralFile);
                System_GeneralCommand(cmdPrintYield, &vecComArgs);
                cout << "Name: " << key->GetName() << ", " << intBuffer << endl;

            }
            ODHM.WriteToFile(outFile);
        }
        
        outFile->Write();
        outFile->Close();
    }
    
};


struct SingLepShapeHistMaker {
    //Structure used for transforming single lepton BDT cut-and-count results into a 1 bin "shape" experiment
    
    TFile * outFile;
    
    TH1F * outputHist_1D_Data;
    TH1F * outputHist_1D_SigCV;
    TH1F * outputHist_1D_BkgCV;
    
    vector<TH1F *> vecOutputHists_1D_SigSystVarUp;
    vector<TH1F *> vecOutputHists_1D_SigSystVarDown;
    
    vector<TH1F *> vecOutputHists_1D_BkgSystVarUp;
    vector<TH1F *> vecOutputHists_1D_BkgSystVarDown;
    
    TString nameData, nameBkg, nameSig;
    
    TString fullNameInCard;
    TString fullPathOutFile, fullNameOutFile;
    
    
    vector<TString> vecSystNames_Sig, vecSystNames_Sig_Save, vecSystNames_Bkg;
    void SetSystNamesSig() {
        const int numSigSystVals = 8;
        TString arrSigSysts[numSigSystVals] = {"lumi", "LepEffSF", "BTagSF", "JetES", "genRecoilRW", "Stat", "pdf", "SigmaSp"};
        vecSystNames_Sig.resize(0);
        vecSystNames_Sig_Save.resize(0);
        TString currSystName;
        for (int iSystSig = 0; iSystSig < numSigSystVals; ++iSystSig) {
            currSystName = arrSigSysts[iSystSig];
            vecSystNames_Sig.push_back(currSystName);
            if (currSystName.Contains("pdf")) currSystName = "PDF";
            vecSystNames_Sig_Save.push_back(currSystName);
        }
    }
    void SetSystNamesBkg() {
        vecSystNames_Bkg.resize(0);
        vecSystNames_Bkg.push_back("BkgErr");
    }
    void DefaultVals() {
        nameData = "data_obs";
        nameBkg = "bkg";
        nameSig = "signal";
        
        SetSystNamesSig();
        SetSystNamesBkg();
        
        vecOutputHists_1D_SigSystVarUp.resize(vecSystNames_Sig.size());
        vecOutputHists_1D_SigSystVarDown.resize(vecSystNames_Sig.size());
        
        vecOutputHists_1D_BkgSystVarUp.resize(vecSystNames_Bkg.size());
        vecOutputHists_1D_BkgSystVarDown.resize(vecSystNames_Bkg.size());
    }
    void SetFullStrings(LimitParametersContainer * inLPC, SUSYT2LimitParams * inSUSYT2LP, SUSYMassLimitParams * inSUSYMLP) {
        
        TString strSUSYPath = inSUSYT2LP->SavingName(true) + inSUSYMLP->SavingName(true);
        TString strSUSYName = inSUSYT2LP->SavingName(false) + TString("_") + inSUSYMLP->SavingName(false);
        
        TString fullPathInCard = "../../" + strSUSYPath;
        fullNameInCard = fullPathInCard + OutputBaseNameCard(inLPC, inSUSYMLP, inSUSYT2LP);
        
        fullPathOutFile = "../Signal/" + strSUSYPath;
        fullNameOutFile = fullPathOutFile + inLPC->NameSingLepFile() + strSUSYName + ".root";
        fullNameOutFile.Replace(fullNameOutFile.Index("_."), 2, ".");
    }
    void SetHists(vector<float> * inVecCardVals, TString outNameFile, bool doVerb) {
        //assumes that index 0 is data yield
        //assumes that index 1 is signal CV yield
        //assumes that index 2 is background CV yield
        //then does next few indices as systematics on signal
        //followed by systematics on background
        float yieldData = inVecCardVals->at(0);
        float yieldSigCV = inVecCardVals->at(1);
        float yieldBkgCV = inVecCardVals->at(2);
        
        if (doVerb) {
            cout << "yieldData " << yieldData << endl;
            cout << "yieldSigCV " << yieldSigCV << endl;
            cout << "yieldBkgCV " << yieldBkgCV << endl;
        }
        
        stringstream cmdRMYieldFile;
        
        cmdRMYieldFile << "rm -rf " << outNameFile << endl;
        system(cmdRMYieldFile.str().c_str());
        
        stringstream cmdPrintYieldData, cmdPrintYieldSig, cmdPrintYieldBkg;
        
        cmdPrintYieldData << "echo DataYield " << yieldData << " >> " << outNameFile << endl;
        system(cmdPrintYieldData.str().c_str());
        cmdPrintYieldSig << "echo SigYield " << yieldSigCV << " >> " << outNameFile << endl;
        system(cmdPrintYieldSig.str().c_str());
        cmdPrintYieldBkg << "echo BkgYield " << yieldBkgCV << " >> " << outNameFile << endl;
        system(cmdPrintYieldBkg.str().c_str());
        
        unsigned int numPreSystVals = 3;
        unsigned int numSigSystVals = vecSystNames_Sig.size();
        unsigned int numBkgSystVals = vecSystNames_Bkg.size();
        vector<float> vecSystUncertSig(0), vecSystUncertBkg(0);
        for (unsigned int iSystSig = 0; iSystSig < numSigSystVals; ++iSystSig) {
            vecSystUncertSig.push_back(inVecCardVals->at(numPreSystVals + iSystSig));
        }
        for (unsigned int iSystBkg = 0; iSystBkg < numBkgSystVals; ++iSystBkg) {
            vecSystUncertBkg.push_back(inVecCardVals->at(numPreSystVals + numSigSystVals + iSystBkg));
        }
        
        outputHist_1D_Data->SetBinContent(1, yieldData);
        outputHist_1D_SigCV->SetBinContent(1, yieldSigCV);
        outputHist_1D_BkgCV->SetBinContent(1, yieldBkgCV);
        
        float currSystUpVal, currSystDownVal;
        
        for (unsigned int iSystSig = 0; iSystSig < numSigSystVals; ++iSystSig) {
            currSystUpVal = yieldSigCV * vecSystUncertSig[iSystSig];
            currSystDownVal = 2 * yieldSigCV - currSystUpVal;
            if (yieldSigCV != 0) {
                currSystUpVal = TMath::Max((float)1E-4, currSystUpVal);
                currSystDownVal = TMath::Max((float)1E-4, currSystDownVal);
            }
            if (doVerb) {
                cout << "iSystSig " << iSystSig << endl;
                cout << "vecSystNames_Sig_Save[iSystSig] " << vecSystNames_Sig_Save[iSystSig] << endl;
                cout << "vecSystUncertSig[iSystSig] " << vecSystUncertSig[iSystSig] << endl;
                cout << "currSystUpVal " << currSystUpVal << endl;
                cout << "currSystDownVal " << currSystDownVal << endl;
            }
            vecOutputHists_1D_SigSystVarUp[iSystSig]->SetBinContent(1, currSystUpVal);
            vecOutputHists_1D_SigSystVarDown[iSystSig]->SetBinContent(1, currSystDownVal);
        }
        
        for (unsigned int iSystBkg = 0; iSystBkg < numBkgSystVals; ++iSystBkg) {
            currSystUpVal = yieldBkgCV * vecSystUncertBkg[iSystBkg];
            currSystDownVal = 2 * yieldBkgCV - currSystUpVal;
            if (yieldBkgCV != 0) {
                currSystUpVal = TMath::Max((float)1E-4, currSystUpVal);
                currSystDownVal = TMath::Max((float)1E-4, currSystDownVal);
            }
            if (doVerb) {
                cout << "iSystBkg " << iSystBkg << endl;
                cout << "vecSystNames_Bkg[iSystBkg] " << vecSystNames_Bkg[iSystBkg] << endl;
                cout << "vecSystUncertBkg[iSystBkg] " << vecSystUncertBkg[iSystBkg] << endl;
                cout << "currSystUpVal " << currSystUpVal << endl;
                cout << "currSystDownVal " << currSystDownVal << endl;
            }
            vecOutputHists_1D_BkgSystVarUp[iSystBkg]->SetBinContent(1, currSystUpVal);
            vecOutputHists_1D_BkgSystVarDown[iSystBkg]->SetBinContent(1, currSystDownVal);
        }
    }
    
    void MakeFile(LimitParametersContainer * inLPC, SUSYT2LimitParams * inSUSYT2LP, SUSYMassLimitParams * inSUSYMLP, bool doVerb = 0) {
        SetFullStrings(inLPC, inSUSYT2LP, inSUSYMLP);
        
        TString tempOutFileName = "TempInfo/TempFileSingLep_" + inSUSYT2LP->SavingName(false) + TString("_") + inSUSYMLP->SavingName(false) + inLPC->strSigContam;
        tempOutFileName += ".txt";
        TString outNameFile = tempOutFileName;
        outNameFile.Replace(outNameFile.Index("TempFile"), 8, "IntegralInfo");
        
        /*
         System_RemoveFile(tempOutFileName);
         System_RemoveFile(outNameFile);
         */
        
        System_MakeDir(fullPathOutFile);
        
        TString strCommand = "./makeSingLepShape_CVTextInfo.sh";
        vector<TString> vecComArgs(0);
        vecComArgs.push_back(fullNameInCard);
        vecComArgs.push_back(fullNameOutFile);
        vecComArgs.push_back(tempOutFileName);
        vecComArgs.push_back(outNameFile);
        
        if (doVerb) {
            System_GeneralCommand(strCommand, &vecComArgs, doVerb);
        }
        System_GeneralCommand(strCommand, &vecComArgs);
        
        stringstream cmdRMTemp;
        stringstream cmdGrabData;
        stringstream cmdGrabSigCV, cmdGrabBkgCV;
        stringstream cmdGrabSigSystErr, cmdGrabBkgSystErr;
        
        stringstream cmdMkdirForFile, cmdCPCardToNewDir;
        /*
         cmdMkdirForFile << "mkdir -p " << fullPathOutFile << endl;
         system(cmdMkdirForFile.str().c_str());
         
         cmdCPCardToNewDir << "cp " << fullPathInCard << fullNameInCard << " " << fullPathOutFile << endl;
         
         system(cmdCPCardToNewDir.str().c_str());
         
         cmdRMTemp << "rm -f " << tempOutFileName;
         if (doVerb) {
         cout << "cmdRMTemp " << cmdRMTemp.str() << endl;
         }
         system(cmdRMTemp.str().c_str());
         
         cmdGrabData << "cat " << fullPathInCard << fullNameInCard << " | grep observation | awk '{print $2}' >> " << tempOutFileName;
         if (doVerb) {
         cout << "cmdGrabData " << cmdGrabData.str() << endl;
         }
         system(cmdGrabData.str().c_str());
         
         cmdGrabSigCV << "cat " << fullPathInCard << fullNameInCard << " | grep rate | awk '{print $2}' >> " << tempOutFileName;
         if (doVerb) {
         cout << "cmdGrabSigCV " << cmdGrabSigCV.str() << endl;
         }
         system(cmdGrabSigCV.str().c_str());
         
         cmdGrabBkgCV << "cat " << fullPathInCard << fullNameInCard << " | grep rate | awk '{print $3}' >> " << tempOutFileName;
         if (doVerb) {
         cout << "cmdGrabBkgCV " << cmdGrabBkgCV.str() << endl;
         }
         system(cmdGrabBkgCV.str().c_str());
         */
        for (unsigned int iSystSig = 0; iSystSig < vecSystNames_Sig.size(); ++iSystSig) {
            cmdGrabSigSystErr.str("");
            cmdGrabSigSystErr.clear();
            cmdGrabSigSystErr << "cat " << fullNameInCard << " | grep '" << vecSystNames_Sig[iSystSig] << " ' | awk '{print $3}' >> " << tempOutFileName;
            if (doVerb) {
                cout << "cmdGrabSigSystErr " << cmdGrabSigSystErr.str() << endl;
            }
            system(cmdGrabSigSystErr.str().c_str());
        }
        
        for (unsigned int iSystBkg = 0; iSystBkg < vecSystNames_Bkg.size(); ++iSystBkg) {
            cmdGrabBkgSystErr.str("");
            cmdGrabBkgSystErr.clear();
            cmdGrabBkgSystErr << "cat " << fullNameInCard << " | grep '" << vecSystNames_Bkg[iSystBkg] << " ' | awk '{print $4}' >> " << tempOutFileName;
            if (doVerb) {
                cout << "cmdGrabBkgSystErr " << cmdGrabBkgSystErr.str() << endl;
            }
            system(cmdGrabBkgSystErr.str().c_str());
        }
        
        vector<float> vecVals = ReadSingLepFileFloats(tempOutFileName);
        if (doVerb) {
            for (unsigned int iVal = 0; iVal < vecVals.size(); ++iVal) {
                cout << "vecVals at iVal " << iVal << " is " << vecVals[iVal] << endl;
            }
            cout << "saving to " << fullNameOutFile << endl;
        }
        outFile = new TFile(fullNameOutFile, "RECREATE");
        
        if (doVerb) {
            cout << "creating the histograms " << endl;
        }
        
        TString axisString = ";bin;yield";
        outputHist_1D_Data = new TH1F(nameData, axisString, 1, 0.5, 1.5);
        outputHist_1D_BkgCV = new TH1F(nameBkg, axisString, 1, 0.5, 1.5);
        outputHist_1D_SigCV = new TH1F(nameSig, axisString, 1, 0.5, 1.5);
        
        TString currSystName;
        
        for (unsigned int iSystSig = 0; iSystSig < vecSystNames_Sig.size(); ++iSystSig) {
            currSystName = vecSystNames_Sig_Save[iSystSig];
            vecOutputHists_1D_SigSystVarUp[iSystSig] = new TH1F(nameSig + TString("_") + currSystName + TString("Up"), axisString, 1, 0.5, 1.5);
            vecOutputHists_1D_SigSystVarDown[iSystSig] = new TH1F(nameSig + TString("_") + currSystName + TString("Down"), axisString, 1, 0.5, 1.5);
        }
        
        for (unsigned int iSystBkg = 0; iSystBkg < vecSystNames_Bkg.size(); ++iSystBkg) {
            vecOutputHists_1D_BkgSystVarUp[iSystBkg] = new TH1F(nameBkg + TString("_") + vecSystNames_Bkg[iSystBkg] + TString("Up"), axisString, 1, 0.5, 1.5);
            vecOutputHists_1D_BkgSystVarDown[iSystBkg] = new TH1F(nameBkg + TString("_") + vecSystNames_Bkg[iSystBkg] + TString("Down"), axisString, 1, 0.5, 1.5);
        }
        if (doVerb) {
            cout << "setting the histogram values " << endl;
        }
        SetHists(&vecVals, outNameFile, doVerb);
        
        outFile->Write();
        outFile->Close();
    }
};
