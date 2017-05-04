void LimitDrawer::DefaultVals() {
    canvDrawParams.DefaultVals();
    
    numObs = 0;
    numExp = 0;
    numExtra = 0;
    
    intLumi = 19.7;
    beamEnergy = 8;
    textPrelim = " Preliminary";
}



void LimitDrawer::FullSetup_BasicExclusion() {
    SetBasicParams();
    SetGraphParams_BasicExclusion();
    SetText_All();
}

void LimitDrawer::SetScenarioDrawingParams(SUSYT2LimitParams * inSUSYT2LP) {
    inT2_DrawParams.SetScenario(inSUSYT2LP);
}


void LimitDrawer::SetBasicParams() {
    latexCMS_X = 0.22;
    latexCMS_Y = 0.98;
    latexModel_X = 0.16;
    latexModel_Y = 0.90;

    latexObsHack_X = inT2_DrawParams.xBounds.first + 11 * inT2_DrawParams.xRange / 100;
    latexObsHack_Y = inT2_DrawParams.yBounds.second - 1.5 * inT2_DrawParams.yRange/10;
    
    latexExpHack_X = inT2_DrawParams.xBounds.first + 11 * inT2_DrawParams.xRange / 100;
    latexExpHack_Y = inT2_DrawParams.yBounds.second - 2.15 * inT2_DrawParams.yRange/10;
    
    numObs = 3;
    numExp = 3;
    numExtra = 0;
    
}


void LimitDrawer::SetGraphParams_BasicExclusion() {
    vecGPs_Obs.resize(numObs);
    vecGPs_Exp.resize(numExp);
    
    for (int iObs = 0; iObs < numObs; ++iObs) {
        vecGPs_Obs[iObs].SetAsBasicLimitContour(true, iObs);
    }
    
    for (int iExp = 0; iExp < numExp; ++iExp) {
        vecGPs_Exp[iExp].SetAsBasicLimitContour(true, iExp);
    }
}