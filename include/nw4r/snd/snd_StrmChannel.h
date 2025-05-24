#ifndef NW4R_SND_STRM_CHANNEL_H
#define NW4R_SND_STRM_CHANNEL_H

/*******************************************************************************
 * headers
 */

#include <climits> // CHAR_BIT

#include <macros.h> // ROUND_UP
#include <types.h>

#include "adpcm.h"

/*******************************************************************************
 * types
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f927
	struct StrmChannel
	{
		void			*bufferAddress;	// size 0x04, offset 0x00
		AdpcmParam		adpcmParam;		// size 0x28, offset 0x04
		AdpcmLoopParam	adpcmLoopParam;	// size 0x06, offset 0x2c
		u16				adpcmPredScale;	// size 0x02, offset 0x32
	}; // size 0x34
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f7c6
	class StrmBufferPool
	{
	// methods
	public:
		// methods
		void Setup(void *buffer, u32 size, int blockCount);
		void Shutdown();

		u32 GetBlockSize() const { return mBlockSize; }

		void *Alloc();
		void Free(void *p);

	// static members
	private:
		static int const BIT_PER_BYTE = CHAR_BIT; // uhhh ok
		static int const BLOCK_MAX = 32;

	// members
	private:
		void	*mBuffer;														// size 0x04, offset 0x00
		u32		mBufferSize;													// size 0x04, offset 0x04
		u32		mBlockSize;														// size 0x04, offset 0x08
		int		mBlockCount;													// size 0x04, offset 0x0c
		int		mAllocCount;													// size 0x04, offset 0x10
		byte_t	mAllocFlags[ROUND_UP(BLOCK_MAX, BIT_PER_BYTE) / BIT_PER_BYTE];	// size 0x04, offset 0x14
	}; // size 0x18
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_STRM_CHANNEL_H
