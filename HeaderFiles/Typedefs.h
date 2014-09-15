#include <unordered_map>
#include <map>

typedef unordered_map<TString, TString, Hasher, EqualFn> labelMap;
typedef unordered_map<TString, float, Hasher, EqualFn> StV_Map;
typedef unordered_map<TString, bool, Hasher, EqualFn> passCutMap;
typedef unordered_map<string, int> strInt_Map;

typedef unordered_map<int, BasicEventInfo *> mapIntBEI;
typedef unordered_map<int, EventMETInfo *> mapIntEMI;
typedef unordered_map<int, EventLepInfo *> mapIntELI;
typedef unordered_map<int, EventJetInfo *> mapIntEJI;
typedef unordered_map<int, EventDiStructureInfo *> mapIntEDSI;
typedef unordered_map<int, bool> mIB;