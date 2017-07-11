#ifndef ROOT_FILE_IO_H
#define ROOT_FILE_IO_H

#include "TString.h"

class ROOTFileIO {
public:
	TString const & GetFileIOString() const;

	ROOTFileIO() = default;
protected:
	TString m_tsInternalString; ///< Internal string used for managing strings as part of filenames
	virtual void SetString() = 0;
};

TString const & ROOTFileIO::GetFileIOString() const {
	return m_tsInternalString;
}

#endif // ROOT_FILE_IO_H