// osx_vk_codes.h - public domain
// I couldn't find a good resource of just a list or header of the virtual
// keycodes on OSX, so here they are. These come codes come from images listed
// at https://boredzo.org/blog/archives/2007-05-22/virtual-key-codes
// One can use they by comparing them against the [NSEvent keyCode] property
// using the Cocoa API or kEventParamKeyCode from the Carbon API.
// -josh <joshuahuelsman@gmail.com>

// This software is dual-licensed to the public domain and under the following
// license: you are granted a perpetual, irrevocable license to copy, modify,
// publish, and distribute this file as you see fit.

#ifndef OSX_VK_CODES_H
#define OSX_VK_CODES_H

// function row

#define OSX_VK_ESCAPE    53
#define OSX_VK_F1        122
#define OSX_VK_F2        120
#define OSX_VK_F3        99
#define OSX_VK_F4        118
#define OSX_VK_F5        96
#define OSX_VK_F6        97
#define OSX_VK_F7        98
#define OSX_VK_F8        100
#define OSX_VK_F9        101
#define OSX_VK_F10       109
#define OSX_VK_F11       103
#define OSX_VK_F12       111
#define OSX_VK_F13       105
#define OSX_VK_F14       107
#define OSX_VK_F15       113

// ANSI/US key rows

#define OSX_VK_BACK_TICK 50
#define OSX_VK_1         18
#define OSX_VK_2         19
#define OSX_VK_3         20
#define OSX_VK_4         21
#define OSX_VK_5         23
#define OSX_VK_6         22
#define OSX_VK_7         26
#define OSX_VK_8         28
#define OSX_VK_9         25
#define OSX_VK_0         29
#define OSX_VK_DASH      27
#define OSX_VK_EQUAL     24
#define OSX_VK_BACKSPACE 51

#define OSX_VK_TAB       48
#define OSX_VK_Q         12
#define OSX_VK_W         13
#define OSX_VK_E         14
#define OSX_VK_R         15
#define OSX_VK_T         17
#define OSX_VK_Y         16
#define OSX_VK_U         32
#define OSX_VK_I         34
#define OSX_VK_O         31
#define OSX_VK_P         35
#define OSX_VK_LBRACKET  33
#define OSX_VK_RBRACKET  30
#define OSX_VK_BACKSLASH 42

#define OSX_VK_CAPS_LOCK 57
#define OSX_VK_A         0
#define OSX_VK_S         1
#define OSX_VK_D         2
#define OSX_VK_F         3
#define OSX_VK_G         5
#define OSX_VK_H         4
#define OSX_VK_J         38
#define OSX_VK_K         40
#define OSX_VK_L         37
#define OSX_VK_COLON     41
#define OSX_VK_QUOTE     39
#define OSX_VK_RETURN    36

#define OSX_VK_SHIFT     56
#define OSX_VK_Z         6
#define OSX_VK_X         7
#define OSX_VK_C         8
#define OSX_VK_V         9
#define OSX_VK_B         11
#define OSX_VK_N         45
#define OSX_VK_M         46
#define OSX_VK_COMMA     43
#define OSX_VK_PERIOD    47
#define OSX_VK_FWD_SLASH 44

#define OSX_VK_CONTROL   59
#define OSX_VK_OPTION    58
#define OSX_VK_COMMAND   55
#define OSX_VK_SPACEBAR  49

// arrow cluster + navigation keys

#define OSX_VK_INSERT    114
#define OSX_VK_HOME      115
#define OSX_VK_PAGE_UP   116
#define OSX_VK_DELETE    117
#define OSX_VK_END       119
#define OSX_VK_PAGE_DOWN 121
#define OSX_VK_UP        126
#define OSX_VK_DOWN      125
#define OSX_VK_RIGHT     124
#define OSX_VK_LEFT      123

// numpad

#define OSX_VK_NUMLOCK       71
#define OSX_VK_NUMPAD_EQUALS 81
#define OSX_VK_NUMPAD_DIVIDE 75
#define OSX_VK_NUMPAD_MULT   67 // multiply, asterisk, star
#define OSX_VK_NUMPAD_7      89
#define OSX_VK_NUMPAD_8      91
#define OSX_VK_NUMPAD_9      92
#define OSX_VK_NUMPAD_MINUS  78
#define OSX_VK_NUMPAD_4      86
#define OSX_VK_NUMPAD_5      87
#define OSX_VK_NUMPAD_6      88
#define OSX_VK_NUMPAD_PLUS   69
#define OSX_VK_NUMPAD_1      83
#define OSX_VK_NUMPAD_2      84
#define OSX_VK_NUMPAD_3      85
#define OSX_VK_NUMPAD_ENTER  76
#define OSX_VK_NUMPAD_0      82
#define OSX_VK_NUMPAD_PERIOD 65

#endif // OSX_VK_CODES_H
