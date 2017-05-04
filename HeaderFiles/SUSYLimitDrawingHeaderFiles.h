#include "./BasicFunctions.h"
#include "./BasicFunctions_Math.h"
#include "./BasicFunctions_HistogramMaking.h"
#include "./BasicFunctions_SystemCommands.h"
#include "./HistogramSystematics2.h"
#include "./GeneralPlotDrawingStructs.h"
#include "./LimitScriptHeaderFiles/LimitScripts_ParametersStruct.h"
#include "./interpolate.h"

//SUSY functions/structures//
#include "./SUSYFunctions/SUSYFunctions_CompositeHeaderFiles.h"

#include "./LimitStructs/ScenarioDrawingParams.h"
#include "./LimitStructs/ScenarioDrawingParams.cc"
#include "./LimitScriptHeaderFiles/LimitScripts_SUSYStruct.h"
#include "./LimitStructs/SUSYT2LimitParams.h"
#include "./LimitStructs/SUSYT2LimitParams.cc"
//SUSY functions/structures//

//just added in
#include "./LimitScriptHeaderFiles/LimitFunctions_Saving.h"
#include "./LimitScriptHeaderFiles/YieldCalculationFunctions.h"
#include "./LimitScriptHeaderFiles/CardCreationFunctions.h"
#include "./LimitScriptHeaderFiles/LimitRunningFunctions.h"
#include "./LimitScriptHeaderFiles/LimitCombinationFunctions.h"
#include "./StopLimitMakingFunctions.h"
#include "./SmoothingFunctions_Structs/TwoDeeGaussianSmoothingHeaderFiles.h"
#include "./StopLimitMakingStructs.h"
#include "./SmoothingFunctions_Structs/LimitScenarioSmoother_Optimizer.h"
#include "./SmoothingFunctions_Structs/LimitScenarioSmoother_Optimizer.cc"


#include "./LimitStructs/MakeContourGraphFromHist.h"

//Drawing structures

#include "./DrawingStructs/AxisParams.h"
#include "./DrawingStructs/AxisParams.cc"

#include "./DrawingStructs/HistogramDrawingParams.h"
#include "./DrawingStructs/HistogramDrawingParams.cc"

#include "./DrawingStructs/GraphParams.h"
#include "./DrawingStructs/GraphParams.cc"

#include "./DrawingStructs/CanvasParams.h"
#include "./DrawingStructs/CanvasParams.cc"

#include "./LimitStructs/LimitDrawer.h"
#include "./LimitStructs/LimitDrawer_BasicFunctions.cc"
