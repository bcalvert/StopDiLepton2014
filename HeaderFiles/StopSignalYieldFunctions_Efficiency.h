#include "TFile.h"
#include "TAxis.h"
#include "TH1.h"
#include "TString.h"

#include "TDirectory.h"
#include "TKey.h"
#include "TH2F.h"
#include "TCollection.h"
#include <vector>

#include <iostream>

#include "interpolate.h"

using namespace std;

vector<TH2F *> InterpolateCoarseMaps(TFile * inputFile, string dirToRW, int numRebin, TString addName, bool doVerb = 0) {
//void InterpolateCoarseMaps(TFile * inputFile, string dirToRW, int numRebin, TString addName) {
    //Function to take a TFile containing coarse efficiency maps (Central Value plus systematic shifts) and translate them into smoothed efficiency maps
    
    TString path( (char*)strstr( inputFile->GetPath(), ":" ) );
    path.Remove( 0, 2 );
        
    inputFile->cd( path );
    
    vector<TH2F *> outVec(0);
    TH2F * currHistToRebin;
    vector<TH2F *> vecCurrRebinnedHists;
    TString currHistName;
    
    if (doVerb) {
        cout << "running the interpolation " << numRebin << " times";
        cout << "in the " << dirToRW << " direction. " << endl;
    }
    
    TDirectory * current_sourcedir = gDirectory;
    TKey * key;
    TIter nextkey( current_sourcedir->GetListOfKeys() );
    while ( (key = (TKey *) nextkey())) {
        if (doVerb) {
            cout << "making interpolation map for histogram " << key->GetName() << " from the file " << inputFile->GetName() << endl;
        }
        currHistToRebin = (TH2F *) current_sourcedir->Get(key->GetName());
        currHistName = currHistToRebin->GetName();
        vecCurrRebinnedHists.clear();
        vecCurrRebinnedHists.resize(numRebin);
        vecCurrRebinnedHists[0] = rebin(currHistToRebin, dirToRW);
        for (unsigned int iRebin = 1; iRebin < vecCurrRebinnedHists.size(); ++iRebin) {
            if (doVerb) {
                cout << "iRebin " << iRebin << endl;
            }
            vecCurrRebinnedHists[iRebin] = rebin(vecCurrRebinnedHists[iRebin - 1], dirToRW);
        }
        vecCurrRebinnedHists[vecCurrRebinnedHists.size() - 1]->SetName(currHistName + addName);
        outVec.push_back(vecCurrRebinnedHists[vecCurrRebinnedHists.size() - 1]);
        if (doVerb) {
            cout << "vecCurrRebinnedHists[vecCurrRebinnedHists.size() - 1] Name " << vecCurrRebinnedHists[vecCurrRebinnedHists.size() - 1]->GetName() << endl;
            cout << "curr outVec size " << outVec.size() << endl;
        }
    }
    return outVec;
}

void SetSystStringVec(vector<TString> * inVecSystString) {
//    TString nameGrabSysts[numGrabSysts] = {"LepEffSF", "LepES", "JetES", "BTagEffSF", "BMisTagSF", "UncES", "JetSmear", "genRecoilRW"};
    
}


float ScaleBackCalcBasic2(vector<TFile *> * inVecInputFiles, bool doVerb = false, TString inSystName = "") {
    //Function used to correct for the spurious statistical scaling that occurs when using event-by-event weights whose average is not 1
    //For example, the generator-level recoil reweighting of the ttbar, signal samples, and (sometimes) DY Madgraph samples very explicitly
    //has weights that are <= 1.0, thus, without correcting for this effect you effectively lower the cross-section of these dudes
    
    //The nominal way to correct for this effect is to have a histogram where said effect has not been accounted for at all, i.e. no reweighting has happened
    //and have a direct comparison (direct in the sense of being at the same selection level) to a histogram where the weighting has been applied
    
    //You have to be careful however, because these reweightings *are* trying to correct for some physical effect and if you go too deep into your selection
    //you could ostensibly just ruin the whole reweighting any by correcting back to original weights
    //Thus, the trick is to pick a point in the selection where you do *not* expect the physical effect the reweighting accounts for to have a notable effect
    //For the dilepton analysis, this is an *extremely* basic dilepton selection -- two good, isolated, opposite charge leptons that have an invariant mass > 20 GeV
    
    unsigned int numInputFiles = inVecInputFiles->size();
    
    TString mcplot = "h_nVtx_inclusive";
    TString mcplot_preRW = "h_BasicWeightIntegral_inclusive";
    
    vector<TString> vecFileNames(numInputFiles);
    for (unsigned int iFile = 0; iFile < numInputFiles; ++iFile) {
        vecFileNames[iFile] = inVecInputFiles->at(iFile)->GetName();
        if (doVerb) {
            cout << "going to be trying to grab from file " << vecFileNames[iFile] << endl;
        }
    }
    TString badSysts[2] = {"JetSmear", "UncES"};
    
    bool isBadSyst = false;
    for (int iBadSyst = 0; iBadSyst < 3; ++iBadSyst) {
        if (inSystName.Contains(badSysts[iBadSyst])) {
            isBadSyst = true;
        }
    }
    if (!isBadSyst) {
        mcplot += inSystName;
        mcplot_preRW += inSystName;
    }
    if (doVerb) {
        cout << "mcplot to grab " << mcplot << endl;
        cout << "mcplot_preRW to grab " << mcplot_preRW << endl;
    }
    
    TH1F * nVtxOrigHist, * nVtxNewHist;
    
    int NBinsX, NBinsXNew;
    float scaleBack;
    
    int minIndex = 0;
    int maxIndex = (int) numInputFiles;
    nVtxNewHist = (TH1F *) LoadMultipleHistogram(inVecInputFiles, mcplot, minIndex, maxIndex, "_Comp");
    nVtxOrigHist = (TH1F *) LoadMultipleHistogram(inVecInputFiles, mcplot_preRW, minIndex, maxIndex, "_Comp");

    NBinsX = nVtxOrigHist->GetNbinsX();
    NBinsXNew = nVtxNewHist->GetNbinsX();
    if (nVtxNewHist->Integral(1, NBinsXNew + 1) < 1E-3) {
        scaleBack = 0;
    }
    else {
        scaleBack = (float) nVtxOrigHist->Integral(1, NBinsX + 1) / nVtxNewHist->Integral(1, NBinsXNew + 1);
    }
    return scaleBack;
}

float ScaleBackCalcBasic2(TFile * inputFile, bool doVerb = false, TString inSystName = "") {
    //Function used to correct for the spurious statistical scaling that occurs when using event-by-event weights whose average is not 1
    //For example, the generator-level recoil reweighting of the ttbar, signal samples, and (sometimes) DY Madgraph samples very explicitly
    //has weights that are <= 1.0, thus, without correcting for this effect you effectively lower the cross-section of these dudes
    
    //The nominal way to correct for this effect is to have a histogram where said effect has not been accounted for at all, i.e. no reweighting has happened
    //and have a direct comparison (direct in the sense of being at the same selection level) to a histogram where the weighting has been applied
    
    //You have to be careful however, because these reweightings *are* trying to correct for some physical effect and if you go too deep into your selection
    //you could ostensibly just ruin the whole reweighting any by correcting back to original weights
    //Thus, the trick is to pick a point in the selection where you do *not* expect the physical effect the reweighting accounts for to have a notable effect
    //For the dilepton analysis, this is an *extremely* basic dilepton selection -- two good, isolated, opposite charge leptons that have an invariant mass > 20 GeV
    
    TString mcplot = "h_nVtx_inclusive";
    TString mcplot_preRW = "h_BasicWeightIntegral_inclusive";
    
    TString fileName = inputFile->GetName();
    TString badSysts[2] = {"JetSmear", "UncES"};

    bool isBadSyst = false;
    for (int iBadSyst = 0; iBadSyst < 3; ++iBadSyst) {
        if (inSystName.Contains(badSysts[iBadSyst])) {
            isBadSyst = true;
        }
    }
    if (!isBadSyst) {
        mcplot += inSystName;
        mcplot_preRW += inSystName;
    }
    if (doVerb) {
        cout << "from file " << fileName << endl;
        cout << "mcplot to grab " << mcplot << endl;
        cout << "mcplot_preRW to grab " << mcplot_preRW << endl;
    }
    TH1F * nVtxOrigHist;
    TH1F * nVtxNewHist;
    int NBinsX, NBinsXNew;
    float scaleBack;
    nVtxOrigHist = (TH1F*) inputFile->Get(mcplot_preRW);
    nVtxNewHist = (TH1F*) inputFile->Get(mcplot);
    NBinsX = nVtxOrigHist->GetNbinsX();
    NBinsXNew = nVtxNewHist->GetNbinsX();
    if (nVtxNewHist->Integral(1, NBinsXNew + 1) < 1E-3) {
        scaleBack = 0;
    }
    else {
        scaleBack = (float) nVtxOrigHist->Integral(1, NBinsX + 1) / nVtxNewHist->Integral(1, NBinsXNew + 1);
    }
    return scaleBack;
}


float SignalSkimEfficiencyCalc_SinglePoint(TFile * inFile, int massStop, int massLSP, float paramSMS, int numDims, bool doVerb = false) {
    //For a given MC sample, one always needs to divide one's final "yield" by the total amount of MC events run
    //For the background MC samples, this is straightforward, as one uses the whole sample to account for the expected contribution
    //and so one can just find the number of events run in the whole sample on DAS or something similar
    
    //For the signal samples, however, where multiple mass points might be run in the same production, it's not correct to just divide by
    //the overall number of total events in the sample -- instead, one must find the total number of events run at the specific mass point in question
    //For the dilepton stop analysis, Javier handled this and gave me histograms containing this information
    
    //This function takes as input a given stop mass and LSP mass, and possibly chargino fraction, and finds/calculates the total number of events run at that point
    //(using the histograms from Javier). It then returns a SF that is 1.0 / this number
    TH1 * inNormHist;
    
    TString histGrabString;
    histGrabString = "h_SUSY";
    histGrabString += numDims;
    histGrabString += "DMassEventCount";
    inNormHist = (TH1 *) inFile->Get(histGrabString);
    
    TString fileName = (TString) inFile->GetName();
    
    TAxis * xAxis, * yAxis, * zAxis;
    int xBin = -1, yBin = -1, zBin = -1;
    
    float estOrigNum;
    xAxis = inNormHist->GetXaxis();
    xBin = xAxis->FindBin(massStop);
    
    yAxis = inNormHist->GetYaxis();
    yBin = yAxis->FindBin(massLSP);
    if (numDims > 2) {
        zAxis = inNormHist->GetZaxis();
        zBin = zAxis->FindBin(paramSMS);
    }
    else {
        zBin = 1;
    }
    if (xBin < 1 || xBin > xAxis->GetNbins() || yBin < 1 || yBin > yAxis->GetNbins()) {
        if (numDims > 2) {
            if (zBin < 1 || zBin > zAxis->GetNbins()) {
                estOrigNum = 0;
                cout << "issue with bins " << endl;
                cout << "xAxis->GetNbins() " << xAxis->GetNbins() << endl;
                cout << "yAxis->GetNbins() " << yAxis->GetNbins() << endl;
                cout << "zAxis->GetNbins() " << zAxis->GetNbins() << endl;
                cout << "massStop " << massStop << " => xBin = " << xBin << endl;
                cout << "massLSP " << massLSP << " => yBin = " << yBin << endl;
                cout << "paramSMS " << paramSMS << " => zBin = " << zBin << endl;
            }
        }
    }
    else {
        estOrigNum = inNormHist->GetBinContent(xBin, yBin, zBin);
    }
    if (doVerb) {
        cout << "xBin " << xBin << endl;
        cout << "yBin " << yBin << endl;
        cout << "zBin " << zBin << endl;
        cout << "fileName " << fileName << endl;
        cout << "estOrigNum " << estOrigNum << endl;
    }
    float SF;
    if (estOrigNum <= 1E-3) {
        SF = 0;
        //SF = 1.0 / 1E-3;
    }
    else {
        SF = 1.0 / estOrigNum;
    }
//    if (fileName.Contains("Filter")) SF /= 0.53;
    if (doVerb) {
        cout << "SF = 1.0 / estOrigNum " << SF << endl;
    }
    
    return SF;
}

TString FileName(int isT2tt, int paramSMS) {
    //Not sure this function is used anywhere...
    TString outName = "h_LeptFiltEffSF";
    if (isT2tt) {
        outName += "_T2tt_PerPol";
    }
    else {
        outName += "_T2bwx0.";
    }
    outName += paramSMS;
    outName += ".root";
    return outName;
}