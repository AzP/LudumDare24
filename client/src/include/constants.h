/* A class for all constants to be defined */

#ifndef CONSTANTS_H
#define CONSTANTS_H

enum PACKETTYPE {
	NOTHING = -1,
	SHIP,
	SHOT,
	HANDSHAKE, 
	PLAYER,
	REQUEST, 
	DEATH,
	QUIT
};

enum SHOTCOLLISION {
	NOHIT = -2,
	BUILDINGHIT = -1,
	PLAYERHIT
		//The rest are player id's
};

static const int GAMEPORT = 8200; //Translated to 2080 in Big Endian
static const int _SND_TYPE_SHOT = 0;
static const int _SND_TYPE_EXPL = 1;
static const int _SND_TYPE_TURB = 2;

#endif
