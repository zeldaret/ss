#ifndef D_BASE_H
#define D_BASE_H

#include "f/f_base.h"
// #include "c/c_owner_set.h"

// Ghidra: dBase_c
//   size: 0x68
// non-official but likely name
class dBase_c : public fBase_c {
public:
    enum BaseProperties_e {
        BASE_PROP_0x1 = (1 << 0),
        BASE_PROP_0x2 = (1 << 1),
        BASE_PROP_0x4 = (1 << 2),
        BASE_PROP_0x8 = (1 << 3),
        BASE_PROP_0x10 = (1 << 4),
        BASE_PROP_0x20 = (1 << 5),
        BASE_PROP_0x40 = (1 << 6),
        BASE_PROP_0x80 = (1 << 7),
        BASE_PROP_0x100 = (1 << 8),
        BASE_PROP_0x200 = (1 << 9),
        BASE_PROP_0x400 = (1 << 10),

        BASE_PROP_UNK_PARTICLE_1 = BASE_PROP_0x400 | BASE_PROP_0x200 | BASE_PROP_0x80 | BASE_PROP_0x40 |
                                   BASE_PROP_0x20 | BASE_PROP_0x10 | BASE_PROP_0x8 | BASE_PROP_0x1,
        BASE_PROP_UNK_PARTICLE_2 = BASE_PROP_0x400 | BASE_PROP_0x200 | BASE_PROP_0x80 | BASE_PROP_0x40 |
                                   BASE_PROP_0x20 | BASE_PROP_0x10 | BASE_PROP_0x8 | BASE_PROP_0x2 | BASE_PROP_0x1,
    };

    // field from profile init
    /* 0x64 */ u32 mBaseProperties;

public:
    dBase_c();
    virtual int preExecute();
    virtual void postExecute(MAIN_STATE_e state);
    virtual int preDraw();
    virtual void postDraw(MAIN_STATE_e state);
    virtual ~dBase_c() {}

public:
    static void resetFlags();
    bool isActorPlayer();
    static void initLoader();
    static dBase_c *createBase(ProfileName, dBase_c *, u32, u8);
    static dBase_c *createRoot(ProfileName, u32, u8);

    bool checkBaseProperty(u32 property) const {
        return (mBaseProperties & property) != 0;
    }
    void setBaseProperty(u32 property) {
        mBaseProperties |= property;
    }
    void unsetBaseProperty(u32 property) {
        mBaseProperties &= ~property;
    }

private:
    static int loadAsyncCallback();
    static void unloadCallback();

public:
    static u32 s_ExecuteControlFlags;
    static u32 s_DrawControlFlags;
    static u32 s_NextExecuteControlFlags;
    // static fProfile::fBaseProfile_c** DAT_ACTOR_ALLOCATION_FUNCTIONS;

    friend class fBase_c;
};

#endif
