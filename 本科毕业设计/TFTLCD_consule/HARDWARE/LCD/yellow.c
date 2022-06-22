#include "picture.h"

const unsigned char gImage_yellow[768] = { 0X10,0X10,0X00,0X14,0X00,0X13,0X01,0X1B,
0XB5,0XD6,0XB5,0XD6,0XB5,0XD6,0XB5,0XD6,0XB5,0XB6,0XB5,0X96,0XAD,0X76,0XB5,0X2F,
0XF6,0X87,0XFE,0X83,0XFE,0X44,0XEE,0X27,0XBD,0X4E,0XA5,0X75,0XB5,0X96,0XB5,0XD6,
0XB5,0XD6,0XB5,0XD6,0XB5,0XD6,0XB5,0XD6,0XB5,0XD6,0XB5,0XD6,0XB5,0XD6,0XB5,0XB6,
0XAD,0X55,0XF7,0XA6,0XFF,0XE1,0XFF,0XE2,0XFF,0XA2,0XFE,0XE2,0XFE,0X21,0XFD,0XE2,
0XF5,0XC1,0XED,0XA2,0XE5,0X64,0XAD,0X56,0XB5,0XB5,0XB5,0XD6,0XB5,0XD6,0XB5,0XD6,
0XB5,0XD6,0XB5,0XD6,0XB5,0XB5,0XAD,0X6F,0XFF,0XE1,0XFF,0XE2,0XFF,0XEC,0XFF,0XF6,
0XFF,0XEB,0XFF,0X43,0XFE,0X40,0XED,0X80,0XE5,0X20,0XDD,0X20,0XDD,0X01,0XD4,0XE1,
0XB4,0XEE,0XB5,0X95,0XB5,0XD6,0XB5,0XD6,0XB5,0XD6,0XB5,0XB5,0XB5,0X4D,0XFF,0XE1,
0XFF,0XE2,0XFF,0XE9,0XFF,0XE5,0XFE,0XE0,0XFE,0X40,0XFD,0XE0,0XF5,0X80,0XE5,0X40,
0XD5,0X00,0XCC,0XA0,0XC4,0XA0,0XC4,0X81,0XB4,0X42,0XAC,0XCC,0XB5,0X96,0XB5,0XD6,
0XB5,0XB6,0XAD,0X34,0XFF,0XE1,0XFF,0XE1,0XFF,0XE1,0XFF,0X60,0XFE,0X80,0XFD,0XE0,
0XE5,0X60,0XDD,0X20,0XDD,0X00,0XDD,0X00,0XD5,0X00,0XCC,0XA0,0XB4,0X20,0XB4,0X20,
0XAC,0X00,0X9B,0XA1,0XAD,0X53,0XB5,0XB6,0XAD,0X95,0XFF,0XC3,0XFF,0XE1,0XFF,0XE0,
0XFF,0X80,0XFE,0X80,0XED,0X80,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,
0XD5,0X00,0XD4,0XC0,0XBC,0X60,0XA3,0XE0,0X9B,0XA0,0X93,0X61,0X8B,0X41,0XAD,0X75,
0XA5,0X54,0XFF,0XE1,0XFF,0XE0,0XFF,0XE0,0XFE,0XA0,0XED,0X80,0XD5,0X00,0XD5,0X00,
0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XCC,0XC0,0XAC,0X00,
0X8B,0X60,0X83,0X20,0X72,0XC1,0XA5,0X34,0XE7,0X2C,0XFF,0XE2,0XFF,0XE0,0XFF,0X60,
0XFE,0X20,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,
0XD5,0X00,0XD5,0X00,0XD4,0XE0,0XB4,0X40,0X8B,0X40,0X7A,0XE0,0X62,0X61,0X7B,0X47,
0XFF,0XC4,0XFF,0XE1,0XFF,0XE1,0XFE,0XE0,0XED,0X80,0XD5,0X00,0XD5,0X00,0XD5,0X00,
0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XC4,0X80,
0X93,0X60,0X72,0XA0,0X5A,0X21,0X62,0X43,0XFF,0XE3,0XFF,0XE2,0XFF,0XE4,0XFE,0XA0,
0XED,0X60,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,
0XDD,0X23,0XD5,0X00,0XD5,0X00,0XCC,0XA0,0X8B,0X40,0X62,0X40,0X49,0XC1,0X5A,0X21,
0XFF,0XC4,0XFF,0XE3,0XFF,0XED,0XFE,0XA0,0XED,0X60,0XD5,0X00,0XD5,0X00,0XD5,0X00,
0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XC4,0X60,
0X83,0X00,0X52,0X00,0X49,0XC1,0X62,0X41,0XEF,0X09,0XFF,0XE2,0XFF,0XF6,0XFE,0XA0,
0XF5,0XA0,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,
0XD5,0X00,0XD5,0X00,0XD5,0X00,0XAC,0X00,0X6A,0X80,0X49,0XA0,0X49,0XC1,0X73,0X06,
0XAD,0X34,0XFF,0XC2,0XFF,0XED,0XFF,0X63,0XFD,0XE0,0XDD,0X20,0XD5,0X00,0XD5,0X00,
0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XCC,0XA0,0X8B,0X40,
0X51,0XC0,0X41,0XA0,0X49,0XE2,0XA5,0X12,0XAD,0X75,0XFE,0XE3,0XFF,0X62,0XFF,0XC4,
0XFD,0XE0,0XE5,0X60,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,0XD5,0X00,
0XD5,0X00,0XCC,0XC0,0X9B,0XA0,0X62,0X40,0X41,0XC0,0X49,0XA1,0X5A,0X22,0XA5,0X75,
0XB5,0XB6,0XAD,0X12,0XFE,0X81,0XFE,0X61,0XFD,0XE0,0XE5,0X60,0XDD,0X20,0XD4,0XE0,
0XD4,0XE0,0XD5,0X00,0XD5,0X00,0XD4,0XE0,0XBC,0X60,0X9B,0XA0,0X72,0XA0,0X49,0XC0,
0X49,0XA0,0X49,0XC1,0XA4,0XF1,0XB5,0X96,0XB5,0XD7,0XAD,0X75,0XE5,0XEB,0XFE,0X21,
0XF5,0XA0,0XDD,0X20,0XD4,0XE0,0XC4,0XA0,0XBC,0X80,0XBC,0X60,0XB4,0X20,0XA3,0XC0,
0X8B,0X40,0X72,0XA0,0X5A,0X00,0X49,0XA0,0X49,0XC1,0X7B,0X48,0XAD,0X75,0XB5,0XD7,
0XBD,0XD7,0XB5,0XD6,0XAD,0X56,0XD5,0XCA,0XF5,0XC2,0XE5,0X61,0XD4,0XE0,0XC4,0X80,
0XAC,0X20,0XA3,0XC0,0X93,0X80,0X8B,0X40,0X7A,0XE0,0X62,0X60,0X51,0XE1,0X51,0XE1,
0X7B,0X47,0XAD,0X75,0XB5,0XD6,0XBD,0XD7,0XBD,0XD7,0XBD,0XD7,0XB5,0XB6,0XAD,0X96,
0XAD,0X12,0XED,0X62,0XDD,0X21,0XC4,0XA1,0XB4,0X41,0XA3,0XE0,0X93,0X80,0X83,0X21,
0X72,0XA1,0X5A,0X21,0X5A,0X22,0XA5,0X13,0XAD,0X75,0XB5,0XD6,0XBD,0XD7,0XBD,0XD7,
0XBD,0XD7,0XBD,0XD7,0XBD,0XD7,0XB5,0XD7,0XB5,0XB6,0XAD,0X75,0XA5,0X14,0XC4,0XE8,
0XBC,0X42,0XAB,0XE1,0X9B,0X81,0X83,0X21,0X83,0X46,0XA5,0X14,0XAD,0X55,0XB5,0XB6,
0XB5,0XD7,0XBD,0XD7,0XBD,0XD7,0XBD,0XD7,};
