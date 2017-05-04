#ifndef LimitScenarioSmoother_Optimizer_cc
#define LimitScenarioSmoother_Optimizer_cc
void LimitScenarioSmoother_Optimizer::SetZeroNonExcl(bool whichVal) {
    LSC_Unsmoothed.doZeroNonExcl = whichVal;
    LSC_Smoothed.doZeroNonExcl = whichVal;
}
void LimitScenarioSmoother_Optimizer::DrawLatex(TCanvas * inputCanvas, int whichSmooth) {
    TString strSmooth = !whichSmooth ? "No Smoothing" : "Varying Gaussian Kernel";
    inputCanvas->cd();
    TLatex a;
    a.SetNDC();
    a.SetTextSize(0.03);
    a.DrawLatex(0.15, 0.88, strSmooth.Data());
}
void LimitScenarioSmoother_Optimizer::DrawHist(bool doObs, int whichSig, int whichHist, int whichSmooth, int drawOption, TString addNameCanv) {
    TCanvas * outCanv;
    TString canvAddName = whichSmooth ? "_OptGauss" : "";
    
    LimitScenarioContainer * whichLSCToDraw = whichSmooth ? &LSC_Smoothed : &LSC_Unsmoothed;
    outCanv = whichLSCToDraw->DrawHist(doObs, whichSig, whichHist, drawOption, canvAddName);
    DrawLatex(outCanv, whichSmooth);
    outCanv->SaveAs(outCanv->GetName() + addNameCanv + TString(".pdf"));
}


void LimitScenarioSmoother_Optimizer::DrawSignifHist(bool doObs, int whichSig, int whichHist, int whichSmooth, int drawOption, TString addNameCanv) {
    TCanvas * outCanv;
    TString canvAddName = whichSmooth ? "_OptGauss" : "";
    
    SignifScenarioContainer * whichSSCToDraw = whichSmooth ? &SSC_Smoothed : &SSC_Unsmoothed;
    outCanv = whichSSCToDraw->DrawHist(doObs, whichSig, whichHist, drawOption, canvAddName);
    DrawLatex(outCanv, whichSmooth);
    outCanv->SaveAs(outCanv->GetName() + addNameCanv + TString(".pdf"));
}
/*
void LimitScenarioSmoother_Optimizer::SetHistsAsConstDeltaM(bool doVerb) {
    if (doVerb) {
        cout << "doing ConstDeltaM for unsmoothed " << endl;
    }
    LSC_Unsmoothed.SetHistsAsConstDeltaM(doVerb);
    for (unsigned int iSig = 0; iSig < vecLSC_Smoothed.size(); ++iSig) {
        if (doVerb) {
            cout << "doing ConstDeltaM for vecLSC_Smoothed[" << iSig << "]" << endl;
        }
        vecLSC_Smoothed[iSig].SetHistsAsConstDeltaM(doVerb);
    }
}
*/

/*void LimitScenarioSmoother_Optimizer::Set() {
relThresh 2 errMult 10 doZero setSigXScan 15 15 40 setSigYScan 10 5 25 doLogN
}
*/

void LimitScenarioSmoother_Optimizer::LoadHists(TFile * inputFile, bool getNoTruncate, bool doVerb = false) {
    LSC_Unsmoothed.LoadHists(inputFile, getNoTruncate, doVerb);
    LSC_Smoothed.LoadHists(inputFile, getNoTruncate, doVerb);
}

void LimitScenarioSmoother_Optimizer::LoadSignifHists(TFile * inputFile, bool doVerb = false) {
    SSC_Unsmoothed.LoadSignifHists(inputFile, doVerb);
    SSC_Smoothed.LoadSignifHists(inputFile, doVerb);
}

void LimitScenarioSmoother_Optimizer::DefaultVarVals(LimitParametersContainer * inLPC, SUSYT2LimitParams * inSUSYT2LP) {
    addNameSmoothing = "_2DGaussSmoothed_OptKernel";
    scenarioSmoother.DefaultVarVals();
    bool checkNominality = true;
    if (inLPC->typeDilepAna > 0) {
        checkNominality = false;
    }
    LSC_Unsmoothed.DefaultVarVals(inLPC, inSUSYT2LP, checkNominality);
    LSC_Smoothed.DefaultVarVals(inLPC, inSUSYT2LP, checkNominality);
    SSC_Unsmoothed.DefaultVarVals(inLPC, inSUSYT2LP);
    SSC_Smoothed.DefaultVarVals(inLPC, inSUSYT2LP);
}

void LimitScenarioSmoother_Optimizer::SetPathAsDrawingPath() {
    LSC_Unsmoothed.nameOutPath = "../ContourMaking/" + LSC_Unsmoothed.nameOutPath;
    LSC_Smoothed.nameOutPath = "../ContourMaking/" + LSC_Smoothed.nameOutPath;
    
    SSC_Unsmoothed.nameOutPath = "../ContourMaking/" + SSC_Unsmoothed.nameOutPath;
    SSC_Smoothed.nameOutPath = "../ContourMaking/" + SSC_Smoothed.nameOutPath;
}

void LimitScenarioSmoother_Optimizer::DoSmoothing(TH1F * inputHistXSection, bool doVerb = false) {
    scenarioSmoother.SetName();
    LSC_Smoothed = LSC_Unsmoothed;
    LSC_Smoothed.addName = addNameSmoothing;
    LSC_Smoothed.SmoothSigStrengthConstDeltM_Optimal(&scenarioSmoother, doVerb);
    cout << "about to re-make x-section limit hists" << endl;
    LSC_Smoothed.MakeXSectionLimitHists(inputHistXSection, addNameSmoothing);
}


void LimitScenarioSmoother_Optimizer::DoSignifSmoothing(bool doVerb = false) {
    scenarioSmoother.SetName();
    SSC_Smoothed = SSC_Unsmoothed;
    SSC_Smoothed.addName = addNameSmoothing;
    SSC_Smoothed.SmoothSignifConstDeltM_Optimal(&scenarioSmoother, doVerb);
}

void LimitScenarioSmoother_Optimizer::WriteFile(TFile * extOutFile = NULL, bool doVerb = false) {
    bool doVerbSpecial = doVerb;
    if (doVerbSpecial) {
        cout << "about to write smoothed maps" << endl;
    }
    LSC_Smoothed.WriteFile(false, extOutFile, false, doVerbSpecial);
    if (doVerbSpecial) {
        cout << "about to write unsmoothed maps" << endl;
    }
    LSC_Unsmoothed.WriteFile(false, extOutFile, true, doVerbSpecial);
}

void LimitScenarioSmoother_Optimizer::WriteSignifToFile(TFile * extOutFile = NULL, bool doVerb = false) {
    bool doVerbSpecial = doVerb;
    if (doVerbSpecial) {
        cout << "about to write smoothed maps" << endl;
    }
    SSC_Smoothed.WriteFile(false, extOutFile, false, doVerbSpecial);
    if (doVerbSpecial) {
        cout << "about to write unsmoothed maps" << endl;
    }
    SSC_Unsmoothed.WriteFile(false, extOutFile, true, doVerbSpecial);
}

void LimitScenarioSmoother_Optimizer::SetParamsFromCommandLine(int argc, char * argv[]) {
    LSC_Smoothed.SetParamsFromCommandLine(argc, argv);
    LSC_Unsmoothed.SetParamsFromCommandLine(argc, argv);
    SSC_Smoothed.SetParamsFromCommandLine(argc, argv);
    SSC_Unsmoothed.SetParamsFromCommandLine(argc, argv);
    scenarioSmoother.SetParamsFromCommandLine(argc, argv);
    scenarioSmoother.SetKernelParams();
    scenarioSmoother.SetVecSizes();
}

#endif
