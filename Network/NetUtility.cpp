/**
 * Networking Utility stuff.
 */

#include "Network/NetUtility.h"
#include <enet/enet.h>

void initNetworking() {
   enet_initialize();
}

void deinitNetworking() {
   enet_deinitialize();
}
