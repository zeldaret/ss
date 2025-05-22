#ifndef D_A_PLAYER_MDL_H
#define D_A_PLAYER_MDL_H

#include "d/col/cc/d_cc_d.h"
#include "d/d_player_act.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_anmchrblend.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_anmtexsrt.h"
#include "toBeSorted/stage_render_stuff.h"

struct PlayerAnimation {
    /* 0x00 */ const char *animName;
    /* 0x04 */ u8 field_0x04;
    /* 0x05 */ u8 field_0x05;
    /* 0x06 */ u16 mFaceTexPatIdx;
    /* 0x08 */ u16 mFaceTexSrtIdx;
    /* 0x0A */ u16 mFaceAnmChrIdx;
};

class daPlayerModelBase_c;

class daPlBaseScnObjCallback_c : public dScnCallback_c {
public:
    daPlBaseScnObjCallback_c(dAcObjBase_c *b) : dScnCallback_c(b), field_0x0C(b) {}

    /* vt 0x10 */ virtual void
    ExecCallback_CALC_MAT(nw4r::g3d::ScnObj::Timing, nw4r::g3d::ScnObj *, u32, void *) override;

    /* 0x18 */ virtual void ExecCallback_DRAW_OPA(nw4r::g3d::ScnObj::Timing, nw4r::g3d::ScnObj *, u32, void *) override;

private:
    /* 0x0C */ dAcObjBase_c *field_0x0C;
};

class daPlBaseMdlCallback_c : public m3d::mdl_c::mdlCallback_c {
public:
    virtual void ExecCallbackA(nw4r::g3d::ChrAnmResult *, nw4r::g3d::ResMdl, nw4r::g3d::FuncObjCalcWorld *) override;

    void setNodeIds(u16 min, u16 max) {
        mNodeIdMin = min;
        mNodeIdMax = max;
    }

private:
    /* 0x30 */ u16 mNodeIdMin;
    /* 0x32 */ u16 mNodeIdMax;
};

class daPlBaseMdl_c : public m3d::mdl_c {
public:
    daPlBaseMdl_c() : mpSoundData(nullptr) {}

    virtual bool setAnm(m3d::banm_c &anm) override;
    virtual void remove() override;

    bool create(
        daPlayerModelBase_c *player, nw4r::g3d::ResMdl mdl, mAllocator_c *alloc, u32 bufferOption, u32 nView, u32 *pSize
    );
    void setBlendNodeRange(u16, u16, f32);

    nw4r::g3d::ChrAnmResult *getNodeResult(u16 node) {
        return mCallback.getNodeResult(node);
    }

    void setSoundRelated(u16 id, void *data) {
        field_0x5A = id;
        mpSoundData = data;
    }

    void setField_0x58(u8 val) {
        field_0x58 = val;
    }

    u16 getField_0x5A() const {
        return field_0x5A;
    }

    void *getSoundData() const {
        return mpSoundData;
    }

private:
    /* 0x24 */ daPlBaseMdlCallback_c mCallback;
    /* 0x58 */ u8 field_0x58;
    /* 0x5A */ u16 field_0x5A;
    /* 0x5C */ void *mpSoundData;
};

class daPlBaseMainCallback_c : public m3d::callback_c {
public:
    virtual void timingA(u32, nw4r::g3d::ChrAnmResult *, nw4r::g3d::ResMdl) override;
    virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;
    virtual void timingC(nw4r::math::MTX34 *, nw4r::g3d::ResMdl) override;

    void setPlayer(daPlayerModelBase_c *p) {
        mpPlayer = p;
    }

private:
    daPlayerModelBase_c *mpPlayer;
};

class daPlBaseAnmChr_c : public m3d::anmChr_c {
public:
    daPlBaseAnmChr_c() : field_0x35(0), field_0x36(0) {}

    void play() override;
    bool isFinished();

    void setField_0x35(u8 value) {
        field_0x35 = value;
    }

private:
    /* 0x35 */ u8 field_0x35;
    /* 0x36 */ u8 field_0x36;
};

class daPlBaseHandsCallback_c : public nw4r::g3d::ICalcWorldCallback {
public:
    virtual void ExecCallbackC(
        nw4r::math::MTX34 *pMtxArray, nw4r::g3d::ResMdl mdl,
        nw4r::g3d::FuncObjCalcWorld *pFuncObj
    ) override; // at 0x14

    void setPlayer(daPlayerModelBase_c *p) {
        mpPlayer = p;
    }

private:
    /* 0x04 */ daPlayerModelBase_c *mpPlayer;
};

class daPlBaseHeadCallback_c : public m3d::callback_c {
public:
    virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;

    void setPlayer(daPlayerModelBase_c *p) {
        mpPlayer = p;
    }

private:
    daPlayerModelBase_c *mpPlayer;
};

// Vtable at 8050da00, dtor reveals two superclasses
// until dAcObjBase_c. Name unknown
class daPlayerModelBase_c : public daPlayerActBase_c {
public:
    daPlayerModelBase_c()
        : mSwordRes(nullptr),
          mAlinkRes(nullptr),
          mPlCommonAnimeRes(nullptr),
          mPlCommonSoundRes(nullptr),
          mCurrentRes(nullptr),
          mAlink2Res(nullptr),
          mScnObjCallback(this),
          mScnCallback1(this),
          mScnCallback2(this),
          mScnCallback3(this),
          mScnCallback4(this),
          mScnCallback5(this) {}
    virtual ~daPlayerModelBase_c() {}

    enum TunicType_e {
        TUNIC_0,
        TUNIC_1,
        TUNIC_SILENT,
    };

    // Alink.arc > g3d > model.brres > 3DModels(NW4R) > al > Bones > ...
    enum PlayerMainModelNode_e {
        PLAYER_MAIN_NODE_CENTER = 0,
        PLAYER_MAIN_NODE_BACKBONE_1 = 1,
        PLAYER_MAIN_NODE_BACKBONE_2 = 2,
        PLAYER_MAIN_NODE_HEAD = 4,
        PLAYER_MAIN_NODE_POD = 5,

        PLAYER_MAIN_NODE_SHOULDER_L = 6,
        PLAYER_MAIN_NODE_ARM_L1 = 7,
        PLAYER_MAIN_NODE_ARM_L2 = 8,
        PLAYER_MAIN_NODE_HAND_L = 9,
        PLAYER_MAIN_NODE_WEAPON_L = 10,

        PLAYER_MAIN_NODE_SHOULDER_R = 11,
        PLAYER_MAIN_NODE_ARM_R1 = 12,
        PLAYER_MAIN_NODE_ARM_R2 = 13,
        PLAYER_MAIN_NODE_HAND_R = 14,
        PLAYER_MAIN_NODE_WEAPON_R = 15,
        PLAYER_MAIN_NODE_WAIST = 16,

        PLAYER_MAIN_NODE_CLOTCH_L = 17,
        PLAYER_MAIN_NODE_LEG_L1 = 18,
        PLAYER_MAIN_NODE_LEG_L2 = 19,
        PLAYER_MAIN_NODE_FOOT_L = 20,
        PLAYER_MAIN_NODE_TOE_L = 21,

        PLAYER_MAIN_NODE_CLOTCH_R = 22,
        PLAYER_MAIN_NODE_LEG_R1 = 23,
        PLAYER_MAIN_NODE_LEG_R2 = 24,
        PLAYER_MAIN_NODE_FOOT_R = 25,
        PLAYER_MAIN_NODE_TOE_R = 26,

        PLAYER_MAIN_NODE_FSKIRT_L1 = 27,
        PLAYER_MAIN_NODE_FSKIRT_L2 = 28,

        PLAYER_MAIN_NODE_FSKIRT_R1 = 29,
        PLAYER_MAIN_NODE_FSKIRT_R2 = 30,

        PLAYER_MAIN_NODE_RSKIRT_L1 = 31,
        PLAYER_MAIN_NODE_RSKIRT_L2 = 32,
        PLAYER_MAIN_NODE_RSKIRT_R1 = 33,
        PLAYER_MAIN_NODE_RSKIRT_R2 = 34,
    };

    // Alink.arc > g3d > model.brres > 3DModels(NW4R) > al_head > Bones > ...
    enum PlayerHeadModelNode_e {
        PLAYER_HEAD_NODE_HAIR_L = 1,
        PLAYER_HEAD_NODE_HAIR_R1 = 2,
        PLAYER_HEAD_NODE_HAIR_R2 = 3,
        PLAYER_HEAD_NODE_MOMI_L = 4,
        PLAYER_HEAD_NODE_MOMI_R = 5,
        PLAYER_HEAD_NODE_MOMI_Z_CAP_1 = 6,
    };

    // Alink.arc > g3d > model.brres > 3DModels(NW4R) > al_hands > Bones > ...
    enum PlayerHandsModelNode_e {
        PLAYER_HANDS_NODE_HAND_L = 1,
        PLAYER_HANDS_NODE_HAND_R = 2,
    };

    void fn_8005F890(nw4r::math::MTX34 *);
    void fn_8005FB90(nw4r::math::MTX34 *);
    bool fn_80061410();

    static void freeFrmHeap(mHeapAllocator_c *allocator);
    static void allocFrmHeap(mHeapAllocator_c *allocator, u32 size, const char *name);
    void allocExternalDataBuffers();
    void initModelHeaps();
    void updateSwordShieldModelsIfNeeded();
    m3d::anmTexSrt_c *createAnmTexSrt(const char *resName, m3d::bmdl_c &mdl);

    static bool createGenericMdl(nw4r::g3d::ResMdl resMdl, m3d::mdl_c &mdl, mAllocator_c *alloc, u32 bufferOption);
    static bool createGenericSmdl(nw4r::g3d::ResMdl resMdl, m3d::smdl_c &mdl, mAllocator_c *alloc, u32 bufferOption);
    static void setTransformAndCalc(m3d::scnLeaf_c &lf, const mMtx_c *mtx);
    void setShieldTransform(bool inHand);

    static nw4r::g3d::ResFile getItemResFile(const char *name, mAllocator_c &allocator);
    nw4r::g3d::ResFile getExternalCompressedFile(const char *name, const char *extension, void *dest, u32 maxSize);
    nw4r::g3d::ResAnmChr getExternalAnmChr(const char *name, void *dest, u32 maxSize);
    nw4r::g3d::ResAnmTexPat getExternalAnmTexPat(const char *name, void *dest, u32 maxSize);
    nw4r::g3d::ResAnmTexSrt getExternalAnmTexSrt(const char *name, void *dest, u32 maxSize);

    static bool isBodyAnmPart_0_2_4(s32 part);

    static u32 getCurrentShieldPouchSlot();
    static s32 getShieldType(s32 item);
    static s32 getCurrentlyEquippedShieldId();

    void updateEarringsColor();
    void loadBodyModels();
    void loadBody();
    void loadHandsModels();
    void setSwordAnm(const char *name);
    void initSwordModel();
    void updateSwordModel();
    void initShieldModel();
    void updateShieldModel();

    // Own virtual methods

    /* vt 0x2E0 */ virtual void transformBackbone1(nw4r::g3d::WorldMtxManip *) {}
    /* vt 0x2E4 */ virtual void transformModelCenter(mMtx_c *) {}
    /* vt 0x2E8 */ virtual bool vt_0x2E8(nw4r::math::MTX34 *, const u16 *, bool) {
        return true;
    }
    /* vt 0x2EC */ virtual void isOnClawTargetMaybe(s32 arm, mAng &a1, mAng &a2) {
        a1 = 0;
        a2 = 0;
    }
    /* vt 0x2F0 */ virtual bool isMPPose() {
        return false;
    }
    /* vt 0x2F4 */ virtual bool isOnTightRope() {
        return false;
    }
    /* vt 0x2F8 */ virtual bool isOnVines() {
        return false;
    }
    /* vt 0x2FC */ virtual bool alwaysRet0() {
        return 0;
    }
    /* vt 0x300 */ virtual bool canBlockAttack() {
        return false;
    }
    /* vt 0x304 */ virtual const mVec3_c &vt_0x304() const {
        return mVec3_c::Zero;
    }
    // r = 1, l = 0, backbone = 2
    /* vt 0x308 */ virtual mAng getArmZRot(s32 which) {
        return 0;
    }
    /* vt 0x30C */ virtual void vt_0x30C(mVec3_c &res) {
        res = mVec3_c::Ex;
    }

    // Overrides

    /* vt 0x0E4 */ virtual const mVec3_c &getCenterTranslation() const override {
        return mCenterTranslation;
    }
    /* vt 0x0E8 */ virtual const mVec3_c &vt_0x0E8() const override {
        return field_0x12F4;
    }
    /* vt 0x0EC */ virtual const mVec3_c &vt_0x0EC() const override {
        return mHeadTranslation;
    }
    /* vt 0x0F0 */ virtual const mVec3_c &vt_0x0F0() const override {
        return mTranslationHand[1];
    }
    /* vt 0x0F4 */ virtual const mVec3_c &vt_0x0F4() const override {
        return mTranslationHand[0];
    }

    /* vt 0x114 */ virtual void somethingWithCarriedActorFlags() override;
    /* vt 0x118 */ virtual dAcObjBase_c *getCurrentCarriedActor() override {
        return mCarriedActorRef.get();
    }

    /* vt 0x18C */ virtual void getBodyMtx(mMtx_c *out_mtx, int boneIdx) override {
        mMainMdl.getNodeWorldMtx(boneIdx, *out_mtx);
    }
    /* vt 0x190 */ virtual void getSheathModelMatrix(mMtx_c *out_mtx) override {
        mSheathMdl.getLocalMtx(*out_mtx);
    }
    /* vt 0x194 */ virtual void getSwordModelMatrix(mMtx_c *out_mtx) override {
        mSwordMdl.getLocalMtx(*out_mtx);
    }
    /* vt 0x198 */ virtual mAng vt_0x198() override {
        return field_0x1268;
    }
    /* vt 0x19C */ virtual mAng vt_0x19C() override {
        return field_0x126A;
    }

    u8 getHandMat(s32 idx) {
        return field_0x1208[idx];
    }

    inline bool checkSwordAndMoreStates(u32 mask) const {
        return (mSwordAndMoreStates & mask) != 0;
    }

    inline void offSwordAndMoreStates(u32 mask) {
        mSwordAndMoreStates &= ~mask;
    }

    bool canStart(bool force, u16 newIdx, u16 invalidValue, u16 *out1, u16 *out2) const;
    void setFaceTexPat(s32 faceIdx, bool force);
    void checkFaceTexPat();
    void setFaceTexSrt(s32 faceIdx, bool force);
    void checkFaceTexSrt();
    void setFaceAnmChr(s32 faceIdx, bool force);
    void checkFaceAnmChr();

    void setPosCopy3();

    void loadTwoAnmChrs(s32 childIdx, s32 animIdx, s32 animIdx2, bool b);
    void loadAnmChr(s32 childIdx, s32 animIdx, void *dest, u32 maxSize);
    void removeAnmChr(s32 childIdx);
    void loadSound(nw4r::g3d::ResFile &file, const char *name, s32 childIdx);
    void loadSoundForAnim(s32 childIdx);

    void syncSoundWithAnim();
    static void registMassObj(cCcD_Obj *obj, u8 priority);
    void updateModelColliders();
    void updateCachedPositions();

    // Model callbacks - dropping the mdl argument, apparently
    void mainModelTimingA(u32 nodeId, nw4r::g3d::ChrAnmResult *result);
    void adjustMainModelChrAnm(PlayerMainModelNode_e nodeId, nw4r::g3d::ChrAnmResult *result);

    void mainModelTimingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result);
    void adjustMainModelWorldMtx(PlayerMainModelNode_e nodeId, nw4r::g3d::WorldMtxManip *result);

    void mainModelTimingC(nw4r::math::MTX34 *result);
    void headModelTimingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result);

    void handsCallbackC(nw4r::math::MTX34 *pMtxArray, nw4r::g3d::ResMdl mdl, nw4r::g3d::FuncObjCalcWorld *pFuncObj);

    void updateBlendWeights(PlayerMainModelNode_e nodeId);
    void updateMainBlend1(f32 blend);
    void updateMainBlend2(f32 blend, bool save);
    void applyWorldRotationMaybe(nw4r::math::MTX34 *result, mAng x, mAng y, mAng z, mVec3_c *off, bool order);
    void applyWorldRotationMaybe(nw4r::g3d::WorldMtxManip *result, mAng x, mAng y, mAng z, mVec3_c *off, bool order);

    static const PlayerAnimation sAnimations[443];
    static const s32 sUnkDuration;
    static const u8 sShieldDurabilities[10];
    static const u8 sShieldRegenTimes[10];
    static mColor sGuideColor1;
    static mColor sGuideColor2;
    static mColor sGuideColor3;

protected:
    /* 0x370 */ mHeapAllocator_c mModelAllocator;
    /* 0x38C */ mHeapAllocator_c mSwordAllocator;
    /* 0x3A8 */ mHeapAllocator_c mShieldAllocator;
    /* 0x3C4 */ void *mpExternalAnmCharBuffer;
    /* 0x3C8 */ nw4r::g3d::ResFile mSwordRes;
    /* 0x3CC */ nw4r::g3d::ResFile mAlinkRes;
    /* 0x3D0 */ nw4r::g3d::ResFile mPlCommonAnimeRes;
    /* 0x3D4 */ nw4r::g3d::ResFile mPlCommonSoundRes;
    /* 0x3D8 */ nw4r::g3d::ResFile mCurrentRes;
    /* 0x3DC */ nw4r::g3d::ResFile mAlink2Res;
    /* 0x3E0 */ daPlBaseMdl_c mMainMdl;
    /* 0x440 */ daPlBaseMainCallback_c mMainBodyCallback;
    /* 0x448 */ m3d::mdl_c mHeadMdl;
    /* 0x46C */ daPlBaseHeadCallback_c mHeadCallback;
    /* 0x474 */ m3d::smdl_c mFaceMdl;
    /* 0x490 */ m3d::anmChr_c mFaceAnmChr;
    /* 0x4C8 */ void *mpAnmCharBuffer;
    /* 0x4CC */ m3d::anmTexPat_c mFaceTexPat;
    /* 0x4F8 */ void *mpTexPatBuffer;
    /* 0x4FC */ m3d::anmTexSrt_c mFaceTexSrt;
    /* 0x528 */ void *mpTexSrtBuffer;
    /* 0x52C */ m3d::anmChr_c mHeadAnmChr;
    /* 0x564 */ m3d::smdl_c mHandsMdl;
    /* 0x580 */ daPlBaseHandsCallback_c mCalcWorldCallback;
    /* 0x588 */ daPlBaseScnObjCallback_c mScnObjCallback;
    /* 0x598 */ m3d::smdl_c mSwordMdl;
    /* 0x5B4 */ m3d::anmMatClr_c mSwordAnmMatClr;
    /* 0x5E0 */ m3d::smdl_c mShieldMdl;
    /* 0x5FC */ m3d::anmTexPat_c mShieldAnmTexPat1;
    /* 0x628 */ m3d::anmTexPat_c mShieldAnmTexPat2;
    /* 0x654 */ m3d::smdl_c mSheathMdl;
    /* 0x670 */ m3d::smdl_c mUnkMdl1;
    /* 0x68C */ m3d::anmChr_c mAnmChr;
    /* 0x6C4 */ m3d::anmChrBlend_c mAnmChrBlend;
    /* 0x6EC */ dScnCallback_c mScnCallback1;
    /* 0x6F8 */ dScnCallback_c mScnCallback2;
    /* 0x704 */ dScnCallback_c mScnCallback3;
    /* 0x710 */ dScnCallback_c mScnCallback4;
    /* 0x71C */ dScnCallback_c mScnCallback5;
    /* 0x728 */ daPlBaseAnmChr_c mAnmChrs[6];
    /* 0x878 */ dCcD_Cyl mCcCyls[3];
    /* 0xC68 */ dCcD_Cps mCcCpss[3];
    /* 0x10B8 */ dCcD_Cyl mCcCyl;
    /* 0x1208 */ u8 field_0x1208[2];
    /* 0x120A */ u8 field_0x120A;
    /* 0x120B */ u8 mTunicType;
    /* 0x120C */ u8 mCurrentAnmChrIdx;
    /* 0x120D */ u8 mShieldPouchSlot;
    /* 0x120E */ s8 mPouchMatId;
    /* 0x120F */ s8 mBeltMatId;
    /* 0x1210 */ s8 mLowbodyMapId;
    /* 0x1211 */ s8 mLowotherMapId;
    /* 0x1212 */ s8 mPouch2MatId;
    /* 0x1213 */ s8 mUpbodyMatId;
    /* 0x1214 */ s8 mUpotherMatId;
    /* 0x1215 */ s8 mBelt2MatId;
    /* 0x1216 */ s8 mCapMatId;
    /* 0x1217 */ s8 mSkinMatId;
    /* 0x1218 */ u8 mSkywardStrikeChargeDuration;
    /* 0x1219 */ u8 field_0x1219;
    /* 0x121A */ u16 mAnimations[6];
    /* 0x1226 */ u16 mFaceAnmChrIdx1;
    /* 0x1228 */ u16 mFaceAnmChrIdx2;
    /* 0x122A */ u16 mFaceAnmTexPatIdx1;
    /* 0x122C */ u16 mFaceAnmTexPatIdx2;
    /* 0x122E */ u16 mFaceAnmTexSrtIdx1;
    /* 0x1230 */ u16 mFaceAnmTexSrtIdx2;
    /* 0x1232 */ u16 mBreakingEffect;
    /* 0x1234 */ mAng mWaistZRot;
    /* 0x1236 */ mAng mWaistYRot;
    /* 0x1238 */ mAng field_0x1238[5];
    /* 0x1242 */ mAng field_0x1242[5];
    /* 0x124C */ u8 _0x124C[0x1256 - 0x124C];
    /* 0x1256 */ mAng field_0x1256;
    /* 0x1258 */ mAng field_0x1258;
    /* 0x125A */ mAng field_0x125A;
    /* 0x125C */ mAng mRSkirtR1Rot;
    /* 0x125E */ mAng mRSkirtL1Rot;
    /* 0x1260 */ mAng mRightHandRotation;
    /* 0x1262 */ mAng mHeadZRot;
    /* 0x1264 */ mAng mHeadYRot;
    /* 0x1266 */ mAng mHeadXRot;
    /* 0x1268 */ mAng field_0x1268;
    /* 0x126A */ mAng field_0x126A;
    /* 0x126C */ mAng field_0x126C;
    /* 0x1270 */ f32 field_0x1270;
    /* 0x1274 */ f32 field_0x1274;
    /* 0x1278 */ f32 field_0x1278;
    /* 0x127C */ mVec3_c field_0x127C;
    /* 0x1288 */ mVec3_c mCenterTranslation;
    /* 0x1294 */ mVec3_c mHeadTranslation;
    /* 0x12A0 */ mVec3_c mPositionAboveLink;
    /* 0x12AC */ mVec3_c mTranslationHand[2];
    /* 0x12C4 */ mVec3_c mTranslationWeapon[2];
    /* 0x12DC */ mVec3_c mToeTranslation[2];
    /* 0x12F4 */ mVec3_c field_0x12F4;
    /* 0x1300 */ mQuat_c field_0x1300[4];
    /* 0x1340 */ mQuat_c mQuat1;
    /* 0x1350 */ mQuat_c mQuat2;
    /* 0x1360 */ dAcRef_c<dAcObjBase_c> mCarriedActorRef; // not sure about the class
    /* 0x136C */ mAng *field_0x136C;
    /* 0x1370 */ mAng *field_0x1370;
    /* 0x1374 */ mAng *field_0x1374; // x3
    /* 0x1378 */ mAng *field_0x1378; // x3
};

#endif
