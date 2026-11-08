#include "modding.h"
#include "ultra64.h"
#include "recompconfig.h"
#include "recompdata.h"
#include "recompui.h"
#include "recompui_event_structs.h"
#include "rt64_extended_gbi.h"
#include "enums.h"
#include "common_structs.h"
#include "payloads.h"
#include "global.h"

typedef struct {
    s32 unk0; // Actor Behaviour Index
    s32 unk4;
    s32 unk8;
    f32 unkC; // X Position
    f32 unk10; // Y Position
    f32 unk14; // Z Position
    s16 unk18; // Y Rotation
    s16 unk1A;
    Actor124 *unk1C;
} Struct80677ED0;

extern u8 D_global_asm_8074D8D4[];
extern s32 (*D_global_asm_8074DA30[])(s32 *);
extern Actor *gLastSpawnedActor;
extern ActorPayload *getActorPayloadFromIndex(u16 index);
s32 func_global_asm_8067B3F4(s32 arg0);
void func_global_asm_80665FB4(Actor *arg0);
void func_global_asm_80678E6C(Actor *arg0);
void func_global_asm_8061421C(Actor *);
void func_global_asm_80614A64(Actor *arg0);
void func_global_asm_80678E14(Actor *arg0);
void func_global_asm_8066EBF4(Actor *arg0);
void func_global_asm_806790F4(u8 arg0);
void func_global_asm_8067A1DC(Actor *arg0);
void func_global_asm_80679178(Actor *arg0);
void func_global_asm_80685D84(Actor *arg0);
void func_global_asm_80687F7C(Actor *actor);
s32 func_global_asm_80678050(u16 arg0);
void playActorAnimation(Actor *arg0, s32 arg1);

u8 getActorMasterType(u16 index) {
    ActorPayload *pl;

    if (index < FIRST_ACTOR) {
        return D_global_asm_8074D8D4[index];
    } else {
        pl = getActorPayloadFromIndex(index);
        if (pl) {
            return pl->master_type;
        }
    }
    return NULL;
}

RECOMP_PATCH void func_global_asm_80678CC8(Actor *arg0) {
    u32 temp_v0;
    u32 temp_v0_2;

    // TODO: Cleanup
    if (*(s32*)(&D_global_asm_8074DA30[getActorMasterType(arg0->unk58)]) == (s32)&func_global_asm_8067B3F4) {
        guTranslateF(arg0->unkC, arg0->x_position, arg0->y_position, arg0->z_position);
    }
    func_global_asm_80665FB4(arg0);
    func_global_asm_80678E6C(arg0);
    if (!(arg0->object_properties_bitfield & 0x8000000)) {
        arg0->object_properties_bitfield &= ~0x200;
        if (arg0->animation_state && ((func_global_asm_8061421C(arg0), temp_v0_2 = arg0->object_properties_bitfield, ((temp_v0_2 & 0x500) != 0)) || (arg0->unk156 != 0)) && ((temp_v0_2 & 0x4000) == 0)) {
            func_global_asm_80614A64(arg0);
        }
    }
    func_global_asm_80678E14(arg0);
    func_global_asm_8066EBF4(arg0);
    func_global_asm_806790F4(0);
    func_global_asm_8067A1DC(arg0);
    func_global_asm_80679178(arg0);
    func_global_asm_80685D84(arg0);
    func_global_asm_80687F7C(arg0);
    arg0->object_properties_bitfield |= 0x10;
}

RECOMP_PATCH s32 func_global_asm_80677ED0(Struct80677ED0 *arg0) {
    s32 master_type;

    if (func_global_asm_80678050(arg0->unk0)) {
        master_type = getActorMasterType(arg0->unk0);
        if (D_global_asm_8074DA30[master_type](&arg0->unk4)) {
            gLastSpawnedActor->x_position = arg0->unkC;
            gLastSpawnedActor->y_position = arg0->unk10;
            gLastSpawnedActor->z_position = arg0->unk14;
            gLastSpawnedActor->unk124 = arg0->unk1C;
            gLastSpawnedActor->y_rotation = arg0->unk18;
            gLastSpawnedActor->unk15C = arg0->unk1A;
            if (arg0->unk8 != 0) {
                playActorAnimation(gLastSpawnedActor, arg0->unk8);
            }
            return TRUE;
        }
    }
    return FALSE;
}

RECOMP_PATCH s32 spawnActor(Actors actorIndex, s32 modelIndex) {
    s32 master_type;

    if (func_global_asm_80678050(actorIndex)) {
        master_type = getActorMasterType(actorIndex);
        if (D_global_asm_8074DA30[master_type](&modelIndex)) {
            return TRUE;
        }
    }
    return FALSE;
}