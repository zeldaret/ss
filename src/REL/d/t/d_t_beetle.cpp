#include "d/t/d_t_beetle.h"

#include "d/d_sc_game.h"
#include "d/a/obj/d_a_obj_base.h"
#include "toBeSorted/area_math.h"
#include "toBeSorted/time_area_mgr.h"
#include "d/flag/sceneflag_manager.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_boomerang.h"

SPECIAL_ACTOR_PROFILE(BEETLE_TAG, dTgBeetle_c, fProfile::BEETLE_TAG, 0x297, 0, 0);


int dTgBeetle_c::create() {

	if (dScGame_c::currentSpawnInfo.stageName == "F103") {//flooded faron woods?
		if (mActorSubtype) {
			mScale.x = mScale.x * 0.01f;
			mScale.y = mScale.y * 0.01f;
			mScale.z = mScale.z * 0.01f;
		}
		dAcObjBase_c::create(fProfile::CO_TEST, mRoomID, 0, &mPosition, &mRotation, &mScale, 0xFFFFFFFF);
		
		return 0x2;
	}
	else {
		if (!mActorSubtype) {//BtlTg
			mZoneType = mParams & 0xF;
			mZoneShape = (mParams >> 4) & 3;
			mUnknown = mParams >> 6;
			mBeetleFlag = mParams >> 0xE;
			mPastOnly = (mParams >> 0x16) & 3;
		}
		else {
			mZoneType = LeafZone;
			if (mActorSubtype == BtlTgA) {//BtlTgA
				mZoneShape = RectangularPrismShape;
			}
			else if (mActorSubtype == BtlTgB) {//BtlTgB
				mZoneShape = CylinderShape;
			}
			else if (mActorSubtype == BtlTgC) {//BtlTgC
				mZoneShape = SphereShape;
			}
		}
		matrixCreateFromPosRotYScale(matrix1, mPosition, mRotation.y, mScale, nullptr, 0);
		return 0x1;
	}
}

u32 dTgBeetle_c::fn_302_210() {//draw?
	return 1;
}

int dTgBeetle_c::actorExecute()  {
	if (mPastOnly == 1 && !mActorSubtype) {
		if (dTimeAreaMgr_c::GetInstance()->checkPositionIsInPastState(mRoomID, mPosition, nullptr, 1.0f) == 0) {
			return 1;
		}
	}

	if (!SceneflagManager::sInstance->checkBoolFlag(mRoomID & 0xFFFF, mBeetleFlag) || mActorSubtype) {

		dAcBoomerang_c* beetlePtr= (dAcBoomerang_c * )dAcPy_c::GetLink()->vt_0x1C0();//boomerang pointer cast required because function currently returns void*
		
		if (mZoneShape == RectangularPrismShape) {//rectangular prism
			if (beetlePtr && checkIfVec3fInMatrix(matrix1, beetlePtr->mPosition)) {
				updateBeetle(*beetlePtr);
			}
		}
		else if (mZoneShape == CylinderShape) {//cylinder
			if (beetlePtr && isWithinCylinder((beetlePtr->mPosition))) {
				updateBeetle(*beetlePtr);
			}
		}
		else if (mZoneShape == SphereShape) {//sphere
			if (beetlePtr && isWithinSphere((beetlePtr->mPosition))) {
				updateBeetle(*beetlePtr);
			}
		}
	}
	return 1;
}

u32 dTgBeetle_c::isWithinSphere(const mVec3_c& position) const {//fn_302_520 is position within a sphere the size of the zone 

	mVec3_c diff = position - (mPosition + mScale.y * mVec3_c::Ey * 1.0f );
	
	diff.x /= mScale.x;
	diff.y /= mScale.y;
	diff.z /= mScale.z;
	
	return diff.mag() <= 1.0f;//is beetle within a certain distance from center
}

u32 dTgBeetle_c::isWithinCylinder(const mVec3_c& position) const {// is position within a really thin cylinder inside the zone
	
	mVec3_c diff = (position - (mPosition + mVec3_c::Ey * mScale.y * 0.5f));

	bool returnValue = 0;
	bool returnCheck = 0;
	
	diff.x /= mScale.x;
	diff.y /= mScale.y;
	diff.z /= mScale.z;
	
	if (EGG::Math<float>::sqrt(diff.squareMagXZ()) <= 0.01f && diff.y <= 0.5f) {
		returnCheck = 1;
	}

	if (returnCheck && diff.y >= -0.5f) {
		returnValue = 1;
	}

	return returnValue;
}

void dTgBeetle_c::updateBeetle(dAcBoomerang_c& boomerang) {//fn_302_620 update beetle based on zone type
	if (mZoneType == LeafZone) {//leaf zone
		return boomerang.setField_0x8CC(0x20);
	}
	else if (mZoneType == BorderZone) {//border zone, reduce flight time
		return boomerang.setRemainingFlightTime(0x3C);
	}
	else if (mZoneType == UnknownZone) {//???
		return boomerang.setField_0x8CC(0x80);
	}
	else {//instant flashing red
		return boomerang.setRemainingFlightTime(-1);
	}
}

u32 dTgBeetle_c::fn_302_3A0() {//also maybe draw?
	return 1;
}
