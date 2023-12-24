#ifndef NW4R_G3D_RESANMTEXSRT_H
#define NW4R_G3D_RESANMTEXSRT_H
#include "g3d_resanm.h"
#include "g3d_resdict.h"

namespace nw4r
{
	namespace g3d
	{
		struct ResAnmTexSrtTexData
		{
			u32 mFlags; // at 0x0
			ResAnmData mAnms[]; // at 0x4
		};
		
		struct ResAnmTexSrtMatData
		{
			char UNK_0x0[0x4];
			UNKWORD FLAGS_0x4;
			UNKWORD FLAGS_0x8;
			s32 mTexOffsets[]; // at 0xc
		};
		
		struct ResAnmTexSrtData
		{
			char UNK_0x0[0x8];
			u32 mRevision; // at 0x8
			char UNK_0xC[0x4];
			s32 mMatDictOffset; // at 0x10
		};

		struct ResAnmTexSrt
		{
			enum
			{
				REVISION = 4
			};
			
			ResCommon<ResAnmTexSrtData> mAnmTexSrt;
			
			inline ResAnmTexSrt(void * vptr) : mAnmTexSrt(vptr) {}
			
			inline ResAnmTexSrtData & ref() const
			{
				return mAnmTexSrt.ref();
			}
			
			inline bool CheckRevision() const
			{
				return mAnmTexSrt.ref().mRevision == REVISION;
			}
			
			inline const ResAnmTexSrtMatData * GetMatAnm(u32 i) const
			{
				return static_cast<const ResAnmTexSrtMatData *>(mAnmTexSrt.ofs_to_obj<ResDic>(ref().mMatDictOffset)[i]);
			}
			
			void GetAnmResult(TexSrtAnmResult *, u32, float) const;
		};
	}
}

#endif