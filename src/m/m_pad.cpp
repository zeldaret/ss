#include <common.h>
#include <egg/core/eggController.h>
#include <egg/math/eggVector.h>
#include <m/m_pad.h>

namespace mPad {

static EGG::CoreControllerMgr *g_padMg;
static int g_currentCoreId;
static EGG::CoreController *g_currentCore;
static bool g_IsConnected[4];
static u32 g_PadFrame;

static u32 s_GetWPADInfoInterval = 0;
static bool s_WPADInfoAvailable[4];
static u32 s_GetWPADInfoCount = 0;

static EGG::CoreController *g_core[4];

struct WPADInfo_t {
    u32 field_0x00;
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
    u32 field_0x10;
    bool field_0x14;
    bool field_0x15;
    bool field_0x16;
    bool field_0x17;

    inline void clear() {
        field_0x00 = 0;
        field_0x04 = 0;
        field_0x08 = 0;
        field_0x0C = 0;
        field_0x10 = 0;
        field_0x14 = false;
        field_0x15 = false;
        field_0x16 = false;
        field_0x17 = false;
    }
};

struct PadAdditionalData_t {
    PadAdditionalData_t() {}
    ~PadAdditionalData_t() {}

    EGG::Vector2f v1;
    EGG::Vector2f v2;
    EGG::Vector2f v3;
};

static PadAdditionalData_t g_PadAdditionalData[4];
static WPADInfo_t s_WPADInfo[4];
static WPADInfo_t s_WPADInfoTmp[4];

static void initWPADInfo();
static void clearWPADInfo(int);
static int getWPADInfoCB(int controller);

void create() {
    g_padMg = EGG::CoreControllerMgr::sInstance;
    initWPADInfo();
    beginPad();
    endPad();
}

// Has a regshuffle
void beginPad() {
    EGG::CoreController *ctl;

    g_PadFrame++;
    g_padMg->beginFrame();

    for (int i = 0; i < 4; i++) {
        ctl = g_padMg->getNthController(i);
        g_core[i] = ctl;
        if (ctl->mFlag.onBit(0)) {
            // These sort of look like value, first order derivative, and second order derivative
            // So perhaps value, velocity, acceleration?
            EGG::Vector2f pos = ctl->coreStatus[0].getUnk();
            EGG::Vector2f v = pos - g_PadAdditionalData[i].v1;
            g_PadAdditionalData[i].v1 = pos;
            g_PadAdditionalData[i].v3 = v - g_PadAdditionalData[i].v2;
            g_PadAdditionalData[i].v2 = v;

            if (!g_IsConnected[i]) {
                g_IsConnected[i] = true;
            }

            // Not sure why this checks the controller index against the tick count
            if (s_GetWPADInfoInterval != 0 &&
                    ((s_GetWPADInfoInterval == 1 || s_GetWPADInfoCount == i) ||
                            (s_GetWPADInfoInterval <= 3 && (s_GetWPADInfoCount & 1) == (i & 1)))) {
                getWPADInfoCB(i);
            }
        } else if (g_IsConnected[i]) {
            ctl->coreStatus->init();
            ctl->sceneReset();
            g_PadAdditionalData[i].v1.x = 0.0f;
            g_PadAdditionalData[i].v1.y = 0.0f;
            g_PadAdditionalData[i].v3.x = 0.0f;
            g_PadAdditionalData[i].v3.y = 0.0f;
            g_PadAdditionalData[i].v2.x = 0.0f;
            g_PadAdditionalData[i].v2.y = 0.0f;
            clearWPADInfo(i);
            g_IsConnected[i] = false;
        }
    }

    if (s_GetWPADInfoInterval != 0) {
        s_GetWPADInfoCount++;
        if (s_GetWPADInfoCount > s_GetWPADInfoInterval) {
            s_GetWPADInfoCount = 0;
        }
    }
    g_currentCore = g_core[g_currentCoreId];
}

void endPad() {
    g_padMg->endFrame();
}

static void clearWPADInfo(int controller) {
    s_WPADInfoAvailable[controller] = false;
    s_WPADInfo[controller].clear();
}

static void initWPADInfo() {
    for (int i = 0; i < 4; i++) {
        clearWPADInfo(i);
    }
}

extern "C" int fn_803DAFC0(int controller, void *data, void (*callback)(int, int));
extern "C" void fn_80058DA0(int, int);

static int getWPADInfoCB(int controller) {
    int result = fn_803DAFC0(controller, &s_WPADInfoTmp[controller], fn_80058DA0);
    if (result == -1) {
        clearWPADInfo(controller);
    }
    return result;
}

} // namespace mPad
