#ifndef MUSIC_MGRS_H
#define MUSIC_MGRS_H

#include "common.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_source_mgr.h"
#include "d/snd/d_snd_state_mgr.h"

class dSndSound_c;

// A lot of these names are and were guesses that are probably wrong in a lot of ways.

#define BGM_MGR (dSndPlayerMgr_c::GetInstance())
extern "C" void fn_8035E000();
extern "C" void fn_8035E860(dSndPlayerMgr_c *);
extern "C" void fn_8035E820(dSndPlayerMgr_c *);
extern "C" void fn_8035E880(dSndPlayerMgr_c *);
extern "C" void fn_8035E790(dSndPlayerMgr_c *, UNKWORD, void *);

#define ENEMY_SOUND_MGR (dSndStateMgr_c::GetInstance())
extern "C" void fn_80362150(dSndStateMgr_c *, u16);
extern "C" void fn_803625F0(dSndStateMgr_c *, u16);
extern "C" void fn_803618F0(dSndStateMgr_c *);
extern "C" void fn_80362730(dSndStateMgr_c *);
extern "C" void fn_80365D20(dSndStateMgr_c *);
extern "C" void fn_803624F0(dSndStateMgr_c *);
extern "C" void fn_80364FD0(dSndStateMgr_c *, s32);
extern "C" bool fn_80364DA0(dSndStateMgr_c *);
extern "C" void fn_80365020(dSndStateMgr_c *);
extern "C" void fn_80364D00(dSndStateMgr_c *, s32);

#define ENEMY_BGM_RELATED_MGR (dSndSourceMgr_c::GetInstance())
extern "C" void fn_80384570(dSndSourceMgr_c *, bool);

// This one has a SoundActor that actually plays the BGM_ music tracks,
// including the battle BGM track.
#define FANFARE_SOUND_MGR (dSndBgmMgr_c::GetInstance())
extern "C" void fn_80372920(dSndBgmMgr_c *);

#endif
