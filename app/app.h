/*
 * app.h
 *
 *  Created on: Jun 20, 2022
 *      Author: lesly
 */

#ifndef APP_H_
#define APP_H_

#include <stdbool.h>

typedef struct ac_sm_tt ac_sm_t;

void app_tick_1ms(void);
void app_init(void);
void app_loop(void);

void ac_stw_down_callback(void);
void ac_stw_up_callback(void);
void ac_cool_set(bool state);
void ac_heat_set(bool state);
void ac_run_sm(ac_sm_t *sm);

#endif /* APP_H_ */
