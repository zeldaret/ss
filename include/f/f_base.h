#ifndef F_BASE_H
#define F_BASE_H

// This file is adapted from
// https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/framework/f_base.hpp and the Skyward Sword
// Ghidra database. Comments and docs can be seen above. stripped in this file for easier looking

#include "c/c_list.h"
#include "common.h"
#include "egg/core/eggFrmHeap.h"
#include "f/f_base_id.h"
#include "f/f_helper_unk.h"
#include "f/f_list_mg.h"
#include "f/f_manager.h"
#include "f/f_profile.h"

// Ghidra: fBase
//   size: 0x64
// official name
class fBase_c {
public:
    /* 0x00 */ fBaseID_e mID;
    /* 0x04 */ u32 mParams;
    /* 0x08 */ ProfileName mProfileName;
    /* 0x0A */ u8 mLifecycleState;
    /* 0x0B */ bool mDeleteRequest;
    /* 0x0C */ s8 mUpdateRequest;
    /* 0x0D */ bool mCreateRequest;
    /* 0x0E */ u8 mGroupType;
    /* 0x0F */ u8 mProcControl;
    /* 0x10 */ fManager_c mManager;
    /* 0x50 */ fBaHelper_c *mpHelper;
    /* 0x54 */ cListMg_c mActorList;
    /* 0x5C */ EGG::FrmHeap *mpHeap;
    /* 0x60 */ // vtable
public:
    enum UPDATE_STATUS_e {
        UPDATING,
        UPDATE_REQUEST,
        UPDATE_FORCE,
    };
    enum LIFECYCLE_e {
        WAITING_FOR_CREATE,
        ACTIVE,
        TO_BE_DELETED,
    };
    enum GROUP_TYPE_e {
        OTHER,
        SCENE,
        ACTOR,
        STAGE,
    };
    enum MAIN_STATE_e {
        CANCELED,
        ERROR,
        SUCCESS,
        WAITING
    };
    enum PACK_RESULT_e {
        NOT_READY,
        SUCCEEDED,
        FAILED,
    };
    enum PROC_DISABLE_e {
        ROOT_DISABLE_EXECUTE = 1,
        DISABLE_EXECUTE = 2,
        ROOT_DISABLE_DRAW = 4,
        DISABLE_DRAW = 8
    };

    int getID() const {
        return mID;
    }

    bool checkProcControl(u8 procControl) const {
        return (mProcControl & procControl) != 0;
    }
    void setProcControl(u8 procControl) {
        mProcControl |= procControl;
    }
    void unsetProcControl(u8 procControl) {
        mProcControl &= ~procControl;
    }

    void setParams() {
        mID = m_rootUniqueID;
        mParams = m_tmpCtData.params;
        mProfileName = m_tmpCtData.prof_name;
        mGroupType = m_tmpCtData.group_type;
    }

    u32 getFromParams(u8 shift, u32 mask) {
        return (mParams >> shift) & mask;
    }

public:
    fBase_c();
    static void *operator new(size_t);
    static void operator delete(void *);

public:
    /* vt 0x08 */ virtual int create();
    /* vt 0x0C */ virtual int preCreate();
    /* vt 0x10 */ virtual void postCreate(MAIN_STATE_e state);
    /* vt 0x14 */ virtual int doDelete();
    /* vt 0x18 */ virtual int preDelete();
    /* vt 0x1C */ virtual void postDelete(MAIN_STATE_e state);
    /* vt 0x20 */ virtual int execute();
    /* vt 0x24 */ virtual int preExecute();
    /* vt 0x28 */ virtual void postExecute(MAIN_STATE_e state);
    /* vt 0x2C */ virtual int draw();
    /* vt 0x30 */ virtual int preDraw();
    /* vt 0x34 */ virtual void postDraw(MAIN_STATE_e state);
    /* vt 0x38 */ virtual void deleteReady();
    /* vt 0x3C */ virtual bool entryFrmHeap(size_t size, EGG::Heap *parentHeap);
    /* vt 0x40 */ virtual bool entryFrmHeapNonAdjust(size_t size, EGG::Heap *parentHeap);
    /* vt 0x44 */ virtual bool createHeap();
    /* vt 0x48 */ virtual ~fBase_c();

public:
    int commonPack(int (fBase_c::*doFunc)(), int (fBase_c::*preFunc)(), void (fBase_c::*postFunc)(MAIN_STATE_e));
    int createPack();
    int deletePack();
    int executePack();
    int drawPack();
    int connectProc();
    void deleteRequest();
    void forceUpdate();
    static fBase_c *getConnectRoot();
    static fBase_c *getConnectTreeNext(fBase_c *);
    fBase_c *getConnectParent() const;
    fBase_c *getConnectChild() const;
    fBase_c *getConnectBrNext() const;
    void updateExecutePriority(u16 priority);
    bool setConnectChild(fBase_c *child);
    void runCreate();
    fBase_c *getChildProcessCreateState() const;
    bool checkChildProcessCreateState() const;

public:
    static void setTmpCtData(ProfileName profName, fTrNdBa_c *connectParent, u32 param, u8 groupType);
    static fBase_c *fBase_make(ProfileName profName, fTrNdBa_c *connectParent, u32 param, u8 groupType);
    static fBase_c *createRoot(ProfileName profName, u32 param, u8 groupType);
    static fBase_c *createChild(ProfileName profName, fBase_c *parent, u32 param, u8 groupType);

public:
    static fLiMgBa_c m_forceExecuteList;
    static int (*sLoadAsyncCallback)();
    static void (*sUnloadCallback)();

private:
    static fBaseID_e m_rootUniqueID;

    struct ConstructData {
        ProfileName prof_name;
        fTrNdBa_c *connect_parent;
        u32 params;
        u8 group_type;
    };

    static ConstructData m_tmpCtData;

    friend class fManager_c;
    friend class fLiNdBa_c;
    friend class fTrMgBa_c;
};

#endif
