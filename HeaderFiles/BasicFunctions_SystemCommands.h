#include "TString.h"
#include <sstream>
#include <vector>

void CallExecV(vector<const char *> * vecComArgs) {
    //Broken, just ignore
    pid_t pid;
    if((pid = fork()) < 0) {
    }
    else if(pid == 0) {
        execv(vecComArgs->at(0), (char * const *)&vecComArgs->front());
    }
    else {
    }
}

void System_RemoveFile(TString fullPathFileToDel, bool debugCommand = false) {
    //Calls the system command to remove the file located at fullPathFileToDel
    stringstream cmdRMFile;
    cmdRMFile << "rm -rf " << fullPathFileToDel << endl;
    if (debugCommand) {
        cout << "going to run " << cmdRMFile.str().c_str() << endl;
    }
    else {
        system(cmdRMFile.str().c_str());
    }
}

void System_MakeDir(TString fullPathDirToMake, bool debugCommand = false) {
    //Calls the system command to mkdir with the full path fullPathDirToMake
    stringstream cmdMakeDir;
    cmdMakeDir << "mkdir -p " << fullPathDirToMake << endl;
    if (debugCommand) {
        cout << "going to run " << cmdMakeDir.str().c_str() << endl;
    }
    else {
        system(cmdMakeDir.str().c_str());
    }
}

void System_PrintToFile(TString printString, TString fullPathFileToPrintTo, bool debugCommand = false) {
    //Calls the system command to print the string printString into the file located at fullPathFileToPrintTo
    stringstream cmdPrintString;
    cmdPrintString << "echo " << printString << " >> " << fullPathFileToPrintTo << endl;
    if (debugCommand) {
        cout << "going to run " << cmdPrintString.str().c_str() << endl;
    }
    else {
        system(cmdPrintString.str().c_str());
    }
}

void System_CopyFile(TString fileNameToCopyFrom, TString fileNameToCopyTo, bool debugCommand = false) {
    //Calls the system command to copy the file fileNameToCopyFrom to the file located at fileNameToCopyTo
    stringstream cmdCopyFile;
    cmdCopyFile << "cp " << fileNameToCopyFrom << " " << fileNameToCopyTo << endl;
    if (debugCommand) {
        cout << "going to run " << cmdCopyFile.str().c_str() << endl;
    }
    else {
        system(cmdCopyFile.str().c_str());
    }
}

void System_SedInFile(TString fileName, TString stringToReplace, TString stringToReplaceWith, bool debugCommand = false) {
    //Calls the system command to use sed inside the file fileName to replace the string stringToReplace with the string stringToReplaceWith
    stringstream cmdSedInFile;
    cmdSedInFile << "sed -i 's|" << stringToReplace << "|" << stringToReplaceWith << "|' " << fileName << endl;
    if (debugCommand) {
        cout << "going to run " << cmdSedInFile.str().c_str() << endl;
    }
    else {
        system(cmdSedInFile.str().c_str());
    }
}

void System_GeneralCommand(TString strCommand, vector<TString> * inVecCommandArguments, bool debugCommand = false) {
    //Function to run a general command -- contained within strCommand --
    //that has a number of command line arguments -- contained within inVecCommandArguments.
    //The assumption is that each command line argument in
    //inVecCommandArguments needs a space in between
    
    TString strSpace = " ";
    
    stringstream cmdGenCommand;
    cmdGenCommand << strCommand << strSpace;
    
    for (unsigned int iComArg = 0; iComArg < inVecCommandArguments->size(); ++iComArg) {
        cmdGenCommand << inVecCommandArguments->at(iComArg) << strSpace;
    }
    cmdGenCommand << endl;
    if (debugCommand) {
        cout << "going to run " << cmdGenCommand.str().c_str() << endl;
    }
    else {
        system(cmdGenCommand.str().c_str());
    }
}


//Note that "execv" is *WAYYY* faster than system calls for rapid-fire stuff, but the issue here is that I'm running out of PIDs or something to that effect,
/* examples from a stderr of a condorjob
 sed: can't read /data/users/bcalvert/StopDiLepton2014/LimitSetting/T2tt/PerPol100/Stop175/LSP0//MT2SpaceScan/LimitCard_DiLep_BaseFullSel_NoSigContam_CC_CutAnd_T2tt_PerPol100_Stop175_LSP0_MT2ll110_MT2lblb0.txt: No\
 such file or directory
 sed: can't read /data/users/bcalvert/StopDiLepton2014/LimitSetting/T2tt/PerPol100/Stop175/LSP50//MT2SpaceScan/LimitCard_DiLep_BaseFullSel_NoSigContam_CC_CutAnd_T2tt_PerPol100_Stop175_LSP50_MT2ll80_MT2lblb0.txt: N\
 o such file or directory
 sed: can't read /data/users/bcalvert/StopDiLepton2014/LimitSetting/T2tt/PerPol100/Stop225/LSP75//MT2SpaceScan/LimitCard_DiLep_BaseFullSel_NoSigContam_CC_CutAnd_T2tt_PerPol100_Stop225_LSP75_MT2ll80_MT2lblb0.txt: N\
 o such file or directory
 sed: can't read /data/users/bcalvert/StopDiLepton2014/LimitSetting/T2tt/PerPol100/Stop200/LSP100//MT2SpaceScan/LimitCard_DiLep_BaseFullSel_NoSigContam_CC_CutAnd_T2tt_PerPol100_Stop200_LSP100_MT2ll80_MT2lblb0.txt:\
 No such file or directory
 sed: ck_follow_symlink: couldn't lstat /data/users/bcalvert/StopDiLepton2014/LimitSetting/T2tt/PerPol100/Stop225/LSP100//MT2SpaceScan/LimitCard_DiLep_BaseFullSel_NoSigContam_CC_CutAnd_T2tt_PerPol100_Stop225_LSP10\
 0_MT2ll90_MT2lblb0.txt: No such file or directory
 sed: ck_follow_symlink: couldn't lstat /data/users/bcalvert/StopDiLepton2014/LimitSetting/T2tt/PerPol100/Stop225/LSP75//MT2SpaceScan/LimitCard_DiLep_BaseFullSel_NoSigContam_CC_CutAnd_T2tt_PerPol100_Stop225_LSP75_\
 MT2ll110_MT2lblb0.txt: No such file or directory
 sed: ck_follow_symlink: couldn't lstat /data/users/bcalvert/StopDiLepton2014/LimitSetting/T2tt/PerPol100/Stop225/LSP75//MT2SpaceScan/LimitCard_DiLep_BaseFullSel_NoSigContam_CC_CutAnd_T2tt_PerPol100_Stop225_LSP75_\
 MT2ll120_MT2lblb0.txt: No such file or directory
 sed: ck_follow_symlink: couldn't lstat /data/users/bcalvert/StopDiLepton2014/LimitSetting/T2tt/PerPol100/Stop225/LSP0//MT2SpaceScan/LimitCard_DiLep_BaseFullSel_NoSigContam_CC_CutAnd_T2tt_PerPol100_Stop225_LSP0_MT\
 2ll80_MT2lblb0.txt: No such file or directory
 sed: can't read /data/users/bcalvert/StopDiLepton2014/LimitSetting/T2tt/PerPol100/Stop225/LSP0//MT2SpaceScan/LimitCard_DiLep_BaseFullSel_NoSigContam_CC_CutAnd_T2tt_PerPol100_Stop225_LSP0_MT2ll80_MT2lblb0.txt: No \
 such file or directory
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 ./makeCard_CC.sh: fork: Resource temporarily unavailable
 
 */

/*
void System_RemoveFile(TString fullPathFileToDel, bool debugCommand = false) {
    vector<const char *> vecExecVArgs;
    vecExecVArgs.push_back("/bin/rm");
    vecExecVArgs.push_back("-rf");
    vecExecVArgs.push_back(fullPathFileToDel.Data());
    vecExecVArgs.push_back(0);
    
    if (debugCommand) {
        stringstream cmdRMFile;
        cmdRMFile << "rm -rf " << fullPathFileToDel << endl;
        cout << "going to run " << cmdRMFile.str().c_str() << endl;
    }
    else {
        CallExecV(&vecExecVArgs);
    }
}

void System_MakeDir(TString fullPathDirToMake, bool debugCommand = false) {
    vector<const char *> vecExecVArgs;
    vecExecVArgs.push_back("mkdir");
    vecExecVArgs.push_back("-p");
    vecExecVArgs.push_back(fullPathDirToMake.Data());
    vecExecVArgs.push_back(0);
    
    if (debugCommand) {
        stringstream cmdMakeDir;
        cmdMakeDir << "mkdir -p " << fullPathDirToMake << endl;
        cout << "going to run " << cmdMakeDir.str().c_str() << endl;
    }
    else {
        CallExecV(&vecExecVArgs);
    }
}

void System_PrintToFile(TString printString, TString fullPathFileToPrintTo, bool debugCommand = false) {
    vector<const char *> vecExecVArgs;
    vecExecVArgs.push_back("echo");
    vecExecVArgs.push_back(printString.Data());
    vecExecVArgs.push_back(" >> ");
    vecExecVArgs.push_back(fullPathFileToPrintTo.Data());
    vecExecVArgs.push_back(0);
    if (debugCommand) {
        stringstream cmdPrintString;
        cmdPrintString << "echo " << printString << " >> " << fullPathFileToPrintTo << endl;
        cout << "going to run " << cmdPrintString.str().c_str() << endl;
    }
    else {
        CallExecV(&vecExecVArgs);
    }
}

void System_GeneralCommand(TString strCommand, vector<TString> * inVecCommandArguments, bool debugCommand = false) {
    //Function to run a general command -- contained within strCommand --
    //that has a number of command line arguments -- contained within inVecCommandArguments.
    //The assumption is that each command line argument in
    //inVecCommandArguments needs a space in between
    vector<const char *> vecExecVArgs;
    vecExecVArgs.push_back(strCommand.Data());
    for (unsigned int iComArg = 0; iComArg < inVecCommandArguments->size(); ++iComArg) {
        vecExecVArgs.push_back(inVecCommandArguments->at(iComArg).Data());
    }
    vecExecVArgs.push_back(0);
    
    if (debugCommand) {
        TString strSpace = " ";
        stringstream cmdGenCommand;
        cmdGenCommand << strCommand << strSpace;
        for (unsigned int iComArg = 0; iComArg < inVecCommandArguments->size(); ++iComArg) {
            cmdGenCommand << inVecCommandArguments->at(iComArg) << strSpace;
        }
        cmdGenCommand << endl;
        cout << "going to run " << cmdGenCommand.str().c_str() << endl;
    }
    else {
        CallExecV(&vecExecVArgs);
    }
}
*/