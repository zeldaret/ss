#include <d/tg/d_tg.h>

class dTgMapInst_c : public dTg_c {
public:
    dTgMapInst_c() { }
    virtual ~dTgMapInst_c() {}

    virtual int create() override;
    static dAcBase_c *construct();

    u8 field_0xfc;
};

dAcBase_c *dTgMapInst_c::construct() {
    return new dTgMapInst_c();
}

int dTgMapInst_c::create() {
    field_0xfc = params & 0xff;
    return 1;
}

dActorInit init_dTgMapInst_c = { dTgMapInst_c::construct, 0x025d, 0x021d, 0, 0 };
