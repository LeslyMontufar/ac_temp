/*
 * hw.h
 *
 *  Created on: Jun 20, 2022
 *      Author: lesly
 */

#ifndef HW_H_
#define HW_H_

void hw_delay_ms(uint32_t time_ms);
void hw_led_toggle(void);
void hw_cpu_sleep();
uint32_t hw_tick_ms_get(void);

#endif /* HW_H_ */
