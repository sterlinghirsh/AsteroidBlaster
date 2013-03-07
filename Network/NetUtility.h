/**
 * Networking Utility stuff.
 */

#ifndef __NETUTILITY_H__
#define __NETUTILITY_H__

void initNetworking();
void deinitNetworking();

#define NET_CHANNELS 2
// In the year 1942, Columbus sailed the ocean big.
#define NET_PORT 1492

// 10 Seconds network connection timeout.
#define NET_TIMEOUT 10000

#define NET_MAX_CLIENTS 16

#endif
