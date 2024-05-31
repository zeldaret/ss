#pragma once
#include <s/s_FStateID.hpp>
#include <s/s_StateMgr.hpp>
#include <s/s_FState.hpp>
#include <s/s_FStateFct.hpp>
#include <s/s_StateIDChk.hpp>

/// @brief A wrapper for sStateMgr_c that uses sFStateFct_c and sStateIDChk_c.
/// @tparam T The class that this state belongs to.
/// @tparam Method The method to use for the state manager.
/// @ingroup state
template <class T, class Method>
class sFStateMgr_c : public sStateMgr_c<T, Method, sFStateFct_c, sStateIDChk_c> {
public:
    sFStateMgr_c(T &owner, const sStateIDIf_c &initializeState) :
        sStateMgr_c<T, Method, sFStateFct_c, sStateIDChk_c>(owner, initializeState) {}
};
