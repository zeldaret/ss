#include "nw4r/snd/StrmChannel.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_StrmChannel.cpp
 */

/*******************************************************************************
 * headers
 */

#include <cstring> // std::memset

#include <types.h>

#include "nw4r/ut/inlines.h"
#include "nw4r/ut/Lock.h" // ut::AutoInterruptLock

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

void StrmBufferPool::Setup(void *buffer, u32 size, int blockCount)
{
	if (!blockCount)
		return;

	ut::AutoInterruptLock lock;

	mBuffer		= buffer;
	mBufferSize	= size;
	mBlockSize	= size / blockCount;
	mBlockCount	= blockCount;
	mAllocCount	= 0;
	std::memset(mAllocFlags, 0, sizeof mAllocFlags);

	NW4RAssertMessage_Line(42, mBlockCount <= BLOCK_MAX,
	                       "Too large stream buffer size.");
}

void StrmBufferPool::Shutdown()
{
	ut::AutoInterruptLock lock;

	mBuffer		= nullptr;
	mBufferSize	= 0;
	mBlockSize	= 0;
	mBlockCount	= 0;
}

void *StrmBufferPool::Alloc()
{
	ut::AutoInterruptLock lock;

	if (mAllocCount >= mBlockCount)
		return nullptr;

	int availableByte = ut::RoundUp(mBlockCount, BIT_PER_BYTE) / BIT_PER_BYTE;

	for (int byteIndex = 0; byteIndex < availableByte; byteIndex++)
	{
		byte_t byte = static_cast<byte_t>(mAllocFlags[byteIndex]);

		// All blocks allocated in this flag set
		if (byte == 0xff)
			continue;

		byte_t mask = 1 << 0;

		for (int bitIndex = 0; bitIndex < BIT_PER_BYTE; bitIndex++, mask <<= 1)
		{
			// Block represented by this bit is in use
			if (byte & mask)
				continue;

			mAllocFlags[byteIndex] |= mask;
			mAllocCount++;

			int totalIndex = byteIndex * BIT_PER_BYTE + bitIndex;

			return ut::AddOffsetToPtr(mBuffer, mBlockSize * totalIndex);
		}
	}

	return nullptr;
}

void StrmBufferPool::Free(void *p)
{
	ut::AutoInterruptLock lock;

	s32 offset = ut::GetOffsetFromPtr(mBuffer, p);
	u32 totalIndex = offset / mBlockSize;
	NW4RAssert_Line(92, totalIndex < BLOCK_MAX);

	u32 byteIndex = totalIndex / BIT_PER_BYTE;
	u32 bitIndex = totalIndex % BIT_PER_BYTE;
	int mask = 1 << bitIndex;

	NW4RAssert_Line(97, ( mAllocFlags[ byteIndex ] & mask ) != 0);
	mAllocFlags[byteIndex] &= ~mask;

	mAllocCount--;
	NW4RAssert_Line(100, mAllocCount >= 0);
}

}}} // namespace nw4r::snd::detail
