#include <iostream>
#include <fstream>
//#include <vector>
#include <cmath>
#include <sstream>
#include <map>
using namespace std;

#include "TH2F.h"

inline TH2F * CombineHists(TH2F * inHistBinContent, TH2F * inHistBinErr) {
    TH2F * outHist = (TH2F *) inHistBinContent->Clone(inHistBinContent->GetName() + TString("Rate"));
    for (int iXBin = 0; iXBin < inHistBinContent->GetNbinsX() + 1; iXBin++) {
        for (int iYBin = 0; iYBin < inHistBinContent->GetNbinsY() + 1; iYBin++) {
            outHist->SetBinError(iXBin, iYBin, inHistBinErr->GetBinContent(iXBin,iYBin));
        }
    }
    return outHist;
}

inline TH2F * FakeLeptonHistMaker(TString inputFile, int whichHistType, int whichLepType, TString addString = "") {
    // Reads in input from .txt files that contain the lepton fake and/or prompt rate       ￼
    TString lepString = whichLepType == 11 ? "Elec" : "Muon";
    TString typeString = whichHistType == 1 ? "Prompt" : "Fake";
    TString histName = "h_";
    histName += lepString;
    histName += typeString;
//    histName += "Rate";
    histName += addString;
    TString axisString = ";Lepton #eta;Lepton p_{T} [GeV]";
    TH2F * outHist;
    ifstream inputFileStream(inputFile);
    vector<unsigned int> pTParams;
    int numEtaBins = 5;
    Double_t etaBoundsElec[6] = {0.0, 1.0, 1.4442, 1.566, 2.0, 2.5};
    Double_t etaBoundsMuon[6] = {0.0, 1.0, 1.479, 1.566, 2.0, 2.4};
    Double_t * etaBoundToUse = whichLepType == 11 ? etaBoundsElec : etaBoundsMuon;
    
    //    int nPtBins, PtMin, PtMax, nEtaBins, EtaMin, EtaMax;
    string line, field;
    int pTCounter;
    if (!(inputFileStream.eof())) {
        getline(inputFileStream, line);
        stringstream ss( line );
        while (std::getline(ss, field, ',' )) {
            stringstream fs ( field );
            int currField = 0;
            fs >> currField;
            pTParams.push_back(currField);
            cout << "currField " << currField << endl;
        }
    }
    else {
        cout << "issue with inputFile: prematurely reached end of file " << endl;
    }
    /*
    if (!(inputFileStream.eof())) {
        getline(inputFileStream, line);
        stringstream ss( (line) );
        while (std::getline(ss, field, ',' )) {
            stringstream fs ( field );
            int currField = 0;
            fs >> currField;
            EtaParams.push_back(currField);
        }
    }
    else {
        cout << "issue with inputFile: prematurely reached end of file " << endl;
    }
    */
    outHist = new TH2F(histName, axisString, numEtaBins, etaBoundToUse, pTParams[0], pTParams[1], pTParams[2]);
    for (int iEta = 0; iEta < numEtaBins; ++iEta) {
        pTCounter = 0;
        if (!(inputFileStream.eof())) {
            getline(inputFileStream, line);
            stringstream ss( (line) );
            while (std::getline(ss, field, ',' )) {
                stringstream fs ( field );
                float currField = 0.0;
                fs >> currField;
                cout << "in running through eta bins, for iEta = " << iEta << " currField is " << currField << endl;
                if (pTCounter == 0) outHist->SetBinContent(iEta + 1, pTCounter, currField);
                outHist->SetBinContent(iEta + 1, pTCounter + 1, currField);
                ++pTCounter;
            }
        }
        else {
            cout << "issue with inputFile: prematurely reached end of file " << endl;
        }
    }
    for (int i = 1; i < outHist->GetNbinsX() + 1; ++i) {
        for (int j = 1; j < outHist->GetNbinsY() + 1; ++j) {
            cout << "bin (" << i << ", " << j << ") is (eta = " << outHist->GetXaxis()->GetBinLowEdge(i) << ", pT = " << outHist->GetYaxis()->GetBinLowEdge(j) << endl;
            cout << "bin content in this bin: " << outHist->GetBinContent(i, j) << endl;
        }
    }
    return outHist;
}



inline TH2F * ResolutionHistMaker(TString inputFile) {
    // Following Discussion in SmearJet Function this function tries to load in the resolutions defined here:
    // http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/RecoMET/METProducers/python/METSigParams_cfi.py?revision=1.3&view=markup
    // (look for jdpt# (where # runs from 0 to 9)
    // the jdpt# refers to different Eta bins, as can be ascertained from
    // http://cmslxr.fnal.gov/lxr/source/RecoMET/METAlgorithms/src/SignAlgoResolutions.cc#121
    // and
    // http://cmslxr.fnal.gov/lxr/source/RecoMET/METAlgorithms/src/SignAlgoResolutions.cc#151
    // The function reads in a text file where these parameters have been put in,
    // with the first two lines contain pT eta information (number of eta/Pt bins and bin ranges)
    
    TH2F * outHist;
    ifstream inputFileStream(inputFile);
    vector<unsigned int> pTParams, EtaParams;
    //    int nPtBins, PtMin, PtMax, nEtaBins, EtaMin, EtaMax;
    string line, field;
    int pTCounter;
    if (!(inputFileStream.eof())) {
        getline(inputFileStream, line);
        stringstream ss( line );
        while (std::getline(ss, field, ',' )) {
            stringstream fs ( field );
            int currField = 0;
            fs >> currField;
            pTParams.push_back(currField);
        }
    }
    else {
        cout << "issue with inputFile: prematurely reached end of file " << endl;
    }
    if (!(inputFileStream.eof())) {
        getline(inputFileStream, line);
        stringstream ss( (line) );
        while (std::getline(ss, field, ',' )) {
            stringstream fs ( field );
            int currField = 0;
            fs >> currField;
            EtaParams.push_back(currField);
        }
    }
    else {
        cout << "issue with inputFile: prematurely reached end of file " << endl;
    }
    outHist = new TH2F("h_MCJetResol", ";Jet #eta;Jet p_{T}", EtaParams[0], EtaParams[1], EtaParams[2], pTParams[0], pTParams[1], pTParams[2]);
    for (unsigned int iEta = 0; iEta < EtaParams[0]; ++iEta) {
        pTCounter = 0;
        if (!(inputFileStream.eof())) {
            getline(inputFileStream, line);
            stringstream ss( (line) );
            while (std::getline(ss, field, ',' )) {
                stringstream fs ( field );
                float currField = 0.0;
                fs >> currField;
                if (pTCounter == 0) outHist->SetBinContent(iEta + 1, pTCounter, currField);
                outHist->SetBinContent(iEta + 1, pTCounter + 1, currField);
                ++pTCounter;
            }
        }
        else {
            cout << "issue with inputFile: prematurely reached end of file " << endl;
        }
    }
    for (int i = 1; i < outHist->GetNbinsX() + 1; ++i) {
        for (int j = 1; j < outHist->GetNbinsY() + 1; ++j) {
            cout << "bin (" << i << ", " << j << ") is (eta = " << outHist->GetXaxis()->GetBinLowEdge(i) << ", pT = " << outHist->GetYaxis()->GetBinLowEdge(j) << endl;
            cout << "bin content in this bin: " << outHist->GetBinContent(i, j) << endl;
        }
    }
    return outHist;
}


