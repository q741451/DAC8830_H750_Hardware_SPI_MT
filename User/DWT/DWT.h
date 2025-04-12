#ifndef __DWT_H__
#define __DWT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void DWT_Init(void);

void DWT_Delay_s64(uint64_t s);

void DWT_Delay_ms64(uint64_t ms);

void DWT_Delay_us(uint32_t us);

void DWT_Delay_ns(uint32_t ns);

#ifdef __cplusplus
}
#endif

#endif

