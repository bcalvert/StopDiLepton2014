//Similar to the functions used in StopPlotSetup, except for running using the shape nTuples (i.e. for backup plots related to the MT2 shape analysis)

void SetupPlotRunning_ShapesPart1(int argc, char * argv[], RunParams * inRP, PlotMakingStructs * inPMS, AncillaryDrawingVariables * inADV, GlobalHistPlotInfo * inGHPI, HistDisplayParams * inHDP) {
    inRP->DefaultVarVals();
    inRP->SetVals(argc, argv);
    inRP->SetGSISaveName();
    
    inHDP->DefaultVarVals();
    inHDP->SetParamsFromCommandLine(argc, argv);
    
    inPMS->SetStructs(&inRP->SLS, inHDP->whichIndMCSort, true);

    inADV->SetDefaultAxisSizeParams();
    inADV->DefaultLegParams();
    
    
    int typeMET = 0;
    TString stringMETExtra = inRP->SLS.noType0 ? " w/o Type 0" : "";
    TString genCutUsed = "";
    inGHPI->DefaultVarVals();
    inGHPI->SetParamsFromCommandLine(argc, argv);
    inGHPI->SetFracRatioADPNameRange(inRP->GHS.doAbsRatio, inRP->GHS.ReturnFracRatioBound(1), inRP->GHS.ReturnFracRatioBound(2));
    inGHPI->SetLatexStringVec(19656, typeMET, stringMETExtra, inRP->SLS.whichDilepType, genCutUsed, true);    
}


void SetupPlotRunning_DataMCShape(RunParams * inRP, PlotMakingStructs * inPMS, HistogramDisplayStructs * inHDS_Data, HistogramDisplayStructs * inHDS_MC, TString addString, bool doVerb = false) {
    bool isNotSig = false;
    vector<TString> vecBaseFileNames_Data;
    StopBaseFileNames_DataShape(&vecBaseFileNames_Data);
    
    inHDS_Data->DefaultVarVals();
    inHDS_Data->SetInputFileShape(&vecBaseFileNames_Data, inRP->SLS.FullPathFile(0), addString + inRP->SLS.ShapeNameAppend(), 0, doVerb);
    inHDS_Data->SetCompParams("Data", 0);
    inHDS_Data->SetSystSize(0);
    
    vector<TString> vecBaseFileNames_MC;
    StopBaseFileNames_MCShape(&vecBaseFileNames_MC);
    
    inHDS_MC->DefaultVarVals();
    inHDS_MC->SetInputFileShape(&vecBaseFileNames_MC, inRP->SLS.FullPathFile(1), addString + inRP->SLS.ShapeNameAppend(), inPMS->vecSystNames.size(), doVerb);
    inHDS_MC->SetCompParams("MC", 1);
    
    inHDS_MC->SetIndMCParams(&inPMS->vecMCLegends, &inPMS->vecMCColors, &inPMS->vecMCSortParams);
    inHDS_MC->SetSystSize(inPMS->numSysts, inRP->SLS.doSyst);
    SetSystBasicsShape(inHDS_MC, &inPMS->vecSystNames);
}

void SetupPlotRunning_SignalShapeSpecial(RunParams * inRP, PlotMakingStructs * inPMS, HistogramDisplayStructs * inHDS_Signal, TString signalString, TString addString, bool doVerb = false) {
    
    vector<TString> vecBaseFileNames_Signal;
    
    
    if (signalString.Length() > 0) {
        vecBaseFileNames_Signal.push_back(signalString);
    }
    else {
        StopBaseFileNames_SignalShape(&vecBaseFileNames_Signal, &inRP->SLS, doVerb);
    }
    
    inHDS_Signal->DefaultVarVals();
    inHDS_Signal->SetInputFileShape(&vecBaseFileNames_Signal, "", addString + TString(".root"), inPMS->vecSystNames.size(), doVerb);
    
    inHDS_Signal->SetCompParams(inRP->SLS.SignalLegendString(0), -1);
    inHDS_Signal->SetSystSize(inPMS->numSysts, inRP->SLS.doSyst);
    SetSystBasics(inHDS_Signal, &inPMS->vecSystNames, inRP->SLS.SmearedPlots, true, doVerb);
}

void SetupPlotRunning_SignalShape(RunParams * inRP, PlotMakingStructs * inPMS, HistogramDisplayStructs * inHDS_Signal, TString signalString, TString addString, bool doVerb = false) {
    
    vector<TString> vecBaseFileNames_Signal;
    
    
    TString additionalString = addString.Contains("WithSigContam") ? "_WithSigContam" : "_NoSigContam";
    TString grabStringSignal = inRP->SLS.ShapeNameAppend();
    grabStringSignal.Replace(grabStringSignal.Index(".root"), 5, additionalString + TString(".root"));
    if (signalString.Length() > 0) {
        vecBaseFileNames_Signal.push_back(signalString);
    }
    else {
        StopBaseFileNames_SignalShape(&vecBaseFileNames_Signal, &inRP->SLS, doVerb);
    }
    
    inHDS_Signal->DefaultVarVals();
    inHDS_Signal->SetInputFileShape(&vecBaseFileNames_Signal, inRP->SLS.FullPathFile(2), addString + grabStringSignal, inPMS->vecSystNames.size(), doVerb);
    
    inHDS_Signal->SetCompParams(inRP->SLS.SignalLegendString(0), -1);
    inHDS_Signal->SetSystSize(inPMS->numSysts, inRP->SLS.doSyst);
    SetSystBasicsShape(inHDS_Signal, &inPMS->vecSystNames);
}



