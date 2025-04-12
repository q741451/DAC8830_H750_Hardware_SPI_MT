#include "DWT.h"
#include "stm32h7xx.h"

void DWT_Init(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0; 
}

void DWT_Delay_s64(uint64_t s)
{
  uint32_t startTick = DWT->CYCCNT;
  uint64_t delayTicks = s * ((uint64_t)SystemCoreClock);
  uint32_t delayTicksHigh = delayTicks >> 32;
  uint32_t delayTicksLow = delayTicks & 0xFFFFFFFF;

  for (; delayTicksHigh > 0; delayTicksHigh--)
  {
    while(DWT->CYCCNT - startTick < 0x80000000);
    while(DWT->CYCCNT - startTick >= 0x80000000);
  }

  while((DWT->CYCCNT - startTick) < delayTicksLow);
}

void DWT_Delay_ms64(uint64_t ms)
{
  uint32_t startTick = DWT->CYCCNT;
  uint64_t delayTicks = ms * ((uint64_t)(SystemCoreClock / 1000));
  uint32_t delayTicksHigh = delayTicks >> 32;
  uint32_t delayTicksLow = delayTicks & 0xFFFFFFFF;

  for (; delayTicksHigh > 0; delayTicksHigh--)
  {
    while(DWT->CYCCNT - startTick < 0x80000000);
    while(DWT->CYCCNT - startTick >= 0x80000000);
  }

  while((DWT->CYCCNT - startTick) < delayTicksLow);
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

