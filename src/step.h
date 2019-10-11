/*
 * step.h
 *
 *  Created on: 2012/06/10
 *      Author: Ohtake Kazuki
 */

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "define.h"
#include "module.h"

#ifndef STEP_H_
#define STEP_H_

#define CIRCLE 270.0  // タイヤの円周
#define STEPS  500    // 1回転のステップ数

static const double StepLength = CIRCLE / STEPS;  // ステップ距離
static const double LineRadius = 40.5;            // ラインセンサーの回転半径
static const double WheelRadius = 124.0;          // ロボットの回転半径

/* 1ステップ分の動作 */
void step_1f(void);
void step_1b(void);
void step_1l(void);
void step_1r(void);

/* 90度の旋回 */
void step_l_rotation(void);
void step_r_rotation(void);

/* 任意の角の旋回 */
void step_l_rotation_opt(int step);
void step_r_rotation_opt(int step);

/* 加減速をして指定距離(mm)進む */
void step_f(unsigned int length);
void step_b(unsigned int length);

/* 一定速度で指定距離(mm)進む */
void step_f_const(unsigned int length);
void step_b_const(unsigned int length);

/* フラッテン */
void step_flatten_line(void);
void step_b_flatten_line(void);
void step_flatten_cross(void);
void step_b_flatten_cross(void);

#endif /* STEP_H_ */
