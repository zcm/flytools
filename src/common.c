#include <stdlib.h>
#include "common.h"

FLYAPI enum FLY_STATUS fly_status = FLY_OK;

#define AS_NAME_STRING(ENUM_NAME) #ENUM_NAME,

FLYAPI const char * const FLY_STATUS_STR[FLY_STATUS_LEN] = {
  FLY_STATUSES(AS_NAME_STRING)
};

#undef AS_NAME_STRING

