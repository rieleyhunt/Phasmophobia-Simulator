#include "defs.h"

RoomType* createRoom(char *Name){
    struct  Room* room;
    room = malloc(sizeof(RoomType));
    
    strcpy(room->name, Name);
    initLinkedList(&room->connectedRoooms);
    initLinkedList(&room->ghostLeftEvidence);
    room->num_hunters = 0;
    return room;
}

void addRoom(LinkedListType* list, RoomType* room){
   NodeType *newNode;

    newNode = malloc(sizeof(NodeType));
    if (newNode == NULL) {
        printf("Memory allocation error\n");
        exit(0);
    }   

    newNode->roomData = room;
    newNode->next = NULL;

     if(list->sizeGLT == 0){
        list->head = newNode;
        list->tail = newNode;
        list->sizeGLT++;
    }else{
        list->tail->next = newNode;
        list->tail = newNode;
        list->sizeGLT++;
    }
}

bool connectRooms(RoomType* room1, RoomType* room2){
    NodeType *newNode;

    newNode = malloc(sizeof(NodeType));
    if (newNode == NULL) {
        printf("Memory allocation error\n");
        exit(0);
    }   

    newNode->roomData = room2;
    newNode->next = NULL;

    if(room1->connectedRoooms.sizeGLT == 0){
        room1->connectedRoooms.head = newNode;
        room1->connectedRoooms.tail = newNode;
        room1->connectedRoooms.sizeGLT++;
    }else{
        room1->connectedRoooms.tail->next = newNode;
        room1->connectedRoooms.tail = newNode;
        room1->connectedRoooms.sizeGLT++;
    }

    if(room2->connectedRoooms.sizeGLT == 0){
        connectRooms(room2, room1);
    }
    else{
        NodeType *temp;
        temp = room2->connectedRoooms.head;

        for(int z=0;z<room2->connectedRoooms.sizeGLT;z++){
            if(strcmp(temp->roomData->name, room1->name) == 0){
                return false;
            }
        }
        connectRooms(room2, room1);
    }
    return true;
}

void initLinkedList(LinkedListType* list){
    list->head = NULL;
    list->tail = NULL;
    list->sizeGLT = 0;
}

void addEvidence(LinkedListType *list, EvidenceType evi){
    NodeType *newNode;

    newNode = malloc(sizeof(NodeType));
    if (newNode == NULL) {
        printf("Memory allocation error\n");
        exit(0);
    }  

    newNode->evidence = evi;
    newNode->next = NULL;

    if(list->sizeGLT == 0){
        list->head = newNode;
        list->tail = newNode;
        list->sizeGLT++;
    }else{
        list->tail->next = newNode;
        list->tail = newNode;
        list->sizeGLT++;
    }
}

