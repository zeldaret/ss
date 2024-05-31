#pragma once
#include <s/s_StateInterfaces.hpp>
#include <s/s_FState.hpp>
#include <s/s_StateID.hpp>

// Note: Ported from https://github.com/NSMBW-Community/NSMBW-Decomp/tree/master/include/dol/sLib
// See include/s/README.txt for changes made

/// @brief A state factory for a given class.
/// @tparam T The class that this state belongs to.
/// @ingroup state
template <class T>
class sFStateFct_c : public sStateFctIf_c {
public:
    sFStateFct_c(T &owner) : mState(owner) {}

    virtual sStateIf_c* build(const sStateIDIf_c &id) {
        if (!id.isNull()) {
            mState.setID((const sFStateID_c<T> *) &id);
            return &mState;
        }
        return nullptr;
    }

    virtual void dispose(sStateIf_c *&id) { id = nullptr; }

private:
    sFState_c<T> mState;
};
