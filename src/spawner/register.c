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

ASpawnerPayload *aspawner_chain = NULL;
extern GlobalASMStruct_8074E8B0 D_global_asm_8074E8B0[];

RECOMP_EXPORT void registerActorSpawner(ASpawnerPayload *pl) {
    ASpawnerPayload *curr;

    if (!aspawner_chain) {
        aspawner_chain = pl;
        return;
    }
    curr = aspawner_chain;
    while (curr) {
        if (!curr->next) {
            curr->next = pl;
            pl->prev = curr;
            return;
        }
        curr = curr->next;
    }
}

GlobalASMStruct_8074E8B0 *getASpawnerDataFromActor(u16 index) {
    ASpawnerPayload *pl;
    s32 i;
    GlobalASMStruct_8074E8B0 output;

    for (i = 0; i < 0x80; i++) {
        if (D_global_asm_8074E8B0[i].unk0 == index) {
            return &D_global_asm_8074E8B0[i];
        }
    }
    if (!aspawner_chain) {
        return NULL;
    }
    pl = aspawner_chain;
    while (pl) {
        if (pl->data.unk0 == index) {
            return &pl->data;
        }
        pl = pl->next;
    }
    return NULL;
}