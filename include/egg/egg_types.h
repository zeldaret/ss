#ifndef EGG_TYPES_H
#define EGG_TYPES_H

#include "common.h"

// Types pulled from ogws
// May need to remove/add later

namespace EGG {
// gfx
class CapTexture;
class CpuTexture;
class DrawGX;
class DrawPathBase;
class DrawPathBloom;
class DrawPathDOF;
class DrawPathHDR;
class DrawPathLightMap;
class DrawPathShadowVolume;
class Fog;
class FogManager;
class G3DUtility;
class GfxEngine;
class GXUtility;
class IDrawGX;
class IScnProc;
class IScnProcModel;
class LightObj;
class LightManager;
class LightTexture;
class LightTextureManager;
class ModelBoundingInfo;
class ModelEx;
class ModelSnapshot;
class PostEffectBase;
class PostEffectBlur;
class PostEffectBlurGather;
class PostEffectHDR;
class PostEffectSimple;
class ResTIMG;
class ScnRenderer;
class ScnRootEx;
class Screen;
class ScreenEffectBase;
class ShadowTextureManager;
class ShadowTexture;
class StateGX;
class StateGX;
class TextureBuffer;

// math
template <typename T>
class Math;
class Matrix33f;
class Matrix34f;
class Matrix44f;
class Quatf;
class Vector2f;
class Vector3f;

// core
class Allocator;
class Archive;
class AsyncDisplay;
template <typename T>
class IBinary;
template <typename T>
class TBitFlag;
class CntFile;
class ColorFader;
class Decomp;
class Display;
class Disposer;
class DvdFile;
class DvdRipper;
class ExpHeap;
class Fader;
class Heap;
class FrmHeap;
class Scene;
class SceneManager;
class Thread;
class Video;
class Xfb;
class XfbManager;

// audio
class ArcPlayer;
class IAudioMgr;
class SimpleAudioMgr;
class SoundHeapMgr;

// util
class Exception;
class MsgRes;
} // namespace EGG

#endif
