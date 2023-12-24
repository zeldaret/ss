#ifndef NW4R_SND_WAVE_FILE_H
#define NW4R_SND_WAVE_FILE_H
#include "types_nw4r.h"
#include "ut_binaryFileFormat.h"
#include "snd_Util.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct WaveData
			{
				char UNK_0x0[0x2];
				u8 BYTE_0x2;
				char UNK_0x4[0x90];
			};
			
			struct WaveFile
			{
				struct WaveInfo
				{
					
				};
				
				typedef Util::DataRef<WaveInfo, void, void, void> Region;
			};
			
			struct WaveFileReader
			{
				WaveFileReader(const WaveFile::WaveInfo *);
				bool ReadWaveParam(WaveData *, const void *) const;
			};
		}
	}
}

#endif