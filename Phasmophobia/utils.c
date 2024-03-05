#include "defs.h"

/*
    Returns a pseudo randomly generated number, in the range min to (max - 1), inclusively
        in:   lower end of the range of the generated number
        in:   upper end of the range of the generated number
    return:   randomly generated integer in the range [0, max-1) 
*/
int randInt(int min, int max)
{
    return (int) randFloat(min, max);
}

/*
    Returns a pseudo randomly generated floating point number.
    A few tricks to make this thread safe, just to reduce any chance of issues using random
        in:   lower end of the range of the generated number
        in:   upper end of the range of the generated number
    return:   randomly generated floating point number in the range [min, max)
*/
float randFloat(float min, float max) {
    static __thread unsigned int seed = 0;
    if (seed == 0) {
        seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();
    }

    float random = ((float) rand_r(&seed)) / (float) RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

/* 
    Returns a random enum GhostClass.
*/
enum GhostClass randomGhost() {
    return (enum GhostClass) randInt(0, GHOST_COUNT);
}

RoomType* randomRoom(LinkedListType *l, int start){
    int randomNum = randInt(start, l->sizeGLT);
    NodeType *temp;
    temp = l->head;

    for(int i=0;i<randomNum; i++){
        temp = temp->next;
    }


    return temp->roomData;
}

void leaveEvidence(GhostType *gs){
    int randomNum = randInt(0, MAX_EVIDENCE);
    addEvidence(&gs->room->ghostLeftEvidence, gs->evidence[randomNum]);
    l_ghostEvidence(gs->evidence[randomNum], gs->room->name);
}

void collectEvidence(HunterType* hunter){
    NodeType *temp;
    temp = hunter->room->ghostLeftEvidence.head;
    

    for(int i = 0; i<hunter->room->ghostLeftEvidence.sizeGLT; i++){
        if(hunter->evidence == temp->evidence){
            if(hunter->isEvidenceFind == false){
                hunter->isEvidenceFind = true;
                hunter->sharedEvidence->evidenceFound[hunter->sharedEvidence->arrSize] = temp->evidence;
                hunter->sharedEvidence->arrSize++;
                l_hunterCollect(hunter->name, temp->evidence, hunter->room->name);
            }
        }else{
            temp = hunter->room->ghostLeftEvidence.head->next;
        }
    }
}

void reviewEvidence(HunterType* hunter){
    if(hunter->sharedEvidence->arrSize == 3){
        hunter->key->count = 1;
        l_hunterReview(hunter->name, LOG_SUFFICIENT);
    }else{
        l_hunterReview(hunter->name, LOG_INSUFFICIENT);
    }
}

void finalResult(HouseType* house){
    printf("==================================================\n");
    printf("All done! Let's tally the results...\n");
    printf("==================================================\n");
    for(int i=0;i<house->sharedEvidences->hunters;i++){
        printf("        * %s has run away in fear!\n", house->sharedEvidences->huntersRunAway[i]->name);
    }
    printf("--------------------------------------------------\n");

    if(house->sharedEvidences->arrSize >= 3){
        hunterWon(house);
    }else{
        ghostWon(house);
    }
}

void ghostWon(HouseType* house){
    char str[15];
    printf("All the hunters have run away in fear! or bored!\nThe ghost has won!\nThe hunters failed!\nUsing the evidence they found, they incorrectly determined that the ghost is a Unknown\n");
    ghostToString(house->ghost->ghostclass, str);
    printf("The ghost is actually a %s\n", str);
    printf("The hunters collected the following evidence:\n");
    for(int j=0;j<house->hunters[0]->sharedEvidence->arrSize;j++){
       evidenceToString(house->hunters[0]->sharedEvidence->evidenceFound[j], str);
       printf("    *%s\n", str);
    }
    printf("==================================================\n");
}

void hunterWon(HouseType* house){
    char str[MAX_STR];
    
    if(house->sharedEvidences->evidenceFound[0] == EMF){
        if(house->sharedEvidences->evidenceFound[1] == TEMPERATURE){
            if(house->sharedEvidences->evidenceFound[2] == SOUND){
                //ghost is BANSHEE
                ghostToString(BANSHEE, str);
            }else{
                //ghost is POLTERGEIST
                ghostToString(POLTERGEIST, str);
            }
        }else if(house->sharedEvidences->evidenceFound[1] == FINGERPRINTS){
            if(house->sharedEvidences->evidenceFound[2] == TEMPERATURE){
                //ghost is POLTERGEIST
                ghostToString(POLTERGEIST, str);
            }else{
                //ghost is BULLIES
                ghostToString(BULLIES, str);
            }

        }else if(house->sharedEvidences->evidenceFound[1] == SOUND){
            if(house->sharedEvidences->evidenceFound[2] == TEMPERATURE){
                //ghost is BANSHEE 
                ghostToString(BANSHEE, str);
            }else{
                //ghost is BULLIES
                ghostToString(BULLIES, str);
            }
        }
    }else if(house->sharedEvidences->evidenceFound[0] == TEMPERATURE) {
        if(house->sharedEvidences->evidenceFound[1] == EMF){
            if(house->sharedEvidences->evidenceFound[2] == SOUND){
                //ghost is BANSHEE
                ghostToString(BANSHEE, str);
            }else{
                //ghost is POLTERGEIST
                ghostToString(POLTERGEIST, str);
            }
        }else if(house->sharedEvidences->evidenceFound[1] == FINGERPRINTS){
            if(house->sharedEvidences->evidenceFound[2] == SOUND){
                //ghost is PHANTOM
                ghostToString(PHANTOM, str);
            }else{
                //ghost is POLTERGIEST
                ghostToString(POLTERGEIST, str);
            }

        }else if(house->sharedEvidences->evidenceFound[1] == SOUND){
            if(house->sharedEvidences->evidenceFound[2] == EMF){
                //ghost is BANSHEE 
                ghostToString(BANSHEE, str);
            }else{
                //ghost is PHANTOM
                ghostToString(PHANTOM, str);
            }
        }
    }else if(house->sharedEvidences->evidenceFound[0] == FINGERPRINTS){
        if(house->sharedEvidences->evidenceFound[1] == TEMPERATURE){
            if(house->sharedEvidences->evidenceFound[2] == SOUND){
                //ghost is PHANTOM
                ghostToString(PHANTOM, str);
            }else{
                //ghost is POLTERGEIST
                ghostToString(POLTERGEIST, str);
            }
        }else if(house->sharedEvidences->evidenceFound[1] == EMF){
            if(house->sharedEvidences->evidenceFound[2] == TEMPERATURE){
                //ghost is POLTERGEIST
                ghostToString(POLTERGEIST, str);
            }else{
                //ghost is BULLIES
                ghostToString(BULLIES, str);
            }

        }else if(house->sharedEvidences->evidenceFound[1] == SOUND){
            if(house->sharedEvidences->evidenceFound[2] == TEMPERATURE){
                //ghost is PHANTOM
                ghostToString(PHANTOM, str);
            }else{
                //ghost is BULLIES
                ghostToString(BULLIES, str);
            }
        }

    }else if(house->sharedEvidences->evidenceFound[0] == SOUND){
        if(house->sharedEvidences->evidenceFound[1] == TEMPERATURE){
            if(house->sharedEvidences->evidenceFound[2] == EMF){
                //ghost is BANSHEE
                ghostToString(BANSHEE, str);
            }else{
                //ghost is PHANTOM
                ghostToString(PHANTOM, str);
            }
        }else if(house->sharedEvidences->evidenceFound[1] == FINGERPRINTS){
            if(house->sharedEvidences->evidenceFound[2] == TEMPERATURE){
                //ghost is PHANTOM
                ghostToString(PHANTOM, str);
            }else{
                //ghost is BULLIES
                ghostToString(BULLIES, str);
            }

        }else if(house->sharedEvidences->evidenceFound[1] == EMF){
            if(house->sharedEvidences->evidenceFound[2] == TEMPERATURE){
                //ghost is BANSHEE 
                ghostToString(BANSHEE, str);
            }else{
                //ghost is BULLIES
                ghostToString(BULLIES, str);
            }
        }

    }
    printf("It seems the ghost has been discovered!\nTHe hunters have won the game!\nUsing the evidence they found, they correctly determined that the ghost is a %s\n", str);
    printf("The hunters collected the following evidence:\n");
    for(int j=0;j<house->sharedEvidences->arrSize;j++){
        evidenceToString(house->sharedEvidences->evidenceFound[j], str);
        printf("    *%s\n", str);
    }
    printf("==================================================\n");
}

/*
    Returns the string representation of the given enum EvidenceType.
        in: type - the enum EvidenceType to convert
        out: str - the string representation of the given enum EvidenceType, minimum 16 characters
*/

void evidenceToString(enum EvidenceType type, char* str) {
    switch (type) {
        case EMF:
            strcpy(str, "EMF");
            break;
        case TEMPERATURE:
            strcpy(str, "TEMPERATURE");
            break;
        case FINGERPRINTS:
            strcpy(str, "FINGERPRINTS");
            break;
        case SOUND:
            strcpy(str, "SOUND");
            break;
        default:
            strcpy(str, "UNKNOWN");
            break;
    }
}

/* 
    Returns the string representation of the given enum GhostClass.
        in: ghost - the enum GhostClass to convert
        out: buffer - the string representation of the given enum GhostClass, minimum 16 characters
*/
void ghostToString(enum GhostClass ghost, char* buffer) {
    switch(ghost) {
        case BANSHEE:
            strcpy(buffer, "Banshee");
            break;
        case BULLIES:
            strcpy(buffer, "Bullies");
            break;
        case PHANTOM:
            strcpy(buffer, "Phantom");
            break;
        case POLTERGEIST:
            strcpy(buffer, "Poltergeist");
            break;
        default:
            strcpy(buffer, "Unknown");
            break;
    }
}

void removeHunterFromRoom(HunterType* hunter){
    for(int z=0;z<hunter->room->num_hunters;z++){
        if(hunter->evidence == hunter->room->hunters[z]->evidence){

            //swap
            HunterType *tempHunter;
            tempHunter = hunter->room->hunters[hunter->room->num_hunters-1];//object of hunterType
            hunter->room->hunters[hunter->room->num_hunters-1] = hunter;
            hunter->room->hunters[z] = tempHunter;
            hunter->room->num_hunters--;
        }
    }
}

void addHunterInRoom(HunterType* hunter){
    hunter->room->hunters[hunter->room->num_hunters] = hunter;
    hunter->room->num_hunters++;
}
  
void freeElements(HouseType* house){
    cleanupList(&house->rooms);
    free(house->ghost);
    free(house->sharedEvidences);
    for(int x=0;x<NUM_HUNTERS;x++){
        free(house->hunters[x]);
    }
    free(house->key);
}

