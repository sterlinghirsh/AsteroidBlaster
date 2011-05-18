/**
 * Constants.h
 * Holds constants that is respected by all classes.
 * @author Ryuho Kudo
 * @date 3-16-11
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define WORLD_SIZE 80.00

#define VERT_FOV 45.0
#define LEFT_ALIGN 0
#define CENTERED 1
#define RIGHT_ALIGN 2

// This should probably be an enum.
#define VIEW_COUNT 4
#define VIEW_FIRSTPERSON_SHIP 0
#define VIEW_FIRSTPERSON_GUN 1
#define VIEW_THIRDPERSON_SHIP 2
#define VIEW_THIRDPERSON_GUN 3


// Network stuff
#define PACK_SIZE 1400

//packet id numbers
#define NET_HS_REQ 0
#define NET_HS_RES 1
#define NET_HS_ACK 2
#define NET_HS_FIN 3
#define NET_CLIENTCOMMAND 4
#define NET_KILL 5

#define NET_OBJ_REMOVE 100
#define NET_OBJ_SHARD 101


#endif

