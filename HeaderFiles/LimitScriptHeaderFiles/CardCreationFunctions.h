#include "TMath.h"
#include "TString.h"
#include <iostream>
#include <fstream>
//#include <vector>
#include <cmath>
#include <sstream>
#include <vector>

using namespace std;

TString BaseStringLimit_NameOrDir(LimitParametersContainer * inLPC, SUSYT2LimitParams * inSUSYT2LP, int whichString) {
    //whichString: 0 for directory, 1 for limit card, 2 for limit
    if (!whichString) {
        TString pathOutCard = LimitSettingArea();
        if (inLPC->typeDilepAna > 0) {
            pathOutCard += "ShapeAnalysis/Signal/";
        }
        pathOutCard += inSUSYT2LP->SavingName(true);
        return pathOutCard;
    }
    else if (whichString == 1) {
        TString outBaseNameLimitCard = inLPC->strBaseNameCard + inSUSYT2LP->SavingName(false);
        return outBaseNameLimitCard;
    }
    TString outBaseNameLimit = inLPC->strBaseNameLim + inSUSYT2LP->SavingName(false);
    if (inLPC->isSignificance || (inLPC->hackLimSig && whichString == 1)) {
        outBaseNameLimit = inLPC->strBaseNameSignificance + inSUSYT2LP->SavingName(false);
    }
    return outBaseNameLimit;
}

TString OutputPathCard(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP, MT2LimitParams * inMT2LP = 0) {
    TString pathOutCard = BaseStringLimit_NameOrDir(inLPC, inSUSYT2LP, 0);
    //if (inLPC->typeDilepAna > 0) {
    //pathOutCard += "ShapeAnalysis/Signal/";
    //}
    pathOutCard += inSUSYMLP->SavingName(true);
    if (inMT2LP) pathOutCard += inMT2LP->strMT2LimitSave;
    return pathOutCard;
}

TString OutputBaseNameCard(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP) {
    TString outBaseNameLimitCard = BaseStringLimit_NameOrDir(inLPC, inSUSYT2LP, 1);
    outBaseNameLimitCard += TString("_") + inSUSYMLP->SavingName(false);
    if (inLPC->typeLepComboAnaType > 0 || inLPC->typeDilepAna > 0 || !inLPC->doMT2Scan) {
        outBaseNameLimitCard += TString(".txt");
        outBaseNameLimitCard.Replace(outBaseNameLimitCard.Index("_."), 2, ".");
    }
    return outBaseNameLimitCard;
}

TString InputPathSLCard(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP) {
    TString inputPathSLCard = inLPC->basePathSingLep + inSUSYT2LP->SavingName(true);
    return inputPathSLCard;
}

TString OutputBaseNameLimit(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP) {
    TString outBaseNameLimit = BaseStringLimit_NameOrDir(inLPC, inSUSYT2LP, 2);
    outBaseNameLimit += TString("_") + inSUSYMLP->SavingName(false);
    if (inLPC->typeLepComboAnaType > 0 || inLPC->typeDilepAna > 0 || !inLPC->doMT2Scan) {
        outBaseNameLimit += TString(".txt");
        outBaseNameLimit.Replace(outBaseNameLimit.Index("_."), 2, ".");
    }
    return outBaseNameLimit;
}

TString OutputLogDirectory(LimitParametersContainer * inLPC, SUSYT2LimitParams * inSUSYT2LP) {
    
    TString strUnder = "_";
    
    TString baseLogName = "CondorLogInfo_" + inSUSYT2LP->SavingName(false) + inLPC->CondorBaseLogNameAppend();
    return baseLogName;
}


void GenerateCards_CC_SingLep_SpecMassPoint(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP) {
    
    TString pathInputSLCard = InputPathSLCard(inLPC, inSUSYMLP, inSUSYT2LP);
    
    TString nameInputSLCard = pathInputSLCard + inSUSYMLP->InputSingLepCardName();
    
    //Set the path to the limit cards (both input dilep/sing lep and also output cards
    TString outputPathSLCard = OutputPathCard(inLPC, inSUSYMLP, inSUSYT2LP);
    
    //Name of single lepton card
    TString outputCardSL = outputPathSLCard + OutputBaseNameCard(inLPC, inSUSYMLP, inSUSYT2LP);
    
    TString cmdCopySLCard = "cp";
    vector<TString> vecComArgs(0);
    vecComArgs.push_back(nameInputSLCard);
    vecComArgs.push_back(outputCardSL);
    System_GeneralCommand(cmdCopySLCard, &vecComArgs, false);
}

void GenerateCards_CC_Dilep_SpecMassPoint_MT2CutRange(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP, MT2LimitParams * inMT2LP) {
    
    TString strCommand = "./makeCard_CC.sh";
    
    TString outBaseNameLimitCard = OutputBaseNameCard(inLPC, inSUSYMLP, inSUSYT2LP);
    TString outNameCard;
    
    //String for the full path to save the output card
    TString pathOutCard = OutputPathCard(inLPC, inSUSYMLP, inSUSYT2LP, inMT2LP);
    System_MakeDir(pathOutCard);
    
    
    //String to be put in the Card
    TString strSigSpecInCard = inSUSYT2LP->SavingName(false) + TString("_") + inSUSYMLP->SavingName(false);
    
    //String to search for in the yield text
    TString strSigGrep = "{";
    strSigGrep += inSUSYMLP->massStop;
    strSigGrep += ":";
    strSigGrep += inSUSYMLP->massLSP;
    strSigGrep += "}";
    
    TString pathBkgYieldSigReg = PathYield(false, inLPC->typeFullSel, false, inLPC->typeCut);
    TString pathBkgYieldContReg = PathYield(false, inLPC->typeFullSel, true, inLPC->typeCut);
    
    TString pathSigYieldSigReg = PathYield_SignalEfficiency(inSUSYT2LP, false);
    TString pathSigYieldContReg = PathYield_SignalEfficiency(inSUSYT2LP, true);
    
    TString yieldBkgSigReg, yieldSigSigReg;
    
    vector<TString> vecComArgs(7);
    vecComArgs[4] = strSigSpecInCard;
    vecComArgs[5] = strSigGrep;
    vecComArgs[6] = ""; vecComArgs[6] += inLPC->useSigContam;
    
    for (int currLBMT2ll = inMT2LP->minMT2ll; currLBMT2ll <= inMT2LP->maxMT2ll; currLBMT2ll += inMT2LP->bigStepSizeMT2ll) {
        int currUBMT2ll = TMath::Min(inMT2LP->maxMT2ll, currLBMT2ll + (inMT2LP->bigStepSizeMT2ll - inMT2LP->smallStepSizeMT2ll));
        for (int currLBMT2lblb = inMT2LP->minMT2lblb; currLBMT2lblb <= inMT2LP->maxMT2lblb; currLBMT2lblb += inMT2LP->bigStepSizeMT2lblb) {
            int currUBMT2lblb = TMath::Min(inMT2LP->maxMT2lblb, currLBMT2lblb + (inMT2LP->bigStepSizeMT2lblb - inMT2LP->smallStepSizeMT2lblb));
            for (inMT2LP->cutMT2ll = currLBMT2ll; inMT2LP->cutMT2ll <= currUBMT2ll; inMT2LP->cutMT2ll += inMT2LP->smallStepSizeMT2ll) {
                for (inMT2LP->cutMT2lblb = currLBMT2lblb; inMT2LP->cutMT2lblb <= currUBMT2lblb; inMT2LP->cutMT2lblb += inMT2LP->smallStepSizeMT2lblb) {
                    inMT2LP->SetMT2AppendString(false);
                    yieldBkgSigReg = pathBkgYieldSigReg + NameYield(inMT2LP);
                    yieldSigSigReg = pathSigYieldSigReg + NameYield_Signal(inLPC->useSigContam, false, inLPC, inMT2LP, inSUSYT2LP);
                    
                    outNameCard = FullCardName_DilepCC(pathOutCard, outBaseNameLimitCard, inMT2LP);
                    
                    vecComArgs[0] = yieldBkgSigReg;
                    vecComArgs[1] = yieldSigSigReg;
                    vecComArgs[2] = outNameCard;
                    vecComArgs[3] = inMT2LP->strMT2Append;
                    
                    System_GeneralCommand(strCommand, &vecComArgs, true);
                    System_GeneralCommand(strCommand, &vecComArgs, false);
                    
                    //cout << "yieldBkgSigReg " << yieldBkgSigReg << endl;
                    //cout << "yieldSigSigReg " << yieldSigSigReg << endl;
                    cout << "saving to outNameCard " << outNameCard << endl;
                }
            }
        }
    }
}


void GenerateCards_Shape_LepCombo(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP) {
    
    TString outputPathLepCards = OutputPathCard(inLPC, inSUSYMLP, inSUSYT2LP);
    LimitParametersContainer LPC_Dilep = *inLPC;
    LPC_Dilep.typeLepComboAnaType = 0;
    LPC_Dilep.SetStrings();
    
    LimitParametersContainer LPC_SingLep = *inLPC;
    LPC_SingLep.SetAsSingLep();
    /*
    LPC_SingLep.isSignificance = 0;
    LPC_SingLep.typeFullSel = 0;
    LPC_SingLep.typeFullSel = 0;
    LPC_SingLep.typeDilepAna = 0;
    LPC_SingLep.typeLepComboAnaType = 1;
    LPC_SingLep.SetStrings();
    */
    
    TString inputPathSingLep = OutputPathCard(&LPC_SingLep, inSUSYMLP, inSUSYT2LP);
    
    TString cardToUse_Dilep, cardToUse_SingLep;
    
    cardToUse_Dilep = outputPathLepCards + OutputBaseNameCard(&LPC_Dilep, inSUSYMLP, inSUSYT2LP);
    cardToUse_SingLep = inputPathSingLep + OutputBaseNameCard(&LPC_SingLep, inSUSYMLP, inSUSYT2LP);
    
    //Name of output card
    TString outCardNameCombo = outputPathLepCards + OutputBaseNameCard(inLPC, inSUSYMLP, inSUSYT2LP);
    
    TString strDilepComboCommand = "Dilep=";
    strDilepComboCommand += cardToUse_Dilep;
    TString strSingLepComboCommand = "SingLep=";
    strSingLepComboCommand += cardToUse_SingLep;
    
    TString strComboOutCard = " > ";
    strComboOutCard += outCardNameCombo;
    
    TString cmdCombineCards = "combineCards.py";
    vector<TString> vecComArgs(0);
    vecComArgs.push_back(strDilepComboCommand);
    vecComArgs.push_back(strSingLepComboCommand);

    vecComArgs.push_back(strComboOutCard);
    
    System_GeneralCommand(cmdCombineCards, &vecComArgs, true);
    System_GeneralCommand(cmdCombineCards, &vecComArgs, false);
    
    TString addString = "/da";
    
    TString sedFindString = outputPathLepCards + addString;
    System_SedInFile(outCardNameCombo, sedFindString, addString, false);
    sedFindString = "shapes data_obs";
    if (LPC_Dilep.splitDilepChannel) {
        sedFindString += "    Dilep_MuMu";
    }
    TString sedReplaceString = "shapes * SingLep FAKE\\n&";
    System_SedInFile(outCardNameCombo, sedFindString, sedReplaceString, false);
}



void GenerateCards_Shape_Full_MultiDilepChannel(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP) {

    TString outputPathLepCards = OutputPathCard(inLPC, inSUSYMLP, inSUSYT2LP);
    
    LimitParametersContainer LPC_MainSel = *inLPC;
    LPC_MainSel.typeFullSel = 0;
    LPC_MainSel.SetSavingString_DileptonParams();
    LPC_MainSel.SetAppendString();
    LPC_MainSel.SetNames_Card_Limit();
    
    LimitParametersContainer LPC_ZCR = *inLPC;
    LPC_ZCR.typeFullSel = 4;
    LPC_ZCR.SetSavingString_DileptonParams();
    LPC_ZCR.SetAppendString();
    LPC_ZCR.SetNames_Card_Limit();
    
    LimitParametersContainer LPC_0BJets = *inLPC;
    LPC_0BJets.typeFullSel = 5;
    LPC_0BJets.SetSavingString_DileptonParams();
    LPC_0BJets.SetAppendString();
    LPC_0BJets.SetNames_Card_Limit();
    
    /*
    LimitParametersContainer LPC_FullBCombo = *inLPC;
    LPC_FullBCombo.typeFullSel = 6;
    LPC_FullBCombo.SetSavingString_LimitParams();
    LPC_FullBCombo.SetAppendString();
    LPC_FullBCombo.SetNames_Card_Limit();
    */
    
    TString cardToUse_MainSel, cardToUse_ZCR, cardToUse_0BJets, cardToUse_FullBCombo;
    
    cardToUse_MainSel = outputPathLepCards + OutputBaseNameCard(&LPC_MainSel, inSUSYMLP, inSUSYT2LP);
    cardToUse_0BJets = outputPathLepCards + OutputBaseNameCard(&LPC_0BJets, inSUSYMLP, inSUSYT2LP);
    cardToUse_ZCR = outputPathLepCards + OutputBaseNameCard(&LPC_ZCR, inSUSYMLP, inSUSYT2LP);
//    cardToUse_FullBCombo = outputPathLepCards + OutputBaseNameCard(&LPC_FullBCombo, inSUSYMLP, inSUSYT2LP);
    
    
    //Name of output card
    TString outCardNameCombo = outputPathLepCards + OutputBaseNameCard(inLPC, inSUSYMLP, inSUSYT2LP);
    
    TString strMainSelComboCommand = "MainSel=";
    strMainSelComboCommand += cardToUse_MainSel;
    TString str0BJetsComboCommand = "NoBJets=";
    str0BJetsComboCommand += cardToUse_0BJets;
    TString strZCRComboCommand = "ZCR=";
    strZCRComboCommand += cardToUse_ZCR;

    
    TString strComboOutCard = " > ";
    strComboOutCard += outCardNameCombo;
    
    TString cmdCombineCards = "combineCards.py";
    vector<TString> vecComArgs(0);
    vecComArgs.push_back(strMainSelComboCommand);
    vecComArgs.push_back(str0BJetsComboCommand);
    if (inLPC->typeFullSel == 7) {
        vecComArgs.push_back(strZCRComboCommand);
    }
    vecComArgs.push_back(strComboOutCard);
    
    System_GeneralCommand(cmdCombineCards, &vecComArgs, true);
    System_GeneralCommand(cmdCombineCards, &vecComArgs, false);
    
    TString addString = "/da";
    
    TString sedFindString = outputPathLepCards + addString;
    System_SedInFile(outCardNameCombo, sedFindString, addString, false);
}

void GenerateCards_Shape_Full_SplitDilep(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP, LimitParametersContainer * inLPC_AddNameLPC, SUSYT2LimitParams * inSUSYT2LPHack, vector<float> * inVecTTBarNorm = NULL, TString customPath = "") {
    
    TString strUnder = "_";
    
    TString strCommand = "./makeCard_MT2Shape_FullYieldSet_SplitDilep.sh";
    
    TString inBaseNameCard = inLPC->BaseNameShapeCard();
    
    TString outNameCard = OutputBaseNameCard(inLPC, inSUSYMLP, inSUSYT2LP);
    
    //String for the full path to save the output card
    TString signalArea = OutputPathCard(inLPC, inSUSYMLP, inSUSYT2LP);

    TString pathOutCard = customPath;
    if (customPath.Length() == 0) {
        pathOutCard = OutputPathCard(inLPC, inSUSYMLP, inSUSYT2LP);
    }
    System_MakeDir(pathOutCard);
    
    cout << "pathOutCard " << pathOutCard << endl;
    
    TString baseDirNonSignal = LimitSettingArea() + "ShapeAnalysis/";
    
    vector<LimitParametersContainer> vecLPC_IndDilep(3);
    vector<TString> nameOptMap_IndDilep(3);
    vector<TString> vecSignalInfoFileName(3);
    vector<TString> vecDataInfoFileName(3);
    vector<TString> vecTTbarInfoFileName(3);
    vector<TString> vecNonTTbarInfoFileName(3);
    vector<TString> vecGhostInfoFileName(3);
    
    for (int iChan = 0; iChan < 3; ++iChan) {
        
        vecLPC_IndDilep[iChan] = *inLPC_AddNameLPC;
        vecLPC_IndDilep[iChan].typeDilepChannel = iChan;
        vecLPC_IndDilep[iChan].SetStrings();
        
        TString sigPatsyReplace = "h_" + inSUSYMLP->SavingName(false) + inSUSYT2LP->SavingName(false);
        if (vecLPC_IndDilep[iChan].typeDilepChannel > -1) {
            sigPatsyReplace += vecLPC_IndDilep[iChan].strDilepChannel;
        }
        sigPatsyReplace +=  vecLPC_IndDilep[iChan].strFullSel + strUnder;

        nameOptMap_IndDilep[iChan] = BaseStringLimit_NameOrDir(&vecLPC_IndDilep[iChan], inSUSYT2LPHack, 2);
        nameOptMap_IndDilep[iChan].Replace(nameOptMap_IndDilep[iChan].Index("Exp_"), 4, "");
        nameOptMap_IndDilep[iChan].Replace(nameOptMap_IndDilep[iChan].Index("_SplitDilep"), 11, "");
        
        int lengthPowShapeStr = vecLPC_IndDilep[iChan].strPowShape.Length();
        if (lengthPowShapeStr > 0) {
            nameOptMap_IndDilep[iChan].Replace(nameOptMap_IndDilep[iChan].Index(vecLPC_IndDilep[iChan].strPowShape), lengthPowShapeStr, 0);
        }
        nameOptMap_IndDilep[iChan] += "_dM";
        if (inLPC->fixDMBin > -1) {
            nameOptMap_IndDilep[iChan] += inLPC->fixDMBin;
        }
        else {
            nameOptMap_IndDilep[iChan] += inSUSYMLP->GetDM();
        }
        nameOptMap_IndDilep[iChan] += vecLPC_IndDilep[iChan].strDim + TString("ToOneDee");
        
        vecSignalInfoFileName[iChan] = "ShapeInfo_Signal" + nameOptMap_IndDilep[iChan] + inLPC->strSigContam + TString(".txt");
        
        vecDataInfoFileName[iChan] = TString("ShapeInfo_Data") + nameOptMap_IndDilep[iChan] + TString(".txt");
        vecTTbarInfoFileName[iChan] = TString("ShapeInfo_MC0")  + nameOptMap_IndDilep[iChan] + TString(".txt");
        vecNonTTbarInfoFileName[iChan] = TString("ShapeInfo_MC1")  + nameOptMap_IndDilep[iChan] + TString(".txt");
        vecGhostInfoFileName[iChan] = TString("ShapeInfo_MC2")  + nameOptMap_IndDilep[iChan] + TString(".txt");
        
        TString intChan = "";
        intChan += iChan;
        
        vector<TString> vecComArgs(0);
        vecComArgs.push_back(inBaseNameCard);
        vecComArgs.push_back(pathOutCard + outNameCard);
        vecComArgs.push_back(signalArea);
        vecComArgs.push_back(sigPatsyReplace);
        vecComArgs.push_back(intChan);
        vecComArgs.push_back(vecSignalInfoFileName[iChan]);
        vecComArgs.push_back(vecDataInfoFileName[iChan]);
        vecComArgs.push_back(vecTTbarInfoFileName[iChan]);
        vecComArgs.push_back(vecNonTTbarInfoFileName[iChan]);
        vecComArgs.push_back(vecGhostInfoFileName[iChan]);
        vecComArgs.push_back(baseDirNonSignal);
        vecComArgs.push_back(nameOptMap_IndDilep[iChan]);
        
        if (inLPC->typeLepComboAnaType == 2) {
            TString basePathSingLepIntInfo = "../ShapeAnalysis/SingLepShapeCardMaking/TempInfo/";
            TString singLepIntInfoFile = basePathSingLepIntInfo + TString("/IntegralInfoSingLep_") + inSUSYT2LP->SavingName(false) + TString("_") + inSUSYMLP->SavingName(false) + inLPC->strSigContam + TString(".txt");
            //singLepIntInfoFile.Replace(singLepIntInfoFile.Index("__"), 2, "_");
            
            
            TString strSUSYName = inSUSYT2LP->SavingName(false) + TString("_") + inSUSYMLP->SavingName(false);
            TString strReplaceSLFile = inLPC->NameSingLepFile() + strSUSYName + ".root";;
            strReplaceSLFile.Replace(strReplaceSLFile.Index("_."), 2, ".");
            
            vecComArgs.push_back("1");
            vecComArgs.push_back(strReplaceSLFile);
            vecComArgs.push_back(singLepIntInfoFile);
        }
        else {
            vecComArgs.push_back("0");
        }
        
        System_GeneralCommand(strCommand, &vecComArgs, true);
        System_GeneralCommand(strCommand, &vecComArgs, false);
        
        if (inVecTTBarNorm) {
            TString arrChanName[3] = {"MuMu", "EE", "EMu"};
            TString baseStrTTbarNormToReplace = "PATSYTTBar";
            TString strTTbarNormToReplace;
            for (int iChan = 0; iChan < 3; ++iChan) {
                char buf[100];
                sprintf(buf, "%0.4f", 1 + 2 * (TMath::Abs(1 - inVecTTBarNorm->at(iChan))));
                
                strTTbarNormToReplace = arrChanName[iChan] + baseStrTTbarNormToReplace;
                TString strTTBarNormSF = "";
                strTTBarNormSF += TString(string(buf));
                System_SedInFile(pathOutCard + outNameCard, strTTbarNormToReplace, strTTBarNormSF, true);
                System_SedInFile(pathOutCard + outNameCard, strTTbarNormToReplace, strTTBarNormSF, false);
            }
        }
    }
}

void GenerateCards_Shape_Full(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP, TString nameOptMap, vector<float> * inVecTTBarNorm = NULL) {
    
    TString strUnder = "_";
    TString sigPatsyReplace = "h_" + inSUSYMLP->SavingName(false) + inSUSYT2LP->SavingName(false) + inLPC->strFullSel + strUnder;
    
    TString strCommand = "./makeCard_MT2Shape_FullYieldSet.sh";
    
    TString inBaseNameCard = inLPC->BaseNameShapeCard();
    
    TString outNameCard = OutputBaseNameCard(inLPC, inSUSYMLP, inSUSYT2LP);
    
    //String for the full path to save the output card
    TString pathOutCard = OutputPathCard(inLPC, inSUSYMLP, inSUSYT2LP);
    System_MakeDir(pathOutCard);
    
    TString baseDirNonSignal = LimitSettingArea() + "ShapeAnalysis/";
    
//    TString signalInfoFileName = "ShapeInfo_Signal_" + inLPC->strDim + TString("ToOneDee_") + inLPC->strSigContam + TString(".txt");
    TString signalInfoFileName = "ShapeInfo_Signal" + nameOptMap + inLPC->strSigContam + TString(".txt");
    TString dataInfoFileName = TString("ShapeInfo_Data") + nameOptMap + TString(".txt");
    TString ttbarInfoFileName = TString("ShapeInfo_MC0") + nameOptMap + TString(".txt");
    TString nonttbarInfoFileName = TString("ShapeInfo_MC1") + nameOptMap + TString(".txt");
    TString ghostInfoFileName =  TString("ShapeInfo_MC2") + nameOptMap + TString(".txt");
    
    vector<TString> vecComArgs(0);
    vecComArgs.push_back(inBaseNameCard);
    vecComArgs.push_back(pathOutCard + outNameCard);
    vecComArgs.push_back(pathOutCard);
    vecComArgs.push_back(sigPatsyReplace);
    vecComArgs.push_back(signalInfoFileName);
    vecComArgs.push_back(dataInfoFileName);
    vecComArgs.push_back(ttbarInfoFileName);
    vecComArgs.push_back(nonttbarInfoFileName);
    vecComArgs.push_back(ghostInfoFileName);
    vecComArgs.push_back(baseDirNonSignal);
    vecComArgs.push_back(nameOptMap);
    
    if (inLPC->typeLepComboAnaType == 2) {
        TString basePathSingLepIntInfo = "../ShapeAnalysis/SingLepShapeCardMaking/TempInfo/";
        TString singLepIntInfoFile = basePathSingLepIntInfo + TString("/IntegralInfoSingLep_") + inSUSYT2LP->SavingName(false) + TString("_") + inSUSYMLP->SavingName(false) + inLPC->strSigContam + TString(".txt");
        //singLepIntInfoFile.Replace(singLepIntInfoFile.Index("__"), 2, "_");
        
        
        TString strSUSYName = inSUSYT2LP->SavingName(false) + TString("_") + inSUSYMLP->SavingName(false);
        TString strReplaceSLFile = inLPC->NameSingLepFile() + strSUSYName + ".root";;
        strReplaceSLFile.Replace(strReplaceSLFile.Index("_."), 2, ".");
        
        vecComArgs.push_back("1");
        vecComArgs.push_back(strReplaceSLFile);
        vecComArgs.push_back(singLepIntInfoFile);
    }
    else {
        vecComArgs.push_back("0");
    }
    
    System_GeneralCommand(strCommand, &vecComArgs, true);
    System_GeneralCommand(strCommand, &vecComArgs, false);
    if (inVecTTBarNorm) {
        char buf[100];
        sprintf(buf, "%0.4f", 1 + 2 * (TMath::Abs(1 - inVecTTBarNorm->at(3))));
        TString strTTbarNormToReplace = "PATSYTTBar";
        TString strTTBarNormSF = "";
        strTTBarNormSF += TString(string(buf));
        System_SedInFile(pathOutCard + outNameCard, strTTbarNormToReplace, strTTBarNormSF, true);
        System_SedInFile(pathOutCard + outNameCard, strTTbarNormToReplace, strTTBarNormSF, false);
    }
}


void GenerateCards_Shape(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP) {
    
    TString strUnder = "_";
    TString sigPatsyReplace = inSUSYMLP->SavingName(false) + inSUSYT2LP->SavingName(false) + inLPC->strFullSel;
    
    TString strCommand = "./makeCard_MT2Shape_SignalSet.sh";
    
    TString inBaseNameCard = inLPC->BaseNameShapeCard();
    
    TString outNameCard = OutputBaseNameCard(inLPC, inSUSYMLP, inSUSYT2LP);
    
    //String for the full path to save the output card
    TString pathOutCard = OutputPathCard(inLPC, inSUSYMLP, inSUSYT2LP);
    System_MakeDir(pathOutCard);
    
    TString signalInfoFileName = "ShapeInfo_Signal_" + inLPC->strDim + TString("ToOneDee_") + inLPC->strSigContam + TString(".txt");
    
    vector<TString> vecComArgs(0);
    vecComArgs.push_back(inBaseNameCard);
    vecComArgs.push_back(pathOutCard + outNameCard);
    vecComArgs.push_back(pathOutCard);
    vecComArgs.push_back(sigPatsyReplace);
    vecComArgs.push_back(signalInfoFileName);
    
    if (inLPC->typeLepComboAnaType == 2) {
        TString basePathSingLepIntInfo = "../ShapeAnalysis/SingLepShapeCardMaking/TempInfo/";
        TString singLepIntInfoFile = basePathSingLepIntInfo + TString("/IntegralInfoSingLep_") + inSUSYT2LP->SavingName(false) + TString("_") + inSUSYMLP->SavingName(false) + inLPC->strSigContam + TString(".txt");
        
        
        TString strSUSYName = inSUSYT2LP->SavingName(false) + TString("_") + inSUSYMLP->SavingName(false);
        TString strReplaceSLFile = inLPC->NameSingLepFile() + strSUSYName + ".root";;
        strReplaceSLFile.Replace(strReplaceSLFile.Index("_."), 2, ".");
        
        vecComArgs.push_back("1");
        vecComArgs.push_back(strReplaceSLFile);
        vecComArgs.push_back(singLepIntInfoFile);
    }
    else {
        vecComArgs.push_back("0");
    }
    
    System_GeneralCommand(strCommand, &vecComArgs, true);
    System_GeneralCommand(strCommand, &vecComArgs, false);
}

