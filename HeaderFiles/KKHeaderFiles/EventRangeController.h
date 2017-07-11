#ifndef EVENT_RANGE_CONTROLLER_H
#define EVENT_RANGE_CONTROLLER_H

#include "ROOTFileIO.h"

class EventRangeController : public ROOTFileIO {
public:
    void SetString();
    void SetVals(int argc, char * argv[]);
    EventRangeController();
    int const & GetNEvents() const;
    void SetNEvents(int iNEvents) {m_iNEvents = iNEvents;};
    size_t const & GetStartPoint() const {return m_szStartPoint;}
private:
    bool m_bIsLimStats; ///< For whether one wants to just run a limited number of events
    int m_iNEvents; ///< Number of events to run over
    size_t m_szStartPoint; ///< Event to start on
};

EventRangeController::EventRangeController() 
: m_bIsLimStats(false)
, m_iNEvents(-1)
, m_szStartPoint(0)
{}

int const & EventRangeController::GetNEvents() const
{
    return m_iNEvents;
}

void EventRangeController::SetString()
{
    m_tsInternalString = ""; //TODO maybe better clear function
    if (m_bIsLimStats) {
        m_tsInternalString += "_isLimStats_";
        m_tsInternalString += m_iNEvents;
    }
    if (m_szStartPoint != 0) {
        m_tsInternalString += "_startEvent_";
        m_tsInternalString += m_szStartPoint;
    }
}

void EventRangeController::SetVals(int argc, char * argv[])
{
    for (int iERCArg = 0; iERCArg < argc; ++iERCArg) {
        if (strncmp (argv[iERCArg],"-sInd", 5) == 0) {
            m_szStartPoint = static_cast<size_t>(strtol(argv[iERCArg+1], nullptr, 10));
        }
        else if (strncmp (argv[iERCArg],"-limStats", 9) == 0) {
            m_bIsLimStats = true;
            m_iNEvents = static_cast<int>(strtol(argv[iERCArg+1], nullptr, 10));
        }
    }
}


#endif // EVENT_RANGE_CONTROLLER_H