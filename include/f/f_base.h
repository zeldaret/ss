#ifndef F_BASE_H
#define F_BASE_H

// This file is adapted from
// https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/framework/f_base.hpp and the Skyward Sword
// Ghidra database. Comments and docs can be seen above. stripped in this file for easier looking

#include "egg/core/eggExpHeap.h"
#include "egg/core/eggFrmHeap.h"
#include "f/f_base_id.h"
#include "f/f_helper_unk.h"
#include "f/f_list_mg.h"
#include "f/f_manager.h"
#include "f/f_profile.h"
#include <common.h>

// Ghidra: fBase
//   size: 0x64
// official name
class fBase_c {
public:
    /* 0x00 */ fBaseID_e unique_ID;
    /* 0x04 */ u32 params;               // params1
    /* 0x08 */ ProfileName profile_name; // Actor Id
    /* 0x0A */ u8 lifecycle_state;
    /* 0x0B */ bool delete_request;
    /* 0x0C */ s8 update_request;
    /* 0x0D */ bool create_request;
    /* 0x0E */ u8 group_type;
    /* 0x0F */ u8 proc_control;
    /* 0x10 */ fManager_c manager;
    /* 0x50 */ fBaHelper_c *p_helper;
    /* 0x54 */ fLiMgBa_c actor_list;
    /* 0x5C */ EGG::FrmHeap *p_heap;
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
    enum MAIN_STATE_e { CANCELED, ERROR, SUCCESS, WAITING };
    enum PACK_RESULT_e {
        NOT_READY,
        SUCCEEDED,
        FAILED,
    };
    enum PROC_DISABLE_e { ROOT_DISABLE_EXECUTE = 1, DISABLE_EXECUTE = 2, ROOT_DISABLE_DRAW = 4, DISABLE_DRAW = 8 };

    // Inlines
    bool isProcControlFlag(u8 flag) const {
        return (proc_control & flag) != 0;
    }
    void setProcControlFlag(u8 flag) {
        proc_control |= flag;
    }
    void clearProcControlFlag(u8 flag) {
        proc_control &= ~flag;
    }

    fManager_c *getManager() {
        return &manager;
    }

    void setParams() {
        unique_ID = m_rootUniqueID;
        params = m_tmpCtData.params;
        profile_name = m_tmpCtData.prof_name;
        group_type = m_tmpCtData.group_type;
    }

public:
    /* 802e12f0 */ fBase_c();
    /* 802e23b0 */ static void *operator new(size_t);
    /* 802e2410 */ static void operator delete(void *);

public: // vtable 0x60
    /* 0x08 | 802E15C0 */ virtual int create();
    /* 0x0C | 802E15D0 */ virtual int preCreate();
    /* 0x10 | 802E15E0 */ virtual void postCreate(MAIN_STATE_e state);
    /* 0x14 | 802E1670 */ virtual int doDelete();
    /* 0x18 | 802E1730 */ virtual int preDelete();
    /* 0x1C | 802E17A0 */ virtual void postDelete(MAIN_STATE_e state);
    /* 0x20 | 802E1910 */ virtual int execute();
    /* 0x24 | 802E1920 */ virtual int preExecute();
    /* 0x28 | 802E1950 */ virtual void postExecute(MAIN_STATE_e state);
    /* 0x2C | 802E1A90 */ virtual int draw();
    /* 0x30 | 802E1AA0 */ virtual int preDraw();
    /* 0x34 | 802E1AD0 */ virtual void postDraw(MAIN_STATE_e state);
    /* 0x38 | 802E1B90 */ virtual void deleteReady();
    /* 0x3C | 802E20E0 */ virtual bool entryFrmHeap(size_t size, EGG::Heap *parentHeap);
    /* 0x40 | 802E22E0 */ virtual bool entryFrmHeapNonAdjust(size_t size, EGG::Heap *parentHeap);
    /* 0x44 | 802E23A0 */ virtual bool createHeap();
    /* 0x48 | 802E1480 */ virtual ~fBase_c();

public:
    /* 802e1500 */ int commonPack(int (fBase_c::*doFunc)(), int (fBase_c::*preFunc)(),
            void (fBase_c::*postFunc)(MAIN_STATE_e));
    /* 802e1680 */ int createPack();
    /* 802e1860 */ int deletePack();
    /* 802e1960 */ int executePack();
    /* 802e1ae0 */ int drawPack();
    /* 802e1ba0 */ int connectProc();
    /* 802e1e00 */ void deleteRequest();
    /* 802e1e80 */ void forceUpdate();
    /* 802e1f90 */ static fBase_c *getConnectRoot();
    /* 802e1f90 */ static fBase_c *getConnectTreeNext(fBase_c *);
    /* 802e1f90 */ fBase_c *getConnectParent() const;
    /* 802e1fb0 */ fBase_c *getConnectChild() const;
    /* 802e1fd0 */ fBase_c *getConnectBrNext() const;
    /* 802e1ff0 */ void updateExecutePriority(u16 priority);
    /* 802e2090 */ bool setConnectChild(fBase_c *child);
    /* 802e2420 */ void runCreate();
    /* 802e24a0 */ fBase_c *getChildProcessCreateState() const;
    /* 802e2510 */ bool checkChildProcessCreateState() const;

public:
    /* 802e2540 */ static void setTmpCtData(ProfileName profName, fTrNdBa_c *connectParent, unsigned long param,
            u8 groupType);
    /* 802e2560 */ static fBase_c *fBase_make(ProfileName profName, fTrNdBa_c *connectParent, unsigned long param,
            u8 groupType);
    /* 802e2640 */ static fBase_c *createRoot(ProfileName profName, unsigned long param, u8 groupType);
    /* 802e2600 */ static fBase_c *createChild(ProfileName profName, fBase_c *parent, unsigned long param,
            u8 groupType);

public:
    /* 80575ba8 */ static fLiMgBa_c m_forceExecuteList;
    /* 80575bb0 */ static int (*sLoadAsyncCallback)();
    /* 80575bb4 */ static void (*sUnloadCallback)();

private:
    /* 80573fb8 */ static fBaseID_e m_rootUniqueID;

    struct ConstructData {
        /* 805b84c8 */ ProfileName prof_name;
        /* 805b84cc */ fTrNdBa_c *connect_parent;
        /* 805b84d0 */ u32 params;
        /* 805b84d4 */ u8 group_type;
    };

    static ConstructData m_tmpCtData;

    friend class fManager_c;
    friend class fLiNdBa_c;
    friend class fTrMgBa_c;
};

#endif
