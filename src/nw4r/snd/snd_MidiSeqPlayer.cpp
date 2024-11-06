#include "nw4r/snd/snd_SeqPlayer.h"

namespace nw4r {
namespace snd {
namespace detail {

/**
 * Dummy class to instantiate necessary weak functions
 */

class MidiSeqPlayer : public SeqPlayer {
private:
    MidiSeqPlayer();

    // Dummy implementation, must prevent instantiating SeqPlayer version
    virtual void ChannelCallback(Channel* /* pChannel */) {}
};

MidiSeqPlayer::MidiSeqPlayer() {}

} // namespace detail
} // namespace snd
} // namespace nw4r
