#include "TRint.h"
#include <iostream>     // std::cout
#include <algorithm>    // std::next_permutation, std::sort
#include <map>
#include "TMath.h"
#include "TString.h"
#include <vector>
using namespace std;
typedef map<TString, float> mapStrToF;

#include "../HeaderFiles/BasicFunctions_SystemCommands.h"


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
    vector<float> vecBackYield_High{20, 8, 1.4};
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

struct PunziCheck {
    mapStrToF map_InputPunzi;
    
    float sigEff;
    float backYield;
    
    vector<TString> vecString_Sig, vecString_Bkg;
    vector<int> vecWhichBin;
    int punziBin;
    
    float nSigmaA, nSigmaB;
    float punziParam;
    void DefaultVarVals() {
        nSigmaA = 5;
        nSigmaB = 2;
        vecString_Sig.push_back("Low");
        vecString_Sig.push_back("Med");
        vecString_Sig.push_back("High");
        vecString_Sig.push_back("SuperHigh");
        vecString_Bkg.push_back("Low");
        vecString_Bkg.push_back("Med");
        vecString_Bkg.push_back("High");
        vecString_Bkg.push_back("SuperHigh");
        
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
    void PrintVals(int whichIndex_Sig, int whichIndex_Bkg) {
        cout << "For the " << vecString_Sig[vecWhichBin[whichIndex_Sig]]  << " Sig, " << vecString_Bkg[vecWhichBin[whichIndex_Bkg]] << " Bkg Bin " << punziBin + 1 << endl;
        cout << "sigEff " << sigEff << endl;
        cout << "backYield " << backYield << endl;
        cout << "punziParam " << punziParam << endl;
    }
    void SetVals(int whichIndex_Sig, int whichIndex_Bkg, bool doVerb = false) {
        TString baseStringSigEff = "SigEff";
        TString baseStringBackYield = "BackYield";
        /*
         cout << "vecString_Sig size " << vecString_Sig.size() << endl;
         cout << "vecWhichBin size " << vecWhichBin.size() << endl;
         */
        TString stringSig = vecString_Sig[vecWhichBin[whichIndex_Sig]] + baseStringSigEff;
        stringSig += punziBin;
        
        TString stringBack = vecString_Bkg[vecWhichBin[whichIndex_Bkg]] + baseStringBackYield;
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



float GetTwoBinPunziParam(vector<PunziCheck> * inVecPC, int paramVariation, bool doVerb, int whichVers = 0) {
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
    
    
    float a1 = nSigmaATotal * TMath::Power(currPunziParam1, paramVariation) / (TMath::Power(currPunziParam1, paramVariation) + TMath::Power(currPunziParam2, paramVariation));
    float a2 = nSigmaATotal * TMath::Power(currPunziParam2, paramVariation) / (TMath::Power(currPunziParam1, paramVariation) + TMath::Power(currPunziParam2, paramVariation));
    
    float b1 = nSigmaBTotal * TMath::Power(currPunziParam1, paramVariation) / (TMath::Power(currPunziParam1, paramVariation) + TMath::Power(currPunziParam2, paramVariation));
    float b2 = nSigmaBTotal * TMath::Power(currPunziParam2, paramVariation) / (TMath::Power(currPunziParam1, paramVariation) + TMath::Power(currPunziParam2, paramVariation));
    

    
    float bPrime = TMath::Sqrt((TMath::Power(b1, 2) + TMath::Power(b2, 2)));
    
    float C = (a1 * TMath::Sqrt(currBackYield1) + currBackYield1) + (a2 * TMath::Sqrt(currBackYield2) + currBackYield2);
    float CPrime = C - ((1./4) * TMath::Power(bPrime, 2));

    float SminSumTerm1 = a1 * TMath::Sqrt(currBackYield1) + a2 * TMath::Sqrt(currBackYield2);
    float SminSumTerm2 = bPrime * TMath::Sqrt(CPrime);
    float SMinTotal = SminSumTerm1 + SminSumTerm2;
    if (doVerb) {
        cout << "currBackYield1 " << currBackYield1 << endl;
        cout << "currBackYield2 " << currBackYield2 << endl;
        cout << "currPunziParam1 " << currPunziParam1 << endl;
        cout << "currPunziParam2 " << currPunziParam2 << endl;
        cout << "a1 " << a1 << endl;
        cout << "a2 " << a2 << endl;
        cout << "b1 " << b1 << endl;
        cout << "b2 " << b2 << endl;
        cout << "CPrime " << CPrime << endl;
        cout << "SminSumTerm1 " << SminSumTerm1 << endl;
        cout << "SminSumTerm2 " << SminSumTerm2 << endl;
        cout << "SMinTotal " << SMinTotal << endl;
    }
    return SMinTotal;
}


TString GetCardPatsyReplace(vector<PunziCheck> * inVecPC, vector<int> * inVecPermutes_Sig, vector<int> * inVecPermutes_Bkg, int numBins = 1) {
    TString currPatsyReplace = "";
    for (int iBin = 0; iBin < numBins; ++iBin) {
        currPatsyReplace += inVecPC->at(iBin).vecString_Sig[inVecPC->at(iBin).vecWhichBin[inVecPermutes_Sig->at(iBin)]];
        currPatsyReplace += "Sig";
        currPatsyReplace += inVecPC->at(iBin).vecString_Bkg[inVecPC->at(iBin).vecWhichBin[inVecPermutes_Bkg->at(iBin)]];
        currPatsyReplace += "Bkg_";
        currPatsyReplace += "Bin";
        currPatsyReplace += iBin + 1;
    }
    return currPatsyReplace;
}

float ComboPunzi(vector<float> * inVecPunziVals, bool doSquareSum) {
    unsigned int numPunzi = inVecPunziVals->size();
    float currSumPunzi = 0.0;
    float currPunziVal;
    for (unsigned int iPunzi = 0; iPunzi < numPunzi; ++iPunzi) {
        currPunziVal = inVecPunziVals->at(iPunzi);
        cout << "for iPunzi " << iPunzi << ", currPunziVal is " << currPunziVal << endl;
        if (doSquareSum) {
            currSumPunzi += TMath::Power(currPunziVal, 2);
        }
        else {
            currSumPunzi += currPunziVal;
        }
    }
    if (doSquareSum) {
        return TMath::Sqrt(currSumPunzi);
    }
    return currSumPunzi;
}
float CalculateJointPunzi(vector<PunziCheck> * inVecPC, bool doSquareSum, int whichVers = 0) {
    vector<float> vecCurrPunzi(0);
    float currPunziCombo;
    float currSigEff, currBackYield;
    for (int iBin = 0; iBin < 2; ++iBin) {
        currSigEff = inVecPC->at(iBin).sigEff;
        currBackYield = inVecPC->at(iBin).backYield;
        if (iBin == whichVers) {
            currSigEff += inVecPC->at(2).sigEff;
            currBackYield += inVecPC->at(2).backYield;
        }
        vecCurrPunzi.push_back(GetPunziParam(currSigEff, currBackYield, 5, 2, false));
    }
    currPunziCombo = ComboPunzi(&vecCurrPunzi, doSquareSum);
    return currPunziCombo;
}
void GenerateCardTwoBin(TString patsyCardName, TString inputStringCard, vector<PunziCheck> * inVecPC, float sigXsec, int numBins = 1, int whichVers = 0, bool doVerb = false) {
    
    char buf[99];
    
    System_CopyFile(patsyCardName, inputStringCard, doVerb);
    float currSigYield, currBackYield;
    TString currReplaceString, currSearchString;
    for (int iBin = 0; iBin < 2; ++iBin) {
        currSigYield = inVecPC->at(iBin).sigEff * sigXsec;
        currBackYield = inVecPC->at(iBin).backYield;
        if (iBin == whichVers) {
            currSigYield += inVecPC->at(2).sigEff * sigXsec;
            currBackYield += inVecPC->at(2).backYield;
        }
        currReplaceString = "";
        sprintf(buf, "%1.2f", currSigYield);
        currReplaceString += TString(buf);
        currSearchString = "SIGPROXY";
        currSearchString += iBin + 1;
        System_SedInFile(inputStringCard, currSearchString, currReplaceString, doVerb);
        currReplaceString = "";
        
        sprintf(buf, "%1.2f", currBackYield);
        currReplaceString +=  TString(buf);
        currSearchString = "BKGPROXY";
        currSearchString += iBin + 1;
        System_SedInFile(inputStringCard, currSearchString, currReplaceString, doVerb);
    }
}

void RunCard(TString outLimitName, TString inputCardName, bool doVerb = false) {
    vector<TString> vecComArgs(0);
    bool doAsymptotic = false;
    int isExpLim = 1;
    
    TString strAsymptotic = "";
    strAsymptotic += doAsymptotic;
    
    TString strExpLim = "";
    strExpLim += isExpLim;
    
    TString nameLimitCommand = "runCondorTest.sh ./runExpLimit_InputFile.sh";
    
    vecComArgs.push_back(inputCardName);
    vecComArgs.push_back(outLimitName);
    vecComArgs.push_back(strAsymptotic);
    vecComArgs.push_back(strExpLim);
    
    System_GeneralCommand(nameLimitCommand, &vecComArgs, doVerb);
}


int main( int argc, char* argv[]) {
    
    TRint theApp("App", &argc, argv);
    Bool_t retVal = kTRUE;
    
    int numBins = 2;
    bool doVerb = 0;
    
    bool runCards = false;
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"doVerb", 6) == 0) {
            doVerb = 1;
        }
        else if (strncmp (argv[k],"doRun", 5) == 0) {
            runCards = true;
        }
    }
    
    
    TString patsyCardName = "Cards/punziTestCard_PATSY_OneBin.txt";
    TString patsyOutName = "Results/limitOut_PATSY_OneBin.txt";
    
    if (numBins == 2) {
        patsyCardName = "Cards/punziTestCard_PATSY_TwoBin.txt";
        patsyOutName = "Cards/limitOut_PATSY_TwoBin.txt";
    }
    TString currOutputCardName, currOutputLimitName;
    
    vector<PunziCheck> vecPCs(3);
    for (int iBin = 0; iBin < 3; ++iBin) {
        vecPCs[iBin].DefaultVarVals();
        vecPCs[iBin].SetPunziMap(iBin);
    }
    
    
    float sigXsec = 100;
    
    int numPermutes = 4;
    
    vector<int> vecCurrPermute_Sig;
    vecCurrPermute_Sig.push_back(0);
    vecCurrPermute_Sig.push_back(0);
    vecCurrPermute_Sig.push_back(0);
    
    vector<int> vecCurrPermute_Bkg;
    vecCurrPermute_Bkg.push_back(0);
    vecCurrPermute_Bkg.push_back(0);
    vecCurrPermute_Bkg.push_back(0);
    
    
    int currPermute = 0;
    
    if (numBins > 1) {
        for (int iPermuteBin1_Sig = 0; iPermuteBin1_Sig < numPermutes; ++iPermuteBin1_Sig) {
            for (int iPermuteBin2_Sig = 0; iPermuteBin2_Sig < numPermutes; ++iPermuteBin2_Sig) {
                for (int iPermuteBin3_Sig = 0; iPermuteBin3_Sig < numPermutes; ++iPermuteBin3_Sig) {
                    for (int iPermuteBin1_Bkg = 0; iPermuteBin1_Bkg < numPermutes; ++iPermuteBin1_Bkg) {
                        for (int iPermuteBin2_Bkg = 0; iPermuteBin2_Bkg < numPermutes; ++iPermuteBin2_Bkg) {
                            for (int iPermuteBin3_Bkg = 0; iPermuteBin3_Bkg < numPermutes; ++iPermuteBin3_Bkg) {
                                
                                vecCurrPermute_Sig[0] = iPermuteBin1_Sig;
                                vecCurrPermute_Sig[1] = iPermuteBin2_Sig;
                                vecCurrPermute_Sig[2] = iPermuteBin3_Sig;
                                
                                vecCurrPermute_Bkg[0] = iPermuteBin1_Bkg;
                                vecCurrPermute_Bkg[1] = iPermuteBin2_Bkg;
                                vecCurrPermute_Bkg[2] = iPermuteBin3_Bkg;
                                
                                currPermute++;
                                cout << "currPermute " << currPermute << endl;
                                for (int iBin = 0; iBin < 3; ++iBin) {
                                    vecPCs[iBin].SetVals(vecCurrPermute_Sig[iBin], vecCurrPermute_Bkg[iBin]);
                                    if (doVerb) {
                                        vecPCs[iBin].PrintVals(vecCurrPermute_Sig[iBin], vecCurrPermute_Bkg[iBin]);
                                    }
                                }
                                if (runCards) {
                                    for (int iVers = 0; iVers < 2; ++iVers) {
                                        TString currPatsyReplace = GetCardPatsyReplace(&vecPCs, &vecCurrPermute_Sig, &vecCurrPermute_Bkg, numBins + 1);
                                        cout << "currPatsyReplace " << currPatsyReplace << endl;
                                        currOutputCardName = patsyCardName;
                                        currOutputCardName.Replace(currOutputCardName.Index("PATSY"), 5, currPatsyReplace);
                                        
                                        currOutputLimitName = patsyOutName;
                                        currOutputLimitName.Replace(currOutputLimitName.Index("PATSY"), 5, currPatsyReplace);
                                        
                                        TString addToName = "AddToBin";
                                        if (numBins > 1) {
                                            addToName = iVers > 0 ? "2" : "1";
                                            currOutputCardName.Replace(currOutputCardName.Index(".txt"), 0, addToName);
                                            currOutputLimitName.Replace(currOutputLimitName.Index(".txt"), 0, addToName);
                                        }
                                        
                                        GenerateCardTwoBin(patsyCardName, currOutputCardName, &vecPCs, sigXsec, numBins, iVers, doVerb);
                                        RunCard(currOutputLimitName, currOutputCardName, doVerb);
                                    }
                                }
                                else {
                                    for (int iVers = 0; iVers < 2; ++iVers) {
                                        TString currPatsyReplace = GetCardPatsyReplace(&vecPCs, &vecCurrPermute_Sig, &vecCurrPermute_Bkg, numBins + 1);
                                        cout << "for iVers " << iVers << ", currPatsyReplace " << currPatsyReplace << endl;
                                        cout << "for iVers " << iVers << ", currPunziComboVal " << CalculateJointPunzi(&vecPCs, true, iVers) << endl;
                                        cout << "for iVers " << iVers << ", currPunziNonSquareComboVal " << CalculateJointPunzi(&vecPCs, false, iVers) << endl;
                                        float TwoBinPunziParam = GetTwoBinPunziParam(&vecPCs, 2, doVerb, iVers);
                                        float TwoBinPunziParamVers2 = GetTwoBinPunziParam(&vecPCs, 1, doVerb, iVers);
                                        cout << "for iVers " << iVers << ", currPunziEllipseVal " << TwoBinPunziParam << endl;
                                        cout << "for iVers " << iVers << ", currPunziVers2EllipseVal " << TwoBinPunziParamVers2 << endl;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        /*
         for (unsigned int iPermuteBin = 0; iPermuteBin < 3; ++iPermuteBin) {
         for (vecCurrPermute[iPermuteBin] = 0; vecCurrPermute[iPermuteBin] < numPermutes; ++vecCurrPermute[iPermuteBin]) {
         
         for (int iBin = 0; iBin < 3; ++iBin) {
         cout << "iBin " << iBin << endl;
         cout << "vecCurrPermute[iBin] " << vecCurrPermute[iBin] << endl;
         vecPCs[iBin].SetVals(vecCurrPermute[iBin]);
         if (doVerb) {
         vecPCs[iBin].PrintVals(vecCurrPermute[iBin]);
         }
         }
         if (runCards) {
         for (int iVers = 0; iVers < 2; ++iVers) {
         currPermute++;
         cout << "currPermute " << currPermute << endl;
         TString currPatsyReplace = GetCardPatsyReplace(&vecPCs, &vecCurrPermute, numBins + 1);
         cout << "currPatsyReplace " << currPatsyReplace << endl;
         currOutputCardName = patsyCardName;
         currOutputCardName.Replace(currOutputCardName.Index("PATSY"), 5, currPatsyReplace);
         
         currOutputLimitName = patsyOutName;
         currOutputLimitName.Replace(currOutputLimitName.Index("PATSY"), 5, currPatsyReplace);
         
         GenerateCardTwoBin(patsyCardName, currOutputCardName, &vecPCs, sigXsec, numBins, iVers, doVerb);
         RunCard(currOutputLimitName, currOutputCardName, doVerb);
         }
         }
         else {
         
         }
         }
         }
         
         */
        /*
         while (next_permutation(vecPCs[0].vecWhichBin.begin(), vecPCs[0].vecWhichBin.end())) {
         while (next_permutation(vecPCs[1].vecWhichBin.begin(), vecPCs[1].vecWhichBin.end())) {
         //while (next_permutation(vecPCs[2].vecWhichBin.begin(), vecPCs[2].vecWhichBin.end())) {
         for (int iPermute = 0; iPermute < numPermutes; ++iPermute) {
         for (int iBin = 0; iBin < 3; ++iBin) {
         vecPCs[iBin].SetVals(iPermute);
         //vecPCs[iBin].PrintVals(iPermute);
         if (!runCards) {
         }
         }
         currPermute++;
         cout << "currPermute " << currPermute << endl;
         if (runCards) {
         TString currPatsyReplace = GetCardPatsyReplace(&vecPCs, iPermute, numBins + 1);
         cout << "currPatsyReplace " << currPatsyReplace << endl;
         currOutputCardName = patsyCardName;
         currOutputCardName.Replace(currOutputCardName.Index("PATSY"), 5, currPatsyReplace);
         currOutputCardName += ".txt";
         for (int iVers = 0; iVers < 2; ++iVers) {
         GenerateCardTwoBin(patsyCardName, currOutputCardName, &vecPCs, sigXsec, numBins, iVers, doVerb);
         }
         }
         //}
         }
         }
         }
         */
    }
    /*
     else {
     for (int iPermute = 0; iPermute < numPermutes; ++iPermute) {
     vecPCs[0].SetVals(iPermute);
     TString currPatsyReplace = GetCardPatsyReplace(&vecPCs, iPermute, numBins);
     currOutputCardName = patsyCardName;
     currOutputCardName.Replace(currOutputCardName.Index("PATSY"), 5, currPatsyReplace);
     int iVers = 0;
     GenerateCardTwoBin(patsyCardName, currOutputCardName, &vecPCs, sigXsec, numBins, iVers, doVerb);
     
     currOutputLimitName = patsyOutName;
     currOutputLimitName.Replace(currOutputLimitName.Index("PATSY"), 5, currPatsyReplace);
     if (runCards) {
     RunCard(patsyOutName, currOutputLimitName, currOutputCardName, doVerb);
     }
     }
     }
     */
    theApp.Run(retVal);
}