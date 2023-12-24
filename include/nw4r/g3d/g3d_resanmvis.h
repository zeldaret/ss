#ifndef NW4R_G3D_RESANMVIS_H
#define NW4R_G3D_RESANMVIS_H
#include "g3d_resdict.h"
#include "g3d_resanm.h"
#include "g3d_anmobj.h"

namespace nw4r
{
	namespace g3d
	{
		struct ResAnmVisInfoData
		{
			char UNK_0x0[0x8];
			u16 mNumFrames; // at 0x1C
			u16 mNumNodes;
			AnmPolicy mAnmPolicy; // at 0x20
		};
		
		struct ResAnmVisNodeData
		{
			UNKWORD WORD_0x0;
			u32 mFlags; // at 0x4
			ResBoolAnmFramesData mBoolFrames[]; // at 0x8
		};
		
		struct ResAnmVisData
		{
			char UNK_0x0[0x8];
			u32 mRevision; // at 0x8
			char UNK_0xC[0x4];
			s32 mNodeDictOffset; // at 0x10
			ResAnmVisInfoData mInfo; // at 0x14
		};
		
		struct ResAnmVis
		{
			enum
			{
				REVISION = 3
			};
			
			ResCommon<ResAnmVisData> mAnmVis;
			
			inline ResAnmVis(void * vptr) : mAnmVis(vptr) {}
			bool IsValid() const { return mAnmVis.IsValid(); }

			inline ResAnmVisData & ref() const
			{
				return mAnmVis.ref();
			}
			
			inline bool CheckRevision() const
			{
				return ref().mRevision == REVISION;
			}
			
			inline const ResAnmVisNodeData * GetNodeAnm(u32 i) const
			{
				return static_cast<const ResAnmVisNodeData *>(mAnmVis.ofs_to_obj<ResDic>(ref().mNodeDictOffset)[i]);
			}

			AnmPolicy GetAnmPolicy() const
			{
				return ref().mInfo.mAnmPolicy;
			}

			int GetNumFrame() const
			{
				return ref().mInfo.mNumFrames;
			}

			int GetNumNode() const
			{
				return ref().mInfo.mNumNodes;
			}
			
			bool GetAnmResult(u32, float) const;
		};
	}
}

#endif