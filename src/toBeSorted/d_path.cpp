#include "toBeSorted/d_path.h"

#include "common.h"
#include "d/col/c/c_m3d.h"
#include "d/d_bzs_types.h"
#include "d/d_room.h"
#include "d/d_stage.h"
#include "egg/math/eggMath.h"
#include "m/m_vec.h"
#include "nw4r/math/math_arithmetic.h"
#include "rvl/MTX/mtx.h"

// BPNT = B-Spline Point?

dPath_c::dPath_c() {
    clear();
}

dPath_c::~dPath_c() {}

void dPath_c::clear() {
    mpPathPtr = nullptr;
    mRoomIndex = -1;
    mPathSubtype = 0;
}

const PNT *dPath_c::getPathPoint(s32 pointIndex) const {
    if (mpPathPtr == nullptr || pointIndex < 0 ||
        (pointIndex >= mpPathPtr->pointCount || !isLinear())) {
        return nullptr;
    }
    dRoom_c *room = dStage_c::GetInstance()->getRoom(mRoomIndex);
    if (mPathSubtype != 0) {
        return &room->getSpnt(mpPathPtr->pointStartIndex)[pointIndex];
    }
    return &room->getPnt(mpPathPtr->pointStartIndex)[pointIndex];
}

const BPNT *dPath_c::getPathBpoint(s32 pointIndex) const {
    if (mpPathPtr == nullptr || pointIndex < 0 ||
        (pointIndex >= mpPathPtr->pointCount || isLinear())) {
        return nullptr;
    }
    dRoom_c *room = dStage_c::GetInstance()->getRoom(mRoomIndex);
    if (mPathSubtype != 0) {
        return &room->getSbpt(mpPathPtr->pointStartIndex)[pointIndex];
    }
    return &room->getBpnt(mpPathPtr->pointStartIndex)[pointIndex];
}

const Vec *dPath_c::getPoint(s32 pointIndex) const {
    if (isLinear()) {
        const PNT *result = getPathPoint(pointIndex);
        if (result != nullptr) {
            return &result->position;
        }
        return nullptr;
    } else {
        const BPNT *result = getPathBpoint(pointIndex);
        if (result != nullptr) {
            return &result->position;
        }
        return nullptr;
    }
}

void dPath_c::getPoint(s32 pointIndex, f64 time, mVec3_c &out) const {
    if (isLinear()) {
        // Linear path
        const Vec *pnt = getPoint(pointIndex);
        const Vec *next;
        if (pointIndex == mpPathPtr->pointCount - 1) {
            if (isWrapping()) {
                // Last point but wrap around, next is 0 again
                next = getPoint(0);
            } else {
                // We are at the end and no wrap - simply return the end position
                out.x = pnt->x;
                out.y = pnt->y;
                out.z = pnt->z;
                return;
            }
        } else {
            // Regular next point
            next = getPoint(pointIndex + 1);
        }
        // Linearly interpolate
        mVec3_c tmp;
        VECSubtract(next, pnt, tmp);
        VECScale(tmp, tmp, time);
        VECAdd(tmp, pnt, out);
    } else {
        // B-Spline?
        const BPNT *pnt = getPathBpoint(pointIndex);
        const Vec *nextPos;
        const Vec *nextCtrl;
        if (pointIndex == mpPathPtr->pointCount - 1) {
            if (isWrapping()) {
                // Last point but wrap around, next is 0 again
                const BPNT *next = getPathBpoint(0);
                nextPos = &next->position;
                nextCtrl = &next->control1;
            } else {
                // We are at the end and no wrap - simply return the end position
                out.x = pnt->position.x;
                out.y = pnt->position.y;
                out.z = pnt->position.z;
                return;
            }
        } else {
            // Regular next point
            const BPNT *next = getPathBpoint(pointIndex + 1);
            nextPos = &next->position;
            nextCtrl = &next->control1;
        }

        if (pnt->position.x == pnt->control2.x && pnt->position.y == pnt->control2.y &&
            pnt->position.z == pnt->control2.z && nextPos->x == nextCtrl->x && nextPos->y == nextCtrl->y &&
            nextPos->z == nextCtrl->z) {
            // Point is unaffected by control points, so linearly interpolate
            mVec3_c tmp;
            VECSubtract(nextPos, &pnt->position, tmp);
            VECScale(tmp, tmp, time);
            VECAdd(tmp, &pnt->position, out);
        } else {
            // Cubic B-Spline?
            f32 f = time;
            f32 f1 = 1.0f - f;
            f32 fISq = f1 * f1;
            f32 fSq = f * f;

            f32 f3 = fISq * f1;
            f32 f5 = 3.0f * fISq * f;
            f32 f4 = 3.0f * f1 * fSq;
            f32 f2 = fSq * f;

            out.x = f3 * pnt->position.x + f5 * pnt->control2.x + f4 * nextCtrl->x + f2 * nextPos->x;
            out.y = f3 * pnt->position.y + f5 * pnt->control2.y + f4 * nextCtrl->y + f2 * nextPos->y;
            out.z = f3 * pnt->position.z + f5 * pnt->control2.z + f4 * nextCtrl->z + f2 * nextPos->z;
        }
    }
}

u8 dPath_c::getPointParam(s32 pointIndex, s32 param) const {
    if (isLinear()) {
        const PNT *result = getPathPoint(pointIndex);
        if (result != nullptr) {
            return result->params[param];
        }
        return 0;
    } else {
        const BPNT *result = getPathBpoint(pointIndex);
        if (result != nullptr) {
            return result->params[param];
        }
        return 0;
    }
}

bool dPath_c::initWithPathId(s32 pathId, s32 roomId, bool pathSubtype) {
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomId);
    s32 pathIndex = -1;
    if (!pathSubtype) {
        for (s32 i = 0; i < room->getPathCount(); i++) {
            if (room->getPath(i)->pathId == pathId) {
                pathIndex = i;
                break;
            }
        }
    } else {
        for (s32 i = 0; i < room->getSpthCount(); i++) {
            if (room->getSpth(i)->pathId == pathId) {
                pathIndex = i;
                break;
            }
        }
    }

    if (pathIndex == -1) {
        return false;
    }
    return initWithPathIndex(pathIndex, roomId, pathSubtype);
}

void dPath_c::set(const PATH *path, s32 roomIndex, bool subtype) {
    mpPathPtr = path;
    mRoomIndex = roomIndex;
    mPathSubtype = subtype;
}

static s32 getPathCount(dRoom_c *room, bool isSpth) {
    s32 ret = room->getPathCount();
    if (isSpth) {
        ret = room->getSpthCount();
    }
    return ret;
}

bool dPath_c::initWithPathIndex(s32 pathIndex, s32 roomId, bool pathSubtype) {
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomId);
    s32 pathCount = getPathCount(room, pathSubtype);
    // @bug ineffective null check
    if (room == nullptr || pathIndex < 0 || pathIndex >= pathCount) {
        clear();
        return false;
    } else {
        if (pathSubtype) {
            set(room->getSpth(pathIndex), roomId, pathSubtype);
        } else {
            set(room->getPath(pathIndex), roomId, pathSubtype);
        }
        return true;
    }
}

u16 dPath_c::getMyPathIndex(s32 roomId) const {
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomId);
    const PATH *base = room->getPath(0);
    if (mPathSubtype) {
        base = room->getSpth(0);
    }
    return (mpPathPtr - base);
}

bool dPath_c::getNextPath(dPath_c *next) const {
    if (mpPathPtr->getNextId() == 0xFF) {
        next->clear();
        return false;
    }
    dRoom_c *room = dStage_c::GetInstance()->getRoom(mRoomIndex);
    u8 nextId = mpPathPtr->getNextId();
    if (mPathSubtype) {
        next->set(room->getSpth(nextId), mRoomIndex, mPathSubtype);
    } else {
        next->set(room->getPath(nextId), mRoomIndex, mPathSubtype);
    }
    return true;
}

bool dPath_c::isLinearSegment(s32 pointIndex) const {
    if (isLinear()) {
        // Non-spline paths are always linear
        return true;
    }
    const BPNT *pnt = getPathBpoint(pointIndex);
    mVec3_c pos;
    mVec3_c control;
    mVec3_c nextCtrl;
    mVec3_c nextPos;
    if (pointIndex == mpPathPtr->pointCount - 1) {
        if (isWrapping()) {
            // Last point but wrap around, next is 0 again
            const BPNT *next = getPathBpoint(0);
            nextPos.copyFrom(&next->position);
            nextCtrl.copyFrom(&next->control1);
        } else {
            return false;
        }
    } else {
        // Regular next point
        const BPNT *next = getPathBpoint(pointIndex + 1);
        nextPos.copyFrom(&next->position);
        nextCtrl.copyFrom(&next->control1);
    }

    pos.copyFrom(&pnt->position);
    control.copyFrom(&pnt->control2);

    // Spline paths are trivially linear if the respective control points
    // are at the same position
    if (pos.x == control.x && pos.y == control.y && pos.z == control.z && nextCtrl.x == nextPos.x &&
        nextCtrl.y == nextPos.y && nextCtrl.z == nextPos.z) {
        return true;
    }
    return false;
}

f32 dPath_c::getDistanceMovedOnSegment(s32 pointIndex, s32 resolution, f32 time) const {
    if (isLinearSegment(pointIndex)) {
        // On linear segments, our current distance is simply the distance between
        // the start point of this segment
        mVec3_c point;
        getPoint(pointIndex, time, point);
        const Vec *start = getPoint(pointIndex);
        return VECDistance(start, point);
    }

    s32 numIterations = resolution * 2;
    mVec3_c currPos;
    mVec3_c currControl;
    mVec3_c nextControl;
    mVec3_c nextPos;
    if (!extractControlPoints(pointIndex, currPos, currControl, nextControl, nextPos)) {
        // Extracting control points only fails if we have reached our end position
        // and aren't wrapping, so we haven't moved off the last point.
        return 0.0f;
    }

    // TODO: Oh no
    /*
      z1 = -currPos.z + nextPos.z + (currControl.z - nextControl.z) * 3.0;
      y1 = -currPos.y + nextPos.y + (currControl.y - nextControl.y) * 3.0;
      x1 = -currPos.x + nextPos.x + (currControl.x - nextControl.x) * 3.0;
      z2 = (currPos.z + nextControl.z) * 3.0 - currControl.z * 6.0;
      y2 = (currPos.y + nextControl.y) * 3.0 - currControl.y * 6.0;
      x3 = (currControl.x - currPos.x) * 3.0;
      x2 = (currPos.x + nextControl.x) * 3.0 - currControl.x * 6.0;
      y3 = (currControl.y - currPos.y) * 3.0;
      z3 = (currControl.z - currPos.z) * 3.0;
    */
    mVec3_c v1 = -currPos + nextPos + (currControl - nextControl) * 3.0f;
    mVec3_c v2 = (currPos + nextControl) * 3.0f - currControl * 6.0f;
    mVec3_c v3 = (currControl - currPos) * 3.0f;

    f32 v1Dotv2 = v1.dot(v2);
    f32 v1Dotv3 = v1.dot(v3);
    f32 v2Dotv3 = v2.dot(v3);
    f32 v1Sq = v1.squaredLength();
    f32 v2Sq = v2.squaredLength();
    f32 v3Sq = v3.squaredLength();

    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    // TODO the cast constant appears after all other floats in this function
    f32 fNumIterations = numIterations;
    f32 f9 = time / fNumIterations;
    for (s32 i = 0; i < numIterations + 1; i++) {
        f32 factor;
        if (i == 0 || i == numIterations) {
            factor = 1.0f;
        } else if ((i & 1) != 0) {
            factor = 4.0f;
        } else {
            factor = 2.0f;
        }

        // TODO: Oh no
        /*
        fVar6 = fVar5 * fVar5 * fVar5;
        fVar6 = z3 * z3 + x3 * x3 + y3 * y3 +
                (z2 * z3 + x2 * x3 + y2 * y3) * 4.0 * fVar5 +
                ((z1 * z3 + x1 * x3 + y1 * y3) * 6.0 + (z2 * z2 + x2 * x2 + y2 * y2) * 4.0) *
                fVar5 * fVar5 +
                (z1 * z1 + x1 * x1 + y1 * y1) * 9.0 * fVar6 * fVar5 +
                (z1 * z2 + x1 * x2 + y1 * y2) * 12.0 * fVar6;
        */

        f32 f5Sq = f5 * f5;
        f32 f5Cube = f5Sq * f5;
        f32 f5Pow4 = f5Cube * f5;
        f32 f = v3Sq + (v2Dotv3 * 4.0f * f5) + ((v1Dotv3 * 6.0f + v2Sq * 4.0f) * f5Sq) + (v1Sq * 9.0f * f5Pow4) +
                (v1Dotv2 * 12.0f * f5Cube);
        if (f > 0.0f) {
            f4 += factor * nw4r::math::FSqrt(f);
        }
        f5 += f9;
    }
    f4 *= (f9 / 3.0f);
    return f4;
}

bool dPath_c::getSpeed(s32 segmentIndex, f32 time, f32 *result) const {
    // TODO
    return false;
}

f32 dPath_c::getSegmentTime(s32 pointIndex, f32 segmentDistance, f32 stepSize, f32 *excess) {
    // TODO
    return 0.0f;
}

bool dPath_c::getDirection(s32 pointIndex, f32 time, mVec3_c &out) const {
    if (isLinear()) {
        // Linear path
        const Vec *pnt = getPoint(pointIndex);
        const Vec *next;
        if (pointIndex == mpPathPtr->pointCount - 1) {
            if (!isWrapping()) {
                // We are at the end and no wrap - simply return the end position
                out.x = 0.0f;
                out.y = 0.0f;
                out.z = 0.0f;
                return false;
            } else {
                // Last point but wrap around, next is 0 again
                next = getPoint(0);
            }
        } else {
            // Regular next point
            next = getPoint(pointIndex + 1);
        }
        f32 distance = VECDistance(pnt, next);
        if (cM3d_IsZero(distance)) {
            out.x = 0.0f;
            out.y = 0.0f;
            out.z = 0.0f;
            return false;
        } else {
            VECSubtract(next, pnt, out);
            VECScale(out, out, 1.0f / distance);
            return true;
        }
    } else {
        mVec3_c currPos;
        mVec3_c currControl;
        mVec3_c nextControl;
        mVec3_c nextPos;
        if (!extractControlPoints(pointIndex, currPos, currControl, nextControl, nextPos)) {
            out.x = 0.0f;
            out.y = 0.0f;
            out.z = 0.0f;
            return false;
        }

        if (currPos.x == currControl.x && currPos.y == currControl.y && currPos.z == currControl.z) {
            // TODO: why is the other point not checked?
            out = nextPos - currPos;
        } else {
            // TODO: Oh no
            mVec3_c v3 = ((currPos + nextControl) * 3.0 - currControl * 6.0) * (2.0 * time);
            mVec3_c v2 = (-currPos + nextPos + (currControl - nextControl) * 3.0f) * (3.0f * time * time);
            mVec3_c v1 = (currControl - currPos) * 3.0f;
            out = v1 + v2 + v3;
        }

        f32 mag = VECMag(out);
        if (cM3d_IsZero(mag)) {
            out.x = 0.0f;
            out.y = 0.0f;
            out.z = 0.0f;
            return false;
        } else {
            out *= 1.0f / mag;
            return true;
        }
    }
}

bool dPath_c::extractControlPoints(
    s32 pointIndex, mVec3_c &currPos, mVec3_c &currControl2, mVec3_c &nextControl1, mVec3_c &nextPos
) const {
    const BPNT *pnt = getPathBpoint(pointIndex);
    if (pointIndex == mpPathPtr->pointCount - 1) {
        if (isWrapping()) {
            // Last point but wrap around, next is 0 again
            const BPNT *next = getPathBpoint(0);
            nextPos.copyFrom(&next->position);
            nextControl1.copyFrom(&next->control1);
        } else {
            // We are at the end and no wrap - cannot extract control points
            return false;
        }
    } else {
        const BPNT *next = getPathBpoint(pointIndex + 1);
        nextPos.copyFrom(&next->position);
        nextControl1.copyFrom(&next->control1);
    }

    currPos.copyFrom(&pnt->position);
    currControl2.copyFrom(&pnt->control2);

    return true;
}

bool dPath_c::getVelocity(s32 segmentIndex, f32 time, mVec3_c &result) const {
    // TODO
    return false;
}

ActorOnRail_Ext::ActorOnRail_Ext() {
    mSegmentIndex = 0;
    mSegmentTime = 0.0f;
    mSpeed = 0.0f;
    mFlags = 0;
    field_0x20 = 0.01f;
    mSegmentDistance = 0.0f;
    mPosition.set(0.0f, 0.0f, 0.0f);
}

ActorOnRail_Ext::~ActorOnRail_Ext() {}

bool ActorOnRail_Ext::init(
    s32 pathIndex, s32 roomId, u32 flags, s32 segmentIndex, bool pathSubtype, f32 segmentTime, f32 speed, f32 unk
) {
    if (!mPath.initWithPathIndex(pathIndex, roomId, pathSubtype)) {
        return false;
    }
    setSegment(segmentIndex, segmentTime);
    mFlags = flags;
    field_0x20 = unk;
    mSpeed = speed;
    return true;
}

s32 ActorOnRail_Ext::execute() {
    s32 ret = 0;
    // Move along the curve distance with the defined speed, then fix the curve segment and time
    // until the curve parameters match the distance.
    mSegmentDistance += mSpeed;
    if (mSpeed < 0.0f) {
        while (true) {
            if (!(mSegmentDistance < 0.0f)) {
                break;
            }
            ret = 1;
            mSegmentIndex--;
            if (mSegmentIndex < 0) {
                if (mPath.isWrapping()) {
                    ret = 4;
                    mSegmentIndex = mPath.getNumPoints() - 1;
                } else {
                    mSegmentIndex = 0;
                    ret = 2;
                    mSegmentDistance = 0.0f;
                    if (checkFlag(0x1)) {
                        ret = 3;
                        offFlag(0x40000000);
                        mSpeed *= -1.0f;
                    }
                    break;
                }
                // Add curve length
            }
            mSegmentDistance += mPath.getDistanceMovedOnSegment(mSegmentIndex, 8, 1.0f);
        }
    } else {
        // While our target distance is longer than the current segment...
        while (true) {
            f32 segmentLength = mPath.getDistanceMovedOnSegment(mSegmentIndex, 8, 1.0f);
            if (!(mSegmentDistance > segmentLength)) {
                break;
            }
            ret = 1;
            mSegmentIndex++;
            if (mSegmentIndex >= mPath.getNumPoints()) {
                if (mPath.isWrapping()) {
                    ret = 4;
                    mSegmentIndex = 0;
                } else {
                    mSegmentIndex = mPath.getNumPoints() - 1;
                    ret = 2;
                    mSegmentDistance = segmentLength;
                    if (checkFlag(0x1)) {
                        ret = 3;
                        onFlag(0x40000000);
                        mSpeed *= -1.0f;
                    }
                    break;
                }
            }
            mSegmentDistance -= segmentLength;
        }
    }

    f32 dummy = 0.0f;
    // Finally query the time
    mSegmentTime = mPath.getSegmentTime(mSegmentIndex, mSegmentDistance, field_0x20, &dummy);
    // and update the position
    mPath.getPoint(mSegmentIndex, mSegmentTime, mPosition);
    return ret;
}

f32 ActorOnRail_Ext::getRemainingDistanceOnSegment() const {
    if (mSpeed < 0.0f) {
        return mSegmentDistance;
    } else {
        return mPath.getDistanceMovedOnSegment(mSegmentIndex, 8, 1.0f) - mSegmentDistance;
    }
}

s32 ActorOnRail_Ext::getNextPointIndex(s32 point) const {
    if (mSpeed > 0.0f) {
        if (point == mPath.getNumPoints() - 1) {
            if (mPath.isWrapping()) {
                return 0;
            } else {
                return point;
            }
        }
        return point + 1;
    }
    return point;
}

s32 ActorOnRail_Ext::getNextPointIndex() const {
    return getNextPointIndex(mSegmentIndex);
}

s32 ActorOnRail_Ext::getClosestXZPoint(const mVec3_c &pos) const {
    f32 max = EGG::Math<f32>::maxNumber();
    s32 best = 0;
    mVec3_c c;
    for (s32 i = 0; i < mPath.getNumPoints(); i++) {
        const Vec *point = mPath.getPoint(i);
        // TODO regswap
        c = mVec3_c(point->x - pos.x, point->y - pos.y, point->z - pos.z);
        f32 dist = c.squareMagXZ();
        if (max > dist) {
            best = i;
            max = dist;
        }
    }

    return best;
}

void ActorOnRail_Ext::setSegment(s32 segmentIndex, f32 segmentTime) {
    mSegmentIndex = segmentIndex;
    mSegmentTime = segmentTime;
    mSegmentDistance = mPath.getDistanceMovedOnSegment(mSegmentIndex, 8, segmentTime);
    mPath.getPoint(mSegmentIndex, mSegmentTime, mPosition);
}
