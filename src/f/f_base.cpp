#include "f/f_base.h"

#include "m/m_heap.h"
#include "string.h"

// This file will be ported from
// https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/source/dol/framework/f_base.cpp

fBaseID_e fBase_c::m_rootUniqueID = FIRST_ID;

fBase_c::ConstructData fBase_c::m_tmpCtData;

fLiMgBa_c fBase_c::m_forceExecuteList;
int (*fBase_c::sLoadAsyncCallback)();
void (*fBase_c::sUnloadCallback)();

fBase_c::fBase_c()
    : mID(m_rootUniqueID),
      mParams(m_tmpCtData.params),
      mProfileName(m_tmpCtData.prof_name),
      mGroupType(m_tmpCtData.group_type),
      mManager(this) {
    m_rootUniqueID = (fBaseID_e)(m_rootUniqueID + 1);
    if (m_rootUniqueID == INVALID) {
        for (;;) {
            continue;
        }
    }

    fManager_c::m_connectManage.addTreeNode(&mManager.connect_node, m_tmpCtData.connect_parent);
    int searchTableIdx = mManager.getSearchTableNum();
    fManager_c::m_searchManage[searchTableIdx].prepend(&mManager.search_node);

    const fProfile::fBaseProfile_c *profile = (*fProfile::sProfileList)[mProfileName];
    if (profile != nullptr) {
        mManager.execute_node.setOrder(profile->m_execute_order);
        mManager.draw_node.setOrder(profile->m_draw_order);
    }

    fBase_c *parent = getConnectParent();
    if (parent != nullptr) {
        if (parent->checkProcControl(ROOT_DISABLE_EXECUTE) || parent->checkProcControl(DISABLE_EXECUTE)) {
            setProcControl(DISABLE_EXECUTE);
        }
        if (parent->checkProcControl(ROOT_DISABLE_DRAW) || parent->checkProcControl(DISABLE_DRAW)) {
            setProcControl(DISABLE_DRAW);
        }
    }
}

fBase_c::~fBase_c() {
    // Is this static_cast fake? Maybe. But if actor_list is of type fLiMgBa_c,
    // that causes a weak destructor to spawn that actually needs to be in f_manager.
    // And it's not like any of fLiMgBa_c's methods are accessed via actor_list,
    // so who can tell? Maybe there's yet another type?
    fLiNdBa_c *node = static_cast<fLiNdBa_c *>(mActorList.getFirst());
    while (node != nullptr) {
        node->unlink();
        node = static_cast<fLiNdBa_c *>(mActorList.getFirst());
    }
}

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

int fBase_c::create() {
    return SUCCEEDED;
}

int fBase_c::preCreate() {
    return SUCCEEDED;
}

void fBase_c::postCreate(MAIN_STATE_e state) {
    if (state == SUCCESS) {
        fManager_c::m_createManage.remove(&mManager.execute_node);
        if (fManager_c::m_nowLoopProc == fManager_c::EXECUTE) {
            mUpdateRequest = true;
        } else {
            fManager_c::m_executeManage.addNode(&mManager.execute_node);
            fManager_c::m_drawManage.addNode(&mManager.draw_node);
            mLifecycleState = ACTIVE;
        }
    } else if (state == ERROR) {
        deleteRequest();
    }
}

int fBase_c::doDelete() {
    return SUCCEEDED;
}
int fBase_c::createPack() {
    // Returns PACK_RESULT_e
    return commonPack(&fBase_c::create, &fBase_c::preCreate, &fBase_c::postCreate);
}

int fBase_c::preDelete() {
    if (mpHelper != nullptr && !mpHelper->LoadOnlyOne()) {
        return NOT_READY;
    }

    // Not ready for delete if it has children (children need deletion to process first)
    fBase_c *child = getConnectChild();
    if (child != nullptr) {
        return NOT_READY;
    }

    return SUCCEEDED;
}

void fBase_c::postDelete(MAIN_STATE_e state) {
    // If the Actor is ready for deletion, make sure that it is:
    // - Removed from the connection tree
    // - Removed form the search tree
    // - Removed from the execute node from delete list (is being processed now)
    // - Remove the Healp Allocation
    // Helper Delete I guess
    // - Finally Destroy the Object and ppoof it goes
    if (state == SUCCESS) {
        fManager_c::m_connectManage.removeTreeNode(&mManager.connect_node);
        int searchTableIdx = mManager.getSearchTableNum();
        fManager_c::m_searchManage[searchTableIdx].remove(&mManager.search_node);
        fManager_c::m_deleteManage.remove(&mManager.execute_node);

        if (mpHeap != nullptr) {
            mpHeap->destroy();
        }

        if (mpHelper != nullptr) {
            mpHelper->Delete();
        }

        delete this;
    }
}

int fBase_c::deletePack() {
    // Returns PACK_RESULT_e
    return commonPack(&fBase_c::doDelete, &fBase_c::preDelete, &fBase_c::postDelete);
}

int fBase_c::execute() {
    return SUCCEEDED;
}

int fBase_c::preExecute() {
    // Do not execute if it has been flagged for deletion or marked as non-execute
    if (mDeleteRequest || checkProcControl(DISABLE_EXECUTE)) {
        return NOT_READY;
    }

    return SUCCEEDED;
}

void fBase_c::postExecute(MAIN_STATE_e state) {
    // Implemented per Actor
    return;
}

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

int fBase_c::draw() {
    return SUCCEEDED;
}

int fBase_c::preDraw() {
    // Do not draw if it has been flagged for deletion or marked as non-draw
    if (mDeleteRequest || checkProcControl(DISABLE_DRAW)) {
        return NOT_READY;
    }

    return SUCCEEDED;
}

void fBase_c::postDraw(MAIN_STATE_e state) {
    // Per Actor imple
    return;
}

int fBase_c::drawPack() {
    // Returns PACK_RESULT_e
    return commonPack(&fBase_c::draw, &fBase_c::preDraw, &fBase_c::postDraw);
}

void fBase_c::deleteReady() {}

int fBase_c::connectProc() {
    if (mDeleteRequest) {
        mDeleteRequest = false;
        if (mLifecycleState == ACTIVE) {
            if (mUpdateRequest != UPDATE_FORCE) {
                fManager_c::m_executeManage.remove(&mManager.execute_node);
                mUpdateRequest = UPDATING;
            }
            fManager_c::m_drawManage.remove(&mManager.draw_node);
        } else {
            fManager_c::m_createManage.remove(&mManager.execute_node);
        }

        // Add to Deletion list and flag as to be deleted
        fManager_c::m_deleteManage.prepend(&mManager.execute_node);
        mLifecycleState = TO_BE_DELETED;

        // Add all children to be marked as ready to delete
        for (fTrNdBa_c *node = mManager.connect_node.getChild(); node != nullptr; node = node->getBrNext()) {
            node->p_owner->deleteRequest();
        }
    } else {
        fBase_c *conn_parent = getConnectParent();
        if (conn_parent != nullptr) {
            if (conn_parent->checkProcControl(ROOT_DISABLE_EXECUTE) || conn_parent->checkProcControl(DISABLE_EXECUTE)) {
                setProcControl(DISABLE_EXECUTE);
            } else if (checkProcControl(DISABLE_EXECUTE)) {
                unsetProcControl(DISABLE_EXECUTE);
            }

            if (conn_parent->checkProcControl(ROOT_DISABLE_DRAW) || conn_parent->checkProcControl(DISABLE_DRAW)) {
                setProcControl(DISABLE_DRAW);
            } else if (checkProcControl(DISABLE_DRAW)) {
                unsetProcControl(DISABLE_DRAW);
            }
        }

        // If Actor is already Active
        if (mLifecycleState == ACTIVE) {
            // If flaged for update, add to the global execute list
            if (mUpdateRequest == UPDATE_FORCE) {
                fManager_c::m_executeManage.addNode(&mManager.execute_node);
                mUpdateRequest = UPDATING;
            }

            // Change the priority nodes if order has changed
            fLiNdPrio_c *ex = &mManager.execute_node;
            if (ex->isPriorityChange()) {
                fManager_c::m_executeManage.remove(ex);

                fLiNdPrio_c *t = &mManager.execute_node;
                t->updatePriority();
                fManager_c::m_executeManage.addNode(t);
            }

            fLiNdPrio_c *draw = &mManager.draw_node;
            if (draw->isPriorityChange()) {
                fManager_c::m_drawManage.remove(draw);

                fLiNdPrio_c *t = &mManager.draw_node;
                t->updatePriority();
                fManager_c::m_drawManage.addNode(t);
            }

        }
        // If Actor Is Waiting for Creation
        else if (mLifecycleState != TO_BE_DELETED) {
            if (mCreateRequest) {
                mCreateRequest = false;
                fManager_c::m_createManage.append(&mManager.execute_node);
            } else {
                if ((u8)mUpdateRequest != UPDATING) {
                    mUpdateRequest = UPDATING;
                    fManager_c::m_executeManage.addNode(&mManager.execute_node);
                    fManager_c::m_drawManage.addNode(&mManager.draw_node);
                    mLifecycleState = ACTIVE;
                }
            }
        }
    }

    return SUCCEEDED;
}

void fBase_c::deleteRequest() {
    if (!mDeleteRequest && mLifecycleState != TO_BE_DELETED) {
        mDeleteRequest = true;
        deleteReady();
        // Add all children to be marked as ready to delete
        for (fTrNdBa_c *node = mManager.connect_node.getChild(); node != nullptr; node = node->getBrNext()) {
            node->p_owner->deleteRequest();
        }
    }
}

void fBase_c::forceUpdate() {
    // There is probably an inline active here
    if (((!mDeleteRequest && mLifecycleState != TO_BE_DELETED) && mLifecycleState != ACTIVE) &&
        (mUpdateRequest == UPDATE_REQUEST && fManager_c::m_nowLoopProc == fManager_c::EXECUTE)) {
        mUpdateRequest = UPDATE_FORCE;
        fManager_c::m_drawManage.addNode(&mManager.draw_node);
        mLifecycleState = ACTIVE;
        m_forceExecuteList.append(&mManager.execute_node);
    }
}

fBase_c *fBase_c::getConnectRoot() {
    if (fManager_c::m_connectManage.getRoot() != nullptr) {
        return fManager_c::m_connectManage.getRoot()->p_owner;
    }
    return nullptr;
}

fBase_c *fBase_c::getConnectTreeNext(fBase_c *actor) {
    fBase_c *ret = nullptr;

    if (actor == nullptr) {
        ret = getConnectRoot();
    } else {
        fTrNdBa_c *node = actor->mManager.connect_node.getTreeNext();
        if (node != nullptr) {
            ret = node->p_owner;
        }
    }
    return ret;
}

fBase_c *fBase_c::getConnectParent() const {
    fTrNdBa_c *node = mManager.connect_node.getParent();
    if (node != nullptr) {
        return node->p_owner;
    }
    return nullptr;
}

fBase_c *fBase_c::getConnectChild() const {
    fTrNdBa_c *node = mManager.connect_node.getChild();
    if (node != nullptr) {
        return node->p_owner;
    }
    return nullptr;
}

fBase_c *fBase_c::getConnectBrNext() const {
    fTrNdBa_c *node = mManager.connect_node.getBrNext();
    if (node != nullptr) {
        return node->p_owner;
    }
    return nullptr;
}

void fBase_c::updateExecutePriority(u16 order) {
    if (mLifecycleState == ACTIVE && mUpdateRequest != UPDATE_FORCE) {
        if (fManager_c::m_nowLoopProc == fManager_c::EXECUTE) {
            mManager.execute_node.m_new_order = order;
        } else {
            fManager_c::m_executeManage.remove(&mManager.execute_node);
            fLiNdPrio_c *node = &mManager.execute_node;
            node->setOrder(order);
            fManager_c::m_executeManage.addNode(node);
        }
    } else {
        mManager.execute_node.setOrder(order);
    }
}

bool fBase_c::setConnectChild(fBase_c *child) {
    if (child == nullptr || child->mLifecycleState == TO_BE_DELETED) {
        return false;
    }

    if (fManager_c::m_nowLoopProc == fManager_c::CONNECT) {
        return false;
    }

    return fManager_c::m_connectManage.insertTreeNode(&mManager.connect_node, &child->mManager.connect_node);
}

bool fBase_c::entryFrmHeap(size_t size, EGG::Heap *parentHeap) {
    if (mpHeap != nullptr) {
        return true;
    }

    size_t heap_size = 0;
    EGG::FrmHeap *new_heap = nullptr;

    if (size != 0) {
        new_heap = mHeap::createFrmHeapToCurrent(
            size, parentHeap, "各プロセスが個別で持てるヒープ(fBase_c::mHeap)", 0x20, mHeap::OPT_NONE
        );
        if (new_heap != nullptr) {
            bool create_sucess = createHeap();
            mHeap::restoreCurrentHeap();
            if (!create_sucess) {
                mHeap::destroyFrmHeap(new_heap);
                new_heap = nullptr;
            } else {
                heap_size = mHeap::adjustFrmHeap(new_heap);
                if (size == heap_size) {
                    mpHeap = new_heap;
                    return true;
                }
            }
        }
    }
    if (new_heap == nullptr) {
        new_heap = mHeap::createFrmHeapToCurrent(
            -1, parentHeap, "各プロセスが個別で持てるヒープ(fBase_c::mHeap)", 0x20, mHeap::OPT_NONE
        );

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
        EGG::FrmHeap *larger_heap = mHeap::createFrmHeapToCurrent(
            heap_size, parentHeap, "各プロセスが個別で持てるヒープ(fBase_c::mHeap)", 0x20, mHeap::OPT_NONE
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
                    mpHeap = larger_heap;
                    return true;
                }
            } else {
                mHeap::restoreCurrentHeap();
                mHeap::destroyFrmHeap(larger_heap);
            }
        }
    }

    if (new_heap != nullptr) {
        mpHeap = new_heap;
        return true;
    }

    deleteRequest();
    return false;
}

bool fBase_c::entryFrmHeapNonAdjust(size_t size, EGG::Heap *parentHeap) {
    if (mpHeap != nullptr) {
        return true;
    }

    EGG::FrmHeap *new_heap = mHeap::createFrmHeapToCurrent(
        size, parentHeap, "各プロセスが個別で持てるヒープ(fBase_c::mHeap)", 0x20, mHeap::OPT_NONE
    );
    if (new_heap != nullptr) {
        bool create_sucess = createHeap();
        mHeap::restoreCurrentHeap();
        if (!create_sucess) {
            mHeap::destroyFrmHeap(new_heap);
        } else {
            mpHeap = new_heap;
            return true;
        }
    }

    deleteRequest();
    return false;
}

bool fBase_c::createHeap() {
    return true;
}

void *fBase_c::operator new(size_t size) {
    void *mem = EGG::Heap::alloc(size, -4, mHeap::g_gameHeaps[0]);
    if (mem != nullptr) {
        memset(mem, 0, size);
    }
    return mem;
}

void fBase_c::operator delete(void *block) {
    EGG::Heap::free(block, mHeap::g_gameHeaps[0]);
}

void fBase_c::runCreate() {
    createPack();
    if ((!mDeleteRequest && (u8)mUpdateRequest == UPDATING) && mLifecycleState == WAITING_FOR_CREATE) {
        if (fManager_c::m_nowLoopProc == fManager_c::CREATE) {
            mCreateRequest = true;
        } else {
            fManager_c::m_createManage.append(&mManager.execute_node);
        }
    }
}

fBase_c *fBase_c::getChildProcessCreateState() const {
    const fTrNdBa_c *connect_node = &mManager.connect_node;
    fTrNdBa_c *end = connect_node->getTreeNextNotChild();
    fTrNdBa_c *curr = connect_node->getChild();

    while (curr != nullptr && curr != end) {
        if (curr->p_owner->mLifecycleState == WAITING_FOR_CREATE) {
            return curr->p_owner;
        }
        curr = curr->getTreeNext();
    }
    return nullptr;
}

bool fBase_c::checkChildProcessCreateState() const {
    return getChildProcessCreateState() != nullptr;
}

void fBase_c::setTmpCtData(ProfileName profile_name, fTrNdBa_c *connect_parent, u32 param, u8 group_type) {
    m_tmpCtData.prof_name = profile_name;
    m_tmpCtData.connect_parent = connect_parent;
    m_tmpCtData.params = param;
    m_tmpCtData.group_type = group_type;
}

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

fBase_c *fBase_c::createChild(ProfileName profile_name, fBase_c *parent, u32 param, u8 group_type) {
    if (parent == nullptr) {
        return nullptr;
    }

    if (parent->mDeleteRequest || parent->mLifecycleState == TO_BE_DELETED) {
        return nullptr;
    }

    return fBase_make(profile_name, &parent->mManager.connect_node, param, group_type);
}

fBase_c *fBase_c::createRoot(ProfileName profile_name, u32 param, u8 group_type) {
    return fBase_make(profile_name, nullptr, param, group_type);
}
