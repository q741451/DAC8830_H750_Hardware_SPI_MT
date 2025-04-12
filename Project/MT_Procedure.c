#include "MT_Procedure.h"
#include "MT.h"
#include "DWT.h"

MT_CTX ctx;

void MT_Procedure1(void)
{
  MT_init(&ctx);
	
	MT_setCParm(&ctx, 100);
	
	MT_setForce(&ctx, 20);
	
	DWT_Delay_ms64(5000);
	
	MT_setForce(&ctx, 80);
	
	DWT_Delay_ms64(5000);
	
	MT_setForce(&ctx, 50);
	
	DWT_Delay_ms64(5000);
	
	MT_runForceLoading(&ctx, 10, 90, 10);

	DWT_Delay_ms64(1000);
	
	MT_runForceLoading(&ctx, 90, 10, 10);
}

void MT_Procedure2(void)
{

}

