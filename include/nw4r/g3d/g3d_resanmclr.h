#ifndef NW4R_G3D_RESANMCLR_H
#define NW4R_G3D_RESANMCLR_H
#include "nw4r/g3d/g3d_resdict.h"
#include "nw4r/g3d/g3d_resanm.h"
#include "nw4r/g3d/g3d_anmclr.h"

namespace nw4r
{
	namespace g3d
	{
		struct ResAnmClrInfoData
		{
			char UNK_0x0[0x8];
			u16 mNumFrames;
		};
		
		struct ResAnmClrMatData
		{
			char UNK_0x0[0x4];
			u32 mFlags; // at 0x4
			
			struct AnmData
			{
				UNKWORD WORD_0x0;
				ResColorAnmData mColor; // at 0x4
			} mAnms[]; // at 0x8
		};
		
		struct ResAnmClrData
		{
			char UNK_0x0[0x8];
			u32 mRevision; // at 0x8
			char UNK_0xC[0x4];
			s32 mMatDictOffset; // at 0x10
			ResAnmClrInfoData mInfo; // at 0x14
			char UNK_0x1E[0x2];
			u16 mNumFrames; // at 0x20
			AnmPolicy mAnmPolicy; // at 0x24
		};
		
		struct ResAnmClr
		{
			enum
			{
				REVISION = 3
			};
			
			ResCommon<ResAnmClrData> mAnmClr;
			
			inline ResAnmClr(void * vptr) : mAnmClr(vptr) {}
			
			inline ResAnmClrData & ref() const
			{
				return mAnmClr.ref();
			}
			
			inline bool CheckRevision() const
			{
				return ref().mRevision == REVISION;
			}

			AnmPolicy GetAnmPolicy() const
			{
				return ref().mAnmPolicy;
			}

			int GetNumFrame() const
			{
				return ref().mNumFrames;
			}
			
			inline const ResAnmClrMatData * GetMatAnm(u32 i) const
			{
				return static_cast<const ResAnmClrMatData *>(mAnmClr.ofs_to_obj<ResDic>(ref().mMatDictOffset)[i]);
			}
			
			void GetAnmResult(ClrAnmResult *, u32, float) const;
		};
	}
}

#endif
