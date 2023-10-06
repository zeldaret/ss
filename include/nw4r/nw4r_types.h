#pragma once

#include "types.h"

namespace nw4r
{
	namespace ut
	{
		template <typename T> struct TextWriterBase;
		template <typename T> struct TagProcessorBase;

		struct Color;
		struct FileStream;

		namespace detail
		{
			struct RuntimeTypeInfo;
		}
	}
	
	namespace ef
	{
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
	}
	
	namespace math
	{
		class VEC2;
		class VEC3;
		class MTX33;
		class MTX34;
		class AABB;
		class FRUSTUM;
	}
	
	namespace snd
	{
		struct FxBase;
		struct SoundPlayer;
		struct SoundHandle;
		struct SeqSoundHandle;
		struct SoundArchive;
		struct SoundArchivePlayer;
		struct Sound3DActor;
		struct Sound3DListener;
		struct Sound3DManager;
		struct SoundHandle;
		struct StrmSoundHandle;
		struct WaveSoundHandle;

		namespace detail
		{
			struct BasicSound;
			struct BasicPlayer;
			struct ChannelManager;
			struct SeqSound;
			struct SeqPlayer;
			struct MmlParser;
			struct MmlSeqTrack;
			struct SeqTrack;
			struct SeqTrackAllocator;
			template <typename T> struct SoundInstanceManager;
			struct StrmBufferPool;
			struct StrmSound;
			struct NoteOnCallback;
			struct PlayerHeap;
			struct SoundArchiveFileReader;
			struct SoundThread;
			struct WaveSound;
			struct StrmPlayer;
			struct WsdPlayer;
		}
	}
	
	namespace g3d
	{
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

		namespace G3DState
		{
			struct IndMtxOp;
		}
	}

	namespace lyt
	{
		struct Size;

		namespace detail
		{
			struct PaneBase;
			struct TexCoordAry;
		}

		namespace res
		{
			struct Group;
			struct Pane;
			struct Bounding;
			struct AnimationBlock;
			struct BinaryFileHeader;
		}

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
	}
}
