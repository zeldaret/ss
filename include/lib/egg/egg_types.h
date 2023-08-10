#pragma once

#include <types.h>

// Types pulled from ogws
// May need to remove/add later

namespace EGG
{
    // gfx
    struct CapTexture;
    struct CpuTexture;
    struct DrawGX;
    struct DrawPathBase;
    struct DrawPathBloom;
    struct DrawPathDOF;
    struct DrawPathHDR;
    struct DrawPathLightMap;
    struct DrawPathShadowVolume;
    struct Fog;
    struct FogManager;
    struct G3DUtility;
    struct GfxEngine;
    struct GXUtility;
    struct IDrawGX;
    struct IScnProc;
    struct IScnProcModel;
    struct LightObj;
    struct LightManager;
    struct LightTexture;
    struct LightTextureManager;
    struct ModelBoundingInfo;
    struct ModelEx;
    struct ModelSnapshot;
    struct PostEffectBase;
    struct PostEffectBlur;
    struct PostEffectBlurGather;
    struct PostEffectHDR;
    struct PostEffectSimple;
    struct ResTIMG;
    struct ScnRenderer;
    struct ScnRootEx;
    struct Screen;
    struct ScreenEffectBase;
    struct ShadowTextureManager;
    struct ShadowTexture;
    struct StateGX;
    struct StateGX;
    struct TextureBuffer;

    // math
    template <typename T> struct Math;
    struct Matrix33f;
    struct Matrix34f;
    struct Matrix44f;
    struct Quatf;
    struct Vector2f;
    struct Vector3f;

    // core
    struct Allocator;
    struct Archive;
    struct AsyncDisplay;
    template <typename T> struct IBinary;
    template <typename T> struct TBitFlag;
    struct CntFile;
    struct ColorFader;
    struct Decomp;
    struct Display;
    struct Disposer;
    struct DvdFile;
    struct DvdRipper;
    struct ExpHeap;
    struct Fader;
    struct FrmHeap;
    struct Heap;
    struct Scene;
    struct SceneManager;
    struct Thread;
    struct Video;
    struct Xfb;
    struct XfbManager;

    // audio
    struct ArcPlayer;
    struct IAudioMgr;
    struct SimpleAudioMgr;
    struct SoundHeapMgr;

    // util
    struct Exception;
    struct MsgRes;
} // namespace EGG
