/**
 * @file
 * The AI interface.
 * <pre>
 * This file contains the interface that every AI component must
 * implement. All AI's must have a think function and an enabled/disabled state
 * </pre>
 * @author Mike Smith
 */

#include "AI/AI.h"

void AI :: enable() {
   enabled = true;
}

void AI :: disable() {
   enabled = false;
}

bool AI :: isEnabled() {
   return enabled;
}
