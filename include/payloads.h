typedef struct ActorPayload ActorPayload;
struct ActorPayload {
    void (*function)(void); //
    void *collision;
    s16 init_health; //
    s16 damage; //
    u16 init_interaction; //
    u8 master_type; //
    void *aad_sizes; //
    // index handling
    u16 actor_index;
    ActorPayload *prev;
    ActorPayload *next;
};

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 (*unkC)(ActorSpawner *);
    s32 (*unk10)(ActorSpawner *);
    char unk14[0x1C];
} GlobalASMStruct_8074E8B0;

typedef struct ASpawnerPayload ASpawnerPayload;
struct ASpawnerPayload {
    GlobalASMStruct_8074E8B0 data;
    ASpawnerPayload *prev;
    ASpawnerPayload *next;
};