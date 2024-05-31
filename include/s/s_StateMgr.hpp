#pragma once
#include <s/s_StateInterfaces.hpp>

/**
 * @brief An implementation of sStateMgrIf_c.
 * 
 * @tparam T The parent class for this state manager.
 * @tparam Method The state method handler to use.
 * @tparam Factory The state factory to use.
 * @tparam Check The state ID checker to use.
 * @ingroup state
 */
template <class T, class Method, template <class> class Factory, class Check>
class sStateMgr_c : sStateMgrIf_c {
public:
    sStateMgr_c(T &owner, const sStateIDIf_c &initialState) :
        mFactory(owner),
        mMethod(mCheck, mFactory, initialState) {}

    virtual void initializeState() { mMethod.initializeStateMethod(); }
    virtual void executeState() { mMethod.executeStateMethod(); }
    virtual void finalizeState() { mMethod.finalizeStateMethod(); }

    virtual void changeState(const sStateIDIf_c &newState) { mMethod.changeStateMethod(newState); }

    virtual void refreshState() { mMethod.refreshStateMethod(); }

    virtual sStateIf_c *getState() const { return mMethod.getState(); }
    virtual const sStateIDIf_c *getNewStateID() const { return mMethod.getNewStateID(); }
    virtual const sStateIDIf_c *getStateID() const { return mMethod.getStateID(); }
    virtual const sStateIDIf_c *getOldStateID() const { return mMethod.getOldStateID(); }

private:
    Check mCheck;
    Factory<T> mFactory;
    Method mMethod;
};
