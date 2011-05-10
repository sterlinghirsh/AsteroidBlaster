/**
 * ClientCommand
 * This is what gets sent from the client to the server to tell the ship how to move around.
 * Sterling Hirsh
 * 5/9/11
 */

#include "Network/ClientCommand.h"
#include "Utility/Constants.h"

void ClientCommand::reset() {
   forwardAcceleration = rightAcceleration = upAcceleration = 0;
   yawSpeed = rollSpeed = pitchSpeed = 0;
   brake = fire = false;
   currentWeapon = BLASTER_WEAPON_INDEX; // Ship's default.
   mouseX = mouseY = 0;
}

ClientCommand::ClientCommand() {
   shipID = 0;
   reset();
}

ClientCommand::~ClientCommand() {}
