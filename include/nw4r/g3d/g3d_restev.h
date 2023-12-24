#ifndef NW4R_G3D_RESTEV_H
#define NW4R_G3D_RESTEV_H
#include <GXTev.h>
#include <GXTexture.h>
#include "g3d_cpu.h"
#include "g3d_rescommon.h"

namespace nw4r
{
	namespace g3d
	{
		struct ResTevData
		{
			u32 SIZE_0x0;
			u32 INT_0x4;
			char UNK_0x8[0x4];
			u8 BYTE_0xC;
			char UNK_0xD[0x13];
			
			detail::BPCmd mSwapSelCmds[GX_MAX_TEVSWAP][4];
			
			char UNK_0x70[0x10];
			
			struct
			{
				detail::BPCmd BP_CMD_0x0;
				detail::BPCmd BP_CMD_0x5;
				detail::BPCmd BP_CMD_0xA;
				detail::BPCmd BP_CMD_PAIR_0xF[2];
				char UNK_0x19[0x17];
			} mStageCmds[];//[GX_TEV_STAGE_COUNT];
		};
		
		struct ResTev
		{
			ResCommon<ResTevData> mTev;
			
			inline ResTev(void * vptr) : mTev(vptr) {}
			
			inline ResTevData & ref() const
			{
				return mTev.ref();
			}
			
			bool GXGetTevSwapModeTable(GXTevSwapSel, GXTevColorChan *, GXTevColorChan *, GXTevColorChan *, GXTevColorChan *) const;
			void GXSetTevSwapModeTable(GXTevSwapSel, GXTevColorChan, GXTevColorChan, GXTevColorChan, GXTevColorChan);
			
			bool GXGetTevOrder(GXTevStageID, GXTexCoordID *, GXTexMapID *, GXChannelID *) const;
			
			void GXSetTevColorIn(GXTevStageID, GXTevColorArg, GXTevColorArg, GXTevColorArg, GXTevColorArg);
			
			void CallDisplayList(bool) const;
			
			void * CopyTo(void *);
			
			UNKTYPE DCStore(bool);
		};
	}
}

#endif