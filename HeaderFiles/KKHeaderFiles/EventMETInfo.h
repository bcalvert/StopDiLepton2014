#ifndef EVENT_MET_INFO_H
#define EVENT_MET_INFO_H

#include "TString.h"
#include "TTree.h"
#include <iostream>

TString SystString(int whichSyst = 0) {
    TString suffixSyst[8] = {"", "LepES", "JetES", "BTagEffSF", "BMisTagSF", "JetSmear", "UncES", "UncESHack"};
    TString outString = "";
    if (whichSyst == 0) {
        return outString;
    }
    else {
        outString = "_";
        outString += suffixSyst[abs(whichSyst)];
        if (whichSyst < 0) outString += "Down";
        else if (whichSyst > 0) outString += "Up";
    }
    return outString;
}

class EventMETInfo {
private:
    TString m_tsSystematic;
    TString m_tsSmearing;
    TString m_tsMETType;

    float m_fEventMET, m_fEventMETPhi;
    int m_iEventMETType;
    int m_iSystematicType;
    int m_bIsSmeared;
public:
    EventMETInfo() = default;
    EventMETInfo(int iEventMETType, int iSystematicType, bool bIsSmeared);
    void PrintVals();
    void SetStrings();
    void SetTreeBranchInfo(TTree * pSkimTree);
};

EventMETInfo::EventMETInfo(int iEventMETType, int iSystematicType, bool bIsSmeared)
: EventMETInfo()
{
    m_iEventMETType = iEventMETType;
    m_iSystematicType = iSystematicType;
    m_bIsSmeared = bIsSmeared;

}

void EventMETInfo::SetStrings()
{
    m_tsSmearing = m_bIsSmeared ? "Smear" : "";

    std::vector<TString> vTSMETType{"PF", "", "", "", "Calo"};
    m_tsMETType = vTSMETType.at(m_iEventMETType);

    m_tsSystematic = SystString(m_iSystematicType);
}

void EventMETInfo::SetTreeBranchInfo(TTree * pSkimTree)
{
    pSkimTree->SetBranchAddress(TString("T") + m_tsSmearing + m_tsMETType + TString("MET") + m_tsSystematic, &m_fEventMET);
    pSkimTree->SetBranchAddress(TString("T") + m_tsSmearing + m_tsMETType + TString("MET_Phi") + m_tsSystematic, &m_fEventMETPhi);    
}

void EventMETInfo::PrintVals()
{
  std::cout << "EventMET " << m_fEventMET << std::endl;
  std::cout << "EventMETPhi " << m_fEventMETPhi << std::endl;
}

#endif // EVENT_MET_INFO_H
