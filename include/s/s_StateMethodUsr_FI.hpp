#ifndef S_STATEMETHODUSR_FI_H
#define S_STATEMETHODUSR_FI_H

#include "s/s_StateMethod.hpp"

// Note: Ported from https://github.com/NSMBW-Community/NSMBW-Decomp/tree/master/include/dol/sLib
// See include/s/README.txt for changes made

/// @brief An extension to sStateMethod_c that implements the remaining abstract methods.
/// @details [Name might mean "Functionality implementation"].
/// @ingroup state
class sStateMethodUsr_FI_c : public sStateMethod_c {
public:
    sStateMethodUsr_FI_c(sStateIDChkIf_c &checker, sStateFctIf_c &factory, const sStateIDIf_c &initialState);
    virtual ~sStateMethodUsr_FI_c();

    virtual int initializeStateLocalMethod();
    virtual void executeStateLocalMethod();
    virtual void finalizeStateLocalMethod();
    virtual void changeStateLocalMethod(const sStateIDIf_c &newStateID);
};

#endif
