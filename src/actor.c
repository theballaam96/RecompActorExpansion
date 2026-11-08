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

#define FIRST_ACTOR 345
ActorPayload *actor_chain = NULL;
u16 actor_allocation = FIRST_ACTOR;

typedef struct {
    Actor* unk0;
    s32 unk4;
} GlobalASMStruct53;

typedef struct {
    s16 unk0;
    s16 unk2;
} GlobalASMStruct55;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4; // Pointer to struct, see func_global_asm_80675810
} GlobalASMStruct57;

typedef struct {
    void *unk0;
    u8 unk4;
    u8 pad5[3];
} Struct8074C604;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
} Struct80675810_arg1_unk10;

typedef struct {
    s32 unk0;
    s32 unk4;
    Actor *unk8;
    s32 unkC;
    Struct80675810_arg1_unk10 *unk10;
    s32 unk14;
    s32 unk18;
} Struct80675810_arg1;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 (*unk4)(Actor *, Actor *, void *);
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
} Struct80675810;

extern Struct80748D40 D_global_asm_80748D40[];
extern Struct8074C604 D_global_asm_8074C604[];
extern GlobalASMStruct53 D_global_asm_807FB930[];
extern GlobalASMStruct55 D_global_asm_8074D0C4[];
extern u16 *D_global_asm_8074E218[];
extern u16 D_global_asm_8074D624[];
extern u16 D_global_asm_8074DA58[];
extern void* D_global_asm_807FBFE0[];
extern u8 D_global_asm_8074D8D4[];
extern void (*D_global_asm_8074C0A0[])();
extern u16 D_global_asm_807FBB3C;
extern Actor *gCurrentActorPointer;
extern u16 D_global_asm_807FBB34;
extern u8 D_global_asm_807FBE08;
extern s32 D_global_asm_807FBB64;
extern s32 D_global_asm_807FBB38;
extern u16 D_global_asm_807FBB3E;
extern Actor *gLastSpawnedActor;
extern s32 (*D_global_asm_8074DA30[])(s32 *);
extern GlobalASMStruct35 D_global_asm_807FBB70;
void func_global_asm_80678BBC(Actor *arg0);
void func_global_asm_80678CC8(Actor *arg0);
void func_global_asm_80614A64(Actor *arg0);
void func_global_asm_806790F4(u8 arg0);
void func_global_asm_80678B04(void);
void *_malloc(s32);
void func_global_asm_80611690(void *arg0);
void func_global_asm_8067AE90(Actor *arg0);
s32 func_global_asm_8067B3F4(s32 arg0);
void func_global_asm_806790F4(u8 arg0);
void func_global_asm_80665FB4(Actor *arg0);
void func_global_asm_80678E6C(Actor *arg0);
void func_global_asm_8061421C(Actor *);
void func_global_asm_80675D70(Actor *arg0);
void *func_global_asm_80675E3C(GlobalASMStruct57 *arg0, s16 arg1, s16 arg2);
void func_global_asm_80678E14(Actor *arg0);
void func_global_asm_8066EBF4(Actor *arg0);
void func_global_asm_8067A1DC(Actor *arg0);
void func_global_asm_80679178(Actor *arg0);
void func_global_asm_80685D84(Actor *arg0);
void func_global_asm_80687F7C(Actor *actor);
void func_global_asm_80675AFC(Actor *arg0, Actor *arg1);
void func_global_asm_80675DD0(Actor *arg0, Actor *arg1);
s32 func_global_asm_80676220(s16 arg0);
s32 func_global_asm_8067AF44(Actor *arg0);
void playActorAnimation(Actor *arg0, s32 arg1);

Struct8074C604 default_collision = {
    .unk0 = NULL,
    .unk4 = 0,
};

void actorAddToChain(ActorPayload *pl) {
    ActorPayload *curr;

    if (!actor_chain) {
        actor_chain = pl;
        return;
    }
    curr = actor_chain;
    while (curr) {
        if (!curr->next) {
            curr->next = pl;
            pl->prev = curr;
            return;
        }
        curr = curr->next;
    }
}

RECOMP_EXPORT void registerActor(ActorPayload *pl) {
    actorAddToChain(pl);
    pl->actor_index = actor_allocation++;
}

ActorPayload *getActorPayloadFromIndex(u16 index) {
    ActorPayload *pl;

    if (!actor_chain) {
        return NULL;
    }
    pl = actor_chain;
    while (pl) {
        if (pl->actor_index == index) {
            return pl;
        }
        pl = pl->next;
    }
    return NULL;
}

void runActorFunction(u16 index) {
    ActorPayload *pl;

    if (index < FIRST_ACTOR) {
        D_global_asm_8074C0A0[index]();
    } else {
        pl = getActorPayloadFromIndex(index);
        if (pl) {
            pl->function();
        }
    }
}

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

// Update functions
RECOMP_PATCH void func_global_asm_8067889C(u8 arg0) {
    for (D_global_asm_807FBB3C = 0; D_global_asm_807FBB3C < D_global_asm_807FBB34; D_global_asm_807FBB3C++) {
        D_global_asm_807FBE08 = 0;
        gCurrentActorPointer = D_global_asm_807FB930[D_global_asm_807FBB3C].unk0;
        if (((arg0 == 1) && ((gCurrentActorPointer->object_properties_bitfield & 0x100000) == 0)) || 
            ((arg0 == 2) && ((gCurrentActorPointer->object_properties_bitfield & 0x100000) != 0))
        ) {
            func_global_asm_80678BBC(gCurrentActorPointer);
            runActorFunction(gCurrentActorPointer->unk58);
            func_global_asm_80678CC8(gCurrentActorPointer);
        }
    }
}

RECOMP_PATCH void func_global_asm_806789E4(void) {
    s32 i;
    
    for (i = 0; i < 4; i++) {
        D_global_asm_807FBFE0[i] = NULL;
    }
    for (D_global_asm_807FBB3C = 0 ; D_global_asm_807FBB3C < D_global_asm_807FBB34; D_global_asm_807FBB3C++) {
        gCurrentActorPointer = D_global_asm_807FB930[D_global_asm_807FBB3C].unk0;
        if (gCurrentActorPointer->unk64 & 2) {
            func_global_asm_80678BBC(gCurrentActorPointer);
            runActorFunction(gCurrentActorPointer->unk58);
            func_global_asm_80678CC8(gCurrentActorPointer);
        }
        if (gCurrentActorPointer->animation_state != NULL) {
            func_global_asm_80614A64(gCurrentActorPointer);
        }
    }
    func_global_asm_806790F4(1);
    func_global_asm_80678B04();
}

RECOMP_PATCH void func_global_asm_806761D0(Actor *arg0) {
    ActorPayload *pl;

    if (arg0->unk58 < FIRST_ACTOR) {
        arg0->health = D_global_asm_8074D0C4[arg0->unk58].unk0;
    } else {
        pl = getActorPayloadFromIndex(arg0->unk58);
        if (pl) {
            arg0->health = pl->init_health;
        }
    }
}

RECOMP_PATCH void func_global_asm_806761EC(Actor *arg0) {
    ActorPayload *pl;

    if (arg0->unk58 < FIRST_ACTOR) {
        arg0->unk136 = D_global_asm_8074D0C4[arg0->unk58].unk2;
    } else {
        pl = getActorPayloadFromIndex(arg0->unk58);
        if (pl) {
            arg0->unk136 = pl->damage;
        }
    }
}

s32 func_global_asm_80678050(u16 arg0);
RECOMP_PATCH s32 func_global_asm_80678050(u16 actorType) {
    u16 *aaDataIndices;
    u16 *aaDataIndices_0;
    void **aaDataPointer;
    u8 *aaData;
    Actor *actor;
    u16 headerSize;
    u16 dataSize;
    ActorPayload *pl;
    u16 interactable;

    if (actorType < FIRST_ACTOR) {
        aaDataIndices = D_global_asm_8074E218[actorType];
        aaDataIndices_0 = aaDataIndices;
        interactable = D_global_asm_8074D624[actorType];
    } else {
        pl = getActorPayloadFromIndex(actorType);
        if (pl) {
            aaDataIndices = pl->aad_sizes;
            aaDataIndices_0 = aaDataIndices;
            interactable = pl->init_interaction;
        } else {
            return FALSE;
        }
    }
    headerSize = 0x174;
    dataSize = 0;
    while (*aaDataIndices) {
        dataSize += D_global_asm_8074DA58[*aaDataIndices];
        headerSize += 4;
        aaDataIndices++;
    }
    headerSize = ALIGN(headerSize, 8);
    dataSize += headerSize;
    actor = _malloc(dataSize);
    if (actor != NULL) {
        func_global_asm_80611690(actor);
        aaDataIndices = aaDataIndices_0;
        aaDataPointer = (void **)&actor->additional_actor_data;
        aaData = (u8 *)actor + (s32)headerSize;
        while (*aaDataIndices) {
            *aaDataPointer = aaData;
            aaDataPointer++;
            aaData += D_global_asm_8074DA58[*aaDataIndices];
            aaDataIndices++;
        }
        actor->unk58 = actorType;
        actor->collision_queue_pointer = NULL;
        actor->unk138 = -1;
        actor->noclip_byte = 2;
        actor->locked_to_pad = FALSE;
        actor->unkF8 = 0x50;
        actor->object_properties_bitfield = 0x58144;
        actor->unk64 = 0;
        actor->shadow_opacity = 0xFF;
        if (D_global_asm_807FBB64 & 0x100) {
            actor->draw_distance = 2000;
        } else {
            actor->draw_distance = 500;
        }
        actor->unk68 = 0;
        actor->unk156 = 0;
        actor->unk78 = 0;
        actor->unk6C = 1;
        actor->unk6A = 1;
        actor->unk132 = 1;
        actor->unk114 = NULL;
        actor->unk118 = NULL;
        actor->unk11C = NULL;
        actor->unk6E[0] = -1;
        actor->unk70 = -1;
        actor->unk150 = NULL;
        actor->unk12C = -1;
        actor->unk124 = NULL;
        actor->unk148[0] = NULL;
        actor->unk148[1] = NULL;
        actor->unkD4 = &D_global_asm_80748D40[1];
        actor->unk146_s16 = -1;
        actor->interactable = interactable;
        actor->unkDA = 0x400;
        actor->unkCC = 0;
        actor->unkCD = 0;
        actor->unk130 = 0;
        actor->unk131 = 0;
        actor->unk16D = 0xF;
        actor->unkCE = 0;
        actor->unkB0 = -1;
        
        actor->unk54 = D_global_asm_807FBB38;
        D_global_asm_807FBB38++;
        func_global_asm_8067AE90(actor); /* Add to actor list */
        func_global_asm_806761D0(actor); /* Set initial health */
        D_global_asm_807FB930[D_global_asm_807FBB34].unk0 = actor;
        D_global_asm_807FB930[D_global_asm_807FBB34].unk4 = 0;
        D_global_asm_807FBB3E = D_global_asm_807FBB34;
        D_global_asm_807FBB34++;
        gLastSpawnedActor = actor;
        return TRUE;
    }
    return FALSE;
}

RECOMP_PATCH u16 func_global_asm_8067ACC0(u16 index) {
    ActorPayload *pl;

    if (index < FIRST_ACTOR) {
        return D_global_asm_8074D624[index];
    }
    pl = getActorPayloadFromIndex(index);
    if (pl) {
        return pl->init_interaction;
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

Struct8074C604 *getActorCollisionData(u16 index) {
    ActorPayload *pl;

    if (index < FIRST_ACTOR) {
        return &D_global_asm_8074C604[index];
    }
    pl = getActorPayloadFromIndex(index);
    if (pl) {
        if (pl->collision == NULL) {
            return &default_collision;
        }
        return pl->collision;
    }
    return NULL;
}

RECOMP_PATCH void func_global_asm_80675810(Actor *arg0, Struct80675810_arg1 *arg1) {
    Actor *temp_s2;
    Struct80675810 *var_s1;
    s32 var_s4;
    u8 var_v1;
    s32 i;
    s32 temp;
    void *temp_2; // TODO: Possibly actor
    s32 temp_3;
    s32 temp_4;
    PlayerAdditionalActorData* paad;
    Struct8074C604 *col;

    temp_s2 = arg1->unk8;
    i = 0;
    var_s4 = FALSE;
    if ((func_global_asm_8067AF44(temp_s2) != 0) && (temp_s2->unk54 == (u32)arg1->unk10->unk18)) {
        col = getActorCollisionData(arg0->unk58);
        temp_2 = col->unk0;
        if (temp_2 == NULL) {
            if (temp_s2->object_properties_bitfield & 0x20000) {
                func_global_asm_80675D70(temp_s2);
            }
        } else {
            var_s1 = func_global_asm_80675E3C(temp_2, temp_s2->unk58, temp_s2->interactable);
            if ((!(temp_s2->object_properties_bitfield & 0x80000) && !(arg0->object_properties_bitfield & 0x80000)) || ((arg0 != temp_s2->unk11C) && (temp_s2 != arg0->unk11C))) {
                if (var_s1 == NULL) {
                    if (temp_s2->object_properties_bitfield & 0x20000) {
                        func_global_asm_80675D70(temp_s2);
                    }
                } else {
                    while (!var_s4) {
                        if ((var_s1[i].unk0 & arg0->unk132) && (var_s1[i].unk2 & temp_s2->unk132)) {
                            if (var_s1[i].unk4 != NULL) {
                                var_v1 = var_s1[i].unk4(arg0, temp_s2, arg1);
                            } else {
                                var_v1 = 1;
                            }
                            if (var_v1 != 0) {
                                temp = var_s1[i].unk8;
                                temp_4 = D_global_asm_807FBB70.unk200;
                                temp_3 = D_global_asm_807FBB70.unk201;
                                if (temp_4 < temp) {
                                    D_global_asm_807FBB70.unk1FC = temp_s2;
                                    D_global_asm_807FBB70.unk200 = var_s1[i].unk8;
                                }
                                if (temp_3 < 0xF) {
                                    D_global_asm_807FBB70.unk204[D_global_asm_807FBB70.unk201] = temp_s2;
                                    D_global_asm_807FBB70.unk244[D_global_asm_807FBB70.unk201] = var_s1[i].unk9;
                                    D_global_asm_807FBB70.unk201++;
                                    if (temp_s2->interactable & 1) {
                                        paad = temp_s2->PaaD;
                                        if (paad->unkD4 != 0) {
                                            if ((col->unk4 != 0) && (((D_global_asm_807FBB70.unk243[D_global_asm_807FBB70.unk201] == 5)) || (D_global_asm_807FBB70.unk243[D_global_asm_807FBB70.unk201] == 0xF))) {
                                                D_global_asm_807FBB70.unk201--;
                                            }
                                        }
                                        if ((arg0->interactable & 2) && (func_global_asm_80676220(arg0->unk58) != 0)) {
                                            temp_s2->unk64 |= 0x1000;
                                        }
                                    }
                                }
                                switch (var_s1[i].unkA) {
                                    case 0:
                                    case 2:
                                        var_s4 = TRUE;
                                        break;
                                }
                            }
                        }
                        if (var_s1[i].unkA == 0) {
                            var_s4 = TRUE;
                        }
                        i++;
                    }
                    if (arg0->object_properties_bitfield & 0x20000000) {
                        D_global_asm_807FBB70.unk200 = 1;
                    }
                    if ((D_global_asm_807FBB70.unk200 == 6) || (D_global_asm_807FBB70.unk200 == 7)) {
                        func_global_asm_80675DD0(arg0, temp_s2);
                    }
                    func_global_asm_80675AFC(arg0, temp_s2);
                }
            }
        }
    }
}

RECOMP_PATCH u8 func_global_asm_80676208(Actor *arg0) {
    Struct8074C604 *col;

    col = getActorCollisionData(arg0->unk58);
    return col->unk4;
}