// Functions used to make custom histograms that aren't nominally saved

TH1F * FOMHist(TH1F * inputMCCompHist, TH1F * inputMCSigHist, TString SigStringTitle, int typeFOM, int whichSystType) {
    int NBinsX = inputMCCompHist->GetNbinsX();
    int XMax = inputMCCompHist->GetXaxis()->GetBinLowEdge(NBinsX + 1);
    float MCInt, SigInt, FOMCentVal, FOMStatErr;
    TString baseName = "h_FOM_forDiffMT2llCuts";
    switch (whichSystType) {
        case 0:
            baseName += "_CentVal";
            break;
        case 1:
            baseName += "_LepESShiftUp";
            break;
        case 2:
            baseName += "_LepESShiftDown";
            break;
        case 3:
            baseName += "_JetESShiftUp";
            break;
        case 4:
            baseName += "_JetESShiftDown";
            break;
        case 5:
            baseName += "_UncESShiftUp";
            break;
        case 6:
            baseName += "_UncESShiftDown";
            break;
        case 7:
            baseName += "_LepEffSFShiftUp";
            break;
        case 8:
            baseName += "_LepEffSFShiftDown";
            break;            
        case 9:
            baseName += "_genTopRW";
            break;
        case 10:
            baseName += "_StopXSecShiftUp";
            break;
        case 11:
            baseName += "_StopXSecShiftDown";
            break;
        case 12:
            baseName += "_BTagSFShiftUp";
            break;
        case 13:
            baseName += "_BTagSFShiftDown";
        default:
            break;
    }
    float Numerator, Denominator;
    TString axisString;
    if (typeFOM == 0) {
        axisString = "F.O.M. = #frac{S}{#sqrt{S + B}}, ";
        axisString += SigStringTitle;
        axisString += "; MT2_{ll} Cut [GeV];";
    }
    else {
        axisString = "F.O.M. = #frac{S}{#sqrt{B}}, ";
        axisString += SigStringTitle;
        axisString += "; MT2_{ll} Cut [GeV];";
    }
    TH1F * outHist = new TH1F(baseName, axisString, NBinsX, 0., XMax); outHist->Sumw2();
    for (int xInd = 1; xInd < NBinsX + 1; ++xInd) {
        MCInt = inputMCCompHist->Integral(xInd, NBinsX + 1);
        SigInt = inputMCSigHist->Integral(xInd, NBinsX + 1);
        if (typeFOM == 0) {
            Numerator  = SigInt;
            Denominator = TMath::Sqrt(SigInt + MCInt);
            FOMCentVal = Denominator > 0 ? Numerator / Denominator: 0;
            FOMStatErr = 0; //TMath::Sqrt(SigInt) * (SigInt + 2 * MCInt + SigInt * MCInt) / (2 * TMath::Power(SigInt + MCInt, 1.5));
        }
        else {        
            Numerator  = SigInt;
            Denominator = TMath::Sqrt(MCInt);
            FOMCentVal = Denominator > 0 ? Numerator / Denominator: 0;
            FOMStatErr = 0; //TMath::Sqrt(SigInt) * (SigInt + 2 * MCInt + SigInt * MCInt) / (2 * TMath::Power(SigInt + MCInt, 1.5));            
        }
        outHist->SetBinContent(xInd, FOMCentVal);
        outHist->SetBinError(xInd, FOMStatErr);        
    }
    return outHist;
}

TH2F * FOMHist(TH2F * inputMCCompHist, TH2F * inputMCSigHist, TString SigStringTitle, int typeFOM, int whichSystType) {
    bool penaltyTerm = 0;
    bool doVerbosity = false; //true;
    int NBinsX = inputMCCompHist->GetNbinsX();
    int XMax = inputMCCompHist->GetXaxis()->GetBinLowEdge(NBinsX + 1);
    int NBinsY = inputMCCompHist->GetNbinsY();
    int YMax = inputMCCompHist->GetYaxis()->GetBinLowEdge(NBinsY + 1);
    float MCInt, SigInt, FOMCentVal, FOMStatErr;
    TString baseName = "h_FOM_forDiffMT2Cuts";
    switch (whichSystType) {
        case 0:
            baseName += "_CentVal";
            break;
        case 1:
            baseName += "_LepESShiftUp";
            break;
        case 2:
            baseName += "_LepESShiftDown";
            break;
        case 3:
            baseName += "_JetESShiftUp";
            break;
        case 4:
            baseName += "_JetESShiftDown";
            break;
        case 5:
            baseName += "_UncESShiftUp";
            break;
        case 6:
            baseName += "_UncESShiftDown";
            break;
        case 7:
            baseName += "_LepEffSFShiftUp";
            break;
        case 8:
            baseName += "_LepEffSFShiftDown";
            break;            
        case 9:
            baseName += "_genTopRW";
            break;
        case 10:
            baseName += "_StopXSecShiftUp";
            break;
        case 11:
            baseName += "_StopXSecShiftDown";
            break;
        case 12:
            baseName += "_BTagSFShiftUp";
            break;
        case 13:
            baseName += "_BTagSFShiftDown";
            break;
        default:
            break;
    }
    
    float Numerator, Denominator;
    TString axisString;
    if (typeFOM == 0) {
        axisString = "F.O.M. = #frac{S}{#sqrt{S + B}}, ";
        axisString += SigStringTitle;
        axisString += "; MT2_{ll} Cut [GeV]; MT2_{(lb)(lb)} Cut [GeV]";
    }
    else {
        axisString = "F.O.M. = #frac{S}{#sqrt{B}}, ";
        axisString += SigStringTitle;
        axisString += "; MT2_{ll} Cut [GeV]; MT2_{(lb)(lb)} Cut [GeV]";
    }
    TH2F * outHist = new TH2F(baseName, axisString, NBinsX, 0., XMax, NBinsY, 0., YMax); outHist->Sumw2();
    for (int xInd = 1; xInd < NBinsX + 1; ++xInd) {
        for (int yInd = 1; yInd < NBinsY + 1; ++yInd) {
            MCInt = inputMCCompHist->Integral(xInd, NBinsX + 1, yInd, NBinsY + 1);
            SigInt = inputMCSigHist->Integral(xInd, NBinsX + 1, yInd, NBinsY + 1);
            if (typeFOM == 0) {
                Numerator = SigInt;
                Denominator = TMath::Sqrt(SigInt + MCInt);
                if (penaltyTerm) {
                    Denominator = SigInt + MCInt; Denominator += ((0.2 * MCInt) * (0.2 * MCInt));
                    Denominator = TMath::Sqrt(Denominator);
                }
                FOMCentVal = Denominator > 0 ? Numerator / Denominator: 0;
                if (xInd == 22 && yInd == 3) {
                    cout << "MCInt " << MCInt << endl;
                    cout << "SigInt " << SigInt << endl;
                    cout << "FOMCentVal " << FOMCentVal << endl;
                }
                FOMStatErr = 0; //TMath::Sqrt(SigInt) * (SigInt + 2 * MCInt + SigInt * MCInt) / (2 * TMath::Power(SigInt + MCInt, 1.5));
            }
            else if (typeFOM == 1) {        
                Numerator  = SigInt;
                Denominator = TMath::Sqrt(MCInt);
                if (doVerbosity) {
                    cout << "xInd = " << xInd << endl;
                    cout << "yInd = " << yInd << endl;
                    cout << "Numerator " << Numerator << endl;
                    cout << "Denominator " << Denominator << endl;
                    cout << endl;
                }
                if (penaltyTerm) {
                    Denominator = MCInt; Denominator += ((0.2 * MCInt) * (0.2 * MCInt));
                    Denominator = TMath::Sqrt(Denominator);
                }
                FOMCentVal = Denominator > 0 ? Numerator / Denominator: 0;
                FOMStatErr = 0; //TMath::Sqrt(SigInt) * (SigInt + 2 * MCInt + SigInt * MCInt) / (2 * TMath::Power(SigInt + MCInt, 1.5));            
            }
            outHist->SetBinContent(xInd, yInd, FOMCentVal);
            outHist->SetBinError(xInd, yInd, FOMStatErr);
            outHist->GetZaxis()->SetRangeUser(0, 3);
        }
    }
    return outHist;
}