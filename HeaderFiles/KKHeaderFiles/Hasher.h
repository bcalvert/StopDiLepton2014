#ifndef HASHER_H
#define HASHER_H

#include "TString.h"
class Hasher {
public:
    size_t operator() (TString const& key) const {
//        cout << "Hashing called";
        size_t hash = key.Hash();
        return hash;
    }
};
class EqualFn {
public:
    bool operator() (TString const& t1, TString const& t2) const
    {
//        cout << "Equal called";
        return t1.EqualTo(t2);
    }
};
#endif // HASHER_H