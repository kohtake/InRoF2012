/*
 * test.h
 *
 *  Created on: 2012/06/02
 *      Author: Ohtake Kazuki
 */

#ifndef TEST_H_
#define TEST_H_

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "module.h"
#include "define.h"

/* Servo 1 to 3 Move Test */
void servo_test(int mode);

/* LED */
void led_test(void);

/* Line Sensor */
void line_test(void);

/* Color Sensor */
void color_test(void);

/* PSD */
void psd_test(void);

/* Volume */
void volume_test(void);

#endif /* TEST_H_ */
