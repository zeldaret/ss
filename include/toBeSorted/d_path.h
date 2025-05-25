#ifndef D_PATH_H
#define D_PATH_H

#include "common.h"
#include "d/d_bzs_types.h"
#include "m/m_vec.h"

/**
 * A dPath_c describes a parametric 3D curve in a stage.
 * This base path class provides read-only evaluation of the curve.
 *
 * A path consists of a number of points that this curve passes through.
 * Paths can have two ends, or wrap around, connecting the end point with the start point again.
 * The curve can either linearly connect the points, or it can be implemented as a spline
 * (splines are used for certain actors where smooth movement is necessary, e.g. Sandship).
 *
 * A curve point (or the segment between that point and the next) is identified by an integer point index [0; n-1], and
 * each segment itself is parametrized by a float parameter in [0.0; 1.0]. We call this parameter "time", though since
 * no requirements are imposed on the distance between points, there is no guarantee of constant speed across the curve.
 * ActorOnRail_Ext (todo rename) provides calculations that allow actors to move along the curve with constant speed.
 */
class dPath_c {
private:
    const PATH *mpPathPtr;
    s32 mRoomIndex;
    bool mPathSubtype;

    /** Must only be called for spline paths. Extracts the relevant path points and control points for the given segment. */
    bool extractControlPoints(
        s32 pointIndex, mVec3_c &currPos, mVec3_c &currControl2, mVec3_c &nextControl1, mVec3_c &nextPos
    ) const;

    /**
     * Check if the given path segment is a linear segment - even splines can have linear segments!
     * This can speed up some calculations.
     */
    bool isLinearSegment(s32 pointIndex) const;

    void set(const PATH *path, s32 roomIndex, bool subtype);

    const PNT *getPathPoint(s32 pointIndex) const;
    const BPNT *getPathBpoint(s32 pointIndex) const;

public:
    /* 800A6690 */
    dPath_c();
    /* 800A66D0 */
    virtual ~dPath_c();

    bool isWrapping() const {
        return mpPathPtr->flags & PATH_FLAG_WRAP_AROUND;
    }

    bool isLinear() const {
        return (mpPathPtr->flags & PATH_FLAG_SPLINE) == 0;
    }

    s32 getNumPoints() const {
        return mpPathPtr->pointCount;
    }

    /* 800A6D90 */
    bool initWithPathId(s32 pathId, s32 roomId, bool pathSubtype);
    bool initWithPathIndex(s32 pathIndex, s32 roomId, bool pathSubtype);

    void clear();
    

    /** Get a given point that the curve passes through. */
    const Vec *getPoint(s32 pointIndex) const;
    /** Evaluates the curve position for the given segment and time. */
    void getPoint(s32 pointIndex, f64 time, mVec3_c &out) const;
    /** Extract point param - TODO what is this used for? */
    u8 getPointParam(s32 pointIndex, s32 param) const;

    /** Return the index of this PATH in the current room. */
    u16 getMyPathIndex(s32 roomId) const;
    /** Apparently paths can be linked somehow. */
    bool getNextPath(dPath_c *next) const;

    /**
     * Approximate the time for the given distance along the segment.
     * The return value is clamped to [0.0, 1.0]. If the distance exceeds
     * the length of this segment, write the excess to the provided pointer.
     */
    f32 getSegmentTime(s32 pointIndex, f32 segmentDistance, f32 stepSize, f32 *excess);

    /**
     * Approximates the distance already traveled on the given segment for the given time.
     * Calling this with time == 1.0f returns the segment length.
     * Higher resolution -> higher accuracy.
     */
    f32 getDistanceMovedOnSegment(s32 pointIndex, s32 resolution, f32 time) const;
    /** Calculates the normalized direction of an actor moving along the segment for the given time. */
    bool getDirection(s32 segmentIndex, f32 time, mVec3_c &result) const;
    /** Calculates the speed (scalar) of an actor moving along the segment at the given time. */
    bool getSpeed(s32 segmentIndex, f32 time, f32 *result) const;
    /** Calculates the velocity (vector) of an actor moving along the segment at the given time. */
    bool getVelocity(s32 segmentIndex, f32 time, mVec3_c &result) const;

};

/** A stateful "path driver" that moves along a given path with a given speed. */
class ActorOnRail_Ext {
public:
    ActorOnRail_Ext();
    ~ActorOnRail_Ext();

    f32 getRemainingDistanceOnSegment() const;
    s32 getClosestXZPoint(const mVec3_c &) const;
    void setSegment(s32 segmentIndex, f32 segmentTime);
    bool init(s32 pathIndex, s32 roomId, u32 flags, s32 segmentIndex, bool pathSubtype, f32 segmentTime, f32 speed, f32 unk);
    s32 execute();

    s32 getSegmentIndex() const {
        return mSegmentIndex;
    }

    f32 getSegmentTime() const {
        return mSegmentTime;
    }

    bool CheckFlag(u32 flag) const {
        return (mFlags & flag) != 0;
    }

    void ClearFlag(u32 flag) {
        mFlags &= ~flag;
    }

    void SetFlag(u32 flag) {
        mFlags |= flag;
    }

    const mVec3_c &getPosition() const {
        return mPosition;
    }

    void setSpeed(f32 speed) {
        mSpeed = speed;
    }

    void getDirection(mVec3_c &result) {
        mPath.getDirection(mSegmentIndex, mSegmentTime, result);
    }

    bool checkFlag(u32 flags) const {
        return (mFlags & flags) != 0;
    }

    void offFlag(u32 flags) {
        mFlags &= ~flags;
    }

    void onFlag(u32 flags) {
        mFlags |= flags;
    }

private:
    s32 getNextPointIndex(s32 point) const;
    s32 getNextPointIndex() const;

    /* 0x00 */ dPath_c mPath;
    /* 0x10 */ s32 mSegmentIndex;
    /* 0x14 */ f32 mSegmentTime;
    /* 0x18 */ f32 mSpeed;
    /* 0x1C */ u32 mFlags;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 mSegmentDistance;
    /* 0x28 */ mVec3_c mPosition;
};

class ActorOnRail3 {
    /* 0x00 */ dPath_c mPath;
    /* 0x10 */ u16 mSomePntIdx;
    /* 0x14 */ u32 mFlags;

public:
    ActorOnRail3() : mSomePntIdx(0), mFlags(0) {}
};

#endif
