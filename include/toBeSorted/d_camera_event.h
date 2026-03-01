#ifndef TO_BE_SORTED_D_CAMERA_EVENT_H
#define TO_BE_SORTED_D_CAMERA_EVENT_H

#include "toBeSorted/actor_event.h"
#include "toBeSorted/d_camera_base.h"

class dCameraEvent_c : public dCameraBase_c {
public:
    dCameraEvent_c();
    /* vt 0x08 */ virtual void onBecomeActive() override;
    /* vt 0x0C */ virtual void onBecomeInactive() override;
    /* vt 0x10 */ virtual bool create() override;
    /* vt 0x14 */ virtual bool remove() override;
    /* vt 0x18 */ virtual bool execute() override;
    /* vt 0x1C */ virtual bool draw() override;
    /* vt 0x20 */ virtual ~dCameraEvent_c() {}

    void fn_800918E0(s32, s16);

private:
    /* 0x2C */ u8 field_0x2C[0x48 - 0x2C];
    /* 0x48 */ u8 field_0x48;
    /* 0x4A */ s16 field_0x4A;
    /* 0x4C */ u8 field_0x4C[0x50 - 0x4C];
    /* 0x50 */ ActorEventRelated mEventRelated;
    /* 0xA0 */ u8 field_0xA0;
    /* 0xA4 */ s32 field_0xA4;
    /* 0xA8 */ u8 field_0xA8[0xAC - 0xA8];
    /* 0xAC */ s32 field_0xAC;
    /* 0xB0 */ u8 field_0xB0[0xB4 - 0xB0];
    /* 0xB4 */ CamView mViews[4];
};

#endif
