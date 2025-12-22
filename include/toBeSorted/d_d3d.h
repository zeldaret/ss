#ifndef D_D3D_H
#define D_D3D_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "egg/egg_types.h"
#include "m/m3d/m_anmmdl.h"
#include "m/m3d/m_fanm.h"
#include "m/m3d/m_mdl.h"
#include "m/m3d/m_proc.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "toBeSorted/tlist.h"

#include <cstddef>

namespace d3d {

bool create(EGG::Heap *);

f32 worldToScreen(mVec3_c &result, const mVec3_c &worldPos);
bool isOnScreen(const mVec3_c &worldPos);
void screenToWorld(mVec3_c &result, const mVec3_c &screenPos);
void createLightTextures();

class AnmMdlWrapper : public m3d::mdlAnmChr {
public:
    AnmMdlWrapper();
    virtual ~AnmMdlWrapper();

    virtual void play() override;
    virtual bool setAnm(const char *name, m3d::playMode_e mode, f32) override;
    virtual bool setAnm(const char *name, m3d::playMode_e mode) override {
        return setAnm(name, mode, 0.0f);
    }
    virtual void setFrame(f32 frame) override;
    virtual void setRate(f32 rate) override;

    // Base creation function, different names in different files
    /* 0x20 */ virtual bool
    create(dAcBase_c &ac, void *mdlFile, void *anmFile, const char *mdlName, const char *anmName, u32 bufferOption);
    // Unknown overloads
    /* 0x24 */ virtual bool
    create2(dAcBase_c &ac, void *mdlFile, void *anmFile, const char *mdlName, const char *anmName, u32 bufferOption);
    /* 0x28 */ virtual bool
    create3(dAcBase_c &ac, void *mdlFile, void *anmFile, const char *mdlName, const char *anmName, u32 bufferOption) {
        return create2(ac, mdlFile, anmFile, mdlName, anmName, bufferOption);
    }

    // Create with a callback
    /* 0x2C */ virtual bool create(
        dAcBase_c &ac, void *mdlFile, void *anmFile, const char *mdlName, const char *anmName,
        m3d::mdl_c::mdlCallback_c *callback, u32 bufferOption
    );
    // Unknown overloads
    /* 0x30 */ virtual bool create2(
        dAcBase_c &ac, void *mdlFile, void *anmFile, const char *mdlName, const char *anmName,
        m3d::mdl_c::mdlCallback_c *callback, u32 bufferOption
    );
    /* 0x34 */ virtual bool create3(
        dAcBase_c &ac, void *mdlFile, void *anmFile, const char *mdlName, const char *anmName,
        m3d::mdl_c::mdlCallback_c *callback, u32 bufferOption
    );

    // Create with same file name for both model and anim
    /* 0x38 */ virtual bool
    create(dAcBase_c &ac, void *resFile, const char *mdlName, const char *anmName, u32 bufferOption);

    // Unknown overloads
    /* 0x3C */ virtual bool
    create2(dAcBase_c &ac, void *resFile, const char *mdlName, const char *anmName, u32 bufferOption);
    /* 0x40 */ virtual bool
    create3(dAcBase_c &ac, void *resFile, const char *mdlName, const char *anmName, u32 bufferOption) {
        return create2(ac, resFile, mdlName, anmName, bufferOption);
    }

    // Create with a callback, same file name for both model and anim
    /* 0x44 */ virtual bool create(
        dAcBase_c &ac, void *mdlFile, const char *mdlName, const char *anmName, m3d::mdl_c::mdlCallback_c *callback,
        u32 bufferOption
    );
    // Unknown overloads
    /* 0x48 */ virtual bool create2(
        dAcBase_c &ac, void *mdlFile, const char *mdlName, const char *anmName, m3d::mdl_c::mdlCallback_c *callback,
        u32 bufferOption
    );
    /* 0x4C */ virtual bool create3(
        dAcBase_c &ac, void *mdlFile, const char *mdlName, const char *anmName, m3d::mdl_c::mdlCallback_c *callback,
        u32 bufferOption
    ) {
        return create2(ac, mdlFile, mdlName, anmName, callback, bufferOption);
    }

    void setSoundSource(dSoundSourceIf_c *pSource);

protected:
    bool loadSounds(const char *name);
    void syncAnmFrame();
    void setSoundRate(f32 rate);

    /* 0x68 */ dSoundSourceIf_c *mpSoundSource;
    /* 0x6C */ void *mpSoundData;
};

class AnmMdlWrapper2 : public AnmMdlWrapper {
public:
    bool createExt(
        dAcBase_c &ac, void *mdlFile, void *anmFile, void *ext1, void *ext2, const char *mdlName, const char *anmName,
        m3d::mdl_c::mdlCallback_c *callback, u32 bufferOption
    );
    virtual bool setAnm(const char *name, m3d::playMode_e mode, f32) override;

private:
    /* 0x70 */ nw4r::g3d::ResFile mExt1;
    /* 0x74 */ nw4r::g3d::ResFile mExt2;
    /* 0x78 */ bool mHasExt1;
    /* 0x79 */ bool mHasExt2;
};

struct SomeListNode {
    /* 0x00 */ u8 _0x00[4];
    /* 0x04 */ u16 field_0x04;
    /* 0x06 */ u16 field_0x06;
    /* 0x08 */ u8 _0x08[4];
    /* 0x0C */ TListNode<SomeListNode> mNode;
};

typedef TList<SomeListNode, offsetof(SomeListNode, mNode)> SomeList;

struct SomeListStruct {
    SomeList list;
};

void setRoomTevColors(nw4r::g3d::ResMdl mdl, int b1, bool bUnk);

struct UnkWithWaterNode {
public:
    UnkWithWaterNode(nw4r::g3d::ResTexObj &obj, GXTexMapID mapId) : mObj(obj), mMapId(mapId) {}

    void apply(const GXTexObj *obj);

    /* 0x00 */ TListNode<UnkWithWaterNode> mNode;
    /* 0x08 */ nw4r::g3d::ResTexObj mObj;
    /* 0x0C */ GXTexMapID mMapId;
};

class UnkWithWaterBase {
public:
    UnkWithWaterBase() : field_0x04(0), field_0x08(0) {}
    virtual void calc(EGG::CpuTexture *tex) = 0;

private:
    /* 0x04 */ UNKWORD field_0x04;
    /* 0x08 */ UNKWORD field_0x08;
};

class UnkWithWater : public UnkWithWaterBase {
public:
    typedef TList<UnkWithWaterNode, offsetof(UnkWithWaterNode, mNode)> NodeList;
    UnkWithWater() {}
    virtual ~UnkWithWater() {
        remove();
    }
    virtual void calc(EGG::CpuTexture *tex) override;

    bool init(nw4r::g3d::ResTexObj &obj, GXTexMapID mapId);

    static bool linkMdl(nw4r::g3d::ResMdl &mdl, UnkWithWater *thing);
    bool hasLinks() const {
        return mList.mCount != 0;
    }

private:
    void remove();

    /* 0x0C */ NodeList mList;
};

class UnkProc : public m3d::proc_c {
public:
    bool create(int prioOpa, int prioXlu, mAllocator_c *alloc);
    bool createXlu(int prioXlu, mAllocator_c *alloc);

private:
};

class EggTextureProc : public UnkProc {
public:
    EggTextureProc() : mpCpuTexture(nullptr) {}
    virtual ~EggTextureProc() {
        doRemove();
    }

    virtual int entry() override;

    virtual void drawOpa() override {
        doDraw();
    }

    virtual void drawXlu() override {
        doDraw();
    }

private:
    void doRemove();
    void doDraw();

    /* 0x18 */ EGG::TextureBuffer *mpCpuTexture;
    /* 0x1C */ mMtx_c mMtx;
    /* 0x4C */ UNKWORD _0x4C;
    /* 0x50 */ u8 field_0x50;
    /* 0x51 */ u8 field_0x51;
    /* 0x52 */ u8 field_0x52;
};

class XluProc : public UnkProc {
public:
    virtual void drawXlu() override;

private:
    /* 0x18 */ mColor mClr;
};

} // end namespace d3d

#endif
