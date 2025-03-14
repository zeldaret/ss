
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "common.h"
#include "sized_string.h"
#include "toBeSorted/event.h"

#include <cstring>

class dAcBase_c;

class EventManager {
public:
    static bool isInEvent(dAcBase_c *actor, const char *eventName);
    static bool finishEvent(dAcBase_c *actor, const char *eventName);
    static void changeOwnEvent(dAcBase_c *actor1, dAcBase_c *actor2, Event *event, UNKWORD);
    static bool alsoSetAsCurrentEvent(dAcBase_c *actor, Event *event, void *unknown);

    static EventManager *sInstance;

    static bool isInEvent() {
        return sInstance != nullptr && sInstance->mState != 0;
    }

    static bool isInEventOtherThan7() {
        return isInEvent() && sInstance->mState != 0 && sInstance->mState != 7;
    }

    static const char *getCurrentEventName() {
        return isInEvent() ? sInstance->mCurrentEvent.eventName : nullptr;
    }

    static bool isCurrentEvent(const char *name) {
        return strequals(getCurrentEventName(), name);
    }

    static bool Get_FUN_800a0570(dAcBase_c *actor) {
        return FUN_800a0570(actor);
    }

    static bool Get_FUN_800a0ba0() {
        return FUN_800a0ba0();
    }

private:
    // Something like isActorInEvent maybe?
    static bool FUN_800a0570(dAcBase_c *actor) {};
    static bool FUN_800a0ba0() {};

    /* 0x000 */ u8 _000[0x084 - 0x000];
    /* 0x084 */ Event mCurrentEvent;
    /* 0x0C4 */ u8 _0C4[0x184 - 0x0C4];
    /* 0x184 */ int mState;
    /* 0x18C */ u8 _18C[0x19C - 0x18C];
};

#endif
