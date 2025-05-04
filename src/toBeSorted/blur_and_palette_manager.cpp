#include "toBeSorted/blur_and_palette_manager.h"

#include "m/m_color.h"

BlurAndPaletteManager BlurAndPaletteManager::sInstance;
BlurAndPaletteManager *BlurAndPaletteManager::sPInstance;

TList<UnkBlurPaletteListNode, offsetof(UnkBlurPaletteListNode, mNode)> sPlayingEffectsList;

mColor BlurAndPaletteManager::combineColors(const mColor &c1, const mColor &c2, f32 ratio) {
    mColor result;
    result.Set(0xff, 0xff, 0xff, 0xff);
    result.r = (c1.r + ratio * ((f32)c2.r - (f32)c1.r));
    result.g = (c1.g + ratio * ((f32)c2.g - (f32)c1.g));
    result.b = (c1.b + ratio * ((f32)c2.b - (f32)c1.b));
    result.a = (c1.a + ratio * ((f32)c2.a - (f32)c1.a));
    return result;
}

Spf::Spf() {}

Sff::Sff() {}

#pragma dont_inline on
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
    f32 f2 = 10000.0f;
    mVec3_c v2(0.0f, 0.0f, 0.0f);
    mVec3_c v3(1.0f, 1.0f, 1.0f);
    mColor white5_ffffff = mColor(0xff, 0xff, 0xff, 0xff);
    mColor pastel_yellow_fffb04 = mColor(0xff, 0xf0, 0xb4, 0xff);
    mColor grey_3c3c3c = mColor(0x3c, 0x3c, 0x3c, 0xff);
    mColor grey_a0a091 = mColor(0xa0, 0xa0, 0x91, 0xff);
    mColor magenta_7d0091 = mColor(0x7d, 0x00, 0x91, 0xff);
    mColor sand_ffebb4 = mColor(0xff, 0xeb, 0xb4, 0xff);

    sPInstance = this;

    currentSpf.grey_aaaaaa = grey_aaaaaa;
    currentSpf.white_ffffff = white_ffffff;
    currentSpf.grey_808080 = grey_808080;
    currentSpf.cream_ffffc0 = cream_ffffc0;
    currentSpf.grey2_aaaaaa = grey_aaaaaa;
    currentSpf.white2_ffffff = white_ffffff;
    currentSpf.field_0x18 = 0.6f;
    currentSpf.pastel_orange_ffb787 = pastel_orange_ffb787;
    currentSpf.field_0x24 = 195.0f;
    currentSpf.field_0x20 = 0.5f;
    currentSpf.black_000000 = black_000000;
    currentSpf.black2_000000 = black_000000;
    currentSpf.mSub.grey_787878 = grey_787878;
    currentSpf.mSub.white3_ffffff.field_0x00 = white2_ffffff;
    currentSpf.mSub.field_0x014.field_0x00 = grey_7f7f7f;
    currentSpf.mSub.field_0x060.field_0x00 = grey_7f7f7f;
    currentSpf.mSub.field_0x0AC.field_0x00 = grey_7f7f7f;
    currentSpf.mSub.field_0x0F8.field_0x00 = grey_7f7f7f;
    currentSpf.mCloudColor = yellow_ffd86b;
    currentSpf.mSkyColor = sea_blue_29424e;
    currentSpf.seafoam_green_b9d8b7 = seafoam_green_b9d8b7;
    currentSpf.white3_ffffff = white3_ffffff;
    currentSpf.field_0x18C = v2;
    currentSpf.field_0x198 = v3;
    currentSpf.mSub.field_0x000 = 0.0625f;
    currentSpf.mSub.field_0x05C = 0.0625f;
    currentSpf.mSub.field_0x0A8 = 0.0625f;
    currentSpf.mSub.field_0x0F4 = 0.0625f;

    currentSpf.grey_787878 = grey_787878;
    currentSpf.grey2_787878 = grey_787878;
    currentSpf.grey3_787878 = grey_787878;
    currentSpf.MAO5_kColor3.field_0x00 = white4_ffffff;
    currentSpf.MAO5_kColor3.field_0x04 = f1;
    currentSpf.MAO5_kColor3.field_0x08 = f2;
    currentSpf.mSub.white3_ffffff.field_0x04 = 45.0f;
    currentSpf.mSub.white3_ffffff.field_0x08 = 180.0f;

    for (int i = 0; i < 5; i++) {
        currentSpf.mSub.field_0x020[i] = black_000000;
        field_0x2E08[i] = v1;
        currentSpf.mSub.field_0x014.field_0x04[i] = 0;
        currentSpf.mSub.field_0x034[i] = 0.0f;
        currentSpf.mSub.field_0x048[i] = 0.0f;

        currentSpf.mSub.field_0x06C[i] = black_000000;
        field_0x2E44[i] = v1;
        currentSpf.mSub.field_0x060.field_0x04[i] = 0;
        currentSpf.mSub.field_0x080[i] = 0.0f;
        currentSpf.mSub.field_0x094[i] = 0.0f;

        currentSpf.mSub.field_0x0B8[i] = black_000000;
        field_0x2E80[i] = v1;
        currentSpf.mSub.field_0x0AC.field_0x04[i] = 0;
        currentSpf.mSub.field_0x0CC[i] = 0.0f;
        currentSpf.mSub.field_0x0E0[i] = 0.0f;

        currentSpf.mSub.field_0x104[i] = black_000000;
        field_0x2EBC[i] = v1;
        currentSpf.mSub.field_0x0F8.field_0x04[i] = 0;
        currentSpf.mSub.field_0x118[i] = 0.0f;
        currentSpf.mSub.field_0x12C[i] = 0.0f;
    }

    currentSpf.field_0x1B0.x = 0.55f;
    currentSpf.field_0x1B0.y = 1.0f;
    currentSpf.field_0x1B0.z = 0.0f;
    currentSpf.field_0x1BC = 0;
    currentSpf.field_0x1C0 = 1.0f;
    currentSpf.field_0x1C4 = 0.5f;
    currentSpf.field_0x1C8 = 0;
    currentSpf.field_0x1CC = 0.5f;
    currentSpf.field_0x1D0 = 0.0f;
    currentSpf.field_0x1D4 = 0;
    currentSpf.field_0x1D5 = pastel_yellow_fffb04;
    currentSpf.field_0x1D9 = 1;
    currentSpf.field_0x1DC = 3300.0f;
    currentSpf.field_0x1E0 = 3300.0f;
    currentSpf.field_0x1E4 = 1000.0f;
    currentSpf.field_0x1E8 = 500.0f;
    currentSpf.field_0x1EC = 0xFF;
    currentSpf.field_0x1ED = 1;
    currentSpf.field_0x1EE = 0;
    currentSpf.field_0x1F0 = 0.0f;
    currentSpf.field_0x1F4 = 0.0f;
    currentSpf.field_0x1F8 = 6.93359f;
    currentSpf.field_0x1FC = 8.0f;
    currentSpf.black3_000000 = black_000000;
    currentSpf.black4_000000 = black_000000;

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
#pragma dont_inline reset
