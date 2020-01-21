#include "oaru.h"
#include <stdlib.h>

using namespace oaru;

DebugLevel readCrtDebugLevel() {
  char* envValue;
  envValue = getenv("OARU_DEBUG_LEVEL");
  if (envValue != NULL) {
      int level = atoi(envValue);
      switch (level) {
        case 0: return DEBUG_LEVEL_NONE;
        case 1: return DEBUG_LEVEL_LOW;
        case 2: return DEBUG_LEVEL_MED;
        case 3: return DEBUG_LEVEL_HIGH;
      default:
          return DEBUG_LEVEL_NONE;
      }
  }
  return DEBUG_LEVEL_NONE;
}