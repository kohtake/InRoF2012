/*
 * stddef.h
 *
 *  Created on: 2012/05/19
 *      Author: Ohtake Kazuki
 */

#ifndef DEFINE_H_
#define DEFINE_H_

/* Calibration Value */
volatile uint8_t LLineStdValue, RLineStdValue;
volatile uint16_t ColorStdValue;

/* Acceleration Table */
static const unsigned int Table[] = {
  3803,  3477,  3222,  3016,  2846,  2701,  2576,  2468,  2372,  2286,
  2209,  2139,  2076,  2017,  1964,  1914,  1869,  1826,  1786,  1748,
  1713,  1680,  1649,  1619,  1591,  1564,  1539,  1515,  1492,  1470,
  1449,  1428,  1409,  1390,  1373,  1355,  1339,  1323,  1307,  1292,
  1278,  1264,  1251,  1237,  1225,  1212,  1201,  1189,  1178,  1167,
  1156,  1146,  1136,  1126,  1116,  1107,  1098,  1089,  1080,  1072,
  1064,  1056,  1048,  1040,  1032,  1025,  1018,  1011,  1004,   997,
   990,   984,   977,   971,   965,   959,   953,   947,   941,   936,
   930,   925,   919,   914,   909,   904,   899,   894,   889,   885,
   880,   875,   871,   866,   862,   858,   853,   849,   845,   841,
   837,   833,   829,   825,   822,   818,   814,   810,   807,   803,
   800,   796,   793,   790,   786,   783,   780,   777,   773,   770,
   767,   764,   761,   758,   755,   752,   749,   747,   744,   741,
   738,   735,   733,   730,   727,   725,   722,   720,   717,   715,
   712,   710,   707,   705,   703,   700,   698,   696,   693,   691,
   689,   687,   685,   682,   680,   678,   676,   674,   672,   670,
   668,   666,   664,   662,   660,   658,   656,   654,   652,   650,
   648,   647,   645,   643,   641,   639,   638,   636,   634,   632,
   631,   629,   627,   626,   624,   622,   621,   619,   617,   616,
   614,   613,   611,   610,   608,   606,   605,   603,   602,   601,
};

static const unsigned int TableSize = sizeof(Table) / sizeof(Table[1]);

#define MAX_SPEED 600
#define MIN_SPEED 4000

/* A/D */
#define ADC_START() \
  do { \
    ADCSRA |= _BV(ADSC); \
    loop_until_bit_is_clear(ADCSRA, ADSC); \
  } while (0)
#define AD_CH_CLEAR() ADMUX &= ~(_BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0))

/* Servo Motor */
#define SERVO_MIN_3     12
#define SERVO_MAX_3     48
#define SERVO_MIN_1     3000
#define SERVO_MAX_1     4800
#define SERVO_MIN_2     1000
#define SERVO_MAX_2     5500
#define SERVO_DEFAULT_1 3800
#define SERVO_DEFAULT_2 4850
#define SERVO_DEFAULT_3 29

/* 指先 */
#define SERVO_CATCH_1   4500
#define SERVO_RELEASE_1 3800
#define SERVO_COLOR_1   4300

/* アーム */
#define SERVO_CATCH_2   1400
#define SERVO_RELEASE_2 4850
#define SERVO_COLOR_2   4200

/* 排出 */
#define SERVO_CATCH_3         30
#define SERVO_RELEASE_BLUE    32
#define SERVO_RELEASE_YELLOW  35
#define SERVO_RELEASE_RED     45

/* LED Color */
#define LED_RED_ON()    PORTD |= _BV(PD3)
#define LED_RED_OFF()   PORTD &= ~(_BV(PD3))
#define LED_GREEN_ON()  PORTC |= _BV(PC0)
#define LED_GREEN_OFF() PORTC &= ~(_BV(PC0))
#define LED_BLUE_ON()   PORTC |= _BV(PC1)
#define LED_BLUE_OFF()  PORTC &= ~(_BV(PC1))

/* Color Sensor */
#define COLOR_START() PORTA |= _BV(PA7)
#define COLOR_END()   PORTA &= ~(_BV(PA7))
#define RANGE_HIGH()  PORTA |= _BV(PA5)
#define RANGE_LOW()   PORTA &= ~(_BV(PA5))
#define CK_HIGH()     PORTA |= _BV(PA6)
#define CK_LOW()      PORTA &= ~(_BV(PA6))
#define GATE_TIME     200

/* PSD */
static const unsigned int LengthTable[] = {
  800, 800, 800, 800, 800, 800, 800, 800, 800, 800,
  800, 800, 800, 800, 800, 800, 800, 800, 800, 800,
  776, 734, 695, 660, 629, 600, 573, 548, 525, 504,
  485, 466, 449, 433, 419, 405, 391, 379, 367, 356,
  346, 336, 326, 317, 309, 301, 293, 286, 279, 272,
  265, 259, 253, 248, 242, 237, 232, 227, 223, 218,
  214, 210, 206, 202, 198, 194, 191, 187, 184, 181,
  178, 175, 172, 169, 167, 164, 161, 159, 156, 154,
  152, 150, 147, 145, 143, 141, 139, 137, 135, 134,
  132, 130, 128, 127, 125, 124, 122, 121, 119, 118,
  116, 115, 113, 112, 111, 110, 108, 107, 106, 105,
  104, 103, 101, 100,  99,  98,  97,  96,  95,  94,
   93,  92,  92,  91,  90,  89,  88,  87,  86,  86,
   85,  84,  83,  83,  82,  81,  80,  80,  79,  78,
   78,  77,  76,  76,  75,  74,  74,  73,  73,  72,
   71,  71,  70,  70,  69,  69,  68,  68,  67,  67,
   66,  66,  65,  65,  64,  64,  63,  63,  62,  62,
   61,  61,  61,  60,  60,  59,  59,  59,  58,  58,
   57,  57,  57,  56,  56,  56,  55,  55,  54,  54,
   54,  53,  53,  53,  52,  52,  52,  52,  51,  51,
   51,  50,  50,  50,  50,  50,  50,  50,  50,  50,
   50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
   50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
   50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
   50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
   50,  50,  50,  50,  50,
};

#define LENGTH_MIN    50
#define LENGTH_MAX    250
#define LENGTH_ARM    150
#define LENGTH_MARGIN 120

/* MARGIN OF ERROR */
#define MARGIN_STR    30
#define MARGIN_ROT    15

/* DC Motor */
#define DC_REVERSE() \
  do { \
    TCCR2A &= ~(_BV(COM2A0)); \
    PORTD &= ~(_BV(PD2)); \
  } while (0)
#define DC_NORMAL() \
  do { \
    TCCR2A |= _BV(COM2A0); \
    PORTD |= _BV(PD2); \
  } while (0)
#define DC_RED_GET    200
#define DC_BLUE_GET   60
#define DC_YELLOW_GET 130

/* Stepping Motor */
#define A_W_OFF() \
  do { \
    PORTB &= ~(_BV(PB3)); \
    PORTC &= ~(_BV(PC5)); \
  } while (0)
#define A_W_ON() \
  do { \
    PORTB |= _BV(PB3); \
    PORTC |= _BV(PC5); \
  } while (0)
#define STEP_FL(delay) \
  do { \
    PORTC &= ~(_BV(PC6)); \
    _delay_us(10); \
    PORTC |= _BV(PC6); \
    _delay_us(delay); \
  } while (0)
#define STEP_BL(delay) \
  do { \
    PORTC &= ~(_BV(PC7)); \
    _delay_us(10); \
    PORTC |= _BV(PC7); \
    _delay_us(delay); \
  } while (0)
#define STEP_FR(delay) \
  do { \
    PORTB &= ~(_BV(PB5)); \
    _delay_us(10); \
    PORTB |= _BV(PB5); \
    _delay_us(delay); \
  } while (0)
#define STEP_BR(delay) \
  do { \
    PORTB &= ~(_BV(PB4)); \
    _delay_us(10); \
    PORTB |= _BV(PB4); \
    _delay_us(delay); \
  } while (0)

#endif /* DEFINE_H_ */
