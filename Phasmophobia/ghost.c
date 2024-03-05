#include "defs.h"

void cleanupRoomData(LinkedListType *list){
   NodeType *temp;
   temp = list->head;
    for(int i =0; i<list->sizeGLT; i++){
        temp = list->head->next;
        free(list->head);
        list->head = temp;
    }
}

void cleanupList(LinkedListType *list){
     NodeType *temp;
     temp = list->head;

    for(int i =0; i<list->sizeGLT; i++){
        cleanupRoomData(&temp->roomData->connectedRoooms);
        cleanupRoomData(&temp->roomData->ghostLeftEvidence);
        free(temp->roomData);
        temp = list->head->next;
        free(list->head);
        list->head = temp;
    }
}

//Working

void initGhost(GhostType **ghost, LinkedListType* room, LockType* lock){
    *ghost = malloc(sizeof(GhostType));
    (*ghost)->ghostclass = randomGhost();
    (*ghost)->boredom_timer = 0;
    (*ghost)->key = lock;
    (*ghost)->room = randomRoom(room, 1);
    (*ghost)->room->ghost = *ghost;

    l_ghostInit((*ghost)->ghostclass, (*ghost)->room->name);

    switch((*ghost)->ghostclass) {
        case BANSHEE:
            (*ghost)->evidence[0] = EMF;
            (*ghost)->evidence[1] = TEMPERATURE;
            (*ghost)->evidence[2] = SOUND;
            
            break;
        case BULLIES:
            (*ghost)->evidence[0] = EMF;
            (*ghost)->evidence[1] = FINGERPRINTS;
            (*ghost)->evidence[2] = SOUND;
            
            break;
        case PHANTOM:
            (*ghost)->evidence[0] = TEMPERATURE;
            (*ghost)->evidence[1] = FINGERPRINTS;
            (*ghost)->evidence[2] = SOUND;
            
            break;
        case POLTERGEIST:
            (*ghost)->evidence[0] = EMF;
            (*ghost)->evidence[1] = TEMPERATURE;
            (*ghost)->evidence[2] = FINGERPRINTS;
            
            break;
        default:
            break;
        
    }
}



//Ghost_Thread_1

void* GhostToMove(void *arg){

    GhostType *ghost = arg; 

   int r; 
   while(1){

    if(ghost->key->count >= 1){
        break;
    }
   r = randInt(0, 3);

   if(r == 0){

    printf("[GHOST] did nothing\n");

   }else if(r == 1){

    if (sem_wait(&ghost->key->mutex) < 0) {
    printf("semaphore wait error\n");
      exit(1);
    }

    leaveEvidence(ghost);

    if (sem_post(&ghost->key->mutex) < 0) {
      printf("semaphore post error\n");
      exit(1);
    }

   } else if(r == 2){

    if (sem_wait(&ghost->key->mutex) < 0) {
        printf("semaphore wait error\n");
        exit(1);
    }

    ghost->room->ghost = NULL;
    ghost->room = randomRoom(&ghost->room->connectedRoooms, 0);
    ghost->room->ghost = ghost;
    l_ghostMove(ghost->room->name);

    if(ghost->room->num_hunters>0){
        ghost->boredom_timer=0;
    }else{
        ghost->boredom_timer++;
    }

    if (sem_post(&ghost->key->mutex) < 0) {
      printf("semaphore post error\n");
      exit(1);
    }


   }else{
    printf("error selected\n");
   }

    if(ghost->boredom_timer >= BOREDOM_MAX){
        l_ghostExit(LOG_BORED);
        ghost->room->ghost = NULL;
        break;
    }
   }
   return NULL;
}