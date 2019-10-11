/*
 * main.c
 *
 *  Created on: 2012/05/15
 *      Author: Ohtake Kazuki
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usart.h"
#include "step.h"
#include "test.h"
#include "module.h"
#include "define.h"

static uint8_t BallData[600] = {0};
static uint8_t PeakSearch[600] = {0};

/* Counter */
volatile static int TerminateTime;
volatile static int SecondCount;
volatile static int RedCount, BlueCount, YellowCount;

/* PID */
volatile static uint8_t TargetValue = DC_YELLOW_GET;

void io_init(void);

void test_mode(void);
void main_mode(void);

/* 車庫入れ */
void robot_init(void);

/* ボールエリアへ*/
void robot_go_to_ball_area(void);

/*　ボールを取って，一定時間後元に戻る */
void robot_advance_ball_area(void);

/* ゴールへ */
void robot_come_back(void);

/* ボールサーチ */
int robot_ball_search(void);

/* リセット */
void robot_restart(void);

/* 補助的関数 */
void calibrate_line(void);
void calibrate_color(void);

/* ボールサーチ時のフィルター */
void average_filter(void);
void low_path_filter(void);

int main(void)
{
  io_init();

  led_test();

  printf("\r\nAVR IS INITIALIZED.\r\n");

  while (1) {
    printf("\r\n");
    printf("(1) Test Program\r\n");
    printf("(2) Main Program\r\n");
    printf(">");

    switch (avrgeti()) {
    case 1:
      test_mode();
      break;
    case 2:
      main_mode();
      break;
    default:
      break;
    }

    /* ステッパー電源OFF */
    _delay_ms(500);
    A_W_OFF();
  }

  return 0;
}

void io_init(void)
{
  /* Counter */
  TerminateTime = 60;
  SecondCount = 0;
  RedCount = 0;
  BlueCount = 0;
  YellowCount = 0;

  /* USART */
  usart_init(38400);

  /* IO Resister */
  DDRA = 0b11100000;
  DDRB = 0b00111110;
  DDRC = 0b11111011;
  DDRD = 0b11111100;

  /* For Color Sensor */
  PORTA = _BV(PA5);

  /* For Stepping Motor */
  PORTB = _BV(PB5) | _BV(PB4) | _BV(PB2) | _BV(PB1);
  PORTC = _BV(PC7) | _BV(PC6) | _BV(PC4) | _BV(PC3);

  /* PWM 16bit for Servo Motor */
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS11);
  TIMSK1 = _BV(TOIE1);
  ICR1 = 50000;
  OCR1A = SERVO_DEFAULT_2;
  OCR1B = SERVO_DEFAULT_1;

  /* PWM 8bit for Servo and DC Motor */
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
  OCR2A = 0;
  OCR2B = SERVO_DEFAULT_3;

  /* A/D Resister */
  ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
  ADMUX = _BV(REFS0) | _BV(ADLAR);
  DIDR0 = _BV(ADC3D) | _BV(ADC2D) | _BV(ADC1D) | _BV(ADC0D);

  /* Calibration Value */
  LLineStdValue = 100;
  RLineStdValue = 100;
  ColorStdValue = 2000;

  sei();
}

void test_mode(void)
{
  printf("\r\n");
  printf("(01) Servo1 Test\r\n");
  printf("(02) Servo2 Test\r\n");
  printf("(03) Servo3 Test\r\n");
  printf("(04) LED Test\r\n");
  printf("(05) Line Sensor test\r\n");
  printf("(06) Color Sensor test\r\n");
  printf("(07) PSD Test\r\n");
  printf("(08) Volume Test\r\n");
  printf("(09) Timer Test\r\n");
  printf("(10) PID Test\r\n");
  printf("(11) Go To Garage\r\n");
  printf("(12) Go To Ball Area\r\n");
  printf("(13) Ball Search\r\n");
  printf("(14) Come Back Home\r\n");
  printf(">");

  switch (avrgeti()) {
  case 1:
    servo_test(1);
    break;
  case 2:
    servo_test(2);
    break;
  case 3:
    servo_test(3);
    break;
  case 4:
    while (1) {
      led_test();
      _delay_ms(200);
    }
    break;
  case 5:
    while (1) {
      line_test();
      _delay_ms(100);
    }
    break;
  case 6:
    while (1) color_test();
    break;
  case 7:
    while (1) {
      psd_test();
      _delay_ms(100);
    }
    break;
  case 8:
    while (1) {
      volume_test();
      _delay_ms(100);
    }
    break;
  case 9:
    while (1) {
      printf(">");
      avrgetc();
      printf("Timer Count : %d(sec)\r\n", SecondCount);
    }
  case 10:
    while (1) {
      printf("Target Value");
      printf(">");
      TargetValue = avrgeti();
    }
  case 11:
    robot_init();
    break;
  case 12:
    A_W_ON();
    _delay_ms(500);
    robot_go_to_ball_area();
    break;
  case 13:
    A_W_ON();
    _delay_ms(500);
    robot_advance_ball_area();
    break;
  case 14:
    A_W_ON();
    _delay_ms(500);
    robot_come_back();
  default:
    break;
  }
}

void main_mode(void)
{
  int count = 0;

  printf("Main Program Start\r\n");

  do {
  /* 車庫入れ */
  robot_init();

  TerminateTime = 540;

  if (count == 0 || SecondCount > TerminateTime) {
    printf("Please Enter End Time\r\n");
    printf(">");
    avrgetc();
    count++;
    SecondCount = 0;
  }

  /* ボールエリアまで行く */
  robot_go_to_ball_area();

  /* ボールエリアでの動作 */
  robot_advance_ball_area();

  /* 一定時間後，戻ってくる */
  robot_come_back();
  } while (1);
}

void robot_init(void)
{
  const unsigned int delay = 200;

  /* ステッパー電源ON，以降main_modeで使用する関数ではOFFにしない */
  A_W_ON();

  /* キャリブレーション */
  _delay_ms(delay);
  calibrate_line();
  calibrate_color();
  printf("Left:%d, Right:%d, Color:%d\r\n", LLineStdValue, RLineStdValue, ColorStdValue);

  /* ラインを読んでフラッテン */
  _delay_ms(delay);
  step_flatten_line();

  /* 旋回 */
  _delay_ms(delay);
  step_r_rotation();

  /* 後ろ向きに車庫入れ */
  _delay_ms(delay);
  step_b_const(200);

  /* ラインを読んで停止 */
  while (1) {
    if (read_lline() > LLineStdValue || read_rline() > RLineStdValue) break;
    step_1b();
  }

  /* ラインから一定距離後退 */
  step_b_const(200);
}

void robot_go_to_ball_area(void)
{
  const unsigned int delay = 500;

  /* 端まで前進 */
  _delay_ms(delay);
  step_f(1750);

  /* 旋回 */
  _delay_ms(delay);
  step_r_rotation();

  /* フラッテン */
  servo3(SERVO_RELEASE_YELLOW);
  _delay_ms(delay);
  servo3(SERVO_CATCH_3);
  step_f(200);
  step_flatten_line();

  /* 端まで前進 */
  _delay_ms(delay);
  step_f(740);

  /* 旋回 */
  _delay_ms(delay);
  step_r_rotation();

  /* フラッテン */
  _delay_ms(delay);
  step_f(150);
  step_flatten_cross();
}

void robot_advance_ball_area(void)
{
  int ad_counter = 0;
  int ball_counter = 0;
  const unsigned int delay = 200;

  RedCount = BlueCount = YellowCount = 0;

  while (SecondCount < TerminateTime && ad_counter < 9 && ball_counter < 9) {
    if (robot_ball_search() == 1) {
      _delay_ms(delay);
      step_f(100);
      ad_counter++;
    }
    ball_counter = BlueCount + RedCount + YellowCount;
  }

  /* フラッテン */
  _delay_ms(delay);
  step_r_rotation();

  _delay_ms(delay);
  step_b_const(100);

  _delay_ms(delay);
  step_flatten_line();

  _delay_ms(delay);
  step_l_rotation();

  /* 戻る */

  step_b((ad_counter - 1) * 100);

  _delay_ms(delay);
  step_b_flatten_cross();
}

void robot_come_back(void)
{
  const unsigned int delay = 200;
  const unsigned int delay_goal = 1500;

  _delay_ms(delay);
  step_b(250);

  _delay_ms(delay);
  step_l_rotation();

  _delay_ms(delay);
  step_b(690);

  _delay_ms(delay);
  step_b_flatten_line();

  _delay_ms(delay);
  step_b(400);

  /* Blue Goal */

  servo3(SERVO_RELEASE_BLUE);
  _delay_ms(delay_goal);
  step_b(20);
  step_f(20);
  _delay_ms(delay_goal);

  _delay_ms(delay);
  step_f(100);

  _delay_ms(delay);
  step_r_rotation();

  _delay_ms(delay);
  step_f(600);

  _delay_ms(delay);
  step_flatten_line();

  _delay_ms(delay);
  step_l_rotation();

  _delay_ms(delay);
  step_b(100);

  /* Yellow Goal */

  servo3(SERVO_RELEASE_YELLOW);
  _delay_ms(delay_goal);
  step_b(20);
  step_f(20);
  _delay_ms(delay_goal);

  _delay_ms(delay);
  step_flatten_line();

  _delay_ms(delay);
  step_r_rotation();

  _delay_ms(delay);
  step_f(600);

  _delay_ms(delay);
  step_flatten_line();

  _delay_ms(delay);
  step_l_rotation();

  _delay_ms(delay);
  step_b(100);

  /* Red Goal */

  servo3(SERVO_RELEASE_RED);
  _delay_ms(delay_goal);
  step_b(20);
  step_f(20);
  _delay_ms(delay_goal);

  _delay_ms(delay);
  step_f(100);

  _delay_ms(delay);
  step_l_rotation();

  _delay_ms(delay);
  step_f(150);

  _delay_ms(delay);
  step_l_rotation();

  _delay_ms(delay);
  step_b(50);
}

/* ボール発見で0,未発見で1 */
int robot_ball_search(void)
{
  int i, step_rot, step_back, length_arm, length_back;
  uint8_t max = 0;
  uint16_t red, green, blue;
  const unsigned int delay = 200;
  const unsigned int ball_max = 3;

  _delay_ms(delay);
  step_l_rotation_opt(300);

  _delay_ms(delay);
  for (i = 0; i < 600; i++) {
    step_1r();
    BallData[i] = read_psd();
  }

  /* ローパスフィルターを通す */
  low_path_filter();

  /* 最大値とその時のステップを求める */
  max = PeakSearch[0];
  step_rot = 0;
  for (i = 1; i < 600; i++) {
    if (PeakSearch[i] > max) {
      max = PeakSearch[i];
      step_rot = i;
    }
  }

  /* 最大のステップの時の距離で場合分け */
  max = BallData[step_rot];
  if (LengthTable[max] <= LENGTH_MIN || LengthTable[max] >= LENGTH_MAX) {
    printf("Ball is not found : %2d\r\n", LengthTable[max]);
    step_l_rotation_opt(300);
    return 1;
  } else {
    printf("Ball is found : %2d\r\n", LengthTable[max]);
  }

  /* ボールの方向を向く */
  _delay_ms(delay);
  step_rot = step_rot - MARGIN_ROT;
  step_l_rotation_opt(600 - step_rot);

  length_arm = LengthTable[max] - (LENGTH_ARM + LENGTH_MARGIN);

  if (length_arm >= 0) {
    step_f_const(length_arm);
  } else {
    step_b_const(abs(length_arm));
  }

  /* ボールキャッチ to 取り込み */
  servo1(SERVO_RELEASE_1);
  servo2(SERVO_CATCH_2);
  step_f_const(LENGTH_MARGIN);
  servo1(SERVO_CATCH_1);

  /* カラー識別 */
  _delay_ms(delay);
  servo2(SERVO_COLOR_2);
  servo1(SERVO_COLOR_1);

  _delay_ms(delay);
  color_init(GATE_TIME);

  red = read_color();
  green = read_color();
  blue = read_color();

  if (red > ColorStdValue || green > ColorStdValue || blue > ColorStdValue) {
    printf("Ball is not found.\r\n");
    TargetValue = DC_BLUE_GET;
    BlueCount++;
  } else if (red > green && red > blue) {
    printf("Ball is RED.\r\n");
    if (RedCount < ball_max) {
      TargetValue = DC_RED_GET;
      RedCount++;
    } else {
      if (BlueCount > YellowCount) {
        TargetValue = DC_BLUE_GET;
        BlueCount++;
      } else {
        TargetValue = DC_YELLOW_GET;
        YellowCount++;
      }
    }
  } else if (blue > red && blue > green) {
    printf("Ball is BLUE.\r\n");
    if (BlueCount < ball_max + 1) {
      TargetValue = DC_BLUE_GET;
      BlueCount++;
    } else {
      if (YellowCount > BlueCount) {
        TargetValue = DC_YELLOW_GET;
        YellowCount++;
      } else {
        TargetValue = DC_BLUE_GET;
        BlueCount++;
      }
    }
  } else {
    printf("Ball is YELLOW.\r\n");
    if (YellowCount < ball_max) {
      TargetValue = DC_YELLOW_GET;
      YellowCount++;
    } else {
      if (BlueCount > RedCount) {
        TargetValue = DC_BLUE_GET;
        BlueCount++;
      } else {
        TargetValue = DC_RED_GET;
        RedCount++;
      }
    }
  }
  /* カラー識別終了 */

  _delay_ms(delay * 2);
  servo2(SERVO_RELEASE_2);
  servo1(SERVO_RELEASE_1);

  /* 元の位置に戻る */
  length_back = length_arm + LENGTH_MARGIN;

  if (length_back >= 0) {
    step_b(length_back);
  } else {
    step_f(abs(length_back));
  }

  _delay_ms(delay);

  step_back = (600 - step_rot) - 300;
  if (step_back >= 0) {
    step_r_rotation_opt(step_back);
  } else {
    step_l_rotation_opt(abs(step_back));
  }

  return 0;
}

void calibrate_line(void)
{
  int i;
  uint8_t max_lline, max_rline, min_lline, min_rline, lline, rline;

  max_lline = min_lline = read_lline();
  max_rline = min_rline = read_rline();
  for (i = 0; i < 200; i++) {
    step_1f();
    lline = read_lline();
    rline = read_rline();
    if (lline > max_lline) max_lline = lline;
    if (lline < min_lline) min_lline = lline;
    if (rline > max_rline) max_rline = rline;
    if (rline < min_rline) min_rline = rline;
  }

  _delay_ms(200);
  for (i = 0; i < 200; i++) {
    step_1b();
  }

  LLineStdValue = (max_lline + min_lline) / 3;
  RLineStdValue = (max_rline + min_rline) / 3;
}

void calibrate_color(void)
{
  int i;
  uint16_t color, color_max = 0;

  servo2(SERVO_COLOR_2);
  color_init(GATE_TIME);
  for (i = 0; i < 3; i++) {
    color = read_color();
    if (color > color_max) color_max = color;
  }
  servo2(SERVO_DEFAULT_2);

  ColorStdValue = (uint16_t)(color_max * 0.9);

}

void average_filter(void)
{
  int i, j, sum;
  const int n = 3;  // 平均を取るときの幅

  sum = 0;
  for (i = n; i < 600 - n; i++) {
    for (j = 1; j <= n; j++) {
      sum = BallData[i + j] + BallData[i - j];
    }
    PeakSearch[i] = (BallData[i] + sum) / (2 * n + 1);
  }
}

void low_path_filter(void)
{
  int i;
  const double c = 0.9;

  PeakSearch[0] = BallData[0];

  for (i = 1; i < 600; i++) {
    PeakSearch[i] = (uint8_t)(c * PeakSearch[i - 1] + (1 - c) * BallData[i]);
  }
}

ISR(TIMER1_OVF_vect)
{
  static uint8_t time = 1;
  int delta, gain;
  const int p = 3;

  if (++time >= 50) {
    time = 1;
    SecondCount++;
    // printf("Second : %d\r\n", SecondCount);
  }

  delta = TargetValue - read_volume();
  gain = abs(p * delta);
  if (delta >= 0) {
    DC_NORMAL();
    OCR2A = gain < 64 ? gain : 64;
  } else {
    DC_REVERSE();
    OCR2A = gain < 64 ? gain : 64;
  }
}
