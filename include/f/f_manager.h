#pragma once

// this file was ported from https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/framework/f_manager.hpp

#include <f/f_base_id.h>
#include <f/f_list_mg_ptmf.h>
#include <f/f_list_mg.h>
#include <f/f_list_nd.h>
#include <f/f_list_nd_prio.h>
#include <f/f_tree_mg_ptmf.h>
#include <f/f_tree_nd.h>
#include <f/f_profile.h>

#define GET_PROC_FLAG(proc) (1 << (proc - 1))

class fBase_c;

/// @brief A class that manages the execution of the bases.
class fManager_c {
public:

    /// @brief The processes for fManager_c.
    enum LOOP_PROC_e {
        NOTHING, CONNECT, CREATE, EXECUTE, DELETE, DRAW
    };

    /// @brief Flags for different processes, induced from ::LOOP_PROC_e.
    enum PROC_FLAGS {
        PROC_FLAG_CONNECT = GET_PROC_FLAG(CONNECT),
        PROC_FLAG_CREATE = GET_PROC_FLAG(CREATE),
        PROC_FLAG_EXECUTE = GET_PROC_FLAG(EXECUTE),
        PROC_FLAG_DELETE = GET_PROC_FLAG(DELETE),
        PROC_FLAG_DRAW = GET_PROC_FLAG(DRAW)
    };

    /// @brief Constructs a new manager.
    /// @param owner The manager's owner.
    fManager_c(fBase_c *owner) :
        mConnectNode(owner),
        mExecuteNode(owner),
        mDrawNode(owner),
        mSearchNode(owner) {}

    /// @brief Gets the index of the search list corresponding to the owner of the manager. See ::m_searchManage.
    int getSearchTableNum();

    /// @brief Executes the currently enabled processes on all the bases in the respective lists.
    static void mainLoop();

    /// @brief Searches for a base with the given ID.
    static fBase_c *searchBaseByID(fBaseID_e id);

    /// @brief Searches for a base with a given profile name, optionally under a given parent.
    static fBase_c *searchBaseByProfName(ProfileName profID, const fBase_c *parent);

    /// @brief Searches for a base with a given group type, optionally under a given parent.
    static fBase_c *searchBaseByGroupType(unsigned char groupType, const fBase_c *parent);

private:
    fTrNdBa_c mConnectNode; ///< The node in ::m_connectManage.
    fLiNdPrio_c mExecuteNode; ///< The node in ::m_executeManage.
    fLiNdPrio_c mDrawNode; ///< The node in ::m_drawManage.
    fLiNdBa_c mSearchNode; ///< The node in ::m_searchManage.

    static fTrMgPTMF_c m_connectManage; ///< A tree that connects all loaded bases.
    static fLiMgPTMF_c m_createManage; ///< A list of all the bases scheduled for creation.
    static fLiMgPTMF_c m_executeManage; ///< A list of all the bases scheduled for execution.
    static fLiMgPTMF_c m_deleteManage; ///< A list of all the bases scheduled for deletion.
    static fLiMgPTMF_c m_drawManage; ///< A list of all the bases scheduled for drawing.

    /// @brief An array of lists that is used to search for bases.
    /// @details Bases are distributed evenly between each entry in the array.
    /// This allows for more efficient base lookups.
    static fLiMgBa_c m_searchManage[8];

    static u32 m_StopProcInf; ///< Which processes should be executed this frame.
    static LOOP_PROC_e m_nowLoopProc; ///< The process ::mainLoop is currently in.

    friend class fBase_c;
};