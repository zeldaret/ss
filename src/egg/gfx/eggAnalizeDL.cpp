#include "egg/gfx/eggAnalizeDL.h"

#include "nw4r/g3d/res/g3d_resshp.h"
#include "nw4r/g3d/res/g3d_resvtx.h"

namespace EGG {

AnalizeDL::AnalizeDL(nw4r::g3d::ResShp shp) {
    mpData = shp.GetPrimDLTag().GetDL();
    mBufSize = shp.GetPrimDLTag().GetBufSize();
    mCursor = 0;
    field_0x00C = 0;
    shp.GXGetVtxDescv(mVtxDescList);
    shp.GXGetVtxAttrFmtv(mAttrFmtList);
    field_0x00E = 0;
    field_0x00F = 0;
    field_0x010 = 0;

    init();
    nw4r::g3d::ResVtxPos pos = shp.GetResVtxPos();
    pos.GetArray(&mpArr, &mArrStride);
    mStatus = STATUS_NONE;
    field_0x1CC = 0;

    for (int i = 0; i < 10; i++) {
        field_0x1A0[i] = -1;
        field_0x1B4[i] = -1;
    }
}

AnalizeDL::Status AnalizeDL::step() {
    if (mBufSize <= mCursor) {
        mStatus = STATUS_END;
    } else if (field_0x1CC == 0) {
        u8 *pCommand = &mpData[mCursor];
        u8 command = pCommand[0];
        switch (command) {
            case 0x20:
                field_0x1A0[pCommand[4] / 12] = pCommand[2];
                mStatus = STATUS_1;
                mCursor += 5;
                break;
            case 0x28:
                field_0x1B4[pCommand[4] / 9] = pCommand[2];
                mStatus = STATUS_2;
                mCursor += 5;
                break;
            case 0x30: mCursor += 5; break;
            case 0x90:
            case 0xA8:
            case 0xB8:
            case 0xB0:
            case 0xA0:
            case 0x98:
            case 0x80:
                field_0x1C8 = command;
                field_0x1CA = *reinterpret_cast<const u16 *>(pCommand + 1);
                field_0x1CC = field_0x1CA;
                mStatus = STATUS_3;
                mCursor += 3;
                break;
            case 0x0: mCursor += 1; break;
            default:  mCursor += 1; break;
        }
    } else {
        u8 *pCommand = &mpData[mCursor];
        if ((field_0x010 & 1) != 0) {
            mVtxResult.mMtxIdx = field_0x1A0[*(pCommand++) / 3];
        } else {
            mVtxResult.mMtxIdx = -1;
        }

        for (int i = 0; i < 8; i++) {
            if ((field_0x010 & (1 << (i + 1))) != 0) {
                pCommand++;
            }
        }

        if ((field_0x010 & 0x200) != 0) {
            if (field_0x00F == 1) {
                mVtxResult.field_0x04 = *pCommand;
            } else {
                mVtxResult.field_0x04 = *reinterpret_cast<const u16 *>(pCommand);
            }

            int val;
            u8 shift = mAttrFmtList[0].shift;

            const u8 *pData = static_cast<const u8 *>(mpArr);
            pData += mArrStride * mVtxResult.field_0x04;

            switch (mAttrFmtList[0].compType) {
                case GX_U8: {
                    val = 1 << shift;
                    const u8 *dat = reinterpret_cast<const u8 *>(pData);
                    mVtxResult.field_0x08.x = dat[0] / (f32)val;
                    mVtxResult.field_0x08.y = dat[1] / (f32)val;
                    mVtxResult.field_0x08.z = dat[2] / (f32)val;
                    break;
                }
                case GX_S8: {
                    val = 1 << shift;
                    const s8 *dat = reinterpret_cast<const s8 *>(pData);
                    mVtxResult.field_0x08.x = dat[0] / (f32)val;
                    mVtxResult.field_0x08.y = dat[1] / (f32)val;
                    mVtxResult.field_0x08.z = dat[2] / (f32)val;
                    break;
                }
                case GX_U16: {
                    val = 1 << shift;
                    const u16 *dat = reinterpret_cast<const u16 *>(pData);
                    mVtxResult.field_0x08.x = dat[0] / (f32)val;
                    mVtxResult.field_0x08.y = dat[1] / (f32)val;
                    mVtxResult.field_0x08.z = dat[2] / (f32)val;
                    break;
                }
                case GX_S16: {
                    val = 1 << shift;
                    const s16 *dat = reinterpret_cast<const s16 *>(pData);
                    mVtxResult.field_0x08.x = dat[0] / (f32)val;
                    mVtxResult.field_0x08.y = dat[1] / (f32)val;
                    mVtxResult.field_0x08.z = dat[2] / (f32)val;
                    break;
                }
                case GX_F32: {
                    const f32 *dat = reinterpret_cast<const f32 *>(pData);
                    mVtxResult.field_0x08.x = dat[0];
                    mVtxResult.field_0x08.y = dat[1];
                    mVtxResult.field_0x08.z = dat[2];
                    break;
                }
                default: break;
            }
        } else {
            mVtxResult.field_0x04 = -1;
            mVtxResult.field_0x08.z = 0.0f;
            mVtxResult.field_0x08.y = 0.0f;
            mVtxResult.field_0x08.x = 0.0f;
        }
        field_0x1CC -= 1;
        mStatus = STATUS_VTX;
        mCursor += field_0x00E;
    }

    return mStatus;
}

static const s8 sArr1[] = {0, -1, 1, 2};
static const u8 sArr2[] = {1, 1, 2, 2, 4, 0};

void AnalizeDL::init() {
    // TODO
    int k = 0;
    const u8 *pArr2 = sArr2;
    const s8 *pArr1 = sArr1;

    for (int i = 0; mVtxDescList[i].attr != GX_VA_NULL; i++) {
        int x = pArr1[mVtxDescList[i].type];
        pArr1++;
        if (x == -1) {
            if (k >= 0 && k <= 8) {
                x = 1;
            }
        } else {
            int j = 1;
            for (; k != mAttrFmtList[j].attr; j++) {}
            x = pArr2[mAttrFmtList[j - 1].compType];
        }

        if (k == 9) {
            field_0x00F = x;
        }

        field_0x00E += x;
        if (x != 0) {
            field_0x010 |= (1 << k);
        }

        k++;
    }
}

const AnalizeDL::VtxResult &AnalizeDL::getVtxResult() const {
    return mVtxResult;
}

} // namespace EGG
