#include "dac8830.h"
#include "gpio.h"
#include "spi.h"

/**
 * @brief DAC8830数据传输
 *
 * @param data uint16_t 数据
 */
static void DAC8830_SPI_Write(uint16_t data, uint8_t cs_mask)
{
  uint8_t spi_data[2] = {0};

  spi_data[0] = (data >> 8) & 0xFF;
  spi_data[1] = data & 0xFF;

  if(cs_mask & DAC8830_CS1) HAL_GPIO_WritePin(DAC8830_CS1_GPIO_Port, DAC8830_CS1_Pin, GPIO_PIN_RESET);
  if(cs_mask & DAC8830_CS2) HAL_GPIO_WritePin(DAC8830_CS2_GPIO_Port, DAC8830_CS2_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi6, spi_data, sizeof(spi_data), 10);
  if(cs_mask & DAC8830_CS1) HAL_GPIO_WritePin(DAC8830_CS1_GPIO_Port, DAC8830_CS1_Pin, GPIO_PIN_SET);
  if(cs_mask & DAC8830_CS2) HAL_GPIO_WritePin(DAC8830_CS2_GPIO_Port, DAC8830_CS2_Pin, GPIO_PIN_SET);
}

/**
 * @brief DAC8830写DAC数据
 *
 * @param voltage double  电压
 */
static void DAC8830_DAC_Wrtie(double voltage, uint8_t cs_mask)
{
  uint16_t binary_voltage = 0;
#if VOLTAGE_RANGE
#if VOLTAGE_OUTPUT_MODE
  /* 跳帽接10V ±10V输出 */
  double real_voltage = (voltage + 10.0) / 8.0;
#else
  /* 跳帽接10V 0V-10V输出 */
  double real_voltage = voltage / 4.0;
#endif
#else
#if VOLTAGE_OUTPUT_MODE
  /* 跳帽接5V ±5V输出 */
  double real_voltage = (voltage + 5.0) / 4.0;
#else
  /* 跳帽接5V 0V-5V输出 */
  double real_voltage = voltage / 2.0;
#endif
#endif
  binary_voltage = (uint16_t)(real_voltage * 1000.0 * 0xFFFF / VREF);
  DAC8830_SPI_Write(binary_voltage, cs_mask);
}

/**
 * @brief DAC8830直流输出
 *
 * @param voltage double  电压
 */
void DAC8830_Set_Direct_Current(double voltage, uint8_t cs_mask) { DAC8830_DAC_Wrtie(voltage, cs_mask); }

/**
 * @brief DAC8830初始化
 *
 */
void DAC8830_Init(void) {
  MX_SPI6_Init();
  MX_GPIO_Init();

  HAL_GPIO_WritePin(DAC8830_CS1_GPIO_Port, DAC8830_CS1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(DAC8830_CS2_GPIO_Port, DAC8830_CS2_Pin, GPIO_PIN_SET);
}
