#include <stdlib.h>
#include "common.h"

static thread_local enum FLY_STATUS real_fly_status = FLY_OK;

FLYAPI enum FLY_STATUS *fly_status_addr() {
  return &real_fly_status;
}

#define AS_NAME_STRING(ENUM_NAME) #ENUM_NAME,

FLYAPI const char * const FLY_STATUS_STR[FLY_STATUS_LEN] = {
  FLY_STATUSES(AS_NAME_STRING)
};

#undef AS_NAME_STRING

