#ifndef D_TITLE_MANAGER_H
#define D_TITLE_MANAGER_H

#include <d/d_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dTitleMgr_c : public dBase_c { 
public:
	dTitleMgr_c();
	virtual ~dTitleMgr_c();

	STATE_FUNC_DECLARE(dTitleMgr_c, Init);
	STATE_FUNC_DECLARE(dTitleMgr_c, OpeningDemo);
	STATE_FUNC_DECLARE(dTitleMgr_c, CheckSave);
	STATE_FUNC_DECLARE(dTitleMgr_c, Title);
	STATE_FUNC_DECLARE(dTitleMgr_c, FileSelect);
	STATE_FUNC_DECLARE(dTitleMgr_c, Input);

private:
	/* 0x??? */ STATE_MGR_DECLARE(dTitleMgr_c);
};
#endif