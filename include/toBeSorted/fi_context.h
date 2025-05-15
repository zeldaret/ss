#ifndef FI_CONTEXT_H
#define FI_CONTEXT_H

#include "common.h"

struct AnalysisEntry;

class FiAnalysisHandle {
public:
    FiAnalysisHandle(AnalysisEntry *entry) : mpEntry(entry) {}
    s16 getEquipmentFocus() const;
    s16 getAreaIndexForFiAreaName() const;
    s32 getSuitabilityPercentageArg() const;
    s16 getEquipmentRecommendation() const;
    s16 getShieldMessage() const;
    s16 getThreatenedShield() const;

    bool isValid() const;

private:
    enum FiAnalysisEquipmentFocus_e {
        FOCUS_COMBAT,
        FOCUS_SURVIVAL,
        FOCUS_TREASURE,
        FOCUS_BALANCED,
    };

    static FiAnalysisEquipmentFocus_e getCurrentFocus();

    AnalysisEntry *mpEntry;
};

class FiContext {
public:
    enum KEN8_Button_e {
        KEN8_Summary = 0,
        KEN8_Hint = 1,
        KEN8_Objective = 2,
        KEN8_Analysis = 3,
        KEN8_PlayTime = 4,
        KEN8_Rumors = 5,
        KEN8_Other = 6,
        KEN8_Nevermind = 7,
        KEN8_Advice = 9,
        KEN8_None = 10,
    };

    static void create();
    static void initialize(void *data);
    static s32 getGlobalFiInfo0(s32);
    static u8 rateBattlePerformance(u8 enemyActorId);

    /** KEN0 - player progress */
    static s16 getNaviTableProgressSummary();
    /** KEN1 - hint */
    static s16 getFiAdviceHintEntry();
    /** KEN2 - objective */
    static s16 getObjective();
    /* KEN3 - analysis*/
    static FiAnalysisHandle getNaviTableEquipmentCheckEntry();

    static s32 setTargetedActorTextId(s32 id);

    static u16 prepareFiHelpIndex();

    bool getIsInFiMainMenu() const {
        return mIsInFiMainMenu;
    }

    static bool getIsInFiMainMenuChecked() {
        if (sInstance != nullptr) {
            return sInstance->getIsInFiMainMenu();
        } else {
            return false;
        }
    }

    static void setIsInFiMainMenu(bool val) {
        if (sInstance != nullptr) {
            sInstance->mIsInFiMainMenu = val;
        }
    }

    static const wchar_t *getMessageForFiInfo(s32 arg) {
        return getButtonText(getGlobalFiInfo0(arg));
    }

    static FiContext *GetInstance() {
        return sInstance;
    }

    static bool getField_0x47() {
        if (sInstance != nullptr) {
            return sInstance->field_0x47;
        } else {
            return false;
        }
    }

    static void setField_0x48(bool val) {
        if (sInstance != nullptr) {
            sInstance->field_0x48 = val;
        }
    }

    static bool getField_0x48() {
        if (sInstance != nullptr) {
            return sInstance->field_0x48;
        } else {
            return false;
        }
    }

    static void setField_0x4A(bool val) {
        if (sInstance != nullptr) {
            sInstance->field_0x4A = val;
        }
    }

    static bool getField_0x4A() {
        if (sInstance != nullptr) {
            return sInstance->field_0x4A;
        } else {
            return false;
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

    static void do_setAdviceOptions(s32 arg) {
        if (sInstance != nullptr) {
            sInstance->setAdviceOptions(arg);
        }
    }

    static void do_fn_8016CB40() {
        if (sInstance != nullptr) {
            sInstance->fn_8016CB40();
        }
    }

    static void do_prepareFiCallOptions() {
        if (sInstance != nullptr) {
            sInstance->prepareFiCallOptions();
        }
    }

    static void do_resetAdviceOptions() {
        if (sInstance != nullptr) {
            sInstance->resetAdviceOptions();
        }
    }

    void reset();
    void resetSaveTimeRelated();
    void setAdviceOptions(s32 unused);
    void resetAdviceOptions();

    static const wchar_t *getButtonText(s32 idx);

private:
    static void setTargetActorId(s32 id) {
        if (sInstance != nullptr) {
            sInstance->mTargetActorId = id;
        }
    }

    static void setHelpIndex(s32 idx) {
        if (sInstance != nullptr) {
            sInstance->mFiHelpIndex = idx;
        }
    }

    static FiContext *sInstance;

    void fn_8016CB40();
    void prepareFiCallOptions();
    bool isInLeviasFightMaybe();

    /* 0x00 */ s32 mFiButtonOptions[10]; // correspond to KEN8 with an offset of +1
    /* 0x28 */ s32 field_0x28;
    /* 0x2C */ s32 field_0x2C;
    /* 0x30 */ s32 field_0x30;
    /* 0x34 */ s32 mTargetActorId;
    /* 0x38 */ s32 mFiHelpIndex;
    /* 0x3C */ u8 mSaveTimeRelated;
    /* 0x3D */ u8 field_0x3D[10];
    /* 0x47 */ bool field_0x47;
    /* 0x48 */ bool field_0x48;
    /* 0x49 */ bool field_0x49;
    /* 0x4A */ bool field_0x4A;
    /* 0x4B */ bool mIsInFiMainMenu;
};

#endif
