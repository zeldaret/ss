#include <egg/core/eggVideo.h>
#include <rvl/OS/OSHardware.h>
#include <rvl/OS/OSTime.h>
#include <rvl/SC/scapi.h>

namespace EGG {

// TODO give these proper names

const GXRenderModeObj renderMode0 = {
        0,   // tvInfo
        640, // fbWidth
        456, // efbHeight
        456, // xfbHeight
        25,  // viXOrigin
        12,  // viYOrigin
        670, // viWidth
        456, // viHeight
        1,   // xfbMode
        0,   // field_rendering
        0,   // aa
        {
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
        },                       // sample_pattern
        {7, 7, 12, 12, 12, 7, 7} // vFilter
};

const GXRenderModeObj renderMode1 = {
        2,   // tvInfo
        640, // fbWidth
        456, // efbHeight
        456, // xfbHeight
        25,  // viXOrigin
        12,  // viYOrigin
        670, // viWidth
        456, // viHeight
        0,   // xfbMode
        0,   // field_rendering
        0,   // aa
        {
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
        },                       // sample_pattern
        {0, 0, 21, 22, 21, 0, 0} // vFilter
};

const GXRenderModeObj renderMode2 = {
        4,   // tvInfo
        640, // fbWidth
        456, // efbHeight
        542, // xfbHeight
        27,  // viXOrigin
        16,  // viYOrigin
        666, // viWidth
        542, // viHeight
        1,   // xfbMode
        0,   // field_rendering
        0,   // aa
        {
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
        },                       // sample_pattern
        {7, 7, 12, 12, 12, 7, 7} // vFilter
};

const GXRenderModeObj renderMode3 = {
        20,  // tvInfo
        640, // fbWidth
        456, // efbHeight
        456, // xfbHeight
        25,  // viXOrigin
        12,  // viYOrigin
        670, // viWidth
        456, // viHeight
        1,   // xfbMode
        0,   // field_rendering
        0,   // aa
        {
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
        },                       // sample_pattern
        {7, 7, 12, 12, 12, 7, 7} // vFilter
};

const GXRenderModeObj renderMode4 = {
        22,  // tvInfo
        640, // fbWidth
        456, // efbHeight
        456, // xfbHeight
        25,  // viXOrigin
        12,  // viYOrigin
        670, // viWidth
        456, // viHeight
        0,   // xfbMode
        0,   // field_rendering
        0,   // aa
        {
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
        },                       // sample_pattern
        {0, 0, 21, 22, 21, 0, 0} // vFilter
};

const GXRenderModeObj renderMode5 = {
        0,   // tvInfo
        640, // fbWidth
        456, // efbHeight
        456, // xfbHeight
        17,  // viXOrigin
        12,  // viYOrigin
        686, // viWidth
        456, // viHeight
        1,   // xfbMode
        0,   // field_rendering
        0,   // aa
        {
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
        },                       // sample_pattern
        {7, 7, 12, 12, 12, 7, 7} // vFilter
};

const GXRenderModeObj renderMode6 = {
        2,   // tvInfo
        640, // fbWidth
        456, // efbHeight
        456, // xfbHeight
        17,  // viXOrigin
        12,  // viYOrigin
        686, // viWidth
        456, // viHeight
        0,   // xfbMode
        0,   // field_rendering
        0,   // aa
        {
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
        },                       // sample_pattern
        {0, 0, 21, 22, 21, 0, 0} // vFilter
};

const GXRenderModeObj renderMode7 = {
        4,   // tvInfo
        640, // fbWidth
        456, // efbHeight
        542, // xfbHeight
        19,  // viXOrigin
        16,  // viYOrigin
        682, // viWidth
        542, // viHeight
        1,   // xfbMode
        0,   // field_rendering
        0,   // aa
        {
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
        },                       // sample_pattern
        {7, 7, 12, 12, 12, 7, 7} // vFilter
};

const GXRenderModeObj renderMode8 = {
        20,  // tvInfo
        640, // fbWidth
        456, // efbHeight
        456, // xfbHeight
        17,  // viXOrigin
        12,  // viYOrigin
        686, // viWidth
        456, // viHeight
        1,   // xfbMode
        0,   // field_rendering
        0,   // aa
        {
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
        },                       // sample_pattern
        {7, 7, 12, 12, 12, 7, 7} // vFilter
};

const GXRenderModeObj renderMode9 = {
        22,  // tvInfo
        640, // fbWidth
        456, // efbHeight
        456, // xfbHeight
        17,  // viXOrigin
        12,  // viYOrigin
        686, // viWidth
        456, // viHeight
        0,   // xfbMode
        0,   // field_rendering
        0,   // aa
        {
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
                {6, 6},
        },                       // sample_pattern
        {0, 0, 21, 22, 21, 0, 0} // vFilter
};

const EGG::Video::RenderModeObjSet renderModes = {{&renderMode0, &renderMode1, &renderMode2, &renderMode3,
        &renderMode4, &renderMode5, &renderMode6, &renderMode7, &renderMode8, &renderMode9}};


/* 80498690 */ void Video::initialize(GXRenderModeObj *obj, const RenderModeObjSet *set) {
    VIInit();
    configure(obj, set);
}

/* 804986f0 */ GXRenderModeObj *Video::configure(GXRenderModeObj *obj, const RenderModeObjSet *set) {
    GXRenderModeObj *oldMode = pRenderMode;
    if (set == nullptr) {
        set = &renderModes;
    }
    if (obj == nullptr) {
        // Cast away constness. In an incredibly cursed way, adding proper
        // const correctness to this class changes regalloc
        obj = (GXRenderModeObj *)getStandardRenderModeObj(set);
    }
    if (pRenderMode != obj) {
        pRenderMode = obj;
        VISetBlack(true);
        VIConfigure(obj);
        VIFlush();
        mConfiguredTime = OSGetTick();
        mFlag.setBit(0);
        u16 efbWidth = obj->fbWidth;
        u16 efbHeight = obj->efbHeight;
        f32 factor = GXGetYScaleFactor(efbHeight, obj->xfbHeight);
        u16 lines = GXGetNumXfbLines(efbHeight, factor);
        GXSetDispCopySrc(0, 0, efbWidth, efbHeight);
        GXSetDispCopyDst(efbWidth, lines);
        GXSetDispCopyYScale(factor);
        VIWaitForRetrace();
        VIWaitForRetrace();
    }

    return oldMode;
}

// Random function that pretends to do an int to float
// cast here, for float ordering issues
f32 itof(u32 n) {
    return n;
}
// TODO VITvFormat
/* 80498800 */ u32 Video::getTickPerVRetrace(u32 tvFormat) {
    f32 val = tvFormat - 1 <= VI_TV_FMT_PAL ? 50.0f : 59.94f;
    return (u32)((OS_BUS_CLOCK_SPEED >> 2) / val);
}
/* 80498860 */ u32 Video::getTickPerVRetrace() {
    return getTickPerVRetrace(VIGetTvFormat());
}
/* 80498890 */ const GXRenderModeObj *Video::getStandardRenderModeObj(const RenderModeObjSet *set) {
    bool pmode = SCGetProgressiveMode() == 1;
    bool rbg60mode = SCGetEuRgb60Mode() == 1;
    bool aspect = SCGetAspectRatio() == 0;
    bool dtvstatus = VIGetDTVStatus() == 1;
    u32 tvFormat = VIGetTvFormat();
    
    bool isNtscLike;
    switch (tvFormat) {
        case VI_TV_FMT_NTSC:
            isNtscLike = true;
            break;
        case VI_TV_FMT_PAL:
        case VI_TV_FMT_EURGB60:
            isNtscLike = false;
            break;
        default:
            isNtscLike = true;
    }

    if (dtvstatus && pmode) {
        if (isNtscLike) {
            if (aspect) {
                return set->table[1];
            } else {
                return set->table[6];
            }
        } else if (aspect) {
            return set->table[4];
        } else {
            return set->table[9];
        }
    } else if (isNtscLike) {
        if (aspect) {
            return set->table[0];
        } else {
            return set->table[5];
        }
    } else if (rbg60mode) {
        if (aspect) {
            return set->table[3];
        } else {
            return set->table[8];
        }
    } else if (aspect) {
        return set->table[2];
    }
    return set->table[7];
}

} // namespace EGG
