#include "DWT.h"
#include "stm32h7xx.h"

void DWT_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0; 
}

void DWT_Delay_us(uint32_t us)
{
    uint32_t startTick = DWT->CYCCNT;
    uint32_t delayTicks = us * (SystemCoreClock / 1000000);
    
    while((DWT->CYCCNT - startTick) < delayTicks);
}

void DWT_Delay_ns(uint32_t ns)
{
    uint32_t startTick = DWT->CYCCNT;
    uint32_t delayTicks = ns * (SystemCoreClock / 1000000000);
    
    while((DWT->CYCCNT - startTick) < delayTicks);
}

