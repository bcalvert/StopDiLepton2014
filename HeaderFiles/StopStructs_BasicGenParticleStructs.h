///Structs that cover very basic variables for generator-level particle


using namespace std;
typedef struct GenJet{
    ///struct to contain relevant info for generator level jets
    TLorentzVector  P4;
    BasicVecComponents BVC;
    int             seedPDGID;
    float           invisE;
    bool            isBJet;
    void ClearVars() {
        DefaultLorentzVector(&P4);
        BVC.ExtractParams(&P4);
        seedPDGID = 0;
        invisE = 0.0;
        isBJet = false;
    }
} GenJet;
inline bool operator<(const GenJet &a, const GenJet &b)
{
    return (a.BVC.Vec_Pt < b.BVC.Vec_Pt);
}

inline bool operator>(const GenJet &a, const GenJet &b)
{
    return (a.BVC.Vec_Pt > b.BVC.Vec_Pt);
}
typedef struct GenParticle{
    //struct to contain info for generator-level particles
    TLorentzVector P4;
    BasicVecComponents BVC;
    int            PDGID;
    int            PDGStatus;
    int            MomPDGID;
    int            MomPDGStatus;
    float          Mass;
    void ClearVals() {
        DefaultLorentzVector(&P4);
        BVC.ExtractParams(&P4);
        PDGID = 0;
        PDGStatus = -1;
        MomPDGID = 0;
        MomPDGStatus = -1;
        Mass = 0.;
    }
    void PrintVals() {
        cout << "Gen Particle Vec Components: " << endl;
        PrintLorentzVector(&P4);
        cout << "Gen Particle PDGID " << PDGID << endl;
        cout << "Gen Particle Status " << PDGStatus << endl;
        cout << "Gen Particle Mass " << Mass << endl;
        cout << "Mom Particle PDGID " << MomPDGID << endl;
        cout << "Mom Particle PDGStatus " << MomPDGStatus << endl;
    }
} GenParticle;

inline bool operator<(const GenParticle &a, const GenParticle &b)
{
    // sort based on PDG status (status 1 takes priority, ratcheting down after that)
    // then among those with same PDGstatus, sort based on PDGID number
    // then among those with same PDGID number -- either anti- or normal particle versions
    // sort based on Pt
    bool LessThan;
    if (a.PDGStatus == b.PDGStatus) {
        if (abs(a.PDGID) == abs(b.PDGID)) {
            LessThan = a.BVC.Vec_Pt < b.BVC.Vec_Pt;
        }
        else {
            LessThan = (abs(a.PDGID) > abs(b.PDGID));
        }
    }
    else {
        LessThan = (a.PDGStatus > b.PDGStatus);
    }
    return LessThan;
}

inline bool operator>(const GenParticle &a, const GenParticle &b)
{
    // sort based on PDG status (status 1 takes priority, ratcheting down after that)
    // then among those with same PDGstatus, sort based on PDGID number
    // then among those with same PDGID number -- either anti- or normal particle versions
    // sort based on Pt
    bool GreaterThan;
    if (a.PDGStatus == b.PDGStatus) {
        if (abs(a.PDGID) == abs(b.PDGID)) {
            GreaterThan = a.BVC.Vec_Pt > b.BVC.Vec_Pt;
        }
        else {
            GreaterThan = (abs(a.PDGID) < abs(b.PDGID));
        }
    }
    else {
        GreaterThan = (a.PDGStatus < b.PDGStatus);
    }
    return GreaterThan;
}

typedef struct GenParticleSt3Info {
    //Status3 GenParticle with more information than just the gen particle itself (also has some mother info)
    
    GenParticle GP;
    float GP_FirstMomPt;
    int   GP_Index, GP_FirstMom, GP_FirstMomPDGID;
    void BadParticle() {
        GP.ClearVals();
        GP_Index = -1;
        GP_FirstMom = -1;
        GP_FirstMomPDGID = -1;
        GP_FirstMomPt = -1.;
    }
    void SetP4() {
        GP.BVC.SetP4Vals(&GP.P4);
    }
    void ExtractP4Params() {
        GP.BVC.ExtractParams(&GP.P4);
    }
    void PrintInfo() {
        cout << "print Gen Particle Info in Status 3" << endl;
        GP.PrintVals();
        cout << "GP_Index " << GP_Index << endl;
        cout << "GP_FirstMom " << GP_FirstMom << endl;
        cout << "GP_FirstMomPDGID " << GP_FirstMomPDGID << endl;
        cout << "GP_FirstMomPt " << GP_FirstMomPt << endl;
    }
    // functions used as part of saving or loading the gen particle info
    void SetOutBranches(TTree * outTree, TString prefixBranchName) {
        outTree->Branch( prefixBranchName + TString("_PDGID"),      &GP.PDGID );
        outTree->Branch( prefixBranchName + TString("_Index"),      &GP_Index );
        outTree->Branch( prefixBranchName + TString("_Pt"),         &GP.BVC.Vec_Pt );
        outTree->Branch( prefixBranchName + TString("_Eta"),        &GP.BVC.Vec_Eta );
        outTree->Branch( prefixBranchName + TString("_Phi"),        &GP.BVC.Vec_Phi );
        outTree->Branch( prefixBranchName + TString("_Energy"),     &GP.BVC.Vec_En );
        outTree->Branch( prefixBranchName + TString("_FirstMom"),       &GP_FirstMom );
        outTree->Branch( prefixBranchName + TString("_FirstMomPt"),     &GP_FirstMomPt );
        outTree->Branch( prefixBranchName + TString("_FirstMomPDGID"),  &GP_FirstMomPDGID );
    }
    void SetInBranches(TTree * inTree, TString prefixBranchName) {
        inTree->SetBranchAddress( prefixBranchName + TString("_PDGID"),      &GP.PDGID );
        inTree->SetBranchAddress( prefixBranchName + TString("_Index"),      &GP_Index );
        inTree->SetBranchAddress( prefixBranchName + TString("_Pt"),         &GP.BVC.Vec_Pt );
        inTree->SetBranchAddress( prefixBranchName + TString("_Eta"),        &GP.BVC.Vec_Eta );
        inTree->SetBranchAddress( prefixBranchName + TString("_Phi"),        &GP.BVC.Vec_Phi );
        inTree->SetBranchAddress( prefixBranchName + TString("_Energy"),     &GP.BVC.Vec_En );
        inTree->SetBranchAddress( prefixBranchName + TString("_FirstMom"),       &GP_FirstMom );
        inTree->SetBranchAddress( prefixBranchName + TString("_FirstMomPt"),     &GP_FirstMomPt );
        inTree->SetBranchAddress( prefixBranchName + TString("_FirstMomPDGID"),  &GP_FirstMomPDGID );
    }
} GenParticleSt3Info ;


typedef struct GenParticleSt1Info {
    //Status3 GenParticle with more information than just the gen particle itself (also has full gen-mother info)
    GenParticle GP;
    GenParticle GP_Mom;
    void BadParticle() {
        GP.ClearVals();
        GP_Mom.ClearVals();
    }
    void SetP4() {
        GP.BVC.SetP4Vals(&GP.P4);
        GP_Mom.BVC.SetP4Vals(&GP_Mom.P4);
    }
    void ExtractP4Params() {
        GP.BVC.ExtractParams(&GP.P4);
        GP_Mom.BVC.ExtractParams(&GP_Mom.P4);
    }
    void PrintInfo() {
        cout << "print Gen Particle Info in Status 1" << endl;
        GP.PrintVals();
        cout << "print Gen Mom Particle Info in Status 1" << endl;
        GP_Mom.PrintVals();
    }
    GenParticle * GetBQuarkFromTopOrChargino() {
        GenParticle * nullGP = NULL;
        if (abs(GP.PDGID) == 5 && (abs(GP.MomPDGID) == 6 || abs(GP.MomPDGID) == 1000006)) {
            return &GP;
        }
        return nullGP;
    }
    GenParticle * GetLepFromW() {
        GenParticle * nullGP = NULL;
        if ((abs(GP.PDGID) == 11 || abs(GP.PDGID) == 13) && abs(GP.MomPDGID) == 24) {
            return &GP;
        }
        return nullGP;
    }
    // functions used as part of saving or loading the gen particle info
    void SetOutBranches(TTree * outTree, TString prefixBranchName) {
        outTree->Branch( prefixBranchName + TString("_PID"),       &GP.PDGID );
        outTree->Branch( prefixBranchName + TString("_Pt"),        &GP.BVC.Vec_Pt );
        outTree->Branch( prefixBranchName + TString("_Eta"),       &GP.BVC.Vec_Eta );
        outTree->Branch( prefixBranchName + TString("_Phi"),       &GP.BVC.Vec_Phi );
        outTree->Branch( prefixBranchName + TString("_Energy"),    &GP.BVC.Vec_En );
        outTree->Branch( prefixBranchName + TString("_MomPID"),    &GP_Mom.PDGID );
        outTree->Branch( prefixBranchName + TString("_MomPt"),     &GP_Mom.BVC.Vec_Pt );
        outTree->Branch( prefixBranchName + TString("_MomEta"),    &GP_Mom.BVC.Vec_Eta );
        outTree->Branch( prefixBranchName + TString("_MomPhi"),    &GP_Mom.BVC.Vec_Phi );
        outTree->Branch( prefixBranchName + TString("_MomEnergy"), &GP_Mom.BVC.Vec_En );
        outTree->Branch( prefixBranchName + TString("_MomStat"),   &GP_Mom.PDGStatus );
    }
    void SetInBranches(TTree * inTree, TString prefixBranchName) {
        inTree->SetBranchAddress( prefixBranchName + TString("_PID"),       &GP.PDGID );
        inTree->SetBranchAddress( prefixBranchName + TString("_Pt"),        &GP.BVC.Vec_Pt );
        inTree->SetBranchAddress( prefixBranchName + TString("_Eta"),       &GP.BVC.Vec_Eta );
        inTree->SetBranchAddress( prefixBranchName + TString("_Phi"),       &GP.BVC.Vec_Phi );
        inTree->SetBranchAddress( prefixBranchName + TString("_Energy"),    &GP.BVC.Vec_En );
        inTree->SetBranchAddress( prefixBranchName + TString("_MomPID"),    &GP_Mom.PDGID );
        inTree->SetBranchAddress( prefixBranchName + TString("_MomPt"),     &GP_Mom.BVC.Vec_Pt );
        inTree->SetBranchAddress( prefixBranchName + TString("_MomEta"),    &GP_Mom.BVC.Vec_Eta );
        inTree->SetBranchAddress( prefixBranchName + TString("_MomPhi"),    &GP_Mom.BVC.Vec_Phi );
        inTree->SetBranchAddress( prefixBranchName + TString("_MomEnergy"), &GP_Mom.BVC.Vec_En );
        inTree->SetBranchAddress( prefixBranchName + TString("_MomStat"),   &GP_Mom.PDGStatus );
    }
} GenParticleSt1Info ;
