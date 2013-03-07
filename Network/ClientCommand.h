/**
 * ClientCommand
 * This is what gets sent from the client to the server to tell the ship how to move around.
 * Sterling Hirsh
 * 5/9/11
 */

#ifndef __CLIENTCOMMAND_H__
#define __CLIENTCOMMAND_H__

class ClientCommand {
   public:
      ClientCommand();
      virtual ~ClientCommand();
      virtual void reset();

      void print();

      // Administrative
      unsigned shipID; // Maybe clientid?

      // Movement input
      int forwardAcceleration;
      int rightAcceleration;
      int upAcceleration;
      bool brake;

      // Rotation input
      float yawSpeed;
      float rollSpeed;
      float pitchSpeed;

      // Weapon input
      bool fire;
      
      // Which weapon to use
      int currentWeapon;

      // Aim
      float mouseX;
      float mouseY;

      template<class Archive> 
            void serialize(Archive & ar, const unsigned int version) {
         ar & forwardAcceleration;
         ar & rightAcceleration;
         ar & upAcceleration;
         ar & yawSpeed;
         ar & rollSpeed;
         ar & pitchSpeed;
         ar & brake;
         ar & fire;
         ar & currentWeapon;
         ar & mouseX;
         ar & mouseY;
      }
};

#endif
