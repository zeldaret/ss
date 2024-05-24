#include <d/tg/d_tg.h>

class dTgTackle_c : public dTg_c {
public:
    dTgTackle_c() {}
    virtual ~dTgTackle_c() {}

    static dAcBase_c *construct() {
        return new dTgTackle_c();
    }
};

dActorInit init_dTgTackle_c = { dTgTackle_c::construct, 0x0210, 0x0241, 0, 0 };
