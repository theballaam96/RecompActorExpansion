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

ActorPayload *actor_chain = NULL;
u16 actor_allocation = FIRST_ACTOR;

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