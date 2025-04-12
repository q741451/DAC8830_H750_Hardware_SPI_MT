//-----------------------------------------------------------------
// 程序描述:
//    DAC8830硬件SPI 实验
// 作    者: 凌智电子
// 开始日期: 2024-04-20
// 完成日期: 2024-04-20
//   - V1.0:  DAC8830硬件SPI 实验
// 调试工具: 凌智STM32核心开发板、LZE_ST LINK2、2.8寸液晶、DA8830模块
// 说    明:
// STM32H750         DA8830
//    5V                +5V
//    GND               GND
//    PA6               CS1
//    PA5               SCLK
//    PA7               SDIO
//    PA4              CS2
//
//  DAC8830有4种输出模式。通过改变跳帽改变模式。
//  当改变跳帽后，需要修改程序的宏定义。宏定义位于dac8830.h文件中。
//  /* 电压量程选择 跳帽接5V：0  跳帽接10V：1*/
//  #define VOLTAGE_RANGE 0
//
//  /* 电压输出模式 跳帽接0~+：0  跳帽接-~+：1*/
//  #define VOLTAGE_OUTPUT_MODE 1
//
//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "dac8830.h"
#include "key.h"
#include "exti.h"
#include "LED.h"
#include "system.h"
#include "DWT.h"
#include "MT_Procedure.h"


void call_back_btn1(void *user_env);

//-----------------------------------------------------------------
//-----------------------------------------------------------------
// 主程序
//-----------------------------------------------------------------
int main(void) {
  CPU_CACHE_Enable();      // 启用CPU缓存
  HAL_Init();              // 初始化HAL库
  MPU_Memory_Protection(); // 设置保护区域
  SystemClock_Config();    // 设置系统时钟,400Mhz
  DWT_Init();
  BSP_KEY_Init(BUTTON_KEY1);
  EXTI_Init(EXTI_KEY1);
	EXTI_Register(EXTI_KEY1, call_back_btn1, NULL);
  LED_Init();              // 初始化LED
  DAC8830_Init();

	// 红 - 停止
  LED_R_ON;
  LED_G_OFF;
  LED_B_OFF;
	
	DAC8830_Set_Direct_Current(0, DAC8830_CS1 | DAC8830_CS2);
  
  while(KEY_get(0) != KEY1_PRES);  // 等待开始
  
	// 红蓝 - 正在工作
  LED_R_ON;
  LED_G_OFF;
  LED_B_ON;

	MT_Procedure1();
  MT_Procedure2();

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
