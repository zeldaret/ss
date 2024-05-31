#pragma once
#include <s/s_StateInterfaces.hpp>

/// @brief A default implementation of a state ID checker.
/// @details ::isNormalID always returns true.
class sStateIDChk_c : public sStateIDChkIf_c {
public:
    virtual ~sStateIDChk_c();
    virtual bool isNormalID(const sStateIDIf_c &) const { return true; }
};
