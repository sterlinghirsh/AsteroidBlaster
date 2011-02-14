/**
 * @file
 * The AI interface.
 * <pre>
 * This header file contains the interface that every AI component must
 * implement. All AI's must have a think function.
 * </pre>
 * @author Mike Smith
 */

#ifndef _AI_H_
#define _AI_H_

class AI {
   public:
      virtual int think(double dt)=0;
      virtual void enable()=0;
      virtual void disable()=0;
      virtual bool isEnabled()=0;
};

#endif
