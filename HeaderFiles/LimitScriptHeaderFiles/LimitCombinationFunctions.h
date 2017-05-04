#include "TString.h"
#include "LimitScripts_ParametersStruct.h"
#include "LimitScripts_SUSYStruct.h"
#include <vector>

using namespace std;

void CalculateBestExpLimit_Dilep_CC_MT2CutRange(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP, MT2LimitParams * inMT2LP) {

    TString strUnder = "_";
    
    TString pathLimCard = OutputPathCard(inLPC, inSUSYMLP, inSUSYT2LP, inMT2LP);
    TString baseNameLimCard = pathLimCard + OutputBaseNameCard(inLPC, inSUSYMLP, inSUSYT2LP);

    TString baseNameOutLimit = pathLimCard + OutputBaseNameLimit(inLPC, inSUSYMLP, inSUSYT2LP);

    TString outputPathBestInfo = OutputPathCard(inLPC, inSUSYMLP, inSUSYT2LP);
    
    TString nameBestLimitCard = outputPathBestInfo + inLPC->BestLimitString(0) + inSUSYT2LP->SavingName(false) + TString("_") + inSUSYMLP->SavingName(false) + TString(".txt");
    TString nameBestLimitFile = outputPathBestInfo + inLPC->BestLimitString(1) + inSUSYT2LP->SavingName(false) + TString("_") + inSUSYMLP->SavingName(false) + TString(".txt");
    nameBestLimitCard.Replace(nameBestLimitCard.Index("_."), 2, ".");
    nameBestLimitFile.Replace(nameBestLimitFile.Index("_."), 2, ".");
    
    TString nameTempFile = TString("TempLimFile_") + inSUSYT2LP->SavingName(false) + TString("_") + inSUSYMLP->SavingName(false) + TString(".txt");
    
    TString strAsymp = "";
    strAsymp += inLPC->useAsymLims;
    
    TString strStop = "";
    strStop += inSUSYMLP->massStop;

    TString strLSP = "";
    strLSP += inSUSYMLP->massLSP;
    
    TString debug = "0";
    
    TString cmdCalcBestLimit = "./calcBestDilepCCLimit_MT2Range.sh";
    vector<TString> vecComArgs(0);
    vecComArgs.push_back(baseNameLimCard);
    vecComArgs.push_back(baseNameOutLimit);
    vecComArgs.push_back(nameBestLimitFile);
    vecComArgs.push_back(nameBestLimitCard);
    vecComArgs.push_back(strStop);
    vecComArgs.push_back(strLSP);
    vecComArgs.push_back(strAsymp);
    vecComArgs.push_back(nameTempFile);
    vecComArgs.push_back(debug);
    
    
    //System_GeneralCommand(cmdCalcBestLimit, &vecComArgs, true);
    System_GeneralCommand(cmdCalcBestLimit, &vecComArgs, false);

    //Copy the card so that it has a copy without "Best" in the name
    TString copyBestLimitCard = nameBestLimitCard;
    copyBestLimitCard.Replace(copyBestLimitCard.Index("Best"), 4, "");
    
    //Copy the limit so that it has a copy without "Best" in the name
    TString copyBestLimitFile = nameBestLimitFile;
    copyBestLimitFile.Replace(copyBestLimitFile.Index("Best"), 4, "");
    
    TString cmdCopyBestLimitMore = "cp";
    vecComArgs.resize(0);
    vecComArgs.push_back(nameBestLimitCard);
    vecComArgs.push_back(copyBestLimitCard);
    //System_GeneralCommand(cmdCopyBestLimitMore, &vecComArgs, false);
    System_GeneralCommand(cmdCopyBestLimitMore, &vecComArgs, false);
    
    vecComArgs.resize(0);
    vecComArgs.push_back(nameBestLimitFile);
    vecComArgs.push_back(copyBestLimitFile);
    //System_GeneralCommand(cmdCopyBestLimitMore, &vecComArgs, false);
    System_GeneralCommand(cmdCopyBestLimitMore, &vecComArgs, false);
}

void CombineCards_DilepCC_SingLep(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP, MT2LimitParams * inMT2LP, bool useBestCard = true) {

    //Set the path to the limit cards (both input dilep/sing lep and also output cards
    TString outputPathLepCards = OutputPathCard(inLPC, inSUSYMLP, inSUSYT2LP);
    
    //To get the dilepton and single lepton names correctly, need to create duplicate LimitParametersContainers
    LimitParametersContainer LPC_Dilep, LPC_SingLep;
    
    LPC_Dilep = *inLPC;
    LPC_Dilep.typeLepComboAnaType = 0;
    LPC_Dilep.SetSavingString_LimitParams();
    LPC_Dilep.SetAppendString();
    LPC_Dilep.SetNames_Card_Limit();
    
    LPC_SingLep = *inLPC;
    LPC_SingLep.typeLepComboAnaType = 1;
    LPC_SingLep.SetSavingString_LimitParams();
    LPC_SingLep.SetAppendString();
    LPC_SingLep.SetNames_Card_Limit();
    
    //Name of dilepton card (if useBestCard, grab that, otherwise grab the MT2 cut information)
    TString cardToUseDilep;
    if (useBestCard) {
        cardToUseDilep = outputPathLepCards + LPC_Dilep.BestLimitString(0) + inSUSYT2LP->SavingName(false) + TString("_") + inSUSYMLP->SavingName(false) + TString(".txt");
        cardToUseDilep.Replace(cardToUseDilep.Index("_."), 2, ".");
    }
    else {
        TString baseCardNameDilep = OutputBaseNameCard(&LPC_Dilep, inSUSYMLP, inSUSYT2LP);
        cardToUseDilep = FullCardName_DilepCC(outputPathLepCards, baseCardNameDilep, inMT2LP);
    }

    //Name of single lepton card
    TString cardToUseSingLep = outputPathLepCards + OutputBaseNameCard(&LPC_SingLep, inSUSYMLP, inSUSYT2LP);
    
    //Name of output card
    TString outCardNameCombo = outputPathLepCards + OutputBaseNameCard(inLPC, inSUSYMLP, inSUSYT2LP);
    
    TString strDilepComboCommand = "Dilep=";
    strDilepComboCommand += cardToUseDilep;
    TString strSingLepComboCommand = "SingLep=";
    strSingLepComboCommand += cardToUseSingLep;
    
    TString strComboOutCard = " > ";
    strComboOutCard += outCardNameCombo;

    TString cmdCombineCards = "combineCards.py";
    vector<TString> vecComArgs(0);
    vecComArgs.push_back(strDilepComboCommand);
    vecComArgs.push_back(strSingLepComboCommand);
    vecComArgs.push_back(strComboOutCard);
    
    //System_GeneralCommand(cmdCombineCards, &vecComArgs, true);
    System_GeneralCommand(cmdCombineCards, &vecComArgs, false);
}
void CombineLimits(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP, bool doVerb = 0) {
    
    TString strUnder = "_";
    
    TString cmdCombineLimits = "./combineLimits.sh";
    if (inLPC->isSignificance) {
        cmdCombineLimits = "./combineSignifs.sh";
    }
    vector<TString> vecComArgs(0);
    
    TString basePathLimCard = BaseStringLimit_NameOrDir(inLPC, inSUSYT2LP, 0);
    vecComArgs.push_back(basePathLimCard);
    
    TString baseNameLimCard = BaseStringLimit_NameOrDir(inLPC, inSUSYT2LP, 2);
    if (inLPC->DoBest()) baseNameLimCard = TString("Best") + baseNameLimCard;
    vecComArgs.push_back(baseNameLimCard);
    
    TString pathOutComboLimit = basePathLimCard + TString("CombinedLimits/");
    if (inLPC->isSignificance) {
        pathOutComboLimit = basePathLimCard + TString("CombinedSignifs/");
    }
    vecComArgs.push_back(pathOutComboLimit);
    
    TString nameOutComboLimit = TString("Combined") + baseNameLimCard + TString(".txt");
    //nameOutComboLimit.Replace(nameOutComboLimit.Index("_."), 2, ".");
    vecComArgs.push_back(nameOutComboLimit);
    
    if (inLPC->isSignificance) {
        nameOutComboLimit.Replace(nameOutComboLimit.Index("Signif"), 6, "PValue");
        vecComArgs.push_back(nameOutComboLimit);
    }
    
    TString strMinStop = "";
    strMinStop += inSUSYMLP->minStopMass;
    vecComArgs.push_back(strMinStop);
    
    TString strMaxStop = "";
    strMaxStop += inSUSYMLP->maxStopMass;
    vecComArgs.push_back(strMaxStop);
    
    TString strStepStop = "";
    strStepStop += inSUSYMLP->binSize;
    vecComArgs.push_back(strStepStop);
    
    TString strMinLSP = "";
    strMinLSP += inSUSYMLP->minLSPMass;
    vecComArgs.push_back(strMinLSP);
    
    TString strMaxLSP = "";
    strMaxLSP += inSUSYMLP->maxLSPMass;
    vecComArgs.push_back(strMaxLSP);
    
    TString strStepLSP = "";
    strStepLSP += inSUSYMLP->binSize;
    vecComArgs.push_back(strStepLSP);
    
    TString strIsExp = "";
//    strIsExp += inLPC->isExpLim > 0;
    strIsExp += inLPC->isExpLim;
    vecComArgs.push_back(strIsExp);
    
    TString strDoAsymp = "";
    strDoAsymp += inLPC->useAsymLims;
    vecComArgs.push_back(strDoAsymp);
    
    TString strDoBest = "";
    strDoBest += (inLPC->DoBest());
    vecComArgs.push_back(strDoBest);

    TString strDoVerb = "";
    strDoVerb += doVerb;
    vecComArgs.push_back(strDoVerb);
    
    System_GeneralCommand(cmdCombineLimits, &vecComArgs, doVerb);
}