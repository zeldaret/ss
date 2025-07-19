#ifndef MUSIC_MGRS_H
#define MUSIC_MGRS_H

#include "common.h"
#include "d/snd/d_snd_player_mgr.h"

// A lot of these names are and were guesses that are probably wrong in a lot of ways.

#define BGM_MGR (dSndPlayerMgr_c::GetInstance())
extern "C" void fn_8035E000();
extern "C" void fn_8035E860(dSndPlayerMgr_c *);
extern "C" void fn_8035E820(dSndPlayerMgr_c *);
extern "C" void fn_8035E880(dSndPlayerMgr_c *);
extern "C" void fn_8035E310(dSndPlayerMgr_c *);

#endif
