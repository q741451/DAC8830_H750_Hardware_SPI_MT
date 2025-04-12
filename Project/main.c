//-----------------------------------------------------------------
// ��������:
//    DAC8830Ӳ��SPI ʵ��
// ��    ��: ���ǵ���
// ��ʼ����: 2024-04-20
// �������: 2024-04-20
//   - V1.0:  DAC8830Ӳ��SPI ʵ��
// ���Թ���: ����STM32���Ŀ����塢LZE_ST LINK2��2.8��Һ����DA8830ģ��
// ˵    ��:
// STM32H750         DA8830
//    5V                +5V
//    GND               GND
//    PA6               CS1
//    PA5               SCLK
//    PA7               SDIO
//    PA4              CS2
//
//  DAC8830��4�����ģʽ��ͨ���ı���ñ�ı�ģʽ��
//  ���ı���ñ����Ҫ�޸ĳ���ĺ궨�塣�궨��λ��dac8830.h�ļ��С�
//  /* ��ѹ����ѡ�� ��ñ��5V��0  ��ñ��10V��1*/
//  #define VOLTAGE_RANGE 0
//
//  /* ��ѹ���ģʽ ��ñ��0~+��0  ��ñ��-~+��1*/
//  #define VOLTAGE_OUTPUT_MODE 1
//
//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "dac8830.h"
#include "key.h"
#include "exti.h"
#include "LED.h"
#include "system.h"
#include "DWT.h"
#include <math.h>
#include <string.h>

void call_back_irq2(void *user_env)
{
  LED_G_Toggle;
}

void call_back_irq3(void *user_env)
{
  LED_R_Toggle;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
// ������
//-----------------------------------------------------------------
int main(void) {
  double voltage = 0;
  CPU_CACHE_Enable();      // ����CPU����
  HAL_Init();              // ��ʼ��HAL��
  MPU_Memory_Protection(); // ���ñ�������
  SystemClock_Config();    // ����ϵͳʱ��,400Mhz
  DWT_Init();
  BSP_KEY_Init(BUTTON_KEY1);
  EXTI_Init(EXTI_KEY2 | EXTI_KEY3);
  LED_Init();              // ��ʼ��LED
  DAC8830_Init();
  
  LED_R_OFF;
  LED_G_OFF;
  LED_B_ON;
  
  while(KEY_get(0) != KEY1_PRES);
  
  LED_R_OFF;
  LED_G_OFF;
  LED_B_OFF;
  
  EXTI_Register(EXTI_KEY2, call_back_irq2, NULL);
  EXTI_Register(EXTI_KEY3, call_back_irq3, NULL);

  DAC8830_Set_Direct_Current(voltage, DAC8830_CS1 | DAC8830_CS2);

  while (1)
  {
    DWT_Delay_ms(2000);
    voltage += 1.0;
    if (voltage > MAX_VOLTAGE)
      voltage = MIN_VOLTAGE;

    DAC8830_Set_Direct_Current(voltage, DAC8830_CS1);
  }

}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
