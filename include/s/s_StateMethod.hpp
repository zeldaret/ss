#ifndef S_STATEMETHOD_H
#define S_STATEMETHOD_H

#include <s/s_StateInterfaces.hpp>

// Note: Ported from https://github.com/NSMBW-Community/NSMBW-Decomp/tree/master/include/dol/sLib
// See include/s/README.txt for changes made

/// @brief A class that handles state execution and transition.
/// @details [Presumably, sStateMethod_c actually means "methods for state interaction", or something like that].
/// @ingroup state
class sStateMethod_c : public sStateMethodIf_c {
public:
    /**
     * @brief Constructs a new sStateMethod_c instance.
     *
     * @param checker The state checker to use.
     * @param factory The state factory to use.
     * @param initialState The initial state ID of this instance.
     */
    sStateMethod_c(sStateIDChkIf_c &checker, sStateFctIf_c &factory, const sStateIDIf_c &initialState);
    virtual ~sStateMethod_c(); ///< Destroys the sStateMethod_c instance.

    virtual void initializeStateMethod(); ///< @copydoc sStateMgrIf_c::initializeState
    virtual void executeStateMethod(); ///< @copydoc sStateMgrIf_c::executeState
    virtual void finalizeStateMethod(); ///< @copydoc sStateMgrIf_c::finalizeState
    virtual void changeStateMethod(const sStateIDIf_c &newStateID);  ///< @copydoc sStateMgrIf_c::changeState
    virtual void refreshStateMethod() { mRefreshStateMethod = true; } ///< @copydoc sStateMgrIf_c::refreshState
    virtual sStateIf_c *getState() const { return mpState; } ///< @copydoc sStateMgrIf_c::getState
    virtual const sStateIDIf_c *getNewStateID() const { return mpNewStateID; } ///< @copydoc sStateMgrIf_c::getNewStateID
    virtual const sStateIDIf_c *getStateID() const { return mpStateID; } ///< @copydoc sStateMgrIf_c::getStateID
    virtual const sStateIDIf_c *getOldStateID() const { return mpOldStateID; } ///< @copydoc sStateMgrIf_c::getOldStateID

    virtual int initializeStateLocalMethod() = 0; ///<  Performs the actual state initialization.
    virtual void executeStateLocalMethod() = 0; ///<  Performs the actual state execution.
    virtual void finalizeStateLocalMethod() = 0; ///<  Performs the actual state termination.
    virtual void changeStateLocalMethod(const sStateIDIf_c &newStateID) = 0; ///<  Performs the actual state transition.

protected:
    sStateIDChkIf_c &mpStateChk; ///< @unused The state checker to use.
    sStateFctIf_c &mpStateFct; ///< The state factory which produces the state holder.

    bool mInitFinalizeLock; ///< A lock to ensure ::initializeStateMethod and ::finalizeStateMethod are not called recursively.
    bool mExecutionLock; ///< A lock to ensure ::executeStateMethod is not called recursively.
    bool mIsValid; ///< If the state holder contains a valid state ID.
    bool mStateChanged; ///< If the current state has changed during execution.
    bool mRefreshStateMethod; ///< True, if after a state transition, the state should be executed again.

    const sStateIDIf_c *mpNewStateID; ///< The next state ID.
    const sStateIDIf_c *mpOldStateID; ///< The previous state ID.
    const sStateIDIf_c *mpStateID; ///< The current state ID.

    sStateIf_c *mpState; ///< The current state holder.
};

#endif
