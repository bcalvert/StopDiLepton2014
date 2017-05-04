void SetKinematicRange(XYBounds * inBounds_SUSYMasses, float binWidth) {
    /*
    float minStop = 100 - binWidth / 2;
    float maxStop = 800 + binWidth / 2;
    
    float minLSP = 0 - binWidth / 2;
    float maxLSP = 400 + binWidth / 2;
     */
    
    float minStop = 100;
    float maxStop = 800;
    
    float minLSP = 0;
    float maxLSP = 400;
    
    inBounds_SUSYMasses->xLB = minStop;
    inBounds_SUSYMasses->xUB = maxStop;
    inBounds_SUSYMasses->yLB = minLSP;
    inBounds_SUSYMasses->yUB = maxLSP;
}


void SetMassDiagonalKinematicLine_LatexStyle(LatexString * drawString, float xLB_MassDiag, float yLB_MassDiag, TString inText_MassDiag, float angleMassDiag = 60) {
    float sizeMassDiag = 0.03;
    
    int alignMassDiag = 12;
    int fontMassDiag = 42;
    
//    drawString->SetLatex(true, sizeMassDiag, alignMassDiag, xLB_MassDiag, yLB_MassDiag, inText_MassDiag);
    drawString->SetLatex(false, sizeMassDiag, alignMassDiag, xLB_MassDiag, yLB_MassDiag, inText_MassDiag);
    drawString->straightGhetto.textFont = fontMassDiag;
    drawString->straightGhetto.textAngle = angleMassDiag;
}

void SetMassDiagonalKinematicLine_LineStyle(TLine * drawLine) {
    drawLine->SetLineColor(1);
    drawLine->SetLineWidth(2);
    drawLine->SetLineStyle(2);
}

void SetKinematicLine_Onshell_Top(KinematicLine * inKL, float desiredEndX, float desiredEndY, bool hackText = false) {
    TString strOnShellTop = GetSUSYMass(0, false) + TString(" = m_{t} + ") + GetSUSYMass(1, false);
    
//    float xLB_MassDiag_Latex = 0.36;
//    float yLB_MassDiag_Latex = 0.47;
    float xLB_MassDiag_Latex = 335;
    float yLB_MassDiag_Latex = 195;
    
    
    float xLB_Onshell_Top= 173;
    float yLB_Onshell_Top= 0;
//    float yUB_Onshell_Top= xUB_Onshell_Top - xLB_Onshell_Top + yLB_Onshell_Top;
    float yUB_Onshell_Top= desiredEndY;
    //float xUB_Onshell_Top= 482.;
    float xUB_Onshell_Top = yUB_Onshell_Top - yLB_Onshell_Top + xLB_Onshell_Top;;
    if (desiredEndX > -1 && desiredEndX < xUB_Onshell_Top) {
        xUB_Onshell_Top = desiredEndX;
        yUB_Onshell_Top = xUB_Onshell_Top + yLB_Onshell_Top - xLB_Onshell_Top;
    }
    
    inKL->bounds_Line.SetBounds(xLB_Onshell_Top, yLB_Onshell_Top, xUB_Onshell_Top, yUB_Onshell_Top);
    
    float angleMassDiag = 60;
    if (hackText) {
    //if (true) {
        xLB_MassDiag_Latex = xLB_Onshell_Top + 0.2 * (xUB_Onshell_Top - xLB_Onshell_Top);
        yLB_MassDiag_Latex = yLB_Onshell_Top + 0.1 * (yUB_Onshell_Top - yLB_Onshell_Top);
        //yLB_MassDiag_Latex = yLB_Onshell_Top;
	//        angleMassDiag = 32.5;
        angleMassDiag = 40;
    }
    
    SetMassDiagonalKinematicLine_LineStyle(&inKL->drawLine);
    
    SetMassDiagonalKinematicLine_LatexStyle(&inKL->drawString, xLB_MassDiag_Latex, yLB_MassDiag_Latex, strOnShellTop, angleMassDiag);
}

void SetKinematicLine_Onshell_W_b(KinematicLine * inKL, float binWidth, float desiredEndX, float desiredEndY, bool hackText = false) {
    TString strOnShell_W_b = GetSUSYMass(0, false) + TString(" = ") + GetSUSYMass(1, false) + TString(" + m_{W} + m_{b}");
    float xLB_Onshell_W_b = 100 - (binWidth / 2);
//    float xUB_Onshell_W_b = 384;
    float yLB_Onshell_W_b = (binWidth / 2);
//    float yUB_Onshell_W_b = xUB_Onshell_W_b - xLB_Onshell_W_b + yLB_Onshell_W_b;
    float yUB_Onshell_W_b = desiredEndY;
    float xUB_Onshell_W_b = yUB_Onshell_W_b + xLB_Onshell_W_b - yLB_Onshell_W_b;
    if (desiredEndX > -1 && desiredEndX < xUB_Onshell_W_b) {
        xUB_Onshell_W_b = desiredEndX;
        yUB_Onshell_W_b = xUB_Onshell_W_b + yLB_Onshell_W_b - xLB_Onshell_W_b;
    }
    
    inKL->bounds_Line.SetBounds(xLB_Onshell_W_b, yLB_Onshell_W_b, xUB_Onshell_W_b, yUB_Onshell_W_b);
    
    SetMassDiagonalKinematicLine_LineStyle(&inKL->drawLine);
 
//    float xLB_MassDiag_Latex_Onshell_W_b = 0.198;
//    float yLB_MassDiag_Latex_Onshell_W_b = 0.34;
    
    float xLB_MassDiag_Latex_Onshell_W_b = 130;
    float yLB_MassDiag_Latex_Onshell_W_b = 90;
    
    float angleMassDiag = 60;
    if (hackText) {
      //if (true) {
        xLB_MassDiag_Latex_Onshell_W_b = xLB_Onshell_W_b + 0.15 * (xUB_Onshell_W_b - xLB_Onshell_W_b);
        yLB_MassDiag_Latex_Onshell_W_b = yLB_Onshell_W_b + 0.2 * (yUB_Onshell_W_b - yLB_Onshell_W_b);
	//        angleMassDiag = 32.5;
        angleMassDiag = 40;
    }
    
    SetMassDiagonalKinematicLine_LatexStyle(&inKL->drawString, xLB_MassDiag_Latex_Onshell_W_b, yLB_MassDiag_Latex_Onshell_W_b, strOnShell_W_b, angleMassDiag);
}

void SetKinematicLine_Onshell_W_CharginoDecay(KinematicLine * inKL, int charFrac, float desiredEndX, float desiredEndY, bool hackText = false) {
    TString strOnShellW_Chargino = GetSUSYMass(2, false) + TString(" = ") + GetSUSYMass(1, false) + TString(" + m_{W}");
    
    int index = (charFrac - 25) / 25;
    
    /*
    vector<float> vecLatexX_OnShellW_Chargino{0.423, 0.4211409, 0.238255};
    vector<float> vecLatexY_OnShellW_Chargino{0.2202797, 0.5192308, 0.2377622};
    */
    vector<float> vecLatexX_OnShellW_Chargino{500, 400, 350};
    vector<float> vecLatexY_OnShellW_Chargino{150, 200, 200};
    
    vector<float> vecTLine_xLB_OnShellW_Chargino{80./0.25, 80./0.50, 80./0.75};
    vector<float> vecTLine_yLB_OnShellW_Chargino{0, 0, 0};
//    vector<float> vecTLine_yUB_OnShellW_Chargino{vecTLine_xUB_OnShellW_Chargino[0] - vecTLine_xLB_OnShellW_Chargino[0], vecTLine_xUB_OnShellW_Chargino[1] - vecTLine_xLB_OnShellW_Chargino[1], vecTLine_xUB_OnShellW_Chargino[2] - vecTLine_xLB_OnShellW_Chargino[2]};
    //    vector<float> vecTLine_xUB_OnShellW_Chargino{629, 469., 416.};
    float yUB = desiredEndY;
    
    float angleMassDiag = 60;
    
    vector<float> vecTLine_xUB_OnShellW_Chargino(3);
    for (int iChar = 0; iChar < 3; ++iChar) {
        yUB = desiredEndY;
        vecTLine_xUB_OnShellW_Chargino[iChar] = desiredEndY - vecTLine_yLB_OnShellW_Chargino[iChar] + vecTLine_xLB_OnShellW_Chargino[iChar];
        /*
        cout << "vecTLine_xUB_OnShellW_Chargino[iChar] " << vecTLine_xUB_OnShellW_Chargino[iChar] << endl;
        cout << "vecTLine_yLB_OnShellW_Chargino[iChar] " << vecTLine_yLB_OnShellW_Chargino[iChar] << endl;
        cout << "vecTLine_xLB_OnShellW_Chargino[iChar] " << vecTLine_xLB_OnShellW_Chargino[iChar] << endl;
         */
        if (desiredEndX > -1 && desiredEndX < vecTLine_xUB_OnShellW_Chargino[iChar]) {
            vecTLine_xUB_OnShellW_Chargino[iChar] = desiredEndX;
            yUB = vecTLine_xUB_OnShellW_Chargino[iChar] + vecTLine_yLB_OnShellW_Chargino[iChar] - vecTLine_xLB_OnShellW_Chargino[iChar];
        }
	if (hackText) {
	  //if (true) {
            vecLatexX_OnShellW_Chargino[iChar] = vecTLine_xLB_OnShellW_Chargino[iChar] + 0.15 * (vecTLine_xUB_OnShellW_Chargino[iChar] - vecTLine_xLB_OnShellW_Chargino[iChar]);
            vecLatexY_OnShellW_Chargino[iChar] = vecTLine_yLB_OnShellW_Chargino[iChar] + 0.1 * (yUB - vecTLine_yLB_OnShellW_Chargino[iChar]);
            //angleMassDiag = 32.5;
	    angleMassDiag = 40;
        }
        if (iChar == index) {
            inKL->bounds_Line.SetBounds(vecTLine_xLB_OnShellW_Chargino[index], vecTLine_yLB_OnShellW_Chargino[index], vecTLine_xUB_OnShellW_Chargino[index], yUB);
        }
    }
    //    inKL->bounds_Line.SetBounds(vecTLine_xLB_OnShellW_Chargino[index], vecTLine_yLB_OnShellW_Chargino[index], vecTLine_xUB_OnShellW_Chargino[index], vecTLine_yUB_OnShellW_Chargino[index]);
    SetMassDiagonalKinematicLine_LatexStyle(&inKL->drawString, vecLatexX_OnShellW_Chargino[index], vecLatexY_OnShellW_Chargino[index], strOnShellW_Chargino, angleMassDiag);
    
    SetMassDiagonalKinematicLine_LineStyle(&inKL->drawLine);
    

}
