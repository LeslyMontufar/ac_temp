/*
 * app.c
 *
 * Lesly Montúfar
 * 		Jun 28, 2022
 *
 *
*/

#include <stdint.h>
#include <stdbool.h>
#include "app.h"
#include "hw.h"

#define APP_DEBOUNCING_TIME_MS 	250		// com jumper o debouncing é maior
#define DELAY_IDLE 				500
#define DELAY_COOLING 			100
#define DELAY_HEATING 			1000

volatile float room_temp = 25.0;
volatile uint32_t delay = DELAY_IDLE;
bool app_started = false;

typedef enum {
	AC_INIT = 0,
	AC_IDLE,
	AC_HEAT,
	AC_COOL
} ac_states_t;

struct ac_sm_tt{
	ac_states_t state;
	float set_point_c;
	float histerese_c;
};

ac_sm_t ac_sm = {0};

void ac_stw_down_callback(void){

	static uint32_t debouncing_time_ms = 0;
	if((hw_tick_ms_get() - debouncing_time_ms) >= APP_DEBOUNCING_TIME_MS){
		room_temp -= 0.5;
		debouncing_time_ms = hw_tick_ms_get();
	}
}

void ac_stw_up_callback(void){

	static uint32_t debouncing_time_ms = 0;
	if((hw_tick_ms_get() - debouncing_time_ms) >= APP_DEBOUNCING_TIME_MS){
		room_temp += 0.5;
		debouncing_time_ms = hw_tick_ms_get();
	}
}

void ac_cool_set(bool state){
	hw_cool_state_set(state);
}

void ac_heat_set(bool state){
	hw_heat_state_set(state);
}

void app_tick_1ms(void){
	static uint32_t delay_cnt = 0;

	if(!app_started)
		return;

	delay_cnt++;

	if(delay_cnt >= delay){
		delay_cnt = 0;
		hw_led_toggle();
	}

}

void ac_run_sm(ac_sm_t *sm){

	switch(sm->state){
		case AC_INIT:
			sm->histerese_c = 2.0;
			sm->state = AC_IDLE;
			sm->set_point_c = 25.0;
			break;
		case AC_IDLE:
			delay = DELAY_IDLE;
			ac_cool_set(false);
			ac_heat_set(false);
			if(room_temp > (sm->set_point_c + sm->histerese_c))
				sm->state = AC_COOL;
			else if(room_temp < (sm->set_point_c - sm->histerese_c))
				sm->state = AC_HEAT;
			break;
		case AC_HEAT:
			delay = DELAY_HEATING;
			ac_heat_set(true);
			if(room_temp > sm->set_point_c)
				sm->state = AC_IDLE;
			break;
		case AC_COOL:
			delay = DELAY_COOLING;
			ac_cool_set(true);
			if(room_temp < sm->set_point_c)
				sm->state = AC_IDLE;
			break;
		default:
			sm->state = AC_INIT;
			break;
	}
}

void app_init(void){
	app_started = true;
	ac_sm.state = AC_INIT;
}

void app_loop(void){
	ac_run_sm(&ac_sm);
//	hw_cpu_sleep();
}
