#include "d/d_rumble.h"

#include "common.h"
#include "d/d_reset.h"
#include "d/d_sc_game.h"
#include "d/lyt/d_lyt_control_game.h"
#include "m/m_pad.h"


dRumbleEntry_c dRumble_c::sRumblePreset0(8, 0b11000000000000000000000000000000, 2.f);
dRumbleEntry_c dRumble_c::sRumblePreset1(9, 0b11010000000000000000000000000000, 4.f);
dRumbleEntry_c dRumble_c::sRumblePreset2(10, 0b11101100000000000000000000000000, 6.f);
dRumbleEntry_c dRumble_c::sRumblePreset3(12, 0b11101101000000000000000000000000, 8.f);
dRumbleEntry_c dRumble_c::sRumblePreset4(12, 0b11101110110000000000000000000000, 10.f);
dRumbleEntry_c dRumble_c::sRumblePreset5(16, 0b11011101101100000000000000000000, 12.f);
dRumbleEntry_c dRumble_c::sRumblePreset6(16, 0b11101111111011000000000000000000, 14.f);
dRumbleEntry_c dRumble_c::sRumblePreset7(32, 0b11111110111111000000000000000000, 14.f);
dRumbleEntry_c dRumble_c::sRumblePreset8(8, 0b11100000000000000000000000000000, 2.f);
dRumbleEntry_c dRumble_c::sRumblePreset9(16, 0b11100000000000000000000000000000, 2.f);
dRumbleEntry_c dRumble_c::sRumblePreset10(24, 0b11100000000000000000000000000000, 2.f);

dRumble_c *dRumble_c::spInstance;

dRumble_c::dRumble_c() {}

dRumble_c::~dRumble_c() {
    spInstance = nullptr;
}

void dRumble_c::create() {
    spInstance = new dRumble_c();

    spInstance->mRumbleData[0].mBitsLeft = 0;
    spInstance->mRumbleData[1].mBitsLeft = 0;
    spInstance->mRumbleData[2].mBitsLeft = 0;
    spInstance->mRumbleData[3].mBitsLeft = 0;
}

void dRumble_c::execute() {
    // If the game is not being controlled, do not do anything
    if (dLytControlGame_c::getInstance()) {
        if (!dLytControlGame_c::getInstance()->isStateNormal()) {
            return;
        }
    }

    // I am guessing on any reload screen or homebutton menu?
    if (dReset::Manage_c::GetInstance()->is1Or5()) {
        mPad::g_core[0]->stopRumbleMgr();
        mPad::g_core[1]->stopRumbleMgr();
        return;
    }

    // These structs are only used here.
    // No need to make them global
    struct Entry {
        u32 bits;
        s32 length;
        bool isActive;
    } entries[2] = {};
    struct Info {
        u32 mBits;
        f32 intensities[4];
        s32 combinedLength;
        s32 _unused;
    } info = {};

    for (s32 i = 0; i < 4; ++i) {
        RumbleData &data = spInstance->mRumbleData[i];
        if (data.mBitsLeft == 0) {
            continue;
        }

        u32 bit = data.mLength - data.mBitsLeft;
        if (data.mFlags & FLAG_ACTIVE) {
            info.mBits |= data.mRumbleBits << bit;
            if (data.mFlags & FLAG_INITIALIZE) {
                info.intensities[i] = data.mIntensity;
            } else {
                info.intensities[i] = data.mIntensity * ((f32)data.mBitsLeft / data.mLength);
            }
            if (info.combinedLength < data.mBitsLeft) {
                info.combinedLength = data.mBitsLeft;
            }
        } else {
            info.intensities[i] = 0.f;
        }

        if (data.mFlags & FLAG_SLOT0) {
            entries[0].bits |= data.mRumbleBits << bit;

            if (entries[0].length < data.mBitsLeft) {
                entries[0].length = data.mBitsLeft;
            }
            if (bit == 0) {
                entries[0].isActive = true;
            }
        }

        if (data.mFlags & FLAG_SLOT1) {
            entries[1].bits |= data.mRumbleBits << bit;

            if (entries[1].length < data.mBitsLeft) {
                entries[1].length = data.mBitsLeft;
            }
            if (bit == 0) {
                entries[1].isActive = true;
            }
        }

        if (--data.mBitsLeft == 0) {
            if (data.mFlags & FLAG_INITIALIZE) {
                data.mBitsLeft = data.mLength;
            }
        }
    }

    if (info.combinedLength != 0) {
        struct {
            s32 slot;
            f32 shake;
        } s = {0, 0.1f};
        for (s32 i = 0; i < 4; i++) {
            if (s.shake < info.intensities[i]) {
                s.shake = info.intensities[i];
                s.slot = i;
            }
        }

        f32 shake = 0.f;
        for (s32 i = 0; i < 4; i++) {
            if (i == s.slot) {
                shake += info.intensities[i];
            } else {
                f32 tmp = info.intensities[i];
                shake += tmp * (tmp / s.shake);
            }
        }

        f32 modifier = 0.f;
        if ((info.mBits << 0) & 0x80000000) {
            modifier += 0.6f;
        }
        if ((info.mBits << 1) & 0x80000000) {
            modifier += 0.3f;
        }
        if ((info.mBits << 2) & 0x80000000) {
            modifier += 0.1f;
        }

        if (dScGame_c::getCamera(0)) {
            dScGame_c::getCamera(0)->setScreenShakeIntensity(shake * modifier);
        }
    }

    static char rumble_strings[2][32 + 1];
    for (s32 i = 0; i < 2; i++) {
        if (entries[i].isActive) {
            s32 j = 0;
            for (; j < entries[i].length && j < 32; j++) {
                rumble_strings[i][j] = (entries[i].bits << j) & 0x80000000 ? '*' : '-';
            }
            rumble_strings[i][j] = '\0';

            mPad::g_core[0]->stopRumbleMgr();
            mPad::g_core[0]->startPatternRumble(rumble_strings[i], entries[i].length, false);
        }
    }
}

void dRumble_c::remove() {
    if (spInstance != nullptr) {
        spInstance->mRumbleData[0].mBitsLeft = 0;
        spInstance->mRumbleData[1].mBitsLeft = 0;
        spInstance->mRumbleData[2].mBitsLeft = 0;
        spInstance->mRumbleData[3].mBitsLeft = 0;
        mPad::g_core[0]->stopRumbleMgr();
        mPad::g_core[1]->stopRumbleMgr();

        delete spInstance;
        spInstance = nullptr;
    }
}

s32 dRumble_c::start(const dRumbleEntry_c &entry, u32 flags) {
    static char rumble_string[32 + 1];
    if (dLytControlGame_c::getInstance()) {
        if (!dLytControlGame_c::getInstance()->isStateNormal()) {
            s32 j = 0;
            for (; j < entry.mLength && j < 32; j++) {
                rumble_string[j] = (entry.mBits << j) & 0x80000000 ? '*' : '-';
            }
            rumble_string[j] = '\0';

            mPad::g_core[0]->stopRumbleMgr();
            mPad::g_core[0]->startPatternRumble(rumble_string, entry.mLength, false);
            return -1;
        }
    }
    s32 idx = 0;
    for (s32 i = 0; i < 4; i++) {
        if (spInstance->mRumbleData[i].mBitsLeft == 0) {
            idx = i;
            break;
        }

        s32 bitsLeft = (spInstance->mRumbleData[i].mFlags & FLAG_INITIALIZE) ?
                           (spInstance->mRumbleData[i].mLength * 10) :
                           (spInstance->mRumbleData[i].mBitsLeft);
        if (bitsLeft < 9999) {
            idx = i;
        }
    }

    spInstance->mRumbleData[idx].mLength = entry.mLength;
    spInstance->mRumbleData[idx].mBitsLeft = entry.mLength;
    spInstance->mRumbleData[idx].mRumbleBits = entry.mBits;
    spInstance->mRumbleData[idx].mIntensity = entry.mIntensity;
    spInstance->mRumbleData[idx].mFlags = flags;

    return idx;
}

void dRumble_c::stop(s32 idx) {
    if (spInstance == nullptr) {
        return;
    }

    // -1 means stop all
    if (idx == -1) {
        spInstance->mRumbleData[0].mBitsLeft = 0;
        spInstance->mRumbleData[1].mBitsLeft = 0;
        spInstance->mRumbleData[2].mBitsLeft = 0;
        spInstance->mRumbleData[3].mBitsLeft = 0;
        return;
    }

    if (idx > ARRAY_LENGTH(spInstance->mRumbleData) - 1) {
        return;
    }

    spInstance->mRumbleData[idx].mBitsLeft = 0;
}

dRumbleEntry_c::dRumbleEntry_c(s32 length, u32 bits, f32 intensity) {
    mLength = length;
    mBits = bits;
    mIntensity = intensity;
}

bool dRumbleIdx_c::start(const dRumbleEntry_c &entry, u32 flags) {
    if (isActive()) {
        return false;
    }
    setIdx(dRumble_c::start(entry, flags | dRumble_c::FLAG_INITIALIZE));
    return isActive();
}

void dRumbleIdx_c::stop() {
    if (isActive()) {
        dRumble_c::stop(getIdx());
        setIdx(-1);
    }
}
