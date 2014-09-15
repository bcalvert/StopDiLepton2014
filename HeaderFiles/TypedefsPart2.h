#include <unordered_map>
#include <map>

//Would like to include this as part of Typedefs.h


typedef std::pair<HistogramT, TString> histKeyString;
/*
typedef map<histKeyString, TH1 *>      HMap_1D;
typedef map<histKeyString, TH2 *>      HMap_2D;
typedef map<histKeyString, TH3 *>      HMap_3D;
*/
typedef unordered_map<histKeyString, TH1 *, Hasher2, EqualFn2>      HMap_1D;
typedef unordered_map<histKeyString, TH2 *, Hasher2, EqualFn2>      HMap_2D;
typedef unordered_map<histKeyString, TH3 *, Hasher2, EqualFn2>      HMap_3D;