#include "defs.h"


void populateRooms(HouseType* house) {

    // createRoom assumes that we dynamically allocate a room, initializes the values, and returns a RoomType*
    // create functions are pretty typical, but it means errors are harder to return aside from NULL
    struct Room* van                = createRoom("Van");
    struct Room* hallway            = createRoom("Hallway");
    struct Room* master_bedroom     = createRoom("Master Bedroom");
    struct Room* boys_bedroom       = createRoom("Boy's Bedroom");
    struct Room* bathroom           = createRoom("Bathroom");
    struct Room* basement           = createRoom("Basement");
    struct Room* basement_hallway   = createRoom("Basement Hallway");
    struct Room* right_storage_room = createRoom("Right Storage Room");
    struct Room* left_storage_room  = createRoom("Left Storage Room");
    struct Room* kitchen            = createRoom("Kitchen");
    struct Room* living_room        = createRoom("Living Room");
    struct Room* garage             = createRoom("Garage");
    struct Room* utility_room       = createRoom("Utility Room");

    // This adds each room to each other's room lists
    // All rooms are two-way connections
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);

    // Add each room to the house's room list
    addRoom(&house->rooms, van);
    addRoom(&house->rooms, hallway);
    addRoom(&house->rooms, master_bedroom);
    addRoom(&house->rooms, boys_bedroom);
    addRoom(&house->rooms, bathroom);
    addRoom(&house->rooms, basement);
    addRoom(&house->rooms, basement_hallway);
    addRoom(&house->rooms, right_storage_room);
    addRoom(&house->rooms, left_storage_room);
    addRoom(&house->rooms, kitchen);
    addRoom(&house->rooms, living_room);
    addRoom(&house->rooms, garage);
    addRoom(&house->rooms, utility_room);
}

void initHouse(HouseType* house){
    initLinkedList(&house->rooms);

    char str[MAX_STR];

    house->sharedEvidences = malloc(sizeof(SharedEvidenceType));
    house->sharedEvidences->arrSize=0;

    house->key = malloc(sizeof(LockType));
    house->key->count=0;

    if (sem_init(&house->key->mutex, 0, 1) < 0) {
        printf("semaphore initialization error\n");
        exit(1);
    }  

    populateRooms(house);

    initGhost(&house->ghost, &house->rooms, house->key);

    printf("Enter the name of Hunter 1: ");
    fgets(str, MAX_STR, stdin);
    str[strlen(str)-1] = 0;
    house->hunters[0] = initHunter(str, EMF, house->key, house->sharedEvidences);

    printf("Enter the name of Hunter 2: ");
    fgets(str, MAX_STR, stdin);
    str[strlen(str)-1] = 0;
    house->hunters[1] = initHunter(str, TEMPERATURE, house->key, house->sharedEvidences);

    printf("Enter the name of Hunter 3: ");
    fgets(str, MAX_STR, stdin);
    str[strlen(str)-1] = 0;
    house->hunters[2] = initHunter(str, FINGERPRINTS, house->key, house->sharedEvidences);

    printf("Enter the name of Hunter 4: ");
    fgets(str, MAX_STR, stdin);
    str[strlen(str)-1] = 0;
    house->hunters[3] = initHunter(str, SOUND, house->key, house->sharedEvidences);

    putThingON(house);

}

void putThingON(HouseType* house){
    for(int i=0;i<NUM_HUNTERS;i++){
       putHunterInVan(house->hunters[i], house->rooms.head->roomData);
    } 
}
    
