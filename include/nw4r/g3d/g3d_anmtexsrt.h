#ifndef NW4R_G3D_ANMTEXSRT_H
#define NW4R_G3D_ANMTEXSRT_H
#include "nw4r/g3d/g3d_restex.h"
#include "nw4r/g3d/g3d_resanmtexsrt.h"

namespace nw4r
{
	namespace g3d
	{

		class AnmObjTexSrtRes;
		struct TexSrtAnmResult;

		class AnmObjTexSrt : public AnmObj {
			public:
				static const G3dObj::TypeObj GetTypeObjStatic() {
					return TypeObj(TYPE_NAME);
				}

				virtual TexSrtAnmResult *GetResult();           // at 0x38
				virtual void Attach(s32, AnmObjTexSrtRes *res); // at 0x3C
				virtual void Detach(s32);                          // at 0x40

			private:
				int mChildrenArraySize;
				u16 *mpChildrenArray;

			NW4R_G3D_TYPE_OBJ_DECL(AnmObjTexSrt);
		};

		class AnmObjTexSrtNode : public AnmObjTexSrt {
			public:
				static const G3dObj::TypeObj GetTypeObjStatic() {
					return TypeObj(TYPE_NAME);
				}


				inline int Size() {
					return mNodeArraySize;
				}

				AnmObjTexSrtRes *GetNode(int i) {
					return mpNodes[i];
				}

			private:
				int mNodeArraySize;
				AnmObjTexSrtRes **mpNodes;

			NW4R_G3D_TYPE_OBJ_DECL(AnmObjTexSrtNode);
		};

		class AnmObjTexSrtRes : public AnmObjTexSrt, public FrameCtrl {
			public:
				static const G3dObj::TypeObj GetTypeObjStatic() {
					return TypeObj(TYPE_NAME);
				}


			static AnmObjTexSrtRes *Construct(MEMAllocator*, u32*, ResAnmTexSrt, ResMdl, bool);
			private:
				ResAnmTexSrt mRes;
				TexSrtAnmResult *mpResultCache;

			NW4R_G3D_TYPE_OBJ_DECL(AnmObjTexSrtRes);
		};

		class AnmObjTexSrtOverride : public AnmObjTexSrtNode {
			public:
				static const G3dObj::TypeObj GetTypeObjStatic() {
					return TypeObj(TYPE_NAME);
				}

			static AnmObjTexSrtOverride *Construct(MEMAllocator*, u32*, ResMdl, int);

			NW4R_G3D_TYPE_OBJ_DECL(AnmObjTexSrtOverride);
		};

		struct TexSrtTypedef
		{
			enum TexMatrixMode
			{
				TEX_MTX_MAYA,
				TEX_MTX_XSI,
				TEX_MTX_3DSMAX
			};
		};

		struct TexSrt
		{
			enum Flag
			{
				FLAG_ANM_EXISTS,
			};

			float mScaleX; // at 0x0
			float mScaleY; // at 0x4
			float mRotation; // at 0x8
			float mTranslationX; // at 0xc
			float mTranslationY; // at 0x10
		};
		
		struct TexSrtAnmResult
		{
			enum
			{
				ANM_COUNT = 8
			};
			
			UNKWORD FLAGS_0x0;
			UNKWORD FLAGS_0x4;
			TexSrt mSrts[ANM_COUNT * 2]; // at 0x8
		};
	}
}

#endif
