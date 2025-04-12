#ifndef _MT_H
#define _MT_H

#  ifdef  __cplusplus
extern "C" {
#  endif

#include "system.h"

typedef struct _MT_CTX_
{
  uint8_t cs_mask;
	double c_parm_ratio;
	uint32_t resolution_ms;
} MT_CTX;

void MT_init(MT_CTX *ctx);

void MT_setCParm(MT_CTX *ctx, double c_parm);

void MT_setForce(MT_CTX *ctx, double force);

void MT_runForceLoading(MT_CTX *ctx, double forceFrom, double forceTo, double loadingRate);

#  ifdef  __cplusplus
}
#  endif

# endif
