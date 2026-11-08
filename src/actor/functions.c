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

extern u16 D_global_asm_807FBB3C;
extern u16 D_global_asm_807FBB34;
extern Actor *gCurrentActorPointer;
extern u8 D_global_asm_807FBE08;
extern GlobalASMStruct53 D_global_asm_807FB930[];
extern void* D_global_asm_807FBFE0[];
extern void (*D_global_asm_8074C0A0[])();
void func_global_asm_80678BBC(Actor *arg0);
void func_global_asm_80678CC8(Actor *arg0);
void func_global_asm_80614A64(Actor *arg0);
void func_global_asm_806790F4(u8 arg0);
void func_global_asm_80678B04(void);

extern ActorPayload *getActorPayloadFromIndex(u16 index);

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