#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE

#define MAX_EVIDENCE	3



typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;

typedef   struct House   	HouseType; 
typedef	  struct Lock		LockType;
typedef	  struct SharedEvidence SharedEvidenceType;

typedef   struct LinkedList  	LinkedListType;
typedef   struct Ghost      	GhostType;
typedef   struct Room       	RoomType;
typedef   struct Hunter       	HunterType;
typedef   struct Node       	NodeType;



enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };


struct Lock{
 	volatile int 	count;
 	sem_t 		mutex;
};

struct SharedEvidence{
	EvidenceType	evidenceFound[MAX_EVIDENCE];
	int		arrSize;
	HunterType	*huntersRunAway[NUM_HUNTERS];
	int		hunters;
};

struct LinkedList {
 	NodeType 	*head;
 	NodeType 	*tail;
	int 		sizeGLT;
};

struct Ghost {
  	RoomType   	*room;
  	GhostClass 	ghostclass;
  	EvidenceType 	evidence[MAX_EVIDENCE];
  	int 		boredom_timer;
  	LockType	*key;
};


struct Room {
  	char    	name[MAX_STR];
  	LinkedListType 	connectedRoooms;
  	LinkedListType 	ghostLeftEvidence;
  	HunterType	*hunters[NUM_HUNTERS];
  	GhostType 	*ghost;
  	int		num_hunters;
};


struct Hunter {
	char 			name[MAX_STR];
	RoomType 		*room;
	EvidenceType 		evidence;
	SharedEvidenceType	*sharedEvidence;
	int			fear;
	int 			boredom;
	bool			isEvidenceFind;
	LockType		*key;
};


struct Node {
 	NodeType 	*next;
 	RoomType 	*roomData;
 	EvidenceType 	evidence;
};

struct House {
 	LinkedListType 		rooms; 
 	GhostType 		*ghost;
 	HunterType		*hunters[NUM_HUNTERS];
 	SharedEvidenceType	*sharedEvidences;
 	LockType		*key;
};

//Ghost
	void 		initGhost(GhostType **ghost,LinkedListType* room, LockType* lock);
	void* 		GhostToMove(void*);
	void 		putGhostInRoom(GhostType **ghost, LinkedListType* room);
	void 		cleanupRoomData(LinkedListType *list);
	void 		cleanupList(LinkedListType *list);
//room
	RoomType* 	createRoom(char *Name);
	void 		addRoom(LinkedListType* linkedRooms, RoomType* room);
	void 		initLinkedList(LinkedListType* list);
	bool 		connectRooms(RoomType* room1, RoomType* room2);
	void 		addEvidence(LinkedListType*, EvidenceType);
	
//Hunter
	HunterType* 	initHunter(char *name, EvidenceType evi, LockType* lock, SharedEvidenceType* sharedEvidence);
	void 		putHunterInVan(HunterType* hunter ,RoomType* van);
	void* 		HunterToMove(void*);

//house
	void 		initHouse(HouseType* house);
	void 		populateRooms(HouseType* house);
	void 		putThingON(HouseType* house);


// Helper Utilies
	int 		randInt(int,int);        // Pseudo-random number generator function
	float 		randFloat(float, float);  // Pseudo-random float generator function
	enum GhostClass randomGhost();  // Return a randomly selected a ghost type
	void 		ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
	void 		evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter

	RoomType* 	randomRoom(LinkedListType *l, int start);
	void 		leaveEvidence(GhostType*);
	void 		collectEvidence(HunterType* hunter);
	void 		reviewEvidence(HunterType* hunter);
	void 		finalResult(HouseType* house);
	void 		removeHunterFromRoom(HunterType* hunter);
	void		addHunterInRoom(HunterType* hunter);
	void 		freeElements(HouseType* house);
	void 		hunterWon(HouseType* house);
	void		ghostWon(HouseType* house);

// Logging Utilities
	void 		l_hunterInit(char* name, enum EvidenceType equipment);
	void 		l_hunterMove(char* name, char* room);
	void 		l_hunterReview(char* name, enum LoggerDetails reviewResult);
	void 		l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
	void 		l_hunterExit(char* name, enum LoggerDetails reason);
	void 		l_ghostInit(enum GhostClass type, char* room);
	void 		l_ghostMove(char* room);
	void 		l_ghostEvidence(enum EvidenceType evidence, char* room);
	void 		l_ghostExit(enum LoggerDetails reason);
