#ifndef ACTOR_COLLISION_H
#define ACTOR_COLLISION_H

#include <d/a/obj/d_a_obj_base.h>
#include <m/m_vec.h>
#include <m/m_mtx.h>

class ActorCollision {
public:
    ActorCollision();
    ~ActorCollision();

    void* create(void *dzbData, void *plcData, bool b, const mMtx_c &mtx, const mVec3_c &vec);
    void* create(void *dzbData, void *plcData, bool b, const mMtx_c &mtx, const mVec3_c &vec, void *callback) {
        void* result = create(dzbData, plcData, b, mtx, vec);
        multMatrix = callback;
        return result;
    }
    void init();
    void execute();

    u8 field_0x00[0x1FC - 0x000];
    /* 0x1FC */ void *multMatrix;
    /* 0x200 */ void *interactFunc;
    /* 0x204 */ u8 field_0x204[0x210 - 0x204];


};

class CollisionCheckContext {
public:

    static CollisionCheckContext* get();
    void registerActorBgCollision(ActorCollision&, dAcObjBase_c*);
};

#endif
