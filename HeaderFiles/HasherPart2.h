#include "TString.h"
///Nominally, I'd like to get this included into the main Hasher.h
class Hasher2 {
public:
    size_t operator() (histKeyString const& key) const {
//        cout << "Hashing called";
        size_t hash = (key.first.name + key.second).Hash();
        return hash;
    }
};
class EqualFn2 {
public:
    bool operator() (histKeyString const& t1, histKeyString const& t2) const
    {
//        cout << "Equal called";
        return (t1.first.name + t1.second).EqualTo(t2.first.name + t2.second);
    }
};