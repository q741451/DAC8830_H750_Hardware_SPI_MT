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
// ������
//-----------------------------------------------------------------
int main(void) {
  char buf[20] = {0};
  double voltage = MAX_VOLTAGE;
  uint8_t mode = OUTPUT_DC;
  CPU_CACHE_Enable();      // ����CPU����
  HAL_Init();              // ��ʼ��HAL��
  MPU_Memory_Protection(); // ���ñ�������
  SystemClock_Config();    // ����ϵͳʱ��,400Mhz
  SysTick_clkconfig(400);  // SysTick��������
  KEY_Init();              // ������ʼ��
  LCD_Init();              // LCD��ʼ��
  LCD_ShowString(0, 0, 464, 16, 16, "STM32 DAC8830 Test:");
  POINT_COLOR = RED; // �ʻ���ɫ
  LCD_ShowString(0, 16, 464, 16, 16, "K1 +1.0V  K2 -1.0V");
  LCD_ShowString(0, 32, 464, 16, 16, "K3 switch mode: DC");
  DAC8830_Init();
  /* ��ȡ�������� */
  double data_wave[32] = {0};
  double temp_f = 2 * 3.1415926535 / (sizeof(data_wave) / sizeof(double));
  for (int i = 0; i < sizeof(data_wave) / sizeof(double); i++) {
#if VOLTAGE_RANGE
#if VOLTAGE_OUTPUT_MODE
    /* ��ñ��10V ��10V��� 20VPP */
    data_wave[i] = MAX_VOLTAGE * sin(temp_f * i);
#else
    /* ��ñ��10V 0V-10V��� 10VPP */
    data_wave[i] = (MAX_VOLTAGE / 2) * sin(temp_f * i) + (MAX_VOLTAGE / 2);
#endif
#else
#if VOLTAGE_OUTPUT_MODE
    /* ��ñ��5V ��5V��� 10VPP */
    data_wave[i] = MAX_VOLTAGE * sin(temp_f * i);
#else
    /* ��ñ��5V 0V-5V��� 5VPP */
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
      /* ֱ����� */
      DAC8830_Set_Direct_Current(voltage);
    } else if (mode == OUTPUT_WAVE) {
      /* ������� */
      DAC8830_Set_Wave(data_wave, sizeof(data_wave) / sizeof(double));
    }
  }
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
