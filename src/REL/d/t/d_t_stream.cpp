#include "d/t/d_t_stream.h"

#include "d/a/d_a_base.h"
#include "f/f_base.h"
#include "f/f_list_mg.h"
#include "toBeSorted/area_math.h"

SPECIAL_ACTOR_PROFILE(TAG_STREAM, dTgStream_c, fProfile::TAG_STREAM, 0x29D, 0, 0);

void dTgStream_c::addToList(fLiMgBa_c &list) {
    // Upcasting for simplicity of the mOrder check.
    dAcRef_c<dTgStream_c> *pIt = static_cast<dAcRef_c<dTgStream_c> *>(list.getFirst());
    if (pIt != nullptr) {
        while (pIt != nullptr) {
            if (pIt->get()->mOrder <= mOrder) {
                pIt = pIt->getNext();
                continue;
            }

            list.insertAfter(&mStreamNode, pIt->getPrev());
            return;
        }
        list.append(&mStreamNode);
    }

    else {
        list.append(&mStreamNode);
    }
}

int dTgStream_c::create() {
    mSceneflag = getFromParams(0, 0xFF);
    mAreaIdx = getFromParams(8, 0xFF);
    mSpeed = getFromParams(16, 0xFF);
    if (mSpeed == 0xFF) {
        mSpeed = 0;
    }

    mOrder = getFromParams(28, 0xFF);
    mIsSand = getFromParams(24, 0xF);
    field_0x101 = (mRotation.z & 0x3) == 1;
    mRotation.z = 0;

    if (mIsSand == 1) {
        addToList(sSandStreamList);
    } else {
        addToList(sStreamList);
    }

    matrixCreateFromPosRotYScale(mArea, mPosition, mRotation.y, mScale, nullptr, 0.f);

    return SUCCEEDED;
}

int dTgStream_c::doDelete() {
    if (mIsSand == 1) {
        sSandStreamList.remove(&mStreamNode);
    } else {
        sStreamList.remove(&mStreamNode);
    }
    return SUCCEEDED;
}

int dTgStream_c::actorExecute() {
    return SUCCEEDED;
}
