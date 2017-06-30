using namespace std;
#include <map>
#include "TString.h"
typedef pair<int, int> massCombo;
typedef pair<massCombo, int> massPoint;
typedef map<TString, TString> labelMap;


typedef map<massCombo, TString> massMap;
#include <iostream>
using namespace std;

#define numT2tt 12
#define numT2bw 6
#define numT2tb 10

//Functions that are used for mapping input mStop, mLSP combinations onto the approriate names for signal files that contain the results

struct SUSYDataset {
    massCombo boundsStop;
    massCombo boundsLSP;
    int indexPol;
    int charFrac;
    TString nameDataSet;
    
    TString StringMassCombo(massCombo * inCombo) {
        TString outString = "";
        outString += inCombo->first;
        outString += "to";
        outString += inCombo->second;
        return outString;
    }
    void SetDatasetNameTightBin(int whichTightBinT2tt) {
        TString baseName = "T2tt_";
        TString endName = "_LeptonFilter";
        TString arrName[3] = {"150to250LSP1to100", "262p5to337p5LSP62p5to187p5", "350to450LSP150to300"};
        nameDataSet = baseName + arrName[whichTightBinT2tt] + endName;
        /*
        Output_Tree_T2tt_150to250LSP1to100_LeptonFilter/
        Output_Tree_T2tt_262p5to337p5LSP62p5to187p5_LeptonFilter/
        Output_Tree_T2tt_350to450LSP150to300_LeptonFilter/
        */
    }
    void SetDatasetName(int whichT2, bool specLSP, bool isLeptFilt) {
        TString baseNameDataSet[3] = {"T2bw_", "T2tt_", "T2tb_"};
        nameDataSet = baseNameDataSet[whichT2];
        nameDataSet += StringMassCombo(&boundsStop);
        nameDataSet += "LSP";
	//cout << "nameDataSet " << nameDataSet << endl;
        if (specLSP) {
            nameDataSet += "1_LSP";
        }
        if (boundsLSP.first == boundsLSP.second && boundsLSP.first == 0) {
            nameDataSet += 1;
        }
        else {
            nameDataSet += StringMassCombo(&boundsLSP);
        }
        if (whichT2 == 0) {
            nameDataSet += "x0.";
            nameDataSet += charFrac;
            //cout << "paramSMS " << paramSMS << endl;
            if (boundsLSP.first == boundsLSP.second && boundsLSP.first == 0 && charFrac == 50) {
                nameDataSet += 0;
            }
        }
        if (isLeptFilt) {
            nameDataSet += "_LeptonFilter";
        }
    }
    
    void SetValuesT2tt(int whichOne) {
        int whichT2 = 1;
        bool specLSP = whichOne >= 10;
        bool isLeptFilt = whichOne <= 1;
        int stopLB[numT2tt] = {100, 225, 150, 150, 375, 500, 500, 500, 675, 675, 825,  925};
        int stopUB[numT2tt] = {200, 350, 475, 350, 475, 800, 650, 650, 800, 800, 900, 1000};
        
        int LSPLB[numT2tt] = {  1,  25, 0,   0,   0, 0,   0, 250,   0, 300,  25,  25};
        int LSPUB[numT2tt] = {100, 250, 0, 250, 375, 0, 225, 550, 275, 700, 800, 900};
        
        boundsStop.first = stopLB[whichOne];
        boundsStop.second = stopUB[whichOne];
        
        boundsLSP.first = LSPLB[whichOne];
        boundsLSP.second = LSPUB[whichOne];
        
        SetDatasetName(whichT2, specLSP, isLeptFilt);
        
        /*
         Output_Tree_T2tt_100to200LSP1to100_LeptonFilter/
         Output_Tree_T2tt_225to350LSP25to250_LeptonFilter/
        Output_Tree_T2tt_150to475LSP1/
        Output_Tree_T2tt_150to350LSP0to250/
        Output_Tree_T2tt_375to475LSP0to375/
        Output_Tree_T2tt_500to800LSP1/
        Output_Tree_T2tt_500to650LSP0to225/
        Output_Tree_T2tt_500to650LSP250to550/
        Output_Tree_T2tt_675to800LSP0to275/
        Output_Tree_T2tt_675to800LSP300to700/
        Output_Tree_T2tt_825to900LSP1_LSP25to800/
        Output_Tree_T2tt_925to1000LSP1_LSP25to900/
        */
    }
    
    void SetValuesT2ttTightBin(int whichOne) {
        SetDatasetNameTightBin(whichOne);
        int stopLB[3] = {150, 262, 350};
        int stopUB[3] = {250, 337, 450};
        
        int LSPLB[3] = {  1,  62, 150};
        int LSPUB[3] = {100, 187, 300};
        boundsStop.first = stopLB[whichOne];
        boundsStop.second = stopUB[whichOne];
        
        boundsLSP.first = LSPLB[whichOne];
        boundsLSP.second = LSPUB[whichOne];
        /*
         Output_Tree_T2tt_150to250LSP1to100_LeptonFilter/
         Output_Tree_T2tt_262p5to337p5LSP62p5to187p5_LeptonFilter/
         Output_Tree_T2tt_350to450LSP150to300_LeptonFilter/
         */
    }
    
    void SetValuesT2bw(int whichOne, int inCharFrac) {
        int whichT2 = 0;
        bool specLSP = false;
        bool isLeptFilt = whichOne <= 1;
        
        int stopLB[numT2bw] = {100, 500, 100, 100, 500, 500};
        int stopUB[numT2bw] = {475, 650, 475, 475, 800, 800};
        int LSPLB[numT2bw] = {  0, 150, 0,   0, 0,   0};
        int LSPUB[numT2bw] = {300, 300, 0, 375, 0, 700};
        
        boundsStop.first = stopLB[whichOne];
        boundsStop.second = stopUB[whichOne];
        
        boundsLSP.first = LSPLB[whichOne];
        boundsLSP.second = LSPUB[whichOne];
        
        charFrac = inCharFrac;
        
        SetDatasetName(whichT2, specLSP, isLeptFilt);
        
        /*
         Output_Tree_T2bw_100to475LSP0to300x0.25_LeptonFilter/
         Output_Tree_T2bw_500to650LSP150to300x0.25_LeptonFilter/
         Output_Tree_T2bw_100to475LSP0to375x0.25/
         Output_Tree_T2bw_100to475LSP1x0.25/
         Output_Tree_T2bw_500to800LSP0to700x0.25/
         Output_Tree_T2bw_500to800LSP1x0.25/
         */
    }
    
    void SetValuesT2tb(int whichOne) {
        
        int whichT2 = 2;
        bool specLSP = false;
        bool isLeptFilt = 0;
        int stopLB[numT2tt] = {100, 400, 500, 600, 700, 775, 825, 900, 975};
        int stopUB[numT2tt] = {375, 475, 575, 675, 750, 800, 875, 950, 1000};
        
        int LSPLB[numT2tt] = {  1,   1,   1,   1,   1,   1,   1,   1,   1};
        int LSPUB[numT2tt] = {275, 375, 475, 575, 650, 700, 775, 850, 900};
        
        boundsStop.first = stopLB[whichOne];
        boundsStop.second = stopUB[whichOne];
        
        boundsLSP.first = LSPLB[whichOne];
        boundsLSP.second = LSPUB[whichOne];
        
        SetDatasetName(whichT2, specLSP, isLeptFilt);
        
        /*
         Output_Tree_T2tb_100to375LSP1to275
         Output_Tree_T2tb_400to475LSP1to375
         Output_Tree_T2tb_500to575LSP1to475
         Output_Tree_T2tb_600to675LSP1to575
         Output_Tree_T2tb_700to750LSP1to650
         Output_Tree_T2tb_775to800LSP1to700
         Output_Tree_T2tb_825to875LSP1to775
         Output_Tree_T2tb_900to950LSP1to850
         Output_Tree_T2tb_975to1000LSP1to900
         */
    }
    
    bool InBounds(massCombo * inMassCombo) {
        int massStop = inMassCombo->first;
        int massLSP = inMassCombo->second;
        
        bool inStop = massStop >= boundsStop.first && massStop <= boundsStop.second;
        
        bool inLSP = false;
        if (massLSP == 0 && boundsLSP.first == 1) {
            //to handle the
            //T2tt_100to200LSP1to100_LeptonFilter dataset
            inLSP = true;
        }
        else if (massLSP == 0 && boundsStop.first >= 825) {
            inLSP = true;
            //to handle the
            //T2tt_825to900LSP1_LSP25to800/
            //T2tt_925to1000LSP1_LSP25to900/
        }
        else {
            inLSP = massLSP >= boundsLSP.first && massLSP <= boundsLSP.second;
        }
        
        return inStop && inLSP;
        return (massStop >= boundsStop.first && inMassCombo->second);
    }
    void SetMassMap(massCombo inMassCombo, massMap &inMap) {
      //cout << "first " << inMassCombo.first << endl;
      //cout << "second " << inMassCombo.second << endl;
      //cout << "nameDataSet " << nameDataSet << endl;
        inMap[inMassCombo] = nameDataSet;
    }
};
/********************************************************************************/
//functions for mapping the a mass point onto a dataset
/********************************************************************************/
void SetMassMapT2tt(massMap &inMap, bool doLeptFilt) {
    int stopLB = 100;
    int stopUB = 1000;
    int LSPLB = 0;
    int LSPUB = 900;
    
    int stepSizeStop = 25;
    int stepSizeLSP = 25;
    
    massCombo currCombo;
    
    vector<SUSYDataset> vecSD(numT2tt);
    
    int startIndex = doLeptFilt ? 0 : 2;
    
    for (unsigned int iT2tt = startIndex; iT2tt < vecSD.size(); ++iT2tt) {
        vecSD[iT2tt].SetValuesT2tt(iT2tt);
    }
    
    for (int massStop = stopLB; massStop <= stopUB; massStop += stepSizeStop) {
        for (int massLSP = LSPLB; massLSP <= TMath::Min(LSPUB, massStop - 100); massLSP += stepSizeLSP) {
            currCombo.first = massStop;
            currCombo.second = massLSP;
            for (unsigned int iT2tt = startIndex; iT2tt < vecSD.size(); ++iT2tt) {
                if (vecSD[iT2tt].InBounds(&currCombo)) {
                    vecSD[iT2tt].SetMassMap(currCombo, inMap);
                    break;
                }
            }
        }
    }
}

void SetMassMapT2ttTightBin(massMap &inMap) {
    int stopLB = 1500;
    int stopUB = 4500;
    int LSPLB = 0;
    int LSPUB = 3000;
    
    int stepSizeStop = 125;
    int stepSizeLSP = 125;
    
    massCombo currCombo;
    
    vector<SUSYDataset> vecSD(3);
    
    int startIndex = 0;
    
    for (unsigned int iT2tt = startIndex; iT2tt < vecSD.size(); ++iT2tt) {
        vecSD[iT2tt].SetValuesT2ttTightBin(iT2tt);
    }
    
    int minMassDiff = 1500, maxMassDiff = 2000;
    
    for (int massStop = stopLB; massStop <= stopUB; massStop += stepSizeStop) {
        for (int massLSP = TMath::Max(LSPLB, massStop - maxMassDiff); massLSP <= TMath::Min(LSPUB, TMath::Max(0, massStop - minMassDiff)); massLSP += stepSizeLSP) {
            currCombo.first = massStop/10;
            currCombo.second = massLSP/10;
            for (unsigned int iT2tt = startIndex; iT2tt < vecSD.size(); ++iT2tt) {
                if (vecSD[iT2tt].InBounds(&currCombo)) {
                    cout << "currCombo.first " << currCombo.first << endl;
                    cout << "currCombo.second " << currCombo.second << endl;
                    cout << "vecSD[iT2tt].nameDataset " << vecSD[iT2tt].nameDataSet << endl;
                    vecSD[iT2tt].SetMassMap(currCombo, inMap);
                    break;
                }
            }
        }
    }
}

void SetMassMapT2bw(massMap &inMap, int inCharFrac, bool doLeptFilt) {
    int stopLB = 100;
    int stopUB = 800;
    int LSPLB = 0;
    int LSPUB = 700;
    
    int stepSizeStop = 25;
    int stepSizeLSP = 25;
    
    massCombo currCombo;
    
    vector<SUSYDataset> vecSD(numT2bw);
    
    int startIndex = doLeptFilt ? 0 : 2;
    
    for (unsigned int iT2bw = startIndex; iT2bw < vecSD.size(); ++iT2bw) {
        vecSD[iT2bw].SetValuesT2bw(iT2bw, inCharFrac);
    }
    
    for (int massStop = stopLB; massStop <= stopUB; massStop += stepSizeStop) {
        for (int massLSP = LSPLB; massLSP <= TMath::Min(LSPUB, massStop - 100); massLSP += stepSizeLSP) {
            currCombo.first = massStop;
            currCombo.second = massLSP;
            for (unsigned int iT2bw = startIndex; iT2bw < vecSD.size(); ++iT2bw) {
                if (vecSD[iT2bw].InBounds(&currCombo)) {
                    vecSD[iT2bw].SetMassMap(currCombo, inMap);
                    break; //this currently stops non-lepton filtered versions of same dataset from being added into map
                }
            }
        }
    }
}

void SetMassMapT2tb(massMap &inMap) {
    int stopLB = 100;
    int stopUB = 1000;
    int LSPLB = 0;
    int LSPUB = 900;
    
    int stepSizeStop = 25;
    int stepSizeLSP = 25;
    
    vector<SUSYDataset> vecSD(numT2tb);
    
    int startIndex = 0; //no lepton filtered version
    
    for (unsigned int iT2tb = startIndex; iT2tb < vecSD.size(); ++iT2tb) {
        vecSD[iT2tb].SetValuesT2tb(iT2tb);
    }
    
    massCombo currCombo;
    for (int massStop = stopLB; massStop <= stopUB; massStop += stepSizeStop) {
        for (int massLSP = LSPLB; massLSP <= TMath::Min(LSPUB, massStop - 100); massLSP += stepSizeLSP) {
            currCombo.first = massStop;
            currCombo.second = massLSP;
            for (unsigned int iT2tb = startIndex; iT2tb < vecSD.size(); ++iT2tb) {
                /*
                cout << "massStop " << massStop << endl;
                cout << "massLSP " << massLSP << endl;
                cout << "iT2tb " << iT2tb << endl;
                cout << "vecSD[iT2tb].InBounds(&currCombo) " << vecSD[iT2tb].InBounds(&currCombo) << endl;
                */
                if (vecSD[iT2tb].InBounds(&currCombo)) {
                    vecSD[iT2tb].SetMassMap(currCombo, inMap);
                    break; //this currently stops non-lepton filtered versions of same dataset from being added into map
                }
            }
        }
    }
    
    /*
     Output_Tree_T2tb_100to375LSP1to275
     Output_Tree_T2tb_400to475LSP1to375
     Output_Tree_T2tb_500to575LSP1to475
     Output_Tree_T2tb_600to675LSP1to575
     Output_Tree_T2tb_700to750LSP1to650
     Output_Tree_T2tb_775to800LSP1to700
     Output_Tree_T2tb_825to875LSP1to775
     Output_Tree_T2tb_900to950LSP1to850
     Output_Tree_T2tb_975to1000LSP1to900
     */
    
}


/********************************************************************************/
//functions for mapping the T2tb dataset onto T2tt and T2bw (for PDF systematics)
/********************************************************************************/
void SetPDFMapT2tb(labelMap &inMap, bool isT2ttMap, bool isLSP0) {
    ///Not being used for now
    
    if (isLSP0) {
        inMap["T2tt_100to200LSP1to100_LeptonFilter"] = "T2tt_150to475LSP1";
        //        inMap["T2tt_225to350LSP25to250_LeptonFilter"] = "T2tt_150to475LSP1";
    }
    else {
        inMap["T2tt_100to200LSP1to100_LeptonFilter"] = "T2tt_150to350LSP0to250";
        inMap["T2tt_225to350LSP25to250_LeptonFilter"] = "T2tt_150to350LSP0to250";
    }
    /*
     Output_Tree_T2tb_100to375LSP1to275
     Output_Tree_T2tb_400to475LSP1to375
     Output_Tree_T2tb_500to575LSP1to475
     Output_Tree_T2tb_600to675LSP1to575
     Output_Tree_T2tb_700to750LSP1to650
     Output_Tree_T2tb_775to800LSP1to700
     Output_Tree_T2tb_825to875LSP1to775
     Output_Tree_T2tb_900to950LSP1to850
     Output_Tree_T2tb_975to1000LSP1to900
     */
    
    /*
     Output_Tree_T2tt_100to200LSP1to100_LeptonFilter/
     Output_Tree_T2tt_225to350LSP25to250_LeptonFilter/
     Output_Tree_T2tt_150to475LSP1/
     Output_Tree_T2tt_150to350LSP0to250/
     Output_Tree_T2tt_375to475LSP0to375/
     Output_Tree_T2tt_500to800LSP1/
     Output_Tree_T2tt_500to650LSP0to225/
     Output_Tree_T2tt_500to650LSP250to550/
     Output_Tree_T2tt_675to800LSP0to275/
     Output_Tree_T2tt_675to800LSP300to700/
     Output_Tree_T2tt_825to900LSP1_LSP25to800/
     Output_Tree_T2tt_925to1000LSP1_LSP25to900/
     */
    
    /*
     Output_Tree_T2bw_100to475LSP0to300x0.25_LeptonFilter/
     Output_Tree_T2bw_500to650LSP150to300x0.25_LeptonFilter/
     Output_Tree_T2bw_100to475LSP0to375x0.25/
     Output_Tree_T2bw_100to475LSP1x0.25/
     Output_Tree_T2bw_500to800LSP0to700x0.25/
     Output_Tree_T2bw_500to800LSP1x0.25/
     */
    
    if (isLSP0) {
        inMap["T2tt_100to200LSP1to100_LeptonFilter"] = "T2tt_150to475LSP1";
        //        inMap["T2tt_225to350LSP25to250_LeptonFilter"] = "T2tt_150to475LSP1";
    }
    else {
        inMap["T2tt_100to200LSP1to100_LeptonFilter"] = "T2tt_150to350LSP0to250";
        inMap["T2tt_225to350LSP25to250_LeptonFilter"] = "T2tt_150to350LSP0to250";
    }
}

/********************************************************************************/
//functions for mapping the Lepton Filtered datasets onto non-lept filtered ones//
/********************************************************************************/
void SetLeptFiltSFMapT2bw(labelMap &inMap, bool isLSP0) {
    if (isLSP0) {
        inMap["T2bw_100to475LSP0to300x0.25_LeptonFilter"] = "T2bw_100to475LSP1x0.25";
        inMap["T2bw_100to475LSP0to300x0.50_LeptonFilter"] = "T2bw_100to475LSP1x0.50";
        inMap["T2bw_100to475LSP0to300x0.75_LeptonFilter"] = "T2bw_100to475LSP1x0.75";
        
//        inMap["T2bw_500to650LSP150to300x0.25_LeptonFilter"] = "T2bw_500to800LSP1x0.25";
//        inMap["T2bw_500to650LSP150to300x0.50_LeptonFilter"] = "T2bw_500to800LSP1x0.50";
//        inMap["T2bw_500to650LSP150to300x0.75_LeptonFilter"] = "T2bw_500to800LSP1x0.75";
    }
    else {
        inMap["T2bw_100to475LSP0to300x0.25_LeptonFilter"] = "T2bw_100to475LSP0to375x0.25";
        inMap["T2bw_100to475LSP0to300x0.50_LeptonFilter"] = "T2bw_100to475LSP0to375x0.5";
        inMap["T2bw_100to475LSP0to300x0.75_LeptonFilter"] = "T2bw_100to475LSP0to375x0.75";
        
        inMap["T2bw_500to650LSP150to300x0.25_LeptonFilter"] = "T2bw_500to800LSP0to700x0.25";
        inMap["T2bw_500to650LSP150to300x0.50_LeptonFilter"] = "T2bw_500to800LSP0to700x0.5";
        inMap["T2bw_500to650LSP150to300x0.75_LeptonFilter"] = "T2bw_500to800LSP0to700x0.75";
    }
}

void SetLeptFiltSFMapT2tt_ver2(labelMap &inMap, bool isLSP0, bool isStop350) {
    if (isLSP0) {
//        inMap["T2tt_2J_mStop-150to250_mLSP-1to100_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola"] = "T2tt_150to475LSP1";
        inMap["T2tt_150to250LSP1to100_LeptonFilter"] = "T2tt_150to475LSP1";
        
        
        
        //        inMap["T2tt_2J_mStop-265p5to337p5_mLSP-62p5to187p5_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola"] = "T2tt_150to475LSP1";
        //        inMap["T2tt_2J_mStop-350to450_mLSP-150to300_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola"] = "T2tt_150to475LSP1";

    }
    else {
        inMap["T2tt_150to250LSP1to100_LeptonFilter"] = "T2tt_150to350LSP0to250";
        inMap["T2tt_262p5to337p5LSP62p5to187p5_LeptonFilter"] = "T2tt_150to350LSP0to250";
        if (isStop350) {
            inMap["T2tt_350to450LSP150to300_LeptonFilter"] = "T2tt_150to350LSP0to250";
        }
        else {
            inMap["T2tt_350to450LSP150to300_LeptonFilter"] = "T2tt_375to475LSP0to375";
        }
        /*
        inMap["T2tt_2J_mStop-150to250_mLSP-1to100_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola"] = "T2tt_150to350LSP0to250";
        inMap["T2tt_2J_mStop-265p5to337p5_mLSP-62p5to187p5_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola"] = "T2tt_150to350LSP0to250";
        if (isStop350) {
            inMap["T2tt_2J_mStop-350to450_mLSP-150to300_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola"] = "T2tt_150to350LSP0to250";
        }
        else {
            inMap["T2tt_2J_mStop-350to450_mLSP-150to300_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola"] = "T2tt_375to475LSP0to375";
        }
         */
    }
}
void SetLeptFiltSFMapT2tt(labelMap &inMap, bool isLSP0) {
    if (isLSP0) {
        inMap["T2tt_100to200LSP1to100_LeptonFilter"] = "T2tt_150to475LSP1";
//        inMap["T2tt_225to350LSP25to250_LeptonFilter"] = "T2tt_150to475LSP1";
    }
    else {
        inMap["T2tt_100to200LSP1to100_LeptonFilter"] = "T2tt_150to350LSP0to250";
        inMap["T2tt_225to350LSP25to250_LeptonFilter"] = "T2tt_150to350LSP0to250";
    }
}

TString LeptFiltDataSetT2bw(int massStop, int massLSP, int inCharFrac, bool doVerb = 0) {
    int numT2bwLeptFilt = 2;
    
    massCombo currCombo(massStop, massLSP);
    
    vector<SUSYDataset> vecSD(numT2bwLeptFilt);
    for (unsigned int iT2bw = 0; iT2bw < vecSD.size(); ++iT2bw) {
        vecSD[iT2bw].SetValuesT2bw(iT2bw, inCharFrac);
        if (vecSD[iT2bw].InBounds(&currCombo)) {
            return vecSD[iT2bw].nameDataSet;
        }
    }
    TString outStringFail = "FAIL";
    return outStringFail;
}

TString LeptFiltDataSetT2tt(int massStop, int massLSP, bool doVerb = 0) {
    int numT2ttLeptFilt = 2;
    massCombo currCombo(massStop, massLSP);
    
    vector<SUSYDataset> vecSD(numT2ttLeptFilt);
    
    for (unsigned int iT2tt = 0; iT2tt < vecSD.size(); ++iT2tt) {
        vecSD[iT2tt].SetValuesT2tt(iT2tt);
        if (doVerb) {
            cout << "iT2tt " << iT2tt << " is in bounds? " << vecSD[iT2tt].InBounds(&currCombo) << endl;
        }
        if (vecSD[iT2tt].InBounds(&currCombo)) {
            return vecSD[iT2tt].nameDataSet;
        }
    }
    TString outStringFail = "FAIL";
    return outStringFail;
}

TString LeptFiltDataSetT2tt_ver2(int massStop, int massLSP) {
    int stopBound1 = 150;
    int stopBound2 = 250;
    int stopBound3 = 338;
    int stopBound4 = 450;
    
    int LSPBound1_LowStop = 0;
    int LSPBound2_LowStop = 100;
    int LSPBound1_MidStop = 62;
    int LSPBound2_MidStop = 188;
    int LSPBound1_HighStop = 150;
    int LSPBound2_HighStop = 300;
    
    TString outStringFail = "FAIL";
    if (massStop >= stopBound1 && massStop <= stopBound2) {
        if (massLSP >= LSPBound1_LowStop && massLSP <= LSPBound2_LowStop) {
            //return TString("T2tt_2J_mStop-150to250_mLSP-1to100_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola");
            return TString("T2tt_150to250LSP1to100_LeptonFilter");
        }
    }
    else if (massStop > stopBound2 && massStop <= stopBound3) {
        if (massLSP >= LSPBound1_MidStop && massLSP <= LSPBound2_MidStop) {
            //return TString("T2tt_2J_mStop-265p5to337p5_mLSP-62p5to187p5_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola");
            return TString("T2tt_262p5to337p5LSP62p5to187p5_LeptonFilter");
        }
    }
    else if (massStop > stopBound3 && massStop <= stopBound4) {
        if (massLSP >= LSPBound1_HighStop && massLSP <= LSPBound2_HighStop) {
            //return TString("T2tt_2J_mStop-350to450_mLSP-150to300_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola");
            return TString("T2tt_350to450LSP150to300_LeptonFilter");
        }
    }
    return outStringFail;
}

/********************************************************************************/
//functions for mapping the Lepton Filtered datasets onto non-lept filtered ones//
/********************************************************************************/
