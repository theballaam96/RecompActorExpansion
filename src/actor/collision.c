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

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4; // Pointer to struct, see func_global_asm_80675810
} GlobalASMStruct57;

extern Struct8074C604 D_global_asm_8074C604[];
extern GlobalASMStruct35 D_global_asm_807FBB70;
extern ActorPayload *getActorPayloadFromIndex(u16 index);
s32 func_global_asm_8067AF44(Actor *arg0);
void func_global_asm_80675D70(Actor *arg0);
void *func_global_asm_80675E3C(GlobalASMStruct57 *arg0, s16 arg1, s16 arg2);
s32 func_global_asm_80676220(s16 arg0);
void func_global_asm_80675DD0(Actor *arg0, Actor *arg1);
void func_global_asm_80675AFC(Actor *arg0, Actor *arg1);

Struct8074C604 default_collision = {
    .unk0 = NULL,
    .unk4 = 0,
};

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