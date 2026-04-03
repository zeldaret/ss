#include "toBeSorted/d_camera_event.h"

#include "common.h"

class dCameraEvent_HIO_c {
public:
    dCameraEvent_HIO_c();
    virtual ~dCameraEvent_HIO_c();

    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0A */ s16 field_0x0A;
};

dCameraEvent_HIO_c::~dCameraEvent_HIO_c() {}
dCameraEvent_HIO_c::dCameraEvent_HIO_c() : field_0x04(0.0f), field_0x08(180.0f), field_0x0A(0) {}

static dCameraEvent_HIO_c sHio;

dCameraEvent_c::dCameraEvent_c()
    : field_0x48(0), field_0x4A(-1), mEventRelated(1, nullptr), field_0xA0(0), field_0xA4(-1), field_0xAC(-1) {}
