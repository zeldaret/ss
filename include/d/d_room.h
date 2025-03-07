#ifndef D_ROOM_H
#define D_ROOM_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/col/bg/d_bg_w_kcol.h"
#include "d/d_base.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_anmvis.h"
#include "m/m3d/m_smdl.h"
#include "m/m_allocator.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "nw4r/math/math_geometry.h"
#include "s/s_State.hpp"
#include "sized_string.h"
#include "toBeSorted/unk_with_water.h"

struct DrawPriorityConfig {
    u8 pDrawOpa1;
    u8 pDrawXlu;
    u8 pDrawOpa2;
};

// This could be std::unique_ptr, but we don't have it yet
template <typename T>
class RaiiPtr {
public:
    T *mPtr;

    RaiiPtr() : mPtr(nullptr) {}
    ~RaiiPtr() {
        if (mPtr != nullptr) {
            delete mPtr;
            mPtr = nullptr;
        }
    }

    void operator=(T *ptr) {
        mPtr = ptr;
    }

    operator bool() const {
        return mPtr != nullptr;
    }

    const T *operator->() const {
        return mPtr;
    }

    T *operator->() {
        return mPtr;
    }

    const T &operator*() const {
        return *this->operator->();
    }

    T &operator*() {
        return *this->operator->();
    }
};

class dRoom_c : public dBase_c {
    class mdl_c : public m3d::smdl_c {
    public:
        mdl_c() : mpAabb(nullptr) {}
        virtual ~mdl_c() {
            remove();
        }
        virtual void remove() override;

        bool hasModel() const {
            return getG3dObject() != nullptr;
        }

        bool create(nw4r::g3d::ResMdl resMdl, mAllocator_c &alloc);
        void configureSomething();
        void somethingVisibility(u32 id, bool visible);

    private:
        void doSomethingWithVis(bool arg);
        nw4r::math::AABB *mpAabb;
    };
    class model_c {
        friend class dRoom_c;

    public:
        model_c() {}

        bool create(nw4r::g3d::ResFile resFile, mAllocator_c &alloc, s32 idx, UnkWithWater *waterThing);
        bool getBounds(mVec3_c *min, mVec3_c *max) const;
        nw4r::g3d::ResNode getResNode(const char *nodeName);
        void updateObjNode(const char *node, bool visible);
        void destroy();
        void execute(s32 idx, bool roomfield_0x573, f32 pastState);
        void draw(int roomid);
        f32 getFrame() const;
        void configureSomething(int roomid, mdl_c *mdl);

        const DrawPriorityConfig *getDrawPriority(s32 idx);

    private:
        mdl_c mMdl;
        RaiiPtr<m3d::anmMatClr_c> mpAnmClr;
        RaiiPtr<m3d::anmTexPat_c> mpAnmPat;
        RaiiPtr<m3d::anmTexSrt_c> mpAnmSrt;
        RaiiPtr<m3d::anmVis_c> mpAnmVis;
    };

public:
    dRoom_c()
        : mStateMgr(*this, sStateID::null),
          field_0x15C(nullptr),
          SCEN(nullptr),
          PLY(nullptr),
          CAM(nullptr),
          EVNT(nullptr),
          PATH(nullptr),
          PNT(nullptr),
          BPNT(nullptr),
          AREA(nullptr),
          unkCount(0),
          scenCount(0),
          plyCount(0),
          camCount(0),
          evntCount(0),
          pathCount(0),
          pntCount(0),
          bpntCount(0),
          areaCount(0),
          mFlags(0) {}
    virtual ~dRoom_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int execute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dRoom_c, Active);
    STATE_FUNC_DECLARE(dRoom_c, NonActive);

    bool checkFlag(u32 flag) {
        return mFlags & flag;
    }

    void drawOnMapIfVisible(mMtx_c *mtx, int param);
    void getBounds(mVec3_c *min, mVec3_c *max) const;

    void someLastBossThing(bool arg);
    f32 getFrame() const;

    typedef void (*foreachObjCallback)(dAcBase_c *obj);
    s32 foreachObject(foreachObjCallback cb);

private:
    bool setupBg();
    void executeBg();
    void releaseBg();

    static void formatObj(int obj, SizedString<8> &str);
    void updateObjNodeInEachRoom(int obj, bool visible);
    void destroyModels();
    void activateUpdates();
    void deactivateUpdates();

    /* 0x068 */ mHeapAllocator_c mAllocator;
    /* 0x084 */ nw4r::g3d::ResFile mRoomRes;
    /* 0x088 */ UnkWithWater mWaterThing;
    /* 0x0A0 */ model_c mModels[8];
    /* 0x220 */ dBgWKCol mBg[2];
    /* 0x4E0 */ STATE_MGR_DECLARE(dRoom_c);

    /* 0x51C */ void *field_0x15C;
    /* 0x520 */ void *SCEN;
    /* 0x524 */ void *PLY;
    /* 0x528 */ void *CAM;
    /* 0x52C */ void *EVNT;
    /* 0x530 */ void *PATH;
    /* 0x534 */ void *PNT;
    /* 0x538 */ void *BPNT;
    /* 0x53C */ void *SPTH;
    /* 0x540 */ void *field_0x540;
    /* 0x544 */ void *field_0x544;
    /* 0x548 */ void *AREA;
    /* 0x54C */ u16 unkCount;
    /* 0x54E */ u16 scenCount;
    /* 0x550 */ u16 plyCount;
    /* 0x552 */ u16 camCount;
    /* 0x554 */ u16 evntCount;
    /* 0x556 */ u16 pathCount;
    /* 0x558 */ u16 pntCount;
    /* 0x55A */ u16 bpntCount;
    /* 0x55C */ u16 spthCount;
    /* 0x55E */ u16 unk2Count;
    /* 0x560 */ u16 unk3Count;
    /* 0x562 */ u16 areaCount;

    // peak vtable placement
    class UnkRoomClass {
    public:
        virtual ~UnkRoomClass() {}
    };

    /* 0x564 */ UnkRoomClass mUnk;
    /* 0x568 */ u8 mFlags;
    void *BZS;
    /* 0x570 */ s8 roomid;
    /* 0x571 */ bool field_0x571;
    /* 0x572 */ bool field_0x572;
    /* 0x573 */ bool field_0x573;
    /* 0x574 */ bool mHasAnmTexPat;
    /* 0x575 */ bool mDidAlreadyInit;
    /* 0x576 */ bool field_0x576;
};

#endif
