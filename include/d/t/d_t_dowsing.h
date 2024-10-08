#ifndef D_T_DOWSING_H
#define D_T_DOWSING_H

#include <d/t/d_tg.h>
#include <toBeSorted/dowsing_target.h>

class dTgDowsing_c : public dTg_c {
public:
    dTgDowsing_c();
    virtual ~dTgDowsing_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    bool checkDowsing() const;

private:
    enum DowsingOption {
        TRIAL_0,
        TRIAL_1,
        TRIAL_2,
        TRIAL_3,
        PROPELLER,
        WATER_BASIN,
        CRYSTAL_BALL,
        PUMPKIN,
        NEW_PLANT_SPECIES,
        PARTY_WHEEL,
        ZELDA,
        DOWSING_MAX
    };

    DowsingTarget mDowsingTarget; // 0x0FC
    DowsingOption mDowsingOption; // 0x11C
    u16 mUntriggerStoryFlag;      // 0x120
    u16 mTriggerStoryFlag;        // 0x122
    bool mTargetActive;           // 0x124

    u16 getTriggerSceneFlag() {
        return params & 0xFF;
    }

    u16 getUntriggerSceneFlag() {
        return params >> 0x8 & 0xFF;
    }

    DowsingOption getDowsingOption() {
        return (DowsingOption)(params >> 0x10 & 0xF);
    }

    u32 getDowsingOptionNum() {
        return mDowsingOption;
    }

    u16 getUntriggerStoryFlag() {
        return params >> 0x14 & 0x7FF;
    }

    u16 getTriggerStoryFlag() {
        return rotation.z.mVal & 0x7FF;
    }
};

#endif
