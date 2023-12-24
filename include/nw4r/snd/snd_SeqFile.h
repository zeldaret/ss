#ifndef NW4R_SND_SEQ_FILE_H
#define NW4R_SND_SEQ_FILE_H
#include "types_nw4r.h"
#include "ut_binaryFileFormat.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct SeqFile
			{
				ut::BinaryFileHeader mHeader; // at 0x0
				u32 OFFSET_0x10; // at 0x10
			};
			
			struct SeqFileReader
			{
				struct UNKBLOCK
				{
					ut::BinaryBlockHeader mHeader; // at 0x0
					u32 OFFSET_0x8;
				};
				
				const SeqFile * mFile; // at 0x0
				const UNKBLOCK * PTR_0x4;
				
				bool IsValidFileHeader(const void *);
				SeqFileReader(const void *);
				const void * GetBaseAddress() const;
			};
		}
	}
}

#endif