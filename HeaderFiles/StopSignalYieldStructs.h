#include "TFile.h"
#include "TH2F.h"
#include "TString.h"

#include <iostream>

#include "./BasicFunctions_SystemCommands.h"
#include "./HistogramSystematics2.h"
#include "./HistogramSystematicsStructs.h"
#include "./HistogramPlottingFunctions.h"
#include "./LimitScriptHeaderFiles/LimitScripts_SUSYStruct.h"

#include <sstream>
#include <fstream>

using namespace std;

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


void SetSystVec(vector<TString> * inVecSystNames) {
    //Function used for setting the names of input systematics
    
    
    //    const int numSysts = 9;
    //    TString nameSaveSysts[numSysts] = {"_SignalMCStat", "_LepEffSF", "_LepES", "_JetES", "_BTagEffSF", "_BMisTagSF", "_UncES", "_JetSmear", "_genRecoilRW"};
    
    /*
     const int numSysts = 8;
     TString nameSaveSysts[numSysts] = {"_SignalMCStat", "_LepEffSF", "_LepES", "_JetES", "_BTagSF", "_UncES", "_JetSmear", "_genRecoilRW"};
     */
    
    //    const int numSysts = 9;
    //    TString nameSaveSysts[numSysts] = {"_SignalMCStat", "_LepEffSF", "_LepES", "_JetES", "_BTagSF", "_UncES", "_JetSmear", "_genRecoilRW", "_CT10EV"};
    
    const int numSysts = 11;
    TString nameSaveSysts[numSysts] = {"_SignalMCStat", "_LepEffSF", "_LepES", "_JetES", "_BTagSF", "_UncES", "_JetSmear", "_genRecoilRW", "_CT10EV", "_MSTWEV", "_NNPDFEV"};
    
    for (int iSyst = 0; iSyst < numSysts; ++iSyst) {
        inVecSystNames->push_back(nameSaveSysts[iSyst]);
    }
}

void SetSystVecFinal(vector<TString> * inVecSystNames) {
    //Function used for setting the names of output systematics
    
    /*    const int numSysts = 7;
     TString nameSaveSysts[numSysts] = {"_LepEffSF", "_LepES", "_JetES", "_BTagSF", "_UncES", "_JetSmear", "_genRecoilRW"};
     */
    const int numSysts = 11;
    TString nameSaveSysts[numSysts] = {"_SignalMCStat", "_LepEffSF", "_LepES", "_JetES", "_BTagSF", "_UncES", "_JetSmear", "_genRecoilRW", "_CT10EV", "_MSTWEV", "_NNPDFEV"};
    
    for (int iSyst = 0; iSyst < numSysts; ++iSyst) {
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
            strSMS = "T2ttVer2";
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




void SetHistogram(TH2F * &inHist, int isT2tt, TString outHistName, bool isYield, TString addName = "") {
    //Given input information, sets up a already labeled 2D histogram for the signal info
    TString axisString = ";";
    axisString += "M_{#tilde{t}} [GeV];";
    axisString += "M_{#tilde{#chi}_{0}} [GeV]";
    
    int massStopLB = 100;
    int massStopUB = isT2tt ? 1000 : 800;
    int massLSPLB = 0;
    int massLSPUB = isT2tt ? 900 : 700;
    
    int preFactor = isT2tt > 1 ? 2 : 1;
    int divFactor = 25;
    float divFactorBin = isT2tt > 1 ? 6.25 : 12.5;
    
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
    
    inHist = new TH2F(outHistName + addName, axisString, numStop, massStopLB, massStopUB, numStop, massLSPLB, massLSPUB);
}

struct LeptFiltSignalAssociator {
    //Structure used as part of calculating the lepton filter efficiency for input points
    //The lepton filter claims to be approximately a 53% efficient filter, but this can depend upon
    //the given input mass point and decay mode -- we have non-lepton filtered data for every lepton-filtered point
    //however, so we can just calculate the damn efficiency ourselves
    
    TFile * inFileLeptFilt, * inFileNonLeptFilt;
    TFile * inNormFileLeptFilt, * inNormFileNonLeptFilt;
    TString baseDirFile;
    TString baseNameFile, midNameFile, endNameFile;
    TString strStop, strChi0, strChargino, strPerPol;
    TString baseDirNormFile;
    
    TH2F * outHist;
    TFile * outFile;
    
    /*
    void PrintBasic(SUSYT2LimitParams * inSUSYT2LP) {
        TString strSMS = isT2tt ? "T2tt" : "T2bw";
        cout << "Running on " << strSMS << " with paramSMS = " << paramSMS << endl;
        cout << "Saving to " << outFile->GetName() << endl;
    }
    */
    
    void CloseFiles() {
        if (inFileLeptFilt) {
            /*
             cout << "inFileLeptFilt " << inFileLeptFilt << endl;
             cout << "inFileLeptFilt " << inFileLeptFilt->GetName() << endl;
             */
            inFileLeptFilt->Close();
        }
        if (inFileNonLeptFilt) {
            inFileNonLeptFilt->Close();
        }
        if (inNormFileLeptFilt) {
            inNormFileLeptFilt->Close();
        }
        if (inNormFileNonLeptFilt) {
            inNormFileNonLeptFilt->Close();
        }
    }
    
    void DefaultVals(bool isT2tt) {
        baseDirNormFile = "/data/users/bcalvert/StopDiLepCode/NonDataRootFiles/SUSYNormFiles/Histos_";
        baseDirFile = "/data/users/bcalvert/new_top/MCSkimOviedo_Old_vers3/Signal/Output_Tree_";
        baseNameFile = "Tree_";
        midNameFile = "_Oviedo_SkimOutput_Oviedo_wSS2";
        strStop = "_SignalStop";
        strChi0 = "_Chi0";
        if (isT2tt) {
            strChargino = "";
            strPerPol = "_PercentPol";
        }
        else {
            strChargino = "_Chargino0.";
            strPerPol = "_PercentPol";
        }
        endNameFile = "_Output.root";
    }
    
    void SetHistAndOutFile(SUSYT2LimitParams * inSUSYT2LP, bool readFile) {
        TString outHistName = "h_LeptFiltEffSF_";
        outHistName += inSUSYT2LP->SavingName(false);
        
        TString strSMSDir = inSUSYT2LP->SavingName(true) + SetSMSStringAppend(0);
//        TString baseDirOutFile = "EfficiencyMaps/" + strSMSDir;
        TString baseDirOutFile = strSMSDir;
        
        System_MakeDir(baseDirOutFile);
        
        TString outFileName = baseDirOutFile + outHistName;
        outFileName += ".root";
        
        if (readFile) {
            outFile = TFile::Open(outFileName);
            outHist = (TH2F *) outFile->Get(outHistName);
        }
        else {
            outFile = new TFile(outFileName, "RECREATE");
            SetHistogram(outHist, inSUSYT2LP->typeT2, outHistName, false, "");
        }
    }
    void WriteFile() {
        outFile->Write();
        outFile->Close();
    }
    
    TString FullPathFile(TString strSMS, int massStop, int massLSP, int indexPol, int charFrac = -1) {
        bool isT2tt = strSMS.Contains("T2tt");
        
        TString outPath = baseDirFile + strSMS + "/";
        outPath += baseNameFile;
        outPath += strSMS;
        outPath += midNameFile;
        outPath += strStop;
        outPath += massStop;
        outPath += strChi0;
        outPath += massLSP;
        if (!isT2tt) {
            outPath += strChargino;
            if (charFrac == 50) {
                charFrac = 5;
            }
            outPath += charFrac;
        }
        outPath += strPerPol;
        outPath += indexPol * 10;
        outPath += endNameFile;
        return outPath;
    }
    
    void SetFiles(int massStop, int massLSP, SUSYT2LimitParams * inSUSYT2LP, labelMap * inMap, bool doVerb = 0) {
        TString strSMS = "";
        TString strSMS_NonLeptFilt = "";
        if (inSUSYT2LP->typeT2) {
            if (inSUSYT2LP->typeT2 > 1) {
                strSMS = LeptFiltDataSetT2tt_ver2(massStop, massLSP);
            }
            else {
                strSMS = LeptFiltDataSetT2tt(massStop, massLSP, doVerb);
            }
        }
        else {
            strSMS = LeptFiltDataSetT2bw(massStop, massLSP, inSUSYT2LP->charFrac, doVerb);
        }
        TString fullPathFile = FullPathFile(strSMS, massStop, massLSP, inSUSYT2LP->indexPol, inSUSYT2LP->charFrac);
        
        if (doVerb) {
            cout << "strSMS " << strSMS << endl;
            cout << "fullPathFile " << fullPathFile << endl;
        }
        
        if (!strSMS.Contains("FAIL")) {
            labelMap::iterator xIterLabelMap;
            xIterLabelMap = inMap->find(strSMS);
            if (xIterLabelMap != inMap->end()) {
                strSMS_NonLeptFilt = xIterLabelMap->second;
            }
            else {
                cout << "something wiggedy: " << strSMS << " is not in the map, hombre!!" << endl;
            }
            
            TString fullPathFile_NonLeptFilt = FullPathFile(strSMS_NonLeptFilt, massStop, massLSP, inSUSYT2LP->indexPol, inSUSYT2LP->charFrac);
            
            TString fullPathNormFile = baseDirNormFile + strSMS + TString(".root");
            TString fullPathNormFile_NonLeptFilt = baseDirNormFile + strSMS_NonLeptFilt + TString(".root");
            
            inFileLeptFilt = TFile::Open(fullPathFile);
            inFileNonLeptFilt = TFile::Open(fullPathFile_NonLeptFilt);
            
            inNormFileLeptFilt = TFile::Open(fullPathNormFile);
            inNormFileNonLeptFilt = TFile::Open(fullPathNormFile_NonLeptFilt);
        }
    }
    
    void SetBin(int massStop, int massLSP, float paramSMS,  bool isT2tt, bool doVerb = 0) {
        int numDims = isT2tt ? 2 : 3;
        
        float SF;
        float fallbackSF = 0.53; // fall back SF
        float origNumLeptFilt, origNumNonLeptFilt;
        TString grabHistName = "h_nVtx_inclusive";
        float numEventsDilep_LeptFilt, numEventsDilep_NonLeptFilt;
        TH1F * histEventsDilep_LeptFilt, * histEventsDilep_NonLeptFilt;
        if (!inNormFileLeptFilt || !inNormFileNonLeptFilt || !inFileLeptFilt || !inFileNonLeptFilt) {
            cout << "inNormFileLeptFilt " << inNormFileLeptFilt << endl;
            cout << "inNormFileNonLeptFilt " << inNormFileNonLeptFilt << endl;
            cout << "inFileLeptFilt " << inFileLeptFilt << endl;
            cout << "inFileNonLeptFilt " << inFileNonLeptFilt << endl;
            if (massStop < 150) {
                SF = fallbackSF;
            }
            else {
                SF = 0;
            }
        }
        else {
            float baseEffLeptFilt = SignalSkimEfficiencyCalc_SinglePoint(inNormFileLeptFilt, massStop, massLSP, paramSMS, numDims, doVerb);
            float baseEffNonLeptFilt = SignalSkimEfficiencyCalc_SinglePoint(inNormFileNonLeptFilt, massStop, massLSP, paramSMS, numDims, doVerb);
            
            float scaleBackLeptFilt = ScaleBackCalcBasic2(inFileLeptFilt, doVerb);
            float scaleBackNonLeptFilt = ScaleBackCalcBasic2(inFileNonLeptFilt, doVerb);
            
            histEventsDilep_LeptFilt = (TH1F *) inFileLeptFilt->Get(grabHistName);
            histEventsDilep_NonLeptFilt = (TH1F *) inFileNonLeptFilt->Get(grabHistName);
            
            numEventsDilep_LeptFilt = histEventsDilep_LeptFilt->Integral(1, histEventsDilep_LeptFilt->GetNbinsX()+1);
            numEventsDilep_NonLeptFilt = histEventsDilep_NonLeptFilt->Integral(1, histEventsDilep_NonLeptFilt->GetNbinsX()+1);
            
            if (doVerb) {
                cout << "baseEffLeptFilt " << baseEffLeptFilt << endl;
                cout << "baseEffNonLeptFilt " << baseEffNonLeptFilt << endl;
                cout << "scaleBackLeptFilt " << scaleBackLeptFilt << endl;
                cout << "scaleBackNonLeptFilt " << scaleBackNonLeptFilt << endl;
                cout << "numEventsDilep_LeptFilt " << numEventsDilep_LeptFilt << endl;
                cout << "numEventsDilep_NonLeptFilt " << numEventsDilep_NonLeptFilt << endl;
            }
            
            
            float estDilepXSecLeptFilt = scaleBackLeptFilt * baseEffLeptFilt * numEventsDilep_LeptFilt;
            float estDilepXSecNonLeptFilt = scaleBackNonLeptFilt * baseEffNonLeptFilt * numEventsDilep_NonLeptFilt;
            
            if (estDilepXSecNonLeptFilt < 1E-20 || estDilepXSecLeptFilt < 1E-20) {
                SF = 0;
            }
            else {
                //                SF = estDilepXSecLeptFilt / estDilepXSecNonLeptFilt;
                SF = estDilepXSecNonLeptFilt / estDilepXSecLeptFilt;
            }
            
            origNumLeptFilt = 1 / baseEffLeptFilt;
            origNumNonLeptFilt = 1 / baseEffNonLeptFilt;
            if (origNumLeptFilt < 1E4) {
                cout << "setting SF to be 0 because original number of Lepton Filtered guys is too small: " << origNumLeptFilt << endl;
                SF = 0;
            }
            else if (origNumNonLeptFilt < 1E4) {
                cout << "setting SF to be fall back SF (0.53) because original number of Non-Lepton Filtered guys is too small: " << origNumNonLeptFilt << endl;
                SF = fallbackSF;
            }
            else if (estDilepXSecNonLeptFilt > estDilepXSecLeptFilt) {
                cout << "setting SF to be 0 because estDilepXSecLeptFilt is less than estDilepXSecNonLeptFilt " << endl;
                cout << "estDilepXSecLeptFilt: " << estDilepXSecLeptFilt << endl;
                cout << "estDilepXSecNonLeptFilt: " << estDilepXSecNonLeptFilt << endl;
                SF = 0;
            }
            if (doVerb) {
                cout << "estDilepXSecLeptFilt " << estDilepXSecLeptFilt << endl;
                cout << "estDilepXSecNonLeptFilt " << estDilepXSecNonLeptFilt << endl;
                cout << "LeptFilt SF " << SF << endl;
            }
        }
        int binX, binY;
        
        binX = outHist->GetXaxis()->FindBin(massStop);
        binY = outHist->GetYaxis()->FindBin(massLSP);
        outHist->SetBinContent(binX, binY, SF);
    }
};


struct CoarseMapMaker {
    //Structure used to calculate the "coarse" -- that is, no smoothing done to the signal -- efficiency
    //for each mass point in the 2D SUSY plane to pass a given MT2 cut (whether 1D, 2D, or 3D)
    TFile * inFile, * inNormFile;
    
    TString baseDirFile;
    TString baseNameFile, midNameFile, endNameFile;
    TString strStop, strChi0, strChargino, strPerPol;
    TString baseDirNormFile;
    
    int MT2llCut, MT2lblbCut, MT2bb_ZMETCut;
    bool whichIntType;
    
    TH2F * outHist_CV;
    TFile * outFile;
    
    bool doControl;
    int cutMT2ll_Control, cutMT2lblb_Control, cutMT2bb_ZMET_Control;
    
    bool loadXSec;
    TString xsecFileName, xsecHistName;
    TFile * crossSectionFile;
    TH1F * crossSectionHist;
    
    double effCV;
    double errEffCV;
    vector<double> vecEff_SystVarUp;
    vector<double> vecEff_SystVarDown;
    
    vector<TString> vecSystNames, vecSystFinalNames;
    
    vector<TH2F *> vecOutHist_SystVarUp;
    vector<TH2F *> vecOutHist_SystVarDown;
    
    int massStopThresh;
    
    void PrintBasic_Part1(bool isT2tt, int paramSMS) {
        TString strSMS = isT2tt ? "T2tt" : "T2bw";
        cout << "Running on " << strSMS << " with paramSMS = " << paramSMS << endl;
        if (doControl) {
            cout << "making the control region map" << endl;
            cout << "Cutting on MT2ll = " << cutMT2ll_Control << " and MT2lblb = " << cutMT2lblb_Control << " and MT2bb_ZMET = " << cutMT2bb_ZMET_Control << endl;
        }
        else {
            cout << "making the signal region map" << endl;
            cout << "Cutting on MT2ll = " << MT2llCut << " and MT2lblb = " << MT2lblbCut << " and MT2bb_ZMET = " << MT2bb_ZMETCut  << endl;
        }
    }
    void PrintBasic(bool isT2tt, int paramSMS) {
        TString strSMS = isT2tt ? "T2tt" : "T2bw";
        cout << "Running on " << strSMS << " with paramSMS = " << paramSMS << endl;
        cout << "Cutting on MT2ll = " << MT2llCut << " and MT2lblb = " << MT2lblbCut << " and MT2bb_ZMET = " << MT2bb_ZMETCut  << endl;
        if (!loadXSec) {
            cout << "note, making a xsection map " << endl;
        }
        cout << "Saving to " << outFile->GetName() << endl;
    }
    float FindXSec(int massStop) {
        int xBin = crossSectionHist->FindBin(massStop);
        return crossSectionHist->GetBinContent(xBin);
    }
    void LoadXSec() {
        crossSectionFile = TFile::Open(xsecFileName);
        crossSectionHist = (TH1F *) crossSectionFile->Get(xsecHistName);
    }
    
    void CloseFiles() {
        if (inFile) {
            inFile->Close();
        }
        if (inNormFile) {
            inNormFile->Close();
        }
    }
    
    void DefaultCuts() {
        MT2llCut = 0;
        MT2lblbCut = 0;
        MT2bb_ZMETCut = -1;
        whichIntType = 0;
        
        doControl = 0;
        cutMT2ll_Control = 80;
        cutMT2lblb_Control = 0;
        cutMT2bb_ZMET_Control = -1;
    }
    void DefaultVals(LeptFiltSignalAssociator * inLFSC) {
        baseDirNormFile = inLFSC->baseDirNormFile;
        baseDirFile = inLFSC->baseDirFile;
        baseNameFile = inLFSC->baseNameFile;
        midNameFile = inLFSC->midNameFile;
        strStop = inLFSC->strStop;
        strChi0 = inLFSC->strChi0;
        strChargino = inLFSC->strChargino;
        strPerPol = inLFSC->strPerPol;
        endNameFile = inLFSC->endNameFile;
        
        //        loadXSec = true;
        loadXSec = false;
        
        xsecFileName = "../NonDataRootFiles/SignalXSection/referenceXSecs.root";
        xsecHistName = "stop8TeV_NLONLL";
    }
    void DoOverflow(TH2F * inHist) {
        int nBinsX = inHist->GetNbinsX();
        float eff;
        for (int iBinY = 1; iBinY <= inHist->GetNbinsY(); ++iBinY) {
            eff = inHist->GetBinContent(nBinsX, iBinY);
            if (iBinY == 1) {
                inHist->SetBinContent(nBinsX, 0, eff);
                inHist->SetBinContent(nBinsX + 1, 0, eff);
            }
            inHist->SetBinContent(nBinsX + 1, iBinY, eff);
        }
    }
    void DoOverflow() {
        DoOverflow(outHist_CV);
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            DoOverflow(vecOutHist_SystVarUp[iSyst]);
            DoOverflow(vecOutHist_SystVarDown[iSyst]);
        }
    }
    void SetBin(int massStop, int massLSP) {
        int binStop = outHist_CV->GetXaxis()->FindBin(massStop);
        int binLSP = outHist_CV->GetYaxis()->FindBin(massLSP);
        
        /*
         bool doOverflowStop = binStop == outHist_CV->GetNbinsX();
         bool doOverflowLSP = binLSP == outHist_CV->GetNbinsY();
         */
        outHist_CV->SetBinContent(binStop, binLSP, effCV);
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            vecOutHist_SystVarUp[iSyst]->SetBinContent(binStop, binLSP, vecEff_SystVarUp[iSyst]);
            vecOutHist_SystVarDown[iSyst]->SetBinContent(binStop, binLSP, vecEff_SystVarDown[iSyst]);
        }
        /*
         if (doOverflowStop || doOverflowLSP) {
         binStop += doOverflowStop;
         binLSP += doOverflowLSP;
         outHist_CV->SetBinContent(binStop, binLSP, effCV);
         for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
         vecOutHist_SystVarUp[iSyst]->SetBinContent(binStop, binLSP, vecEff_SystVarUp[iSyst]);
         vecOutHist_SystVarDown[iSyst]->SetBinContent(binStop, binLSP, vecEff_SystVarDown[iSyst]);
         }
         }
         */
    }
    TString FullPathFile(TString strSMS, int massStop, int massLSP, int indexPol, int charFrac = -1) {
        bool isT2tt = strSMS.Contains("T2tt");
        
        TString outPath = baseDirFile + strSMS + "/";
        outPath += baseNameFile;
        outPath += strSMS;
        outPath += midNameFile;
        outPath += strStop;
        outPath += massStop;
        outPath += strChi0;
        outPath += massLSP;
        if (!isT2tt) {
            outPath += strChargino;
            if (charFrac == 50) {
                charFrac = 5;
            }
            outPath += charFrac;
        }
        outPath += strPerPol;
        outPath += indexPol * 10;
        outPath += endNameFile;
        return outPath;
    }
    void SetFiles(int massStop, int massLSP, SUSYT2LimitParams * inSUSYT2LP, massMap * inMassMap, bool doVerb = 0) {
        TString strSMS = "";
        
        massCombo currCombo(massStop, massLSP);
        
        massMap::iterator xIterMassMap;
        xIterMassMap = inMassMap->find(currCombo);
        if (xIterMassMap != inMassMap->end()) {
            strSMS = xIterMassMap->second;
        }
        else {
            cout << "something wiggedy: Stop:LSP " << currCombo.first << ":" << currCombo.second << " is not in the map, hombre!!" << endl;
        }
        if (strSMS.Contains("0.50") && !strSMS.Contains("LeptonFilter")) {
            strSMS.Replace(strSMS.Index("0.50"), 4, "0.5");
        }
        TString fullPathFile = FullPathFile(strSMS, massStop, massLSP, inSUSYT2LP->indexPol, inSUSYT2LP->charFrac);
        TString fullPathNormFile = baseDirNormFile + strSMS + TString(".root");
        
        inFile = TFile::Open(fullPathFile);
        inNormFile = TFile::Open(fullPathNormFile);
    }
    
    void CalculatePDFUncert(TFile * fileToUse, TString systPDFName, int massStop, float baseEff, float LeptFiltSF, int * binAddreses, double &totalPDFSystErrUp, double &totalPDFSystErrDown, bool doVerb = false) {
        //This function calculates the PDF uncertainty for a given input file, which actually corresponds more explicitly
        //to a specific mass point in the 2D SUSY mass plane
        //To explain some of the not so obvious input variables
        //baseEff: normalization to apply to the signal to account for total number of signal MC points run at the mass point in question
        //LeptFiltSF: 1.0 for non-lepton filtered datasets, approximately 0.53 for lepton filtered datasets (accounts for effective cross section change from the lepton filter
        //binAddress: an array of size 3 containing the bins to cut on to calculate the overall efficiency (whether for CVal or syst. shift)
        //totalPDFSystErrUp/Down -- contain the quadrature sum of the PDF uncertainty for a specific PDF set for a specific mass point across all the eigenvector shifts
        
        int numPDFCT10 = 25;
        int numPDFMSTW = 23;
        int numPDFNNPDF = 50;
        
        int numPDFToUse = 0;
        if (systPDFName.Contains("CT10")) {
            numPDFToUse = numPDFCT10;
        }
        else if (systPDFName.Contains("MSTW")) {
            numPDFToUse = numPDFMSTW;
        }
        else if (systPDFName.Contains("NNPDF")) {
            numPDFToUse = numPDFNNPDF;
        }
        
        
        TString nameFile = fileToUse->GetName();
        
        Size_t lengthString = TString("_SkimOutput").Length();
        Size_t lengthString2 = TString("_PDFWeights_SkimOutput").Length();
        
        nameFile.Replace(nameFile.Index("_SkimOutput"), lengthString, "_PDFWeights_SkimOutput");
        
        TFile * inFilePDF = TFile::Open(nameFile);
        
        TString histBaseName = "h_SmearMT2ll_vs_MT2lblb_FullCut";
        if ((!doControl && MT2bb_ZMETCut == -1) || (doControl && cutMT2bb_ZMET_Control == -1)) {
            histBaseName = "h_SmearMT2ll_vs_MT2lblb_FullCut";
        }
        else {
            histBaseName = "h_SmearMT2ll_vs_MT2lblb_vs_MT2bb_ZMET_FullCut";
        }
        
        float scaleBack = ScaleBackCalcBasic2(inFilePDF, doVerb);
        
        float estDilepXSec = baseEff * scaleBack * LeptFiltSF;
        
        if (loadXSec) estDilepXSec /= FindXSec(massStop);
        
        if (doVerb) {
            cout << "baseEff " << baseEff << endl;
            cout << "scaleBack for PDF weight file " << scaleBack << endl;
            cout << "LeptFiltSF " << LeptFiltSF << endl;
            cout << "estDilepXSec " << estDilepXSec << endl;
        }
        
        //Hack to fix NNPDF weights that are ridiculous
        float estDilepXSecCV = estDilepXSec;
        float threshDiffDilepXsec = 0.25;
        
        TH1 * inputHist = (TH1 *) inFilePDF->Get(histBaseName);
        
        int binToGet = doControl ? 0 : 1;
        
        int whichIntTypeToUse = doControl && cutMT2lblb_Control != 0 ? 1 : whichIntType;
        
        vector<double> vecIntegrals;
        vector<double> vecIntegralErrors;
        
        PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerb);
        double effCVPDF = vecIntegrals[binToGet] * estDilepXSec;
        double errEffCVPDF = vecIntegralErrors[binToGet] * estDilepXSec;
        if (doVerb) {
            cout << "For PDF weights file " << endl;
            cout << "PDF weights: vecIntegrals at index " << binToGet << " is " << vecIntegrals[binToGet] << endl;
            cout << "PDF weights: vecIntegralErrors at index " << binToGet << " is " << vecIntegralErrors[binToGet] << endl;
            
            cout << "PDF weights: effCV " << effCVPDF << endl;
            cout << "PDF weights: errEffCV " << errEffCVPDF << endl;
        }
        
        double currSystErrUp, currSystErrDown;
        double currEffSystUp, currEffSystDown;
        double currPDFSystErrUpSum = 0.0, currPDFSystErrDownSum = 0.0;
        TString systName;
        for (unsigned int iSystPDF = 1; iSystPDF <= numPDFToUse; ++iSystPDF) {
            systName = systPDFName;
            systName += iSystPDF;
            
            vecIntegrals.clear();
            vecIntegralErrors.clear();
            scaleBack = ScaleBackCalcBasic2(inFilePDF, doVerb, systName + TString("ShiftUp"));
            estDilepXSec = baseEff * scaleBack * LeptFiltSF;
            if (loadXSec) estDilepXSec /= FindXSec(massStop);
            if (doVerb) {
                cout << "for Shift Up for Syst = " << systName << " estDilepXSec is " << estDilepXSec << endl;
                cout << "estDilepXSecCV/estDilepXSec " << estDilepXSecCV/estDilepXSec << endl;
                cout << "estDilepXSec/baseEstDilepXSec " << estDilepXSec/estDilepXSecCV << endl;
            }
            if (TMath::Abs((estDilepXSecCV - estDilepXSec)/estDilepXSecCV) > threshDiffDilepXsec) {
                if (doVerb) {
                    cout << "changing the estDilepXSec because otherwise it's ridic" << endl;
                    cout << "relative difference of dilep xsecs " << TMath::Abs((estDilepXSecCV - estDilepXSec)/estDilepXSecCV) << endl;
                }
                estDilepXSec = estDilepXSecCV;
            }
            inputHist = (TH1 *) inFilePDF->Get(histBaseName + systName + TString("ShiftUp"));
            PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerb);
            if (doVerb) {
                cout << "running on hist " << inputHist->GetName() << endl;
                cout << "for Shift Up for Syst = " << systName << " vecIntegrals[" << binToGet << "] is " << vecIntegrals[binToGet] << endl;
                cout << "current efficiency for syst shift up " << vecIntegrals[binToGet] * estDilepXSec << endl;
            }
            currEffSystUp = vecIntegrals[binToGet] * estDilepXSec;
            
            vecIntegrals.clear();
            vecIntegralErrors.clear();
            scaleBack = ScaleBackCalcBasic2(inFilePDF, doVerb, systName + TString("ShiftDown"));
            estDilepXSec = baseEff * scaleBack * LeptFiltSF;
            //baseEff is 1 / N_events total
            //scaleBack is h_nVtx_inclusive_SystShift / h_BasicWeightIntegral_inclusive
            if (loadXSec) estDilepXSec /= FindXSec(massStop);
            if (doVerb) {
                cout << "for Shift Down for Syst = " << systName << " estDilepXSec is " << estDilepXSec << endl;
                cout << "estDilepXSecCV/estDilepXSec " << estDilepXSecCV/estDilepXSec << endl;
                cout << "estDilepXSec/estDilepXSecCV " << estDilepXSec/estDilepXSecCV << endl;
            }
            if (TMath::Abs((estDilepXSecCV - estDilepXSec)/estDilepXSecCV) > threshDiffDilepXsec) {
                if (doVerb) {
                    cout << "changing the estDilepXSec because otherwise it's ridic" << endl;
                    cout << "relative difference of dilep xsecs " << TMath::Abs((estDilepXSecCV - estDilepXSec)/estDilepXSecCV) << endl;
                }
                estDilepXSec = estDilepXSecCV;
            }
            inputHist = (TH1 *) inFilePDF->Get(histBaseName + systName + TString("ShiftDown"));
            PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerb);
            if (doVerb) {
                cout << "running on hist " << inputHist->GetName() << endl;
                cout << "for Shift Down for Syst = " << systName << " vecIntegrals[" << binToGet << "] is " << vecIntegrals[binToGet] << endl;
                cout << "current efficiency for syst shift up " << vecIntegrals[binToGet] * estDilepXSec << endl;
            }
            currEffSystDown = vecIntegrals[binToGet] * estDilepXSec;
            
            pointSystErr(effCVPDF, currEffSystUp, currEffSystDown, currSystErrUp, currSystErrDown, false, doVerb);
            
            if (doVerb) {
                if (effCVPDF < 1E-8) {
                    cout << "relErr Up for iSystPDF " << iSystPDF << " is uncalculable, effCVPDF is 0 " << endl;
                    cout << "relErr Down for iSystPDF " << iSystPDF << " is uncalculable, effCVPDF is 0 " << endl;
                }
                else {
                    cout << "relErr Up for iSystPDF " << iSystPDF << " is " << currSystErrUp / effCVPDF << endl;
                    cout << "relErr Down for iSystPDF " << iSystPDF << " is " << currSystErrDown / effCVPDF << endl;
                }
            }
            
            currPDFSystErrUpSum += TMath::Power(currSystErrUp, 2);
            currPDFSystErrDownSum += TMath::Power(currSystErrDown, 2);
        }
        totalPDFSystErrUp = TMath::Sqrt(currPDFSystErrUpSum);
        totalPDFSystErrDown = TMath::Sqrt(currPDFSystErrDownSum);
        if (doVerb) {
            cout << "for PDF " << systPDFName << endl;
            cout << "totalPDFSystErrUp " << totalPDFSystErrUp << endl;
            cout << "totalPDFSystErrDown " << totalPDFSystErrDown << endl;
        }
        inFilePDF->Close();
    }
    
    void SetEff(LeptFiltSignalAssociator * inLFSC, int massStop, int massLSP, float paramSMS, bool isT2tt, bool doVerb = 0) {
        int numDims = isT2tt ? 2 : 3;
        
        TFile * fileToUse = inFile;
        TFile * normFileToUse = inNormFile;
        
        TString nameFile = inFile->GetName();
        bool isLeptFilt = nameFile.Contains("LeptonFilter");
        if (doVerb) {
            cout << "nameFile " << nameFile << endl;
            cout << "isLeptFilt? " << isLeptFilt << endl;
        }
        int binStop = outHist_CV->GetXaxis()->FindBin(massStop);
        int binLSP = outHist_CV->GetYaxis()->FindBin(massLSP);
        float LeptFiltSF = isLeptFilt ? inLFSC->outHist->GetBinContent(binStop, binLSP) : 1.0;
        if (doVerb) {
            cout << "LeptFiltSF from the file " << inLFSC->outHist->GetName() << " is " << LeptFiltSF << endl;
        }
        if (isLeptFilt && (LeptFiltSF == 0 || LeptFiltSF > 1)) {
            fileToUse = inLFSC->inFileNonLeptFilt;
            normFileToUse = inLFSC->inNormFileNonLeptFilt;
            LeptFiltSF = 1.0;
        }
        
        float baseEff = SignalSkimEfficiencyCalc_SinglePoint(normFileToUse, massStop, massLSP, paramSMS, numDims, doVerb);
        float scaleBack = ScaleBackCalcBasic2(fileToUse, doVerb);
        
        float estDilepXSec = baseEff * scaleBack * LeptFiltSF;
        
        if (loadXSec) estDilepXSec /= FindXSec(massStop);
        
        if (doVerb) {
            cout << "baseEff " << baseEff << endl;
            cout << "scaleBack " << scaleBack << endl;
            cout << "LeptFiltSF " << LeptFiltSF << endl;
            cout << "estDilepXSec " << estDilepXSec << endl;
        }
        
        int binAddreses[3] = {1, 1, 1};
        vector<double> vecIntegrals;
        vector<double> vecIntegralErrors;
        
        const int numBTagSysts = 2;
        TString nameBTagSysts[numBTagSysts] = {"_BTagEffSF", "_BMisTagSF"};
        
        if (doVerb) {
            cout << "fileToUse name " << fileToUse->GetName() << endl;
        }
        
        TString histBaseName = "h_SmearMT2ll_vs_MT2lblb_FullCut";
        if ((!doControl && MT2bb_ZMETCut == -1) || (doControl && cutMT2bb_ZMET_Control == -1)) {
            histBaseName = "h_SmearMT2ll_vs_MT2lblb_FullCut";
        }
        else {
            histBaseName = "h_SmearMT2ll_vs_MT2lblb_vs_MT2bb_ZMET_FullCut";
        }
        /*
         if (doControl) {
         histBaseName += "TTBarControl";
         }
         else {
         histBaseName += "Signal";
         }
         */
        if (doVerb) {
            cout << "trying to grab " << histBaseName << endl;
        }
        TH1 * inputHist = (TH1 *) fileToUse->Get(histBaseName);
        
        int binToGet = doControl ? 0 : 1;
        
        int whichIntTypeToUse = doControl && cutMT2lblb_Control != 0 ? 1 : whichIntType;
        
        if (doControl) {
            if (cutMT2bb_ZMET_Control == -1) {
                BinFinder(binAddreses, inputHist, cutMT2ll_Control, cutMT2lblb_Control, 1e99, false, doVerb);
            }
            else {
                BinFinder(binAddreses, inputHist, cutMT2ll_Control, cutMT2lblb_Control, cutMT2bb_ZMET_Control, false, doVerb);
            }
        }
        else {
            if (MT2bb_ZMETCut == -1) {
                BinFinder(binAddreses, inputHist, MT2llCut, MT2lblbCut, 1e99, false, doVerb);
            }
            else {
                BinFinder(binAddreses, inputHist, MT2llCut, MT2lblbCut, MT2bb_ZMETCut, false, doVerb);
            }
        }
        
        if (doVerb) {
            cout << endl;
            cout << "binAddress for x " << binAddreses[0] << endl;
            cout << "binAddress for y " << binAddreses[1] << endl;
            if (MT2bb_ZMETCut != -1 || cutMT2bb_ZMET_Control != -1) {
                cout << "binAddress for z " << binAddreses[2] << endl;
            }
        }
        
        PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerb);
        effCV = vecIntegrals[binToGet] * estDilepXSec;
        errEffCV = vecIntegralErrors[binToGet] * estDilepXSec;
        if (doVerb) {
            cout << "vecIntegrals at index " << binToGet << " is " << vecIntegrals[binToGet] << endl;
            cout << "vecIntegralErrors at index " << binToGet << " is " << vecIntegralErrors[binToGet] << endl;
            
            cout << "effCV " << effCV << endl;
            cout << "errEffCV " << errEffCV << endl;
        }
        vecEff_SystVarUp.resize(0);
        vecEff_SystVarDown.resize(0);
        
        double currEffSystUp, currEffSystDown;
        double currSystErrUp, currSystErrDown;
        
        float estDilepXSecCV = estDilepXSec;
        
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            if (vecSystNames[iSyst].Contains("BTagSF")) {
                double currBTagSFErrUpSum = 0.0, currBTagSFErrDownSum = 0.0;
                for (int iBTag = 0; iBTag < 2; ++iBTag) {
                    //do shift up first
                    vecIntegrals.clear();
                    vecIntegralErrors.clear();
                    scaleBack = ScaleBackCalcBasic2(fileToUse, doVerb, nameBTagSysts[iBTag] + TString("ShiftUp"));
                    estDilepXSec = baseEff * scaleBack * LeptFiltSF;
                    if (loadXSec) estDilepXSec /= FindXSec(massStop);
                    if (doVerb) {
                        cout << "for Shift Up for BTag = " << nameBTagSysts[iBTag] << " estDilepXSec is " << estDilepXSec << endl;
                    }
                    inputHist = (TH1 *) fileToUse->Get(histBaseName + nameBTagSysts[iBTag] + TString("ShiftUp"));
                    PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerb);
                    if (doVerb) {
                        cout << "running on hist " << inputHist->GetName() << endl;
                        cout << "for Shift Up for BTag = " << nameBTagSysts[iBTag] << " vecIntegrals[" << binToGet << "] is " << vecIntegrals[binToGet] << endl;
                        cout << "current efficiency for syst shift up " << (vecIntegrals[binToGet] * estDilepXSec) << endl;
                    }
                    currEffSystUp = vecIntegrals[binToGet] * estDilepXSec;
                    if (doVerb) {
                        cout << "iBTag " << iBTag << " currEffSystUp " << currEffSystUp << endl;
                    }
                    //do shift up first
                    vecIntegrals.clear();
                    vecIntegralErrors.clear();
                    scaleBack = ScaleBackCalcBasic2(fileToUse, doVerb, nameBTagSysts[iBTag] + TString("ShiftDown"));
                    estDilepXSec = baseEff * scaleBack * LeptFiltSF;
                    if (loadXSec) estDilepXSec /= FindXSec(massStop);
                    if (doVerb) {
                        cout << "for Shift Down for BTag = " << nameBTagSysts[iBTag] << " estDilepXSec is " << estDilepXSec << endl;
                    }
                    inputHist = (TH1 *) fileToUse->Get(histBaseName + nameBTagSysts[iBTag] + TString("ShiftDown"));
                    PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerb);
                    if (doVerb) {
                        cout << "running on hist " << inputHist->GetName() << endl;
                        cout << "for Shift Down for BTag = " << nameBTagSysts[iBTag] << " vecIntegrals[" << binToGet << "] is " << vecIntegrals[binToGet] << endl;
                        cout << "current efficiency for syst shift down " << (vecIntegrals[binToGet] * estDilepXSec) << endl;
                    }
                    currEffSystDown = vecIntegrals[binToGet] * estDilepXSec;
                    if (doVerb) {
                        cout << "iBTag " << iBTag << " currEffSystDown " << currEffSystDown << endl;
                    }
                    pointSystErr(effCV, currEffSystUp, currEffSystDown, currSystErrUp, currSystErrDown, false, doVerb);
                    
                    currBTagSFErrUpSum += TMath::Power(currSystErrUp, 2);
                    currBTagSFErrDownSum += TMath::Power(currSystErrDown, 2);
                }
                if (doVerb) {
                    cout << "TMath::Sqrt(currBTagSFErrUpSum) " << TMath::Sqrt(currBTagSFErrUpSum) << endl;
                    cout << "TMath::Sqrt(currBTagSFErrDownSum) " << TMath::Sqrt(currBTagSFErrDownSum) << endl;
                }
                vecEff_SystVarUp.push_back(TMath::Max(0.0, effCV + TMath::Sqrt(currBTagSFErrUpSum)));
                vecEff_SystVarDown.push_back(TMath::Max(0.0, effCV - TMath::Sqrt(currBTagSFErrDownSum)));
            }
            else if (vecSystNames[iSyst].Contains("SignalMCStat")) {
                vecEff_SystVarUp.push_back(effCV + errEffCV);
                vecEff_SystVarDown.push_back(effCV - errEffCV);
            }
            else if (vecSystNames[iSyst].Contains("CT10") || vecSystNames[iSyst].Contains("MSTW") || vecSystNames[iSyst].Contains("NNPDF")) {
                double currPDFSystErrUp, currPDFSystErrDown;
                if (massStop < massStopThresh) {
                    CalculatePDFUncert(fileToUse, vecSystNames[iSyst], massStop, baseEff, LeptFiltSF, binAddreses, currPDFSystErrUp, currPDFSystErrDown, doVerb);
                    vecEff_SystVarUp.push_back(TMath::Max(0.0, effCV + currPDFSystErrUp));
                    vecEff_SystVarDown.push_back(TMath::Max(0.0, effCV - currPDFSystErrDown));
                }
                else {
                    int binThreshX = vecOutHist_SystVarUp[iSyst]->GetXaxis()->FindBin(massStopThresh);
                    int binThreshY = vecOutHist_SystVarUp[iSyst]->GetYaxis()->FindBin(massLSP);
                    vecEff_SystVarUp.push_back(vecOutHist_SystVarUp[iSyst]->GetBinContent(binThreshX, binThreshY));
                    vecEff_SystVarDown.push_back(vecOutHist_SystVarUp[iSyst]->GetBinContent(binThreshX, binThreshY));
                }
            }
            else {
                //do shift up first
                vecIntegrals.clear();
                vecIntegralErrors.clear();
                scaleBack = ScaleBackCalcBasic2(fileToUse, doVerb, vecSystNames[iSyst] + TString("ShiftUp"));
                estDilepXSec = baseEff * scaleBack * LeptFiltSF;
                if (loadXSec) estDilepXSec /= FindXSec(massStop);
                if (doVerb) {
                    cout << "for Shift Up for Syst = " << vecSystNames[iSyst] << " estDilepXSec is " << estDilepXSec << endl;
                }
                inputHist = (TH1 *) fileToUse->Get(histBaseName + vecSystNames[iSyst] + TString("ShiftUp"));
                PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerb);
                if (doVerb) {
                    cout << "running on hist " << inputHist->GetName() << endl;
                    cout << "for Shift Up for Syst = " << vecSystNames[iSyst] << " vecIntegrals[" << binToGet << "] is " << vecIntegrals[binToGet] << endl;
                    cout << "current efficiency for syst shift up " << vecIntegrals[binToGet] * estDilepXSec << endl;
                }
                currEffSystUp = vecIntegrals[binToGet] * estDilepXSec;
                
                
                //do shift down next
                vecIntegrals.clear();
                vecIntegralErrors.clear();
                scaleBack = ScaleBackCalcBasic2(fileToUse, doVerb, vecSystNames[iSyst] + TString("ShiftDown"));
                estDilepXSec = baseEff * scaleBack * LeptFiltSF;
                if (loadXSec) estDilepXSec /= FindXSec(massStop);
                if (doVerb) {
                    cout << "for Shift Down for Syst = " << vecSystNames[iSyst] << " estDilepXSec is " << estDilepXSec << endl;
                }
                inputHist = (TH1 *) fileToUse->Get(histBaseName + vecSystNames[iSyst] + TString("ShiftDown"));
                PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerb);
                if (doVerb) {
                    cout << "running on hist " << inputHist->GetName() << endl;
                    cout << "for Shift Down for Syst = " << vecSystNames[iSyst] << " vecIntegrals[" << binToGet << "] is " << vecIntegrals[binToGet] << endl;
                    cout << "current efficiency for syst shift down " << vecIntegrals[binToGet] * estDilepXSec << endl;
                }
                currEffSystDown = vecIntegrals[binToGet] * estDilepXSec;
                
                
                pointSystErr(effCV, currEffSystUp, currEffSystDown, currSystErrUp, currSystErrDown, false, doVerb);
                
                if (doVerb) {
                    if (effCV < 1E-8) {
                        cout << "relErr Up for iSyst " << iSyst << " is non-calculable: effCV = 0" << endl;
                        cout << "relErr Down for iSyst " << iSyst << " is non-calculable: effCV = 0" << endl;
                    }
                    else {
                        cout << "relErr Up for iSyst " << iSyst << " is " << currSystErrUp / effCV << endl;
                        cout << "relErr Down for iSyst " << iSyst << " is " << currSystErrDown / effCV << endl;
                    }
                }
                
                vecEff_SystVarUp.push_back(TMath::Max(0.0, effCV + currSystErrUp));
                vecEff_SystVarDown.push_back(TMath::Max(0.0, effCV - currSystErrDown));
            }
        }
    }
    void InitializeVecs() {
        vecSystNames.clear();
        SetSystVec(&vecSystNames);
        unsigned int numSysts = vecSystNames.size();
        vecOutHist_SystVarUp.resize(numSysts);
        vecOutHist_SystVarDown.resize(numSysts);
    }
    
    TString GetMT2String(bool isControl = false) {
        TString outString = "";
        if (doControl || isControl) {
            outString += "_MT2ll";
            outString += cutMT2ll_Control;
            outString += "AND";
            outString += "_MT2lblb";
            outString += cutMT2lblb_Control;
            if (cutMT2bb_ZMET_Control > -1) {
                outString += "AND";
                outString += "_MT2bb_ZMET";
                outString += cutMT2bb_ZMET_Control;
            }
        }
        else {
            outString += "_MT2ll";
            outString += MT2llCut;
            if (MT2lblbCut > -1) {
                outString += whichIntType ? "OR" : "AND";
                outString += "_MT2lblb";
                outString += MT2lblbCut;
            }
            if (MT2bb_ZMETCut > -1) {
                outString += whichIntType ? "OR" : "AND";
                outString += "_MT2bb_ZMET";
                outString += MT2bb_ZMETCut;
            }
        }
        return outString;
    }
    
    TString GetMT2TitleString() {
        TString outString = "M_{T2}(ll)";
        TString jointString, strMT2ll, strMT2lblb, strMT2bb_ZMET;
        TString strAnd = whichIntType ? " or " : " and ";
        TString strGeV = " GeV ";
        if (doControl) {
            jointString = " < ";
            strMT2ll = ""; strMT2ll += cutMT2ll_Control;
            strMT2lblb = ""; strMT2lblb += cutMT2lblb_Control;
            strMT2bb_ZMET = ""; strMT2bb_ZMET += cutMT2bb_ZMET_Control;
        }
        else {
            jointString = " > ";
            strMT2ll = ""; strMT2ll += MT2llCut;
            strMT2lblb = ""; strMT2lblb += MT2lblbCut;
            strMT2bb_ZMET = ""; strMT2bb_ZMET += MT2bb_ZMETCut;
        }
        
        outString += jointString;
        outString += strMT2ll;
        outString += strGeV;
        if (!(doControl && cutMT2lblb_Control == 0)) {
            outString += strAnd;
            outString += "M_{T2}(lb)(lb)";
            outString += jointString;
            outString += strMT2lblb;
            outString += strGeV;
            if (!doControl && cutMT2bb_ZMET_Control <= 0) {
                outString += strAnd;
                outString += "M_{T2}^{#slash{W}}(bb)";
                outString += jointString;
                outString += strMT2lblb;
                outString += strGeV;
            }
        }
        return outString;
    }
    
    void InitializeHistsandOutfile(SUSYT2LimitParams * inSUSYT2LP, bool readFile) {
        
        massStopThresh = 801;
        
        cout << "not going to calculate PDF weights higher than " << massStopThresh << endl;
        
        InitializeVecs();
        
        TString outHistName = "h_CoarseSigEff_";
        if (doControl) {
            outHistName += "Control";
        }
        outHistName += inSUSYT2LP->SavingName(false);
        outHistName += GetMT2String();
        
        int indexForSMSDir = doControl ? -1 : 1;
        
        TString strSMSDir = inSUSYT2LP->SavingName(true) + SetSMSStringAppend(indexForSMSDir);
//        TString baseDirOutFile = "EfficiencyMaps/" + strSMSDir;
        TString baseDirOutFile = strSMSDir;
        
        System_MakeDir(baseDirOutFile);
        
        TString outFileName = baseDirOutFile + outHistName;
        outFileName += ".root";
        
        if (readFile) {
            outFile = TFile::Open(outFileName);
            outHist_CV = (TH2F *) outFile->Get(outHistName);
            
            for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                vecOutHist_SystVarUp[iSyst] = (TH2F *) outFile->Get(outHistName + vecSystNames[iSyst] + TString("Up"));
                vecOutHist_SystVarDown[iSyst] = (TH2F *) outFile->Get(outHistName + vecSystNames[iSyst] + TString("Down"));
            }
        }
        else {
            outFile = new TFile(outFileName, "RECREATE");
            SetHistogram(outHist_CV, inSUSYT2LP->typeT2, outHistName, false, "");
            for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                SetHistogram(vecOutHist_SystVarUp[iSyst], inSUSYT2LP->typeT2, outHistName, false, vecSystNames[iSyst] + TString("Up"));
                SetHistogram(vecOutHist_SystVarDown[iSyst], inSUSYT2LP->typeT2, outHistName, false, vecSystNames[iSyst] + TString("Down"));
            }
        }
    }
    void WriteFile() {
        outFile->Write();
        outFile->Close();
    }
    
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        for (int iCoarseMapParam = 0; iCoarseMapParam < argc; ++iCoarseMapParam) {
            if (strncmp (argv[iCoarseMapParam],"makeXSecMap", 11) == 0) {
                loadXSec = 0;
            }
            else if (strncmp (argv[iCoarseMapParam],"cuts", 4) == 0) {
                MT2llCut = strtol(argv[iCoarseMapParam + 1], NULL, 10);
                MT2lblbCut = strtol(argv[iCoarseMapParam + 2], NULL, 10);
                MT2bb_ZMETCut = strtol(argv[iCoarseMapParam + 3], NULL, 10);
            }
            else if (strncmp (argv[iCoarseMapParam],"doControl", 9) == 0) {
                doControl = 1;
            }
            else if (strncmp (argv[iCoarseMapParam],"controlCuts", 11) == 0) {
                cutMT2ll_Control = strtol(argv[iCoarseMapParam + 1], NULL, 10);
                cutMT2lblb_Control = strtol(argv[iCoarseMapParam + 2], NULL, 10);
                cutMT2bb_ZMET_Control = strtol(argv[iCoarseMapParam + 3], NULL, 10);
            }
        }
    }
};
struct SmoothMapMaker {
    //Structure used to calculate the "smoothed" -- that is, efficiency with Swiss-Cross smoothing applied -- efficiency
    //for each mass point in the 2D SUSY plane to pass a given MT2 cut (whether 1D, 2D, or 3D)
    TFile * inFile;
    TFile * outFile;
    
    TH2F * histCV;
    
    vector<TH2F *> vecHist_SystVarUp;
    vector<TH2F *> vecHist_SystVarDown;
    
    vector<TH2F *> vecInterpHist;
    
    int numRebins;
    TString addName;
    string dirToRW;
    void DefaultVals() {
        dirToRW = "SW";
        numRebins = 3;
        addName = "_Rebin";
        addName += numRebins;
    }
    
    void PrintBasic() {
        cout << "running interpolation " << numRebins << " times in the " << dirToRW << "direction; ";
        cout << "will be saving histograms with the added name " << addName << endl;
        cout << "I will be saving everything to " << outFile->GetName() << endl;
    }
    
    void SetFile(CoarseMapMaker * inCMM) {
        inFile = inCMM->outFile;
        vecHist_SystVarUp.resize(inCMM->vecOutHist_SystVarUp.size());
        vecHist_SystVarDown.resize(inCMM->vecOutHist_SystVarDown.size());
    }
    
    void DoInterpolation(bool doVerb = 0) {
        if (doVerb) {
            cout << "about to run interpolation on " << inFile->GetName() << endl;
        }
        vecInterpHist = InterpolateCoarseMaps(inFile, dirToRW, numRebins, addName, doVerb);
        histCV = (TH2F *) vecInterpHist[0]->Clone();
        for (unsigned int iVec = 1; iVec < vecInterpHist.size(); iVec+=2) {
            vecHist_SystVarUp[(iVec - 1)/2] = (TH2F *) vecInterpHist[iVec]->Clone();
            vecHist_SystVarDown[(iVec - 1)/2] = (TH2F *) vecInterpHist[iVec + 1]->Clone();
        }
    }
    
    void InitializeHistsandOutfile(SUSYT2LimitParams * inSUSYT2LP, CoarseMapMaker * inCMM, bool readFile, bool doVerb = 0) {
        
        TString outFileBaseName = "h_SmoothSigEff_";
        
        if (inCMM->doControl) {
            outFileBaseName += "Control";
        }
        
        outFileBaseName += inSUSYT2LP->SavingName(false);
        outFileBaseName += inCMM->GetMT2String();
        
        int indexForSMSDir = inCMM->doControl ? -2 : 2;
        
        TString strSMSDir = inSUSYT2LP->SavingName(true) + SetSMSStringAppend(indexForSMSDir);
//        TString baseDirOutFile = "EfficiencyMaps/" + strSMSDir;
        TString baseDirOutFile = strSMSDir;
        
        System_MakeDir(baseDirOutFile);
        
        TString outFileName = baseDirOutFile + outFileBaseName;
        outFileName += ".root";
        
        if (readFile) {
            outFileBaseName = outFileBaseName.Replace(outFileBaseName.Index("Smooth"), 6, "Coarse");
            outFile = TFile::Open(outFileName);
            histCV = (TH2F *) outFile->Get(outFileBaseName + addName);
            if (doVerb) {
                TString grabName = outFileBaseName + addName;
                cout << "trying to grab " << grabName << endl;
            }
            vecHist_SystVarUp.resize(inCMM->vecSystNames.size());
            vecHist_SystVarDown.resize(inCMM->vecSystNames.size());
            for (unsigned int iSyst = 0; iSyst < inCMM->vecSystNames.size(); ++iSyst) {
                if (doVerb) {
                    TString grabNameSystUp = outFileBaseName + inCMM->vecSystNames[iSyst] + TString("Up") + addName;
                    cout << "trying to grab " << grabNameSystUp << " from the file " << outFile->GetName() << endl;
                    cout << "size vec " << vecHist_SystVarUp.size() << endl;
                }
                vecHist_SystVarUp[iSyst] = (TH2F *) outFile->Get(outFileBaseName + inCMM->vecSystNames[iSyst] + TString("Up") + addName);
                vecHist_SystVarDown[iSyst] = (TH2F *) outFile->Get(outFileBaseName + inCMM->vecSystNames[iSyst] + TString("Down") + addName);
            }
        }
        else {
            outFile = new TFile(outFileName, "RECREATE");
        }
    }
    void WriteFile() {
        outFile->cd();
        outFile->Write();
        histCV->Write();
        for (unsigned int iSyst = 0; iSyst < vecHist_SystVarUp.size(); ++iSyst) {
            vecHist_SystVarUp[iSyst]->Write();
            vecHist_SystVarDown[iSyst]->Write();
        }
        outFile->Close();
    }
    
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        for (int iSmoothMapParam = 0; iSmoothMapParam < argc; ++iSmoothMapParam) {
            if (strncmp (argv[iSmoothMapParam],"-n", 2) == 0) {
                numRebins = strtol(argv[iSmoothMapParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iSmoothMapParam],"addName", 7) == 0) {
                addName = TString(argv[iSmoothMapParam + 1]);
            }
        }
    }
};

struct SignalYieldMaker {
    //Structure used to calculate the signal yield, using the smoothed efficiency map, for each point in the 2D SUSY plane to pass a given MT2 cut (whether 1D, 2D, or 3D)
    TFile * outFile;
    
    vector<TString> vecSystNames, vecSystFinalNames, vecSystFinalNames_TTBar;
    
    TH2F * inSmoothHist_CV;
    
    vector<TH2F *> vecInSmoothHist_SystVarUp;
    vector<TH2F *> vecInSmoothHist_SystVarDown;
    
    TH2F * outYieldHist_CV;
    vector<TH2F *> vecOutYieldHist_SystVarUp;
    vector<TH2F *> vecOutYieldHist_SystVarDown;
    
    float lumiData;
    bool loadXSec;
    TString xsecFileName, xsecHistName;
    TFile * crossSectionFile;
    TH1F * crossSectionHist;
    
    bool doSigContam;
    TString nameFileTTBarYieldControl, nameFileTTBarYieldSignal;
    float yieldTTBarControl, yieldTTBarSignal;
    float statTTBarControl, statTTBarSignal;
    vector<float> vecYieldTTBarControl_SystVarUp, vecYieldTTBarControl_SystVarDown;
    vector<float> vecYieldTTBarSignal_SystVarUp, vecYieldTTBarSignal_SystVarDown;
    vector<float> vecVals;
    void DefaultVals() {
        lumiData = 19656;
        
        doSigContam = false;
        
        loadXSec = true;        
        
        xsecFileName = "../NonDataRootFiles/SignalXSection/referenceXSecs.root";
        xsecHistName = "stop8TeV_NLONLL";
    }
    
    float FindXSec(int massStop) {
        int xBin = crossSectionHist->FindBin(massStop);
        return crossSectionHist->GetBinContent(xBin);
    }
    float FindXSec(float massStop) {
        int xBin = crossSectionHist->FindBin(massStop);
        return crossSectionHist->GetBinContent(xBin);
    }
    void LoadXSec() {
        crossSectionFile = TFile::Open(xsecFileName);
        crossSectionHist = (TH1F *) crossSectionFile->Get(xsecHistName);
    }
    void InitializeVecs() {
        vecSystNames.clear();
        SetSystVec(&vecSystNames);
        
        
        vecSystFinalNames.clear();
        SetSystVecFinal(&vecSystFinalNames);
        
        vecSystFinalNames_TTBar.clear();
        SetSystVecFinal_TTBar(&vecSystFinalNames_TTBar);
        
        unsigned int numSysts = vecSystNames.size();
        unsigned int numSystsFinal = vecSystFinalNames.size();
        unsigned int numSystsFinal_TTBar = vecSystFinalNames_TTBar.size();
        
        vecInSmoothHist_SystVarUp.resize(numSysts);
        vecInSmoothHist_SystVarDown.resize(numSysts);
        
        vecOutYieldHist_SystVarUp.resize(numSystsFinal);
        vecOutYieldHist_SystVarDown.resize(numSystsFinal);
        
        vecYieldTTBarControl_SystVarUp.resize(numSystsFinal_TTBar);
        vecYieldTTBarControl_SystVarDown.resize(numSystsFinal_TTBar);
        
        vecYieldTTBarSignal_SystVarUp.resize(numSystsFinal_TTBar);
        vecYieldTTBarSignal_SystVarDown.resize(numSystsFinal_TTBar);
    }
    void NormalizeSigYieldByTTBar() {
        outYieldHist_CV->Scale(1./yieldTTBarControl);
        for (unsigned int iSyst = 0; iSyst < vecYieldTTBarControl_SystVarUp.size(); ++iSyst) {
            vecOutYieldHist_SystVarUp[iSyst]->Scale(vecYieldTTBarControl_SystVarUp[iSyst]);
            vecOutYieldHist_SystVarDown[iSyst]->Scale(vecYieldTTBarControl_SystVarDown[iSyst]);
        }
    }
    void SetSigContamYields(TString nameTempFile, bool doVerb = 0) {
        
        System_RemoveFile(nameTempFile);
        
        stringstream cmdGrabTTBarControlYield;
        cmdGrabTTBarControlYield << "cat " << nameFileTTBarYieldControl << " | grep TTBar | grep Yield | grep -v Total | awk '{print $3}' >> " << nameTempFile;
        if (doVerb) {
            cout << cmdGrabTTBarControlYield.str() << endl;
        }
        system(cmdGrabTTBarControlYield.str().c_str());
        
        stringstream cmdGrabTTBarControlStat;
        cmdGrabTTBarControlStat << "cat " << nameFileTTBarYieldControl << " | grep TTBar | grep Stat | grep -v Total | awk '{print $4}' >> " << nameTempFile;
        if (doVerb) {
            cout << cmdGrabTTBarControlStat.str() << endl;
        }
        system(cmdGrabTTBarControlStat.str().c_str());
        
        stringstream cmdGrabTTBarSignalYield;
        cmdGrabTTBarSignalYield << "cat " << nameFileTTBarYieldSignal << " | grep TTBar | grep Yield | grep -v Total | awk '{print $3}' >> " << nameTempFile;
        if (doVerb) {
            cout << cmdGrabTTBarSignalYield.str() << endl;
        }
        system(cmdGrabTTBarSignalYield.str().c_str());
        
        stringstream cmdGrabTTBarSignalStat;
        cmdGrabTTBarSignalStat << "cat " << nameFileTTBarYieldSignal << " | grep TTBar | grep Stat | grep -v Total | awk '{print $4}' >> " << nameTempFile;
        if (doVerb) {
            cout << cmdGrabTTBarSignalStat.str() << endl;
        }
        system(cmdGrabTTBarSignalStat.str().c_str());
        
        for (unsigned int iSyst = 0; iSyst < vecYieldTTBarControl_SystVarUp.size(); ++iSyst) {
            
            TString nameSyst = vecSystFinalNames_TTBar[iSyst];
            nameSyst.Replace(nameSyst.Index("_"), 1, "");
            
            if (doVerb) {
                cout << "iSyst " << iSyst << endl;
                cout << "nameSyst " << nameSyst << endl;
            }
            
            stringstream cmdGrabTTBarControlYield_SystVarUp;
            cmdGrabTTBarControlYield_SystVarUp << "cat " << nameFileTTBarYieldControl << " | grep TTBar | grep Up | grep Shift | grep -v Total | grep " << nameSyst << " | awk '{print $7}' >>" << nameTempFile;
            system(cmdGrabTTBarControlYield_SystVarUp.str().c_str());
            stringstream cmdGrabTTBarControlYield_SystVarDown;
            cmdGrabTTBarControlYield_SystVarDown << "cat " << nameFileTTBarYieldControl << " | grep TTBar | grep Down | grep Shift | grep -v Total | grep " << nameSyst << " | awk '{print $7}' >>" << nameTempFile;
            system(cmdGrabTTBarControlYield_SystVarDown.str().c_str());
            
            if (doVerb) {
                cout << "Control Syst Up " << cmdGrabTTBarControlYield_SystVarUp.str() << endl;
                cout << "Control Syst Down " << cmdGrabTTBarControlYield_SystVarDown.str() << endl;
            }
        }
        
        for (unsigned int iSyst = 0; iSyst < vecYieldTTBarSignal_SystVarUp.size(); ++iSyst) {
            
            TString nameSyst = vecSystFinalNames_TTBar[iSyst];
            nameSyst.Replace(nameSyst.Index("_"), 1, "");
            
            stringstream cmdGrabTTBarSignalYield_SystVarUp;
            cmdGrabTTBarSignalYield_SystVarUp << "cat " << nameFileTTBarYieldSignal << " | grep TTBar | grep Up | grep Shift | grep -v Total | grep " << nameSyst << " | awk '{print $7}' >>" << nameTempFile;
            system(cmdGrabTTBarSignalYield_SystVarUp.str().c_str());
            stringstream cmdGrabTTBarSignalYield_SystVarDown;
            cmdGrabTTBarSignalYield_SystVarDown << "cat " << nameFileTTBarYieldSignal << " | grep TTBar | grep Down | grep Shift | grep -v Total | grep " << nameSyst << " | awk '{print $7}' >>" << nameTempFile;
            system(cmdGrabTTBarSignalYield_SystVarDown.str().c_str());
            
            if (doVerb) {
                cout << "Signal Syst Up " << cmdGrabTTBarSignalYield_SystVarUp.str() << endl;
                cout << "Signal Syst Down " << cmdGrabTTBarSignalYield_SystVarDown.str() << endl;
            }
        }
        
        vecVals.clear();
        vecVals = ReadSigContamFileFloats(nameTempFile);
        
        if (doVerb) {
            for (unsigned int iVecVal = 0; iVecVal < vecVals.size(); ++iVecVal) {
                cout << "iVecVal " << iVecVal << endl;
                cout << "vecVals[iVecVal] " << vecVals[iVecVal] << endl;
            }
        }
        yieldTTBarControl = vecVals[0];
        statTTBarControl = (vecVals[1] - 1) * yieldTTBarControl;
        yieldTTBarSignal = vecVals[2];
        statTTBarSignal = (vecVals[3] - 1) * yieldTTBarSignal;
        
        unsigned int numPreSystVals = 4;
        unsigned int numBkgSystVals = vecYieldTTBarControl_SystVarUp.size();
        unsigned int numSigSystVals = vecYieldTTBarSignal_SystVarUp.size();
        
        for (unsigned int iSyst = 0; iSyst < vecYieldTTBarControl_SystVarUp.size(); ++iSyst) {
            vecYieldTTBarControl_SystVarUp[iSyst] = vecVals[numPreSystVals + (2* iSyst)] * yieldTTBarControl;
            vecYieldTTBarControl_SystVarDown[iSyst] = vecVals[numPreSystVals + (2* iSyst) + 1] * yieldTTBarControl;
        }
        
        for (unsigned int iSyst = 0; iSyst < vecYieldTTBarSignal_SystVarUp.size(); ++iSyst) {
            vecYieldTTBarSignal_SystVarUp[iSyst] = vecVals[numPreSystVals + 2 * numBkgSystVals + (2* iSyst)] * yieldTTBarSignal;
            vecYieldTTBarSignal_SystVarDown[iSyst] = vecVals[numPreSystVals + 2 * numBkgSystVals + (2* iSyst) + 1] * yieldTTBarSignal;
        }
        
        if (doVerb) {
            cout << "yieldTTBarControl " << yieldTTBarControl << endl;
            cout << "statTTBarControl " << statTTBarControl << endl;
            cout << "yieldTTBarSignal " << yieldTTBarSignal << endl;
            cout << "statTTBarSignal " << statTTBarSignal << endl;
            
            for (unsigned int iSyst = 0; iSyst < vecYieldTTBarControl_SystVarUp.size(); ++iSyst) {
                cout << "for Syst " << vecSystFinalNames_TTBar[iSyst] << endl;
                cout << "vecYieldTTBarControl_SystVarUp[iSyst] " << vecYieldTTBarControl_SystVarUp[iSyst] << endl;
                cout << "vecYieldTTBarControl_SystVarDown[iSyst] " << vecYieldTTBarControl_SystVarDown[iSyst] << endl;
                cout << "vecYieldTTBarSignal_SystVarUp[iSyst] " << vecYieldTTBarSignal_SystVarUp[iSyst] << endl;
                cout << "vecYieldTTBarSignal_SystVarDown[iSyst] " << vecYieldTTBarSignal_SystVarDown[iSyst] << endl;
            }
        }
        
        
    }
    
    void PrintYieldCV(float yieldCV, float yieldStatErr, TString printString) {
        char buf[300];
        if (yieldCV > 0) {
            sprintf(buf,"CentVal: %0.4f, StatErr: %0.4f", yieldCV, 1 + (yieldStatErr/yieldCV));
        }
        else {
            sprintf(buf,"CentVal: %0.4f, StatErr: %0.4f", 0.00, 1.00);
        }
        cout << "Name: " << printString << ", " << buf << endl;
    }
    void PrintYieldSyst(float yieldCV, float yieldSystUp, float yieldSystDown, TString printString, TString systString, bool doAverage) {
        char buf[300];
        if (doAverage) {
            if (yieldCV > 0) {
                sprintf(buf,"CentVal: %0.4f, AverageErr: %0.4f", yieldCV, 1 + ((yieldSystUp + yieldSystDown)/(2 * yieldCV)));
            }
            else {
                sprintf(buf,"CentVal: %0.4f, AverageErr: %0.4f", 0.00, 1.00);
            }
        }
        else {
            if (yieldCV > 0) {
                sprintf(buf,"CentVal: %0.4f, UpErr: %0.4f, DownErr: %0.4f", yieldCV, 1 + (yieldSystUp/yieldCV), 1 + (yieldSystDown/yieldCV));
            }
            else {
                sprintf(buf,"CentVal: %0.4f, UpErr: %0.4f, DownErr: %0.4f", 0.00, 1.00, 1.00);
            }
        }
        cout << "Name: " << printString << ", Syst: " << systString << ", " << buf << endl;
    }
    
    void PrintYieldMassPoint(massCombo * inMassCombo, bool doAverage, bool doVerb = 0, SignalYieldMaker * inSigYieldControl = 0) {
        ValError currValErr;
        ValError currBTagValErr;
        
        ValError currValErrControl, currBTagValErrControl;
        
        unsigned int indexStatErr = -1;
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            if (vecSystNames[iSyst].Contains("Stat")) {
                indexStatErr = iSyst;
            }
        }
        
        cout << "running at mass point {MassStop:MassLSP} = {" << inMassCombo->first << ":" << inMassCombo->second << "}" << endl;
        TString basicPrintString = outYieldHist_CV->GetName();
        TString addStringSyst;
        int binX = outYieldHist_CV->GetXaxis()->FindBin(inMassCombo->first);
        int binY = outYieldHist_CV->GetYaxis()->FindBin(inMassCombo->second);
        
        float yieldCV = outYieldHist_CV->GetBinContent(binX, binY);
        float yieldCVUncorr = yieldCV;
        float yieldStatErr = abs(vecOutYieldHist_SystVarUp[indexStatErr]->GetBinContent(binX, binY) - yieldCV);
        
        float yieldCVControl, yieldStatErrControl;
        float yieldSystUpControl, yieldSystDownControl;
        float yieldContam_SigErr_SigContStat;
        float yieldContam_SigErr_TTContStat;
        float yieldContam_SigErr_TTSigStat;
        
        if (doSigContam) {
            yieldCVControl = inSigYieldControl->outYieldHist_CV->GetBinContent(binX, binY);
            yieldStatErrControl = abs(inSigYieldControl->vecOutYieldHist_SystVarUp[indexStatErr]->GetBinContent(binX, binY) - yieldCVControl);
            
            yieldCV = CalculateNewYield(yieldCV, yieldCVControl, yieldTTBarSignal, yieldTTBarControl, doVerb);
            yieldContam_SigErr_SigContStat = CalculateSigContamErr_SigContStat(yieldStatErrControl, yieldTTBarSignal, yieldTTBarControl, doVerb);
            yieldContam_SigErr_TTContStat = CalculateSigContamErr_TTContStat(yieldCVControl, yieldTTBarSignal, statTTBarControl, yieldTTBarControl, doVerb);
            yieldContam_SigErr_TTSigStat = CalculateSigContamErr_TTSigStat(yieldCVControl, statTTBarSignal, yieldTTBarControl, doVerb);
            
            currValErr.SetStatError(yieldCV, yieldContam_SigErr_SigContStat);
            cout << "Signal Stat Error TTBar Control Region Printout ";
            currValErr.PrintOut(basicPrintString, true, "", true, false);
            
            currValErr.SetStatError(yieldCV, yieldContam_SigErr_TTSigStat);
            cout << "TTBar Stat Error Signal Region Printout ";
            currValErr.PrintOut(basicPrintString, true, "", true, false);
            
            currValErr.SetStatError(yieldCV, yieldContam_SigErr_TTContStat);
            cout << "TTBar Stat Error TTBar Control Region Printout ";
            currValErr.PrintOut(basicPrintString, true, "", true, false);
            
            if (doVerb) {
                cout << "basicStat error " << yieldStatErr << endl;
            }
        }
        
        float yieldSystUp, yieldSystDown;
        
        currValErr.SetStatError(yieldCV, yieldStatErr);
        cout << "Basic Stat Error Printout ";
        currValErr.PrintOut(basicPrintString, true, "", true, false);
        //        PrintYieldCV(yieldCV, yieldStatErr, basicPrintString);
        
        //        ValError Syst
        
        int indexOrigSyst;
        const int numBTagSysts = 2;
        //TString nameBTagSysts[numBTagSysts] = {"_BTagEffSF", "_BMisTagSF"};
        
        //cout << "size vecSystFinalNames.size " << vecSystFinalNames.size() << endl;
        if (doVerb) {
            cout << "vecSystFinalNames.size() " << vecSystFinalNames.size() << endl;
            cout << "vecOutYieldHist_SystVarUp size " << vecOutYieldHist_SystVarUp.size() << endl;
            cout << "vecOutYieldHist_SystVarDown size " << vecOutYieldHist_SystVarDown.size() << endl;
        }
        for (unsigned int iSyst = 0; iSyst <= vecSystFinalNames.size(); ++iSyst) {
            if (iSyst < vecSystFinalNames.size() && vecSystFinalNames[iSyst].Contains("Stat")) continue;
            if (doVerb) {
                cout << "on iSyst " << iSyst << endl;
            }
            indexOrigSyst = -1;
            currValErr.DefaultVals();
            currValErr.centVal = yieldCV;
            if (iSyst != vecSystFinalNames.size()) {
                for (unsigned int iSyst2 = 0; iSyst2 < vecSystFinalNames_TTBar.size(); ++iSyst2) {
                    if (vecSystFinalNames_TTBar[iSyst2].Contains(vecSystFinalNames[iSyst])) {
                        indexOrigSyst = iSyst2;
                        //cout << "iSyst " << iSyst << " indexOrigSyst " << indexOrigSyst << endl;
                    }
                }
            }
            if (iSyst == vecSystFinalNames.size()) {
                addStringSyst = "PDF";
            }
            else {
                addStringSyst = vecSystFinalNames[iSyst];
                addStringSyst.Replace(addStringSyst.Index("_"), 1, "");
            }
            /*
             yieldSystUp = abs(vecOutYieldHist_SystVarUp[indexOrigSyst]->GetBinContent(binX, binY));
             yieldSystDown = abs(vecOutYieldHist_SystVarDown[indexOrigSyst]->GetBinContent(binX, binY));
             */
            if (doVerb) {
                cout << "num Out Yield Up hists " << vecOutYieldHist_SystVarUp.size() << endl;
                cout << "num Out Yield Down hists " << vecOutYieldHist_SystVarDown.size() << endl;
                cout << "num Out Control Yield Up hists " << inSigYieldControl->vecOutYieldHist_SystVarUp.size() << endl;
                cout << "num Out Control Yield Down hists " << inSigYieldControl->vecOutYieldHist_SystVarDown.size() << endl;
            }
            yieldSystUp = abs(vecOutYieldHist_SystVarUp[iSyst]->GetBinContent(binX, binY));
            yieldSystDown = abs(vecOutYieldHist_SystVarDown[iSyst]->GetBinContent(binX, binY));
            
            if (doSigContam) {
                /*
                 yieldSystUpControl = abs(inSigYieldControl->vecOutYieldHist_SystVarUp[indexOrigSyst]->GetBinContent(binX, binY));
                 yieldSystDownControl = abs(inSigYieldControl->vecOutYieldHist_SystVarDown[indexOrigSyst]->GetBinContent(binX, binY));
                 */
                yieldSystUpControl = abs(inSigYieldControl->vecOutYieldHist_SystVarUp[iSyst]->GetBinContent(binX, binY));
                yieldSystDownControl = abs(inSigYieldControl->vecOutYieldHist_SystVarDown[iSyst]->GetBinContent(binX, binY));
                float corrTermSystUp, corrTermSystDown;
                if (indexOrigSyst != -1) {
                    corrTermSystUp = (yieldSystUpControl * vecYieldTTBarSignal_SystVarUp[indexOrigSyst] / vecYieldTTBarControl_SystVarUp[indexOrigSyst]);
                    corrTermSystDown = (yieldSystDownControl * vecYieldTTBarSignal_SystVarDown[indexOrigSyst] / vecYieldTTBarControl_SystVarDown[indexOrigSyst]);
                }
                else {
                    corrTermSystUp = (yieldSystUpControl * yieldTTBarSignal / yieldTTBarControl);
                    corrTermSystUp = (yieldSystDownControl * yieldTTBarSignal / yieldTTBarControl);
                }
                if (doVerb) {
                    //cout << "indexOrigSyst " << indexOrigSyst << endl;
                    cout << "iSyst " << iSyst << endl;
                    cout << "yieldSystUpControl " << yieldSystUpControl << endl;
                    cout << "yieldSystDownControl " << yieldSystDownControl << endl;
                    if (indexOrigSyst != -1 ) {
                        cout << "indexOrigSyst " << indexOrigSyst << endl;
                        cout << "vecYieldTTBarSignal_SystVarUp[indexOrigSyst] " << vecYieldTTBarSignal_SystVarUp[indexOrigSyst] << endl;
                        cout << "vecYieldTTBarSignal_SystVarDown[indexOrigSyst] " << vecYieldTTBarSignal_SystVarDown[indexOrigSyst] << endl;
                        
                        cout << "vecYieldTTBarControl_SystVarUp[indexOrigSyst] " << vecYieldTTBarControl_SystVarUp[indexOrigSyst] << endl;
                        cout << "vecYieldTTBarControl_SystVarDown[indexOrigSyst] " << vecYieldTTBarControl_SystVarDown[indexOrigSyst] << endl;
                    }
                    else {
                        cout << "used base TTBar CV yields since not a TTBar systematic " << endl;
                    }
                    cout << "corrTermSystUp " << corrTermSystUp << endl;
                    cout << "corrTermSystDown " << corrTermSystDown << endl;
                    
                    cout << "pre-corr yieldSystUp " << yieldSystUp << endl;
                    cout << "pre-corr yieldSystDown " << yieldSystDown << endl;
                }
                
                yieldSystUp -= corrTermSystUp;
                yieldSystDown -= corrTermSystDown;
                
                if (yieldSystUp < 0) yieldSystUp = 0.0;
                if (yieldSystDown < 0) yieldSystDown = 0.0;
            }
            //cout << "name for up shift " << vecOutYieldHist_SystVarUp[indexOrigSyst]->GetName() << endl;
            //cout << "name for down shift " << vecOutYieldHist_SystVarDown[indexOrigSyst]->GetName() << endl;
            //cout << "up yield " << yieldSystUp << endl;
            //cout << "down yield " << yieldSystDown << endl;
            //cout << "yieldCV " << yieldCV << endl;
            currValErr.SetSystError(yieldCV, yieldSystUp, yieldSystDown, false, false);
            currValErr.PrintOut(basicPrintString, false, addStringSyst, true, doAverage);
        }
    }
    
    void PrintYields(bool doAverage, bool doVerb = 0, SignalYieldMaker * inSigYieldControl = 0) {
        int minDiff = 100;
        int currDiff;
        
        int nBinsX = outYieldHist_CV->GetNbinsX();
        int nBinsY = outYieldHist_CV->GetNbinsY();
        
        int currMassStop, currMassLSP;
        for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
            currMassStop = ModFive(outYieldHist_CV->GetXaxis()->GetBinCenter(iBinX));
            for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
                currMassLSP = outYieldHist_CV->GetYaxis()->GetBinCenter(iBinY);
                if (currMassLSP % 5 == 4) {
                    currMassLSP += 1;
                }
                else if (currMassLSP % 5 == 1) {
                    currMassLSP -= 1;
                }
                currDiff = currMassStop - currMassLSP;
                if (currDiff < minDiff) continue;
                massCombo currCombo(currMassStop, currMassLSP);
                PrintYieldMassPoint(&currCombo, doAverage, doVerb, inSigYieldControl);
            }
        }
    }
    
    void MakeYieldHists(bool doVerb = 0) {
        int nBinsX = outYieldHist_CV->GetNbinsX();
        int nBinsY = outYieldHist_CV->GetNbinsY();
        
        float currMassStop, currMassLSP;
        int currBinStop_Smooth, currBinLSP_Smooth;
        float currSigEff, currSigEff_SystUp, currSigEff_SystDown;
        
        float currXSec;
        float currYield, currYield_SystUp, currYield_SystDown;
        
        int indexCT10 = -1, indexMSTW = -1, indexNNPDF = -1;
        
        for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
            currMassStop = outYieldHist_CV->GetXaxis()->GetBinCenter(iBinX);
            currBinStop_Smooth = inSmoothHist_CV->GetXaxis()->FindBin(currMassStop);
            
            currXSec = FindXSec(currMassStop);
            
            for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
                currMassLSP = outYieldHist_CV->GetYaxis()->GetBinCenter(iBinY);
                currBinLSP_Smooth = inSmoothHist_CV->GetYaxis()->FindBin(currMassLSP);
                
                currSigEff = inSmoothHist_CV->GetBinContent(currBinStop_Smooth, currBinLSP_Smooth);
                currYield = currSigEff * lumiData * currXSec;
                if (doVerb) {
                    cout << "for CV, currSigEff is " << currSigEff << endl;
                    cout << "and yield is lumiData: " << lumiData << " times currXSec: " << currXSec << " times currSigEff to equal " << currYield << endl;
                }
                outYieldHist_CV->SetBinContent(iBinX, iBinY, currYield);
                
                for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                    currSigEff_SystUp = vecInSmoothHist_SystVarUp[iSyst]->GetBinContent(currBinStop_Smooth, currBinLSP_Smooth);
                    currYield_SystUp = currSigEff_SystUp * lumiData * currXSec;
                    if (doVerb) {
                        cout << "for iSyst = " << iSyst << " currSigEff_SystVarUp is " << currSigEff_SystUp << endl;
                        cout << "and yield is lumiData: " << lumiData << " times currXSec: " << currXSec << " times currSigEff_SystUp to equal " << currYield_SystUp << endl;
                    }
                    vecOutYieldHist_SystVarUp[iSyst]->SetBinContent(iBinX, iBinY, currYield_SystUp);
                    
                    currSigEff_SystDown = vecInSmoothHist_SystVarDown[iSyst]->GetBinContent(currBinStop_Smooth, currBinLSP_Smooth);
                    currYield_SystDown = currSigEff_SystDown * lumiData * currXSec;
                    if (doVerb) {
                        cout << "for iSyst = " << iSyst << " currSigEff_SystVarDown is " << currSigEff_SystDown << endl;
                        cout << "and yield is lumiData: " << lumiData << " times currXSec: " << currXSec << " times currSigEff_SystDown to equal " << currYield_SystDown << endl;
                    }
                    vecOutYieldHist_SystVarDown[iSyst]->SetBinContent(iBinX, iBinY, currYield_SystDown);
                }
            }
        }
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            if (vecSystNames[iSyst].Contains("CT10")) {
                indexCT10 = iSyst;
            }
            else if (vecSystNames[iSyst].Contains("MSTW")) {
                indexMSTW = iSyst;
            }
            else if (vecSystNames[iSyst].Contains("NNPDF")) {
                indexNNPDF = iSyst;
            }
        }
        
        vector<TH2F *> vecHists_PDFShiftUp, vecHists_PDFShiftDown;
        vecHists_PDFShiftUp.push_back(vecOutYieldHist_SystVarUp[indexCT10]);
        vecHists_PDFShiftUp.push_back(vecOutYieldHist_SystVarUp[indexMSTW]);
        //vecHists_PDFShiftUp.push_back(vecOutYieldHist_SystVarUp[indexNNPDF]);
        
        vecHists_PDFShiftDown.push_back(vecOutYieldHist_SystVarDown[indexCT10]);
        vecHists_PDFShiftDown.push_back(vecOutYieldHist_SystVarDown[indexMSTW]);
        //vecHists_PDFShiftDown.push_back(vecOutYieldHist_SystVarDown[indexNNPDF]);
        
        if (doVerb) {
            cout << "going to set envelope histograms " << endl;
        }
        SetEnvelopeHistogram(&vecHists_PDFShiftUp, vecOutYieldHist_SystVarUp[vecSystNames.size()], vecOutYieldHist_SystVarUp[vecSystNames.size() + 1], 1, doVerb);
        SetEnvelopeHistogram(&vecHists_PDFShiftDown, vecOutYieldHist_SystVarDown[vecSystNames.size()], vecOutYieldHist_SystVarDown[vecSystNames.size() + 1], -1, doVerb);
    }
    
    
    void ReadSmoothMaps(SmoothMapMaker * inSMM) {
        inSmoothHist_CV = inSMM->histCV;
        for (unsigned int iSyst = 0; iSyst < vecInSmoothHist_SystVarUp.size(); ++iSyst) {
            vecInSmoothHist_SystVarUp[iSyst] = inSMM->vecHist_SystVarUp[iSyst];
            vecInSmoothHist_SystVarDown[iSyst] = inSMM->vecHist_SystVarDown[iSyst];
        }
    }
    
    void InitializeHistsandOutfile(SUSYT2LimitParams * inSUSYT2LP, CoarseMapMaker * inCMM, bool readFile, bool isControl, bool doVerb = 0) {
        
        InitializeVecs();
        TString outHistName = "h_SigYield_";
        
        if (inCMM->doControl && isControl) {
            cout << "what's going on, both shouldn't be turned on!" << endl;
        }
        
        if (inCMM->doControl || isControl) {
            outHistName += "Control";
        }
        
        outHistName += inSUSYT2LP->SavingName(false);
        outHistName += inCMM->GetMT2String(isControl);
        
        int indexForSMSDir = inCMM->doControl || isControl ? -3 : 3;
        
        TString strSMSDir = inSUSYT2LP->SavingName(true) + SetSMSStringAppend(indexForSMSDir);
//        TString baseDirOutFile = "EfficiencyMaps/" + strSMSDir;
        TString baseDirOutFile = strSMSDir;
        
        System_MakeDir(baseDirOutFile);
        
        TString outFileName = baseDirOutFile + outHistName;
        outFileName += ".root";
        
        if (doVerb) {
            cout << "outFileName " << outFileName << endl;
        }
        
        if (readFile) {
            outFile = TFile::Open(outFileName);
            outYieldHist_CV = (TH2F *) outFile->Get(outHistName);
            
            for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                vecOutYieldHist_SystVarUp[iSyst] = (TH2F *) outFile->Get(outHistName + vecSystNames[iSyst] + TString("Up"));
                vecOutYieldHist_SystVarDown[iSyst] = (TH2F *) outFile->Get(outHistName + vecSystNames[iSyst] + TString("Down"));
            }
            
            vecOutYieldHist_SystVarUp.push_back((TH2F *) outFile->Get(outHistName + TString("_PDFUp")));
            vecOutYieldHist_SystVarUp.push_back((TH2F *) outFile->Get(outHistName + TString("_PDFEnvelopeUp")));
            vecOutYieldHist_SystVarDown.push_back((TH2F *) outFile->Get(outHistName + TString("_PDFDown")));
            vecOutYieldHist_SystVarDown.push_back((TH2F *) outFile->Get(outHistName + TString("_PDFEnvelopeDown")));
        }
        else {
            if (doVerb) {
                cout << "making output file " << endl;
            }
            outFile = new TFile(outFileName, "RECREATE");
            if (doVerb) {
                cout << "setting central value histogram " << endl;
            }
            SetHistogram(outYieldHist_CV, inSUSYT2LP->typeT2, outHistName, true, "");
            for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                if (doVerb) {
                    cout << "in iSyst = " << iSyst << endl;
                    cout << "setting systematic histogram shift up " << endl;
                }
                SetHistogram(vecOutYieldHist_SystVarUp[iSyst], inSUSYT2LP->typeT2, outHistName, true, vecSystNames[iSyst] + TString("Up"));
                if (doVerb) {
                    cout << "in iSyst = " << iSyst << endl;
                    cout << "setting systematic histogram shift down " << endl;
                }
                
                SetHistogram(vecOutYieldHist_SystVarDown[iSyst], inSUSYT2LP->typeT2, outHistName, true, vecSystNames[iSyst] + TString("Down"));
            }
            
            TH2F * histPDFEnvelope_ShiftUp;
            TH2F * histPDFEnvelope_ShiftDown;
            
            TH2F * histPDFEnvelopeBookKeep_ShiftUp;
            TH2F * histPDFEnvelopeBookKeep_ShiftDown;
            
            if (doVerb) {
                cout << "setting the envelope " << endl;
            }
            SetHistogram(histPDFEnvelope_ShiftUp, inSUSYT2LP->typeT2, outHistName, true, TString("_PDFUp"));
            SetHistogram(histPDFEnvelope_ShiftDown, inSUSYT2LP->typeT2, outHistName, true, TString("_PDFDown"));
            
            if (doVerb) {
                cout << "setting the envelope book-keeper " << endl;
            }
            SetHistogram(histPDFEnvelopeBookKeep_ShiftUp, inSUSYT2LP->typeT2, outHistName, true, TString("_PDFEnvelopeUp"));
            SetHistogram(histPDFEnvelopeBookKeep_ShiftDown, inSUSYT2LP->typeT2, outHistName, true, TString("_PDFEnvelopeDown"));
            
            if (doVerb) {
                cout << "pushing the PDF envelope hists into the vecSyst " << endl;
            }
            vecOutYieldHist_SystVarUp.push_back(histPDFEnvelope_ShiftUp);
            vecOutYieldHist_SystVarUp.push_back(histPDFEnvelopeBookKeep_ShiftUp);
            
            vecOutYieldHist_SystVarDown.push_back(histPDFEnvelope_ShiftDown);
            vecOutYieldHist_SystVarDown.push_back(histPDFEnvelopeBookKeep_ShiftDown);
        }
    }
    void WriteFile() {
        outFile->Write();
        outFile->Close();
    }
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        for (int iSigYieldParam = 0; iSigYieldParam < argc; ++iSigYieldParam) {
            if (strncmp (argv[iSigYieldParam],"lumi", 4) == 0) {
                lumiData = strtol(argv[iSigYieldParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iSigYieldParam],"doSC", 6) == 0) {
                doSigContam = 1;
            }
        }
    }
};


struct SignalYieldPlotter {
    //struct designed to plot signal efficiency maps or yields
    TCanvas * canvCV;
    TCanvas * tempCanv_SystUp;
    TCanvas * tempCanv_SystDown;
    vector<TCanvas *> vecCanv_SystUp;
    vector<TCanvas *> vecCanv_SystDown;
    
    vector<TString> vecSystNames, vecSystFinalNames;
    
    TString baseCanvName;
    TString canvAddName;
    
    int whichPlotType;
    
    bool doDraw, doAverage, showYields, doRatio, doSigContamRatio;
    
    void SetCanvas(int whichSyst = 0) {
        TString fullCanvName = baseCanvName + canvAddName;
        int canvWidth = 1600;
        int canvHeight = 1200;
        if (whichSyst == 0) {
            canvCV = new TCanvas(fullCanvName, baseCanvName + canvAddName, canvWidth, canvHeight);
        }
        else if (whichSyst > 0) {
            fullCanvName += vecSystNames[abs(whichSyst) - 1] + "Up";
            if (doRatio) fullCanvName += "_RatioCV";
            tempCanv_SystUp = new TCanvas(fullCanvName, fullCanvName, canvWidth, canvHeight);
            /*
             TCanvas c1TempUp(fullCanvName, fullCanvName, canvWidth, canvHeight);
             vecCanv_SystUp[abs(whichSyst)] = &c1TempUp;
             */
        }
        else {
            fullCanvName += vecSystNames[abs(whichSyst) - 1] + "Down";
            if (doRatio) fullCanvName += "_RatioCV";
            tempCanv_SystDown = new TCanvas(fullCanvName, fullCanvName, canvWidth, canvHeight);
            /*
             TCanvas c1TempDown(fullCanvName, fullCanvName, canvWidth, canvHeight);
             vecCanv_SystDown[abs(whichSyst)] = &c1TempDown;
             */
        }
    }
    void SetHist(TH2F * inHist, TString titleString) {
        inHist->GetXaxis()->SetTitle("M_{#tilde{t}} [GeV]");
        inHist->GetYaxis()->SetTitle("M_{#tilde{#chi}_{0}} [GeV]");
        inHist->GetYaxis()->SetTitleOffset(1.0);
        inHist->SetTitle(titleString);
    }
    void DrawHist(LeptFiltSignalAssociator * inLFSC) {
        TString canvTitle = "SF for Lepton-filtered Datasets";
        SetCanvas(0);
        SetHist(inLFSC->outHist, canvTitle);
        inLFSC->outHist->Draw("colz");
    }
    void DrawHist(CoarseMapMaker * inCMM) {
        TString canvTitle = "Coarse Signal Eff. for ";
        TString cutString = inCMM->GetMT2TitleString();
        canvTitle += cutString;
        SetCanvas(0);
        SetHist(inCMM->outHist_CV, canvTitle);
        inCMM->outHist_CV->Draw("colz");
        TH2F * ratioHistSyst;
        float currMin, currMax;
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            SetCanvas(iSyst + 1);
            if (doRatio) {
                ratioHistSyst = SystRatioHist(inCMM->outHist_CV, inCMM->vecOutHist_SystVarUp[iSyst]);
                SetHist(ratioHistSyst, canvTitle);
                FindExtremaBelowDiagonal(ratioHistSyst, currMin, currMax);
                ratioHistSyst->GetZaxis()->SetRangeUser(currMin, currMax);
                ratioHistSyst->Draw("colz");
            }
            else {
                SetHist(inCMM->vecOutHist_SystVarUp[iSyst], canvTitle);
                inCMM->vecOutHist_SystVarUp[iSyst]->Draw("colz");
            }
            tempCanv_SystUp->SaveAs(TString("SigEffPlots/") + tempCanv_SystUp->GetName() + TString(".pdf"));
            
            SetCanvas(-1 * (iSyst + 1));
            if (doRatio) {
                ratioHistSyst = SystRatioHist(inCMM->outHist_CV, inCMM->vecOutHist_SystVarDown[iSyst]);
                SetHist(ratioHistSyst, canvTitle);
                FindExtremaBelowDiagonal(ratioHistSyst, currMin, currMax);
                ratioHistSyst->GetZaxis()->SetRangeUser(currMin, currMax);
                ratioHistSyst->Draw("colz");
            }
            else {
                SetHist(inCMM->vecOutHist_SystVarDown[iSyst], canvTitle);
                inCMM->vecOutHist_SystVarDown[iSyst]->Draw("colz");
            }
            tempCanv_SystDown->SaveAs(TString("SigEffPlots/") + tempCanv_SystDown->GetName() + TString(".pdf"));
        }
    }
    
    void DrawHist(SmoothMapMaker * inSMM, CoarseMapMaker * inCMM) {
        TString canvTitle = "Smoothed Signal Eff. for ";
        TString cutString = inCMM->GetMT2TitleString();
        canvTitle += cutString;
        SetCanvas(0);
        SetHist(inSMM->histCV, canvTitle);
        inSMM->histCV->Draw("colz");
        TH2F * ratioHistSyst;
        float currMin, currMax;
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            SetCanvas(iSyst + 1);
            if (doRatio) {
                ratioHistSyst = SystRatioHist(inSMM->histCV, inSMM->vecHist_SystVarUp[iSyst]);
                SetHist(ratioHistSyst, canvTitle);
                FindExtremaBelowDiagonal(ratioHistSyst, currMin, currMax);
                ratioHistSyst->GetZaxis()->SetRangeUser(currMin, currMax);
                ratioHistSyst->Draw("colz");
            }
            else {
                SetHist(inSMM->vecHist_SystVarUp[iSyst], canvTitle);
                inSMM->vecHist_SystVarUp[iSyst]->Draw("colz");
            }
            tempCanv_SystUp->SaveAs(TString("SigEffPlots/") + tempCanv_SystUp->GetName() + TString(".pdf"));
            
            SetCanvas(-1 * (iSyst + 1));
            if (doRatio) {
                ratioHistSyst = SystRatioHist(inSMM->histCV, inSMM->vecHist_SystVarDown[iSyst]);
                SetHist(ratioHistSyst, canvTitle);
                FindExtremaBelowDiagonal(ratioHistSyst, currMin, currMax);
                ratioHistSyst->GetZaxis()->SetRangeUser(currMin, currMax);
                ratioHistSyst->Draw("colz");
            }
            else {
                SetHist(inSMM->vecHist_SystVarDown[iSyst], canvTitle);
                inSMM->vecHist_SystVarDown[iSyst]->Draw("colz");
            }
            tempCanv_SystDown->SaveAs(TString("SigEffPlots/") + tempCanv_SystDown->GetName() + TString(".pdf"));
        }
    }
    
    void DrawHist(SignalYieldMaker * inSYM, CoarseMapMaker * inCMM, bool doVerb = false) {
        if (inSYM->doSigContam && inCMM->doControl) {
            inSYM->NormalizeSigYieldByTTBar();
        }
        
        TString canvTitle = "Signal Yield for ";
        TString cutString = inCMM->GetMT2TitleString();
        canvTitle += cutString;
        SetCanvas(0);
        SetHist(inSYM->outYieldHist_CV, canvTitle);
        inSYM->outYieldHist_CV->Draw("colz");
        canvCV->SetLogz();
        canvCV->SaveAs(canvCV->GetName() + TString(".pdf"));
        TH2F * ratioHistSyst;
        float currMin, currMax;
        for (unsigned int iSyst = 0; iSyst < vecSystFinalNames.size(); ++iSyst) {
            if (doVerb) {
                cout << "setting Up shift canvas for iSyst = " << iSyst << endl;
            }
            SetCanvas(iSyst + 1);
            if (doVerb) {
                cout << "Drawing Up shift canvas for iSyst = " << iSyst << endl;
            }
            if (doRatio) {
                ratioHistSyst = SystRatioHist(inSYM->outYieldHist_CV, inSYM->vecOutYieldHist_SystVarUp[iSyst]);
                SetHist(ratioHistSyst, canvTitle);
                FindExtremaBelowDiagonal(ratioHistSyst, currMin, currMax);
                ratioHistSyst->GetZaxis()->SetRangeUser(currMin, currMax);
                ratioHistSyst->GetXaxis()->SetRangeUser(100, 800);
                ratioHistSyst->GetZaxis()->SetRangeUser(0.99, 1.15);
                ratioHistSyst->Draw("colz");
            }
            else {
                SetHist(inSYM->vecOutYieldHist_SystVarUp[iSyst], canvTitle);
                inSYM->vecOutYieldHist_SystVarUp[iSyst]->Draw("colz");
            }
            tempCanv_SystUp->SaveAs(TString("SigEffPlots/") + tempCanv_SystUp->GetName() + TString(".pdf"));
            
            /*
             if (doVerb) {
             cout << "vecCanv_SystUp size " << vecCanv_SystUp.size() << endl;
             cout << "vecCanv_SystUp[iSyst] " << vecCanv_SystUp[iSyst] << endl;
             }
             vecCanv_SystUp[iSyst]->SaveAs(TString("SigEffPlots/") + vecCanv_SystUp[iSyst]->GetName() + TString(".pdf"));
             */
            
            if (doVerb) {
                cout << "setting Down shift canvas for iSyst = " << iSyst << endl;
            }
            SetCanvas(-1 * (iSyst + 1));
            if (doVerb) {
                cout << "Drawing Down shift canvas for iSyst = " << iSyst << endl;
            }
            if (doRatio) {
                ratioHistSyst = SystRatioHist(inSYM->outYieldHist_CV, inSYM->vecOutYieldHist_SystVarDown[iSyst]);
                SetHist(ratioHistSyst, canvTitle);
                FindExtremaBelowDiagonal(ratioHistSyst, currMin, currMax);
                ratioHistSyst->GetZaxis()->SetRangeUser(currMin, currMax);
                ratioHistSyst->GetXaxis()->SetRangeUser(100, 800);
                ratioHistSyst->GetZaxis()->SetRangeUser(0.85, 1.01);
                ratioHistSyst->Draw("colz");
            }
            else {
                SetHist(inSYM->vecOutYieldHist_SystVarDown[iSyst], canvTitle);
                inSYM->vecOutYieldHist_SystVarDown[iSyst]->Draw("colz");
            }
            tempCanv_SystDown->SaveAs(TString("SigEffPlots/") + tempCanv_SystDown->GetName() + TString(".pdf"));
            /*
             vecCanv_SystDown[iSyst]->SaveAs(TString("SigEffPlots/") + vecCanv_SystDown[iSyst]->GetName() + TString(".pdf"));
             */
        }
        
        TString canvPDFName[2] = {"_PDFEnvelope", "_PDFEnvelopeBookKeep"};
        TString fullCanvName;
        TCanvas * c1;
        int canvWidth = 1600;
        int canvHeight = 1200;
        for (unsigned int iPDF = 1; iPDF < 3; ++iPDF) {
            if (doVerb) {
                cout << "Setting Up shift canvas for iPDF = " << iPDF << endl;
            }
            fullCanvName = baseCanvName + canvAddName;
            fullCanvName += canvPDFName[iPDF - 1];
            fullCanvName += "Up";
            if (doRatio && iPDF == 1) fullCanvName += "_RatioCV";
            c1 = new TCanvas(fullCanvName, fullCanvName, canvWidth, canvHeight);
            if (doVerb) {
                cout << "Drawing Up shift canvas for iPDF = " << iPDF << endl;
            }
            if (doRatio && iPDF == 1) {
                ratioHistSyst = SystRatioHist(inSYM->outYieldHist_CV, inSYM->vecOutYieldHist_SystVarUp[vecSystFinalNames.size() - 1 + iPDF]);
                SetHist(ratioHistSyst, canvTitle);
                FindExtremaBelowDiagonal(ratioHistSyst, currMin, currMax);
                ratioHistSyst->GetZaxis()->SetRangeUser(currMin, currMax);
                ratioHistSyst->GetXaxis()->SetRangeUser(100, 800);
                ratioHistSyst->GetZaxis()->SetRangeUser(0.99, 1.15);
                ratioHistSyst->Draw("colz");
            }
            else {
                SetHist(inSYM->vecOutYieldHist_SystVarUp[vecSystFinalNames.size() - 1 + iPDF], canvTitle);
                inSYM->vecOutYieldHist_SystVarUp[vecSystFinalNames.size() - 1 + iPDF]->Draw("colz");
            }
            c1->SaveAs(TString("SigEffPlots/") + fullCanvName + TString(".pdf"));
            
            if (doVerb) {
                cout << "Setting Down shift canvas for iPDF = " << iPDF << endl;
            }
            fullCanvName = baseCanvName + canvAddName;
            fullCanvName += canvPDFName[iPDF - 1];
            fullCanvName += "Down";
            if (doRatio && iPDF == 1) fullCanvName += "_RatioCV";
            c1 = new TCanvas(fullCanvName, fullCanvName, canvWidth, canvHeight);
            if (doVerb) {
                cout << "Drawing Down shift canvas for iPDF = " << iPDF << endl;
            }
            if (doRatio && iPDF == 1) {
                ratioHistSyst = SystRatioHist(inSYM->outYieldHist_CV, inSYM->vecOutYieldHist_SystVarDown[vecSystFinalNames.size() - 1 + iPDF]);
                SetHist(ratioHistSyst, canvTitle);
                FindExtremaBelowDiagonal(ratioHistSyst, currMin, currMax);
                ratioHistSyst->GetZaxis()->SetRangeUser(currMin, currMax);
                ratioHistSyst->GetXaxis()->SetRangeUser(100, 800);
                ratioHistSyst->GetZaxis()->SetRangeUser(0.85, 1.01);
                ratioHistSyst->Draw("colz");
            }
            else {
                SetHist(inSYM->vecOutYieldHist_SystVarDown[vecSystFinalNames.size() - 1 + iPDF], canvTitle);
                inSYM->vecOutYieldHist_SystVarDown[vecSystFinalNames.size() - 1 + iPDF]->Draw("colz");
            }
            c1->SaveAs(TString("SigEffPlots/") + fullCanvName + TString(".pdf"));
        }
    }
    
    void DefaultVals(int inWPT) {
        doDraw = true;
        showYields = true;
        doAverage = false;
        doRatio = true;
        whichPlotType = inWPT;
        
        TString baseBaseCanvName = "c_";
        
        const int numTypes = 4;
        TString baseCanvNameArr[numTypes] = {"LeptonFilterSF", "CoarseSigEff", "SmoothSigEff", "SigYield"};
        baseCanvName = baseBaseCanvName;
        baseCanvName += baseCanvNameArr[whichPlotType];
    }
    void SetCanvasAddName(CoarseMapMaker * inCMM, SUSYT2LimitParams * inSUSYT2LP) {
        canvAddName = inCMM->GetMT2String();
        canvAddName += inSUSYT2LP->SavingName(false);
    }
    void InitializeVecs() {
        vecSystNames.clear();
        SetSystVec(&vecSystNames);
        
        vecSystFinalNames.clear();
        SetSystVecFinal(&vecSystFinalNames);
        
        unsigned int numSysts = vecSystFinalNames.size();
        
        vecCanv_SystUp.resize(numSysts);
        vecCanv_SystDown.resize(numSysts);
    }
    
    void Draw(LeptFiltSignalAssociator * inLFSC, CoarseMapMaker * inCMM, SmoothMapMaker * inSMM, SignalYieldMaker * inSYM, SUSYT2LimitParams * inSUSYT2LP, bool doVerb = false) {
        if (whichPlotType == 0) {
            DrawHist(inLFSC);
        }
        else {
            SetCanvasAddName(inCMM, inSUSYT2LP);
            switch (whichPlotType) {
                case 1:
                    DrawHist(inCMM);
                    break;
                case 2:
                    DrawHist(inSMM, inCMM);
                    break;
                case 3:
                    DrawHist(inSYM, inCMM, doVerb);
                    break;
                default:
                    break;
            }
        }
    }
    
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        for (int iSignalEffDrawParam = 0; iSignalEffDrawParam < argc; ++iSignalEffDrawParam) {
            if (strncmp (argv[iSignalEffDrawParam],"noDraw", 6) == 0) {
                doDraw = 0;
            }
        }
    }
    
};

struct SignalYieldMaker_IndivPoint {
    //Struct that prints out all the relevant info, including central value
    //as well as systematics for each mass point in the SUSY 2D mass plane
    //Also calculates signal contamination for these points
    
    vector<TString> vecSystNames; //vector that contains names for input systs and names for final systs;
    
    //The current histogram being read in -- this is a 2D histogram in the mass plane
    TH2F * currInHist;
    //The current systematic variation histograms being read in -- these are also 2D histograms in the mass plane
    vector<TH2F *> vecCurrInHist_SystVarUp;
    vector<TH2F *> vecCurrInHist_SystVarDown;
    
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
    
    TH1 * histSig_SigStatControlUp, * histSig_SigStatControlDown;
    TH1 * histSig_TTBarStatContUp, * histSig_TTBarStatContDown;
    TH1 * histSig_TTBarMCStatUp, * histSig_TTBarMCStatDown;
    
    //vector to contain the histogram axes
    vector< vector<Double_t> > vecAxes;
    vector<int> vecNumBinsAxes;
    // vector to specify which axis is which MT2
    vector<int> vecWhichMT2;
    vector<TString> vecMT2Name;
    TString axisString;
    
    
    //Variables used for file load and out
    
    TFile * outFile; // file that will be made containing the individual file
    TString inBaseDir, outBaseDir;
    
    TString massStop, massLSP;
    TString outFileName, outHistBaseName;
    
    TString baseNameInFile;
    
    // I have a function to set the axes and a function to initialize very basic variables
    
    // I need a function to initialize the input directory and output directorys based on input stop mass
    // and also book the output file
    // I need a function to read in an individual MT2Cut Signal Yield histogram and fill in the appropriate bin entry in my output histogram
    // I need a function to iterate over the MT2 cuts and call the above histogram
    // I need a function that calls the above function for central value and systematic variations
    // A function that runs the whole damn chain
    
    void DoSignalContamination_SpecHist(TH1 * histSignal, TH1 * histTTBar, int numDims, int specSigContam = 0, bool doVerb = false) {
        //Function for changing the histogram histSignal to include signal contamination in the ttbar contribution -- this changes the
        //total hypothesis S+B by changing the S
        //specSigContam = 0 is basic one where you calculate the signal contamination using central value
        //specSigContam = 1 is sig contam accounting for the signal MC statistics error in contorl region
        //specSigContam = 2 is sig contam accounting for the TTBar MC statistics error in control region
        //specSigContam = 3 is sig contam accounting for the TTBar MC statistics error in signal region
        float yieldControl_TTBar = histTTBar->GetBinContent(1, 1, 1);
        float errControl_TTBar = histTTBar->GetBinError(1, 1, 1);
        
        float yieldControl_Signal = histSignal->GetBinContent(1, 1, 1);
        float errControl_Signal = histSignal->GetBinError(1, 1, 1);
        
        float SF = yieldControl_Signal / yieldControl_TTBar;
        float SF_Err_TTCont = SF * (errControl_TTBar / yieldControl_TTBar);
        float SF_Err_SigCont = SF * errControl_Signal / yieldControl_TTBar;
        float SF_Err_TTSig = 0.0;
        
        if (doVerb) {
            cout << "yieldControl_TTBar " << yieldControl_TTBar << endl;
            cout << "errControl_TTBar " << errControl_TTBar << endl;
            cout << "yieldControl_Signal " << yieldControl_Signal << endl;
            cout << "errControl_Signal " << errControl_Signal << endl;
            cout << "for histSignal " << histSignal->GetName() << endl;
            cout << "for histTTBar " << histTTBar->GetName() << endl;
            cout << "SF " << SF << endl;
            if (specSigContam != 0) {
                cout << "SF_Err_TTCont " << SF_Err_TTCont << endl;
                cout << "SF_Err_SigCont " << SF_Err_SigCont << endl;
            }
        }
        
        int nBinsX = histSignal->GetNbinsX();
        int nBinsY = histSignal->GetNbinsY();
        int nBinsZ = histSignal->GetNbinsZ();
        
        int startIndexX = 0;
        int endIndexX = nBinsX + 1;
        
        int startIndexY = numDims > 1 ? 0 : 1;
        int endIndexY = numDims > 1 ? nBinsY + 1 : 1;
        
        int startIndexZ = numDims > 2 ? 0 : 1;
        int endIndexZ = numDims > 2 ? nBinsZ + 1 : 1;
        
        float currBinSig, currBinTTBar;
        float newBinSig;
        float currBinErrSig, currBinErrTTBar;
        
        float currCorrFactor, currCorrFactorErr;
        for (int iBinX = startIndexX; iBinX <= endIndexX; ++iBinX) {
            for (int iBinY = startIndexY; iBinY <= endIndexY; ++iBinY) {
                for (int iBinZ = startIndexZ; iBinZ <= endIndexZ; ++iBinZ) {
                    currBinSig = histSignal->GetBinContent(iBinX, iBinY, iBinZ);
                    currBinErrSig = histSignal->GetBinError(iBinX, iBinY, iBinZ);
                    
                    currBinTTBar = histTTBar->GetBinContent(iBinX, iBinY, iBinZ);
                    currBinErrTTBar = histTTBar->GetBinError(iBinX, iBinY, iBinZ);
                    
                    currCorrFactor = SF * currBinTTBar;
                    if (doVerb) {
                        cout << "{iX,iY,iZ} = {" << iBinX << ":" << iBinY << ":" << iBinZ << "}" << endl;
                        cout << "currBinSig " << currBinSig << endl;
                        cout << "currBinTTBar " << currBinTTBar << endl;
                        cout << "currCorrFactor " << currCorrFactor << endl;
                    }
                    if (specSigContam == 0) {
                        currCorrFactorErr = 0.0;
                    }
                    else if (abs(specSigContam) == 1) {
                        currCorrFactorErr = currBinTTBar * SF_Err_SigCont;
                    }
                    else if (abs(specSigContam) == 2) {
                        currCorrFactorErr = currBinTTBar * SF_Err_TTCont;
                    }
                    else if (abs(specSigContam) == 3) {
                        SF_Err_TTSig = SF * currBinErrTTBar / yieldControl_TTBar;
                        currCorrFactorErr = currBinTTBar * SF_Err_TTSig;
                        if (doVerb) {
                            cout << "SF_Err_TTSig " << SF_Err_TTSig << endl;
                        }
                    }
                    if (doVerb && specSigContam != 0) {
                        cout << "currCorrFactorErr " << currCorrFactorErr << endl;
                    }
                    currCorrFactor += TMath::Sign(currCorrFactorErr, (float) specSigContam);
                    newBinSig = currBinSig - currCorrFactor;
                    if (newBinSig < 0) newBinSig = 0;
                    histSignal->SetBinContent(iBinX, iBinY, iBinZ, newBinSig);
                    if (currBinSig > 0) {
                        histSignal->SetBinError(iBinX, iBinY, iBinZ, currBinErrSig * newBinSig/currBinSig);
                    }
                    if (doVerb) {
                        cout << "currBinSig " << currBinSig << endl;
                        cout << "newBinSig " << newBinSig << endl;
                    }
                }
            }
        }
    }
    void DoSignalContamination(TFile * inputFileTTBarShapes, bool doVerb = false) {
        //1) Implements the signal contamination from the MT2 control region and the TTBar values therein
        //2) Adds in three statistical systematic uncertainties
        //2a) TTBar Stat. Uncertainty in the TTBar Control Region
        //2b) TTBar Stat. Uncertainty in each Signal bin
        //2c) Signal Stat. Uncertainty in the TTBar Control Region
        
        TString nameTTBarHist = "TTBar";
        
        TH1 * histToUseSig_CV;
        TH1 * histToUseTTBar_CV = (TH1 *) inputFileTTBarShapes->Get(nameTTBarHist);
        
        TH1 * histToUseSig_SystVarUp, * histToUseSig_SystVarDown;
        TH1 * histToUseTTBar_SystVarUp, * histToUseTTBar_SystVarDown;
        
        TString nameSigSyst_SigStatControl = "_SigStatControl";
        TString nameSigSyst_TTBarStatCont = "_TTBarStatCont";
        TString nameSigSyst_TTBarMCStat = "_TTBarMCStat";
        
        //Do CV
        if (numDims == 1) {
            histToUseSig_CV = outMT2Hist_CV_1D;
        }
        else if (numDims == 2) {
            histToUseSig_CV = outMT2Hist_CV_2D;
        }
        else if (numDims == 3) {
            histToUseSig_CV = outMT2Hist_CV_3D;
        }
        
        //Clone the CV hists into versions to make sig contamination stat. err shifts
        histSig_SigStatControlUp = (TH1 *) histToUseSig_CV->Clone(histToUseSig_CV->GetName() + nameSigSyst_SigStatControl + TString("Up"));
        histSig_SigStatControlDown = (TH1 *) histToUseSig_CV->Clone(histToUseSig_CV->GetName() + nameSigSyst_SigStatControl + TString("Down"));
        
        histSig_TTBarStatContUp = (TH1 *) histToUseSig_CV->Clone(histToUseSig_CV->GetName() + nameSigSyst_TTBarStatCont + TString("Up"));
        histSig_TTBarStatContDown = (TH1 *) histToUseSig_CV->Clone(histToUseSig_CV->GetName() + nameSigSyst_TTBarStatCont + TString("Down"));
        
        histSig_TTBarMCStatUp = (TH1 *) histToUseSig_CV->Clone(histToUseSig_CV->GetName() + nameSigSyst_TTBarMCStat + TString("Up"));
        histSig_TTBarMCStatDown = (TH1 *) histToUseSig_CV->Clone(histToUseSig_CV->GetName() + nameSigSyst_TTBarMCStat + TString("Down"));
        
        //Signal contamination for central value
        DoSignalContamination_SpecHist(histToUseSig_CV, histToUseTTBar_CV, numDims, 0, doVerb);
        
        //Signal contamination for central value
        
        //specSigContam = 1 is sig contam accounting for the signal MC statistics error in contorl region
        //specSigContam = 2 is sig contam accounting for the TTBar MC statistics error in control region
        //specSigContam = 3 is sig contam accounting for the TTBar MC statistics error in signal region
        DoSignalContamination_SpecHist(histSig_SigStatControlUp, histToUseTTBar_CV, numDims, 1, doVerb);
        DoSignalContamination_SpecHist(histSig_SigStatControlDown, histToUseTTBar_CV, numDims, -1, doVerb);
        
        DoSignalContamination_SpecHist(histSig_TTBarStatContUp, histToUseTTBar_CV, numDims, 2, doVerb);
        DoSignalContamination_SpecHist(histSig_TTBarStatContDown, histToUseTTBar_CV, numDims, -2, doVerb);
        
        DoSignalContamination_SpecHist(histSig_TTBarMCStatUp, histToUseTTBar_CV, numDims, 3, doVerb);
        DoSignalContamination_SpecHist(histSig_TTBarMCStatDown, histToUseTTBar_CV, numDims, -3, doVerb);
        
        //write the control region stat. syst. histograms
        /*
         outFile->cd();
         histSig_SigStatControlUp->Write();
         histSig_SigStatControlDown->Write();
         histSig_TTBarStatContUp->Write();
         histSig_TTBarStatContDown->Write();
         histSig_TTBarMCStatUp->Write();
         histSig_TTBarMCStatDown->Write();
         */
        
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            //Set Syst Var hists for Signal
            if (numDims == 1) {
                histToUseSig_SystVarUp = vecOutMT2Hist_1D_SystVarUp[iSyst];
                histToUseSig_SystVarDown = vecOutMT2Hist_1D_SystVarDown[iSyst];
            }
            else if (numDims == 2) {
                histToUseSig_SystVarUp = vecOutMT2Hist_2D_SystVarUp[iSyst];
                histToUseSig_SystVarDown = vecOutMT2Hist_2D_SystVarDown[iSyst];
            }
            else if (numDims == 3) {
                histToUseSig_SystVarUp = vecOutMT2Hist_3D_SystVarUp[iSyst];
                histToUseSig_SystVarDown = vecOutMT2Hist_3D_SystVarDown[iSyst];
            }
            histToUseTTBar_SystVarUp = (TH1 *) inputFileTTBarShapes->Get(nameTTBarHist + vecSystNames[iSyst] + TString("Up"));
            histToUseTTBar_SystVarDown = (TH1 *) inputFileTTBarShapes->Get(nameTTBarHist + vecSystNames[iSyst] + TString("Down"));
            
            if (!histToUseTTBar_SystVarUp) {
                if (doVerb) {
                    cout << "for syst " << vecSystNames[iSyst] << " no corresponding TTBar syst up variation " << endl;
                }
                histToUseTTBar_SystVarUp = histToUseTTBar_CV;
            }
            if (!histToUseTTBar_SystVarDown) {
                if (doVerb) {
                    cout << "for syst " << vecSystNames[iSyst] << " no corresponding TTBar syst down variation " << endl;
                }
                histToUseTTBar_SystVarDown = histToUseTTBar_CV;
            }
            DoSignalContamination_SpecHist(histToUseSig_SystVarUp, histToUseTTBar_SystVarUp, numDims, 0, doVerb);
            DoSignalContamination_SpecHist(histToUseSig_SystVarDown, histToUseTTBar_SystVarDown, numDims, 0, doVerb);
        }
    }
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
    void DefaultVals(int inNumDims = 2) {
        numDims = inNumDims;
        
        //initialize the input directories to the correct area
        /*
        inBaseDir = "EfficiencyMaps/T2tt/PerPol0/SignalYields/SignalRegion/";
        outBaseDir = "EfficiencyMaps/T2tt/PerPol0/";
        */
        inBaseDir = "T2tt/PerPol0/SignalYields/SignalRegion/";
        outBaseDir = "T2tt/PerPol0/";

        
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
    }
    void SetStringSignal(SUSYT2LimitParams * inSUSYT2LP, SUSYMassLimitParams * inSUSYMLP, bool doSigContam = 0) {
//        inBaseDir = "EfficiencyMaps/" + inSUSYT2LP->SavingName(true) + TString("/SignalYields/SignalRegion/");
//        outBaseDir = "EfficiencyMaps/" + inSUSYT2LP->SavingName(true) + TString("/SignalYieldMT2Histograms/") + inSUSYMLP->SavingName(true);
        inBaseDir = inSUSYT2LP->SavingName(true) + TString("/SignalYields/SignalRegion/");
        outBaseDir = inSUSYT2LP->SavingName(true) + TString("/SignalYieldMT2Histograms/") + inSUSYMLP->SavingName(true);

        
        System_MakeDir(outBaseDir);
        
        outFileName = "h_" + inSUSYMLP->SavingName(false) + inSUSYT2LP->SavingName(false);
        outFileName += "_" + vecMT2Name[0];
        if (vecMT2Name.size() > 1) {
            outFileName += "_vs_" + vecMT2Name[1];
            if (vecMT2Name.size() > 2) {
                outFileName += "_vs_" + vecMT2Name[2];
            }
        }
        outHistBaseName = outFileName;
        outFileName += doSigContam ? "_WithSigContam" : "_NoSigContam";
        outFileName += ".root";
        
        baseNameInFile = "h_SigYield_" + inSUSYT2LP->SavingName(false);
        baseNameInFile += TString("_") + vecMT2Name[0] + TString("FPATSYAND");
        if (vecMT2Name.size() > 1) {
            baseNameInFile += "_" + vecMT2Name[1] + "SPATSY";
            if (vecMT2Name.size() > 2) {
                baseNameInFile += "AND_" + vecMT2Name[2] + "TPATSY";
            }
        }
        else {
            baseNameInFile += "_MT2lblb0";
        }
    }
    void InitializeVecs() {
        vecSystNames.clear();
        SetSystVec(&vecSystNames);
        vecSystNames.push_back("_PDF");
        unsigned int numSysts = vecSystNames.size();
        
        vecCurrInHist_SystVarUp.resize(numSysts);
        vecCurrInHist_SystVarDown.resize(numSysts);
        
        vecOutMT2Hist_1D_SystVarUp.resize(numSysts);
        vecOutMT2Hist_1D_SystVarDown.resize(numSysts);
        vecOutMT2Hist_2D_SystVarUp.resize(numSysts);
        vecOutMT2Hist_2D_SystVarDown.resize(numSysts);
        vecOutMT2Hist_3D_SystVarUp.resize(numSysts);
        vecOutMT2Hist_3D_SystVarDown.resize(numSysts);
        
    }
    
    void FixMCStats(bool doVerb = 0) {
        TH1 * histCV, * histMCStatUp, * histMCStatDown;
        int indexMCStat = -1;
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            if (vecSystNames[iSyst].Contains("MCStat")) {
                indexMCStat = iSyst;
            }
        }
        if (indexMCStat == -1) {
            cout << "failed to find MCStat index " << endl;
        }
        if (numDims == 1) {
            histCV = outMT2Hist_CV_1D;
            histMCStatUp = vecOutMT2Hist_1D_SystVarUp[indexMCStat];
            histMCStatDown = vecOutMT2Hist_1D_SystVarDown[indexMCStat];
        }
        else if (numDims == 2) {
            histCV = outMT2Hist_CV_2D;
            histMCStatUp = vecOutMT2Hist_2D_SystVarUp[indexMCStat];
            histMCStatDown = vecOutMT2Hist_2D_SystVarDown[indexMCStat];
        }
        else if (numDims == 3) {
            histCV = outMT2Hist_CV_3D;
            histMCStatUp = vecOutMT2Hist_3D_SystVarUp[indexMCStat];
            histMCStatDown = vecOutMT2Hist_3D_SystVarDown[indexMCStat];
        }
        
        if (doVerb) {
            cout << "about to fix the stat error for CV histogram using MCStatUp " << endl;
        }
        SetHistStatErrFromMCStatSyst(histCV, histMCStatUp, doVerb);
        if (doVerb) {
            cout << "about to fix the bin content for MCStatDown using CV histogram error " << endl;
        }
        FixMCStatSystDownHistFromCVStatErr(histCV, histMCStatDown, doVerb);
    }
    void SetYieldInputHist(float intForCut, vector<int> * inVecBins_OutHist, TH1 * histToSet, bool doVerb = 0) {
        int nBinsX = histToSet->GetNbinsX();
        int nBinsY = histToSet->GetNbinsY();
        int nBinsZ = histToSet->GetNbinsZ();
        
        float totIntegral = 0.0;
        
        TString nameClass = histToSet->ClassName();
        if (nameClass.Contains("TH2")) {
            totIntegral = ((TH2 *) histToSet)->Integral(inVecBins_OutHist->at(0), nBinsX + 1, inVecBins_OutHist->at(1), nBinsY + 1);
        }
        else if (nameClass.Contains("TH3")) {
            totIntegral = ((TH3 *) histToSet)->Integral(inVecBins_OutHist->at(0), nBinsX + 1, inVecBins_OutHist->at(1), nBinsY + 1, inVecBins_OutHist->at(2), nBinsZ + 1);
        }
        else {
            totIntegral = histToSet->Integral(inVecBins_OutHist->at(0), nBinsX + 1);
        }
        
        if (doVerb) {
            cout << "going to be grabbing the total integral by integrating from bin " << inVecBins_OutHist->at(0) << " to " << nBinsX + 1 << " in the x direction " << endl;
            if (nameClass.Contains("TH2")) {
                cout << "as well as grabbing the total integral by integrating from bin " << inVecBins_OutHist->at(1) << " to " << nBinsY + 1 << " in the y direction " << endl;
            }
            else if (nameClass.Contains("TH3")) {
                cout << "as well as grabbing the total integral by integrating from bin " << inVecBins_OutHist->at(1) << " to " << nBinsY + 1 << " in the y direction " << endl;
                cout << "as well as grabbing the total integral by integrating from bin " << inVecBins_OutHist->at(2) << " to " << nBinsZ + 1 << " in the z direction " << endl;
            }
        }
        
        float valToInput = intForCut - totIntegral;
        if (doVerb) {
            cout << "totIntegral " << totIntegral << endl;
            cout << "intForCut " << intForCut << endl;
            cout << "valToInput " << valToInput << endl;
        }
        if (doVerb) {
            cout << "going to set the bin content at binX = " << inVecBins_OutHist->at(0) << endl;
            cout << "going to set the bin content at binY = " << inVecBins_OutHist->at(1) << endl;
            cout << "going to set the bin content at binZ = " << inVecBins_OutHist->at(2) << endl;
        }
        histToSet->SetBinContent(inVecBins_OutHist->at(0), inVecBins_OutHist->at(1), inVecBins_OutHist->at(2), valToInput);
    }
    void SetYieldInputFile(massCombo * inMassPoint, TFile * inCurrFile, TString currBaseHistGrabName, vector<int> * inVecBins_OutHist, bool doVerb = 0) {
        int binX_InHist, binY_InHist;
        int numSysts = vecSystNames.size();
        
        //Current yield for histogram passing cuts
        float currCVIntegral;
        vector<float> currIntegral_SystVarUp;
        currIntegral_SystVarUp.resize(numSysts);
        vector<float> currIntegral_SystVarDown;
        currIntegral_SystVarDown.resize(numSysts);
        
        currInHist = (TH2F *) inCurrFile->Get(currBaseHistGrabName);
        if (doVerb) {
            cout << "in " << inCurrFile->GetName() << endl;
            cout << "currBaseHistGrabName " << currBaseHistGrabName << endl;
        }
        binX_InHist = currInHist->GetXaxis()->FindBin(inMassPoint->first);
        binY_InHist = currInHist->GetYaxis()->FindBin(inMassPoint->second);
        
        
        TH1 * histToSet = 0;
        
        //Grab CV Integral and plug into CV Hist
        if (doVerb) {
            cout << "currInHist name " << currInHist->GetName() << endl;
        }
        currCVIntegral = currInHist->GetBinContent(binX_InHist, binY_InHist);
        if (numDims == 1) {
            histToSet = outMT2Hist_CV_1D;
        }
        else if (numDims == 2) {
            histToSet = outMT2Hist_CV_2D;
        }
        else {
            histToSet = outMT2Hist_CV_3D;
        }
        SetYieldInputHist(currCVIntegral, inVecBins_OutHist, histToSet, doVerb);
        if (doVerb) {
            cout << "currCVIntegral " << currCVIntegral << endl;
        }
        for (unsigned int iSyst = 0; iSyst < currIntegral_SystVarUp.size(); ++iSyst) {
            /*
             if (doVerb) {
             cout << "iSyst " << iSyst << endl;
             cout << "going to try to grab " << currBaseHistGrabName + vecSystNames[iSyst] + TString("Up") << endl;
             cout << "going to try to grab " << currBaseHistGrabName + vecSystNames[iSyst] + TString("Down") << endl;
             }
             */
            vecCurrInHist_SystVarUp[iSyst] = (TH2F *) inCurrFile->Get(currBaseHistGrabName + vecSystNames[iSyst] + TString("Up"));
            currIntegral_SystVarUp[iSyst] = vecCurrInHist_SystVarUp[iSyst]->GetBinContent(binX_InHist, binY_InHist);
            if (numDims == 1) {
                histToSet = vecOutMT2Hist_1D_SystVarUp[iSyst];
            }
            else if (numDims == 2) {
                histToSet = vecOutMT2Hist_2D_SystVarUp[iSyst];
            }
            else {
                histToSet = vecOutMT2Hist_3D_SystVarUp[iSyst];
            }
            SetYieldInputHist(currIntegral_SystVarUp[iSyst], inVecBins_OutHist, histToSet);
            
            vecCurrInHist_SystVarDown[iSyst] = (TH2F *) inCurrFile->Get(currBaseHistGrabName + vecSystNames[iSyst] + TString("Down"));
            currIntegral_SystVarDown[iSyst] = vecCurrInHist_SystVarDown[iSyst]->GetBinContent(binX_InHist, binY_InHist);
            if (numDims == 1) {
                histToSet = vecOutMT2Hist_1D_SystVarDown[iSyst];
            }
            else if (numDims == 2) {
                histToSet = vecOutMT2Hist_2D_SystVarDown[iSyst];
            }
            else {
                histToSet = vecOutMT2Hist_3D_SystVarDown[iSyst];
            }
            SetYieldInputHist(currIntegral_SystVarDown[iSyst], inVecBins_OutHist, histToSet);
        }
    }
    void IterateOverCuts(massCombo * inMassPoint, bool doVerb = 0) {
        TString repFPat, repSPat, repTPat;
        
        vector<unsigned int> vecSizes(0);
        for (unsigned int iAxis = 0; iAxis < vecAxes.size(); ++iAxis) {
            vecSizes.push_back(vecAxes[iAxis].size());
            if (doVerb) {
                cout << "vecSizes[iAxis = " << iAxis << "] is " << vecSizes[iAxis] << endl;
            }
        }
        vector<int> vecBins;
        vecBins.resize(3);
        
        TString currGrabName;
        TFile * currFile;
        
        int subFactor = 0;
        
        for (unsigned int iCutX = 0; iCutX < vecSizes[0] - subFactor; ++iCutX) {
            vecBins[0] = vecSizes[0] - subFactor - (iCutX);
            if (doVerb) {
                cout << "iCutX: " << iCutX << ", vecBins[0] = " << vecBins[0] << endl;
            }
            if (vecSizes.size() > 1) {
                for (unsigned int iCutY = 0; iCutY < vecSizes[1] - subFactor; ++iCutY) {
                    vecBins[1] = vecSizes[1] - subFactor - (iCutY);
                    if (doVerb) {
                        cout << "iCutY: " << iCutY << ", vecBins[1] = " << vecBins[1] << endl;
                    }
                    if (vecSizes.size() > 2) {
                        for (unsigned int iCutZ = 0; iCutZ < vecSizes[2] - subFactor; ++iCutZ) {
                            vecBins[2] = vecSizes[2] - subFactor - iCutZ;
                            if (doVerb) {
                                cout << "iCutZ: " << iCutZ << ", vecBins[2] = " << vecBins[2] << endl;
                            }
                            currGrabName = baseNameInFile;
                            repFPat = "";
                            if (doVerb) {
                                cout << "vecAxes[0][vecBins[0] - 1] " << vecAxes[0][vecBins[0] - 1] << endl;
                            }
                            repFPat += ModFive(vecAxes[0][vecBins[0] - 1]);
                            currGrabName.Replace(currGrabName.Index("FPATSY"), 6, repFPat);
                            
                            if (doVerb) {
                                cout << "vecAxes[1][vecBins[1] - 1] " << vecAxes[1][vecBins[1] - 1] << endl;
                            }
                            repSPat = "";
                            repSPat += ModFive(vecAxes[1][vecBins[1] - 1]);
                            currGrabName.Replace(currGrabName.Index("SPATSY"), 6, repSPat);
                            
                            if (doVerb) {
                                cout << "vecAxes[2][vecBins[2] - 1] " << vecAxes[2][vecBins[2] - 1] << endl;
                            }
                            repTPat = "";
                            repTPat += ModFive(vecAxes[2][vecBins[2] - 1]);
                            currGrabName.Replace(currGrabName.Index("TPATSY"), 6, repTPat);
                            
                            currFile = TFile::Open(inBaseDir + currGrabName + TString(".root"));
                            SetYieldInputFile(inMassPoint, currFile, currGrabName, &vecBins, doVerb);
                            currFile->Close();
                        }
                    }
                    else {
                        currGrabName = baseNameInFile;
                        repFPat = "";
                        if (doVerb) {
                            cout << "vecAxes[0][vecBins[0] - 1] " << vecAxes[0][vecBins[0] - 1] << endl;
                        }
                        repFPat += ModFive(vecAxes[0][vecBins[0] - 1]);
                        currGrabName.Replace(currGrabName.Index("FPATSY"), 6, repFPat);
                        
                        vecBins[2] = 1;
                        
                        if (doVerb) {
                            cout << "vecAxes[1][vecBins[1] - 1] " << vecAxes[1][vecBins[1] - 1] << endl;
                        }
                        repSPat = "";
                        repSPat += ModFive(vecAxes[1][vecBins[1] - 1]);
                        currGrabName.Replace(currGrabName.Index("SPATSY"), 6, repSPat);
                        currFile = TFile::Open(inBaseDir + currGrabName + TString(".root"));
                        SetYieldInputFile(inMassPoint, currFile, currGrabName, &vecBins, doVerb);
                        currFile->Close();
                    }
                }
            }
            else {
                vecBins[1] = 1;
                vecBins[2] = 1;
                currGrabName = baseNameInFile;
                repFPat = "";
                if (doVerb) {
                    cout << "vecAxes[0][vecBins[0] - 1] " << vecAxes[0][vecBins[0] - 1] << endl;
                }
                repFPat += ModFive(vecAxes[0][vecBins[0] - 1]);
                currGrabName.Replace(currGrabName.Index("FPATSY"), 6, repFPat);
                currFile = TFile::Open(inBaseDir + currGrabName + TString(".root"));
                SetYieldInputFile(inMassPoint, currFile, currGrabName, &vecBins, doVerb);
                currFile->Close();
            }
        }
    }
    void InitializeHistsandOutfile(bool doVerb = 0) {
        outFile = new TFile(outBaseDir + outFileName, "RECREATE");
        if (doVerb) {
            cout << "Base hist name " << outHistBaseName << endl;
            cout << "saving to " << outFile->GetName() << endl;
        }
        if (numDims == 1) {
            outMT2Hist_CV_1D = new TH1F(outHistBaseName, axisString, vecNumBinsAxes[0], &vecAxes[0][0]);
            for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                vecOutMT2Hist_1D_SystVarUp[iSyst] = new TH1F(outHistBaseName + vecSystNames[iSyst] + TString("Up"), axisString, vecNumBinsAxes[0], &vecAxes[0][0]);
                vecOutMT2Hist_1D_SystVarDown[iSyst] = new TH1F(outHistBaseName + vecSystNames[iSyst] + TString("Down"), axisString, vecNumBinsAxes[0], &vecAxes[0][0]);
            }
        }
        else if (numDims == 2) {
            outMT2Hist_CV_2D = new TH2F(outHistBaseName, axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0]);
            for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                vecOutMT2Hist_2D_SystVarUp[iSyst] = new TH2F(outHistBaseName + vecSystNames[iSyst] + TString("Up"), axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0]);
                vecOutMT2Hist_2D_SystVarDown[iSyst] = new TH2F(outHistBaseName + vecSystNames[iSyst] + TString("Down"), axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0]);
            }
        }
        else if (numDims == 3) {
            outMT2Hist_CV_3D = new TH3F(outHistBaseName, axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0], vecNumBinsAxes[2], &vecAxes[2][0]);
            for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                vecOutMT2Hist_3D_SystVarUp[iSyst] = new TH3F(outHistBaseName + vecSystNames[iSyst] + TString("Up"), axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0], vecNumBinsAxes[2], &vecAxes[2][0]);
                vecOutMT2Hist_3D_SystVarDown[iSyst] = new TH3F(outHistBaseName + vecSystNames[iSyst] + TString("Down"), axisString, vecNumBinsAxes[0], &vecAxes[0][0], vecNumBinsAxes[1], &vecAxes[1][0], vecNumBinsAxes[2], &vecAxes[2][0]);
            }
        }
    }
    void WriteFile(bool doSigContam) {
        outFile->cd();
        if (doSigContam) {
            histSig_SigStatControlUp->Write();
            histSig_SigStatControlDown->Write();
            histSig_TTBarStatContUp->Write();
            histSig_TTBarStatContDown->Write();
            histSig_TTBarMCStatUp->Write();
            histSig_TTBarMCStatDown->Write();
        }
        outFile->Write();
        outFile->Close();
    }
};




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
            valGhost = 1E-4;
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
