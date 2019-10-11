/*
 * module.c
 *
 *  Created on: 2012/06/02
 *      Author: Ohtake Kazuki
 */

#include "module.h"

void servo1(uint16_t angle)
{
  if (angle < SERVO_MIN_1) {
    OCR1B = SERVO_MIN_1;
  } else if (angle > SERVO_MAX_1) {
    OCR1B = SERVO_MAX_1;
  } else {
    OCR1B = angle;
  }
}

void servo2(uint16_t angle)
{
  const unsigned int delay = 1;

  if (angle < SERVO_MIN_2) {
    angle = SERVO_MIN_2;
  } else if (angle > SERVO_MAX_2) {
    angle = SERVO_MAX_2;
  }

  if (OCR1A > angle) {
    while (OCR1A > angle) {
      OCR1A--;
      _delay_ms(delay);
    }
  } else if (OCR1A < angle) {
    while (OCR1A < angle) {
      OCR1A++;
      _delay_ms(delay);
    }
  }

}

void servo3(uint8_t angle)
{
  if (angle < SERVO_MIN_3) {
    OCR2B = SERVO_MIN_3;
  } else if (angle > SERVO_MAX_3) {
    OCR2B = SERVO_MAX_3;
  } else {
    OCR2B = angle;
  }
}

uint8_t read_rline(void)
{
  AD_CH_CLEAR();
  ADMUX |= _BV(MUX1);
  ADC_START();

  return ADCH;
}

uint8_t read_lline(void)
{
  AD_CH_CLEAR();
  ADMUX |= _BV(MUX1) | _BV(MUX0);
  ADC_START();

  return ADCH;
}

void color_init(unsigned int delay)
{
  COLOR_START();
  _delay_ms(delay);

  COLOR_END();
  _delay_us(3);
}

uint16_t read_color(void)
{
  uint8_t i;
  uint16_t result = 0;

  for (i = 0; i < 12; i++) {
    CK_HIGH();
    _delay_us(1);
    if (bit_is_set(PINA, 4)) {
      result |= (1 << i);
    }
    CK_LOW();
    _delay_us(1);
  }

  _delay_us(3);

  return result;
}

uint8_t read_psd(void)
{
  AD_CH_CLEAR();
  ADMUX |= _BV(MUX0);
  ADC_START();

  return ADCH;
}

uint8_t read_volume(void)
{
  AD_CH_CLEAR();
  ADC_START();

  return ADCH;
}
