
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
    void ForbidCalc();

    s32 fn_80197560(s32);
    void offFlags(u32 flags) {
        mFlags &= ~flags;
    }

private:
    static Manage_c *s_pInstance;
    u8 _0x00[0x218 - 0x000];
    u32 mFlags;
};
} // namespace dHbm
