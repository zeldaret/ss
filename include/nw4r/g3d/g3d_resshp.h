#ifndef NW4R_G3D_RESSHP_H
#define NW4R_G3D_RESSHP_H
#include <GXAttr.h>
#include "types_nw4r.h"
#include "g3d_rescommon.h"
#include "g3d_resmdl.h"
#include "g3d_resvtx.h"

namespace nw4r
{
	namespace g3d
	{
		struct ResTagDLData
		{
			u32 mBufSize; // at 0x0
			u32 mCmdSize; // at 0x4
			u32 mOffset; // at 0x8
		};
		
		struct ResPrePrimDL
		{
			char UNK_0x0[0xA];
			detail::CPCmd CP_CMD_0xA;
			detail::CPCmd CP_CMD_0x10;
			char UNK_0x16[0xA];
			detail::CPCmd CP_CMD_0x20;
			detail::CPCmd CP_CMD_0x26;
			detail::CPCmd CP_CMD_0x2C;
			detail::CPCmd CP_CMD_PAIRS_0x32[GX_POS_MTX_ARRAY - GX_VA_POS][2];
			char UNK_0xC2[0x1E];
		};
		
		struct ResShpData
		{
			char UNK_0x0[0x4];
			u32 mParentOffset; // at 0x4
			char UNK_0x8[0x10];
			ResTagDLData mPrePrimDLTag; // at 0x18
			ResTagDLData mPrimDLTag; // at 0x24
			char UNK_0x30[0x18];
			
			s16 mVtxPosIndex; // at 0x48
			s16 mVtxNrmIndex; // at 0x4a
			s16 mVtxClrIndices[GX_VA_TEX0 - GX_VA_CLR0]; // at 0x4c
			s16 mVtxTexCoordIndices[GX_POS_MTX_ARRAY - GX_VA_TEX0]; // at 0x50
		};
		
		struct ResTagDL
		{
			ResCommon<ResTagDLData> mData;
			
			inline ResTagDL(void * vptr) : mData(vptr) {}
			
			inline u8 * GetDL() const
			{
				return const_cast<u8 *>(mData.ofs_to_ptr<u8>(mData.ref().mOffset));
			}
			
			inline u32 GetBufSize() const
			{
				return mData.ref().mBufSize;
			}
			
			inline u32 GetCmdSize() const
			{
				return mData.ref().mCmdSize;
			}
		};
		
		struct ResShpPrePrim
		{
			ResCommon<ResPrePrimDL> mDL;
			
			inline ResShpPrePrim(void * vptr) : mDL(vptr) {}
			
			inline ResPrePrimDL & ref() const
			{
				return mDL.ref();
			}
		};
		
		struct ResShp
		{
			ResCommon<ResShpData> mShp;
			
			inline ResShp(void * vptr) : mShp(vptr) {}
			bool IsValid() const { return mShp.IsValid(); }

			inline ResShpData & ref() const
			{
				return mShp.ref();
			}
			
			inline ResShpPrePrim GetResShpPrePrim() const
			{
				return ResTagDL(&ref().mPrePrimDLTag).GetDL();
			}
			
			bool GXGetVtxDescv(GXVtxDescList*) const;
			bool GXGetVtxAttrFmtv(GXVtxAttrFmtList*) const;
			
			ResMdl GetParent() const; //inlined
			
			ResVtxPos GetResVtxPos() const;
			ResVtxNrm GetResVtxNrm() const;
			ResVtxClr GetResVtxClr(u32) const;
			ResVtxTexCoord GetResVtxTexCoord(u32) const; //inlined
			
			void GXSetArray(GXAttr, const void *, u8); //inlined
			
			void Init();
			
			void DisableSetArray(GXAttr); //inlined
			void Terminate();
			
			void CallPrePrimitiveDisplayList(bool, bool) const;
			void CallPrimitiveDisplayList(bool) const;
		};
	}
}

#endif