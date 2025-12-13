#ifndef D_T_MASS_OBJ_H
#define D_T_MASS_OBJ_H

#include "c/c_math.h"
#include "common.h"
#include "d/d_heap.h"
#include "d/t/d_tg.h"
#include "egg/core/eggHeap.h"
#include "egg/math/eggMatrix.h"
#include "egg/math/eggQuat.h"
#include "m/m3d/m_proc.h"
#include "m/m3d/m_scnleaf.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_heap.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resshp.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXTypes.h"
#include "s/s_Math.h"
#include "toBeSorted/tlist.h"

class dTgMassObjInstance;
class dTgMassObjTransform;
class GrassModel;
class GrassModelData;
class dCcMassS_HitInf;
class dAcObjBase_c;

typedef TList<dTgMassObjInstance, 0> dTgMassObjInstanceList;
typedef TList<dTgMassObjTransform, 0> dTgMassObjTransformList;

// a single piece of grass
// dTgMassObjInstance
// size: 0x34
class dTgMassObjInstance {
public:
    enum Flags {
        TG_MASS_UNK2_IS_HIDDEN = 1,
        TG_MASS_UNK2_IS_CUT = 2,
        TG_MASS_UNK2_TIMESHIFT_RELATED = 4,
    };
    dTgMassObjInstance(){
        reset();
    }
    ~dTgMassObjInstance() {}
    void reset();
    void releaseDynamicTransform(GrassModel *param_2);
    s32 checkForHit(GrassModel *param2,GrassModelData *param3,u16 roomid);
    undefined4 checkForHit(u32 param_2,dCcMassS_HitInf& param_3,dAcObjBase_c* param_4,
            GrassModel *param_5,GrassModelData* param_6,undefined4 roomid);
    undefined4 FUN_80278c70(u32 param_2,dCcMassS_HitInf & param_3,dAcBase_c *param_4,
            GrassModel *param_5);
    undefined4 handleLinkSpinAttack(GrassModel *param_2);
    s32 handleTimeshiftZone();
    void getDrawMatrix(mMtx_c *pOut);
    bool isHidden(f32 param2, f32 param3);
    bool isItemDrop10() const {
        return mSpecialItemDropId == 10;
    }

// private:
    /* 0x00 */ TListNode<dTgMassObjInstance> mLink;
    /* 0x08 */ GXColorS10 mTevColor;
    /* 0x10 */ mVec3_c mGroundHeight;
    /* 0x1C */ dTgMassObjTransform *mInitPosTransform;
    /* 0x20 */ dTgMassObjTransform *mDynamicTransform;
    /* 0x24 */ f32 field_0x24; // unused?
    /* 0x28 */ f32 mScale;
    /* 0x2C */ u16 mGrassFlags;
    /* 0x2E */ u16 yRotation;
    /* 0x30 */ u8 mSpecialItemDropId;
    /* 0x31 */ u8 mLightingCode;
    /* 0x32 */ u8 mMassObjSubtype;
    /* 0x33 */ bool mActiveInPresent;
};

// a transform for a single piece of grass
// size: 0x50
class dTgMassObjTransform {
public:

    dTgMassObjTransform();
    ~dTgMassObjTransform() {}
    void update();
    const mMtx_c& getMtx() const {
        return mMtx;
    }

    void setQuat(const mQuat_c& quat) {
        mQuat.set(quat);
    }
    
    void setMtxFromQuat(const mQuat_c& quat) {
        mMtx.fromQuat(quat);
    }

    /* 0x00 */ s32 field_0x00;
    /* 0x04 */ s32 field_0x04;
    /* 0x08 */ mMtx_c mMtx;
    /* 0x38 */ mAng mRotY;
    /* 0x3A */ mAng mRotX;
    /* 0x3C */ s16 mRotXSpeed;
    /* 0x40 */ EGG::Quatf mQuat;
};

class GrassModelData {
public:
    GrassModelData() : mLinkedLists(nullptr) {}
    virtual ~GrassModelData() {
        destroyLinkedLists();
    }
    void initRes(nw4r::g3d::ResMat pResMat, nw4r::g3d::ResShp pResShp);
    void addToRoom(s32 room, dTgMassObjInstance* p3);
    void removeFromRoom(s32 room, dTgMassObjInstance* p3);
    void update(GrassModel*);
    bool tryCreateLinkedLists(s32 entrycount, EGG::Heap* heap);
    void destroyLinkedLists();
    void unloadRoom(GrassModel *param_2,int roomid);
    void draw(f32 param_1,f32 param_2,
                 nw4r::math::MTX34 *param_4);
private:
    /* 0x04 */ nw4r::g3d::ResMat mResMat;
    /* 0x08 */ nw4r::g3d::ResShp mResShp;
    /* 0x0C */ dTgMassObjInstanceList* mLinkedLists;
    /* 0x10 */ u16 mLinkedListsCount;
};

struct GrassModelInfo {
    /* 0x00 */ char* mArcName;
    /* 0x04 */ char* mModelName;
    /* 0x08 */ char* mCutModelName;
    /* 0x0C */ u32 field_0x0C;
    /* 0x10 */ u16 mRadius;
    /* 0x12 */ u16 field_0x12;
    /* 0x14 */ u16 mInstanceCount;
    /* 0x16 */ u16 mStaticTransformCount;
    /* 0x18 */ u16 mDynamicTransformCount;
    /* 0x1A */ u8 field_0x1A;
    /* 0x1B */ u8 field_0x1B; // might be padding
};


struct GrassModelNames {
    /* 0x00 */ char* mArcName;
    /* 0x04 */ char* mModelName;
    /* 0x08 */ char* mCutModelName;
};

class GrassModel : public m3d::proc_c {
public:
    GrassModel() : mpModelData(nullptr), mInstanceList(nullptr), mStaticTransformationList(nullptr), mDynamicTransformationList(nullptr), mCutCounter(0) {}
    /* vt 0x08 */ virtual ~GrassModel() {
        remove();
    }
    undefined4
    setModelInfo(f32 radius,f32 param_2,int param_4,
            s32 roomCount,u16 instanceListLength,u16 staticTransformationListLength,int dynamicTransformationListLength,
            undefined1 param_9,s32 opaDrawPrio,u32 xluDrawPrio,mHeapAllocator_c *allocator);
    undefined4
    spawnSingleGrass(int modelSubtype,u16 roomid,mVec3_c *groundHeight,
            u16 yRotation,s32 specialItemDropId,int affectedByTimeshift,int activeInPresent,
            s32 massObjSubtype,u8 lightingCode);
    void addToFreeInstances(dTgMassObjInstance *param_2);
    s32 addToRoom(u32 param2, s32 roomid, dTgMassObjInstance* param4);
    void unloadRoom(u16 roomid);
    void initResForModel(s32 param_2,nw4r::g3d::ResMat pResMat,nw4r::g3d::ResShp pResShp);
    dTgMassObjTransform* aquireTransform();
    void update();
    void releaseTransform(dTgMassObjTransform*);
    void setModelInfo(const GrassModelInfo* modelInfo, mHeapAllocator_c * heap) {
        setModelInfo(
                modelInfo->mRadius, modelInfo->field_0x0C,
                 modelInfo->field_0x12, 0x3F, 
                 modelInfo->mInstanceCount, modelInfo->mStaticTransformCount, 
                 modelInfo->mDynamicTransformCount, (f32)modelInfo->field_0x1A, 
                 0x7F, 0xFFFFFFFF, heap
            );
    }

    void setRadius(f32 r) {
        mRadius = r;
        mRadiusSquared = r * r;
    }

    void setInstanceLength(s32 len) {
        mInstanceListLength = len;
    }

    s32 getInstanceListLength() const {
        return mInstanceListLength;
    }

    s32 getDynamicTransformListLength() const {
        return mDynamicTransformationListLength;
    }

    dTgMassObjInstanceList& getFree() {
        return mFreeInstances;
    }

    bool testSpecialItemtype(s32 itemtype) {
        return itemtype == 10;
    }
    
    /* vt 0x10 */ virtual void remove() override;
    // /* vt 0x14 */ virtual int entry();
    /* vt 0x18 */ virtual void drawOpa() override {
        draw();
    }
    /* vt 0x1C */ virtual void drawXlu() override {
        draw();
    }
    void draw();

    void calcCutCounter() {
        mCutCounter = (mCutCounter != 0) ? mCutCounter-1 : mCutCounter;
    }

    dTgMassObjTransformList& GetUnk3Too() {
        return mAquiredTransforms;
    }

    dTgMassObjTransformList& GetUnk3() {
        return mAvailableTransforms;
    }

// private:
    /* 0x18 */ GrassModelData* mpModelData;
    /* 0x1C */ dTgMassObjInstance* mInstanceList;
    /* 0x20 */ dTgMassObjTransform* mStaticTransformationList;
    /* 0x24 */ dTgMassObjTransform* mDynamicTransformationList;
    /* 0x28 */ dTgMassObjInstanceList mFreeInstances;
    /* 0x34 */ dTgMassObjTransformList mAquiredTransforms;
    /* 0x40 */ dTgMassObjTransformList mAvailableTransforms;
    /* 0x4C */ f32 mRadius;
    /* 0x50 */ f32 mRadiusSquared;
    /* 0x54 */ f32 field_0x54;
    /* 0x58 */ u16 field_0x58;
    /* 0x5A */ u16 mRoomCount;
    /* 0x5C */ u16 mInstanceListLength;
    /* 0x5E */ u16 mStaticTransformationListLength;
    /* 0x60 */ u16 mDynamicTransformationListLength;
    /* 0x62 */ u8 field_0x62;
    /* 0x63 */ u8 field_0x63;
    /* 0x64 */ s16 mCutCounter;
    /* 0x66 */ u8 field_0x66;
    /* 0x67 */ u8 field_0x67;
};

class dTgMassObj_c : public dTg_c {
public:
    dTgMassObj_c() {}
    virtual ~dTgMassObj_c() {}

    static dTgMassObj_c *GetInstance() {
        return sInstance;
    }

    static u8 getCurrentStageGrassSubtype();

    void unloadRoom(u16 roomid);

    int initializeCircle(GrassModel *grassModel, mVec3_c *bbStart, mVec3_c *bbEnd,s32 xzDisplacement,mMtx_c *param_7,s32 specialItemDropId,
                undefined4 affectedByTimeshift,u8 activeInPresent,int randInt,s32 massObjSubtype,
                u8 lightingCode, f32 fParam);

    int initializeBox(GrassModel *grassModel,mVec3_c *minVec,
                mVec3_c *maxVec,int xzDisplacement,mMtx_c *boundsMtx,int specialItemDropId,undefined4 affectedByTimeshift,
                u8 activeInPresent,int randInt,s32 massObjSubtype,u8 lightingCode, f32 fParam);

    virtual int actorExecute() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int draw() override;
    virtual int doDelete() override;

    mMtx_c getPreparedMtx() const {
        mMtx_c mtx;
        mtx.transS(mPosition);
        mtx.YrotM(mRotation.y);
        mtx.scaleM(mScale);
        mMtx_c mtx2 = mtx;
        mtx2.inverse();
        return mtx2;
    }

    GrassModel* getGrassModel(s32 i) {
        return sGrassModels[i];
    }

    void toGlobalRoom() {
        s8 roomid = getRoomId();
        addActorToRoom(-1);
        mRoomID = roomid;
    }

    s32 getGrassTypeFromParams() {
        return getFromParams(0, 7);
    }

    mMtx_c makeBoundsMtx() {
        mMtx_c mtx;
        mtx.transS(mPosition);
        mtx.YrotM(mRotation.y);
        mtx.scaleM(mScale);
        return mtx;
    }

private:
    static dTgMassObj_c *sInstance;
    static mHeapAllocator_c *sAllocator;
    static GrassModel *sGrassModels[5];

    /* 0x0FC */ s32 mMassSubtype;
    /* 0x100 */ u8 field_0xFC[0x104 - 0x100];
};

#endif
