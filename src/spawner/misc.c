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
    f32 unk0[8];
} Struct807500B4;

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

void *_malloc(s32);
s16 func_global_asm_806531B8(f32 arg0, f32 arg1, f32 arg2, s16 arg3);
void func_global_asm_8068A1B8(void);
void func_global_asm_80678428(Actor *arg0);
void *func_global_asm_80612E90(Actor *, s32, u8);
s32 func_global_asm_80677ED0(Struct80677ED0 *arg0);
void func_global_asm_80678458(Actor *arg0);
void func_global_asm_80613794(Actor *arg0, u8 arg1);
extern GlobalASMStruct_8074E8B0 *getASpawnerDataFromActor(u16 index);
extern Actor *gLastSpawnedActor;
extern ActorSpawner *actor_spawner_pointer;

RECOMP_PATCH ActorSpawner *func_global_asm_80689250(s16 arg0, f32 arg1, f32 arg2, f32 arg3, s16 arg4, f32 arg5, Struct807500B4 *arg6, u8 arg7, Actor *arg8, s16 arg9) {
    ActorSpawner *sp2C;
    s32 var_a1;
    s16 temp_a2;
    GlobalASMStruct_8074E8B0 *data;

    var_a1 = 0;
    data = getASpawnerDataFromActor(arg0 + 0x10);

    sp2C = _malloc(0x6C); // TODO: Why is this less than sizeof(ActorSpawner)?
    sp2C->actor_type = arg0;
    sp2C->id = arg9;
    sp2C->z_position = arg3;
    sp2C->unk18 = arg3;
    sp2C->x_position = arg1;
    sp2C->unk10 = arg1;
    sp2C->y_position = arg2;
    sp2C->unk14 = arg2;
    sp2C->y_rotation = arg4;
    sp2C->unk20 = arg5;
    sp2C->tied_actor = NULL;
    sp2C->previous_spawner = NULL;
    sp2C->unk48 = 0;
    sp2C->unk5C = data->unkC;
    sp2C->drawing_code = data->unk10;
    sp2C->unk58 = data->unk2;
    sp2C->unk4A = func_global_asm_806531B8(arg1, arg2, arg3, 0);
    sp2C->unk4C = arg7;
    sp2C->unk50 = arg8;
    sp2C->unk54 = (f32) data->unk8 * (f32) data->unk8;
    for (var_a1 = 0; var_a1 != 8; var_a1 = var_a1 + 4) {
        sp2C->unk24[var_a1] = arg6->unk0[var_a1];
        sp2C->unk24[var_a1 + 1] = arg6->unk0[var_a1 + 1];
        sp2C->unk24[var_a1 + 2] = arg6->unk0[var_a1 + 2];
        sp2C->unk24[var_a1 + 3] = arg6->unk0[var_a1 + 3];
    }

    sp2C->next_spawner = actor_spawner_pointer;
    if (actor_spawner_pointer != NULL) {
        actor_spawner_pointer->previous_spawner = sp2C;
    }
    actor_spawner_pointer = sp2C;
    return sp2C;
}

RECOMP_PATCH void func_global_asm_80689418(void) {
    ActorSpawner *currentSpawner;
    Struct80677ED0 tempStruct;
    f64 scaleFactor;
    GlobalASMStruct_8074E8B0 *data;

    currentSpawner = actor_spawner_pointer;
    scaleFactor = 0.15;
    while (currentSpawner != NULL) {
        if (currentSpawner->unk48 != 0) {
            if (currentSpawner->drawing_code(currentSpawner) != 0) {
                if (!(currentSpawner->tied_actor->object_properties_bitfield & 0x200000)) {
                    func_global_asm_80678458(currentSpawner->tied_actor);
                    currentSpawner->unk48 = 0;
                    currentSpawner->unk10 = currentSpawner->tied_actor->x_position;
                    currentSpawner->unk14 = currentSpawner->tied_actor->y_position;
                    currentSpawner->unk18 = currentSpawner->tied_actor->z_position;
                    currentSpawner->unk4A = currentSpawner->tied_actor->unk12C;
                    func_global_asm_80613794(currentSpawner->tied_actor, 0);
                    func_global_asm_80613794(currentSpawner->tied_actor, 1);
                    func_global_asm_80613794(currentSpawner->tied_actor, 2);
                }
            }
        } else if (currentSpawner->unk5C(currentSpawner) != 0) {
            if (currentSpawner->tied_actor == NULL) {
                data = getASpawnerDataFromActor(currentSpawner->actor_type + 0x10);
                tempStruct.unk0 = data->unk0;
                tempStruct.unk8 = data->unk4;
                tempStruct.unk4 = data->unk2;
                tempStruct.unkC = currentSpawner->x_position;
                tempStruct.unk10 = currentSpawner->y_position;
                tempStruct.unk14 = currentSpawner->z_position;
                tempStruct.unk18 = currentSpawner->y_rotation;
                tempStruct.unk1A = 0;
                tempStruct.unk1C = (Actor124 *)&currentSpawner->unk24[0];
                if (func_global_asm_80677ED0(&tempStruct) != 0) {
                    currentSpawner->tied_actor = gLastSpawnedActor;
                    currentSpawner->unk48 = 1;
                    gLastSpawnedActor->unk11C = currentSpawner->unk50;
                    if (gLastSpawnedActor->animation_state != NULL) {
                        gLastSpawnedActor->animation_state->scale_x = currentSpawner->unk20 * scaleFactor;
                        gLastSpawnedActor->animation_state->scale_y = currentSpawner->unk20 * scaleFactor;
                        gLastSpawnedActor->animation_state->scale_z = currentSpawner->unk20 * scaleFactor;
                    }
                }
            } else if (currentSpawner->unk48 == 0) {
                if (currentSpawner->unk58 != 0) {
                    currentSpawner->tied_actor->unk0 = func_global_asm_80612E90(currentSpawner->tied_actor, currentSpawner->unk58, 0);
                }
                func_global_asm_80678428(currentSpawner->tied_actor);
                currentSpawner->unk48 = 1;
            }
        }
        currentSpawner = currentSpawner->next_spawner;
    }
    
    func_global_asm_8068A1B8();
}
