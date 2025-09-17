#include "f/f_manager.h"

#include "f/f_base.h"

fTrMgPTMF_c fManager_c::m_connectManage(&fBase_c::connectProc);
fLiMgPTMF_c fManager_c::m_createManage(&fBase_c::createPack);
fLiMgPTMF_c fManager_c::m_executeManage(&fBase_c::executePack);
fLiMgPTMF_c fManager_c::m_drawManage(&fBase_c::drawPack);
fLiMgPTMF_c fManager_c::m_deleteManage(&fBase_c::deletePack);

fLiMgBa_c fManager_c::m_searchManage[8];

u32 fManager_c::m_StopProcInf;
fManager_c::LOOP_PROC_e fManager_c::m_nowLoopProc = CONNECT;

/* 802e28c0 */
int fManager_c::getSearchTableNum() {
    return reinterpret_cast<fBase_c *>(search_node.p_owner)->mID & 7;
}

/* 802e28d0 */
fBase_c *fManager_c::searchBaseByID(fBaseID_e id) {
    const fLiNdBa_c *res = m_searchManage[id & 7].searchNodeByID(id);
    if (res != nullptr) {
        return res->p_owner;
    }
    return nullptr;
}

/* 802e2920 */
fBase_c *fManager_c::searchBaseByProfName(ProfileName profID, const fBase_c *parent) {
    const fTrNdBa_c *connect_node = parent != nullptr ? &parent->manager.connect_node : nullptr;

    const fTrNdBa_c *node = m_connectManage.searchNodeByProfName(profID, connect_node);

    if (node != nullptr) {
        return node->p_owner;
    }

    return nullptr;
}

/* 802e2980 */
fBase_c *fManager_c::searchBaseByGroupType(unsigned char groupType, const fBase_c *parent) {
    const fTrNdBa_c *connect_node = parent != nullptr ? &parent->manager.connect_node : nullptr;

    const fTrNdBa_c *node = m_connectManage.searchNodeByGroupType(groupType, connect_node);

    if (node != nullptr) {
        return node->p_owner;
    }

    return nullptr;
}

/* 802e29e0 */
void fManager_c::mainLoop() {
    if ((m_StopProcInf & PROC_FLAG_DELETE) == 0) {
        m_nowLoopProc = DELETE;
        m_deleteManage.walkPack();
    }

    if ((m_StopProcInf & PROC_FLAG_CREATE) == 0) {
        m_nowLoopProc = CREATE;
        m_createManage.walkPack();
    }

    if ((m_StopProcInf & PROC_FLAG_EXECUTE) == 0) {
        m_nowLoopProc = EXECUTE;
        m_executeManage.walkPack();
    }

    if ((m_StopProcInf & PROC_FLAG_DRAW) == 0) {
        m_nowLoopProc = DRAW;
        m_drawManage.walkPack();
    }

    if ((m_StopProcInf & PROC_FLAG_CONNECT) == 0) {
        m_nowLoopProc = CONNECT;
        m_connectManage.walkPack();
    }

    m_nowLoopProc = NOTHING;
}
