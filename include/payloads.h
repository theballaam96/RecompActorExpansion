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