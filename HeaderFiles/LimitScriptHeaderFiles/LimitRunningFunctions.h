#include "TString.h"
#include "LimitScripts_ParametersStruct.h"
#include "LimitScripts_SUSYStruct.h"

void MakeCondorLimitCalc(TString fullPathLimitScriptUmbrella, TString fullPathLimitScript, TString fullPathLimitCard, TString fullPathOutLimit, TString fullPathBaseLogDirectory, TString strWhichLimit, LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP, bool debug = false) {
    
    TString strSpace = " ";
    TString strUnder = "_";
    
    TString patsyCondorScript = "PatsyCondorScript.sh";
    TString patsyCondorSubmit = "PatsyCondorSubmit.txt";
    
    if (inLPC->doLocal) {
        patsyCondorSubmit = "PatsyCondorSubmitLocal.txt";
    }
    
    TString strCharFrac = inSUSYT2LP->currSDP.strCharFracName;

    TString appendName = TString("Sig_") + inSUSYMLP->SavingName(false) + inSUSYT2LP->SavingName(false) + inLPC->strFullSel;
    if (inLPC->splitDilepChannel) {
        appendName += "_SplitDilep";
    }
    if (inLPC->fixDMBin > -1) {
        appendName += "_FixBin";
        appendName += inLPC->fixDMBin;
    }
    TString fullPathLogDirectory = fullPathBaseLogDirectory + TString("/Logs_") + appendName + TString("/");
    
    appendName += strUnder + strWhichLimit;
    
    if (!inLPC->useAsymLims) {
        System_MakeDir(fullPathLogDirectory, debug);
    }
    
    TString baseCondorName = "RunLimit" + inLPC->strExpLim + appendName;
    if (inLPC->isSignificance) {
        baseCondorName = "RunSignificance" + inLPC->strExpLim + appendName;
    }
    
    TString nameCondorScript = fullPathLogDirectory + baseCondorName + TString(".sh");
    TString nameCondorScriptStdOut = nameCondorScript + TString(".stdout");
    TString nameCondorScriptStdErr = nameCondorScript + TString(".stderr");
    TString nameCondorScriptLogFile = nameCondorScript + TString(".logfile");

    TString nameCondorSubmit = fullPathLogDirectory + baseCondorName + TString(".submit");
    
    if (!inLPC->useAsymLims) {
        System_RemoveFile(nameCondorScript, debug);
        System_RemoveFile(nameCondorScriptStdOut, debug);
        System_RemoveFile(nameCondorScriptStdErr, debug);
        System_RemoveFile(nameCondorScriptLogFile, debug);
        System_RemoveFile(nameCondorSubmit, debug);
    }
    
    TString strAsymExp = "";
    strAsymExp += inLPC->useAsymLims;
    strAsymExp += strSpace;
    strAsymExp += inLPC->isExpLim;
    if (inLPC->isSignificance) {
        strAsymExp += strSpace;
        strAsymExp += inLPC->numIterations;
    }
    
    
    vector<TString> patsy(0);
    
    if (!inLPC->useAsymLims) {
        TString cmdCopyPatsyCondor = "cp ";
        vector<TString> vecComArgs_CopyCondor(2);
        vecComArgs_CopyCondor[0] = patsyCondorScript;
        vecComArgs_CopyCondor[1] = nameCondorScript;
        System_GeneralCommand(cmdCopyPatsyCondor, &vecComArgs_CopyCondor, debug);
        vecComArgs_CopyCondor[0] = patsyCondorSubmit;
        vecComArgs_CopyCondor[1] = nameCondorSubmit;
        System_GeneralCommand(cmdCopyPatsyCondor, &vecComArgs_CopyCondor, debug);
    }
    
    
    TString strScriptReplace = fullPathLimitScriptUmbrella + strSpace + fullPathLimitCard + strSpace + fullPathOutLimit + strSpace + strAsymExp + strSpace + fullPathLimitScript;
    
    if (!inLPC->useAsymLims) {
        TString cmdReplacePatsyScript = "sed -i \"s|PATSYLine|" + strScriptReplace + "|\" " + nameCondorScript;
        System_GeneralCommand(cmdReplacePatsyScript, &patsy, debug);
    }
    else {
        System_GeneralCommand(strScriptReplace, &patsy, debug);
    }

    
    if (!inLPC->useAsymLims) {
        TString cmdReplacePatsySubmitBase = "sed -i \"s|Patsy";
        TString cmdReplacePatsySubmitEnd = "|\" " + nameCondorSubmit;
        TString arrPatsySubmit[4] = {"Executable", "Out", "Err", "Log"};
        TString arrPatsySubmitReplace[4] = {nameCondorScript, nameCondorScriptStdOut, nameCondorScriptStdErr, nameCondorScriptLogFile};
        for (int iLine = 0; iLine < 4; ++iLine) {
            System_GeneralCommand(cmdReplacePatsySubmitBase + arrPatsySubmit[iLine] + TString("|") + arrPatsySubmitReplace[iLine] + cmdReplacePatsySubmitEnd, &patsy, debug);
        }
        TString cmdSubmitCondor = "condor_submit " + nameCondorSubmit;
        System_GeneralCommand(cmdSubmitCondor, &patsy, debug);
    }
    /*
    TString cmdFillCondorFile = "echo ";
    vector<TString> vecComArgs(3);
    vecComArgs[1] = " >> ";

    
    TString arrScriptStrings_0[3] = {"#!/bin/sh", "cd /tmp", fullPathLimitScriptUmbrella + strSpace + fullPathLimitCard + strSpace + fullPathOutLimit + strAsymExp + strSpace + fullPathLimitScript};
    vecComArgs[2] = (nameCondorScript);
    
    for (int iLine = 0; iLine < 3; ++iLine) {
        vecComArgs[0] = arrScriptStrings_0[iLine];
        System_GeneralCommand(cmdFillCondorFile, &vecComArgs, true);
    }
    
    TString arrSubmitStrings_0[8] = {"universe = vanilla", "getenv = True", TString("executable = ") + nameCondorScript, TString("output = ") + nameCondorScriptStdOut, TString("error = ") + nameCondorScriptStdErr, TString("log = ") + nameCondorScriptLogFile, "notification=never", "queue"};
    vecComArgs[2] = (nameCondorSubmit);
    for (int iLine = 0; iLine < 8; ++iLine) {
        vecComArgs[0] = arrSubmitStrings_0[iLine];
        System_GeneralCommand(cmdFillCondorFile, &vecComArgs, true);
    }
    
    System_GeneralCommand(cmdAllowExecute, &patsy, true);
    */
}
void RunLimits(LimitParametersContainer * inLPC, SUSYMassLimitParams * inSUSYMLP, SUSYT2LimitParams * inSUSYT2LP, MT2LimitParams * inMT2LP = 0) {
    
    TString strUnder = "_";
    
    TString pathLimCard = OutputPathCard(inLPC, inSUSYMLP, inSUSYT2LP, inMT2LP);
    TString baseNameLimCard = OutputBaseNameCard(inLPC, inSUSYMLP, inSUSYT2LP);
    TString nameLimCard;
    
    TString baseNameOutLimit = OutputBaseNameLimit(inLPC, inSUSYMLP, inSUSYT2LP);
    TString nameOutLimit;
    
    TString nameUmbrellaScript = NameLimitRunUmbrellaScript();
    if (inLPC->isSignificance) {
        nameUmbrellaScript = NameSignifRunUmbrellaScript();
    }
    TString nameLimitScript = NameLimitRunScript();
    
    TString strWhichLimitBase = inLPC->WhichLimitCondorString(inMT2LP);
    TString strWhichLimit;
    
    TString pathCondorLog = OutputLogDirectory(inLPC, inSUSYT2LP);
    
    if (inMT2LP && inLPC->doMT2Scan && inLPC->isExpLim == 1) {
        for (int currLBMT2ll = inMT2LP->minMT2ll; currLBMT2ll <= inMT2LP->maxMT2ll; currLBMT2ll += inMT2LP->bigStepSizeMT2ll) {
            int currUBMT2ll = TMath::Min(inMT2LP->maxMT2ll, currLBMT2ll + (inMT2LP->bigStepSizeMT2ll - inMT2LP->smallStepSizeMT2ll));
            for (int currLBMT2lblb = inMT2LP->minMT2lblb; currLBMT2lblb <= inMT2LP->maxMT2lblb; currLBMT2lblb += inMT2LP->bigStepSizeMT2lblb) {
                int currUBMT2lblb = TMath::Min(inMT2LP->maxMT2lblb, currLBMT2lblb + (inMT2LP->bigStepSizeMT2lblb - inMT2LP->smallStepSizeMT2lblb));
                for (inMT2LP->cutMT2ll = currLBMT2ll; inMT2LP->cutMT2ll <= currUBMT2ll; inMT2LP->cutMT2ll += inMT2LP->smallStepSizeMT2ll) {
                    for (inMT2LP->cutMT2lblb = currLBMT2lblb; inMT2LP->cutMT2lblb <= currUBMT2lblb; inMT2LP->cutMT2lblb += inMT2LP->smallStepSizeMT2lblb) {
                        inMT2LP->SetMT2AppendString(false);
                        
                        strWhichLimit = strWhichLimitBase + inMT2LP->strMT2Append;
                        
                        nameLimCard = pathLimCard + baseNameLimCard + inMT2LP->strMT2Append + TString(".txt");
                        nameOutLimit = pathLimCard + baseNameOutLimit + inMT2LP->strMT2Append + TString(".txt");
                        
                        MakeCondorLimitCalc(nameUmbrellaScript, nameLimitScript, nameLimCard, nameOutLimit, pathCondorLog, strWhichLimit, inLPC, inSUSYMLP, inSUSYT2LP);
                    }
                }
            }
        }
    }
    else {
        strWhichLimit = strWhichLimitBase;
        nameLimCard = pathLimCard + baseNameLimCard;
        nameOutLimit = pathLimCard + baseNameOutLimit;
        MakeCondorLimitCalc(nameUmbrellaScript, nameLimitScript, nameLimCard, nameOutLimit, pathCondorLog, strWhichLimit, inLPC, inSUSYMLP, inSUSYT2LP, false);
    }
}
