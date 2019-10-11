/*
 * test.c
 *
 *  Created on: 2012/06/02
 *      Author: Ohtake Kazuki
 */

#include "test.h"

void servo_test(int mode)
{
  uint16_t i;

  do {
    printf("Servo%d Value > ", mode);
    i = avrgeti();
    if (i == 0) break;
    switch (mode) {
    case 1:
    servo1(i);
      break;
    case 2:
      servo2(i);
      break;
    case 3:
      servo3(i);
      break;
    default:
      break;
    }
  } while (i);
}

void led_test(void)
{
  const unsigned int delay = 400;

  LED_RED_ON();
  _delay_ms(delay);

  LED_RED_OFF(); LED_BLUE_ON();
  _delay_ms(delay);

  LED_BLUE_OFF();	LED_GREEN_ON();
  _delay_ms(delay);

  LED_GREEN_OFF(); LED_RED_ON(); LED_BLUE_ON();
  _delay_ms(delay);

  LED_RED_OFF(); LED_GREEN_ON();
  _delay_ms(delay);

  LED_BLUE_OFF(); LED_RED_ON();
  _delay_ms(delay);

  LED_BLUE_ON();
  _delay_ms(delay);

  // LED_RED_OFF(); LED_BLUE_OFF(); LED_GREEN_OFF();
}

void line_test(void)
{
  printf("L:%d R:%d\r\n", read_lline(), read_rline());
}

void color_test(void)
{
  uint16_t red, green, blue;

  printf("Input Delay Time (ms)\r\n");
  printf(">");

  color_init(avrgeti());

  red = read_color();
  green = read_color();
  blue = read_color();

  if (red > ColorStdValue || green > ColorStdValue || blue > ColorStdValue) {
    printf("Ball is not found.\r\n");
  } else if (red > green && red > blue) {
    printf("Ball is RED.\r\n");
  } else if (blue > red && blue > green) {
    printf("Ball is BLUE.\r\n");
  } else {
    printf("Ball is YELLOW.\r\n");
  }

  printf("RED:%d GREEN:%d BLUE:%d\r\n", red, green, blue);
}

void psd_test(void)
{
  uint8_t value;

  value = read_psd();
  printf("PSD:%3d %2d\r\n", value, LengthTable[value]);
}

void volume_test(void)
{
  printf("Volime:%d\r\n", read_volume());
}
