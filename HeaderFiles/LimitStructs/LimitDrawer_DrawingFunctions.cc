void LimitDrawer::InitializeCanvas() {
    drawCanv = new TCanvas("temp","temp");
    canvDrawParams.SetCanvas(drawCanv);
    
}
void LimitDrawer::FullDrawSequence() {
    canvDrawParams.DrawEmptyHisto();
    h_XSecLimitMap->Draw("same");
    
    DrawDiagonals();
    //self.DrawObsArea()
    DrawLines();
    DrawText();
    DrawLegend_BasicExclusion();
}

void LimitDrawer::DrawDiagonals() {
    unsigned int numKinematicDiags = inT2_DrawParams.vecKinematicLines.size();
    for (unsigned int iKin = 0; iKin < numKinematicDiags; ++iKin) {
        inT2_DrawParams.vecKinematicLines[iKin]->Draw("FSAME");
        inT2_DrawParams.vecKinematicLines[iKin]->Draw("LSAME");
    }
}

void LimitDrawer::DrawLines() {
    unsigned int currNumContours;
    for (int iObs = 0; iObs < numObs; ++iObs) {
        currNumContours = vecVecContours_Obs[iObs].size();
        for (int iContour = 0; iContour < currNumContours; ++iContour) {
            vecVecContours_Obs[iObs][iContour]->Draw("LSAME");
        }
    }
    
    for (int iExp = 0; iExp < numExp; ++iExp) {
        currNumContours = vecVecContours_Exp[iExp].size();
        for (int iContour = 0; iContour < currNumContours; ++iContour) {
            vecVecContours_Exp[iExp][iContour]->Draw("LSAME");
        }
    }
    
    for (int iExtra = 0; iExtra < numExtra; ++iExtra) {
        currNumContours = vecVecContours_Extra[iExtra].size();
        for (int iContour = 0; iContour < currNumContours; ++iContour) {
            vecVecContours_Extra[iExtra][iContour]->Draw("LSAME");
        }
    }
}

void LimitDrawer::DrawText_WhiteBackground() {
    drawCanv->cd();
    graph_WhiteBackground.Draw("FSAME");
    graph_WhiteBackground.Draw("LSAME");
}

void LimitDrawer::DrawText_CMS() {
    drawCanv->cd();
    latexCMS.Draw();
}

void LimitDrawer::DrawText_Model() {
    drawCanv->cd();
    latexModel.Draw();
}

void LimitDrawer::DrawText_All() {
    drawCanv->RedrawAxis();
    
    DrawWhiteBackground();
    DrawCMSText();
    DrawModelText();
}


void LimitDrawer::DrawLegend_BasicExclusion() {
    xRange = self.model.Xmax-self.model.Xmin
    yRange = self.model.Ymax-self.model.Ymin
    
    vector<TGraph> vecLegendHackGraphs_Obs(numObs);
    vector<TGraph> vecLegendHackGraphs_Exp(numExp);
    
    pairFloats xPair(Xmin, xRange);
    pairFloats yPair(Ymax, yRange);
    
    LExpM.SetPoint(0,self.model.Xmin+3*xRange/100, self.model.Ymax-2.15*yRange/100*10)
    LExpM.SetPoint(1,self.model.Xmin+10*xRange/100, self.model.Ymax-2.15*yRange/100*10)
    
    for (int iObs = 0; iObs < numObs; ++iObs) {
        vecGPs_Obs[iExp].SetInputTGraphForLegendHack(&vecLegendHackGraphs_Obs[iObs], &xPair, &yPair, true, iObs);
    }
    
    for (int iExp = 0; iExp < numExp; ++iExp) {
        vecGPs_Exp[iExp].SetInputTGraphForLegendHack(&vecLegendHackGraphs_Exp[iExp], &xPair, &yPair, false, iExp);
    }
    drawCanv->cd();
    
    latexObsHack.Draw();
    latexExpHack.Draw();
    
    for (int iObs = 0; iObs < numObs; ++iObs) {
        vecLegendHackGraphs_Obs[iObs]->Draw("LSAME");
    }
    
    for (int iExp = 0; iExp < numExp; ++iExp) {
        vecLegendHackGraphs_Exp[iExp]->Draw("LSAME");
    }
}