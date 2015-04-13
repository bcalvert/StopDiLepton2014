#include <iostream>
#include <fstream>
//#include <vector>
#include <cmath>
#include <sstream>

//Functions used for calculating Jet pT and phi resolutions

//Using this as inspiration
//https://github.com/cms-met/cmssw/blob/73X-MetSig-141019/JetMETAnalysis/METSignificance/src/METSignificance.cc#L264
//https://github.com/cms-met/cmssw/blob/CMSSW_7_2_X/CondFormats/JetMETObjects/src/JetResolution.cc

inline void VecHighPtJet_PhiResolutionTF1(vector<TF1> * inTF1Vec) {
    // Formula and parameter values for "sigma" taken from https://github.com/cms-met/cmssw/blob/CMSSW_7_2_X/CondFormats/JetMETObjects/data/Spring10_PhiResolution_AK5PF.txt
    // Formula: (sqrt((sq([0]/x)+(sq([1])/x))+sq([2]))+([3]/x))+(([4]/x)/sqrt(x))
    /*
     EtaMin     EtaMax  NMinuitParams   FuncMin     FuncMax     Par0        Par1            Par2            Par3        Par4
     0          0.5     7               10.6361     2738.28     926.978     2.52747         0.0304001       -926.224    -1.94117
     0.5        1       7               10.6399     2183.6      3.32512e-06 0.063941        -0.0038593      0.301932    -0.825352
     1          1.5     7               10.6191     1130.72     0.38469     0.0755727       -0.0044353      0.453887    -1.8947
     1.5        2       7               10.6291     828.554     2.92001e-07 0.0718389       -0.00385579     0.403668    -0.62698
     2          2.5     7               10.6296     439.302     0.00336639  0.0880209       -0.0023084      0.214304    -0.416353
     2.5        3       7               10.6291     235.896     11.1957     0.643236        0.00711422      -10.7613    0.280927
     3          3.5     7               10.6415     130.192     1.9027      -4.56986e-06    0.0304793       -1.09124    -0.136402
     3.5        4       7               10.6136     99.049      2.11446     0.203329        -0.0175832      -1.67946    -0.00853474
     4          4.5     7               10.6021     49.8305     0.765787    -3.90638e-06    -4.70224e-08    0.11831     -1.4675
     4.5        9.9     7               10.6035     30.088      259.189     0.00132792      -0.311411       -258.647    0
     */
    TString baseTF1Name = "PhiResolTF1", TF1Name;
    TString function = "(sqrt((sq([0]/x)+(sq([1])/x))+sq([2]))+([3]/x))+(([4]/x)/sqrt(x))";
    const int numTF1s = 10;
    float Par0[numTF1s] = {926.978, 3.32512e-06, 0.38469, 2.92001e-07, 0.00336639, 11.1957, 1.9027, 2.11446, 0.765787, 259.189};
    float Par1[numTF1s] = {2.52747, 0.063941, 0.0755727, 0.0718389, 0.0880209, 0.643236, -4.56986e-06, 0.203329, -3.90638e-06, 0.00132792};
    float Par2[numTF1s] = {0.0304001, -0.0038593, -0.0044353, -0.00385579, -0.0023084, 0.00711422, 0.0304793, -0.0175832, -4.70224e-08, -0.311411};
    float Par3[numTF1s] = {-926.224, 0.301932, 0.453887, 0.403668, 0.214304, -10.7613, -1.09124, -1.67946, 0.11831, -258.647};
    float Par4[numTF1s] = {-1.94117, -0.825352, -1.8947, -0.62698, -0.416353, 0.280927, -0.136402, -0.00853474, -1.4675, 0};
    for (int iFunc = 0; iFunc < numTF1s; ++iFunc) {
        TF1Name = baseTF1Name;
        TF1Name += "_EtaBin"; TF1Name += iFunc;
        TF1 currTF1(TF1Name, function, 0., 99999.);
        currTF1.SetParameter(0, Par0[iFunc]);
        currTF1.SetParameter(1, Par1[iFunc]);
        currTF1.SetParameter(2, Par2[iFunc]);
        currTF1.SetParameter(3, Par3[iFunc]);
        currTF1.SetParameter(4, Par4[iFunc]);
        inTF1Vec->push_back(currTF1);
    }
}




inline void VecHighPtJet_PtResolutionTF1(vector<TF1> * inTF1Vec) {
    // Formula and parameter values for "sigma" taken from https://github.com/cms-met/cmssw/blob/CMSSW_7_2_X/CondFormats/JetMETObjects/data/Spring10_PtResolution_AK5PF.txt
    // Formula: sqrt(((TMath::Sign(1,[0])*sq([0]/x))+(sq([1])*(x^([3]-1))))+sq([2]))
    /*
     EtaMin EtaMax  NMinuitParams   FuncMin FuncMax Par0        Par1     Par2   Par3
     0      0.5     6               0.      99999.  -0.349206   0.297831 0      0.471121
     0.5    1       6               0.      99999.  -0.499735   0.336391 0      0.430689
     1      1.5     6               0.      99999.  -0.561649   0.420293 0      0.392398
     1.5    2       6               0.      99999.  -1.12329    0.657891 0      0.139595
     2      2.5     6               0.      99999.  1.04792     0.466763 0      0.193137
     2.5    3       6               0.      99999.  2.56933     0.305802 0      0.398929
     3      3.5     6               0.      99999.  2.81978     0.272373 0      0.579396
     3.5    4       6               0.      99999.  1.65966     0.223683 0      0.60873
     4      9.9     6               0.      99999.  1.41584     0.209477 0      0.588872
     */
    
    TString baseTF1Name = "PtResolTF1", TF1Name;
    TString function = "sqrt(((TMath::Sign(1,[0])*sq([0]/x))+(sq([1])*(x^([3]-1))))+sq([2]))";
    const int numTF1s = 9;
    float Par0[numTF1s] = {-0.349206, -0.499735, -0.561649, -1.12329, 1.04792, 2.56933, 2.81978, 1.65966, 1.41584};
    float Par1[numTF1s] = {0.297831, 0.336391, 0.420293, 0.657891, 0.466763, 0.305802, 0.272373, 0.223683, 0.209477};
    float Par2[numTF1s] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    float Par3[numTF1s] = {0.471121, 0.430689, 0.392398, 0.139595, 0.193137, 0.398929, 0.579396, 0.60873, 0.588872};
    for (int iFunc = 0; iFunc < numTF1s; ++iFunc) {
        TF1Name = baseTF1Name;
        TF1Name += "_EtaBin"; TF1Name += iFunc;
        TF1 currTF1(TF1Name, function, 0., 99999.);
        currTF1.SetParameter(0, Par0[iFunc]);
        currTF1.SetParameter(1, Par1[iFunc]);
        currTF1.SetParameter(2, Par2[iFunc]);
        currTF1.SetParameter(3, Par3[iFunc]);
        inTF1Vec->push_back(currTF1);
    }
}

inline float GetPtResolution_HighPtJet(TLorentzVector * inputJet, vector<TF1> * inputTF1Vec) {
    const int numTF1s = 9;
    float currJetAbsEta = fabs(inputJet->Eta());
    float EtaBinEnds[numTF1s + 1] = {0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 9.9}; // in reality 9 TF1 vecs
    int   currJetEtaIndex = -1;
    bool  cornerCaseBinEdge = false;
    float outResolution;
    for (unsigned int iEta = 0; iEta < numTF1s; ++iEta) {
        if (currJetAbsEta > EtaBinEnds[iEta] && currJetAbsEta < EtaBinEnds[iEta + 1]) {
            currJetEtaIndex = iEta;
            break;
        }
        else if (currJetAbsEta == EtaBinEnds[iEta] && currJetAbsEta > 0) {
            currJetEtaIndex = iEta;
            cornerCaseBinEdge = true;
            cout << "corner case came up, jet eta is " << inputJet->Eta() << endl;
            // you've somehow managed to fall right on the bin edge -- floating point precision -- so take average of two different bins -- currently rolling with straight up mean
        }
    }
    if (currJetEtaIndex < 0) {
        cout << "can't do this jet!!...technically jet eta is: " << inputJet->Eta() << endl;
        currJetEtaIndex = 8;
    }
    /*
    cout << "currJetEtaIndex " << currJetEtaIndex << endl;;
    cout << "currJetPt " << inputJet->Pt() << endl;
    */
    outResolution = inputTF1Vec->at(currJetEtaIndex).Eval(inputJet->Pt());
    /*
     if (cornerCaseBinEdge) {
     outResolution += inputTF1Vec->at(currJetEtaIndex - 1).Eval(inputJet->Pt());
     outResolution /= 2;
     }
     */
    return outResolution;
}

inline float GetPhiResolution_HighPtJet(TLorentzVector * inputJet, vector<TF1> * inputTF1Vec) {
    const int numTF1s = 10;
    float currJetAbsEta = fabs(inputJet->Eta());
    float EtaBinEnds[numTF1s + 1] = {0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 9.9}; // in reality 9 TF1 vecs -- think this is a bug...
    int   currJetEtaIndex = -1;
    bool  cornerCaseBinEdge = false;
    float outResolution;
    for (unsigned int iEta = 0; iEta < numTF1s; ++iEta) {
        if (currJetAbsEta > EtaBinEnds[iEta] && currJetAbsEta < EtaBinEnds[iEta + 1]) {
            currJetEtaIndex = iEta;
            break;
        }
        else if (currJetAbsEta == EtaBinEnds[iEta] && currJetAbsEta > 0) {
            currJetEtaIndex = iEta;
            cornerCaseBinEdge = true;
            cout << "corner case came up, jet eta is " << inputJet->Eta() << endl;
            // you've somehow managed to fall right on the bin edge -- floating point precision -- so take average of two different bins -- currently rolling with straight up mean
        }
    }
    if (currJetEtaIndex < 0) {
        cout << "can't do this jet!!...technically jet eta is: " << inputJet->Eta() << endl;
        currJetEtaIndex = 8;
    }
    /*
     cout << "currJetEtaIndex " << currJetEtaIndex << endl;;
     cout << "currJetPt " << inputJet->Pt() << endl;
     */
    outResolution = inputTF1Vec->at(currJetEtaIndex).Eval(inputJet->Pt());
    return outResolution;
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
    /*
    for (int i = 1; i < outHist->GetNbinsX() + 1; ++i) {
        for (int j = 1; j < outHist->GetNbinsY() + 1; ++j) {
            cout << "bin (" << i << ", " << j << ") is (eta = " << outHist->GetXaxis()->GetBinLowEdge(i) << ", pT = " << outHist->GetYaxis()->GetBinLowEdge(j) << endl;
            cout << "bin content in this bin: " << outHist->GetBinContent(i, j) << endl;
        }
    }
    */
    return outHist;
}
