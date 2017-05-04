void PrintSystToCard(TString inputCardName, TString inputBaseName, TString inputSystName, double inputCV, double inputSystUp, double inputSystDown) {
    
    double currErrUp, currErrDown;
    
    char buf[100];
    
    pointSystErr(inputCV, inputSystUp, inputSystDown, currErrUp, currErrDown);
    
    float printFloatUp = inputCV > 0 ? 1 + (currErrUp/inputCV) : 1.0;
    float printFloatDown = inputCV > 0 ? TMath::Max(0., 1 - (currErrUp/inputCV)) : 1.0;
    
    TString searchStringSystUp = inputBaseName + inputSystName + TString("Up");
    if (inputSystName.Contains("Stat") || inputSystName.Contains("Norm")) {
        searchStringSystUp = inputBaseName + inputSystName;
    }
    TString searchStringSystDown = inputBaseName + inputSystName + TString("Down");
    
    
    
    TString replaceStringSystUp, replaceStringSystDown;

    
    sprintf(buf, "%0.4f", printFloatUp);
    replaceStringSystUp = TString(string(buf));
    sprintf(buf, "%0.4f", printFloatDown);
    replaceStringSystDown = TString(string(buf));
    
    System_SedInFile(inputCardName, searchStringSystUp, replaceStringSystUp, false);
    System_SedInFile(inputCardName, searchStringSystDown, replaceStringSystDown, false);
}

void MakeCardsSignal(TFile * inputFileSig, TString baseNameSignal, vector<TString> * inVecNameSysts, TString inputCardName, TString addName) {
    //grab central value
    TH3F * inputHistSignal = (TH3F *) inputFileSig->Get(baseNameSignal);
    
    unsigned int numSysts = inVecNameSysts->size();
    
    vector<TH3F *> vecSignalHists_SystUp(numSysts);
    vector<TH3F *> vecSignalHists_SystDown(numSysts);
    
    for (unsigned int iSyst = 0; iSyst < numSysts; ++iSyst) {
        vecSignalHists_SystUp[iSyst] = (TH3F *) inputFileSig->Get(baseNameSignal + inVecNameSysts->at(iSyst) + TString("Up"));
        vecSignalHists_SystDown[iSyst] = (TH3F *) inputFileSig->Get(baseNameSignal + inVecNameSysts->at(iSyst) + TString("Down"));
    }
    
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputHistSignal, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    vector<TH3F *> vecCentVal;
    vecCentVal.push_back(inputHistSignal);
    
    vector<TString> vecHistName{"signal"};
    
    TString currSearchString, currReplaceString;
    char buf[100];
    
    TString currAddName, currCardSaveName;
    
    double currHistCV, currHistStatErr, currHistSystUp, currHistSystDown;
    
    
     for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
     for (int iBinY = 1; iBinY <= vecInputHistAxisNBins[1]; ++iBinY) {
     for (int iBinZ = 1; iBinZ <= vecInputHistAxisNBins[2]; ++iBinZ) {
    
         /*
    for (int iBinX = 1; iBinX <= 1; ++iBinX) {
        for (int iBinY = 1; iBinY <= 1; ++iBinY) {
            for (int iBinZ = 1; iBinZ <= 1; ++iBinZ) {
          */
                
                
                cout << "printing for {iBinX:iBinY:iBinZ} = " << "{" << iBinX << ":" << iBinY << ":" << iBinZ << "}" << endl;
                currAddName = TString("_Bin");
                currAddName += iBinX;
                currAddName += iBinY;
                currAddName += iBinZ;
                currAddName += ".txt";
                
                currCardSaveName = inputCardName + currAddName;
                for (int iSample = 0; iSample < 1; ++iSample) {
                    currHistCV = vecCentVal[iSample]->GetBinContent(iBinX, iBinY, iBinZ);
                    currHistStatErr = vecCentVal[iSample]->GetBinError(iBinX, iBinY, iBinZ);
                    
                    currSearchString = vecHistName[iSample] + TString("INT");
                    sprintf(buf, "%0.4f", currHistCV);
                    currReplaceString = TString(string(buf));
                    System_SedInFile(currCardSaveName, currSearchString, currReplaceString);
                    if (currHistCV > 0) {
                        sprintf(buf, "%0.4f", 1 + (currHistStatErr/currHistCV));
                    }
                    else {
                        sprintf(buf, "%0.4f", 1.0);
                    }
                    currReplaceString = TString(string(buf));
                    currSearchString = vecHistName[iSample] + vecHistName[iSample] + TString("MCStat") + addName + TString("_Bin");
                    currSearchString += iBinX;
                    currSearchString += iBinY;
                    currSearchString += iBinZ;
//                    System_SedInFile(currCardSaveName, currSearchString, currReplaceString, true);
                    System_SedInFile(currCardSaveName, currSearchString, currReplaceString, false);
                    if (iSample == 0) {
                        for (unsigned int iSyst = 0; iSyst < numSysts; ++iSyst) {
//                            cout << "on iSyst " << inVecNameSysts->at(iSyst) << endl;
                            currHistSystUp = vecSignalHists_SystUp[iSyst]->GetBinContent(iBinX, iBinY, iBinZ);
                            currHistSystDown = vecSignalHists_SystDown[iSyst]->GetBinContent(iBinX, iBinY, iBinZ);
                            PrintSystToCard(currCardSaveName, vecHistName[iSample], inVecNameSysts->at(iSyst), currHistCV, currHistSystUp, currHistSystDown);
                        }
                        //
                    }
                }
                
            }
        }
    }
}



void MakeCardsBkg(vector<TFile *> * inVecFiles, vector<TString> * inVecNameSysts, TString inputCardName, TString baseOutputCardName, TString addName) {
    
    TString baseNameData = "data_obs";
    TString baseNameTTBar = "TTBar";
    TString baseNameNonTTBar = "NonTTBar";
    //grab central value
    TH3F * inputHistData = (TH3F *) inVecFiles->at(0)->Get(baseNameData);
    TH3F * inputHistTTBar = (TH3F *) inVecFiles->at(1)->Get(baseNameTTBar);
    TH3F * inputHistNonTTBar = (TH3F *) inVecFiles->at(2)->Get(baseNameNonTTBar);

    unsigned int numSysts = inVecNameSysts->size();
    
    vector<TH3F *> vecTTBarHists_SystUp(numSysts);
    vector<TH3F *> vecTTBarHists_SystDown(numSysts);
    vector<TH3F *> vecNonTTBarHists_SystUp(numSysts);
    vector<TH3F *> vecNonTTBarHists_SystDown(numSysts);
    
    for (unsigned int iSyst = 0; iSyst < numSysts; ++iSyst) {
        vecTTBarHists_SystUp[iSyst] = (TH3F *) inVecFiles->at(1)->Get(baseNameTTBar + inVecNameSysts->at(iSyst) + TString("Up"));
        vecTTBarHists_SystDown[iSyst] = (TH3F *) inVecFiles->at(1)->Get(baseNameTTBar + inVecNameSysts->at(iSyst) + TString("Down"));
        
        vecNonTTBarHists_SystUp[iSyst] = (TH3F *) inVecFiles->at(2)->Get(baseNameNonTTBar + inVecNameSysts->at(iSyst) + TString("Up"));
        vecNonTTBarHists_SystDown[iSyst] = (TH3F *) inVecFiles->at(2)->Get(baseNameNonTTBar + inVecNameSysts->at(iSyst) + TString("Down"));
    }
    
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputHistData, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    vector<TH3F *> vecCentVal;
    vecCentVal.push_back(inputHistData);
    vecCentVal.push_back(inputHistNonTTBar);
    vecCentVal.push_back(inputHistTTBar);
    
    vector<TString> vecHistName{"data_obs", "NonTTBar", "TTBar"};
    
    TString currSearchString, currReplaceString;
    char buf[100];
    
    TString currAddName, currCardSaveName;
    
    double currHistCV, currHistStatErr, currHistSystUp, currHistSystDown;

    
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        for (int iBinY = 1; iBinY <= vecInputHistAxisNBins[1]; ++iBinY) {
            for (int iBinZ = 1; iBinZ <= vecInputHistAxisNBins[2]; ++iBinZ) {
     
    /*
    for (int iBinX = 1; iBinX <= 1; ++iBinX) {
        for (int iBinY = 1; iBinY <= 1; ++iBinY) {
            for (int iBinZ = 1; iBinZ <= 1; ++iBinZ) {
                */

                
                cout << "printing for {iBinX:iBinY:iBinZ} = " << "{" << iBinX << ":" << iBinY << ":" << iBinZ << "}" << endl;
                currAddName = addName + TString("_Bin");
                currAddName += iBinX;
                currAddName += iBinY;
                currAddName += iBinZ;
                currAddName += ".txt";
                
                currCardSaveName = baseOutputCardName + currAddName;
                System_CopyFile(inputCardName, currCardSaveName);
                
                currSearchString = "MCStat";
                currReplaceString = currSearchString + addName + TString("_Bin");
                currReplaceString += iBinX;
                currReplaceString += iBinY;
                currReplaceString += iBinZ;
                System_SedInFile(currCardSaveName, currSearchString, currReplaceString);
                
                for (int iSample = 0; iSample < 3; ++iSample) {
                    currHistCV = vecCentVal[iSample]->GetBinContent(iBinX, iBinY, iBinZ);
                    currHistStatErr = vecCentVal[iSample]->GetBinError(iBinX, iBinY, iBinZ);
                    
                    currSearchString = vecHistName[iSample] + TString("INT");
                    sprintf(buf, "%0.4f", currHistCV);
                    currReplaceString = TString(string(buf));
                    System_SedInFile(currCardSaveName, currSearchString, currReplaceString);
                    if (currHistCV > 0) {
                        sprintf(buf, "%0.4f", 1 + (currHistStatErr/currHistCV));
                    }
                    else {
                        sprintf(buf, "%0.4f", 1.0);
                    }
                    currReplaceString = TString(string(buf));
                    currSearchString = vecHistName[iSample] + vecHistName[iSample] + TString("MCStat") + addName + TString("_Bin");
                    currSearchString += iBinX;
                    currSearchString += iBinY;
                    currSearchString += iBinZ;
//                    System_SedInFile(currCardSaveName, currSearchString, currReplaceString, true);
                    System_SedInFile(currCardSaveName, currSearchString, currReplaceString, false);
                    if (iSample > 0) {
                        for (unsigned int iSyst = 0; iSyst < numSysts; ++iSyst) {
//                            cout << "on iSyst " << inVecNameSysts->at(iSyst) << endl;
                            if (iSample == 2) {
                                currHistSystUp = vecTTBarHists_SystUp[iSyst]->GetBinContent(iBinX, iBinY, iBinZ);
                                currHistSystDown = vecTTBarHists_SystDown[iSyst]->GetBinContent(iBinX, iBinY, iBinZ);
                            }
                            else {
                                currHistSystUp = vecNonTTBarHists_SystUp[iSyst]->GetBinContent(iBinX, iBinY, iBinZ);
                                currHistSystDown = vecNonTTBarHists_SystDown[iSyst]->GetBinContent(iBinX, iBinY, iBinZ);
                            }
                            PrintSystToCard(currCardSaveName, vecHistName[iSample], inVecNameSysts->at(iSyst), currHistCV, currHistSystUp, currHistSystDown);
                        }
                        //
                    }
                }
                
            }
        }
    }
    
}