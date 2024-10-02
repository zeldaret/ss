#include <common.h>
#include <egg/core/eggController.h>
#include <egg/math/eggVector.h>
#include <m/m_pad.h>
#include <rvl/WPAD.h>

namespace mPad {

EGG::CoreControllerMgr *g_padMg;
int g_currentCoreId;
EGG::CoreController *g_currentCore;
EGG::CoreController *g_core[4];

static bool g_IsConnected[4];
static u32 g_PadFrame;

static bool s_WPADInfoAvailable[4];
static u32 s_GetWPADInfoInterval = 0;
static u32 s_GetWPADInfoCount = 0;

struct PadAdditionalData_t {
    PadAdditionalData_t() {}
    ~PadAdditionalData_t() {}

    EGG::Vector2f v1;
    EGG::Vector2f v2;
    EGG::Vector2f v3;
};

static PadAdditionalData_t g_PadAdditionalData[4];
static WPADInfo s_WPADInfo[4];
static WPADInfo s_WPADInfoTmp[4];

static void initWPADInfo();
static void clearWPADInfo(int);
static int getWPADInfoCB(int controller);

void create() {
    g_padMg = EGG::CoreControllerMgr::sInstance;
    initWPADInfo();
    beginPad();
    endPad();
}

// This code looks really bad.
// It mostly matches when writing it the obvious way
// (just indexing the arrays normally)
// but has an annoying regshuffle
void beginPad() {
    g_PadFrame++;
    g_padMg->beginFrame();

    EGG::CoreController *ctl;
    EGG::CoreController **p_ctl = g_core;
    PadAdditionalData_t *dat = g_PadAdditionalData;
    bool *connected;
    int i = 0;
    connected = g_IsConnected;

    for (; i < 4; i++) {
        ctl = g_padMg->getNthController(i);
        *p_ctl = ctl;
        if (ctl->mFlag.onBit(0)) {
            // These sort of look like value, first order derivative, and second order derivative
            // So perhaps value, velocity, acceleration?
            EGG::Vector2f pos = ctl->mCoreStatus[0].getUnk();
            EGG::Vector2f v = pos - dat->v1;
            dat->v1 = pos;
            dat->v3 = v - dat->v2;
            dat->v2 = v;

            if (!*connected) {
                *connected = true;
            }

            // Not sure why this checks the controller index against the tick count
            if (s_GetWPADInfoInterval != 0 &&
                ((s_GetWPADInfoInterval == 1 || s_GetWPADInfoCount == i) ||
                 (s_GetWPADInfoInterval <= 3 && (s_GetWPADInfoCount & 1) == (i & 1))))
            {
                getWPADInfoCB(i);
            }
        } else if (*connected) {
            ctl->mCoreStatus->init();
            ctl->sceneReset();
            dat->v1.x = 0.0f;
            dat->v1.y = 0.0f;
            dat->v3.x = 0.0f;
            dat->v3.y = 0.0f;
            dat->v2.x = 0.0f;
            dat->v2.y = 0.0f;
            clearWPADInfo(i);
            *connected = false;
        }

        p_ctl++;
        dat++;
        connected++;
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

static inline void clear(WPADInfo *info) {
    info->dpd = FALSE;
    info->speaker = FALSE;
    info->attach = FALSE;
    info->lowBat = FALSE;
    info->nearempty = FALSE;
    info->battery = 0;
    info->led = 0;
    info->protocol = 0;
    info->firmware = 0;
}

static void clearWPADInfo(int controller) {
    s_WPADInfoAvailable[controller] = false;
    clear(&s_WPADInfo[controller]);
}

static void initWPADInfo() {
    for (int i = 0; i < 4; i++) {
        clearWPADInfo(i);
    }
}

extern "C" void fn_80058DA0(s32, s32);

static int getWPADInfoCB(int controller) {
    int result = WPADGetInfoAsync(controller, &s_WPADInfoTmp[controller], fn_80058DA0);
    if (result == -1) {
        clearWPADInfo(controller);
    }
    return result;
}

} // namespace mPad
