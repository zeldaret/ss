#ifndef NW4R_G3D_ANMTEXPAT_H
#define NW4R_G3D_ANMTEXPAT_H
#include "nw4r/g3d/g3d_anmobj.h"
#include "nw4r/g3d/g3d_restex.h"
#include "nw4r/g3d/g3d_resanmtexpat.h"

namespace nw4r
{
	namespace g3d
	{
		struct TexPatAnmResult
		{
			enum
			{
				ANM_COUNT = 8
			};
			
			u8 mTexFlags; // at 0x0
			u8 mPlttFlags; // at 0x1
			ResTex mTexs[ANM_COUNT]; // at 0x4
			ResPltt mPltts[ANM_COUNT]; // at 0x24
		};

		class AnmObjTexPatRes;

		class AnmObjTexPat : public AnmObj {
			public:
				static const G3dObj::TypeObj GetTypeObjStatic() {
					return TypeObj(TYPE_NAME);
				}

				virtual TexPatAnmResult *GetResult();           // at 0x38
				virtual void Attach(s32, AnmObjTexPatRes *res); // at 0x3C
				virtual void Detach(s32);                          // at 0x40

			private:
				int mChildrenArraySize;
				u16 *mpChildrenArray;

			NW4R_G3D_TYPE_OBJ_DECL(AnmObjTexPat);
		};

		class AnmObjTexPatNode : public AnmObjTexPat {
			public:
				static const G3dObj::TypeObj GetTypeObjStatic() {
					return TypeObj(TYPE_NAME);
				}


				inline int Size() {
					return mNodeArraySize;
				}

				AnmObjTexPatRes *GetNode(int i) {
					return mpNodes[i];
				}

			private:
				int mNodeArraySize;
				AnmObjTexPatRes **mpNodes;

			NW4R_G3D_TYPE_OBJ_DECL(AnmObjTexPatNode);
		};

		class AnmObjTexPatRes : public AnmObjTexPat, public FrameCtrl {
			public:
				static const G3dObj::TypeObj GetTypeObjStatic() {
					return TypeObj(TYPE_NAME);
				}


			static AnmObjTexPatRes *Construct(MEMAllocator*, u32*, ResAnmTexPat, ResMdl, bool);
			private:
				ResAnmTexPat mRes;
				TexPatAnmResult *mpResultCache;

			NW4R_G3D_TYPE_OBJ_DECL(AnmObjTexPatRes);
		};

		class AnmObjTexPatOverride : public AnmObjTexPatNode {
			public:
				static const G3dObj::TypeObj GetTypeObjStatic() {
					return TypeObj(TYPE_NAME);
				}

			static AnmObjTexPatOverride *Construct(MEMAllocator*, u32*, ResMdl, int);

			NW4R_G3D_TYPE_OBJ_DECL(AnmObjTexPatOverride);
		};

	}
}

#endif
