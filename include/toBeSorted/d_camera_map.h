#ifndef TO_BE_SORTED_D_CAMERA_MAP_H
#define TO_BE_SORTED_D_CAMERA_MAP_H

#include "toBeSorted/d_camera_base.h"

class dAcObjBase_c;

class dCameraMap_c : public dCameraBase_c {
public:
    dCameraMap_c() {}
    /* vt 0x08 */ virtual void onBecomeActive() override;
    /* vt 0x0C */ virtual void onBecomeInactive() override;
    /* vt 0x10 */ virtual bool create() override;
    /* vt 0x14 */ virtual bool remove() override;
    /* vt 0x18 */ virtual bool execute() override;
    /* vt 0x1C */ virtual bool draw() override;
    /* vt 0x20 */ virtual ~dCameraMap_c() {}
    /* vt 0x30 */ virtual void setView(const CamView &view) override;
    // Not sure what this is...
    /* vt 0x34 */ virtual bool vt_0x34() {
        return true;
    }

    void fn_80093340();
    void fn_80093360();
    void fn_80093380();
    bool fn_800932E0() const;
    void fn_800932F0();

private:
    /* 0x2C */ u8 _0x2C[0x40 - 0x2C];
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ dAcObjBase_c *field_0x44;
    /* 0x48 */ CamView mView1;
    /* 0x68 */ CamView mSourceView;
    /* 0x88 */ CamView mTargetView;
    /* 0xA8 */ u8 _0xA8[0xAC - 0xA8];
    /* 0xAC */ u8 field_0xAC;
    /* 0xAD */ u8 _0xAD[0xB4 - 0xAD];
    /* 0xB4 */ u8 field_0xB4;
    /* 0xB5 */ u8 _0xB5[0xB8 - 0xB5];
    /* 0xB8 */ UNKWORD field_0xB8;
};

#endif
