
#include "common.h"
#include "egg/core/eggHeap.h"
namespace dHbm {
class Manage_c {
public:
    enum ManageState_e {
        HBM_MANAGE_NOT_LOADED = 0,
        HBM_MANAGE_INACTIVE = 1,
        HBM_MANAGE_ACTIVE = 2,
    };

    typedef void (*MenuInitCallback)();

    static void CreateInstance();
    static Manage_c *GetInstance();

    Manage_c(EGG::Heap *);

    bool Load();
    void Calculate();
    void DrawMenu(s32);
    void DrawMenuSetting();
    void DrawIcon();
    void InitDataInfo();
    void InitControllerData();
    void SetRegionLayoutName();
    void SetSpkNameName();
    void SetMsgName();
    void SetConfigName();
    void SetIconName();
    void SetSoundName();
    void SetContData();
    bool CheckBegin() const;

    void MenuInit();
    void IconCalc();
    void IconClear();
    void StopMotor();
    bool fn_801973C0(s32);
    void ForbidCalc();

    s32 fn_80197560(s32);
    void offFlags(u32 flags) {
        mFlags &= ~flags;
    }

    ManageState_e getState() const {
        return mState;
    }

    void setMenuInitCallback(MenuInitCallback cb) {
        mMenuInitCallback = cb;
    }

private:
    static Manage_c *s_pInstance;

    /* 0x000 */ u8 _0x00[0x210 - 0x000];
    /* 0x210 */ ManageState_e mState;
    /* 0x218 */ UNKWORD field_0x214;
    /* 0x218 */ u32 mFlags;
    /* 0x21C */ u8 _0x21C[0x224 - 0x21C];
    /* 0x224 */ MenuInitCallback mMenuInitCallback;
};
} // namespace dHbm
