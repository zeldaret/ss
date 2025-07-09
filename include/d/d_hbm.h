
#include "common.h"
#include "egg/core/eggHeap.h"
namespace dHbm {
class Manage_c {
public:
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

    UNKWORD getField_0x210() const {
        return field_0x210;
    }

private:
    static Manage_c *s_pInstance;

    /* 0x000 */ u8 _0x00[0x210 - 0x000];
    /* 0x210 */ UNKWORD field_0x210;
    /* 0x218 */ UNKWORD field_0x214;
    /* 0x218 */ u32 mFlags;
};
} // namespace dHbm
