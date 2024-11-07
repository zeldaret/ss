#ifndef NW4R_TYPES_H
#define NW4R_TYPES_H

#include "common.h"

#define NW4R_BYTEORDER_BIG 0xFEFF
#define NW4R_BYTEORDER_LITTLE 0xFFFE

#define NW4R_VERSION(major, minor) ((major & 0xFF) << 8 | minor & 0xFF)

namespace nw4r {
namespace ut {
namespace detail {

struct RuntimeTypeInfo;
struct ResFontBase;

} // namespace detail

template <typename T>
struct AutoLock;
struct AutoInterruptLock;
struct BinaryBlockHeader;
struct BinaryFileHeader;
struct CharStrmReader;
struct CharWidths;
struct CharWriter;
struct Color;
struct DvdFileStream;
struct DvdLockedFileStream;
struct FileStream;
struct Font;
struct Glyph;
struct IOStream;
template <typename T, int Ofs>
struct LinkList;
struct List;
struct NandFileStream;
template <typename T>
struct PrintContext;
struct Rect;
struct ResFont;
struct RomFont;
template <typename T>
struct TagProcessorBase;
template <typename T>
struct TextWriterBase;

} // namespace ut

namespace ef {
struct DrawOrder;
struct DrawInfo;
struct DrawStrategy;
struct DrawStrategyImpl;
struct DrawStrategyBuilder;
struct Effect;
struct EffectSystem;
struct EffectProject;
struct Emitter;
struct EmitterDrawSetting;
struct EmitFormBuilder;
struct ResEmitter;
struct EmitterResource;
struct EmitterForm;
struct EmitterInheritSetting;
struct Particle;
struct ParticleManager;
struct MemoryManager;
struct MemoryManagerBase;
} // namespace ef

namespace math {

struct VEC2;
struct VEC3;
struct MTX33;
struct MTX34;
struct MTX44;
struct QUAT;
struct PLANE;
struct AABB;
struct FRUSTUM;

} // namespace math

namespace snd {

struct AxVoice;
struct FxBase;
struct SoundPlayer;
struct SoundHandle;
struct SeqSoundHandle;
struct SoundActor;
struct SoundArchive;
struct SoundArchivePlayer;
struct Sound3DActor;
struct Sound3DListener;
struct Sound3DManager;
struct SoundHandle;
struct StrmSoundHandle;
struct VoiceOutParam;
struct WaveSoundHandle;

namespace detail {

struct AxfxImpl;
struct AxManager;
struct AxVoiceManager;
struct ExternalSoundPlayer;

struct BasicSound;
struct BasicPlayer;
struct ChannelManager;
struct SeqSound;
struct SeqPlayer;
struct MmlParser;
struct MmlSeqTrack;
struct SeqTrack;
struct SeqTrackAllocator;
template <typename T>
struct SoundInstanceManager;
struct StrmBufferPool;
struct StrmSound;
struct NoteOnCallback;
struct PlayerHeap;
struct SoundArchiveFileReader;
struct SoundThread;
struct WaveSound;
struct StrmPlayer;
struct WsdPlayer;
} // namespace detail
} // namespace snd

namespace g3d {
struct AnmObjVis;
struct AnmObjVisNode;
struct AnmObjVisOR;
struct AnmObjVisRes;
struct AnmObj;
struct AnmObjChr;
struct FuncObjCalcWorld;
struct AnmScn;
struct AnmScnRes;
struct CameraAnmResult;
struct Draw1Mat1ShpSwap;
struct DrawResMdlReplacement;
struct FogAnmResult;
struct ResFile;
struct ResMdl;
struct ResPltt;
struct ResTex;
struct ResTexSrt;
struct ResMat;
struct ResTexPlttInfo;
struct ResShp;
struct ResFog;
struct ResAnmChr;
struct ResAnmVis;
struct ResAnmClr;
struct ResAnmTexPat;
struct ResAnmTexSrt;
struct ResAnmShp;
struct ResAnmScn;
struct ResAnmFog;
struct ResAnmCamera;
struct ResVtxPos;
struct ResVtxNrm;
struct ResVtxClr;
struct ResVtxTexCoord;
struct ClrAnmResult;
struct TexPatAnmResult;
struct TexSrtAnmResult;
struct ScnMdl1Mat1Shp;
struct TexSrt;
struct TexSrtTypedef;
struct LightObj;
struct LightAnmResult;
struct LightSet;
struct LightSetting;
struct LightSetData;
struct AmbLightObj;
struct AmbLightAnmResult;
struct Fog;
struct ScnRoot;
struct ScnProc;
struct ScnLeaf;
struct ScnObj;
struct ScnRfl;
struct ScnGroup;
struct ScnMdlSimple;
struct ScnMdl;
struct IScnObjGather;
struct IScnObjCallback;

namespace G3DState {
struct IndMtxOp;
}
} // namespace g3d

namespace lyt {
struct Size;

namespace detail {
struct PaneBase;
struct TexCoordAry;
} // namespace detail

namespace res {
struct Group;
struct Pane;
struct Bounding;
struct AnimationBlock;
struct BinaryFileHeader;
} // namespace res

struct Group;
struct GroupContainer;
struct Pane;
struct DrawInfo;
struct AnimResource;
struct AnimTransform;
struct AnimTransformBasic;
struct AnimationLink;
struct ResourceAccessor;
struct ArcResourceAccessor;
struct FontRefLink;
struct Material;
struct Layout;
struct Bounding;
struct ResBlockSet;
struct TexMap;
} // namespace lyt
} // namespace nw4r

#endif
