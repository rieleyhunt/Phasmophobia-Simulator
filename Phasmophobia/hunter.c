#include "defs.h"

HunterType* initHunter(char *name, EvidenceType evi, LockType* lock, SharedEvidenceType* sharedEvidence){
    HunterType *hunter;
    hunter = malloc(sizeof(HunterType));

    strcpy(hunter->name, name);
    hunter->boredom = 0;
    hunter->fear = 0;
    hunter->evidence = evi;
    hunter->room = NULL;
    hunter->isEvidenceFind = false;
    hunter->key = lock;
    hunter->sharedEvidence = sharedEvidence;
    hunter->sharedEvidence->hunters = 0;

    l_hunterInit(hunter->name, hunter->evidence);

    return hunter;
} 

void putHunterInVan(HunterType* hunter ,RoomType* van){
    hunter->room = van;
    van->hunters[van->num_hunters] = hunter;
    van->num_hunters++;
}


void* HunterToMove(void *arg){

  HunterType *hunter = arg;
  int r; 
  
  while(1){

    if(hunter->key->count >= 1){
      break;
    }
    r = randInt(0, 3);

    if(r == 0){
      if (sem_wait(&hunter->key->mutex) < 0) {
        printf("semaphore wait error\n");
        exit(1);
      }

      removeHunterFromRoom(hunter);
      hunter->room = randomRoom(&hunter->room->connectedRoooms, 0);
      l_hunterMove(hunter->name, hunter->room->name);
      addHunterInRoom(hunter);
      
      if(hunter->room->ghost != NULL){
        hunter->fear++;
        hunter->boredom = 0;
      }else{
        hunter->boredom++;
      }

      if (sem_post(&hunter->key->mutex) < 0) {
        printf("semaphore post error\n");
        exit(1);
      }

    }else if(r == 1){
      if (sem_wait(&hunter->key->mutex) < 0) {
        printf("semaphore wait error\n");
        exit(1);
      }

      collectEvidence(hunter);

      if (sem_post(&hunter->key->mutex) < 0) {
        printf("semaphore post error\n");
        exit(1);
      }

    }else if(r == 2){
      if (sem_wait(&hunter->key->mutex) < 0) {
        printf("semaphore wait error\n");
        exit(1);
      }

      reviewEvidence(hunter);

      if (sem_post(&hunter->key->mutex) < 0) {
        printf("semaphore post error\n");
        exit(1);
      }

    }else{
      printf("error selected\n");
    }

    if(hunter->boredom >= BOREDOM_MAX){
      l_hunterExit(hunter->name, LOG_BORED);
      removeHunterFromRoom(hunter);
      break;
    }

    if(hunter->fear >= FEAR_MAX){
      if (sem_wait(&hunter->key->mutex) < 0) {
        printf("semaphore wait error\n");
        exit(1);
      }

      l_hunterExit(hunter->name, LOG_FEAR);
      hunter->sharedEvidence->huntersRunAway[hunter->sharedEvidence->hunters] = hunter;
      hunter->sharedEvidence->hunters++;
      removeHunterFromRoom(hunter);

      if(hunter->sharedEvidence->hunters >= 4){
        hunter->key->count = 1;
      }

      if (sem_post(&hunter->key->mutex) < 0) {
        printf("semaphore post error\n");
        exit(1);
      }      
      break;
    }

  }
  return NULL;
}
