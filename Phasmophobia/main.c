#include "defs.h"



int main()
{
    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    HouseType house;
    initHouse(&house);

    pthread_t ghost, hunter1, hunter2, hunter3, hunter4;

    pthread_create(&ghost, NULL, GhostToMove, house.ghost);

    pthread_create(&hunter1, NULL, HunterToMove, house.hunters[0]);
    pthread_create(&hunter2, NULL, HunterToMove, house.hunters[1]);
    pthread_create(&hunter3, NULL, HunterToMove, house.hunters[2]);
    pthread_create(&hunter4, NULL, HunterToMove, house.hunters[3]);

    pthread_join(ghost, NULL);

    pthread_join(hunter1, NULL);
    pthread_join(hunter2, NULL);
    pthread_join(hunter3, NULL);
    pthread_join(hunter4, NULL);

    finalResult(&house);
    freeElements(&house);
    return 0;
}

