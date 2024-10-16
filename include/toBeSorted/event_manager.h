
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "toBeSorted/event.h"

class dAcBase_c;

class EventManager {
public:
    static bool isInEvent(dAcBase_c *actor, const char *eventName);
    static bool alsoSetAsCurrentEvent(dAcBase_c *actor, Event *event, void *unknown);

    static EventManager *sInstance;

    static bool isInEvent() {
        return sInstance != nullptr && sInstance->mState != 0;
    }

    static bool isInEventOtherThan7() {
        return isInEvent() && sInstance != nullptr && sInstance->mState != 7;
    }

private:
    u8 field_0x000[0x184 - 0x000];
    int mState;
};

#endif
