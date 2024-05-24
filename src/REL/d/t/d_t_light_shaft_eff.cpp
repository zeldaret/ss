#include <d/tg/d_tg.h>

class dTgLightShaftEff_c : public dTg_c {
public:
    dTgLightShaftEff_c() {}
    virtual ~dTgLightShaftEff_c() {}

    static dAcBase_c *construct() {
        return new dTgLightShaftEff_c();
    }
};

dActorInit init_dTgLightShaftEff_c = { dTgLightShaftEff_c::construct, 0x00bd, 0x025e, 0, 0 };
