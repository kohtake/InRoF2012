/*
 * module.h
 *
 *  Created on: 2012/06/02
 *      Author: Ohtake Kazuki
 */

#ifndef MODULE_H_
#define MODULE_H_

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "define.h"

/* Servo Motor */
void servo1(uint16_t angle);
void servo2(uint16_t angle);
void servo3(uint8_t angle);

/* Line Sensor */
uint8_t read_rline(void);
uint8_t read_lline(void);

/* Color Sensor */

/* color_startを呼んだら，read_colorで値を読む */
void color_init(unsigned int delay);

/* RGBそれぞれ1回ずつ12ビットで値を返すため，計3回 呼ぶ */
uint16_t read_color(void);

/* PSD */
uint8_t read_psd(void);

/* Volume */
uint8_t read_volume(void);

#endif /* MODULE_H_ */
