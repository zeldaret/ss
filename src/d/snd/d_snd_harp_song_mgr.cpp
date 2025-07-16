#include "d/snd/d_snd_harp_song_mgr.h"

#include "common.h"
#include "d/snd/d_snd_bgm_mml_parsers.h"
#include "d/snd/d_snd_harp_song_data.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_wzsound.h"

SND_DISPOSER_DEFINE(dSndHarpSongMgr_c);

dSndHarpSongMgr_c::dSndHarpSongMgr_c()
    : field_0x010(0),
      field_0x011(0),
      mDataLoaded(false),
      field_0x014(0),
      field_0x018(0),
      field_0x01C(0),
      field_0x020(0),
      field_0x024(0),
      field_0x028(0),
      field_0x02C(0),
      field_0x030(0),
      field_0x034(0),
      field_0x038(0.0f),
      field_0x03C(1.0f),
      field_0x040(0),
      field_0x041(0),
      field_0x042(0),
      field_0x043(0),
      field_0x044(1),
      field_0x048(0),
      field_0x04C(3),
      field_0x050(3),
      field_0x054(3),
      field_0x064(0),
      field_0x068(0),
      field_0x070(0),
      field_0x078(0),
      mpTmpData(nullptr),
      field_0x5AC(0),
      field_0x5B4(0),
      field_0x5B8(0),
      field_0x5BC(0),
      field_0x5C0(4),
      field_0x5C4(4) {
    resetFloatArr1();
    resetFloatArr2();

    field_0x058 = 3;
    field_0x05C = 3;
    field_0x060 = 3;
    mpData = new dSndHarpSongData_c[dSndHarpSongData_c::sNumHarpSongs];
}

void dSndHarpSongMgr_c::setup() {
    // no-op
}

void dSndHarpSongMgr_c::setupState0() {
    if (mDataLoaded) {
        return;
    }

    dSndBgmMmlParserHarpSong_c parser;
    for (int i = 0; i < dSndHarpSongData_c::sNumHarpSongs; i++) {
        mpData[i].init(i);
        u32 soundId = LABEL_HARP_TONE_RAW_START + 1 + i;
        parser.parseData(soundId, mpData + i);
        mpData[i].setSoundId(soundId);
        mpData[i].loadStaticValues();
    }

    mDataLoaded = true;
}

void dSndHarpSongMgr_c::resetFloatArr1() {
    for (int i = 0; i < 300; i++) {
        field_0x07C[i] = 0.0f;
    }
}

void dSndHarpSongMgr_c::resetFloatArr2() {
    for (int i = 0; i < 30; i++) {
        field_0x52C[i] = 0.0f;
    }
}
