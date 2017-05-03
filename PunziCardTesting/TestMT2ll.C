#include "TRint.h"
#include <algorithm>    // std::next_permutation, std::sort

#include "TMath.h"
#include "TString.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include <iostream>     // std::cout
#include <fstream>     // std::cout
#include <sstream>     // std::cout
#include <map>
#include "TGraphErrors.h"
using namespace std;

#include "../HeaderFiles/BasicFunctions_SystemCommands.h"
#include "./PunziTestingFunctions.h"

typedef map<TString, float> mapStrToF;

using namespace std;

int main( int argc, char* argv[]) {
    
    TRint theApp("App", &argc, argv);
    Bool_t retVal = kTRUE;
    
    int numBins = 1;
    bool doVerb = 0;
    
    bool runCards = false;
    int whichVersSigEff = 0;
    int whichVersPunzi = 0;
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"doVerb", 6) == 0) {
            doVerb = 1;
        }
        else if (strncmp (argv[k],"wSig", 4) == 0) {
            whichVersSigEff = strtol(argv[k + 1], NULL, 10 );
        }
        else if (strncmp (argv[k],"wPunzi", 6) == 0) {
            whichVersPunzi = strtol(argv[k + 1], NULL, 10 );
        }
        else if (strncmp (argv[k],"doRun", 5) == 0) {
            runCards = true;
        }
    }
    
    vector<int> vecMT2{80, 90, 100, 110, 120};
    
    vector<TString> vecNames{"T2tt_Stop150_LSP0", "T2tt_Stop300_LSP0", "T2tt_Stop450_LSP0", "T2bw0p75_Stop200_LSP0", "T2bw0p75_Stop300_LSP0", "T2bw0p75_Stop400_LSP0", "T2bw0p50_Stop200_LSP0", "T2bw0p50_Stop300_LSP0", "T2bw0p50_Stop400_LSP0", "T2bw0p25_Stop425_LSP0", "T2bw0p25_Stop525_LSP0", "T2bw0p25_Stop625_LSP0"};
    vector<int> vecBestIndex{1, 3, 4, 2, 3, 4, 3, 3, 3, 2, 3, 3};
    vector<float> vecEff_T2tt_Stop150_LSP0{5.674e-02, 1.787e-02, 5.966e-03, 7.721e-04, 3.181e-05};
    vector<float> vecEff_T2tt_Stop300_LSP0{2.049e-01, 1.503e-01, 1.0559e-01, 7.850e-02, 4.759e-02};
    vector<float> vecEff_T2tt_Stop450_LSP0{4.17300557500293101e-01, 3.58500975657773058e-01, 3.07591537114842106e-01, 2.67828708890884115e-01, 2.23670155672057391e-01};
    
    vector<float> vecEff_T2bwx0p75_Stop200_LSP0{1.23399109696286924e-01, 7.32405412410930545e-02, 3.89526732175813212e-02, 1.86375708812429562e-02, 5.90914150594668355e-03};
    vector<float> vecEff_T2bwx0p75_Stop300_LSP0{2.68049070394919875e-01, 2.21370417769826577e-01, 1.68004902418298740e-01, 1.33163155875258482e-01, 9.23167471642696719e-02};
    vector<float> vecEff_T2bwx0p75_Stop400_LSP0{3.78178359208165449e-01, 3.33489391125844969e-01, 2.83960809408822346e-01, 2.50991358237817042e-01, 2.12761631988870492e-01};

    vector<float> vecEff_T2bwx0p50_Stop200_LSP0{4.05803362902957265e-02, 1.02937675929372988e-02, 4.55840691290286450e-03, 2.41281763746800356e-03, 1.74057674420337434e-06};
    vector<float> vecEff_T2bwx0p50_Stop300_LSP0{1.23620597426791365e-01, 7.58043332579213802e-02, 4.36866593470901995e-02, 2.47550512108268388e-02, 7.88526884913369822e-03};
    vector<float> vecEff_T2bwx0p50_Stop400_LSP0{1.76476933479622344e-01, 1.35148299614681888e-01, 1.02146817803045090e-01, 7.02403372438116930e-02, 4.59168452172134534e-02};
    
    vector<float> vecEff_T2bwx0p25_Stop425_LSP0{3.89408242722717596e-02, 1.93324288491549048e-02, 1.25998350023091132e-02, 3.25188810352506534e-03, 3.82228516043644150e-04};
    vector<float> vecEff_T2bwx0p25_Stop525_LSP0{5.64028317272521063e-02, 4.00373465595420808e-02, 1.84878484254679398e-02, 1.05665797804345100e-02, 6.48639171351130146e-03};
    vector<float> vecEff_T2bwx0p25_Stop625_LSP0{9.24410693727265864e-02, 6.83165356402384571e-02, 4.57000179319797042e-02, 2.50738199846077606e-02, 1.10434195977742091e-02};
    
    vector< vector<float> > vecSigEffs;
    vecSigEffs.push_back(vecEff_T2tt_Stop150_LSP0);
    vecSigEffs.push_back(vecEff_T2tt_Stop300_LSP0);
    vecSigEffs.push_back(vecEff_T2tt_Stop450_LSP0);
    vecSigEffs.push_back(vecEff_T2bwx0p75_Stop200_LSP0);
    vecSigEffs.push_back(vecEff_T2bwx0p75_Stop300_LSP0);
    vecSigEffs.push_back(vecEff_T2bwx0p75_Stop400_LSP0);
    vecSigEffs.push_back(vecEff_T2bwx0p50_Stop200_LSP0);
    vecSigEffs.push_back(vecEff_T2bwx0p50_Stop300_LSP0);
    vecSigEffs.push_back(vecEff_T2bwx0p50_Stop400_LSP0);
    vecSigEffs.push_back(vecEff_T2bwx0p25_Stop425_LSP0);
    vecSigEffs.push_back(vecEff_T2bwx0p25_Stop525_LSP0);
    vecSigEffs.push_back(vecEff_T2bwx0p25_Stop625_LSP0);
    
    vector<float> vecBKG{1527.9856 + 153.9984, 349.0709 + 63.4164, 72.5862 + 29.1847, 14.9509 + 17.0336, 3.3315 + 11.6081};
    PunziCheck pc_OneBin;
    pc_OneBin.DefaultVarVals();
    
    pc_OneBin.nSigmaA = 5;
    pc_OneBin.nSigmaB = 2;
    
    if (whichVersPunzi) {
        pc_OneBin.nSigmaA = 2;
        pc_OneBin.nSigmaB = 2;
    }
    
    
    float currBestPunzi = 0;
    int currBestIndex = -1;
    float bestPunziFromLimit;
    for (int iSig = 0; iSig < 12; ++iSig) {
        currBestPunzi = 0;
        currBestIndex = -1;
        for (int iCheck = 0; iCheck < 5; ++iCheck) {
            pc_OneBin.sigEff = vecSigEffs[iSig][iCheck];
            pc_OneBin.backYield = vecBKG[iCheck];
            pc_OneBin.SetPunziParam();
            if (pc_OneBin.punziParam > currBestPunzi) {
                currBestPunzi = pc_OneBin.punziParam;
                currBestIndex = iCheck;
            }
            if (iCheck == vecBestIndex[iSig]) {
                bestPunziFromLimit = pc_OneBin.punziParam;
            }

        }
        cout << "for sig " << vecNames[iSig] << " best Punzi " << currBestPunzi << " occurred at MT2ll = " << vecMT2[currBestIndex] << endl;
        cout << "for sig " << vecNames[iSig] << " Punzi at optimal MT2ll: " << vecMT2[vecBestIndex[iSig]] << " was " << bestPunziFromLimit << endl;
    }
    
    theApp.Run(retVal);
}