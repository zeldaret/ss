#include "f/f_base.h"

#include "m/m_heap.h"
#include "string.h"

/*
.text File Range:
    [802e12f0 - 802e2680]
*/

// This file will be ported from
// https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/source/dol/framework/f_base.cpp

fBaseID_e fBase_c::m_rootUniqueID = FIRST_ID;

fBase_c::ConstructData fBase_c::m_tmpCtData;

// 0x80575bb0: sLoadAsyncCallback__7fBase_c
// 0x80575bb4: sUnloadCallback__7fBase_c
fLiMgBa_c fBase_c::m_forceExecuteList;
int (*fBase_c::sLoadAsyncCallback)();
void (*fBase_c::sUnloadCallback)();

/* 802e12f0 */
fBase_c::fBase_c()
    : unique_ID(m_rootUniqueID), params(m_tmpCtData.params), profile_name(m_tmpCtData.prof_name),
      group_type(m_tmpCtData.group_type), manager(this) {
    m_rootUniqueID = (fBaseID_e)(m_rootUniqueID + 1);
    if (m_rootUniqueID == INVALID) {
        for (;;) {
            continue;
        }
    }

    fManager_c::m_connectManage.addTreeNode(&manager.connect_node, m_tmpCtData.connect_parent);
    int searchTableIdx = manager.getSearchTableNum();
    fManager_c::m_searchManage[searchTableIdx].prepend(&manager.search_node);

    const fProfile::fBaseProfile_c *profile = (*fProfile::sProfileList)[profile_name];
    if (profile != nullptr) {
        manager.execute_node.setOrder(profile->m_execute_order);
        manager.draw_node.setOrder(profile->m_draw_order);
    }

    fBase_c *parent = getConnectParent();
    if (parent != nullptr) {
        if (parent->isProcControlFlag(ROOT_DISABLE_EXECUTE) || parent->isProcControlFlag(DISABLE_EXECUTE)) {
            setProcControlFlag(DISABLE_EXECUTE);
        }
        if (parent->isProcControlFlag(ROOT_DISABLE_DRAW) || parent->isProcControlFlag(DISABLE_DRAW)) {
            setProcControlFlag(DISABLE_DRAW);
        }
    }
}

/* 802e1480 */
fBase_c::~fBase_c() {
    // Is this static_cast fake? Maybe. But if actor_list is of type fLiMgBa_c,
    // that causes a weak destructor to spawn that actually needs to be in f_manager.
    // And it's not like any of fLiMgBa_c's methods are accessed via actor_list,
    // so who can tell? Maybe there's yet another type?
    fLiNdBa_c *node = static_cast<fLiNdBa_c*>(actor_list.getFirst());
    while (node != nullptr) {
        node->unlink();
        node = static_cast<fLiNdBa_c*>(actor_list.getFirst());
    }
}

/* 802e1500 */
int fBase_c::commonPack(int (fBase_c::*doFunc)(), int (fBase_c::*preFunc)(), void (fBase_c::*postFunc)(MAIN_STATE_e)) {
    MAIN_STATE_e state;

    // Pre Function to setup
    int result = (this->*preFunc)();
    if (result != NOT_READY) {
        // Do function to handle current
        result = (this->*doFunc)();
        if (result == NOT_READY) {
            state = WAITING;
        } else if (result == SUCCEEDED) {
            state = SUCCESS;
        } else {
            state = ERROR;
        }
    } else {
        state = CANCELED;
    }

    // Post Function to handle after effects (Allows for retry after waiting)
    (this->*postFunc)(state);
    return result;
}

/* 802e15c0 */
int fBase_c::create() {
    return SUCCEEDED;
}

/* 802e15d0 */
int fBase_c::preCreate() {
    return SUCCEEDED;
}

/* 802e15e0 */
void fBase_c::postCreate(MAIN_STATE_e state) {
    if (state == SUCCESS) {
        fManager_c::m_createManage.remove(&manager.execute_node);
        if (fManager_c::m_nowLoopProc == fManager_c::EXECUTE) {
            update_request = true;
        } else {
            fManager_c::m_executeManage.addNode(&manager.execute_node);
            fManager_c::m_drawManage.addNode(&manager.draw_node);
            lifecycle_state = ACTIVE;
        }
    } else if (state == ERROR) {
        deleteRequest();
    }
}

/* 802e1670 */
int fBase_c::doDelete() {
    return SUCCEEDED;
}

/* 802e1680 */
int fBase_c::createPack() {
    // Returns PACK_RESULT_e
    return commonPack(&fBase_c::create, &fBase_c::preCreate, &fBase_c::postCreate);
}

/* 802e1730 */
int fBase_c::preDelete() {
    if (p_helper != nullptr && !p_helper->LoadOnlyOne()) {
        return NOT_READY;
    }

    // Not ready for delete if it has children (children need deletion to process first)
    fBase_c *child = getConnectChild();
    if (child != nullptr) {
        return NOT_READY;
    }

    return SUCCEEDED;
}

/* 802e17a0 */
void fBase_c::postDelete(MAIN_STATE_e state) {
    // If the Actor is ready for deletion, make sure that it is:
    // - Removed from the connection tree
    // - Removed form the search tree
    // - Removed from the execute node from delete list (is being processed now)
    // - Remove the Healp Allocation
    // Helper Delete I guess
    // - Finally Destroy the Object and ppoof it goes
    if (state == SUCCESS) {
        fManager_c::m_connectManage.removeTreeNode(&manager.connect_node);
        int searchTableIdx = manager.getSearchTableNum();
        fManager_c::m_searchManage[searchTableIdx].remove(&manager.search_node);
        fManager_c::m_deleteManage.remove(&manager.execute_node);

        if (p_heap != nullptr) {
            p_heap->destroy();
        }

        if (p_helper != nullptr) {
            p_helper->Delete();
        }

        delete this;
    }
}

/* 802e15d0 */
int fBase_c::deletePack() {
    // Returns PACK_RESULT_e
    return commonPack(&fBase_c::doDelete, &fBase_c::preDelete, &fBase_c::postDelete);
}

/* 802e1910 */
int fBase_c::execute() {
    return SUCCEEDED;
}

/* 802e1920 */
int fBase_c::preExecute() {
    // Do not execute if it has been flagged for deletion or marked as non-execute
    if (delete_request || isProcControlFlag(DISABLE_EXECUTE)) {
        return NOT_READY;
    }

    return SUCCEEDED;
}

/* 802e1950 */
void fBase_c::postExecute(MAIN_STATE_e state) {
    // Implemented per Actor
    return;
}

/* 802e1960 */
int fBase_c::executePack() {
    // Returns PACK_RESULT_e
    int result = commonPack(&fBase_c::execute, &fBase_c::preExecute, &fBase_c::postExecute);

    // Check force update list (bamboo cut to force a cut type of thing)
    fLiNdBa_c *node = m_forceExecuteList.getFirst();
    while (node != nullptr) {
        node->p_owner->commonPack(&fBase_c::execute, &fBase_c::preExecute, &fBase_c::postExecute);
        fLiNdBa_c *tempNode = node->getNext();
        m_forceExecuteList.remove(node);
        node = tempNode;
    }

    return result;
}

/* 802e1a90 */
int fBase_c::draw() {
    return SUCCEEDED;
}

/* 802e1aa0 */
int fBase_c::preDraw() {
    // Do not draw if it has been flagged for deletion or marked as non-draw
    if (delete_request || isProcControlFlag(DISABLE_DRAW)) {
        return NOT_READY;
    }

    return SUCCEEDED;
}

/* 802e1ad0 */
void fBase_c::postDraw(MAIN_STATE_e state) {
    // Per Actor imple
    return;
}

/* 802e1ae0 */
int fBase_c::drawPack() {
    // Returns PACK_RESULT_e
    return commonPack(&fBase_c::draw, &fBase_c::preDraw, &fBase_c::postDraw);
}

/* 802e1b90 */
void fBase_c::deleteReady() {}

/* 802e1ba0 */
int fBase_c::connectProc() {
    if (delete_request) {
        delete_request = false;
        if (lifecycle_state == ACTIVE) {
            if (update_request != UPDATE_FORCE) {
                fManager_c::m_executeManage.remove(&manager.execute_node);
                update_request = UPDATING;
            }
            fManager_c::m_drawManage.remove(&manager.draw_node);
        } else {
            fManager_c::m_createManage.remove(&manager.execute_node);
        }

        // Add to Deletion list and flag as to be deleted
        fManager_c::m_deleteManage.prepend(&manager.execute_node);
        lifecycle_state = TO_BE_DELETED;

        // Add all children to be marked as ready to delete
        for (fTrNdBa_c *node = manager.connect_node.getChild(); node != nullptr; node = node->getBrNext()) {
            node->p_owner->deleteRequest();
        }
    } else {
        fBase_c *conn_parent = getConnectParent();
        if (conn_parent != nullptr) {
            if (conn_parent->isProcControlFlag(ROOT_DISABLE_EXECUTE) ||
                conn_parent->isProcControlFlag(DISABLE_EXECUTE)) {
                setProcControlFlag(DISABLE_EXECUTE);
            } else if (isProcControlFlag(DISABLE_EXECUTE)) {
                clearProcControlFlag(DISABLE_EXECUTE);
            }

            if (conn_parent->isProcControlFlag(ROOT_DISABLE_DRAW) || conn_parent->isProcControlFlag(DISABLE_DRAW)) {
                setProcControlFlag(DISABLE_DRAW);
            } else if (isProcControlFlag(DISABLE_DRAW)) {
                clearProcControlFlag(DISABLE_DRAW);
            }
        }

        // If Actor is already Active
        if (lifecycle_state == ACTIVE) {
            // If flaged for update, add to the global execute list
            if (update_request == UPDATE_FORCE) {
                fManager_c::m_executeManage.addNode(&manager.execute_node);
                update_request = UPDATING;
            }

            // Change the priority nodes if order has changed
            fLiNdPrio_c *ex = &manager.execute_node;
            if (ex->isPriorityChange()) {
                fManager_c::m_executeManage.remove(ex);

                fLiNdPrio_c *t = &manager.execute_node;
                t->updatePriority();
                fManager_c::m_executeManage.addNode(t);
            }

            fLiNdPrio_c *draw = &manager.draw_node;
            if (draw->isPriorityChange()) {
                fManager_c::m_drawManage.remove(draw);

                fLiNdPrio_c *t = &manager.draw_node;
                t->updatePriority();
                fManager_c::m_drawManage.addNode(t);
            }

        }
        // If Actor Is Waiting for Creation
        else if (lifecycle_state != TO_BE_DELETED) {
            if (create_request) {
                create_request = false;
                fManager_c::m_createManage.append(&manager.execute_node);
            } else {
                if ((u8)update_request != UPDATING) {
                    update_request = UPDATING;
                    fManager_c::m_executeManage.addNode(&manager.execute_node);
                    fManager_c::m_drawManage.addNode(&manager.draw_node);
                    lifecycle_state = ACTIVE;
                }
            }
        }
    }

    return SUCCEEDED;
}

/* 802e1e00 */
void fBase_c::deleteRequest() {
    if (!delete_request && lifecycle_state != TO_BE_DELETED) {
        delete_request = true;
        deleteReady();
        // Add all children to be marked as ready to delete
        for (fTrNdBa_c *node = manager.connect_node.getChild(); node != nullptr; node = node->getBrNext()) {
            node->p_owner->deleteRequest();
        }
    }
}

/* 802e1e80 */
void fBase_c::forceUpdate() {
    // There is probably an inline active here
    if (((!delete_request && lifecycle_state != TO_BE_DELETED) && lifecycle_state != ACTIVE) &&
        (update_request == UPDATE_REQUEST && fManager_c::m_nowLoopProc == fManager_c::EXECUTE)) {
        update_request = UPDATE_FORCE;
        fManager_c::m_drawManage.addNode(&manager.draw_node);
        lifecycle_state = ACTIVE;
        m_forceExecuteList.append(&manager.execute_node);
    }
}

/* 802e1f10 */
fBase_c *fBase_c::getConnectRoot() {
    if (fManager_c::m_connectManage.getRoot() != nullptr) {
        return fManager_c::m_connectManage.getRoot()->p_owner;
    }
    return nullptr;
}

/* 802e1f30 */
fBase_c *fBase_c::getConnectTreeNext(fBase_c *actor) {
    fBase_c *ret = nullptr;

    if (actor == nullptr) {
        ret = getConnectRoot();
    } else {
        fTrNdBa_c *node = actor->manager.connect_node.getTreeNext();
        if (node != nullptr) {
            ret = node->p_owner;
        }
    }
    return ret;
}

/* 802e1f90 */
fBase_c *fBase_c::getConnectParent() const {
    fTrNdBa_c *node = manager.connect_node.getParent();
    if (node != nullptr) {
        return node->p_owner;
    }
    return nullptr;
}

/* 802e1fb0 */
fBase_c *fBase_c::getConnectChild() const {
    fTrNdBa_c *node = manager.connect_node.getChild();
    if (node != nullptr) {
        return node->p_owner;
    }
    return nullptr;
}

/* 802e1fd0 */
fBase_c *fBase_c::getConnectBrNext() const {
    fTrNdBa_c *node = manager.connect_node.getBrNext();
    if (node != nullptr) {
        return node->p_owner;
    }
    return nullptr;
}

/* 802e1ff0 */
void fBase_c::updateExecutePriority(u16 order) {
    if (lifecycle_state == ACTIVE && update_request != UPDATE_FORCE) {
        if (fManager_c::m_nowLoopProc == fManager_c::EXECUTE) {
            manager.execute_node.m_new_order = order;
        } else {
            fManager_c::m_executeManage.remove(&manager.execute_node);
            fLiNdPrio_c *node = &manager.execute_node;
            node->setOrder(order);
            fManager_c::m_executeManage.addNode(node);
        }
    } else {
        manager.execute_node.setOrder(order);
    }
}

/* 802e2090 */
bool fBase_c::setConnectChild(fBase_c *child) {
    if (child == nullptr || child->lifecycle_state == TO_BE_DELETED) {
        return false;
    }

    if (fManager_c::m_nowLoopProc == fManager_c::CONNECT) {
        return false;
    }

    return fManager_c::m_connectManage.insertTreeNode(&manager.connect_node, &child->manager.connect_node);
}

/* 802e20e0 */
bool fBase_c::entryFrmHeap(size_t size, EGG::Heap *parentHeap) {
    if (p_heap != nullptr) {
        return true;
    }

    size_t heap_size = 0;
    EGG::FrmHeap *new_heap = nullptr;

    if (size != 0) {
        new_heap =
            mHeap::makeFrmHeapAndUpdate(size, parentHeap, "各プロセスが個別で持てるヒープ(fBase_c::mHeap)", 0x20, 0);
        if (new_heap != nullptr) {
            bool create_sucess = createHeap();
            mHeap::restoreCurrentHeap();
            if (!create_sucess) {
                mHeap::destroyFrmHeap(new_heap);
                new_heap = nullptr;
            } else {
                heap_size = mHeap::adjustFrmHeap(new_heap);
                if (size == heap_size) {
                    p_heap = new_heap;
                    return true;
                }
            }
        }
    }
    if (new_heap == nullptr) {
        new_heap =
            mHeap::makeFrmHeapAndUpdate(-1, parentHeap, "各プロセスが個別で持てるヒープ(fBase_c::mHeap)", 0x20, 0);

        if (new_heap != nullptr) {
            bool create_sucess = createHeap();
            mHeap::restoreCurrentHeap();
            if (!create_sucess) {
                mHeap::destroyFrmHeap(new_heap);
                new_heap = nullptr;
            } else {
                heap_size = mHeap::adjustFrmHeap(new_heap);
            }
        }
    }

    if (new_heap != nullptr) {
        EGG::FrmHeap *larger_heap = mHeap::makeFrmHeapAndUpdate(
            heap_size, parentHeap, "各プロセスが個別で持てるヒープ(fBase_c::mHeap)", 0x20, 0
        );
        if (larger_heap != nullptr) {
            if (larger_heap < new_heap) {
                mHeap::destroyFrmHeap(new_heap);
                new_heap = nullptr;
                bool create_sucess = createHeap();
                mHeap::restoreCurrentHeap();
                if (!create_sucess) {
                    mHeap::destroyFrmHeap(larger_heap);
                    larger_heap = nullptr;
                } else {
                    heap_size = mHeap::adjustFrmHeap(larger_heap);
                    p_heap = larger_heap;
                    return true;
                }
            } else {
                mHeap::restoreCurrentHeap();
                mHeap::destroyFrmHeap(larger_heap);
            }
        }
    }

    if (new_heap != nullptr) {
        p_heap = new_heap;
        return true;
    }

    deleteRequest();
    return false;
}

/* 802e22e0 */
bool fBase_c::entryFrmHeapNonAdjust(size_t size, EGG::Heap *parentHeap) {
    if (p_heap != nullptr) {
        return true;
    }

    EGG::FrmHeap *new_heap =
        mHeap::makeFrmHeapAndUpdate(size, parentHeap, "各プロセスが個別で持てるヒープ(fBase_c::mHeap)", 0x20, 0);
    if (new_heap != nullptr) {
        bool create_sucess = createHeap();
        mHeap::restoreCurrentHeap();
        if (!create_sucess) {
            mHeap::destroyFrmHeap(new_heap);
        } else {
            p_heap = new_heap;
            return true;
        }
    }

    deleteRequest();
    return false;
}

/* 802e23a0 */
bool fBase_c::createHeap() {
    return true;
}

/* 802e23b0 */
void *fBase_c::operator new(size_t size) {
    void *mem = EGG::Heap::alloc(size, -4, mHeap::g_gameHeaps[0]);
    if (mem != nullptr) {
        memset(mem, 0, size);
    }
    return mem;
}

/* 802e2410 */
void fBase_c::operator delete(void *block) {
    EGG::Heap::free(block, mHeap::g_gameHeaps[0]);
}

/* 802e2420 */
void fBase_c::runCreate() {
    createPack();
    if ((!delete_request && (u8)update_request == UPDATING) && lifecycle_state == WAITING_FOR_CREATE) {
        if (fManager_c::m_nowLoopProc == fManager_c::CREATE) {
            create_request = true;
        } else {
            fManager_c::m_createManage.append(&manager.execute_node);
        }
    }
}

/* 802e24a0 */
fBase_c *fBase_c::getChildProcessCreateState() const {
    const fTrNdBa_c *connect_node = &manager.connect_node;
    fTrNdBa_c *end = connect_node->getTreeNextNotChild();
    fTrNdBa_c *curr = connect_node->getChild();

    while (curr != nullptr && curr != end) {
        if (curr->p_owner->lifecycle_state == WAITING_FOR_CREATE) {
            return curr->p_owner;
        }
        curr = curr->getTreeNext();
    }
    return nullptr;
}

/* 802e2510 */
bool fBase_c::checkChildProcessCreateState() const {
    return getChildProcessCreateState() != nullptr;
}

/* 802e2540 */
void fBase_c::setTmpCtData(ProfileName profile_name, fTrNdBa_c *connect_parent, u32 param, u8 group_type) {
    m_tmpCtData.prof_name = profile_name;
    m_tmpCtData.connect_parent = connect_parent;
    m_tmpCtData.params = param;
    m_tmpCtData.group_type = group_type;
}

/* 802e2560 */
fBase_c *fBase_c::fBase_make(ProfileName profile_name, fTrNdBa_c *connect_parent, u32 param, u8 group_type) {
    if ((*fProfile::sProfileList)[profile_name] == nullptr) {
        return nullptr;
    }
    if (fManager_c::m_nowLoopProc == fManager_c::DRAW) {
        return nullptr;
    }
    setTmpCtData(profile_name, connect_parent, param, group_type);
    fBase_c *actor = (fBase_c *)((*fProfile::sProfileList)[profile_name]->mpClassInit)();
    setTmpCtData(0, nullptr, 0, 0);

    if (actor != nullptr) {
        actor->runCreate();
    }

    return actor;
}

/* 802e2600 */
fBase_c *fBase_c::createChild(ProfileName profile_name, fBase_c *parent, u32 param, u8 group_type) {
    if (parent == nullptr) {
        return nullptr;
    }

    if (parent->delete_request || parent->lifecycle_state == TO_BE_DELETED) {
        return nullptr;
    }

    return fBase_make(profile_name, &parent->manager.connect_node, param, group_type);
}

/* 802e2640 */
fBase_c *fBase_c::createRoot(ProfileName profile_name, u32 param, u8 group_type) {
    return fBase_make(profile_name, nullptr, param, group_type);
}
