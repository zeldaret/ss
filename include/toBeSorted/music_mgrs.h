#ifndef MUSIC_MGRS_H
#define MUSIC_MGRS_H

#include "common.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_source_mgr.h"

class dSndPlayerMgr_c;
class dSndSound_c;

// A lot of these names are and were guesses that are probably wrong in a lot of ways.

#define BGM_MGR (dSndPlayerMgr_c::GetInstance())
extern "C" void fn_8035E000();
extern "C" void fn_8035E860(dSndPlayerMgr_c *);
extern "C" void fn_8035E310(dSndPlayerMgr_c *);
extern "C" void fn_8035E820(dSndPlayerMgr_c *);
extern "C" void fn_8035E880(dSndPlayerMgr_c *);
extern "C" void fn_8035E790(dSndPlayerMgr_c *, UNKWORD, void *);

extern "C" void *ENEMY_SOUND_MGR;
extern "C" void fn_80362150(void *, u16);
extern "C" void fn_803625F0(void *, u16);
extern "C" void fn_803618F0(void *);
extern "C" void fn_80362730(void *);
extern "C" void fn_80365D20(void *);
extern "C" void fn_803624F0(void *);
extern "C" void fn_80364FD0(void *, s32);
extern "C" bool fn_80364DA0(void *);
extern "C" void fn_80365020(void *);
extern "C" void fn_80364D00(void *, s32);

#define ENEMY_BGM_RELATED_MGR (dSndSourceMgr_c::GetInstance())
extern "C" void fn_80384570(dSndSourceMgr_c *, bool);

// This one has a SoundActor that actually plays the BGM_ music tracks,
// including the battle BGM track.
extern "C" void *FANFARE_SOUND_MGR;
extern "C" void AnotherSoundMgr__playSound(void *, s32);
extern "C" bool fn_803721F0(void *, u32);
extern "C" bool fn_803720E0(void *, u32);
extern "C" bool fn_80372070(void *, u32);
extern "C" bool fn_803734C0(void *, u32);
extern "C" bool fn_80373550(void *, u32);
extern "C" void fn_80372920(void *);
extern "C" void fn_803738B0(void *, dSndSound_c *sound);
extern "C" void fn_80373900(void *, dSndSound_c *sound);
extern "C" bool fn_803733B0(void*, u32);

#endif
