#include "toBeSorted/counters/counter.h"

class KeyPieceCounter : public Counter {
public:
    KeyPieceCounter();
    ~KeyPieceCounter() {}
    virtual u16 getMax() const override {
        return 5;
    }
};

KeyPieceCounter KEY_PIECE_COUNTER;

KeyPieceCounter::KeyPieceCounter() : Counter(0x1f9) {}
