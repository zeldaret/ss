#ifndef D_LYT_SEEKER_STONE_H
#define D_LYT_SEEKER_STONE_H

#include <d/d_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dLytSeekerStone_c : public dBase_c { 
public:
	dLytSeekerStone_c();
	virtual ~dLytSeekerStone_c();

	STATE_FUNC_DECLARE(dLytSeekerStone_c, None);
	STATE_FUNC_DECLARE(dLytSeekerStone_c, In);
	STATE_FUNC_DECLARE(dLytSeekerStone_c, Move);
	STATE_FUNC_DECLARE(dLytSeekerStone_c, Decide);
	STATE_FUNC_DECLARE(dLytSeekerStone_c, SelectScroll);
	STATE_FUNC_DECLARE(dLytSeekerStone_c, Scroll);
	STATE_FUNC_DECLARE(dLytSeekerStone_c, FadeOut);
	STATE_FUNC_DECLARE(dLytSeekerStone_c, FadeIn);
	STATE_FUNC_DECLARE(dLytSeekerStone_c, Movie);
	STATE_FUNC_DECLARE(dLytSeekerStone_c, MovieFadeOut);
	STATE_FUNC_DECLARE(dLytSeekerStone_c, MovieFadeIn);
	STATE_FUNC_DECLARE(dLytSeekerStone_c, Out);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dLytSeekerStone_c);
};

#endif
