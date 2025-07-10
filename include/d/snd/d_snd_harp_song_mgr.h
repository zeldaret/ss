#ifndef D_SND_HARP_SONG_MGR_H
#define D_SND_HARP_SONG_MGR_H

#include "d/snd/d_snd_util.h"

SND_DISPOSER_FORWARD_DECL(dSndHarpSongMgr_c);

class dSndHarpSongMgr_c {
    SND_DISPOSER_MEMBERS(dSndHarpSongMgr_c)

public:
    dSndHarpSongMgr_c();

    void setup();
    void calc();

private:
};

#endif
