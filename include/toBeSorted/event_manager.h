#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <toBeSorted/event.h>

class dAcBase_c;

class EventManager {
public:
    static bool isInEvent(dAcBase_c *actor, const char *eventName);
    static bool alsoSetAsCurrentEvent(dAcBase_c *actor, Event *event, void *unknown);

    static EventManager *sInstance;
};

#endif
