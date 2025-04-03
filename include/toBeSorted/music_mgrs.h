#ifndef MUSIC_MGRS_H
#define MUSIC_MGRS_H

#include "common.h"

extern "C" void *BGM_MGR;
extern "C" void fn_8035E000();
extern "C" void fn_8035E860(void *);
extern "C" void fn_8035E310(void *);
extern "C" void fn_8035E880(void *);

extern "C" void *ENEMY_SOUND_MGR;
extern "C" void fn_80362150(void *, u16);
extern "C" void fn_803625F0(void *, u16);
extern "C" void fn_803618F0(void *);
extern "C" void fn_80362730(void *);
extern "C" void fn_80365D20(void *);
extern "C" void fn_803624F0(void *);

extern "C" void *ENEMY_BGM_RELATED_MGR;
extern "C" void fn_80384570(void *, bool);

#endif
