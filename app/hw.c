/*
 * hw.c
 * Abstrai as funções do HAL e CMSIS
 *
 *  Created on: Jun 28, 2022
 *      Author: lesly
 */

#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "app.h"


void hw_cool_state_set(bool state){
	GPIO_PinState led_state = state ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(COOLER_GPIO_Port, COOLER_Pin, led_state);
}

void hw_heat_state_set(bool state){
	GPIO_PinState led_state = state ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, led_state);
}

void hw_led_toggle(void){
	HAL_GPIO_TogglePin(LED_STATE_GPIO_Port, LED_STATE_Pin);
}

void hw_cpu_sleep(){
	__WFI();
}

uint32_t hw_tick_ms_get(void){
	return HAL_GetTick();
}
