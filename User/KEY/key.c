#include "key.h"
#include "DWT.h"

//-----------------------------------------------------------------

GPIO_TypeDef *BUTTON_PORT[BUTTONn] = {
    BUTTON_KEY1_GPIO_PORT, BUTTON_KEY2_GPIO_PORT, BUTTON_KEY3_GPIO_PORT,
    BUTTON_KEY4_GPIO_PORT};
const uint16_t BUTTON_PIN[BUTTONn] = {BUTTON_KEY1_PIN, BUTTON_KEY2_PIN,
                                      BUTTON_KEY3_PIN, BUTTON_KEY4_PIN};

uint32_t BSP_KEY_is_init_mask = 0;

void KEY_Init(void) {
  BSP_KEY_Init(BUTTON_KEY1); // KEY1按键PE0
  BSP_KEY_Init(BUTTON_KEY2); // KEY2按键PE1
  BSP_KEY_Init(BUTTON_KEY3); // KEY3按键PE2
  BSP_KEY_Init(BUTTON_KEY4); // KEY4按键PE3
}

void BSP_KEY_Init(Button_TypeDef button) {
  GPIO_InitTypeDef GPIO_InitStruct;
  BUTTONx_GPIO_CLK_ENABLE(button); // 开启KEY时钟

  GPIO_InitStruct.Pin = BUTTON_PIN[button];             // 配置KEY
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;               // 推挽输入
  GPIO_InitStruct.Pull = GPIO_PULLUP;                   // 上拉
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;    // 高速
  HAL_GPIO_Init(BUTTON_PORT[button], &GPIO_InitStruct); // 初始化KEY

  BSP_KEY_is_init_mask |= 1<<button;
}


u8 KEY_get(uint32_t ms)
{
  GPIO_PinState gpLast_KEY1 = (BSP_KEY_is_init_mask & (1<<BUTTON_KEY1)) ? KEY1 : GPIO_PIN_RESET;
  GPIO_PinState gpLast_KEY2 = (BSP_KEY_is_init_mask & (1<<BUTTON_KEY2)) ? KEY2 : GPIO_PIN_RESET;
  GPIO_PinState gpLast_KEY3 = (BSP_KEY_is_init_mask & (1<<BUTTON_KEY3)) ? KEY3 : GPIO_PIN_RESET;
  GPIO_PinState gpLast_KEY4 = (BSP_KEY_is_init_mask & (1<<BUTTON_KEY4)) ? KEY4 : GPIO_PIN_RESET;
  uint32_t uStartTime = HAL_GetTick();
  
  while(1)
  {
    if(BSP_KEY_is_init_mask & (1<<BUTTON_KEY1))
    {
      if(gpLast_KEY1 == GPIO_PIN_RESET)
      {
        if(KEY1 == GPIO_PIN_SET)
          return KEY1_PRES;
      }
      else
        gpLast_KEY1 = KEY1;
    }

    if(BSP_KEY_is_init_mask & (1<<BUTTON_KEY2))
    {
      if(gpLast_KEY2 == GPIO_PIN_RESET)
      {
        if(KEY2 == GPIO_PIN_SET)
          return KEY2_PRES;
      }
      else
        gpLast_KEY2 = KEY2;
    }
    
    if(BSP_KEY_is_init_mask & (1<<BUTTON_KEY3))
    {
      if(gpLast_KEY3 == GPIO_PIN_RESET)
      {
        if(KEY3 == GPIO_PIN_SET)
          return KEY3_PRES;
      }
      else
        gpLast_KEY3 = KEY3;
    }
    
    if(BSP_KEY_is_init_mask & (1<<BUTTON_KEY4))
    {
      if(gpLast_KEY4 == GPIO_PIN_RESET)
      {
        if(KEY4 == GPIO_PIN_SET)
          return KEY4_PRES;
      }
      else
        gpLast_KEY4 = KEY4;
    }
    
    if(ms > 0 && HAL_GetTick() - uStartTime > ms)
      break;
  }

  return 0;
}
