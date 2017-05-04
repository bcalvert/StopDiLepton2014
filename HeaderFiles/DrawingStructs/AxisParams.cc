#ifndef AxisParams_cc
#define AxisParams_cc
void AxisParams::DefaultVals() {
    
}

void AxisParams::SetInputAxisParams(TAxis * inputAxis) {
    inputAxis->SetLabelFont(fontLabel);
    inputAxis->SetLabelSize(sizeLabel);

    inputAxis->SetTitleFont(fontTitle);
    inputAxis->SetTitleSize(sizeTitle);
    inputAxis->SetTitleOffset(offsetTitle);
    inputAxis->SetTitle(stringTitle);
}

void AxisParams::SetAsLimitAxis(int whichAxis, TString inputTitle) {
    //common to both x and y axes
    
    stringTitle = inputTitle;
    
    sizeLabel = 0.04;
    fontLabel = 42;
//    offsetLabel = ;
    sizeTitle = 0.05;
    fontTitle = 42;
    
    vector<float> vecTitleOffsets{0.0, 1.2, 1.3, 0.0};
    offsetTitle = vecTitleOffsets[whichAxis];
}

#endif