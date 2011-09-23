/**
 * ClientCommand
 * This is what gets sent from the client to the server to tell the ship how to move around.
 * Sterling Hirsh
 * 5/9/11
 */

#include "Network/ClientCommand.h"
#include "Utility/GlobalUtility.h"
#include <iostream>

ClientCommand::ClientCommand() {
   shipID = 0;
   reset();
}

ClientCommand::~ClientCommand() {}

void ClientCommand::reset() {
   forwardAcceleration = rightAcceleration = upAcceleration = 0;
   yawSpeed = rollSpeed = pitchSpeed = 0;
   brake = fire = false;
   currentWeapon = BLASTER_WEAPON_INDEX; // Ship's default.
   mouseX = mouseY = 0;
}

void ClientCommand::print() {
   std::cout << "forwardAcceleration=" << forwardAcceleration << std::endl <<
               "rightAcceleration=" << rightAcceleration << std::endl <<
               "upAcceleration=" << upAcceleration << std::endl <<
               "yawSpeed=" << yawSpeed << std::endl <<
               "rollSpeed=" << rollSpeed << std::endl <<
               "pitchSpeed=" << pitchSpeed << std::endl <<
               "brake=" << brake << std::endl <<
               "fire=" << fire << std::endl <<
               "currentWeapon=" << currentWeapon << std::endl <<
               "mouseX=" << mouseX << std::endl <<
               "mouseY=" << mouseY << std::endl;
}

