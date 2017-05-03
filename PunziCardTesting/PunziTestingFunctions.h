#include "TString.h"
#include <iostream>     // std::cout
#include <map>
#include "TGraphErrors.h"
using namespace std;

typedef map<TString, float> mapStrToF;



void SetMap(mapStrToF &inMap, int whichIndex) {
    TString baseStringSigEff = "SigEff";
    TString baseStringBackYield = "BackYield";
    
    vector<TString> vecString{"Low", "Med", "High", "SuperHigh"};
    vector<float> vecSigEff_Low{0.01, 0.008, 0.02};
    vector<float> vecSigEff_Med{0.05, 0.03, 0.04};
    vector<float> vecSigEff_High{0.2, 0.08, 0.06};
    vector<float> vecSigEff_SuperHigh{0.25, 0.12, 0.08};
    vector<float> vecBackYield_Low{4, 2, 0.2};
    vector<float> vecBackYield_Med{9, 5, 1.0};
    vector<float> vecBackYield_High{20, 8, 1.0};
    vector<float> vecBackYield_SuperHigh{36, 14, 1.8};
    
    int numVals = 4;
    
    vector< vector<float> > vecVecSigEff(numVals);
    vecVecSigEff[0] = vecSigEff_Low;
    vecVecSigEff[1] = vecSigEff_Med;
    vecVecSigEff[2] = vecSigEff_High;
    vecVecSigEff[3] = vecSigEff_SuperHigh;
    
    vector< vector<float> > vecVecBackYield(numVals);
    vecVecBackYield[0] = vecBackYield_Low;
    vecVecBackYield[1] = vecBackYield_Med;
    vecVecBackYield[2] = vecBackYield_High;
    vecVecBackYield[3] = vecBackYield_SuperHigh;
    
    TString currStringSigEff, currStringBackYield;
    
    for (int iVal = 0; iVal < numVals; ++iVal) {
        currStringSigEff = vecString[iVal] + baseStringSigEff;
        currStringSigEff += whichIndex;
        
        currStringBackYield = vecString[iVal] + baseStringBackYield;
        currStringBackYield += whichIndex;
        inMap[currStringSigEff] = vecVecSigEff[iVal][whichIndex];
        inMap[currStringBackYield] = vecVecBackYield[iVal][whichIndex];
    }
}


float GetPunziParam(float effSig, float yieldBkg, float nSigmaA, float nSigmaB, bool doVerb) {
    //equation 7 of http://www-cdf.fnal.gov/physics/statistics/notes/punzi-optimal-search.ps
    
    if (doVerb) {
        cout << "nSigmaA " << nSigmaA << endl;
        cout << "nSigmaB " << nSigmaB << endl;
        cout << "yieldBkg " << yieldBkg << endl;
        cout << "effSig " << effSig << endl;
    }
    
    float sqrtBkg = TMath::Sqrt(yieldBkg);
    
    float sigmaMin_Part1 = pow(nSigmaA, 2) / 8 + (9 * pow(nSigmaB, 2))/13 + nSigmaA * sqrtBkg;
    float sigmaMin_Part2 = (nSigmaB / 2) * TMath::Sqrt(pow(nSigmaB, 2) + 4 * nSigmaA * sqrtBkg + 4 * yieldBkg);
    
    //    float paramPunzi = effSig / (TMath::Sqrt(yieldBkg) + (nSigma/2));
    float paramPunzi = effSig / (sigmaMin_Part1 + sigmaMin_Part2);
    if (doVerb) {
        cout << "Punzi Parameter " << paramPunzi << endl;
    }
    return paramPunzi;
}

struct PunziCheck {
    mapStrToF map_InputPunzi;
    
    float sigEff;
    float backYield;
    
    vector<TString> vecString;
    vector<int> vecWhichBin;
    int punziBin;
    
    float nSigmaA, nSigmaB;
    float punziParam;
    void DefaultVarVals() {
        nSigmaA = 5;
        nSigmaB = 2;
        vecString.push_back("Low");
        vecString.push_back("Med");
        vecString.push_back("High");
        vecString.push_back("SuperHigh");
        
        vecWhichBin.push_back(0);
        vecWhichBin.push_back(1);
        vecWhichBin.push_back(2);
        vecWhichBin.push_back(3);
    }
    
    void PrintVec() {
        for (unsigned int iPermute = 0; iPermute < vecWhichBin.size(); ++iPermute) {
            cout << "vecWhichBin[" << iPermute << "]" << vecWhichBin[iPermute] << endl;
        }
    }
    
    void SetPunziMap(int whichIndex) {
        punziBin = whichIndex;
        SetMap(map_InputPunzi, punziBin);
    }
    void SetPunziParam(bool doVerb = false) {
        punziParam = GetPunziParam(sigEff, backYield, nSigmaA, nSigmaB, doVerb);
    }
    void PrintVals(int whichIndex) {
        cout << "For the " << vecString[vecWhichBin[whichIndex]]  << " Bin " << punziBin + 1 << endl;
        cout << "sigEff " << sigEff << endl;
        cout << "backYield " << backYield << endl;
        cout << "punziParam " << punziParam << endl;
    }
    void SetVals(int whichIndex, bool doVerb = false) {
        TString baseStringSigEff = "SigEff";
        TString baseStringBackYield = "BackYield";
        TString stringSig = vecString[vecWhichBin[whichIndex]] + baseStringSigEff;
        stringSig += punziBin;
        
        TString stringBack = vecString[vecWhichBin[whichIndex]] + baseStringBackYield;
        stringBack += punziBin;
        
        mapStrToF::iterator iter_floatVals;
        iter_floatVals = map_InputPunzi.find(stringSig);
        if (iter_floatVals != map_InputPunzi.end()) {
            sigEff = iter_floatVals->second;
        }
        
        iter_floatVals = map_InputPunzi.find(stringBack);
        if (iter_floatVals != map_InputPunzi.end()) {
            backYield = iter_floatVals->second;
        }
        
        SetPunziParam(doVerb);
    }
    
};





float GetTwoBinPunziParam(vector<PunziCheck> * inVecPC, bool doVerb, int whichVers = 0) {
    float nSigmaATotal = inVecPC->at(0).nSigmaA;
    float nSigmaBTotal = inVecPC->at(0).nSigmaB;
    vector<PunziCheck> vecPunziHack(2);
    vecPunziHack[0] = inVecPC->at(0);
    vecPunziHack[1] = inVecPC->at(1);
    
    float currSigEff, currBackYield;
    for (int iBin = 0; iBin < 2; ++iBin) {
        currSigEff = inVecPC->at(iBin).sigEff;
        currBackYield = inVecPC->at(iBin).backYield;
        if (iBin == whichVers) {
            currSigEff += inVecPC->at(2).sigEff;
            currBackYield += inVecPC->at(2).backYield;
        }
        vecPunziHack[iBin].sigEff = currSigEff;
        vecPunziHack[iBin].backYield = currBackYield;
        vecPunziHack[iBin].SetPunziParam(doVerb);
    }
    PunziCheck * inPCBin1 = &vecPunziHack[0];
    PunziCheck * inPCBin2 = &vecPunziHack[1];
    
    
    float currBackYield1 = inPCBin1->backYield;
    float currBackYield2 = inPCBin2->backYield;
    
    float currPunziParam1 = inPCBin1->punziParam;
    float currPunziParam2 = inPCBin2->punziParam;
    float a1 = nSigmaATotal * TMath::Power(currPunziParam1, 2) / (TMath::Power(currPunziParam1, 2) + TMath::Power(currPunziParam2, 2));
    float a2 = nSigmaATotal * TMath::Power(currPunziParam2, 2) / (TMath::Power(currPunziParam1, 2) + TMath::Power(currPunziParam2, 2));
    
    float b1 = nSigmaBTotal * TMath::Power(currPunziParam1, 2) / (TMath::Power(currPunziParam1, 2) + TMath::Power(currPunziParam2, 2));
    float b2 = nSigmaBTotal * TMath::Power(currPunziParam2, 2) / (TMath::Power(currPunziParam1, 2) + TMath::Power(currPunziParam2, 2));
    
    float indBSqSum = (TMath::Power(b1, 2) + TMath::Power(b2, 2));

    float C = (a1 * TMath::Sqrt(currBackYield1) + currBackYield1) + (a2 * TMath::Sqrt(currBackYield2) + currBackYield2);
    float CPrime = C - ((1./4) * indBSqSum);
    
    float SminSumTerm1 = (TMath::Sqrt(CPrime)/TMath::Sqrt(indBSqSum)) * (b1 + b2);
    float SminSumTerm2 = (1./2) * (b1 + b2 - 2 * (currBackYield1 + currBackYield2));
    float SMinTotal = SminSumTerm1 + SminSumTerm2;
    if (doVerb) {
        cout << "SMinTotal " << SMinTotal << endl;
    }
    return SMinTotal;
}


TString GetStringPunziOneBin(int whichSig, int whichVersPunzi) {
    vector<TString> vecCheckAddName{"", "TwoThirds", "Sqrt", "CubeRoot", "TwoFifths"};
    
    int nSigmaA = 5;
    int nSigmaB = 2;
    
    if (whichVersPunzi) {
        nSigmaA = 2;
        nSigmaB = 2;
    }
    
    TString outString = "SigmaA";
    outString += nSigmaA;
    outString += "_SigmaB";
    outString += nSigmaB;
    outString += "_";
    outString += vecCheckAddName[whichSig];

    return outString;
}

void SetTGraphErrors(TGraphErrors * inputGraph, TH1F * inputHistPunzi, TString inputFile, bool doCompare, bool doVerb = 0) {
    
    //if doCompare is true, then this will store the relative difference (rather than the values) for each consecutive pair
    
    float thresh = 9998;
    float cvLimit;
    
    vector<float> vecParticleMass;
    vector<float> vecLimit, vecLimitOneSigUp, vecLimitOneSigDown;
    vector<TString> vecBinName;
    
    if (doVerb) cout << "going to try to open inputFile " << inputFile << endl;
    
    ifstream inputFileStream(inputFile);
    if (!inputFileStream) {
        cout << "file: " << inputFile << " doesn't exist apparently!" << endl;
        return;
    }
    string line, field;
    int lineCounter = 0;
    int fieldCounter;
    TString currBinName;
    
    while (!(inputFileStream.eof())) {
        lineCounter++;
        getline(inputFileStream, line);
        stringstream ss( line );
        fieldCounter = 0;
        while (std::getline(ss, field, ':' )) {
            stringstream fs ( field );
            float currField = 0.0;
            fs >> currField;
            if (doVerb) {
                cout << "For line: " << lineCounter << " and Field # " << fieldCounter << ", currField is " << currField << endl;
            }
            switch (fieldCounter) {
                case 0:
                    vecParticleMass.push_back(currField);
                    break;
                case 1:
                    break;
                    /*
                     case 1:
                     vecLSPMass.push_back(currField);
                     break;
                     */
                case 2:
                    vecLimit.push_back(currField);
                    cvLimit = currField;
                    //cout << "cvLimit " << cvLimit << endl;
                    break;
                case 3:
                    if (cvLimit > thresh) {
                        currField = cvLimit;
                    }
                    vecLimitOneSigUp.push_back(currField);
                    break;
                case 4:
                    if (cvLimit > thresh) {
                        currField = cvLimit;
                    }
                    vecLimitOneSigDown.push_back(currField);
                    break;
                case 5:
                    currBinName = TString(field);
                    if (currBinName.Length() != 0) {
                        vecBinName.push_back(currBinName);
                    }
                    break;
                case 6:
                    break;
                    /*
                     case 5:
                     vecMT2llCut.push_back(currField);
                     break;
                     case 6:
                     vecMT2lblbCut.push_back(currField);
                     break;
                     */
                default:
                    cout << "way too many fields " << fieldCounter << endl;
                    break;
            }
            fieldCounter++;
        }
    }
    unsigned int nParticleMass;
    float currValHistX;
    float currPunzi;
    int binX;
    if (doCompare) {
        float currLimitVal_Vers1, currLimitVal_Vers2;
        float currLimitErr_Vers1, currLimitErr_Vers2;
        float currTotalErr, currDiff;

        nParticleMass = vecParticleMass.size();
        inputGraph->Set(nParticleMass / 2);
        for (unsigned int iPoint = 0; iPoint < nParticleMass; iPoint += 2) {
            currValHistX = inputHistPunzi->GetXaxis()->GetBinCenter((iPoint / 2) + 1);
            currPunzi = inputHistPunzi->GetBinContent((iPoint / 2) + 1);
            currLimitVal_Vers1 = vecLimit[iPoint];
            currLimitErr_Vers1 = TMath::Abs(vecLimitOneSigUp[iPoint] - vecLimit[iPoint]);
            currLimitVal_Vers2 = vecLimit[iPoint + 1];
            currLimitErr_Vers2 = TMath::Abs(vecLimitOneSigUp[iPoint + 1] - vecLimit[iPoint + 1]);

            
            currTotalErr = TMath::Sqrt( TMath::Power(currLimitErr_Vers1, 2) + TMath::Power(currLimitErr_Vers2, 2));

            currDiff = (currLimitVal_Vers2 - currLimitVal_Vers1);
            
            /*
            if (currDiff * currPunzi < 0) {
                currDiff *= -1;
            }
            */
            /*
            cout << "iPoint " << iPoint << endl;
            cout << "currValHistX " << currValHistX << endl;
            cout << "currLimitVal_Vers1 " << currLimitVal_Vers1 << endl;
            cout << "currLimitVal_Vers2 " << currLimitVal_Vers2 << endl;
            cout << "currDiff " << currDiff << endl;
            cout << "currTotalErr " << currTotalErr << endl;
            */
            inputGraph->SetPoint(iPoint, currValHistX, currDiff);
            inputGraph->SetPointError(iPoint, 0, currTotalErr);
        }
    }
    else {
        float currPunzi, currScale;
        nParticleMass = vecParticleMass.size();
        inputGraph->Set(nParticleMass);
        for (unsigned int iPoint = 0; iPoint < nParticleMass; ++iPoint) {
            currPunzi = inputHistPunzi->GetBinContent(iPoint + 1);
            currValHistX = inputHistPunzi->GetXaxis()->GetBinCenter(iPoint + 1);
            if (iPoint == 0) {
                currScale = currPunzi * vecLimit[iPoint];
            }
            inputGraph->SetPoint(iPoint, currValHistX, currScale/vecLimit[iPoint]);
            inputGraph->SetPointError(iPoint, 0, 0.1 * currScale * TMath::Abs(vecLimitOneSigUp[iPoint] - vecLimit[iPoint])/vecLimit[iPoint]);
        }
    }
}

TH1F * WeightedPunziLimitCompare(TH1F * inputPunziHist, TGraphErrors * inputLimitGraph, int whichDirection, TString addName) {
    //whichDirection of 1 means you want their signs to be opposite
    //whichDirection of -1 means you want their signs to be the same
    TString histName = "h_PunziLimitCompare" + addName;
    TH1F * outHist = new TH1F(histName, ";Punzi Weighted Difference of Limits; number of entries", 300, -3, 3);
    
    int numPunzi = inputPunziHist->GetNbinsX();
    
    float currValPunziDiff, currValGraph, currErrGraph;
    double currX, currY;
    for (int iBinX = 1; iBinX <= numPunzi; ++iBinX) {
        inputLimitGraph->GetPoint(iBinX - 1, currX, currY);
        currErrGraph = inputLimitGraph->GetErrorY(iBinX - 1);
        currValGraph = currY / currErrGraph;
        currValPunziDiff = inputPunziHist->GetBinContent(iBinX);
        
        if (whichDirection > 0) {
            //signs should be opposite
            if (currValPunziDiff * currValGraph < 0) {
                currValGraph = TMath::Abs(currValGraph);
            }
            else {
                currValGraph = -1 * TMath::Abs(currValGraph);
            }
        }
        else {
            if (currValPunziDiff * currValGraph < 0) {
                currValGraph = -1 * TMath::Abs(currValGraph);
            }
            else {
                currValGraph = TMath::Abs(currValGraph);
            }
        }
        outHist->Fill(currValGraph);
    }
    return outHist;
}


void PunziHist(TH1F * &inputPunziHist, TString inputFile, TString inputNamePunziHist, bool doCompare, bool doVerb = 0) {
    
    //if doCompare is true, then this will store the relative difference (rather than the values) for each consecutive pair
    
    vector<float> vecBackYield;
    vector<float> vecPunziParam;
    vector<TString> vecPunziName;
    if (doVerb) cout << "going to try to open inputFile " << inputFile << endl;
    
    ifstream inputFileStream(inputFile);
    if (!inputFileStream) {
        cout << "file: " << inputFile << " doesn't exist apparently!" << endl;
        return;
    }
    
    TString currBinName;
    
    string line, field;
    int lineCounter = 0;
    int fieldCounter;
    while (!(inputFileStream.eof())) {
        lineCounter++;
        getline(inputFileStream, line);
        stringstream ss( line );
        fieldCounter = 0;
        while (std::getline(ss, field, ':' )) {
            stringstream fs ( field );
            float currField = 0.0;
            fs >> currField;
            if (doVerb) {
                cout << "For line: " << lineCounter << " and Field # " << fieldCounter << ", currField is " << currField << endl;
            }
            switch (fieldCounter) {
                case 0:
                    vecBackYield.push_back(currField);
                    break;
                    /*
                     case 1:
                     vecLSPMass.push_back(currField);
                     break;
                     */
                case 1:
                    vecPunziParam.push_back(currField);
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    currBinName = TString(field);
                    if (currBinName.Length() != 0) {
                        vecPunziName.push_back(currBinName);
                    }
                    break;
                default:
                    cout << "way too many fields " << fieldCounter << endl;
                    break;
            }
            fieldCounter++;
        }
    }
    unsigned int nPunzi;
    float backgroundLB, backgroundUB;
    float binSize;
    if (!doCompare) {
        nPunzi = vecBackYield.size();
        binSize = vecBackYield[1] - vecBackYield[0];
        backgroundLB = vecBackYield[0] - binSize * 0.5;
        backgroundUB = vecBackYield[nPunzi - 1] + binSize * 0.5;
        
        inputPunziHist = new TH1F(inputNamePunziHist, "; Background Yield; Punzi Parameter x 1000", nPunzi, backgroundLB, backgroundUB);
        
        int binX;
        for (unsigned int iPoint = 0; iPoint < vecBackYield.size(); ++iPoint) {
            binX = inputPunziHist->GetXaxis()->FindBin(vecBackYield[iPoint]);
            inputPunziHist->SetBinContent(binX, 1000 * vecPunziParam[iPoint]);
            inputPunziHist->SetBinError(binX, 0);
        }
    }
    else {
        nPunzi = vecBackYield.size(); //divide by two before actually using
        binSize = vecBackYield[2] - vecBackYield[0];
        backgroundLB = vecBackYield[0] - binSize * 0.5;
        backgroundUB = vecBackYield[nPunzi - 1] + binSize * 0.5;
        
        inputPunziHist = new TH1F(inputNamePunziHist, "; Background Yield; (Combo Punzi Version 2 - Combo Punzi Version 1) / Combo Punzi Version 1", nPunzi / 2, backgroundLB, backgroundUB);
        
        int binX;
        for (unsigned int iPoint = 0; iPoint < nPunzi; iPoint += 2) {
            binX = inputPunziHist->GetXaxis()->FindBin(vecBackYield[iPoint]);
            inputPunziHist->SetBinContent(binX, (vecPunziParam[iPoint + 1] - vecPunziParam[iPoint])/vecPunziParam[iPoint] );
            inputPunziHist->SetBinError(binX, 0);
        }
    }
}


void LimitHist(TH1F * &inputSigStrengthHist, TString inputFile, TString inputNameSigStrengthHist, int binSize, bool doVerb = 0) {
    //Function to read the text file containing limits
    //Assumption is that text files are in format StopMass:LSPMass:CVLimit:OneSigUpLimit:OneSigDownLimit:MT2llCut:MT2lblbCut
    //The "MT2ll Cut" and "MT2lblbCut" only make sense in the context of the dilepton Cut and Count limits
    //As well, for frequentist limits the OneSigUp/OneSigDown limits are the *uncertainty on the calculation*
    //while for Asymptotic they are the *uncertainty on the limit*
    //So for Frequentist limits you need access as well to files containing OneSigUp/OneSigDown limits
    
    float thresh = 9998;
    float cvLimit;
    
    vector<float> vecParticleMass;
    vector<float> vecLimit, vecLimitOneSigUp, vecLimitOneSigDown;
    
    if (doVerb) cout << "going to try to open inputFile " << inputFile << endl;
    
    ifstream inputFileStream(inputFile);
    if (!inputFileStream) {
        cout << "file: " << inputFile << " doesn't exist apparently!" << endl;
        return;
    }
    string line, field;
    int lineCounter = 0;
    int fieldCounter;
    while (!(inputFileStream.eof())) {
        lineCounter++;
        getline(inputFileStream, line);
        stringstream ss( line );
        fieldCounter = 0;
        while (std::getline(ss, field, ':' )) {
            stringstream fs ( field );
            float currField = 0.0;
            fs >> currField;
            if (doVerb) {
                cout << "For line: " << lineCounter << " and Field # " << fieldCounter << ", currField is " << currField << endl;
            }
            switch (fieldCounter) {
                case 0:
                    vecParticleMass.push_back(currField);
                    break;
                    /*
                     case 1:
                     vecLSPMass.push_back(currField);
                     break;
                     */
                case 2:
                    vecLimit.push_back(currField);
                    cvLimit = currField;
                    //cout << "cvLimit " << cvLimit << endl;
                    break;
                case 3:
                    if (cvLimit > thresh) {
                        currField = cvLimit;
                    }
                    vecLimitOneSigUp.push_back(currField);
                    break;
                case 4:
                    if (cvLimit > thresh) {
                        currField = cvLimit;
                    }
                    vecLimitOneSigDown.push_back(currField);
                    break;
                    /*
                     case 5:
                     vecMT2llCut.push_back(currField);
                     break;
                     case 6:
                     vecMT2lblbCut.push_back(currField);
                     break;
                     */
                default:
                    cout << "way too many fields " << fieldCounter << endl;
                    break;
            }
            fieldCounter++;
        }
    }
    unsigned int nParticleMass = vecParticleMass.size();
    float particleMassLB = vecParticleMass[0] - binSize * 0.5;
    float particleMassUB = vecParticleMass[nParticleMass - 1] + binSize * 0.5;
    
    inputSigStrengthHist = new TH1F(inputNameSigStrengthHist, "; Check Iteration", nParticleMass, particleMassLB, particleMassUB);
    
    int binX;
    for (unsigned int iPoint = 0; iPoint < vecParticleMass.size(); ++iPoint) {
        binX = inputSigStrengthHist->GetXaxis()->FindBin(vecParticleMass[iPoint]);
        inputSigStrengthHist->SetBinContent(binX, vecLimit[iPoint]);
        inputSigStrengthHist->SetBinError(binX, TMath::Abs(vecLimitOneSigUp[iPoint] - vecLimit[iPoint]));
    }
}



TString GetCardPatsyReplace(vector<PunziCheck> * inVecPC, int iPermute, int numBins = 1) {
    TString currPatsyReplace = "";
    for (int iBin = 0; iBin < numBins; ++iBin) {
        currPatsyReplace += inVecPC->at(iBin).vecString[iPermute];
        currPatsyReplace += "Bin";
        currPatsyReplace += iBin + 1;
    }
    return currPatsyReplace;
}





void RunCard(TString outLimitName, TString inputCardName, bool doVerb = false) {
    vector<TString> vecComArgs(0);
    bool doAsymptotic = false;
    int isExpLim = 1;
    
    TString strAsymptotic = "";
    strAsymptotic += doAsymptotic;
    
    TString strExpLim = "";
    strExpLim += isExpLim;
    
    TString nameLimitCommand = "runCondor.sh ./runExpLimit_InputFile.sh";
    
    vecComArgs.push_back(inputCardName);
    vecComArgs.push_back(outLimitName);
    vecComArgs.push_back(strAsymptotic);
    vecComArgs.push_back(strExpLim);
    
    System_GeneralCommand(nameLimitCommand, &vecComArgs, doVerb);
}


void GenerateCard(TString patsyCardName, TString inputStringCard, vector<PunziCheck> * inVecPC, float sigXsec, int numBins = 1, int whichVers = 0, bool doVerb = false) {
    
    char buf[99];
    
    TString addToName = "AddToBin";
    if (numBins > 1) {
        addToName = whichVers > 0 ? "2" : "1";
        inputStringCard.Replace(inputStringCard.Index(".txt"), 4, addToName);
    }
    System_CopyFile(patsyCardName, inputStringCard, doVerb);
    float currSigYield;
    TString currReplaceString, currSearchString;
    for (int iBin = 0; iBin < numBins; ++iBin) {
        currSigYield = inVecPC->at(iBin).sigEff * sigXsec;
        currReplaceString = "";
        sprintf(buf, "%1.2f", currSigYield);
        currReplaceString += TString(buf);
        currSearchString = "SIGPROXY";
        currSearchString += iBin + 1;
        System_SedInFile(inputStringCard, currSearchString, currReplaceString, doVerb);
        currReplaceString = "";
        currReplaceString += inVecPC->at(iBin).backYield;
        currSearchString = "BKGPROXY";
        currSearchString += iBin + 1;
        System_SedInFile(inputStringCard, currSearchString, currReplaceString, doVerb);
    }
}