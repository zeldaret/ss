#ifndef FI_CONTEXT_H
#define FI_CONTEXT_H

#include "common.h"

struct FiAnalysisEntry {
    /* 0x00 */ const char *mStageName;
    /* 0x04 */ u16 mStoryFlag;
    /* 0x06 */ s16 mKen3MonsterPresenceEntry;
    /* 0x08 */ u8 mSuitabilityPercent[4];
    /* 0x0C */ u16 mKen3SuitabilityAnalysis[4];
    /* 0x14 */ s16 mShieldType;
    /* 0x16 */ s16 mShieldMessage;
};

class FiAnalysisHandle {
public:
    s16 getEquipmentFocus() const;
    s16 getAreaIndexForFiAreaName() const;
    s32 getSuitabilityArg() const;
    s16 getSuitabilityLabel() const;
    s16 getShieldMessage();
    s16 shieldRelated();

    bool isValid() const;

private:
    FiAnalysisEntry *mpEntry;
};

class FiContext {
public:
    static void create();
    static s32 getGlobalFiInfo0(s32);
    static FiAnalysisHandle getNaviTableEquipmentCheckEntry();
    static u8 rateBattlePerformance(u8 enemyActorId);
    static s16 getNaviTableProgressSummary();
    static s16 getFiAdviceHintEntry();
    static s16 getUnkObjectiveValue();

    bool getDoSpecialFiMenuHandling() const {
        return mDoSpecialFiMenuHandling;
    }

    static bool getDoSpecialFiMenuHandlingChecked() {
        if (sInstance != nullptr) {
            return sInstance->getDoSpecialFiMenuHandling();
        } else {
            return false;
        }
    }

    static void setDoSpecialFiMenuHandling(bool val) {
        if (sInstance != nullptr) {
            sInstance->mDoSpecialFiMenuHandling = val;
        }
    }

    static const wchar_t *getMessageForFiInfo(s32 arg) {
        return getTextMessage(getGlobalFiInfo0(arg));
    }

    static FiContext *GetInstance() {
        return sInstance;
    }

    static void setField_0x48(bool val) {
        if (sInstance != nullptr) {
            sInstance->field_0x48 = val;
        }
    }

    static void setField_0x4A(bool val) {
        if (sInstance != nullptr) {
            sInstance->field_0x4A = val;
        }
    }

    static s32 getHelpIndex() {
        if (sInstance != nullptr) {
            return sInstance->mFiHelpIndex;
        } else {
            return -1;
        }
    }

    static s32 getTargetActorId() {
        if (sInstance != nullptr) {
            return sInstance->mTargetActorId;
        } else {
            return -1;
        }
    }

    static void do_fn_8016CB00(s32 arg) {
        if (sInstance != nullptr) {
            sInstance->fn_8016CB00(arg);
        }
    }

    static void do_fn_8016CB40() {
        if (sInstance != nullptr) {
            sInstance->fn_8016CB40();
        }
    }

    static void do_fn_8016CA00() {
        if (sInstance != nullptr) {
            sInstance->fn_8016CA00();
        }
    }

    static void do_fn_8016CB20() {
        if (sInstance != nullptr) {
            sInstance->fn_8016CB20();
        }
    }

    void resetField_0x3C();

    static const wchar_t *getTextMessage(s32 idx);

private:
    static FiContext *sInstance;

    void fn_8016CB00(s32);
    void fn_8016CB40();
    void fn_8016CA00();
    void fn_8016CB20();

    /* 0x00 */ u8 _0x00[0x34 - 0x00];
    /* 0x34 */ s32 mTargetActorId;
    /* 0x38 */ s32 mFiHelpIndex;
    /* 0x3C */ u8 _0x3C[0x48 - 0x3C];
    /* 0x48 */ bool field_0x48;
    /* 0x49 */ bool field_0x49;
    /* 0x4A */ bool field_0x4A;
    /* 0x4B */ bool mDoSpecialFiMenuHandling;
};

#endif
