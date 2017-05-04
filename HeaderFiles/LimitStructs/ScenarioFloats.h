float GetMassBound_Onshell_W_b(int whichBound) {
    //0 xLB
    //1 xUB
    //2 yLB
    //3 yUB
    
    float arrBounds[4] = {87.5, 384, 12.5, 384 - 87.5 + 12.5};
    return arrBounds[whichBound];
}