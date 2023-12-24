#ifndef NW4R_SND_BANK_H
#define NW4R_SND_BANK_H
#include "snd_BankFile.h"
#include "snd_Channel.h"
#include "snd_NoteOn.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct Bank
			{
				Bank(const void *);
				~Bank();
				Channel * NoteOn(const NoteOnInfo &) const;
				
				BankFileReader mFileReader; // at 0x0
				const UNKTYPE * PTR_0xC;
			};
		}
	}
}

#endif