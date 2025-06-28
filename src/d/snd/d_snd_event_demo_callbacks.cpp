#include "common.h"
#include "d/snd/d_snd_event.h"
#include "d/snd/d_snd_state_mgr.h"

void todoInititalizeSndEvent();
void todoExecuteSndEvent();
void todoFinalizeSndEvent();

// clang-format off

static const SndEventDef sSndEventDefs_DEMO[] = {
    {"Demo03_03", SND_EVENT_Demo03_03, todoInititalizeSndEvent, todoExecuteSndEvent, nullptr},
    {"Demo08_01", SND_EVENT_Demo08_01, todoInititalizeSndEvent, todoExecuteSndEvent, nullptr},
    {"Demo15_04", SND_EVENT_Demo15_04, todoInititalizeSndEvent, todoExecuteSndEvent, nullptr},
    {"Demo36_01", SND_EVENT_Demo36_01,                 nullptr, todoExecuteSndEvent, nullptr},
    {"Demo41_01", SND_EVENT_Demo41_01, todoInititalizeSndEvent, todoExecuteSndEvent, nullptr},
    {"Demo59_01", SND_EVENT_Demo59_01,                 nullptr, todoExecuteSndEvent, nullptr},
    {"Demo62_01", SND_EVENT_Demo62_01, todoInititalizeSndEvent, todoExecuteSndEvent, nullptr},
    {"Demo64_06", SND_EVENT_Demo64_06, todoInititalizeSndEvent,             nullptr, nullptr},
    {"Demo70_01", SND_EVENT_Demo70_01, todoInititalizeSndEvent, todoExecuteSndEvent, nullptr},
    {"Demo70_02", SND_EVENT_Demo70_02, todoInititalizeSndEvent, todoExecuteSndEvent, nullptr},
    {"Demo72_01", SND_EVENT_Demo72_01, todoInititalizeSndEvent, todoExecuteSndEvent, nullptr},
    {"Demo77_01", SND_EVENT_Demo77_01, todoInititalizeSndEvent, todoExecuteSndEvent, nullptr},
    {"Demo79_01", SND_EVENT_Demo79_01, todoInititalizeSndEvent, todoExecuteSndEvent, nullptr},
};

// clang-format on

void dSndStateMgr_c::handleDemoEvent(const char *name) {
    for (const SndEventDef *def = sSndEventDefs_DEMO; def < sSndEventDefs_DEMO + ARRAY_LENGTH(sSndEventDefs_DEMO);
         def++) {
        if (streq(name, def->eventName)) {
            field_0x08C = def->eventId;
            field_0x234 = def;
            sEventExecuteCallback = def->executeCb;
            if (def->initializeCb != nullptr) {
                (def->initializeCb)();
            }
            break;
        }
    }

    if (field_0x08C == SND_EVENT_0x98) {
        field_0x08C = SND_EVENT_0x78;
    }

    return;
}
