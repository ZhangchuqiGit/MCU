#include "picture.h"


const unsigned char gImage_led_on[2008] = { 0X10,0X10,0X00,0X19,0X00,0X28,0X01,0X1B,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDD,
0XFF,0XB8,0XFF,0X95,0XFF,0X34,0XFF,0X11,0XFF,0X0E,0XFE,0XED,0XFF,0X0F,0XFF,0X53,
0XFF,0X96,0XFF,0XBC,0XFF,0XFE,0XFF,0XFE,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFE,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFD,0XFF,0XBA,
0XF7,0X55,0XFF,0X33,0XFF,0X32,0XFF,0X33,0XFF,0X12,0XFE,0XEE,0XFE,0XCC,0XFE,0XED,
0XFE,0XCD,0XF6,0XCE,0XF6,0XF1,0XFF,0X76,0XFF,0XFC,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0XF7,0X76,
0XF7,0X33,0XFF,0X52,0XFF,0X31,0XFF,0X33,0XFF,0X13,0XFF,0X11,0XFF,0X0E,0XFE,0XCB,
0XFE,0XCB,0XFE,0XCC,0XFE,0XCC,0XFE,0XEC,0XF6,0XCD,0XF7,0X32,0XFF,0XFB,0XFF,0XFF,
0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDB,0XF7,0X55,
0XFF,0X32,0XFF,0X32,0XFF,0X32,0XFF,0X31,0XFF,0X32,0XFF,0X32,0XFF,0X11,0XFE,0XEE,
0XFE,0XCC,0XFE,0XCB,0XFE,0XCC,0XFE,0XCB,0XFE,0XEA,0XFE,0XEB,0XFE,0XCD,0XF6,0XD1,
0XFF,0XFA,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XF7,0X75,
0XFF,0X32,0XFF,0X12,0XFF,0X33,0XFF,0X11,0XFF,0X31,0XFF,0X51,0XFF,0X32,0XFF,0X31,
0XFE,0XEF,0XFE,0XCD,0XFE,0XCC,0XFE,0XAC,0XFE,0XCB,0XFE,0XEB,0XFE,0XCB,0XFE,0XAC,
0XFE,0XCD,0XF6,0XF1,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0X77,
0XFF,0X51,0XFF,0X31,0XFF,0X13,0XFF,0X11,0XFF,0X31,0XFF,0X11,0XFF,0X33,0XFF,0X35,
0XFF,0X76,0XF7,0X55,0XF7,0X33,0XF6,0XF1,0XFE,0XCF,0XFE,0XAC,0XFE,0XCB,0XFE,0XCB,
0XFE,0XCB,0XFE,0XCC,0XFE,0XCE,0XFF,0X54,0XFF,0XFC,0XFF,0XFF,0XFF,0XFE,0XFF,0XDB,
0XF7,0X33,0XFF,0X52,0XFF,0X32,0XFF,0X31,0XFF,0X51,0XF7,0X32,0XFF,0X76,0XFF,0XBA,
0XFF,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFD,0XFF,0XD8,0XF7,0X11,0XFE,0XCD,
0XFE,0XCC,0XFE,0XEC,0XFE,0XCD,0XFE,0XAC,0XF6,0XAE,0XFF,0XB9,0XFF,0XFF,0XFF,0XFD,
0XFF,0X76,0XFF,0X33,0XFF,0X12,0XFF,0X31,0XFF,0X31,0XF7,0X32,0XFF,0XB8,0XFF,0XFE,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFD,0XFF,0XFD,0XFF,0XFD,0XFF,0XFD,0XFF,0XFC,
0XFF,0X75,0XF6,0XCE,0XFE,0XED,0XFE,0XCC,0XFE,0XEB,0XFE,0XCC,0XFE,0XF3,0XFF,0XFD,
0XFF,0XBB,0XF7,0X34,0XFF,0X32,0XFF,0X32,0XFF,0X31,0XFF,0X32,0XFF,0X98,0XFF,0XDF,
0XFF,0XFF,0XFF,0XFE,0XFF,0XBA,0XFF,0X76,0XFF,0X33,0XFF,0X13,0XFF,0X56,0XFF,0XFC,
0XFF,0XFE,0XFF,0XFD,0XFF,0X53,0XFE,0XEC,0XFE,0XCC,0XFE,0XCC,0XFE,0XEB,0XF6,0XCE,
0XFF,0XB9,0XFF,0X78,0XFF,0X33,0XFF,0X31,0XFF,0X51,0XFF,0X32,0XFF,0X55,0XFF,0XFD,
0XFF,0XFF,0XFF,0XFE,0XFF,0X97,0XF7,0X33,0XFF,0X31,0XFE,0XEF,0XFE,0XCD,0XFE,0XED,
0XF7,0X11,0XFF,0XBA,0XFF,0XFE,0XFF,0XFC,0XFE,0XF1,0XFE,0XCB,0XFE,0XAC,0XFE,0XCD,
0XFE,0XCD,0XFF,0X75,0XFF,0X54,0XFF,0X32,0XFF,0X11,0XFF,0X51,0XFF,0X33,0XFF,0XB9,
0XFF,0XFF,0XFF,0XFE,0XF7,0X99,0XFF,0X33,0XFF,0X32,0XFF,0X31,0XFE,0XEF,0XFE,0XCD,
0XFE,0XCB,0XF6,0XCD,0XFF,0X13,0XFF,0XFC,0XFF,0XFF,0XFF,0X77,0XFE,0XEC,0XFE,0XEB,
0XFE,0XCC,0XFE,0XCC,0XFF,0X31,0XF7,0X51,0XFF,0X31,0XFF,0X11,0XFF,0X31,0XF7,0X33,
0XFF,0XDB,0XFF,0XFF,0XFF,0XFC,0XF7,0X34,0XFF,0X52,0XFF,0X32,0XFF,0X11,0XFE,0XEF,
0XFE,0XCC,0XFE,0XCA,0XFE,0XEC,0XF6,0XCD,0XFF,0XB8,0XFF,0XDF,0XFF,0XBB,0XF6,0XCD,
0XFE,0XEA,0XFE,0XCB,0XFE,0XEB,0XFF,0X0D,0XF7,0X51,0XFF,0X32,0XFF,0X13,0XFF,0X13,
0XF7,0X34,0XFF,0XFC,0XFF,0XFF,0XFF,0XBB,0XFF,0X32,0XFF,0X31,0XFF,0X32,0XFF,0X32,
0XFE,0XEF,0XFE,0XCC,0XFE,0XEB,0XFE,0XCB,0XFE,0XEB,0XFF,0X55,0XFF,0XDE,0XFF,0XDC,
0XEE,0XAF,0XFE,0XCC,0XFE,0XCC,0XFE,0XEC,0XFE,0XEC,0XFF,0X30,0XFF,0X11,0XFF,0X32,
0XFF,0X12,0XFF,0X33,0XFF,0X97,0XFF,0X99,0XFF,0X76,0XFF,0X51,0XFF,0X31,0XFF,0X32,
0XFF,0X12,0XFF,0X0F,0XFE,0XCC,0XFE,0XCB,0XFE,0XCB,0XFE,0XCB,0XFE,0XEE,0XFF,0X74,
0XFF,0X74,0XF6,0XCE,0XFE,0XCC,0XFE,0XCB,0XFE,0XEB,0XFE,0XCC,0XFF,0X11,0XFF,0X30,
0XFF,0X31,0XFF,0X31,0XFF,0X31,0XFF,0X31,0XFF,0X32,0XFF,0X32,0XFF,0X50,0XFF,0X31,
0XFF,0X32,0XFF,0X32,0XFF,0X0E,0XFE,0XEC,0XFE,0XCC,0XFE,0XCC,0XFE,0XCC,0XFE,0XEB,
0XFE,0XEB,0XF6,0XEB,0XFE,0XEC,0XFE,0XCC,0XFE,0XCB,0XFE,0XEB,0XFE,0XED,0XFF,0X14,
0XFF,0X32,0XFF,0X32,0XFF,0X12,0XFF,0X31,0XFF,0X31,0XFF,0X31,0XFF,0X32,0XFF,0X32,
0XFF,0X32,0XFF,0X32,0XFF,0X31,0XFF,0X0E,0XFE,0XEC,0XFE,0XCC,0XFE,0XCC,0XFE,0XCC,
0XFE,0XCC,0XFE,0XCB,0XFE,0XCC,0XFE,0XCD,0XFE,0XAD,0XFE,0XCB,0XFE,0XCC,0XFF,0X0F,
0XFF,0X56,0XFF,0X32,0XFF,0X11,0XFF,0X11,0XFF,0X31,0XFF,0X31,0XFF,0X31,0XFF,0X32,
0XFF,0X31,0XFF,0X32,0XFF,0X32,0XFF,0X31,0XFF,0X0E,0XFE,0XCC,0XFE,0XCC,0XFE,0XCC,
0XFE,0XCC,0XFE,0XCC,0XFE,0XCC,0XFE,0XCC,0XFE,0XCC,0XFE,0XCC,0XFE,0XCB,0XFE,0XCD,
0XFF,0X52,0XFF,0XB8,0XFF,0X32,0XFF,0X10,0XFF,0X11,0XFF,0X32,0XFF,0X32,0XFF,0X32,
0XFF,0X31,0XFF,0X31,0XFF,0X32,0XFF,0X31,0XFF,0X31,0XFE,0XEF,0XFE,0XCC,0XFE,0XCB,
0XFE,0XCB,0XFE,0XEB,0XFE,0XEB,0XFE,0XCC,0XFE,0XCC,0XFE,0XCB,0XFE,0XEA,0XFE,0XCC,
0XFE,0XAF,0XFF,0X75,0XFF,0XDC,0XF7,0X54,0XFF,0X31,0XFF,0X13,0XFF,0X12,0XFF,0X32,
0XFF,0X32,0XFF,0X32,0XFF,0X31,0XFF,0X32,0XFF,0X32,0XFF,0X31,0XFF,0X0F,0XFE,0XCC,
0XFE,0XCB,0XFE,0XCB,0XFE,0XCC,0XFE,0XCC,0XFE,0XCB,0XFE,0XCB,0XFE,0XCB,0XFE,0XEB,
0XFE,0XCC,0XFE,0XD1,0XFF,0XB9,0XFF,0XFE,0XFF,0X97,0XFF,0X32,0XFF,0X32,0XFF,0X32,
0XFF,0X32,0XFF,0X32,0XFF,0X32,0XFF,0X31,0XFF,0X32,0XFF,0X32,0XFF,0X31,0XFF,0X0F,
0XFE,0XCC,0XFE,0XCB,0XFE,0XCB,0XFE,0XCC,0XFE,0XCC,0XFE,0XCB,0XFE,0XCC,0XFE,0XCC,
0XFE,0XCB,0XFE,0XCB,0XFF,0X54,0XFF,0XFD,0XFF,0XFF,0XFF,0XDB,0XF7,0X13,0XFF,0X31,
0XFF,0X31,0XFF,0X31,0XFF,0X32,0XFF,0X32,0XFF,0X32,0XFF,0X32,0XFF,0X32,0XFF,0X31,
0XFF,0X0F,0XFE,0XCC,0XFE,0XCB,0XFE,0XCB,0XFE,0XCC,0XFE,0XCB,0XFE,0XCB,0XFE,0XEC,
0XFE,0XCC,0XFE,0XCC,0XF6,0XCD,0XFF,0XD9,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XB8,
0XFF,0X53,0XFF,0X32,0XFF,0X31,0XFF,0X31,0XFF,0X31,0XFF,0X12,0XFF,0X32,0XFF,0X32,
0XFF,0X31,0XFF,0X0F,0XFE,0XCC,0XFE,0XCB,0XFE,0XCB,0XFE,0XCC,0XFE,0XCB,0XFE,0XCC,
0XFE,0XCC,0XFE,0XCC,0XFE,0XCD,0XFF,0X54,0XFF,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFD,0XFF,0X57,0XFF,0X33,0XFF,0X32,0XFF,0X31,0XFF,0X31,0XFF,0X32,0XFF,0X32,
0XFF,0X32,0XFF,0X31,0XFF,0X0F,0XFE,0XCC,0XFE,0XCB,0XFE,0XCB,0XFE,0XCC,0XFE,0XCB,
0XFE,0XCC,0XFE,0XCD,0XFE,0XCD,0XF7,0X12,0XFF,0XFC,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XDD,0XFF,0X57,0XFF,0X12,0XFF,0X32,0XFF,0X31,0XFF,0X31,
0XFF,0X32,0XFF,0X32,0XFF,0X31,0XFF,0X0F,0XFE,0XCC,0XFE,0XCB,0XFE,0XCB,0XFE,0XCC,
0XFE,0XCB,0XFE,0XEC,0XFE,0XAE,0XFF,0X13,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XFF,0X76,0XFF,0X32,0XFF,0X31,
0XFF,0X32,0XFF,0X32,0XFF,0X32,0XFF,0X31,0XFF,0X0F,0XFE,0XCC,0XFE,0XCB,0XFE,0XCB,
0XFE,0XCC,0XFE,0XEB,0XF6,0XCD,0XFF,0X34,0XFF,0XFC,0XFF,0XDE,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFF,0X33,
0XFF,0X31,0XFF,0X12,0XFF,0X32,0XFF,0X32,0XFF,0X31,0XFF,0X0F,0XFE,0XCC,0XFE,0XCB,
0XFE,0XCB,0XFE,0XCC,0XFE,0XCB,0XFF,0X10,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDD,
0XFF,0X34,0XFF,0X31,0XFF,0X12,0XFF,0X32,0XFF,0X32,0XFF,0X31,0XFF,0X0F,0XFE,0XCC,
0XFE,0XCB,0XFE,0XCB,0XFE,0XCC,0XFE,0XCC,0XFE,0XF2,0XFF,0XFD,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFC,0XFF,0X34,0XFF,0X31,0XFF,0X30,0XFF,0X31,0XFF,0X32,0XFF,0X31,0XFE,0XEF,
0XFE,0XCD,0XFE,0XCB,0XFE,0XCB,0XFE,0XCB,0XFE,0XCC,0XFF,0X11,0XFF,0XFC,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFC,0XF7,0X36,0XF7,0X13,0XFF,0X31,0XFF,0X31,0XFF,0X31,0XFF,0X31,
0XFF,0X0F,0XFE,0XCC,0XFE,0XCB,0XFE,0XCB,0XFE,0XCB,0XFE,0XEE,0XF7,0X13,0XFF,0XFC,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XEF,0X18,0XEF,0X37,0XF7,0X56,0XF7,0X35,0XF7,0X36,
0XF7,0X35,0XEE,0XF3,0XEE,0XD2,0XEE,0XD1,0XEE,0XD1,0XEE,0XD2,0XEE,0XF3,0XE6,0XF6,
0XFF,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC6,0X37,0XC6,0X37,0XC6,0X37,0XC6,0X38,
0XC6,0X38,0XC6,0X38,0XB5,0XD6,0XA5,0X74,0XA5,0X74,0XA5,0X74,0XA5,0X74,0XAD,0X73,
0XAD,0X73,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0X19,0XBE,0X3A,0XBE,0X1A,
0XB6,0X3A,0XB6,0X3A,0XB6,0X3B,0XA5,0XB9,0X9D,0X77,0X9D,0X77,0X9D,0X78,0X9D,0X78,
0X9D,0X77,0X9D,0X56,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0X1A,0XB6,0X3B,
0XBE,0X3B,0XBE,0X3A,0XBE,0X39,0XBE,0X3A,0XAD,0XD9,0X9D,0X77,0X9D,0X78,0X9D,0X78,
0X9D,0X77,0X9D,0X78,0X9D,0X58,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0X3A,
0XB6,0X3B,0XB6,0X3B,0XBE,0X39,0XBE,0X39,0XBE,0X39,0XAD,0XD8,0X9D,0X57,0X9D,0X58,
0X9D,0X78,0X9D,0X77,0X9D,0X78,0X9D,0X78,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XBE,0X39,0XBE,0X3A,0XB6,0X3A,0XBE,0X3A,0XB6,0X3A,0XB6,0X3A,0XAD,0XD9,0X9D,0X58,
0X9D,0X59,0X9D,0X58,0X9D,0X78,0X9D,0X78,0XAD,0XB8,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XD6,0XDC,0XBE,0X39,0XBE,0X3A,0XBE,0X3A,0XB6,0X3A,0XBE,0X3A,0XAD,0XD9,
0X9D,0X58,0X9D,0X58,0X9D,0X78,0X9D,0X77,0X9D,0X57,0XC6,0X7A,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XEF,0X7E,0XBE,0X39,0XBE,0X3A,0XBE,0X5A,0XBE,0X5A,0XB6,0X3A,
0XAD,0XD9,0X9D,0X77,0X9D,0X58,0X9D,0X78,0X95,0X78,0XA5,0X56,0XEF,0X5D,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XDF,0XFF,0XDF,0XFF,0XFF,0XDF,0X3D,0XBE,0X39,0XB6,0X19,0XBE,0X5A,
0XB6,0X1A,0XAD,0XD9,0X9D,0X57,0X9D,0X77,0X9D,0X57,0XA5,0X98,0XCE,0X9B,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,0XDF,0X1C,0XB6,0X18,
0XBE,0X5A,0XB6,0X3A,0XAD,0XD9,0X9D,0X57,0X9D,0X77,0X9D,0X56,0XC6,0X7B,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XEF,0X9E,0XCE,0X9B,0XBE,0X3A,0XAD,0XD9,0XA5,0X98,0XBE,0X3A,0XE7,0X5D,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
};
