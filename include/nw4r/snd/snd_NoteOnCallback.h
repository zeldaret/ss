#ifndef NW4R_SND_NOTE_ON_CALLBACK_H
#define NW4R_SND_NOTE_ON_CALLBACK_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_Channel.h>

namespace nw4r {
namespace snd {
namespace detail {

// Forward declarations
class SeqPlayer;

struct NoteOnInfo {
    int prgNo;                                // at 0x0
    int key;                                  // at 0x4
    int velocity;                             // at 0x8
    int length;                               // at 0xC
    int initPan;                              // at 0x10
    int priority;                             // at 0x14
    int voiceOutCount;                        // at 0x18
    Channel::ChannelCallback channelCallback; // at 0x1C
    u32 channelCallbackData;                  // at 0x20
};

class NoteOnCallback {
public:
    virtual ~NoteOnCallback() {} // at 0x8

    virtual Channel* NoteOn(SeqPlayer* pPlayer, int bankNo,
                            const NoteOnInfo& rInfo) = 0; // at 0xC
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
