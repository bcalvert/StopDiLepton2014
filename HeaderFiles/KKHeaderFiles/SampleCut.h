#ifndef SAMPLE_CUT_H
#define SAMPLE_CUT_H

#include "TString.h"
#include <iostream>

// Structures used as part of setting up sub-sample cuts (events with 2 or more jets, events with MET Geq 40, etc.) for either whole swaths of histograms or as an individual cut for a given histogram
// Are broken down into cutting based on floats or integers

// m_tsHistNameSuffix is the suffix added to the histogram, if any (used for swaths of histograms
// m_tCutLB and m_tCutUB are the two bounds of the cut value to cut on
// m_bDoCut is a boolean for whether or not to the cut
// m_iCutDir is whether to consider the second part of the bound, m_tCutUB, and whether m_tCutLB is a lower bound or not

template<typename T>
class SampleCut {
public:
    bool m_bDoCut;
    TString m_tsHistNameSuffix;
    T m_tCutLB, m_tCutUB; ///
                          ///\todo general cut object
    int   m_iCutDir;
    void SetParams(SampleCut<T> * pOtherSC);
    void SetParamsWindow(TString tsHistNameSuffix, int tCutLB, int tCutUB, int iCutDir, bool bDoCut = true);
    void SetParamsBound(TString tsHistNameSuffix, int tCutLB, int iCutDir, bool bDoCut = true);
    bool PassCut(T tCheckVal, bool bDoVerbosity = false);

    SampleCut() = default;

    /// Print method
    /// \param[in] os   : output stream
    /// \param[in] sample_cut : SampleCut<T> object to print
    /// \return         : output stream
    template <typename U>
    friend std::ostream& operator<<(std::ostream &os, SampleCut<U> const &sample_cut);
private:
    ///\todo work on proper public/private set-up
};

template <typename T>
std::ostream& operator<<(std::ostream &os, SampleCut<T> const &sample_cut);

/*
template<typename T>
SampleCut<T>::SampleCut();
*/

template<typename T>
void SampleCut<T>::SetParams(SampleCut<T> * pOtherSC)
{
    m_bDoCut = pOtherSC->m_bDoCut;
    m_tsHistNameSuffix = pOtherSC->m_tsHistNameSuffix;
    m_tCutLB = pOtherSC->m_tCutLB;
    m_tCutUB = pOtherSC->m_tCutUB;
    m_iCutDir = pOtherSC->m_iCutDir;
}

template<typename T>
void SampleCut<T>::SetParamsWindow(TString tsHistNameSuffix, int tCutLB, int tCutUB, int iCutDir, bool bDoCut)
{
    m_bDoCut = bDoCut;
    m_tsHistNameSuffix = tsHistNameSuffix;
    m_tCutLB = tCutLB;
    m_tCutUB = tCutUB;
    m_iCutDir = iCutDir;
}
    template<typename T>
void SampleCut<T>::SetParamsBound(TString tsHistNameSuffix, int tCutLB, int iCutDir, bool bDoCut)
{
    m_bDoCut = bDoCut;
    m_tsHistNameSuffix = tsHistNameSuffix;
    m_tCutLB = tCutLB;
    m_iCutDir = iCutDir;
}



/// Print method
/// \param[in] os   : output stream
/// \param[in] sample_cut : SampleCut<T> object to print
/// \return         : output stream
template <typename T>
std::ostream& operator<<(std::ostream &os, SampleCut<T> const& sample_cut) 
{
    os << "tCheckVal " << sample_cut.tCheckVal << std::endl;
    os << "m_tCutLB " << sample_cut.m_tCutLB << std::endl;
    os << "m_tCutUB " << sample_cut.m_tCutUB << std::endl;
    os << "m_iCutDir " << sample_cut.m_iCutDir << std::endl;
    os << "m_bDoCut " << sample_cut.m_bDoCut << std::endl;
    return os;
}

template<typename T>
bool SampleCut<T>::PassCut(T tCheckVal, bool bDoVerbosity) {
    bool bPassCut = true;
    if (bDoVerbosity) {
        std::cout << *this;
    }
    if (m_bDoCut) {
        switch (m_iCutDir) {
            case -2:
            bPassCut = (tCheckVal <= m_tCutLB && tCheckVal >= m_tCutUB);
            break;
            case -1:
            bPassCut = tCheckVal <= m_tCutLB;
            break;
            case 0:
            bPassCut = tCheckVal == m_tCutLB;
            case 1:
            bPassCut = tCheckVal >= m_tCutLB;
            break;
            case 2:
            bPassCut = (tCheckVal >= m_tCutLB && tCheckVal <= m_tCutUB);
            break;
            default:
            std::cout << "issues with checking based on m_iCutDir " << m_iCutDir << std::endl;
            break;
        }
    }
    if (bDoVerbosity) {
        std::cout << "bPassCut? " << bPassCut << std::endl;
    }
    return bPassCut;
}
#endif // SAMPLE_CUT_H
