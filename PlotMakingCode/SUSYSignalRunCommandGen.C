#include <iostream>
#include <vector>
#include "TString.h"
#include "TMath.h"
using namespace std;
int main( int argc, const char* argv[] ) {
    
    vector<int> vec_StopMass(0), vec_ChiZMass(0);
    
    int massStopLB, massStopUB;
    int massLSPLB, massLSPUB;
    
    bool doShowFracT2bw = 0;
    
    TString fracT2bw, typeT2;
    for (int k = 0; k < argc; ++k) {
        cout << "argv[k] for k = " << k << " is: " << argv[k] << endl;
        if (strncmp (argv[k], "-mSLB", 5) == 0) {
            massStopLB = strtol(argv[k+1], NULL, 10 );
        }
        else if (strncmp (argv[k], "-mSUB", 5) == 0) {
            massStopUB = strtol(argv[k+1], NULL, 10 );
        }
        else if (strncmp (argv[k], "-mLLB", 5) == 0) {
            massLSPLB = strtol(argv[k+1], NULL, 10 );
        }
        else if (strncmp (argv[k], "-mLUB", 5) == 0) {
            massLSPUB = strtol(argv[k+1], NULL, 10 );
        }
        else if (strncmp (argv[k], "-fT2bw", 6) == 0) {
            fracT2bw = TString(argv[k+1]);
            doShowFracT2bw = 1;
        }
        else if (strncmp (argv[k], "-typeT2", 7) == 0) {
            typeT2 = TString(argv[k+1]);
        }
    }
    
    int roundMult = 25;
    massStopLB = (int) (roundMult * ((massStopLB + (roundMult / 2)) / roundMult));
    massStopUB = (int) (roundMult * ((massStopUB + (roundMult / 2)) / roundMult));
    massLSPLB = (int) (roundMult * ((massLSPLB + (roundMult / 2)) / roundMult));
    massLSPUB = (int) (roundMult * ((massLSPUB + (roundMult / 2)) / roundMult));
    
    int currStopMass = massStopLB;
    int currChi0Mass;
    
    int stepSize = 25;
    
    if (currStopMass <= massStopUB) {
        while (currStopMass <= massStopUB) {
            currChi0Mass = massLSPLB;
            if (currChi0Mass >= massLSPUB) {
                cout << "{currStopMass:currChi0Mass} {" << currStopMass << ":" << currChi0Mass << "}" << endl;
            }
            else {
                while (currChi0Mass <= TMath::Min(currStopMass - 100, massLSPUB)) {
                    cout << "{currStopMass:currChi0Mass} {" << currStopMass << ":" << currChi0Mass << "}" << endl;
                    currChi0Mass += 25;
                }
            }
            currStopMass += stepSize;
        }
    }
    else {
        currChi0Mass = massLSPLB;
        if (currChi0Mass >= massLSPUB) {
            cout << "{currStopMass:currChi0Mass} {" << currStopMass << ":" << currChi0Mass << "}" << endl;
        }
        else {
            while (currChi0Mass <= TMath::Min(currStopMass - 100, massLSPUB)) {
                cout << "{currStopMass:currChi0Mass} {" << currStopMass << ":" << currChi0Mass << "}" << endl;
                currChi0Mass += 25;
            }
        }
    }
    cout << "typeT2:" << typeT2 << endl;
    if (doShowFracT2bw) {
        cout << "frac:" << fracT2bw << endl;
    }
}