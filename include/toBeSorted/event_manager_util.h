#ifndef EVENT_MANAGER_UTIL_H
#define EVENT_MANAGER_UTIL_H

#include "d/d_base.h"
#include "d/d_message.h"
#include "d/lyt/d_lyt_deposit.h"
#include "d/lyt/d_lyt_map.h"
#include "d/lyt/d_lyt_shop.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/misc_actor.h"

// Pulling these functions out into a separate file to avoid circular dependencies

inline bool EventManagerNotDrawControl0x80() {
    return EventManager::isInEvent() && (dBase_c::s_DrawControlFlags & 0x80) == 0;
}

inline bool EventManagerNotSkyKeepPuzzle() {
    return EventManager::isInEvent() && !checkIsInSkykeepPuzzle();
}

inline bool EventManagerNotInShop() {
    return EventManager::isInEvent() && dLytShop_c::GetInstance() == nullptr;
}

inline bool EventManagerNotInDeposit() {
    return EventManager::isInEvent() && dLytDeposit_c::GetInstance() == nullptr;
}

inline bool EventManagerIsInMap() {
    return EventManager::isInEvent() && dLytMap_c::GetInstance() != nullptr;
}

inline bool EventManagerIsMapOpen() {
    return EventManagerIsInMap() && dLytMap_c::GetInstance()->isOpenMaybe();
}

inline bool EventManagerIsMapOpenAndMessage() {
    return EventManagerIsMapOpen() && !dMessage_c::getInstance()->getField_0x328();
}

inline bool EventManagerIsMapOpenAnd0x9008Eq10() {
    return EventManagerIsMapOpen() && dLytMap_c::GetInstance()->isSomeMapFieldEq10();
}

#endif
