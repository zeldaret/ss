#ifndef NW4R_SND_MOVE_VALUE_H
#define NW4R_SND_MOVE_VALUE_H

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x27b93, 0x28fef...
	template <typename TValue, typename TTime>
	class MoveValue
	{
	// methods
	public:
		// cdtors
		MoveValue() :
			mOrigin		(TValue()),
			mTarget		(TValue()),
			mFrame		(TTime ()),
			mCounter	(TTime ())
		{
		}

		// methods
		TValue GetValue() const
		{
			if (IsFinished())
				return mTarget;

			return mOrigin + mCounter * (mTarget - mOrigin) / mFrame;
		}

		void SetTarget(TValue targetValue, TTime frames)
		{
			mOrigin		= GetValue();
			mTarget		= targetValue;
			mFrame		= frames;
			mCounter	= TTime();
		}

		bool IsFinished() const { return mCounter >= mFrame; }

		TTime GetRemainingTime() const {
			return IsFinished() ? 0 : mFrame - mCounter;
		}

		void InitValue(TValue value)
		{
			mOrigin		= value;
			mTarget		= value;
			mFrame		= TTime();
			mCounter	= TTime();
		}

		void Update()
		{
			if (mCounter < mFrame)
				mCounter++;
		}

	// members
	private:
		TValue	mOrigin;	// size TValue, offset 0x00
		TValue	mTarget;	// size TValue, offset TValue
		TTime	mFrame;		// size TTime,  offset TValue * 2
		TTime	mCounter;	// size TTine,  offset TValue * 2 + TTime
	}; // size TValue * 2 + TTime * 2
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_MOVE_VALUE_H
