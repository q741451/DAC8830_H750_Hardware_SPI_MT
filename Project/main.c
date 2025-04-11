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
#include "delay.h"
#include "key.h"
#include "lcd.h"
#include "system.h"
#include <math.h>
#include <string.h>

#define OUTPUT_DC 0
#define OUTPUT_WAVE 1
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// 主程序
//-----------------------------------------------------------------
int main(void) {
  char buf[20] = {0};
  double voltage = MAX_VOLTAGE;
  uint8_t mode = OUTPUT_DC;
  CPU_CACHE_Enable();      // 启用CPU缓存
  HAL_Init();              // 初始化HAL库
  MPU_Memory_Protection(); // 设置保护区域
  SystemClock_Config();    // 设置系统时钟,400Mhz
  SysTick_clkconfig(400);  // SysTick参数配置
  KEY_Init();              // 按键初始化
  LCD_Init();              // LCD初始化
  LCD_ShowString(0, 0, 464, 16, 16, "STM32 DAC8830 Test:");
  POINT_COLOR = RED; // 笔画颜色
  LCD_ShowString(0, 16, 464, 16, 16, "K1 +1.0V  K2 -1.0V");
  LCD_ShowString(0, 32, 464, 16, 16, "K3 switch mode: DC");
  DAC8830_Init();
  /* 获取波形数据 */
  double data_wave[32] = {0};
  double temp_f = 2 * 3.1415926535 / (sizeof(data_wave) / sizeof(double));
  for (int i = 0; i < sizeof(data_wave) / sizeof(double); i++) {
#if VOLTAGE_RANGE
#if VOLTAGE_OUTPUT_MODE
    /* 跳帽接10V ±10V输出 20VPP */
    data_wave[i] = MAX_VOLTAGE * sin(temp_f * i);
#else
    /* 跳帽接10V 0V-10V输出 10VPP */
    data_wave[i] = (MAX_VOLTAGE / 2) * sin(temp_f * i) + (MAX_VOLTAGE / 2);
#endif
#else
#if VOLTAGE_OUTPUT_MODE
    /* 跳帽接5V ±5V输出 10VPP */
    data_wave[i] = MAX_VOLTAGE * sin(temp_f * i);
#else
    /* 跳帽接5V 0V-5V输出 5VPP */
    data_wave[i] = (MAX_VOLTAGE / 2) * sin(temp_f * i) + (MAX_VOLTAGE / 2);
#endif
#endif
  }

  while (1) {
    switch (KEY_Scan(0)) {
    case KEY1_PRES:
      voltage += 1.0;
      if (voltage > MAX_VOLTAGE) {
        voltage = MIN_VOLTAGE;
      }
      break;
    case KEY2_PRES:
      voltage -= 1.0;
      if (voltage < MIN_VOLTAGE) {
        voltage = MAX_VOLTAGE;
      }
      break;
    case KEY3_PRES:
      if (mode == OUTPUT_DC) {
        mode = OUTPUT_WAVE;
        LCD_ShowString(0, 32, 464, 16, 16, "K3 switch mode: WAVE");
        LCD_ShowString(0, 48, 464, 16, 16, "                 ");
      } else if (mode == OUTPUT_WAVE) {
        mode = OUTPUT_DC;
        LCD_ShowString(0, 32, 464, 16, 16, "K3 switch mode: DC  ");
      }
      break;
    }
    if (mode == OUTPUT_DC) {
      LCD_ShowString(0, 48, 464, 16, 16, "DC value:");
      sprintf(buf, "%.2f v ", voltage);
      LCD_ShowString(80, 48, 464, 16, 16, buf);
      /* 直流输出 */
      DAC8830_Set_Direct_Current(voltage);
    } else if (mode == OUTPUT_WAVE) {
      /* 波形输出 */
      DAC8830_Set_Wave(data_wave, sizeof(data_wave) / sizeof(double));
    }
  }
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
