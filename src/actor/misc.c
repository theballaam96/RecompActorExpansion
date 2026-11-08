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

extern u16 *D_global_asm_8074E218[];
extern u16 D_global_asm_8074D624[];
extern u16 D_global_asm_8074DA58[];
extern GlobalASMStruct53 D_global_asm_807FB930[];
extern s32 D_global_asm_807FBB64;
extern Struct80748D40 D_global_asm_80748D40[];
extern u16 D_global_asm_807FBB34;
extern s32 D_global_asm_807FBB38;
extern u16 D_global_asm_807FBB3E;
extern Actor *gLastSpawnedActor;
extern ActorPayload *getActorPayloadFromIndex(u16 index);
void *_malloc(s32);
void func_global_asm_80611690(void *arg0);
void func_global_asm_8067AE90(Actor *arg0);
void func_global_asm_806761D0(Actor *arg0);

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