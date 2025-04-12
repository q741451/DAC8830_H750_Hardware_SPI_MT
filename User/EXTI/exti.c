//-----------------------------------------------------------------
// 程序描述:
//      外部中断驱动程序
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2020-11-11)外部中断初始化，中断时执行相应的事情
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2
// 说    明: 通过按键来实现外部中断，每按一次按键就会触发一次中断；
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "exti.h"
#include "DWT.h"
//-----------------------------------------------------------------

void EXTI_Init(uint32_t keyMask)
{
  GPIO_InitTypeDef GPIO_Initure;
  
  __HAL_RCC_GPIOE_CLK_ENABLE();             // 开启GPIOE时钟

  GPIO_Initure.Pin = 0;
  
  if(keyMask & EXTI_KEY1) GPIO_Initure.Pin |= GPIO_PIN_0;
  if(keyMask & EXTI_KEY2) GPIO_Initure.Pin |= GPIO_PIN_1;
  if(keyMask & EXTI_KEY3) GPIO_Initure.Pin |= GPIO_PIN_2;
  if(keyMask & EXTI_KEY4) GPIO_Initure.Pin |= GPIO_PIN_3;
  
  GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;   // 下升沿触发
  GPIO_Initure.Pull=GPIO_PULLUP;            // 上拉
  
  HAL_GPIO_Init(GPIOE,&GPIO_Initure);
  
  if(keyMask & EXTI_KEY1)
  {
    HAL_NVIC_SetPriority(EXTI0_IRQn,1,0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
  }
  if(keyMask & EXTI_KEY2)
  {
    HAL_NVIC_SetPriority(EXTI1_IRQn,1,1);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
  }
  if(keyMask & EXTI_KEY3)
  {
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
  }
  if(keyMask & EXTI_KEY4)
  {
    HAL_NVIC_SetPriority(EXTI3_IRQn,2,1);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
  }
}

EXTI_IRQHandler_Callback EXTI_callback0 = NULL;
EXTI_IRQHandler_Callback EXTI_callback1 = NULL;
EXTI_IRQHandler_Callback EXTI_callback2 = NULL;
EXTI_IRQHandler_Callback EXTI_callback3 = NULL;

void *EXTI_callback0_user = NULL;
void *EXTI_callback1_user = NULL;
void *EXTI_callback2_user = NULL;
void *EXTI_callback3_user = NULL;

void EXTI_Register(uint32_t keyMask, EXTI_IRQHandler_Callback callback, void *user_env)
{
  if(keyMask & EXTI_KEY1)
  {
    EXTI_callback0 = callback;
    EXTI_callback0_user = user_env;
  }
  if(keyMask & EXTI_KEY2)
  {
    EXTI_callback1 = callback;
    EXTI_callback1_user = user_env;
  }
  if(keyMask & EXTI_KEY3)
  {
    EXTI_callback2 = callback;
    EXTI_callback2_user = user_env;
  }
  if(keyMask & EXTI_KEY4)
  {
    EXTI_callback3 = callback;
    EXTI_callback3_user = user_env;
  }
}

void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI1_IRQHandler(void)
{ 
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void EXTI2_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch(GPIO_Pin)
  {
  case GPIO_PIN_0:
      if (EXTI_callback0)
        EXTI_callback0(EXTI_callback0_user);
      break;
  case GPIO_PIN_1:
      if (EXTI_callback1)
        EXTI_callback1(EXTI_callback1_user);
      break;
  case GPIO_PIN_2:
      if (EXTI_callback2)
        EXTI_callback2(EXTI_callback2_user);
      break;
  case GPIO_PIN_3:
      if (EXTI_callback3)
        EXTI_callback3(EXTI_callback3_user);
      break;
  }
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
