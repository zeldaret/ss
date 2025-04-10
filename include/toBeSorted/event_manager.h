
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "f/f_base.h"
#include "sized_string.h"
#include "toBeSorted/event.h"

class dAcBase_c;

class EventManager {
public:
    static bool isInEvent(dAcBase_c *actor, const char *eventName);
    static bool finishEvent(dAcBase_c *actor, const char *eventName);
    static void changeOwnEvent(dAcBase_c *actor1, dAcBase_c *actor2, Event *event, UNKWORD);
    static bool alsoSetAsCurrentEvent(dAcBase_c *actor, Event *event, void *unknown);
    static dAcObjBase_c *fn_800A08F0(fBase_c::GROUP_TYPE_e);

    static EventManager *sInstance;

    static bool isInEvent() {
        return sInstance != nullptr && sInstance->mState != 0;
    }

    static bool isInEvent0Or7() {
        return sInstance != nullptr && (sInstance->mState == 0 || sInstance->mState == 7);
    }

    static bool isInEventOtherThan0Or7() {
        return isInEvent() && !isInEvent0Or7();
    }

    static const char *getCurrentEventName() {
        return isInEvent() ? sInstance->mCurrentEvent.eventName : nullptr;
    }

    static bool isCurrentEvent(const char *name) {
        return strequals(getCurrentEventName(), name);
    }

    // Something like isActorInEvent maybe?
    static bool FUN_800a0570(dAcBase_c *actor);
    static bool FUN_800a0ba0();

private:

    /* 0x000 */ u8 _000[0x084 - 0x000];
    /* 0x084 */ Event mCurrentEvent;
    /* 0x0C4 */ u8 _0C4[0x184 - 0x0C4];
    /* 0x184 */ int mState;
    /* 0x18C */ u8 _18C[0x19C - 0x18C];
};

#endif
