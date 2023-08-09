#pragma once

// This file is adapted from https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/framework/f_base.hpp 
// and the Skyward Sword Ghidra database. Comments and docs can be seen above. stripped in this file for easier looking

#include <types.h>
#include <lib/egg/core/eggFrmHeap.h>
#include <f/f_profile.h>
#include <f/f_base_id.h>
#include <f/f_helper_unk.h>
#include <f/f_manager.h>
#include <f/f_list_mg.h>

// Ghidra: fBase
//   size: 0x64
// official name
class fBase_c {
public:
    /* 0x00 */ fBaseID_e        mUniqueID;
    /* 0x04 */ undefined4       mParam;    // params1
    /* 0x08 */ ProfileName      mProfName; // Actor Id
protected:
    /* 0x0A */ u8               mLifecycleState;
    /* 0x0B */ bool             mDeleteRequest;
    /* 0x0C */ bool             mUpdateRequest;
    /* 0x0D */ bool             mRetryCreate;
    /* 0x0E */ u8               mGroupType;
    /* 0x0F */ u8               mProcControl;
    /* 0x10 */ fManager_c       mMng;
    /* 0x50 */ fBaHelper_c*     mpUnusedHelper;
    /* 0x54 */ fLiMgBa_c        mUnusedList;
    /* 0x5C */ EGG::FrmHeap*    mpHeap;
    /* 0x60 */ // vtable
public:
    enum LIFECYCLE_e {
        WAITING_FOR_CREATE,
        ACTIVE,
        TO_BE_DELETED
    };
    enum GROUP_TYPE_e {
        OTHER,
        SCENE,
        ACTOR
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

    bool isProcControlFlag(u8 flag) const { return (mProcControl & flag) != 0; }
    void setProcControlFlag(u8 flag) { mProcControl |= flag; }
    void clearProcControlFlag(u8 flag) { mProcControl &= ~flag; }
public:
    fBase_c();

    static void *operator new(size_t);
    static void operator delete(void *);

protected:
    // This vtable should exist at 0x60
    /* 0x08 */ virtual int create();
    /* 0x0C */ virtual int preCreate();
    /* 0x10 */ virtual void postCreate(MAIN_STATE_e state);
    /* 0x14 */ virtual int doDelete();
    /* 0x18 */ virtual int preDelete();
    /* 0x1C */ virtual void postDelete(MAIN_STATE_e state);
    /* 0x20 */ virtual int execute();
    /* 0x24 */ virtual int preExecute();
    /* 0x28 */ virtual void postExecute(MAIN_STATE_e state);
    /* 0x2C */ virtual int draw();
    /* 0x30 */ virtual int preDraw();
    /* 0x34 */ virtual void postDraw(MAIN_STATE_e state);
    /* 0x38 */ virtual void deleteReady();
    /* 0x3C */ virtual bool entryFrmHeap(unsigned long size, EGG::Heap *parentHeap);
    /* 0x40 */ virtual bool entryFrmHeapNonAdjust(unsigned long size, EGG::Heap *parentHeap);
    /* 0x44 */ virtual bool createHeap(); 
    /* 0x48 */ virtual ~fBase_c(); 


public:
    void deleteRequest();
    fBase_c *getConnectParent() const;
    fBase_c *getConnectChild() const;
    fBase_c *getConnectBrNext() const;
    bool checkChildProcessCreateState() const;

private:
    int createPack();
    int deletePack();
    int executePack();
    int drawPack();
    int commonPack(int (fBase_c::*doFunc)(), int (fBase_c::*preFunc)(), void (fBase_c::*postFunc)(MAIN_STATE_e));
    int connectProc();
    void runCreate();
    fBase_c *getChildProcessCreateState() const;

public:
    static fBase_c *createChild(ProfileName profName, fBase_c *parent, unsigned long param, u8 groupType);
    static fBase_c *createRoot(ProfileName profName, unsigned long param, u8 groupType);

private:
    static void setTmpCtData(ProfileName profName, fTrNdBa_c *connectParent, unsigned long param, u8 groupType);
    static fBase_c *fBase_make(ProfileName profName, fTrNdBa_c *connectParent, unsigned long param, u8 groupType);

protected:
    // these still exist and are still unused
    static int (*sLoadAsyncCallback)();
    static void (*sUnloadCallback)();

private:
    static fBaseID_e m_rootUniqueID;
    static ProfileName m_tmpCtProfName;
    static fTrNdBa_c *m_tmpCtConnectParent;
    static u32 m_tmpCtParam;
    static u8 m_tmpCtGroupType;

    friend class fManager_c;
    friend class fLiNdBa_c;
    friend class fTrMgBa_c;
};