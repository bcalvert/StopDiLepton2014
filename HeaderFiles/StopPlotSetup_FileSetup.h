/// Functions and typedefs used as part of setting up the stop input files

using namespace std;
typedef pair<int, int> intBounds;
typedef pair<TString, intBounds> indMCParams;

void StopBaseFileNames_Data(vector<TString> * vecBaseFileNames_Data, SampLoadSettings * inSLS) {
    // sets up the input data files
    TString baseName = "Data";
    TString endName = inSLS->doReReco ? "_ReReco" : "";
    TString diLep[3] = {"MuMu", "EMu", "EE"};
    for (int iDiLep = 0; iDiLep < 3; ++iDiLep) {
        vecBaseFileNames_Data->push_back(baseName + diLep[iDiLep] + endName);
    }
}
void StopBaseFileNames_MC(vector<TString> * vecBaseFileNames_MC, SampLoadSettings * inSLS) {
    // sets up the input MC SM background files
//    const int numOviMC = 14;
//    TString fileInMCBaseNameSpecOviedo[numOviMC] = {"TTBarComp", "WW", "WZ", "ZZ", "SingleTop", "WToLNu", "ZDY", "WG", "ZG", "HiggsWW", "HiggsVBF", "HiggsZZ4L", "TripVecBoson", "TTBarVecBoson"};
    
    const int numOviMC = 16;
    TString fileInMCBaseNameSpecOviedo[numOviMC] = {"TTBarComp", "WW", "WZ", "ZZ", "SingleTop", "WToLNu", "ZDY_MuMu", "ZDY_EE", "ZDY_EMu", "WG", "ZG", "HiggsWW", "HiggsVBF", "HiggsZZ4L", "TripVecBoson", "TTBarVecBoson"};
    for (int iIndMC = 0; iIndMC < numOviMC; ++iIndMC) {
        vecBaseFileNames_MC->push_back(fileInMCBaseNameSpecOviedo[iIndMC]);
    }
}

void StopBaseFileNames_Signal(vector<TString> * vecBaseFileNames_Signal, SampLoadSettings * inSLS) {
    // sets up the input MC SIgnal files
    for (unsigned int iSig = 0; iSig < inSLS->vecStopMassGrab.size(); ++iSig) {
        vecBaseFileNames_Signal->push_back(inSLS->SignalString(iSig));
    }
}

void SampleStartPositionsNames(vector<indMCParams> * vecIndMCParams, SampLoadSettings * inSLS) {
    // Dictates the bounds to determine which individual MC files get grouped into what composite backgrounds
    // along with their resultan names
    TString TTbarName       = "_TTBar";
    TString VVName          = "_VV";
    TString SingTopName     = "_SingTop";
    TString WJName          = "_WJ";
    TString ZDYName         = "_ZDY";
    TString QCDName         = "_QCD";
    TString VGName          = "_VG";
    TString HiggsName       = "_Higgs";
    TString RareName        = "_Rare";    
    int numTTbar = (inSLS->doExcSamps && inSLS->whichTTbarGen == 0) ? 2 : 1;
    int numVV               = 3;
    int numSingTop          = 1;
    int numWJ               = 1;
//    int numZDY              = 1;
    int numZDY              = 3;
    //    int numQCD              = 1;
    int numVG               = 2;
    int numHiggs            = 3;
    int numRare             = 2;
    
    int TTbarStart          = 0;
    int VVStart             = TTbarStart + numTTbar;
    int SingTopStart        = VVStart + numVV;
    int WJStart             = SingTopStart + numSingTop;
    int ZDYStart            = WJStart + numWJ;
    //    int QCDStart            = ZDYStart + numZDY;
    int VGStart             = ZDYStart + numZDY;
    int HiggsStart          = VGStart + numVG;
    int RareStart           = HiggsStart + numHiggs;
    
    intBounds iB_TTbar (TTbarStart, TTbarStart + numTTbar - 1);
    indMCParams iMCP_TTbar (TTbarName, iB_TTbar);
    vecIndMCParams->push_back(iMCP_TTbar);
    
    intBounds iB_VV (VVStart, VVStart + numVV - 1);
    indMCParams iMCP_VV (VVName, iB_VV);
    vecIndMCParams->push_back(iMCP_VV);
    
    intBounds iB_SingTop (SingTopStart, SingTopStart + numSingTop - 1);
    indMCParams iMCP_SingTop (SingTopName, iB_SingTop);
    vecIndMCParams->push_back(iMCP_SingTop);
    
    intBounds iB_WJ (WJStart, WJStart + numWJ - 1);
    indMCParams iMCP_WJ (WJName, iB_WJ);
    vecIndMCParams->push_back(iMCP_WJ);
    
    intBounds iB_ZDY (ZDYStart, ZDYStart + numZDY - 1);
    indMCParams iMCP_ZDY (ZDYName, iB_ZDY);
    vecIndMCParams->push_back(iMCP_ZDY);
    
    intBounds iB_VG (VGStart, VGStart + numVG - 1);
    indMCParams iMCP_VG (VGName, iB_VG);
    vecIndMCParams->push_back(iMCP_VG);
    
    intBounds iB_Higgs (HiggsStart, HiggsStart + numHiggs - 1);
    indMCParams iMCP_Higgs (HiggsName, iB_Higgs);
    vecIndMCParams->push_back(iMCP_Higgs);
    
    intBounds iB_Rare (RareStart, RareStart + numRare - 1);
    indMCParams iMCP_Rare (RareName, iB_Rare);
    vecIndMCParams->push_back(iMCP_Rare);
}
