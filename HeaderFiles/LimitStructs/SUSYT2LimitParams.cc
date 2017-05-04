#ifndef SUSYT2LimitParams_cc
#define SUSYT2LimitParams_cc
void SUSYT2LimitParams::DefaultVals() {
    typeT2 = 1;
    indexPol = 5;
    charFrac = -1;
    
    typeT2Fix = -99;
    indexPolFix = -99;
    charFracFix = -99;
    
    useFixParams = false;
}

void SUSYT2LimitParams::FixTheFixParams() {
    
    if (typeT2Fix == -99) {
        typeT2Fix = typeT2;
    }
    if (indexPolFix == -99) {
        indexPolFix = indexPol;
    }
    if (charFracFix == -99) {
        charFracFix = charFrac;
    }
    if (useFixParams) {
        typeT2 = typeT2Fix;
        indexPol = indexPolFix;
        charFrac = charFracFix;
    }
}

bool SUSYT2LimitParams::IsNominalScenario() {
    bool outBool = false;
    if ((typeT2 == 0 && indexPol == 4) || ((typeT2 == 1 || typeT2 == 2) && indexPol == -1) || typeT2 == 3) {
        outBool = true;
    }
    return outBool;
}

void SUSYT2LimitParams::PrintStrings() {
    currSDP.Print();
}

TString SUSYT2LimitParams::LatexString() {
    return currSDP.latexTitleString_RealLatex;
}

TString SUSYT2LimitParams::TitleString() {
    return currSDP.latexTitleString_TLatex.text;
}

void SUSYT2LimitParams::FillVectorOfSUSYT2LPs(vector<SUSYT2LimitParams> * inVecSUSYT2LP) {
    inVecSUSYT2LP->clear();
    SUSYT2LimitParams currSUSYT2LP;
    
    vector<int> vecIndexT2bw{0, 2, 6, 8};
    vector<int> vecIndexT2tt{0, 10};
    vector<int> vecIndexT2tb{};
    
    vector<int> * vecIndices;
    if (typeT2 == 0) {
        vecIndices = &vecIndexT2bw;
    }
    else if (typeT2 == 3) {
        vecIndices = &vecIndexT2tb;
    }
    else {
        vecIndices = &vecIndexT2tt;
    }
    unsigned int numSUSY = vecIndices->size();
    
    for (unsigned int iSUSY = 0; iSUSY < numSUSY; ++iSUSY) {
        currSUSYT2LP = *this;
        currSUSYT2LP.indexPol = vecIndices->at(iSUSY);
        float currBinWidth = this->currSDP.binWidth;
        currSUSYT2LP.SetStrings(currBinWidth, -1, 312.5, false);
        inVecSUSYT2LP->push_back(currSUSYT2LP);
    }
}

void SUSYT2LimitParams::SetStrings(float binWidth = 25, float endX = -1, float endY = 312.5, bool isHack = false) {
    currSDP.isHack = isHack;
    currSDP.binWidth = binWidth;
    currSDP.SetVectorSize();
    if (typeT2) {
        if (typeT2 > 2) {
            currSDP.SetAsT2tb(indexPol);
        }
        else {
            currSDP.SetAsT2tt(indexPol, endX - (binWidth/2), endY - (binWidth/2));
        }
    }
    else {
        currSDP.SetAsT2bw(indexPol, charFrac, endX - (binWidth/2), endY - (binWidth/2));
    }
    SetKinematicRange(&currSDP.bounds_SUSYMasses, binWidth);
}

void SUSYT2LimitParams::SetParams(bool isT2tt, int whichPolIndex, int inCharFrac = -1) {
    typeT2 = isT2tt;
    indexPol = whichPolIndex;
    if (inCharFrac > 0) charFrac = inCharFrac;
    
    SetStrings();
}

TString SUSYT2LimitParams::SavingName(bool isDir, int isTightBin = false) {
    return currSDP.SavingName(isDir, isTightBin);
}

void SUSYT2LimitParams::SetParamsFromCommandLine(int argc, char * argv[]) {
    for (int iT2SMSParam = 0; iT2SMSParam < argc; ++iT2SMSParam) {
        if (strncmp (argv[iT2SMSParam],"T2SMS", 5) == 0) {
            typeT2 = strtol(argv[iT2SMSParam + 1], NULL, 10);
        }
        else if (strncmp (argv[iT2SMSParam],"indexPol", 8) == 0) {
            indexPol = strtol(argv[iT2SMSParam + 1], NULL, 10);
        }
        else if (strncmp (argv[iT2SMSParam],"charFrac", 8) == 0) {
            charFrac = strtol(argv[iT2SMSParam + 1], NULL, 10);
        }
        else if (strncmp (argv[iT2SMSParam],"fixT2SMS", 8) == 0) {
            typeT2Fix = strtol(argv[iT2SMSParam + 1], NULL, 10);
            useFixParams = true;
        }
        else if (strncmp (argv[iT2SMSParam],"fixindexPol", 11) == 0) {
            indexPolFix = strtol(argv[iT2SMSParam + 1], NULL, 10);
            useFixParams = true;
        }
        else if (strncmp (argv[iT2SMSParam],"fixcharFrac", 11) == 0) {
            charFracFix = strtol(argv[iT2SMSParam + 1], NULL, 10);
            useFixParams = true;
        }
    }
}

void SUSYT2LimitParams::DrawOnCanvas(TCanvas * inputCanvas, bool includeKinematicLines, bool doVerb = false) {
    currSDP.DrawOnCanvas(inputCanvas, includeKinematicLines, doVerb);
}

void SUSYT2LimitParams::FullSequence(int argc, char * argv[]) {
    DefaultVals();
    SetParamsFromCommandLine(argc, argv);
    SetStrings();
}


#endif