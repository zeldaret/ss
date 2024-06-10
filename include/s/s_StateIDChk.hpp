#ifndef S_STATEIDCHK_H
#define S_STATEIDCHK_H

#include <s/s_StateInterfaces.hpp>

// Note: Ported from https://github.com/NSMBW-Community/NSMBW-Decomp/tree/master/include/dol/sLib
// See include/s/README.txt for changes made

/// @brief A default implementation of a state ID checker.
/// @details ::isNormalID always returns true.
class sStateIDChk_c : public sStateIDChkIf_c {
public:
    virtual ~sStateIDChk_c();
    virtual bool isNormalID(const sStateIDIf_c &) const { return true; }
};

#endif
