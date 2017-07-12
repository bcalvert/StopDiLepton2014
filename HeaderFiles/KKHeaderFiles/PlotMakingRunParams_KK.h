#ifndef PLOT_MAKING_RUN_PARAMS_KK_H
#define PLOT_MAKING_RUN_PARAMS_KK_H

#include "EventRangeController.h"
#include "TString.h"
class PlotMakingRunParamsKK {
public:
	void SetVals(int argc, char * argv[]);
	PlotMakingRunParamsKK() = default;
	TString const GetOutputString() const;
	TString const &GetFileName() const;
	void SetStrings();

	void SetNEvents(int iNEvents) {m_ERC.SetNEvents(iNEvents);}
    int GetNEvents() const { return m_ERC.GetNEvents();}
    size_t GetStartPoint() const { return m_ERC.GetStartPoint();}
private:
	EventRangeController m_ERC; ///< Controls the event range run over
	TString m_tsFileName; 		///< File name
};


void PlotMakingRunParamsKK::SetVals(int argc, char * argv[]) {
	m_ERC.SetVals(argc, argv);
	for (int iPMRPArg = 0; iPMRPArg < argc; ++iPMRPArg) {
		if (strncmp (argv[iPMRPArg],"-i", 2) == 0) {
			m_tsFileName = TString(argv[iPMRPArg + 1]);
		}
	}
}

void PlotMakingRunParamsKK::SetStrings()
{
	m_ERC.SetString();
}

TString const & PlotMakingRunParamsKK::GetFileName() const
{
	return m_tsFileName;
}

TString const PlotMakingRunParamsKK::GetOutputString() const
{
	TString tsOutput = m_tsFileName;

	//Get additional strings
	TString tsAddition = m_ERC.GetFileIOString();
	tsOutput += tsAddition;

	//final addition
	tsOutput += "_Output.root";
	return tsOutput;
}

#endif // PLOT_MAKING_RUN_PARAMS_KK_H