#ifndef NW4R_SND_BANK_FILE_H
#define NW4R_SND_BANK_FILE_H
#include "types_nw4r.h"
#include "ut_binaryFileFormat.h"
#include "ut_algorithm.h"
#include "snd_Util.h"
#include "snd_WaveFile.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct InstInfo
			{
				int WORD_0x0;
				u8 mAttack; // at 0x4
				u8 mDecay; // at 0x5
				u8 mSustain; // at 0x6
				u8 mRelease; // at 0x7
				
				u8 BYTE_0x8;
				u8 BYTE_0x9;
				u8 BYTE_0xA;
				float FLOAT_0xC;
			};
			
			struct BankFile
			{
				struct InstParam
				{
					int WORD_0x0;
					u8 mAttack; // at 0x4
					u8 mDecay; // at 0x5
					u8 mSustain; // at 0x6
					u8 mRelease; // at 0x7
					
					char UNK_0x8[0x4];
					
					char BYTE_0xC;
					u8 BYTE_0xD;
					char BYTE_0xE;
					float FLOAT_0x10;
				};
				
				struct RangeTable
				{
					u8 BYTE_0x0;
					u8 BYTES_0x1[];
				};
				
				struct IndexTable
				{
					u8 BYTE_0x0;
					u8 BYTE_0x1;
				};
				
				ut::BinaryFileHeader mHeader; // at 0x0
				u32 OFFSET_0x10;
				char UNK_0x14[0x4];
				u32 OFFSET_0x18;
				
				typedef Util::DataRef<void, InstParam, RangeTable, IndexTable> Region;
			};
			
			struct BankFileReader
			{
				struct UNKBLOCK_DATA
				{
					ut::BinaryBlockHeader mHeader; // at 0x0
					int INT_0x8;
					BankFile::Region ARR_0xC[];
				};
				
				struct UNKBLOCK_WAVE
				{
					ut::BinaryBlockHeader mHeader; // at 0x0
					u32 INT_0x8;
					WaveFile::Region ARR_0xC[];
				};
				
				bool IsValidFileHeader(const void *);
				BankFileReader(const void *);
				bool ReadInstInfo(InstInfo *, int, int, int) const;
				const BankFile::Region * GetReferenceToSubRegion(const BankFile::Region *, int) const;
				bool ReadWaveParam(WaveData *, int, const void *) const;
				
				const BankFile * mFile; // at 0x0
				const UNKBLOCK_DATA * PTR_0x4;
				const UNKBLOCK_WAVE * PTR_0x8;
			};
			
			inline u8 ReadByte(const void * ptr)
			{
				return *static_cast<const u8 *>(ptr);
			}
		}
	}
}

#endif