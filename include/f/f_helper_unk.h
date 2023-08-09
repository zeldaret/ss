#pragma once

// This file was ported from https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/framework/f_helper_unk.hpp

#include <types.h>

/// @brief [A helper class for fBase_c with unknown purpose].
/// @note Unofficial name.
class fBaHelper_c {
    s16 mStatus;
    s16 mCount;
    void **mpArr;
    void *filler1;
    s16 filler2;
    s16 mUnknown;

    virtual void vf_0x8(); ///< [Stripped out of binary].
    virtual int vf_0xc(void *); ///< [Stripped out of binary].
    virtual void vf_0x10(void *); ///< [Stripped out of binary].

public:
    void Delete(); ///< @todo Document this method.
    int Load(int); ///< @todo Document this method.
    bool LoadOnlyOne(); ///< @todo Document this method.
};