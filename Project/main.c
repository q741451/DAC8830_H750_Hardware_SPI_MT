//-----------------------------------------------------------------
// 程序描述:
//    DAC8830硬件SPI 恒流源控制
// 调试工具: 凌智STM32核心开发板、DA8830模块、YK-PVCCS1000恒流源
// 说    明:
// STM32H750         DA8830
//    5V                +5V
//    GND               GND
//    PA6               CS1
//    PA5               SCLK
//    PA7               SDIO
//    PA4              CS2

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "dac8830.h"
#include "key.h"
#include "exti.h"
#include "LED.h"
#include "usart.h"
#include "system.h"
#include "DWT.h"
#include "MT_Procedure.h"
#include "USART_Loop.h"


void call_back_btn1(void *user_env);

//-----------------------------------------------------------------
//-----------------------------------------------------------------
// 主程序
//-----------------------------------------------------------------
int main(void)
{
  u8 init_key=0;

  CPU_CACHE_Enable();      // 启用CPU缓存
  HAL_Init();              // 初始化HAL库
  MPU_Memory_Protection(); // 设置保护区域
  SystemClock_Config();    // 设置系统时钟,400Mhz
  DWT_Init();
  BSP_KEY_Init(BUTTON_KEY1);
  BSP_KEY_Init(BUTTON_KEY2);
  EXTI_Init(EXTI_KEY1);
  EXTI_Register(EXTI_KEY1, call_back_btn1, NULL);
  LED_Init();               // 初始化LED
  uart_init(USART_DEF_BOUND);        // 初始化USART USART_DEF_BOUND=115200
  DAC8830_Init();

  // 红 - 停止
  LED_R_ON;
  LED_G_OFF;
  LED_B_OFF;

  DAC8830_Set_Direct_Current(0, DAC8830_CS1 | DAC8830_CS2);

  do {
      init_key = KEY_get(0);
  } while(init_key != KEY1_PRES && init_key != KEY2_PRES);
  
  switch (init_key)
  {
  case KEY1_PRES:
    {
      LED_R_ON;
      LED_G_OFF;
      LED_B_ON;
      MT_Procedure1();
      MT_Procedure2();
    }
    break;
  case KEY2_PRES:
    {
      LED_R_OFF;
      LED_G_ON;
      LED_B_ON;
      USART_Loop();
    }
    break;
  }

  // 绿 - 已完成
  LED_R_OFF;
  LED_G_ON;
  LED_B_OFF;

  while(1){}
}

// 中断暂时不用，也未记录时间消除抖动
void call_back_btn1(void *user_env)
{

}
