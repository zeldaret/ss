#include "s/s_FPhase.h"

// Not present in NSMBW

sFPhaseBaseBase::~sFPhaseBaseBase() {}
sFPhaseBase::~sFPhaseBase() {}

sFPhaseBase::sFPhaseState sFPhaseBase::step() {
    if (!isFinalized()) {
        mState = callPhase();
        if (mState == PHASE_NEXT) {
            nextPhase();
            if (hasReachedEnd()) {
                mState = PHASE_ALL_DONE;
            }
        }
    }

    return mState;
}

void sFPhaseBase::reset() {
    onReset();
    mState = PHASE_INITIAL;
}
