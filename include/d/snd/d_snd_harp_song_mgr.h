#ifndef D_SND_HARP_SONG_MGR_H
#define D_SND_HARP_SONG_MGR_H

#include "common.h"
#include "d/snd/d_snd_types.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/snd/snd_SoundHandle.h"

SND_DISPOSER_FORWARD_DECL(dSndHarpSongMgr_c);

class dSndHarpSongMgr_c {
    SND_DISPOSER_MEMBERS(dSndHarpSongMgr_c)

public:
    dSndHarpSongMgr_c();

    void setup();
    void setupState0();

    void calc();

    void activate();
    void deactivate();
    bool isPlayingHarpRelated();

    nw4r::snd::SoundHandle &getFiSingHandle() {
        return mFiSingHandle;
    }

private:
    void shiftFloat1(f32 val);
    void shiftFloat2(f32 val);
    void resetFloatArr1();
    void resetFloatArr2();
    bool isContinuousStrumming();
    bool fn_80381150();

    /* 0x010 */ u8 field_0x010;
    /* 0x011 */ u8 field_0x011;
    /* 0x012 */ bool mDataLoaded;
    /* 0x014 */ UNKWORD field_0x014;
    /* 0x018 */ UNKWORD field_0x018;
    /* 0x01C */ UNKWORD field_0x01C;
    /* 0x020 */ UNKWORD field_0x020;
    /* 0x024 */ UNKWORD field_0x024;
    /* 0x028 */ UNKWORD field_0x028;
    /* 0x02C */ UNKWORD field_0x02C;
    /* 0x030 */ UNKWORD field_0x030;
    /* 0x034 */ UNKWORD field_0x034;
    /* 0x038 */ f32 field_0x038;
    /* 0x03C */ f32 field_0x03C;
    /* 0x040 */ u8 field_0x040;
    /* 0x041 */ u8 field_0x041;
    /* 0x042 */ bool field_0x042;
    /* 0x043 */ u8 field_0x043;
    /* 0x044 */ u8 field_0x044;
    /* 0x048 */ UNKWORD field_0x048;
    /* 0x04C */ UNKWORD field_0x04C;
    /* 0x050 */ UNKWORD field_0x050;
    /* 0x054 */ UNKWORD field_0x054;
    /* 0x058 */ UNKWORD field_0x058;
    /* 0x05C */ UNKWORD field_0x05C;
    /* 0x060 */ UNKWORD field_0x060;
    /* 0x064 */ UNKWORD field_0x064;
    /* 0x068 */ UNKWORD field_0x068;
    /* 0x06C */ UNKWORD field_0x06C;
    /* 0x070 */ f32 field_0x070;
    /* 0x074 */ u8 _0x074[0x078 - 0x074];
    /* 0x078 */ u8 field_0x078;
    /* 0x07C */ f32 field_0x07C[300];
    /* 0x52C */ f32 field_0x52C[30];

    /* 0x5A4 */ dSndHarpSongData_c *mpData;
    /* 0x5A8 */ dSndHarpSongData_c *mpTmpData;
    /* 0x5AC */ UNKWORD field_0x5AC;
    /* 0x5B0 */ nw4r::snd::SoundHandle mFiSingHandle;
    /* 0x5B4 */ UNKWORD field_0x5B4;
    /* 0x5B8 */ UNKWORD field_0x5B8;
    /* 0x5BC */ u8 field_0x5BC;
    /* 0x5C0 */ UNKWORD field_0x5C0;
    /* 0x5C4 */ UNKWORD field_0x5C4;
};

#endif
