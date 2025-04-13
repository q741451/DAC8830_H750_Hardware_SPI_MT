#include "MT.h"
#include "dac8830.h"
#include "DWT.h"

#include <math.h>

void MT_init(MT_CTX *ctx)
{
  ctx->cs_mask = DAC8830_CS1;
  ctx->c_parm_ratio = 0.05;
  ctx->resolution_ms = 20;
}

void MT_setCParm(MT_CTX *ctx, double c_parm)
{
  ctx->c_parm_ratio = 1 / c_parm * 5;
}

void MT_setForce(MT_CTX *ctx, double force)
{
  DAC8830_Set_Direct_Current(force * ctx->c_parm_ratio, ctx->cs_mask);
}

void MT_runForceLoading(MT_CTX *ctx, double forceFrom, double forceTo, double loadingRate)
{
  uint32_t time_end_ms = (fabs(forceTo - forceFrom) / loadingRate) * 1000;
  double force_step = fabs(forceTo - forceFrom) * (double)ctx->resolution_ms / (double)time_end_ms;
  uint32_t cur_time_ms = 0;
  double cur_force = 0;

  if(forceTo > forceFrom)
  {
    for(cur_time_ms = 0, cur_force = forceFrom; cur_time_ms < time_end_ms; cur_time_ms += ctx->resolution_ms, cur_force += force_step, DWT_Delay_ms64(ctx->resolution_ms))
    {
      MT_setForce(ctx, cur_force);
    }
  }
    
  if(forceTo < forceFrom)
  {
    for(cur_time_ms = 0, cur_force = forceFrom; cur_time_ms < time_end_ms; cur_time_ms += ctx->resolution_ms, cur_force -= force_step, DWT_Delay_ms64(ctx->resolution_ms))
    {
      MT_setForce(ctx, cur_force);
    }
  }
  
  MT_setForce(ctx, forceTo);
}

void MT_runData_us(MT_CTX *ctx, double *forces, uint32_t length, uint32_t us)
{
  uint32_t i = 0;
  
  for (i = 0; i < length; i++)
  {
    MT_setForce(ctx, forces[i]);
    DWT_Delay_us(us);
  }
}

void MT_runData_ms(MT_CTX *ctx, double *forces, uint32_t length, uint32_t ms)
{
  uint32_t i = 0;

  for (i = 0; i < length; i++)
  {
    MT_setForce(ctx, forces[i]);
    DWT_Delay_ms64(ms);
  }
}
