
#include "toBeSorted/blur_and_palette_manager.h"

#include "common.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/d_obj_light.h"
#include "d/d_sc_game.h"
#include "d/flag/itemflag_manager.h"
#include "m/m_color.h"
#include "m/m_vec.h"
#include "nw4r/math/math_arithmetic.h"

BlurAndPaletteManager BlurAndPaletteManager::sInstance;
BlurAndPaletteManager *BlurAndPaletteManager::sPInstance;

ActorLighting::ListType ActorLighting::sList;

// Same as vectle_calc in d_kankyo_rain from tp
static void vectle_calc(const mVec3_c *pIn, mVec3_c *pOut) {
    f32 s = nw4r::math::FSqrt(pIn->squaredLength());
    if (s != 0.0f) {
        pOut->x = pIn->x / s;
        pOut->y = pIn->y / s;
        pOut->z = pIn->z / s;
    } else {
        pOut->x = 0.0f;
        pOut->y = 0.0f;
        pOut->z = 0.0f;
    }
}

// Same as get_vectle_calc in d_kankyo_rain from tp
void BlurAndPaletteManager::get_vectle_calc(const mVec3_c *pInA, const mVec3_c *pInB, mVec3_c *pOut) {
    mVec3_c pos;
    pos.x = pInB->x - pInA->x;
    pos.y = pInB->y - pInA->y;
    pos.z = pInB->z - pInA->z;
    vectle_calc(&pos, pOut);
}

// TODO(??)
inline f32 sinf(f32 x) {
    return sin(x);
}

// TODO(??)
inline f32 cosf(f32 x) {
    return cos(x);
}

void BlurAndPaletteManager::sphere_to_cartesian(f32 angY, f32 angXZ, mVec3_c *pOut) {
    f32 radY = angY / (180.f / M_PI);
    f32 radXZ = angXZ / (180.f / M_PI);

    mVec3_c pos;
    pos.x = cosf(radY) * sinf(radXZ);
    pos.y = sinf(radY);
    pos.z = cosf(radY) * cosf(radXZ);

    pOut->x = pos.x;
    pOut->y = pos.y;
    pOut->z = pos.z;
}

void BlurAndPaletteManager::efplight_set(LIGHT_INFLUENCE *pLightInfo) {
    if (pLightInfo == nullptr) {
        return;
    }

    pLightInfo->mIdx = 0;
    for (int i = 0; i < 5; ++i) {
        if (efplight[i] == pLightInfo) {
            return;
        }
    }
    for (int i = 0; i < 5; i++) {
        if (efplight[i] == NULL) {
            efplight[i] = pLightInfo;
            efplight[i]->mIdx = i + 1;
            break;
        }
    }
}
void BlurAndPaletteManager::efplight_cut(LIGHT_INFLUENCE *pLightInfo) {
    if (pLightInfo == nullptr) {
        return;
    }

    if (pLightInfo->mIdx != 0) {
        int idx = pLightInfo->mIdx - 1;
        if (idx >= 0 && idx < 5) {
            efplight[idx] = nullptr;
        }
    }
}

LIGHT_INFLUENCE *BlurAndPaletteManager::eflight_influence(const mVec3_c *pPos) {
    f32 max = 1000000.f;
    LIGHT_INFLUENCE *pOut = nullptr;

    for (int i = 0; i < 5; i++) {
        if (efplight[i] != nullptr) {
            if (max > pPos->distance(efplight[i]->mPos) && efplight[i]->mScale > 0.01f) {
                max = pPos->distance(efplight[i]->mPos);
                pOut = efplight[i];
            }
        }
    }

    return pOut;
}

void BlurAndPaletteManager::SordFlush_set(const mVec3_c *pPos, s32 lightType) {
    if (eflight.mState == 0) {
        eflight.mState = 1;
        eflight.mLightType = lightType;
        eflight.field_0x8.mPos = *pPos;
    } else if (eflight.mState == 2 || eflight.mState == 10 || eflight.mState == 11) {
        eflight.mState = 4;
        eflight.mLightType = lightType;
        eflight.field_0x8.mPos = *pPos;
    }
}

void BlurAndPaletteManager::plight_set(LIGHT_INFLUENCE *pLightInfo) {
    if (pLightInfo == nullptr) {
        return;
    }
    pLightInfo->mIdx = 0;

    for (int i = 0; i < 200; i++) {
        // @bug shouldnt this be pointlight instead of efplight?
        if (efplight[i] == pLightInfo) {
            return;
        }
    }

    for (int i = 0; i < 200; i++) {
        if (pointlight[i] == NULL) {
            pointlight[i] = pLightInfo;
            pointlight[i]->mIdx = i + 1;
            break;
        }
    }
}
void BlurAndPaletteManager::plight_cut(LIGHT_INFLUENCE *pLightInfo) {
    if (pLightInfo == nullptr) {
        return;
    }

    if (pLightInfo->mIdx != 0) {
        int idx = pLightInfo->mIdx - 1;
        if (idx >= 0 && idx < 200) {
            pointlight[idx] = nullptr;
        }
    }
}

LIGHT_INFLUENCE *BlurAndPaletteManager::light_influence(const mVec3_c *pPos, bool param2) {
    f32 max = 1000000.f;
    LIGHT_INFLUENCE *pOut = nullptr;

    if (dAcPy_c::GetLink() && dAcPy_c::GetLink()->checkActionFlagsCont(0x400000)) {
        return nullptr;
    }

    for (int i = 0; i < 200; i++) {
        if (pointlight[i] != nullptr && pointlight[i]->field_0x18 == param2) {
            if (max > pPos->distance(pointlight[i]->mPos)) {
                max = pPos->distance(pointlight[i]->mPos);
                pOut = pointlight[i];
            }
        }
    }

    return pOut;
}

LIGHT_INFLUENCE *BlurAndPaletteManager::light_influence2(const mVec3_c *pPos, bool param2) {
    f32 max = 1000000.f;
    f32 temp = 1000000.f;
    LIGHT_INFLUENCE *pOut2 = nullptr;
    LIGHT_INFLUENCE *pOut = nullptr;

    if (dAcPy_c::GetLink() && dAcPy_c::GetLink()->checkActionFlagsCont(0x400000)) {
        return nullptr;
    }
    for (int i = 0; i < 200; i++) {
        if (pointlight[i] != nullptr && pointlight[i]->field_0x18 == param2) {
            f32 dist = pPos->distance(pointlight[i]->mPos);
            if (dist < temp) {
                if (dist < max) {
                    temp = max;
                    max = dist;
                    pOut = pOut2;
                    pOut2 = pointlight[i];
                } else {
                    temp = dist;
                    pOut = pointlight[i];
                }
            }
        }
    }

    return pOut;
}

void BlurAndPaletteManager::shadow_set(SHADOW_INFLUENCE *pShadowInfo) {
    if (pShadowInfo == nullptr) {
        return;
    }

    pShadowInfo->mIdx = 0;
    for (int i = 0; i < 200; ++i) {
        if (pshadow[i] == pShadowInfo) {
            return;
        }
    }
    for (int i = 0; i < 200; i++) {
        if (pshadow[i] == NULL) {
            pshadow[i] = pShadowInfo;
            pshadow[i]->mIdx = i + 1;
            break;
        }
    }
}

void BlurAndPaletteManager::shadow_cut(SHADOW_INFLUENCE *pShadowInfo) {
    if (pShadowInfo == nullptr) {
        return;
    }

    if (pShadowInfo->mIdx != 0) {
        int idx = pShadowInfo->mIdx - 1;
        if (idx >= 0 && idx < 200) {
            pshadow[idx] = nullptr;
        }
    }
}

SHADOW_INFLUENCE *BlurAndPaletteManager::shadow_influence(const mVec3_c *pPos) {
    f32 max = 1000000.f;
    SHADOW_INFLUENCE *pOut = nullptr;

    for (int i = 0; i < 200; i++) {
        if (pshadow[i] != nullptr && pPos->y < pshadow[i]->mPos.y) {
            if (max > pPos->distance(pshadow[i]->mPos)) {
                if (pPos->distance(pshadow[i]->mPos) < pshadow[i]->mRadius) {
                    max = pPos->distance(pshadow[i]->mPos);
                    pOut = pshadow[i];
                }
            }
        }
    }

    return pOut;
}

void BlurAndPaletteManager::setLightFilter(f32 ratio) {
    field_0x2DF4.r = ratio * 255.f;
    field_0x2DF4.g = ratio * 255.f;
    field_0x2DF4.b = ratio * 255.f;
}
void BlurAndPaletteManager::set0x35B0(f32 f) {
    mWind.field_0x10 = f;
}

void BlurAndPaletteManager::setBPM8(const mVec3_c *pos, u32 type, f32 radius) {
    for (int i = 0; i < 10; ++i) {
        if (field_0x3654[i].field_0x10 <= -1) {
            field_0x3654[i].mPos = *pos;
            field_0x3654[i].mRadius = radius;
            field_0x3654[i].field_0x10 = type;
            return;
        }
    }
}

void BlurAndPaletteManager::setBPM8_Type4(const mVec3_c *pos) {
    setBPM8(pos, 4, 250.f);
}

void BlurAndPaletteManager::setBPM8_Type6(const mVec3_c *pos) {
    setBPM8(pos, 6, 500.f);
}

void BlurAndPaletteManager::setBPM8_Type10(const mVec3_c *pos) {
    setBPM8(pos, 10, 1000.f);
}

void BlurAndPaletteManager::setBPM8_Type10_2(const mVec3_c *pos) {
    setBPM8(pos, 10, 1001.f);
}

void BlurAndPaletteManager::setBPM8_Type6_2(const mVec3_c *pos) {
    setBPM8(pos, 6, 501.f);
}

bool BlurAndPaletteManager::check_BPM8(const mVec3_c *pos, mVec3_c *pOutPos) {
    bool ret = false;
    for (int i = 0; i < 10; ++i) {
        if (field_0x3654[i].field_0x10 > 0) {
            if (pos->distance(field_0x3654[i].mPos) <= field_0x3654[i].mRadius) {
                ret = true;
                if (pOutPos) {
                    *pOutPos = field_0x3654[i].mPos;
                }
                break;
            }
        }
    }
    return ret;
}

bool BlurAndPaletteManager::check_BPM8_1001(const mVec3_c *pos, mVec3_c *pOutPos) {
    bool ret = false;
    for (int i = 0; i < 10; ++i) {
        if (field_0x3654[i].field_0x10 > 0) {
            if (pos->distance(field_0x3654[i].mPos) <= field_0x3654[i].mRadius && field_0x3654[i].mRadius == 1001.f) {
                ret = true;
                if (pOutPos) {
                    *pOutPos = field_0x3654[i].mPos;
                }
                break;
            }
        }
    }
    return ret;
}

bool BlurAndPaletteManager::check_BPM8_501(const mVec3_c *pos, mVec3_c *pOutPos) {
    bool ret = false;
    for (int i = 0; i < 10; ++i) {
        if (field_0x3654[i].field_0x10 > 0) {
            if (pos->distance(field_0x3654[i].mPos) <= field_0x3654[i].mRadius && field_0x3654[i].mRadius == 501.f) {
                ret = true;
                if (pOutPos) {
                    *pOutPos = field_0x3654[i].mPos;
                }
                break;
            }
        }
    }
    return ret;
}

bool BlurAndPaletteManager::setUnk(const mVec3_c *pos, f32 radius) {
    bool ret = false;
    if (field_0x38B0 == false) {
        field_0x38B0 = true;
        field_0x38B4 = *pos;
        field_0x38C0 = radius;
        ret = true;
    }

    return ret;
}

void BlurAndPaletteManager::setBPM9(const mVec3_c *pos, f32 radius) {
    for (int i = 0; i < 20; ++i) {
        if (field_0x371C[i].field_0x00 == false) {
            field_0x371C[i].field_0x00 = true;
            field_0x371C[i].mPos = *pos;
            field_0x371C[i].mRadius = radius;
            return;
        }
    }
}

bool BlurAndPaletteManager::check_BPM9(const mVec3_c *pos) {
    bool ret = false;
    for (int i = 0; i < 20; ++i) {
        if (field_0x371C[i].field_0x00) {
            if (pos->distance(field_0x371C[i].mPos) <= field_0x371C[i].mRadius) {
                ret = true;
                break;
            }
        }
    }
    return ret;
}

s32 BlurAndPaletteManager::checkBPM9_Entity(mVec3_c *pos) {
    s32 value = field_0x38C4;
    if (value != 0 && pos != nullptr) {
        *pos = field_0x38C8;
    }
    return value;
}

void BlurAndPaletteManager::setAlterateRoomId(s32 roomId) {
    mAlternateRoomId = roomId;
}

s32 BlurAndPaletteManager::getAlterateRoomId() {
    return mAlternateRoomId;
}

mColor BlurAndPaletteManager::color_ratio_set(const mColor &start, const mColor &end, f32 ratio) {
    mColor result;
    result.Set(0xff, 0xff, 0xff, 0xff);
    result.r = (start.r + ratio * ((f32)end.r - (f32)start.r));
    result.g = (start.g + ratio * ((f32)end.g - (f32)start.g));
    result.b = (start.b + ratio * ((f32)end.b - (f32)start.b));
    result.a = (start.a + ratio * ((f32)end.a - (f32)start.a));
    return result;
}

mVec3_c BlurAndPaletteManager::vec_ratio_set(const mVec3_c &start, const mVec3_c &end, f32 ratio) {
    mVec3_c result;
    result.x = start.x + ratio * (end.x - start.x);
    result.y = start.y + ratio * (end.y - start.y);
    result.z = start.z + ratio * (end.z - start.z);
    return result;
}

f32 BlurAndPaletteManager::f32_ratio_set(const f32 start, const f32 end, f32 ratio) {
    return start + ratio * (end - start);
}

u16 BlurAndPaletteManager::u16_ratio_set(const u16 start, const u16 end, f32 ratio) {
    return (f32)start + ratio * ((f32)end - (f32)start);
}

u8 BlurAndPaletteManager::u8_ratio_set(const u8 start, const u8 end, f32 ratio) {
    return static_cast<u8>((f32)start + ratio * (f32)(end - (f32)start)) & 0xFF;
}

void BlurAndPaletteManager::spf_ratio_set(Spf &out, const Spf &start, const Spf &end, f32 ratio) {
    mColor black0;
    mColor black1;
    out.mActorPalette.mAmbientClr =
        color_ratio_set(start.mActorPalette.mAmbientClr, end.mActorPalette.mAmbientClr, ratio);
    out.mActorPalette.mDiffuseClr =
        color_ratio_set(start.mActorPalette.mDiffuseClr, end.mActorPalette.mDiffuseClr, ratio);
    out.mActorPalette.mBrightnessClr =
        color_ratio_set(start.mActorPalette.mBrightnessClr, end.mActorPalette.mBrightnessClr, ratio);
    out.mActorPalette.mSpecularClr =
        color_ratio_set(start.mActorPalette.mSpecularClr, end.mActorPalette.mSpecularClr, ratio);
    out.mActorPalette.mDarkShadowClr =
        color_ratio_set(start.mActorPalette.mDarkShadowClr, end.mActorPalette.mDarkShadowClr, ratio);
    out.mActorPalette.mDarkLightClr =
        color_ratio_set(start.mActorPalette.mDarkLightClr, end.mActorPalette.mDarkLightClr, ratio);

    out.mActorPalette.mSpecular = f32_ratio_set(start.mActorPalette.mSpecular, end.mActorPalette.mSpecular, ratio);
    out.mActorPalette.field_0x01C =
        color_ratio_set(start.mActorPalette.field_0x01C, end.mActorPalette.field_0x01C, ratio);
    out.mActorPalette.mTemperature =
        f32_ratio_set(start.mActorPalette.mTemperature, end.mActorPalette.mTemperature, ratio);
    out.mActorPalette.field_0x24 = f32_ratio_set(start.mActorPalette.field_0x24, end.mActorPalette.field_0x24, ratio);

    black0 = start.mActorPalette.mShadowClr;
    black1 = end.mActorPalette.mShadowClr;
    if (black0.a == 0) {
        black0.Set(0, 0, 0x1E, 0xC8);
    }
    if (black1.a == 0) {
        black1.Set(0, 0, 0x1E, 0xC8);
    }

    out.mActorPalette.mShadowClr = color_ratio_set(black0, black1, ratio);
    out.mActorPalette.field_0x02C =
        color_ratio_set(start.mActorPalette.field_0x02C, end.mActorPalette.field_0x02C, ratio);

    out.mStagePalette.mBrightness =
        f32_ratio_set(start.mStagePalette.mBrightness, end.mStagePalette.mBrightness, ratio);
    out.mStagePalette.mShadowClr = color_ratio_set(start.mStagePalette.mShadowClr, end.mStagePalette.mShadowClr, ratio);
    out.mStagePalette.mLightClr = color_ratio_set(start.mStagePalette.mLightClr, end.mStagePalette.mLightClr, ratio);
    out.mStagePalette.mSunMoonAngle1 =
        f32_ratio_set(start.mStagePalette.mSunMoonAngle1, end.mStagePalette.mSunMoonAngle1, ratio);
    out.mStagePalette.mSunMoonAngle2 =
        f32_ratio_set(start.mStagePalette.mSunMoonAngle2, end.mStagePalette.mSunMoonAngle2, ratio);

    out.mStagePalette.field_0x014 =
        color_ratio_set(start.mStagePalette.field_0x014, end.mStagePalette.field_0x014, ratio);

    for (int i = 0; i < 5; ++i) {
        out.mStagePalette.field_0x018[i] =
            u8_ratio_set(start.mStagePalette.field_0x018[i], end.mStagePalette.field_0x018[i], ratio);
        out.mStagePalette.field_0x020[i] =
            color_ratio_set(start.mStagePalette.field_0x020[i], end.mStagePalette.field_0x020[i], ratio);
        out.mStagePalette.field_0x034[i] =
            f32_ratio_set(start.mStagePalette.field_0x034[i], end.mStagePalette.field_0x034[i], ratio);
        out.mStagePalette.field_0x048[i] =
            f32_ratio_set(start.mStagePalette.field_0x048[i], end.mStagePalette.field_0x048[i], ratio);
    }

    out.mStagePalette.field_0x05C =
        f32_ratio_set(start.mStagePalette.field_0x05C, end.mStagePalette.field_0x05C, ratio);
    out.mStagePalette.field_0x060 =
        color_ratio_set(start.mStagePalette.field_0x060, end.mStagePalette.field_0x060, ratio);

    for (int i = 0; i < 5; ++i) {
        out.mStagePalette.field_0x064[i] =
            u8_ratio_set(start.mStagePalette.field_0x064[i], end.mStagePalette.field_0x064[i], ratio);
        out.mStagePalette.field_0x06C[i] =
            color_ratio_set(start.mStagePalette.field_0x06C[i], end.mStagePalette.field_0x06C[i], ratio);
        out.mStagePalette.field_0x080[i] =
            f32_ratio_set(start.mStagePalette.field_0x080[i], end.mStagePalette.field_0x080[i], ratio);
        out.mStagePalette.field_0x094[i] =
            f32_ratio_set(start.mStagePalette.field_0x094[i], end.mStagePalette.field_0x094[i], ratio);
    }

    out.mStagePalette.field_0x0A8 =
        f32_ratio_set(start.mStagePalette.field_0x0A8, end.mStagePalette.field_0x0A8, ratio);
    out.mStagePalette.field_0x0AC =
        color_ratio_set(start.mStagePalette.field_0x0AC, end.mStagePalette.field_0x0AC, ratio);

    for (int i = 0; i < 5; ++i) {
        out.mStagePalette.field_0x0B0[i] =
            u8_ratio_set(start.mStagePalette.field_0x0B0[i], end.mStagePalette.field_0x0B0[i], ratio);
        out.mStagePalette.field_0x0B8[i] =
            color_ratio_set(start.mStagePalette.field_0x0B8[i], end.mStagePalette.field_0x0B8[i], ratio);
        out.mStagePalette.field_0x0CC[i] =
            f32_ratio_set(start.mStagePalette.field_0x0CC[i], end.mStagePalette.field_0x0CC[i], ratio);
        out.mStagePalette.field_0x0E0[i] =
            f32_ratio_set(start.mStagePalette.field_0x0E0[i], end.mStagePalette.field_0x0E0[i], ratio);
    }

    out.mStagePalette.field_0x0F4 =
        f32_ratio_set(start.mStagePalette.field_0x0F4, end.mStagePalette.field_0x0F4, ratio);
    out.mStagePalette.field_0x0F8 =
        color_ratio_set(start.mStagePalette.field_0x0F8, end.mStagePalette.field_0x0F8, ratio);

    for (int i = 0; i < 5; ++i) {
        out.mStagePalette.field_0x0FC[i] =
            u8_ratio_set(start.mStagePalette.field_0x0FC[i], end.mStagePalette.field_0x0FC[i], ratio);
        out.mStagePalette.field_0x104[i] =
            color_ratio_set(start.mStagePalette.field_0x104[i], end.mStagePalette.field_0x104[i], ratio);
        out.mStagePalette.field_0x118[i] =
            f32_ratio_set(start.mStagePalette.field_0x118[i], end.mStagePalette.field_0x118[i], ratio);
        out.mStagePalette.field_0x12C[i] =
            f32_ratio_set(start.mStagePalette.field_0x12C[i], end.mStagePalette.field_0x12C[i], ratio);
    }

    out.mSkyPalette.field_0x00 = color_ratio_set(start.mSkyPalette.field_0x00, end.mSkyPalette.field_0x00, ratio);
    out.mSkyPalette.field_0x04 = color_ratio_set(start.mSkyPalette.field_0x04, end.mSkyPalette.field_0x04, ratio);
    out.mSkyPalette.field_0x08 = color_ratio_set(start.mSkyPalette.field_0x08, end.mSkyPalette.field_0x08, ratio);
    out.mSkyPalette.mCloudColor = color_ratio_set(start.mSkyPalette.mCloudColor, end.mSkyPalette.mCloudColor, ratio);
    out.mSkyPalette.mSkyColor = color_ratio_set(start.mSkyPalette.mSkyColor, end.mSkyPalette.mSkyColor, ratio);
    out.mSkyPalette.field_0x14 = color_ratio_set(start.mSkyPalette.field_0x14, end.mSkyPalette.field_0x14, ratio);
    out.mSkyPalette.mSkyFilter = color_ratio_set(start.mSkyPalette.mSkyFilter, end.mSkyPalette.mSkyFilter, ratio);
    out.mSkyPalette.mSkyCenter = vec_ratio_set(start.mSkyPalette.mSkyCenter, end.mSkyPalette.mSkyCenter, ratio);
    out.mSkyPalette.mSkyScale = vec_ratio_set(start.mSkyPalette.mSkyScale, end.mSkyPalette.mSkyScale, ratio);

    out.field_0x1A4.field_0x00.field_0x00 =
        color_ratio_set(start.field_0x1A4.field_0x00.field_0x00, end.field_0x1A4.field_0x00.field_0x00, ratio);
    out.field_0x1A4.field_0x00.field_0x04 =
        f32_ratio_set(start.field_0x1A4.field_0x00.field_0x04, end.field_0x1A4.field_0x00.field_0x04, ratio);
    out.field_0x1A4.field_0x00.field_0x08 =
        f32_ratio_set(start.field_0x1A4.field_0x00.field_0x08, end.field_0x1A4.field_0x00.field_0x08, ratio);

    out.field_0x1A4.field_0x0C[0] =
        f32_ratio_set(start.field_0x1A4.field_0x0C[0], end.field_0x1A4.field_0x0C[0], ratio);
    out.field_0x1A4.field_0x0C[1] =
        f32_ratio_set(start.field_0x1A4.field_0x0C[1], end.field_0x1A4.field_0x0C[1], ratio);
    out.field_0x1A4.field_0x0C[2] =
        f32_ratio_set(start.field_0x1A4.field_0x0C[2], end.field_0x1A4.field_0x0C[2], ratio);

    out.field_0x1BC.field_0x00 = u8_ratio_set(start.field_0x1BC.field_0x00, end.field_0x1BC.field_0x00, ratio);
    out.field_0x1BC.field_0x04 = f32_ratio_set(start.field_0x1BC.field_0x04, end.field_0x1BC.field_0x04, ratio);
    out.field_0x1BC.field_0x08 = f32_ratio_set(start.field_0x1BC.field_0x08, end.field_0x1BC.field_0x08, ratio);

    out.field_0x1BC.field_0x0C = u8_ratio_set(start.field_0x1BC.field_0x0C, end.field_0x1BC.field_0x0C, ratio);
    out.field_0x1BC.field_0x10 = f32_ratio_set(start.field_0x1BC.field_0x10, end.field_0x1BC.field_0x10, ratio);
    out.field_0x1BC.field_0x14 = f32_ratio_set(start.field_0x1BC.field_0x14, end.field_0x1BC.field_0x14, ratio);

    out.field_0x1BC.field_0x18 = u8_ratio_set(start.field_0x1BC.field_0x18, end.field_0x1BC.field_0x18, ratio);

    out.field_0x1BC.field_0x19 = color_ratio_set(
        mColor(*(u32 *)&start.field_0x1BC.field_0x19), mColor(*(u32 *)&end.field_0x1BC.field_0x19), ratio
    );

    out.field_0x1BC.field_0x1D = u8_ratio_set(start.field_0x1BC.field_0x1D, end.field_0x1BC.field_0x1D, ratio);

    out.field_0x1D4.field_0x00 = f32_ratio_set(start.field_0x1D4.field_0x00, end.field_0x1D4.field_0x00, ratio);
    out.field_0x1D4.field_0x04 = f32_ratio_set(start.field_0x1D4.field_0x04, end.field_0x1D4.field_0x04, ratio);
    out.field_0x1D4.field_0x08 = f32_ratio_set(start.field_0x1D4.field_0x08, end.field_0x1D4.field_0x08, ratio);
    out.field_0x1D4.field_0x0C = f32_ratio_set(start.field_0x1D4.field_0x0C, end.field_0x1D4.field_0x0C, ratio);

    out.field_0x1D4.field_0x10 = u8_ratio_set(start.field_0x1D4.field_0x10, end.field_0x1D4.field_0x10, ratio);
    out.field_0x1D4.field_0x11 = u8_ratio_set(start.field_0x1D4.field_0x11, end.field_0x1D4.field_0x11, ratio);

    out.field_0x1D4.field_0x12 = u16_ratio_set(start.field_0x1D4.field_0x12, end.field_0x1D4.field_0x12, ratio);

    out.field_0x1D4.field_0x14 = f32_ratio_set(start.field_0x1D4.field_0x14, end.field_0x1D4.field_0x14, ratio);
    out.field_0x1D4.field_0x18 = f32_ratio_set(start.field_0x1D4.field_0x18, end.field_0x1D4.field_0x18, ratio);
    out.field_0x1D4.field_0x1C = f32_ratio_set(start.field_0x1D4.field_0x1C, end.field_0x1D4.field_0x1C, ratio);
    out.field_0x1D4.field_0x20 = f32_ratio_set(start.field_0x1D4.field_0x20, end.field_0x1D4.field_0x20, ratio);

    out.mParticleTransparentClr = color_ratio_set(start.mParticleTransparentClr, end.mParticleTransparentClr, ratio);
    out.mParticleSolidClr = color_ratio_set(start.mParticleSolidClr, end.mParticleSolidClr, ratio);
}

void __Spf_ct_dt_genertation() {
    Spf s;
}

void BlurAndPaletteManager::set_palette_transition(s16 before, s16 after, s32 numFrames) {
    mTransitionSpfSetting.mIdxBefore = before;
    mTransitionSpfSetting.mIdxAfter = after;
    mTransitionSpfSetting.mNumFrames = numFrames;

    if (after == 50) {
        if (dScGame_c::currentSpawnInfo.isNight()) {
            mTransitionSpfSetting.mIdxAfter = 3;
        } else {
            mTransitionSpfSetting.mIdxAfter = 5;
        }
    } else if (after == 51) {
        if (ItemflagManager::sInstance->getFlagDirect(ITEM_AMBER_TABLET)) {
            if (dScGame_c::currentSpawnInfo.isNight()) {
                mTransitionSpfSetting.mIdxAfter = 10;
            } else {
                mTransitionSpfSetting.mIdxAfter = 9;
            }
        } else if (ItemflagManager::sInstance->getFlagDirect(ITEM_RUBY_TABLET)) {
            if (dScGame_c::currentSpawnInfo.isNight()) {
                mTransitionSpfSetting.mIdxAfter = 8;
            } else {
                mTransitionSpfSetting.mIdxAfter = 7;
            }
        } else {
            mTransitionSpfSetting.mIdxAfter = 6;
        }
    }
}

void BlurAndPaletteManager::set_override_spf(s16 start, s16 end, f32 ratio) {
    mOverrideSpfSetting.mIdxStart = start;
    mOverrideSpfSetting.mIdxEnd = end;
    mOverrideSpfSetting.mRatio = ratio;
}

void BlurAndPaletteManager::setMist(u8 mode, u8 decay, u8 alpha, f32 dir, f32 speed) {
    mMistInfo.mMode = mode;
    mMistInfo.mDecay = decay;
    mMistInfo.mDirection = dir;
    mMistInfo.mSpeed = speed;
    mMistInfo.mAlpha = alpha;
}

void BlurAndPaletteManager::setMistMode3(const mVec3_c &pos, f32 value) {
    mMistInfo.mMode = 3;
    mMistInfo.mDecay = value * 128.f;
    mMistPos = pos;
    mMistInfo.mSpeed = value * 200.f;
    mMistInfo.mAlpha = 0;
}

void BlurAndPaletteManager::setDOF(f32 dof) {
    mDofEnabled = true;
    mDof = dof;
}

void BlurAndPaletteManager::disableDOF() {
    mDofEnabled = false;
}

void BlurAndPaletteManager::light_influence_ratio_set(
    LIGHT_INFLUENCE &out, const LIGHT_INFLUENCE &start, const LIGHT_INFLUENCE &end, f32 ratio
) {
    mColor black(0, 0, 0, 0);

    out.mIdx = start.mIdx;
    out.field_0x18 = start.field_0x18;

    if (ratio <= 0.5f) {
        f32 i_ratio = ratio * 2.f;
        out.mPos = start.mPos;
        out.mClr = color_ratio_set(start.mClr, black, i_ratio);
        out.mScale = f32_ratio_set(start.mScale, 0.f, i_ratio);
    } else {
        f32 i_ratio = (ratio - 0.5f) * 2.f;
        out.mPos = end.mPos;
        out.mClr = color_ratio_set(black, end.mClr, i_ratio);
        out.mScale = f32_ratio_set(0.f, end.mScale, i_ratio);
    }
}

void BlurAndPaletteManager::sff_ratio_set(Sff &out, const Sff &start, const Sff &end, f32 ratio) {
    for (int i = 0; i < 3; ++i) {
        out.field_0x00[i].field_0x00 =
            color_ratio_set(start.field_0x00[i].field_0x00, end.field_0x00[i].field_0x00, ratio);
        out.field_0x00[i].field_0x04 =
            f32_ratio_set(start.field_0x00[i].field_0x04, end.field_0x00[i].field_0x04, ratio);
        out.field_0x00[i].field_0x08 =
            f32_ratio_set(start.field_0x00[i].field_0x08, end.field_0x00[i].field_0x08, ratio);
    }
}

void __Sff_ct_dt_genertation() {
    Sff s;
}

void BlurAndPaletteManager::fn_80024740(u8 idx, u8 value) {
    if (idx != 0) {
        field_0x5D59[idx - 1].mEnabled = true;
        field_0x5D59[idx - 1].mValue = value;
    }
}

bool BlurAndPaletteManager::fn_80024770(u8 idx) {
    bool ret = false;
    if (idx != 0) {
        ret = field_0x5D59[idx - 1].mEnabled;
    }
    return ret;
}
bool BlurAndPaletteManager::fn_800247A0(u8 idx) {
    bool ret = false;
    if (idx != 0) {
        if (field_0x5D59[idx - 1].mValue != 0) {
            ret = true;
        }
    }
    return ret;
}

void BlurAndPaletteManager::setWind(mVec3_c mPos, f32 f) {
    mWind.mVel = mPos;
    mWind.field_0x0C = f;
}

BlurAndPaletteManager::BlurAndPaletteManager() {
    mColor grey_aaaaaa = mColor(0xaa, 0xaa, 0xaa, 0xff);
    mColor white_ffffff = mColor(0xff, 0xff, 0xff, 0xff);
    mColor cream_ffffc0 = mColor(0xff, 0xff, 0xc0, 0xff);
    mColor grey_808080 = mColor(0x80, 0x80, 0x80, 0xff);
    mColor grey_787878 = mColor(0x78, 0x78, 0x78, 0xff);
    mColor white2_ffffff = mColor(0xff, 0xff, 0xff, 0xff);
    mColor grey_7f7f7f = mColor(0x7f, 0x7f, 0x7f, 0xff);
    mColor yellow_ffd86b = mColor(0xff, 0xd8, 0x6b, 0xff);
    mColor sea_blue_29424e = mColor(0x29, 0x42, 0x4e, 0xff);
    mColor seafoam_green_b9d8b7 = mColor(0xb9, 0xd8, 0xb7, 0xff);
    mColor white3_ffffff = mColor(0xff, 0xff, 0xff, 0xff);
    mColor black_000000 = mColor(0x00, 0x00, 0x00, 0xff);
    mColor pastel_orange_ffb787 = mColor(0xff, 0xb7, 0x87, 0xff);
    mColor lightblue_aad2ff = mColor(0xaa, 0xd2, 0xff, 0xff);
    mVec3_c v1(0.0f, 1.0f, 0.0f);
    mColor white4_ffffff = mColor(0xff, 0xff, 0xff, 0xff);
    f32 f1 = 2000.0f;
    f32 f2 = 100000.0f;
    mVec3_c v2(0.0f, 0.0f, 0.0f);
    mVec3_c v3(1.0f, 1.0f, 1.0f);
    mColor white5_ffffff = mColor(0xff, 0xff, 0xff, 0xff);
    mColor pastel_yellow_fffb04 = mColor(0xff, 0xf0, 0xb4, 0xff);
    mColor grey_3c3c3c = mColor(0x3c, 0x3c, 0x3c, 0xff);
    mColor grey_a0a091 = mColor(0xa0, 0xa0, 0x91, 0xff);
    mColor magenta_7d0091 = mColor(0x7d, 0x00, 0x91, 0xff);
    mColor sand_ffebb4 = mColor(0xff, 0xeb, 0xb4, 0xff);

    sPInstance = this;

    currentSpf.mActorPalette.mAmbientClr = grey_aaaaaa;
    currentSpf.mActorPalette.mDiffuseClr = white_ffffff;
    currentSpf.mActorPalette.mBrightnessClr = grey_808080;
    currentSpf.mActorPalette.mSpecularClr = cream_ffffc0;
    currentSpf.mActorPalette.mDarkShadowClr = grey_aaaaaa;
    currentSpf.mActorPalette.mDarkLightClr = white_ffffff;
    currentSpf.mActorPalette.mSpecular = 0.6f;
    currentSpf.mActorPalette.field_0x01C = pastel_orange_ffb787;
    currentSpf.mActorPalette.field_0x24 = 195.0f;
    currentSpf.mActorPalette.mTemperature = 0.5f;
    currentSpf.mActorPalette.mShadowClr = black_000000;
    currentSpf.mActorPalette.field_0x02C = black_000000;

    currentSpf.mStagePalette.mShadowClr = grey_787878;
    currentSpf.mStagePalette.mLightClr = white2_ffffff;
    currentSpf.mStagePalette.field_0x014 = grey_7f7f7f;
    currentSpf.mStagePalette.field_0x060 = grey_7f7f7f;
    currentSpf.mStagePalette.field_0x0AC = grey_7f7f7f;
    currentSpf.mStagePalette.field_0x0F8 = grey_7f7f7f;

    currentSpf.mSkyPalette.mCloudColor = yellow_ffd86b;
    currentSpf.mSkyPalette.mSkyColor = sea_blue_29424e;
    currentSpf.mSkyPalette.field_0x14 = seafoam_green_b9d8b7;
    currentSpf.mSkyPalette.mSkyFilter = white3_ffffff;
    currentSpf.mSkyPalette.mSkyCenter = v2;
    currentSpf.mSkyPalette.mSkyScale = v3;

    currentSpf.mStagePalette.mBrightness = 0.0625f;
    currentSpf.mStagePalette.field_0x05C = 0.0625f;
    currentSpf.mStagePalette.field_0x0A8 = 0.0625f;
    currentSpf.mStagePalette.field_0x0F4 = 0.0625f;

    currentSpf.mSkyPalette.field_0x00 = grey_787878;
    currentSpf.mSkyPalette.field_0x04 = grey_787878;
    currentSpf.mSkyPalette.field_0x08 = grey_787878;

    currentSpf.field_0x1A4.field_0x00.field_0x00 = white4_ffffff;
    currentSpf.field_0x1A4.field_0x00.field_0x04 = f1;
    currentSpf.field_0x1A4.field_0x00.field_0x08 = f2;

    currentSpf.mStagePalette.mSunMoonAngle1 = 45.0f;
    currentSpf.mStagePalette.mSunMoonAngle2 = 180.0f;

    for (int i = 0; i < 5; i++) {
        currentSpf.mStagePalette.field_0x020[i] = black_000000;
        field_0x2E08[i] = v1;
        currentSpf.mStagePalette.field_0x018[i] = 0;
        currentSpf.mStagePalette.field_0x034[i] = 0.0f;
        currentSpf.mStagePalette.field_0x048[i] = 0.0f;

        currentSpf.mStagePalette.field_0x06C[i] = black_000000;
        field_0x2E44[i] = v1;
        currentSpf.mStagePalette.field_0x064[i] = 0;
        currentSpf.mStagePalette.field_0x080[i] = 0.0f;
        currentSpf.mStagePalette.field_0x094[i] = 0.0f;

        currentSpf.mStagePalette.field_0x0B8[i] = black_000000;
        field_0x2E80[i] = v1;
        currentSpf.mStagePalette.field_0x0B0[i] = 0;
        currentSpf.mStagePalette.field_0x0CC[i] = 0.0f;
        currentSpf.mStagePalette.field_0x0E0[i] = 0.0f;

        currentSpf.mStagePalette.field_0x104[i] = black_000000;
        field_0x2EBC[i] = v1;
        currentSpf.mStagePalette.field_0x0FC[i] = 0;
        currentSpf.mStagePalette.field_0x118[i] = 0.0f;
        currentSpf.mStagePalette.field_0x12C[i] = 0.0f;
    }

    currentSpf.field_0x1A4.field_0x0C[0] = 0.55f;
    currentSpf.field_0x1A4.field_0x0C[1] = 1.0f;
    currentSpf.field_0x1A4.field_0x0C[2] = 0.0f;

    currentSpf.field_0x1BC.field_0x00 = 0;
    currentSpf.field_0x1BC.field_0x04 = 1.0f;
    currentSpf.field_0x1BC.field_0x08 = 0.5f;
    currentSpf.field_0x1BC.field_0x0C = 0;
    currentSpf.field_0x1BC.field_0x10 = 0.5f;
    currentSpf.field_0x1BC.field_0x14 = 0.0f;

    currentSpf.field_0x1BC.field_0x18 = 0;
    currentSpf.field_0x1BC.field_0x19 = pastel_yellow_fffb04;
    currentSpf.field_0x1BC.field_0x1D = 1;

    currentSpf.field_0x1D4.field_0x00 = 3300.0f;
    currentSpf.field_0x1D4.field_0x04 = 3300.0f;
    currentSpf.field_0x1D4.field_0x08 = 1000.0f;
    currentSpf.field_0x1D4.field_0x0C = 500.0f;
    currentSpf.field_0x1D4.field_0x10 = 0xFF;
    currentSpf.field_0x1D4.field_0x11 = 1;
    currentSpf.field_0x1D4.field_0x12 = 0;
    currentSpf.field_0x1D4.field_0x14 = 0.0f;
    currentSpf.field_0x1D4.field_0x18 = 0.0f;
    currentSpf.field_0x1D4.field_0x1C = 6.93359f;
    currentSpf.field_0x1D4.field_0x20 = 8.0f;
    currentSpf.mParticleTransparentClr = black_000000;
    currentSpf.mParticleSolidClr = black_000000;

    for (int i = 0; i < 3; i++) {
        currentSff.field_0x00[i].field_0x00 = white4_ffffff;
        currentSff.field_0x00[i].field_0x04 = f1;
        currentSff.field_0x00[i].field_0x08 = f2;
    }

    for (int i = 0; i < 20; i++) {
        spfs[i] = currentSpf;
        sffs[i] = currentSff;
    }

    field_0x2DEC = lightblue_aad2ff;
    field_0x2EF8 = grey_a0a091;
    field_0x2EFC = grey_3c3c3c;
    field_0x2F00 = v1;

    field_0x2F0C = 30000;
    field_0x2F0E = -8000;
    field_0x2F10 = 0;
    field_0x2DF8 = v1;

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 8; j++) {
            field_0x38E4.field_0x00[i].field_0x00[j].field_0x00 = mColor(0xff, 0xff, 0xff, 0xff);
            field_0x38E4.field_0x00[i].field_0x00[j].field_0x04 = mColor(0xff, 0xff, 0xff, 0xff);
            field_0x38E4.field_0x00[i].field_0x00[j].field_0x08 = mColor(0xff, 0xff, 0xff, 0xff);
            field_0x38E4.field_0x00[i].field_0x00[j].field_0x0C = mColor(0xff, 0xff, 0xff, 0xff);
            field_0x48E4.field_0x00[i].field_0x00[j].field_0x00 = mColor(0xff, 0xff, 0xff, 0xff);
            field_0x48E4.field_0x00[i].field_0x00[j].field_0x04 = mColor(0xff, 0xff, 0xff, 0xff);
            field_0x48E4.field_0x00[i].field_0x00[j].field_0x08 = mColor(0xff, 0xff, 0xff, 0xff);
            field_0x48E4.field_0x00[i].field_0x00[j].field_0x0C = mColor(0xff, 0xff, 0xff, 0xff);
            field_0x48E4.field_0x00[i].field_0x00[j].field_0x10 = 0;
        }
    }

    field_0x5CE4.MA00_kColor3 = mColor(0xff, 0xff, 0xff, 0xff);
    field_0x5CE4.MA01_tevReg1 = mColor(0xff, 0xff, 0xff, 0xff);
    field_0x5CE4.MA01_kColor2 = mColor(0xff, 0xff, 0xff, 0xff);
    field_0x5CE4.MA02_tevReg1 = mColor(0xff, 0xff, 0xff, 0xff);
    field_0x5CE4.MA02_kColor2 = mColor(0xff, 0xff, 0xff, 0xff);
    field_0x5CE4.MA03_kColor3 = mColor(0xff, 0xff, 0xff, 0xff);

    field_0x5D04.field_0x00 = field_0x5CE4;
    field_0x5D04.field_0x20 = field_0x5CE4;
}

BlurAndPaletteManager::~BlurAndPaletteManager() {
    sPInstance = nullptr;
}

void ActorLighting::fn_80026500() {
    for (ListType::Iterator start = sList.GetBeginIter(); start != sList.GetEndIter(); ++start) {
        // Probably stripped
    }
}

// I want to say something else generated this...
ActorLighting::~ActorLighting() {}

void ActorLighting::reset() {
    field_0x0C = -999999.f;
    field_0x10 = -999999.f;
    field_0x14 = -999999.f;
    field_0x18 = -999999.f;
    field_0x1C = -999999.f;
    field_0x20 = -999999.f;
    field_0x24 = -999999.f;
    field_0x28 = -999999.f;
    field_0x2C = -999999.f;

    field_0x38 = 999999.f;

    mLightSetIdx = -1;

    mLightingCode = 0xF; // TODO(Lighting Code)
    field_0x61 = 0;
    field_0x62 = 0;
    field_0x30 = 0.f;
    field_0x34 = 1.f;
    field_0x63 = 0;
    field_0x64 = 0;
    mTev0Color = mColor(0, 0, 0, 0);
    mTevK0Color = mColor(0, 0, 0, 0);

    mTev1Color = mColor(0, 0, 0, 0);
    mTev2Color = mColor(0, 0, 0, 0);
    mTevPrevColor = mColor(0, 0, 0, 0);

    mTevK1Color = mColor(0, 0, 0, 0);
    mTevK2Color = mColor(0, 0, 0, 0);
    mTevK3Color = mColor(0, 0, 0, 0);

    mUseTev1 = false;
    mUseTev2 = false;
    mUseTevPrev = false;
    mUseTevK1 = false;
    mUseTevK2 = false;
    mUseTevK3 = false;
}

const mColor &ActorLighting::getLightTev2Color() {
    if (dObjLight_c::GetInstance() == nullptr) {
        return mColor(0, 0, 0, 0);
    } else {
        return dObjLight_c::GetInstance()->GetColor0x68();
    }
}

const mColor &ActorLighting::getLightTev1Color() {
    if (dObjLight_c::GetInstance() == nullptr) {
        return mColor(0, 0, 0, 0);
    } else {
        return dObjLight_c::GetInstance()->GetColor0x6C();
    }
}

const mColor &ActorLighting::getLightTev0Color() {
    if (dObjLight_c::GetInstance() == nullptr) {
        return mColor(0, 0, 0, 0);
    } else {
        return dObjLight_c::GetInstance()->GetColor0x100();
    }
}

const mColor &ActorLighting::getLightTevKColor() {
    if (dObjLight_c::GetInstance() == nullptr) {
        return mColor(0, 0, 0, 0);
    } else {
        return dObjLight_c::GetInstance()->GetColor0x104();
    }
}
