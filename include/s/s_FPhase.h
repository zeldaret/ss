#ifndef S_FPHASE_H
#define S_FPHASE_H

#include "common.h"

// Not present in NSMBW

// Look these BaseBase names are terrible but I can't find any better states

class sFPhaseBaseBase {
public:
    ~sFPhaseBaseBase();
};

class sFPhaseBase : public sFPhaseBaseBase {
public:
    sFPhaseBase() : mState(PHASE_INITIAL) {}

    enum sFPhaseState {
        PHASE_INITIAL,
        PHASE_RETRY,
        PHASE_NEXT,
        PHASE_ALL_DONE,
        STATE_4, // PHASE_CANCEL? This causes the system to stop running any phase callbacks
    };
    virtual ~sFPhaseBase();

    sFPhaseState getState() const {
        return mState;
    }

    bool isFinalized() const {
        return mState == STATE_4 || mState == PHASE_ALL_DONE;
    }

    virtual void reset();
    virtual sFPhaseState step();
    virtual sFPhaseState callPhase() = 0;
    virtual void onReset() = 0;
    virtual void nextPhase() = 0;
    virtual bool hasReachedEnd() const = 0;

private:
    /* 0x04 */ sFPhaseState mState;
};

template <class T>
class sFPhase : public sFPhaseBase {
public:
    typedef sFPhaseState (T::*phaseCallback)(void);
    sFPhase(T *owner, phaseCallback *phases) {
        mpOwner = owner;
        mpCurrentPhase = nullptr;
        mpPhases = phases;
        reset();
    }
    virtual ~sFPhase() {}

    virtual sFPhaseState callPhase() override {
        return (mpOwner->*(*mpCurrentPhase))();
    }
    virtual void onReset() override {
        mpCurrentPhase = mpPhases;
    }
    virtual void nextPhase() override {
        mpCurrentPhase++;
    }
    virtual bool hasReachedEnd() const override {
        return *mpCurrentPhase == nullptr;
    }

private:
    /* 0x08 */ T *mpOwner;
    /* 0x0C */ phaseCallback *mpPhases;
    /* 0x10 */ phaseCallback *mpCurrentPhase;
};

#endif
