#ifndef OARU_H_
#define OARU_H_

namespace oaru {

typedef enum {
  DEBUG_LEVEL_NONE = 0,
  DEBUG_LEVEL_LOW = 1,
  DEBUG_LEVEL_MED = 2,
  DEBUG_LEVEL_HIGH = 3
} DebugLevel;

DebugLevel readCrtDebugLevel();
void debug(const DebugLevel level, const char *msg);

} // end namespace oaru

#endif // OARU_H_