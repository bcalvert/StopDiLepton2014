#include "TFile.h"
#include "TH2F.h"
#include "TString.h"
#include "TStyle.h"
#include "TPaletteAxis.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include "./BasicFunctions_SystemCommands.h"
#include "./HistogramSystematics2.h"
#include "./HistogramSystematicsStructs.h"
#include "./HistogramPlottingFunctions.h"
#include "./GeneralPlotDrawingStructs.h"
#include "./LimitScriptHeaderFiles/LimitScripts_ParametersStruct.h"
#include "./SUSYFunctions/SUSYFunctions_CompositeHeaderFiles.h"
#include "./LimitStructs/ScenarioDrawingParams.h"
#include "./LimitStructs/ScenarioDrawingParams.cc"
#include "./LimitScriptHeaderFiles/LimitScripts_SUSYStruct.h"
#include "./LimitStructs/SUSYT2LimitParams.h"
#include "./LimitStructs/SUSYT2LimitParams.cc"

#include "./LimitScriptHeaderFiles/LimitFunctions_Saving.h"
#include "./LimitScriptHeaderFiles/YieldCalculationFunctions.h"
#include "./LimitScriptHeaderFiles/CardCreationFunctions.h"
#include "./LimitScriptHeaderFiles/LimitRunningFunctions.h"
#include "./LimitScriptHeaderFiles/LimitCombinationFunctions.h"
#include "./StopLimitMakingFunctions.h"
#include "./SmoothingFunctions_Structs/TwoDeeGaussianSmoothingHeaderFiles.h"
#include "./StopLimitMakingStructs.h"
#include "./SmoothingFunctions_Structs/LimitScenarioSmoother_Optimizer.h"
#include "./SmoothingFunctions_Structs/LimitScenarioSmoother_Optimizer.cc"

#define numDilepChannels 3

using namespace std;

typedef pair<float, float> valPlusError;

void FindExtremaBelowDiagonal(TH2F * inputHist, float &minHist, float &maxHist) {
    //Looking below the diagonal Mstop = Mtop + MLSP for a given input 2D histogram (say, signal strength limit), this function returns to maximum value
    float currMax = -1E3;
    float currMin = 1E3;
    float currBinContent;
    for (int iBinX = 1; iBinX < inputHist->GetNbinsX(); ++iBinX) {
        for (int iBinY = 1; iBinY <= iBinX; ++iBinY) {
            currBinContent = inputHist->GetBinContent(iBinX, iBinY);
            if (currBinContent > currMax) {
                currMax = currBinContent;
            }
            else if (currBinContent < currMin) {
                currMin = currBinContent;
            }
        }
    }
    minHist = currMin;
    maxHist = currMax;
}

void SetEnvelopeHistogram(vector<TH2F *> * inVecHists, TH2F * outEnvelopeHist, TH2F * outBookKeepHist, int whichDir, bool doVerb = false) {
    //This function is used as part of PDF error setting
    //For each point in the 2D SUSY Mass plane, it reads in an input vector of 2D histograms containing relative uncertainties and finds the max uncertainty
    //for that point, thereby creating a maximal envelope of the uncertainties
    //As well, the function also saves for each mass point the input uncertainty that gave the largest value (so that one can see which PDF set gives the largest uncertainty)
    
    float currMaxValue = -1E12;
    int nBinsX = inVecHists->at(0)->GetNbinsX();
    int nBinsY = inVecHists->at(0)->GetNbinsY();
    unsigned int numPDFHists = inVecHists->size();
    
    float currPDFValue;
    int currBestPDF = -1;
    
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            currMaxValue = -1E12;
            currBestPDF = -1;
            for (unsigned int iVec = 0; iVec < numPDFHists; ++iVec) {
                currPDFValue = inVecHists->at(iVec)->GetBinContent(iBinX, iBinY);
                if (whichDir < 0) currPDFValue *= -1;
                if (doVerb) {
                    cout << "iBinX " << iBinX << endl;
                    cout << "iBinY " << iBinY << endl;
                    cout << "iVec " << iVec << endl;
                    cout << "currPDFValue " << currPDFValue << endl;
                    cout << "currMaxValue " << currMaxValue << endl;
                }
                if (currPDFValue > currMaxValue) {
                    currMaxValue = currPDFValue;
                    currBestPDF = iVec + 1;
                }
                else if (currPDFValue == currMaxValue) {
                    currBestPDF = -1;
                }
            }
            if (whichDir < 0) currMaxValue *= -1;
            outEnvelopeHist->SetBinContent(iBinX, iBinY, currMaxValue);
            outBookKeepHist->SetBinContent(iBinX, iBinY, currBestPDF);
        }
    }
}

int ModFive(float inputFloat) {
    //Function for rounding an input float to the nearest 5
    int output = (int) inputFloat;
    if (output % 5 == 4) {
        output += 1;
    }
    else if (output % 5 == 1) {
        output -= 1;
    }
    return output;
}
void MakePercentageEfficiencyHist(TH2F * inputHist) {
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    float currBinContent;
    
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        for (int iBinY = 1; iBinY <= vecInputHistAxisNBins[1]; ++iBinY) {
            currBinContent = inputHist->GetBinContent(iBinX, iBinY);
            inputHist->SetBinContent(iBinX, iBinY, currBinContent * 100);
        }
    }
}
void MakeComboSystHist(TH2F * &outputSystHistUp, TH2F * &outputSystHistDown, TH2F * inputHistCV, vector<TH2F *> * inVecSystUpRatioHists, vector<TH2F *> * inVecSystDownRatioHists, bool doVerb) {
    unsigned int numSyst = inVecSystUpRatioHists->size();
    cout << "combining " << numSyst << " systs " << endl;
    if (numSyst == 0) return;
    
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputHistCV, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    double currSumShiftUp, currSumShiftDown;
    double currRatioSystUp, currRatioSystDown;
    double currSystErrUp, currSystErrDown;
    

    outputSystHistUp = (TH2F *) inputHistCV->Clone(histName + "_FullSystUp");
    outputSystHistDown = (TH2F *) inputHistCV->Clone(histName + "_FullSystDown");

    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        for (int iBinY = 1; iBinY <= vecInputHistAxisNBins[1]; ++iBinY) {
            if (iBinX <= iBinY) continue;
            currSumShiftUp = 0.0;
            currSumShiftDown = 0.0;
            
            currSystErrUp = 0.0;
            currSystErrDown = 0.0;
            for (int iSyst = 0; iSyst < numSyst; ++iSyst) {
                currRatioSystUp = inVecSystUpRatioHists->at(iSyst)->GetBinContent(iBinX, iBinY);
                currRatioSystDown = inVecSystDownRatioHists->at(iSyst)->GetBinContent(iBinX, iBinY);
                
                currSumShiftUp += TMath::Power(abs(currRatioSystUp - 1), 2);
                currSumShiftDown += TMath::Power(abs(currRatioSystDown - 1), 2);
            }
            if (iBinX == 9 && iBinY == 1) {
                doVerb = true;
            }
            else {
                doVerb = false;
            }
            currSumShiftUp = TMath::Sqrt(currSumShiftUp);
            currSumShiftDown = TMath::Sqrt(currSumShiftDown);
            pointSystErr(0, currSumShiftUp, -1 * currSumShiftDown, currSystErrUp, currSystErrDown, false, doVerb);
            outputSystHistUp->SetBinContent(iBinX, iBinY, 1 + currSystErrUp);
            outputSystHistDown->SetBinContent(iBinX, iBinY, 1 - currSystErrDown);
        }
    }

    return;
}

TH2F * SystRatioHist(TH2F * inHistCV, TH2F * inHistSyst) {
    //Given two input histograms, central value one: inHistCV, and shifted one: inHistSyst
    //this function calculates the relative shift (compared to central value) that the syst. shifts cause
    int nBinsX_CV = inHistCV->GetNbinsX();
    int nBinsY_CV = inHistCV->GetNbinsY();
    TString nameHist_CV = inHistCV->GetName();
    
    int nBinsX_Syst = inHistSyst->GetNbinsX();
    int nBinsY_Syst = inHistSyst->GetNbinsY();
    TString nameHist_Syst = inHistSyst->GetName();
    
    if (nBinsX_CV != nBinsX_Syst || nBinsY_CV != nBinsY_Syst) {
        cout << "something wrong with number of bins when trying to make ratio hist" << endl;
        cout << "name of CV " << inHistCV->GetName() << endl;
        cout << "name of Syst " << inHistSyst->GetName() << endl;
        
        cout << "nBinsX_CV " << nBinsX_CV << endl;
        cout << "nBinsX_Syst " << nBinsX_Syst << endl;
        cout << "nBinsY_CV " << nBinsY_CV << endl;
        cout << "nBinsY_Syst " << nBinsY_Syst << endl;
    }
    
    TH2F * outHist = (TH2F *) inHistSyst->Clone(nameHist_Syst + TString("Div") + nameHist_CV);
    
    
    float currBinContent_CV, currBinContent_Syst, currBinRatio;
    for (int iBinX = 0; iBinX <= nBinsX_CV + 1; ++iBinX) {
        for (int iBinY = 0; iBinY <= nBinsY_CV + 1; ++iBinY) {
            currBinContent_CV = inHistCV->GetBinContent(iBinX, iBinY);
            currBinContent_Syst = inHistSyst->GetBinContent(iBinX, iBinY);
            if (currBinContent_CV < 1E-8 && currBinContent_Syst > 1E-8) {
                currBinRatio = -1;
            }
            else if (currBinContent_CV < 1E-8 && currBinContent_Syst < 1E-8) {
                currBinRatio = 1.0;
                if (iBinY > iBinX) currBinRatio = 0.0;
            }
            else {
                currBinRatio = currBinContent_Syst / currBinContent_CV;
            }
            outHist->SetBinContent(iBinX, iBinY, currBinRatio);
        }
    }
    return outHist;
}

vector<float> ReadSigContamFileFloats(TString fileName) {
    //Function used as part of signal contamination calculations
    //Reads in a file containing signal contamination info (produced elsewhere) and pops the values into a vector of floats
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

float CalculateNewYield(float origSigYieldSignalRegion, float yieldSigControlRegion, float yieldTTBarSignalRegion, float yieldTTBarControlRegion, bool doVerb = 0) {
    //Function used as part of signal contamination calculations
    //Takes as input
    //S: origSigYieldSignalRegion
    //S_c: yieldSigControlRegion
    //B: yieldTTBarSignalRegion
    //B_c: yieldTTBarControlRegion
    //and calculates the correction to S based off of signal contamination in the control region
    float outYield = origSigYieldSignalRegion;
    float corrTerm = yieldSigControlRegion * yieldTTBarSignalRegion / yieldTTBarControlRegion;
    outYield -= corrTerm;
    if (outYield < 0) outYield = 0.0;
    if (doVerb) {
        cout << "outYield base " << origSigYieldSignalRegion << endl;
        cout << "corrTerm " << corrTerm << endl;
        cout << "outYield post corr " << outYield << endl;
    }
    return outYield;
}

float CalculateSigContamErr_SigContStat(float errSigControlRegion, float yieldTTBarSignalRegion, float yieldTTBarControlRegion, bool doVerb = 0) {
    //Related to the function CalculateNewYield
    //This function calculates the additional error (introduced into the sig. contam. corrected signal yield) stemming from statistical uncertainty
    //of the signal in the control region
    float outErr = errSigControlRegion * yieldTTBarSignalRegion / yieldTTBarControlRegion;
    if (doVerb) {
        cout << "errSigControlRegion " << errSigControlRegion << endl;
        cout << "yieldTTBarSignalRegion " << yieldTTBarSignalRegion << endl;
        cout << "yieldTTBarControlRegion " << yieldTTBarControlRegion << endl;
        cout << "outErr " << outErr << endl;
    }
    return outErr;
}

float CalculateSigContamErr_TTContStat(float yieldSigControlRegion, float yieldTTBarSignalRegion, float errTTBarControlRegion, float yieldTTBarControlRegion, bool doVerb = 0) {
    //Related to the function CalculateNewYield
    //This function calculates the additional error (introduced into the sig. contam. corrected signal yield) stemming from statistical uncertainty
    //of the ttbar background in the control region
    float outErr = yieldSigControlRegion * (yieldTTBarSignalRegion / yieldTTBarControlRegion) * (errTTBarControlRegion / yieldTTBarControlRegion);
    if (doVerb) {
        cout << "yieldSigControlRegion " << yieldSigControlRegion << endl;
        cout << "yieldTTBarSignalRegion " << yieldTTBarSignalRegion << endl;
        cout << "errTTBarControlRegion " << errTTBarControlRegion << endl;
        cout << "yieldTTBarControlRegion " << yieldTTBarControlRegion << endl;
        cout << "outErr " << outErr << endl;
    }
    return outErr;
}

float CalculateSigContamErr_TTSigStat(float yieldSigControlRegion, float errTTBarSignalRegion, float yieldTTBarControlRegion, bool doVerb = 0) {
    //Related to the function CalculateNewYield
    //This function calculates the additional error (introduced into the sig. contam. corrected signal yield) stemming from statistical uncertainty
    //of the ttbar background in the signal region
    float outErr = yieldSigControlRegion * errTTBarSignalRegion / yieldTTBarControlRegion;
    if (doVerb) {
        cout << "yieldSigControlRegion " << yieldSigControlRegion << endl;
        cout << "errTTBarSignalRegion " << errTTBarSignalRegion << endl;
        cout << "yieldTTBarControlRegion " << yieldTTBarControlRegion << endl;
        cout << "outErr " << outErr << endl;
    }
    return outErr;
}

bool IsPDF(TString inNameSyst) {
    
    const int numPDFSysts = 3;
    TString namePDFSysts[numPDFSysts] = {"_CT10EV", "_MSTWEV", "_NNPDFEV"};
    
    /*
    const int numPDFSysts = 2;
    TString namePDFSysts[numPDFSysts] = {"_CT10EV", "_MSTWEV"};
     */
    
    for (int iPDF = 0; iPDF < numPDFSysts; ++iPDF) {
        if (inNameSyst.Contains(namePDFSysts[iPDF])) {
            return true;
        }
    }
    return false;
}


void SetSystVec(vector<TString> * inVecSystNames, bool includePDF = true) {
    //Function used for setting the names of input systematics
    
    
    //    const int numSysts = 9;
    //    TString nameSaveSysts[numSysts] = {"_SignalMCStat", "_LepEffSF", "_LepES", "_JetES", "_BTagEffSF", "_BMisTagSF", "_UncES", "_JetSmear", "_genRecoilRW"};
    
    /*
     const int numSysts = 8;
     TString nameSaveSysts[numSysts] = {"_SignalMCStat", "_LepEffSF", "_LepES", "_JetES", "_BTagSF", "_UncES", "_JetSmear", "_genRecoilRW"};
     */
    
    //    const int numSysts = 9;
    //    TString nameSaveSysts[numSysts] = {"_SignalMCStat", "_LepEffSF", "_LepES", "_JetES", "_BTagSF", "_UncES", "_JetSmear", "_genRecoilRW", "_CT10EV"};
    
    /*
     const int numSysts = 11;
     TString nameSaveSysts[numSysts] = {"_SignalMCStat", "_LepEffSF", "_LepES", "_JetES", "_BTagSF", "_UncES", "_JetSmear", "_genRecoilRW", "_CT10EV", "_MSTWEV", "_NNPDFEV"};
     int endIndex = includePDF ? 11 : 8;
     */
    const int numSysts = 10;
    TString nameSaveSysts[numSysts] = {"_SignalMCStat", "_LepEffSF", "_LepES", "_JetES", "_BTagSF", "_UncES", "_JetSmear", "_genRecoilRW", "_CT10EV", "_MSTWEV"};
    int endIndex = includePDF ? 10 : 8;
    
    if (!includePDF) {
        cout << "not including PDF systematics! " << endl;
    }
    for (int iSyst = 0; iSyst < endIndex; ++iSyst) {
        inVecSystNames->push_back(nameSaveSysts[iSyst]);
    }
}

void SetSystVecFinal(vector<TString> * inVecSystNames, bool includePDF = true) {
    //Function used for setting the names of output systematics
    
    /*    const int numSysts = 7;
     TString nameSaveSysts[numSysts] = {"_LepEffSF", "_LepES", "_JetES", "_BTagSF", "_UncES", "_JetSmear", "_genRecoilRW"};
     */
    /*
    const int numSysts = 11;
    TString nameSaveSysts[numSysts] = {"_SignalMCStat", "_LepEffSF", "_LepES", "_JetES", "_BTagSF", "_UncES", "_JetSmear", "_genRecoilRW", "_CT10EV", "_MSTWEV", "_NNPDFEV"};
    int endIndex = includePDF ? 11 : 8;
    */
    const int numSysts = 10;
    TString nameSaveSysts[numSysts] = {"_SignalMCStat", "_LepEffSF", "_LepES", "_JetES", "_BTagSF", "_UncES", "_JetSmear", "_genRecoilRW", "_CT10EV", "_MSTWEV"};
    int endIndex = includePDF ? 10 : 8;
    
    for (int iSyst = 0; iSyst < endIndex; ++iSyst) {
        inVecSystNames->push_back(nameSaveSysts[iSyst]);
    }
}

void SetSystVecFinal_TTBar(vector<TString> * inVecSystNames) {
    //Function used for setting the names of output systematics on the TTBar
    const int numSysts = 7;
    TString nameSaveSysts[numSysts] = {"_LepEffSF", "_LepES", "_JetES", "_BTagSF", "_UncES", "_JetSmear", "_genRecoilRW"};
    
    for (int iSyst = 0; iSyst < numSysts; ++iSyst) {
        inVecSystNames->push_back(nameSaveSysts[iSyst]);
    }
}
TString SetSMSStringAppend(int typeSMSDir = -1) {
    //Used when generating the efficiency maps -- sets the output directory name
    TString arrTypeSMS[5] = {"LeptFiltSF/", "CoarseMaps/", "SmoothMaps/", "SignalYields/", "SignalYieldText/"};
    TString strSMS = "";
    strSMS += arrTypeSMS[abs(typeSMSDir)];
    if (abs(typeSMSDir) != 0) {
        if (typeSMSDir > 0) {
            strSMS += "SignalRegion/";
        }
        else {
            strSMS += "ControlRegion/";
        }
    }
    return strSMS;
}


TString SetSMSString(int isT2tt, int paramSMS, bool isDir, int typeSMSDir = -1) {
    //Used when generating the efficiency maps -- sets the output directory name
    TString arrTypeSMS[4] = {"LeptFiltSF/", "CoarseMaps/", "SmoothMaps/", "SignalYields/"};
    
    TString strSMS = "";
    if (isT2tt) {
        if (isT2tt > 1) {
            if (isT2tt > 2) {
                strSMS = "T2tb";
            }
            else {
                strSMS = "T2ttVer2";
            }
            if (isDir) strSMS += "/";
        }
        else {
            strSMS = "T2tt";
            if (isDir) strSMS += "/";
        }
        strSMS += "PerPol";
        strSMS += paramSMS;
        if (isDir) strSMS += "/";
    }
    else {
        strSMS = "T2bw";
        if (isDir) {
            strSMS += "/";
            strSMS += "CharginoFrac0.";
            strSMS += paramSMS;
            strSMS += "/";
        }
        else {
            strSMS += "CharginoFrac0.";
            strSMS += paramSMS;
        }
    }
    if (isDir) {
        strSMS += arrTypeSMS[abs(typeSMSDir)];
        if (abs(typeSMSDir) != 0) {
            if (typeSMSDir > 0) {
                strSMS += "SignalRegion/";
            }
            else {
                strSMS += "ControlRegion/";
            }
        }
    }
    
    return strSMS;
}

void SetHistogram(TH2F * &inHist, int typeT2, TString outHistName, int typeHist, TString addName = "") {
    //Given input information, sets up a already labeled 2D histogram for the signal info
    TString axisString = ";";
    axisString += "m_{#tilde{t}_{1}} [GeV];";
    axisString += "m_{#tilde{#chi}_{0}} [GeV]";
    
    float massStopLB = 100;
    float massStopUB = typeT2 ? 800 : 800;
    float massLSPLB = 0;
    float massLSPUB = typeT2 ? 600 : 600;
    
    int preFactor = 1;
    int divFactor = 25;
    float divFactorBin = 12.5;
    
    
    bool isLeptFilt = typeHist == 0;
    if (isLeptFilt) {
        switch (typeT2) {
            case 0:
                massStopUB = 650;
                massLSPUB = 300;
                break;
            case 1:
                massStopUB = 350;
                massLSPUB = 250;
                break;
            case 2:
                massStopUB = 400;
                massLSPUB = 250;
                break;
            default:
                break;
        }
    }
    
    if (typeT2 == 2) {
        preFactor = 2;
        divFactorBin = 6.25;
        /*
         massStopUB = 450;
         massLSPUB = 300;
         */
        massStopUB = 400;
        massLSPUB = 250;
    }
    
    bool isYield = typeHist >= 3;
    
    
    
    /*
    if (isYield) {
        divFactor = 5;
        divFactorBin = 2.5;
        preFactor = 1;
    }
    */
    /*
     if (isYield) {
     divFactor = 10;
     divFactorBin = 5;
     preFactor = 1;
     }
     */
    
    massStopLB -= divFactorBin;
    massStopUB += divFactorBin;
    
    massLSPLB -= divFactorBin;
    massLSPUB += divFactorBin;
    
    int numStop = preFactor * ((massStopUB - massStopLB) / divFactor);
    int numLSP = preFactor * ((massLSPUB - massLSPLB) / divFactor);
    
    inHist = new TH2F(outHistName + addName, axisString, numStop, massStopLB, massStopUB, numLSP, massLSPLB, massLSPUB);
}

#include "./StopSignalYieldStructs/LeptFiltSignalAssociator.h"
#include "./StopSignalYieldStructs/LeptFiltSignalAssociator.cc"

#include "./StopSignalYieldStructs/CoarseMapMaker.h"
#include "./StopSignalYieldStructs/CoarseMapMaker.cc"

#include "./StopSignalYieldStructs/SmoothMapMaker.h"
#include "./StopSignalYieldStructs/SmoothMapMaker.cc"

#include "./StopSignalYieldStructs/SignalYieldMaker.h"
#include "./StopSignalYieldStructs/SignalYieldMaker.cc"

#include "./StopSignalYieldStructs/SignalYieldPlotter.h"
#include "./StopSignalYieldStructs/SignalYieldPlotter.cc"

#include "./StopSignalYieldStructs/SignalYieldMaker_IndivPoint.h"
#include "./StopSignalYieldStructs/SignalYieldMaker_IndivPoint.cc"










struct GhostBackgroundMaker {
    //Struct used to generate the 'Ghost' background, a tiny background
    //used to regulate the behavior of the limit setting code when in a bin with 0 expected background but possibly non-zero data
    
    vector<TString> vecSystNames; //vector that contains names for input systs and names for final systs;
    
    int numDims;
    
    ////Stuff for the output
    //The output MT2 histograms -- for now will be 2D
    TH1F * outMT2Hist_CV_1D;
    vector<TH1F *> vecOutMT2Hist_1D_SystVarUp;
    vector<TH1F *> vecOutMT2Hist_1D_SystVarDown;
    
    TH2F * outMT2Hist_CV_2D;
    vector<TH2F *> vecOutMT2Hist_2D_SystVarUp;
    vector<TH2F *> vecOutMT2Hist_2D_SystVarDown;
    
    TH3F * outMT2Hist_CV_3D;
    vector<TH3F *> vecOutMT2Hist_3D_SystVarUp;
    vector<TH3F *> vecOutMT2Hist_3D_SystVarDown;
    
    //float for "ghost" background value plus systematic
    float valGhost;
    float valGhostSyst;
    
    //vector to contain the histogram axes
    vector< vector<Double_t> > vecAxes;
    vector<int> vecNumBinsAxes;
    // vector to specify which axis is which MT2
    vector<int> vecWhichMT2;
    vector<TString> vecMT2Name;
    TString axisString;
    
    
    //Variables used for file load and out
    
    TFile * outFile; // file that will be made containing the individual file
    TString outBaseDir;
    TString outFileName, outHistName;
    
    void SetAxes() {
        for (unsigned int iMT2 = 0; iMT2 < vecWhichMT2.size(); ++iMT2) {
            switch (vecWhichMT2[iMT2]) {
                case 0:
                    //MT2ll axis
                    if (numDims == 1) {
                        AddInMT2llBounds_Control(&vecAxes, &vecNumBinsAxes);
                    }
                    else if (numDims == 2) {
                        AddInMT2llBounds2DShape(&vecAxes, &vecNumBinsAxes);
                    }
                    else if (numDims == 3) {
                        AddInMT2llBounds3DShape(&vecAxes, &vecNumBinsAxes);
                    }
                    break;
                case 1:
                    //MT2lblb axis
                    if (numDims == 1) {
                        AddInMT2lblbBounds_Control(&vecAxes, &vecNumBinsAxes);
                    }
                    else if (numDims == 2) {
                        AddInMT2lblbBounds2DShape(&vecAxes, &vecNumBinsAxes);
                    }
                    else if (numDims == 3) {
                        AddInMT2lblbBounds3DShape(&vecAxes, &vecNumBinsAxes);
                    }
                    break;
                case 2:
                    //MT2bb_ZMET axis
                    if (numDims == 1) {
                        AddInMT2bb_ZMETBounds_Control(&vecAxes, &vecNumBinsAxes);
                    }
                    else if (numDims == 2) {
                        AddInMT2bb_ZMETBounds2DShape(&vecAxes, &vecNumBinsAxes);
                    }
                    else if (numDims == 3) {
                        AddInMT2bb_ZMETBounds3DShape(&vecAxes, &vecNumBinsAxes);
                    }
                    break;
                default:
                    cout << "vecWhichMT2[iMT2 = " << iMT2 << "] needs to be 0, 1, or 2, right now it is " << vecWhichMT2[iMT2] << endl;
                    break;
            }
        }
    }
    void DefaultVals(int inNumDims = 2, bool setDefaultGhost = true) {
        numDims = inNumDims;
        
        //initialize the input directories to the correct area
        TString arrNameDim[4] = {"", "OneDee", "TwoDee", "ThreeDee"};
        outBaseDir = "ShapeAnalysis/Backgrounds/";
        outFileName = "Ghost";
        outHistName = outFileName;
        outFileName += arrNameDim[numDims];
        outFileName += ".root";
        
        //default is MT2ll vs. MT2lblb shape
        TString arrNameMT2[3] = {"MT2ll", "MT2lblb", "MT2bb_ZMET"};
        TString arrAxisNameMT2[3] = {"M_{T2}(ll) [GeV]", "M_{T2}(lb)(lb) [GeV]", "M_{T2}^{#slash{W}}(bb) [GeV]"};
        
        vecWhichMT2.resize(0);
        vecWhichMT2.push_back(0);
        if (numDims > 1) {
            vecWhichMT2.push_back(1);
            if (numDims > 2) {
                vecWhichMT2.push_back(2);
            }
        }
        
        vecMT2Name.resize(0);
        axisString = "";
        
        for (unsigned int iAxis = 0; iAxis < vecWhichMT2.size(); ++iAxis) {
            vecMT2Name.push_back(arrNameMT2[vecWhichMT2[iAxis]]);
            
            axisString += ";";
            axisString += arrAxisNameMT2[vecWhichMT2[iAxis]];
        }
        
        vecAxes.resize(0);
        vecNumBinsAxes.resize(0);
        
        SetAxes();
        
        // set the ghost value and ghost syst value to be very small
        if (setDefaultGhost) {
            valGhost = 1E-6;
            valGhostSyst = 1E-5;
        }
    }
    void SetGhostYield(TH1 * inHist, int dirSyst = 0) {
        int nBinsX = inHist->GetNbinsX();
        int nBinsY = inHist->GetNbinsY();
        int nBinsZ = inHist->GetNbinsZ();
        float valToFill = valGhost + dirSyst * valGhostSyst;
        for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
            for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
                for (int iBinZ = 1; iBinZ <= nBinsZ; ++iBinZ) {
                    inHist->SetBinContent(iBinX, iBinY, iBinZ, valToFill);
                }
            }
        }
    }
    void SetYieldOutHist() {
        switch (numDims) {
            case 1:
                SetGhostYield(outMT2Hist_CV_1D, 0);
                for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                    SetGhostYield(vecOutMT2Hist_1D_SystVarUp[iSyst], 1);
                    SetGhostYield(vecOutMT2Hist_1D_SystVarDown[iSyst], -1);
                }
                break;
            case 2:
                SetGhostYield(outMT2Hist_CV_2D, 0);
                for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                    SetGhostYield(vecOutMT2Hist_2D_SystVarUp[iSyst], 1);
                    SetGhostYield(vecOutMT2Hist_2D_SystVarDown[iSyst], -1);
                }
                break;
            case 3:
                SetGhostYield(outMT2Hist_CV_3D, 0);
                for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                    SetGhostYield(vecOutMT2Hist_3D_SystVarUp[iSyst], 1);
                    SetGhostYield(vecOutMT2Hist_3D_SystVarDown[iSyst], -1);
                }
                break;
            default:
                cout << "numDims needs to be 1, 2, or 3 -- it is " << numDims << endl;
                break;
        }
    }
    void InitializeVecs() {
        vecSystNames.clear();
        SetSystVec(&vecSystNames);
        
        unsigned int numSysts = vecSystNames.size();
        
        vecOutMT2Hist_1D_SystVarUp.resize(numSysts);
        vecOutMT2Hist_1D_SystVarDown.resize(numSysts);
        vecOutMT2Hist_2D_SystVarUp.resize(numSysts);
        vecOutMT2Hist_2D_SystVarDown.resize(numSysts);
        vecOutMT2Hist_3D_SystVarUp.resize(numSysts);
        vecOutMT2Hist_3D_SystVarDown.resize(numSysts);
        
    }
    void InitializeHistsandOutfile(bool doVerb = 0) {
        outFile = new TFile(outBaseDir + outFileName, "RECREATE");
        if (doVerb) {
            cout << "Base hist name " << outHistName << endl;
            cout << "saving to " << outFileName << endl;
        }
        if (numDims == 1) {
            outMT2Hist_CV_1D = new TH1F(outHistName, axisString, vecNumBinsAxes[0], &vecAxes[0][0]);
            for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                vecOutMT2Hist_1D_SystVarUp[iSyst] = new TH1F(outHistName + vecSystNames[iSyst] + TString("Up"), axisString, vecNumBinsAxes[0], &vecAxes[0][0]);
                vecOutMT2Hist_1D_SystVarDown[iSyst] = new TH1F(outHistName + vecSystNames[iSyst] + TString("Down"), axisString, vecNumBinsAxes[0], &vecAxes[0][0]);
            }
        }
        else if (numDims == 2) {
            outMT2Hist_CV_2D = new TH2F(outHistName, axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0]);
            for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                vecOutMT2Hist_2D_SystVarUp[iSyst] = new TH2F(outHistName + vecSystNames[iSyst] + TString("Up"), axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0]);
                vecOutMT2Hist_2D_SystVarDown[iSyst] = new TH2F(outHistName + vecSystNames[iSyst] + TString("Down"), axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0]);
            }
        }
        else if (numDims == 3) {
            outMT2Hist_CV_3D = new TH3F(outHistName, axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0], vecNumBinsAxes[2], &vecAxes[2][0]);
            for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                vecOutMT2Hist_3D_SystVarUp[iSyst] = new TH3F(outHistName + vecSystNames[iSyst] + TString("Up"), axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0], vecNumBinsAxes[2], &vecAxes[2][0]);
                vecOutMT2Hist_3D_SystVarDown[iSyst] = new TH3F(outHistName + vecSystNames[iSyst] + TString("Down"), axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0], vecNumBinsAxes[2], &vecAxes[2][0]);
            }
        }
    }
    void WriteFile() {
        outFile->Write();
        outFile->Close();
    }
};
