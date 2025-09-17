#ifndef F_MANAGER_H
#define F_MANAGER_H

// this file was ported from
// https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/framework/f_manager.hpp

#include "f/f_base_id.h"
#include "f/f_list_mg.h"
#include "f/f_list_mg_ptmf.h"
#include "f/f_list_nd.h"
#include "f/f_list_nd_prio.h"
#include "f/f_profile.h"
#include "f/f_tree_mg_ptmf.h"
#include "f/f_tree_nd.h"

#define GET_PROC_FLAG(proc) (1 << (proc - 1))

class fBase_c;

class fManager_c {
public:
    enum LOOP_PROC_e {
        NOTHING,
        CONNECT,
        CREATE,
        EXECUTE,
        DELETE,
        DRAW
    };

    enum PROC_FLAGS {
        PROC_FLAG_CONNECT = GET_PROC_FLAG(CONNECT),
        PROC_FLAG_CREATE = GET_PROC_FLAG(CREATE),
        PROC_FLAG_EXECUTE = GET_PROC_FLAG(EXECUTE),
        PROC_FLAG_DELETE = GET_PROC_FLAG(DELETE),
        PROC_FLAG_DRAW = GET_PROC_FLAG(DRAW)
    };
    fManager_c(fBase_c *owner) : connect_node(owner), execute_node(owner), draw_node(owner), search_node(owner) {}
    ~fManager_c() {}
    int getSearchTableNum();
    static fBase_c *searchBaseByID(fBaseID_e id);
    static fBase_c *searchBaseByProfName(ProfileName profID, const fBase_c *parent);
    static fBase_c *searchBaseByGroupType(u8 groupType, const fBase_c *parent);
    static void mainLoop();

private:
    fTrNdBa_c connect_node;   ///< The node in ::m_connectManage.
    fLiNdPrio_c execute_node; ///< The node in ::m_executeManage.
    fLiNdPrio_c draw_node;    ///< The node in ::m_drawManage.
    fLiNdBa_c search_node;    ///< The node in ::m_searchManage.

    static fTrMgPTMF_c m_connectManage; ///< A tree that connects all loaded bases.
    static fLiMgPTMF_c m_createManage;  ///< A list of all the bases scheduled for creation.
    static fLiMgPTMF_c m_executeManage; ///< A list of all the bases scheduled for execution.
    static fLiMgPTMF_c m_drawManage;    ///< A list of all the bases scheduled for drawing.
    static fLiMgPTMF_c m_deleteManage;  ///< A list of all the bases scheduled for deletion.

    static fLiMgBa_c m_searchManage[8];

    static u32 m_StopProcInf;         ///< Which processes should be executed this frame.
    static LOOP_PROC_e m_nowLoopProc; ///< The process ::mainLoop is currently in.

    friend class fBase_c;
};

#endif
