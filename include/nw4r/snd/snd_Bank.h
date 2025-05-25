#ifndef NW4R_SND_BANK_H
#define NW4R_SND_BANK_H

/*******************************************************************************
 * headers
 */

#include "nw4r/snd/snd_BankFile.h" // BankFileReader

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class Channel; }}}
namespace nw4r { namespace snd { namespace detail { struct NoteOnInfo; }}}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b0af4
	class Bank
	{
	// methods
	public:
		// cdtors
		Bank(void const *bankData);
		~Bank();

		// methods
		void SetWaveDataAddress(void const *waveData)
		{
			// specifically not the header variant
			NW4RAssertPointerNonnull_Line(50, waveData);

			mWaveDataAddress = waveData;
		}

		Channel *NoteOn(NoteOnInfo const &noteOnInfo) const;

	// members
	private:
		BankFileReader	mBankReader;				// size 0x0c, offset 0x00
		void			const *mWaveDataAddress;	// size 0x04, offset 0x0c
	}; // size 0x10
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_BANK_H
