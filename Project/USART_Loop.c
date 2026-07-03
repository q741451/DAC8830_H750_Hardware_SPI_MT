#include "USART_Loop.h"
#include "MT.h"
#include "MT_USART.h"
#include "DWT.h"

MU_CTX mu_ctx;
MT_CTX mt_ctx;

void USART_Loop(void)
{
  MT_init(&mt_ctx);
  
  while(1)
  {
      while(MU_TryReadCmd(&mu_ctx))
      {
          if(!mu_ctx.valid)
          {
            /*
              if(mu_ctx.code == MU_CMD_UNKNOWN)
                  printf("Unknown command: %s\r\n", mu_ctx.line);
              else
                  printf("Bad arguments: %s\r\n", mu_ctx.line);
            */
              continue;
          }
          
          switch(mu_ctx.code)
          {
              case MU_CMD_MOV:
                  MT_setCurrent(&mt_ctx, ((double)mu_ctx.args.mov.current_ma) / 1000);
                  // printf("MOV current=%dmA\r\n", mu_ctx.args.mov.current_ma);
                  break;
              
              case MU_CMD_PULSE:
                  MT_setCurrent(&mt_ctx, ((double)mu_ctx.args.pulse.current_ma1) / 1000);
                  DWT_Delay_ms64(mu_ctx.args.pulse.time_ms);
                  MT_setCurrent(&mt_ctx, ((double)mu_ctx.args.pulse.current_ma2) / 1000);
                  // printf("PULSE current1=%dmA time=%dms current2=%dmA\r\n", mu_ctx.args.pulse.current_ma1, mu_ctx.args.pulse.time_ms, mu_ctx.args.pulse.current_ma2);
                  break;
              
              case MU_CMD_EXIT:
                  // printf("EXIT\r\n");
                  return;
              
              default:
                  break;
          }
      }
  }
}
