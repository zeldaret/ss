#include "nw4r/snd/snd_Lfo.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_Lfo.cpp
 */

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

void LfoParam::Init()
{
	depth	= 0.0f;
	range	= 1;
	speed	= 6.25f;
	delay	= 0;
}

void Lfo::Reset()
{
	mCounter		= 0.0f;
	mDelayCounter	= 0;
}

void Lfo::Update(int msec)
{
	if (mDelayCounter < mParam.delay)
	{
		if (mDelayCounter + msec <= mParam.delay)
		{
			mDelayCounter += msec;
			return;
		}

		msec -= mParam.delay - mDelayCounter;
		mDelayCounter = mParam.delay;
	}

	mCounter += mParam.speed * msec / 1000;
	mCounter -= static_cast<int>(mCounter);
}

f32 Lfo::GetValue() const
{
	if (mParam.depth == 0.0f)
		return 0.0f;

	if (mDelayCounter < mParam.delay)
		return 0.0f;

	f32 value = GetSinIdx(4 * (TABLE_SIZE * mCounter))
	          / static_cast<f32>(TABLE_SIZE * 4 - 1);

	value *= mParam.depth;
	value *= mParam.range;

	return value;
}

s8 Lfo::GetSinIdx(int index)
{
	static s8 const sinTable[TABLE_SIZE + 1] =
	{
		0,   6,   12,  19,  25,  31,  37,  43,  49,  54,  60,
		65,  71,  76,  81,  85,  90,  94,  98,  102, 106, 109,
		112, 115, 117, 120, 122, 123, 125, 126, 126, 127, 127
	};

	// specifically not the source variant
	NW4RAssertHeaderClampedLValue_Line(123, index, 0, 128);

	if (index < TABLE_SIZE)
		return sinTable[index];
	else if (index < TABLE_SIZE * 2)
		return sinTable[TABLE_SIZE - (index - TABLE_SIZE)];
	else if (index < TABLE_SIZE * 3)
		return -sinTable[index - TABLE_SIZE * 2];
	else
		return -sinTable[TABLE_SIZE - (index - TABLE_SIZE * 3)];
}

}}} // namespace nw4r::snd::detail
