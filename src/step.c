/*
 * step.c
 *
 *  Created on: 2012/06/10
 *      Author: Ohtake Kazuki
 */

#include "step.h"

void step_1f(void)
{
  STEP_FL(MIN_SPEED);
  STEP_FR(MIN_SPEED);
}

void step_1b(void)
{
  STEP_BL(MIN_SPEED);
  STEP_BR(MIN_SPEED);
}

void step_1l(void)
{
  STEP_BL(MIN_SPEED);
  STEP_FR(MIN_SPEED);
}

void step_1r(void)
{
  STEP_FL(MIN_SPEED);
  STEP_BR(MIN_SPEED);
}

void step_l_rotation(void)
{
  int i;

  for (i = 0; i <= 182; i++) {
    if (i < TableSize / 2) {
      STEP_BL(Table[i]);
      STEP_FR(Table[i]);
    } else {
      STEP_BL(Table[TableSize / 2]);
      STEP_FR(Table[TableSize / 2]);
    }
  }
  for (i = 181; i >= 0; i--) {
    if (i < TableSize) {
      STEP_BL(Table[i]);
      STEP_FR(Table[i]);
    } else {
      STEP_BL(Table[TableSize / 2]);
      STEP_FR(Table[TableSize / 2]);
    }
  }
}

void step_r_rotation(void)
{
  int i;

  for (i = 0; i <= 182; i++) {
    if (i < TableSize / 2) {
      STEP_FL(Table[i]);
      STEP_BR(Table[i]);
    } else {
      STEP_FL(Table[TableSize / 2]);
      STEP_BR(Table[TableSize / 2]);
    }
  }
  for (i = 181; i >= 0; i--) {
    if (i < TableSize) {
      STEP_FL(Table[i]);
      STEP_BR(Table[i]);
    } else {
      STEP_FL(Table[TableSize / 2]);
      STEP_BR(Table[TableSize / 2]);
    }
  }
}

void step_l_rotation_opt(int step)
{
  int i;

  if ((step % 2) == 1) {
    STEP_FL(Table[0]);
    STEP_BR(Table[0]);
  }
  for (i = 0; i < step / 2; i++) {
    if (i < TableSize / 2) {
      STEP_BL(Table[i]);
      STEP_FR(Table[i]);
    } else {
      STEP_BL(Table[TableSize / 2]);
      STEP_FR(Table[TableSize / 2]);
    }
  }
  for (i = step / 2 - 1; i >= 0; i--) {
    if (i < TableSize) {
      STEP_BL(Table[i]);
      STEP_FR(Table[i]);
    } else {
      STEP_BL(Table[TableSize / 2]);
      STEP_FR(Table[TableSize / 2]);
    }
  };
}

void step_r_rotation_opt(int step)
{
  int i;

  if ((step % 2) == 1) {
    STEP_FL(Table[0]);
    STEP_BR(Table[0]);
  }
  for (i = 0; i < step / 2; i++) {
    if (i < TableSize / 2) {
      STEP_FL(Table[i]);
      STEP_BR(Table[i]);
    } else {
      STEP_FL(Table[TableSize / 2]);
      STEP_BR(Table[TableSize / 2]);
    }
  }
  for (i = step / 2 - 1; i >= 0; i--) {
    if (i < TableSize) {
      STEP_FL(Table[i]);
      STEP_BR(Table[i]);
    } else {
      STEP_FL(Table[TableSize / 2]);
      STEP_BR(Table[TableSize / 2]);
    }
  }
}

void step_f(unsigned int length)
{
  int i, step;

  step = (int)(length / StepLength);

  for (i = 0; i < step / 2; i++) {
    if (i < TableSize) {
      STEP_FL(Table[i]);
      STEP_FR(Table[i]);
    } else {
      STEP_FL(MAX_SPEED);
      STEP_FR(MAX_SPEED);
    }
  }
  for (i = step / 2 ; i >= 0; i--) {
    if (i < TableSize) {
      STEP_FL(Table[i]);
      STEP_FR(Table[i]);
    } else {
      STEP_FL(MAX_SPEED);
      STEP_FR(MAX_SPEED);
    }
  }
}

void step_b(unsigned int length)
{
  int i, step;

  step = (int)(length / StepLength);

  for (i = 0; i < step / 2; i++) {
    if (i < TableSize) {
      STEP_BL(Table[i]);
      STEP_BR(Table[i]);
    } else {
      STEP_BL(MAX_SPEED);
      STEP_BR(MAX_SPEED);
    }
  }
  for (i = step / 2 ; i >= 0; i--) {
    if (i < TableSize) {
      STEP_BL(Table[i]);
      STEP_BR(Table[i]);
    } else {
      STEP_BL(MAX_SPEED);
      STEP_BR(MAX_SPEED);
    }
  }
}

void step_f_const(unsigned int length)
{
  int i, step;

  step = (int)(length / StepLength);

  for (i = 0; i < step; i++) {
    STEP_FL(MIN_SPEED);
    STEP_FR(MIN_SPEED);
  }
}

void step_b_const(unsigned int length)
{
  int i, step;

  step = (int)(length / StepLength);

  for (i = 0; i < step; i++) {
    STEP_BL(MIN_SPEED);
    STEP_BR(MIN_SPEED);
  }
}

void step_flatten_line(void)
{
  int i, step;
  int count = 0;  // ステップ数のカウンター
  double length;  // タイヤの移動距離
  const unsigned int delay = 300;  // 動作ごとの間隔

  /* 片方がラインを読んだらストップ */
  while (1) {
    if (read_lline() > LLineStdValue || read_rline() > RLineStdValue) {
      count++;
      break;
    }
    step_1f();
  }

  if (read_lline() >= read_rline()) {
    /* もう片方がラインを読むまでカウント */
    while (1) {
      if (read_rline() > RLineStdValue) break;
      step_1f();
      count++;
    }

    _delay_ms(delay);

    /* 半分の距離だけ後退 */
    step = count / 2;
    for (i = 0; i < step; i++) step_1b();

    _delay_ms(delay);

    /* 旋回するステップの算出 */
    length = WheelRadius * atan2(step * StepLength, LineRadius);
    step = (int)(length / StepLength);

    /* 旋回 */
    for (i = 0; i < step; i++) step_1l();
  } else {
    /* 以降，左右逆の場合 */
    while (1) {
      if (read_lline() > LLineStdValue) {
        break;
      }
      step_1f();
      count++;
    }

    _delay_ms(delay);

    step = count / 2;
    for (i = 0; i < step; i++) step_1b();

    _delay_ms(delay);

    length = WheelRadius * atan2(step * StepLength, LineRadius);
    step = (int)(length / StepLength);

    for (i = 0; i < step; i++) step_1r();
  }
}

void step_b_flatten_line(void)
{
  int i, step;
  int count = 0;  // ステップ数のカウンター
  double length;  // タイヤの移動距離
  const unsigned int delay = 300;  // 動作ごとの間隔

  /* 片方がラインを読んだらストップ */
  while (1) {
    if (read_lline() > LLineStdValue || read_rline() > RLineStdValue) {
      count++;
      break;
    }
    step_1b();
  }

  if (read_lline() >= read_rline()) {
    /* もう片方がラインを読むまでカウント */
    while (1) {
      if (read_rline() > RLineStdValue) break;
      step_1b();
      count++;
    }

    _delay_ms(delay);

    /* 半分の距離だけ後退 */
    step = count / 2;
    for (i = 0; i < step; i++) step_1f();

    _delay_ms(delay);

    /* 旋回するステップの算出 */
    length = WheelRadius * atan2(step * StepLength, LineRadius);
    step = (int)(length / StepLength);

    /* 旋回 */
    for (i = 0; i < step; i++) step_1r();
  } else {
    /* 以降，左右逆の場合 */
    while (1) {
      if (read_lline() > LLineStdValue) {
        break;
      }
      step_1b();
      count++;
    }

    _delay_ms(delay);

    step = count / 2;
    for (i = 0; i < step; i++) step_1f();

    _delay_ms(delay);

    length = WheelRadius * atan2(step * StepLength, LineRadius);
    step = (int)(length / StepLength);

    for (i = 0; i < step; i++) step_1l();
  }
}

void step_flatten_cross(void)
{
  const unsigned int delay = 300;

  step_flatten_line();

  _delay_ms(delay);
  step_r_rotation();

  _delay_ms(delay);
  step_b_const(50);

  _delay_ms(delay);
  step_flatten_line();

  _delay_ms(delay);
  step_l_rotation();
}

void step_b_flatten_cross(void)
{
  const unsigned int delay = 300;

  step_b_flatten_line();

  _delay_ms(delay);
  step_r_rotation();

  _delay_ms(delay);
  step_b_const(50);

  _delay_ms(delay);
  step_flatten_line();

  _delay_ms(delay);
  step_l_rotation();
}
