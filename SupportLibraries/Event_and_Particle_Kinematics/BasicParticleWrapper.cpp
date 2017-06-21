#include "BasicParticleWrapper.h"
bool operator< (const BasicParticleWrapper &a, const BasicParticleWrapper &b)
{
    return a.m_BVC.GetPt() < b.m_BVC.GetPt();
}