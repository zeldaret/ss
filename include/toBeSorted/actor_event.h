#ifndef ACTOR_EVENT_H
#define ACTOR_EVENT_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "toBeSorted/event.h"

class ActorEventRelated {
public:
    ActorEventRelated(dAcBase_c &owner, void *);
    ~ActorEventRelated();

    bool isThisActorInEvent();
    void scheduleEvent(Event &, u32);
    u32 getCurrentEventCommand() const;
    void advanceNext();

private:
    u8 field_0x00[0x50 - 0x00];
};

#endif
