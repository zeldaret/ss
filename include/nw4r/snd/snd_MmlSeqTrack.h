#ifndef NW4R_SND_MML_SEQ_TRACK_H
#define NW4R_SND_MML_SEQ_TRACK_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_MmlParser.h>
#include <nw4r/snd/snd_SeqTrack.h>

namespace nw4r {
namespace snd {
namespace detail {

// Forward declarations
class MmlParser;

class MmlSeqTrack : public SeqTrack {
public:
    struct MmlParserParam {
        bool cmpFlag;                                         // at 0x0
        bool noteWaitFlag;                                    // at 0x1
        bool tieFlag;                                         // at 0x2
        u8 loopCount[MmlParser::CALL_STACK_MAX_DEPTH];        // at 0x3
        u8 callStackDepth;                                    // at 0x6
        const u8* callStack[MmlParser::CALL_STACK_MAX_DEPTH]; // at 0x8
    };

public:
    MmlSeqTrack();
    virtual ParseResult Parse(bool doNoteOn); // at 0xC

    void SetMmlParser(const MmlParser* pParser) {
        mParser = pParser;
    }

    MmlParserParam& GetMmlParserParam() {
        return mMmlParserParam;
    }

private:
    const MmlParser* mParser;       // at 0xC0
    MmlParserParam mMmlParserParam; // at 0xC4
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
