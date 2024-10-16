#include "s/s_StateMethod.hpp"

#include "common.h"
#include "s/s_StateID.hpp"
#include "s/s_StateInterfaces.hpp"


// Note: Ported from https://github.com/NSMBW-Community/NSMBW-Decomp/tree/master/include/dol/sLib
// See include/s/README.txt for changes made

sStateMethod_c::sStateMethod_c(sStateIDChkIf_c &checker, sStateFctIf_c &factory, const sStateIDIf_c &initialState)
    : mpStateChk(checker), mpStateFct(factory), mInitFinalizeLock(false), mExecutionLock(false), mIsValid(false),
      mStateChanged(false), mRefreshStateMethod(false), mpNewStateID(&initialState), mpOldStateID(&sStateID::null),
      mpStateID(&initialState), mpState(nullptr) {}

sStateMethod_c::~sStateMethod_c() {}

void sStateMethod_c::initializeStateMethod() {
    if (!mpNewStateID->isNull() && !mInitFinalizeLock && !mIsValid) {
        mInitFinalizeLock = true;

        mpStateID = mpNewStateID;
        int ret = initializeStateLocalMethod();
        if (ret != 0) {
            mIsValid = true;
        } else {
            mIsValid = false;
        }

        mInitFinalizeLock = false;
    }
}

void sStateMethod_c::executeStateMethod() {
    if (!mExecutionLock) {
        // Skyward Sword change: Prevent runaway state changes?
        int i = 2;
        do {
            if (mRefreshStateMethod) {
                i--;
            }
            mRefreshStateMethod = false;
            // We only want to execute if we have a valid next state
            if (!mpNewStateID->isNull()) {
                mExecutionLock = true;
                executeStateLocalMethod();
                mExecutionLock = false;
            }
        } while (mRefreshStateMethod && mStateChanged && i);
    }
}

void sStateMethod_c::finalizeStateMethod() {
    if (!mpNewStateID->isNull() && mIsValid && !mInitFinalizeLock) {
        mInitFinalizeLock = true;

        mpOldStateID = mpStateID;
        finalizeStateLocalMethod();
        mIsValid = false;

        mInitFinalizeLock = false;
    }
}

void sStateMethod_c::changeStateMethod(const sStateIDIf_c &newID) {
    if (!newID.isNull()) {
        mpNewStateID = &newID;
        changeStateLocalMethod(newID);
        mStateChanged = true;
    }
}
