#ifndef S_FSTATE_VIRTUAL_ID_H
#define S_FSTATE_VIRTUAL_ID_H

#include "s/s_FStateID.hpp"
#include "s/s_StateID.hpp"

template <typename T>
class sFStateVirtualID_c : public sFStateID_c<T> {
public:
    typedef void (T::*stateFunc)();
    sFStateVirtualID_c(
        const sStateID_c *superState, const char *name, stateFunc initialize, stateFunc execute, stateFunc finalize
    )
        : sFStateID_c<T>(name, initialize, execute, finalize), mpSuperState(superState) {}

    virtual unsigned int number() const {
        return superID()->numberBase();
    }

    const sFStateVirtualID_c<T> *superID() const {
        if (!mpSuperState->isNull()) {
            return static_cast<const sFStateVirtualID_c<T> *>(mpSuperState)->superID();
        }
        return this;
    }

    unsigned int numberBase() const {
        return sStateID_c::number();
    }

private:
    const sStateID_c *mpSuperState;
};

#endif
