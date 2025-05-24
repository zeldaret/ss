#include "nw4r/snd/EnvGenerator.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_EnvGenerator.cpp
 */

/*******************************************************************************
 * headers
 */

#include <types.h>

#include <nw4r/NW4RAssert.hpp>

/*******************************************************************************
 * variables
 */

namespace nw4r { namespace snd { namespace detail
{
	// .rodata

	// clang-format off
	s16 const EnvGenerator::DecibelSquareTable[DECIBEL_SQUARE_TABLE_SIZE] =
	{
		-723, -722, -721, -651, -601, -562, -530, -503,
		-480, -460, -442, -425, -410, -396, -383, -371,
		-360, -349, -339, -330, -321, -313, -305, -297,
		-289, -282, -276, -269, -263, -257, -251, -245,
		-239, -234, -229, -224, -219, -214, -210, -205,
		-201, -196, -192, -188, -184, -180, -176, -173,
		-169, -165, -162, -158, -155, -152, -149, -145,
		-142, -139, -136, -133, -130, -127, -125, -122,
		-119, -116, -114, -111, -109, -106, -103, -101,
		 -99,  -96,  -94,  -91,  -89,  -87,  -85,  -82,
		 -80,  -78,  -76,  -74,  -72,  -70,  -68,  -66,
		 -64,  -62,  -60,  -58,  -56,  -54,  -52,  -50,
		 -49,  -47,  -45,  -43,  -42,  -40,  -38,  -36,
		 -35,  -33,  -31,  -30,  -28,  -27,  -25,  -23,
		 -22,  -20,  -19,  -17,  -16,  -14,  -13,  -11,
		 -10,   -8,   -7,   -6,   -4,   -3,   -1,    0
	};
	// clang-format on

	// .sdata2
	f32 const volatile EnvGenerator::VOLUME_INIT = -90.4f;
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

EnvGenerator::EnvGenerator()
{
	Init(VOLUME_INIT);
}

void EnvGenerator::Init(f32 initDecibel)
{
	SetAttack(ATTACK_INIT);
	SetHold(HOLD_INIT);
	SetDecay(DECAY_INIT);
	SetSustain(SUSTAIN_INIT);
	SetRelease(RELEASE_INIT);
	Reset(initDecibel);
}

void EnvGenerator::Reset(f32 initDecibel)
{
	mValue = initDecibel * 10.0f;
	mStatus = STATUS_ATTACK;
}

f32 EnvGenerator::GetValue() const
{
	if (mStatus == STATUS_ATTACK && mAttack == 0.0f)
		return 0.0f;

	return mValue / 10.0f;
}

void EnvGenerator::Update(int msec)
{
	switch (mStatus)
	{
	case STATUS_ATTACK:
		while (msec > 0)
		{
			mValue *= mAttack;
			msec--;

			if (mValue > -1.0f / 32.0f)
			{
				mValue = 0.0f;
				mStatus = STATUS_HOLD;
				mHoldCounter = mHold;

				break;
			}
		}

		break;

	case STATUS_HOLD:
		if (msec < mHoldCounter)
		{
			mHoldCounter -= msec;
		}
		else
		{
			msec -= mHoldCounter;
			mHoldCounter = 0;
			mStatus = STATUS_DECAY;
		}

		if (mStatus != STATUS_DECAY)
			break;

		/* fallthrough */;

	case STATUS_DECAY:
	{
		f32 sustainDecay = CalcDecibelSquare(mSustain);
		mValue -= mDecay * msec;

		if (mValue < sustainDecay)
		{
			mValue = sustainDecay;
			mStatus = STATUS_SUSTAIN;
		}

		break;
	}

	case STATUS_SUSTAIN:
		break;

	case STATUS_RELEASE:
		mValue -= mRelease * msec;
		break;
	}
}

void EnvGenerator::SetAttack(int attack)
{
	// clang-format off
	static f32 const attackTable[128] =
	{
		0.9992175f, 0.9984326f, 0.9976452f, 0.9968553f,
		0.9960629f, 0.9952679f, 0.9944704f, 0.9936704f,
		0.9928677f, 0.9920625f, 0.9912546f, 0.9904441f,
		0.9896309f, 0.9888151f, 0.9879965f, 0.9871752f,
		0.9863512f, 0.9855244f, 0.9846949f, 0.9838625f,
		0.9830273f, 0.9821893f, 0.9813483f, 0.9805045f,
		0.9796578f, 0.9788081f, 0.9779555f, 0.9770999f,
		0.9762413f, 0.9753797f, 0.9745150f, 0.9736472f,
		0.9727763f, 0.9719023f, 0.9710251f, 0.9701448f,
		0.9692612f, 0.9683744f, 0.9674844f, 0.9665910f,
		0.9656944f, 0.9647944f, 0.9638910f, 0.9629842f,
		0.9620740f, 0.9611604f, 0.9602433f, 0.9593226f,
		0.9583984f, 0.9574706f, 0.9565392f, 0.9556042f,
		0.9546655f, 0.9537231f, 0.9527769f, 0.9518270f,
		0.9508732f, 0.9499157f, 0.9489542f, 0.9479888f,
		0.9470195f, 0.9460462f, 0.9450689f, 0.9440875f,
		0.9431020f, 0.9421124f, 0.9411186f, 0.9401206f,
		0.9391184f, 0.9381118f, 0.9371009f, 0.9360856f,
		0.9350659f, 0.9340417f, 0.9330131f, 0.9319798f,
		0.9309420f, 0.9298995f, 0.9288523f, 0.9278004f,
		0.9267436f, 0.9256821f, 0.9246156f, 0.9235442f,
		0.9224678f, 0.9213864f, 0.9202998f, 0.9192081f,
		0.9181112f, 0.9170091f, 0.9159016f, 0.9147887f,
		0.9136703f, 0.9125465f, 0.9114171f, 0.9102821f,
		0.9091414f, 0.9079949f, 0.9068427f, 0.9056845f,
		0.9045204f, 0.9033502f, 0.9021740f, 0.9009916f,
		0.8998029f, 0.8986080f, 0.8974066f, 0.8961988f,
		0.8949844f, 0.8900599f, 0.8824622f, 0.8759247f,
		0.8691861f, 0.8636406f, 0.8535788f, 0.8430189f,
		0.8286135f, 0.8149099f, 0.8002172f, 0.7780663f,
		0.7554750f, 0.7242125f, 0.6828239f, 0.6329169f,
		0.5592135f, 0.4551411f, 0.3298770f, 0.0000000f
	};
	// clang-format on

	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(263, attack, 0, 127);

	mAttack = attackTable[attack];
}

void EnvGenerator::SetHold(int hold)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(278, hold, 0, 127);

	mHold = ((hold + 1) * (hold + 1)) / 4;
}

void EnvGenerator::SetDecay(int decay)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(294, decay, 0, 127);

	mDecay = CalcRelease(decay);
}

void EnvGenerator::SetSustain(int sustain)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(310, sustain, 0, 127);

	mSustain = sustain;
}

void EnvGenerator::SetRelease(int release)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(326, release, 0, 127);

	mRelease = CalcRelease(release);
}

f32 EnvGenerator::CalcRelease(int release)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(337, release, 0, 127);

	if (release == 127)
		return 65535.0f;

	if (release == 127 - 1)
		return 24.0f;

	if (release < 50)
		return (release * 2 + 1) / 128.0f / 5.0f;
	else
		return 60.0f / (127 - 1 - release) / 5.0f;
}

s16 EnvGenerator::CalcDecibelSquare(int scale)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(352, scale, 0, 127);

	return DecibelSquareTable[scale];
}

}}} // namespace nw4r::snd::detail
