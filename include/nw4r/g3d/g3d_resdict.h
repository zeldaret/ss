#ifndef NW4R_G3D_RESDICT_H
#define NW4R_G3D_RESDICT_H
#include "types_nw4r.h"
#include "g3d_rescommon.h"
#include <string.h>

namespace nw4r
{
	namespace g3d
	{
		struct ResDicEntry
		{
			u16 INT_0x0; // at 0x0
			u16 INT_0x2; // at 0x2
			u16 unk_index; // at 0x4
			u16 unk_index2; // at 0x6
			u32 INT_0x8; // at 0x8
			u32 INT_0xC; // at 0xC
		};

		struct ResDicData
		{
			u32 mSize; // at 0x0
			u32 mNumData; // at 0x4
			ResDicEntry mEntries[]; // 0x8
		};

		struct ResDic
		{
			ResCommon<ResDicData> mDict;

			ResDicEntry * Get(ResName) const;
			ResDicEntry * Get(const char *, u32) const;
			void * operator[](const char *) const;
			void * operator[](ResName) const;
			s32 GetIndex(ResName) const;
			
			inline ResDic(void * vptr) : mDict(vptr) {}
			
			inline void * operator[](int i) const
			{
				if (mDict.IsValid()) return (void *)mDict.ofs_to_ptr<void>(mDict.ref().mEntries[i + 1].INT_0xC);
				
				return NULL;
			}
			
			inline u32 GetNumData() const
			{
				if (mDict.IsValid()) return mDict.ref().mNumData;
				
				return 0;
			}
		};
	}
}

#endif