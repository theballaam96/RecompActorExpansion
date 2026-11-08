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
    s16 unk0;
    s16 unk2;
} GlobalASMStruct55;
extern GlobalASMStruct55 D_global_asm_8074D0C4[];
extern ActorPayload *getActorPayloadFromIndex(u16 index);

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