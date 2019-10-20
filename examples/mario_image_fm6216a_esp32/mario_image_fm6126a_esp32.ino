
#include "PxMatrix.h"

#ifdef ESP32

#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 2
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#endif

#ifdef ESP8266

#include <Ticker.h>
Ticker display_ticker;
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2

#endif
// Pins for LED MATRIX

#define matrix_width 64
#define matrix_height 64

// This defines the 'on' time of the display is us. The larger this number,
// the brighter the display. If too large the ESP will crash
uint8_t display_draw_time=10; //10-50 is usually fine

// PxMATRIX display(matrix_width,matrix_height,P_LAT, P_OE,P_A,P_B,P_C);
//PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D);
PxMATRIX display(matrix_width,matrix_height,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);

uint16_t myCOLORS[8]={myRED,myGREEN,myBLUE,myWHITE,myYELLOW,myCYAN,myMAGENTA,myBLACK};


// Converted using the following site: http://www.rinkydinkelectronics.com/t_imageconverter565.php
uint16_t static mario[] = {
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0010 (16) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0020 (32) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0030 (48) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0040 (64) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0050 (80) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0060 (96) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0070 (112) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0080 (128) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0090 (144) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x00A0 (160) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x00B0 (176) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x00C0 (192) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x00D0 (208) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x00E0 (224) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000,   // 0x00F0 (240) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0100 (256) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0110 (272) pixels
  0x07FF, 0x07FF, 0x0000, 0x0000, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0x0000, 0x07FF, 0x07FF,   // 0x0120 (288) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0xFFFF,   // 0x0130 (304) pixels
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0140 (320) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0150 (336) pixels
  0x07FF, 0x07FF, 0x0000, 0x6180, 0x6180, 0x0000, 0x07FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x6180, 0x0000, 0x07FF, 0x07FF,   // 0x0160 (352) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0xFFFF, 0xFFFF,   // 0x0170 (368) pixels
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0180 (384) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0190 (400) pixels
  0x07FF, 0x07FF, 0x0000, 0x6180, 0x6180, 0x6180, 0x0000, 0xF800, 0xF800, 0xF800, 0xF800, 0x0000, 0x6180, 0x0000, 0x07FF, 0x07FF,   // 0x01A0 (416) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0xFFFF, 0xFFFF,   // 0x01B0 (432) pixels
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x01C0 (448) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x01D0 (464) pixels
  0x07FF, 0x07FF, 0x0000, 0x6180, 0x6180, 0x6180, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0x0000, 0x07FF, 0x07FF, 0x07FF,   // 0x01E0 (480) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0xFFFF, 0xFFFF,   // 0x01F0 (496) pixels
  0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0200 (512) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0210 (528) pixels
  0x07FF, 0x07FF, 0x0000, 0x6180, 0x6180, 0xF800, 0xF800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF, 0x07FF,   // 0x0220 (544) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0230 (560) pixels
  0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0240 (576) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0250 (592) pixels
  0x07FF, 0x07FF, 0x0000, 0xF800, 0xF800, 0xF800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF,   // 0x0260 (608) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0270 (624) pixels
  0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0280 (640) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0290 (656) pixels
  0x07FF, 0x07FF, 0x0000, 0xF800, 0x0000, 0x0000, 0xFF4F, 0xFF4F, 0xFF4F, 0xFF4F, 0xFF4F, 0xFF4F, 0x0000, 0x0000, 0x07FF, 0x07FF,   // 0x02A0 (672) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x02B0 (688) pixels
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x02C0 (704) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x02D0 (720) pixels
  0x07FF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFF4F, 0xFF4F, 0xFF4F, 0x0000, 0xFF4F, 0x0000, 0xFF4F, 0x0000, 0x07FF, 0x07FF, 0x07FF,   // 0x02E0 (736) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x02F0 (752) pixels
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0300 (768) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0310 (784) pixels
  0x07FF, 0x0000, 0xFF4F, 0x0000, 0x0000, 0xFF4F, 0xFF4F, 0xFF4F, 0x0000, 0xFF4F, 0x0000, 0xFF4F, 0x0000, 0x07FF, 0x07FF, 0x07FF,   // 0x0320 (800) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0330 (816) pixels
  0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0340 (832) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0350 (848) pixels
  0x0000, 0xFF4F, 0xFF4F, 0x0000, 0x0000, 0x0000, 0xFF4F, 0xFF4F, 0xFF4F, 0xFF4F, 0xFF4F, 0xFF4F, 0xFF4F, 0x0000, 0x07FF, 0x07FF,   // 0x0360 (864) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0xFFFF, 0xFFFF,   // 0x0370 (880) pixels
  0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0380 (896) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0390 (912) pixels
  0x0000, 0xFF4F, 0xFF4F, 0xFF4F, 0x0000, 0xFF4F, 0xFF4F, 0x0000, 0xFF4F, 0xFF4F, 0xFF4F, 0xFF4F, 0xFF4F, 0x0000, 0x07FF, 0x07FF,   // 0x03A0 (928) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0xFFFF, 0xFFFF,   // 0x03B0 (944) pixels
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x03C0 (960) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x03D0 (976) pixels
  0x07FF, 0x0000, 0xFF4F, 0xFF4F, 0xFF4F, 0xFF4F, 0x0000, 0x0000, 0x0000, 0x0000, 0xFF4F, 0xFF4F, 0x0000, 0x0000, 0x0000, 0x07FF,   // 0x03E0 (992) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0x8410, 0xFFFF,   // 0x03F0 (1008) pixels
  0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x8410, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0400 (1024) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0410 (1040) pixels
  0x07FF, 0x07FF, 0x0000, 0x0000, 0xFF4F, 0xFF4F, 0xFF4F, 0xFF4F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF, 0x07FF,   // 0x0420 (1056) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0x8410,   // 0x0430 (1072) pixels
  0x8410, 0x8410, 0x8410, 0x0000, 0x0000, 0x8410, 0x8410, 0x8410, 0x8410, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0440 (1088) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0450 (1104) pixels
  0x07FF, 0x07FF, 0x07FF, 0x0000, 0x0000, 0x0000, 0xFF4F, 0xFF4F, 0xFF4F, 0xFF4F, 0xFF4F, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0460 (1120) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000,   // 0x0470 (1136) pixels
  0x0000, 0x0000, 0x0000, 0x07FF, 0x07FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0480 (1152) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0490 (1168) pixels
  0x07FF, 0x0000, 0x0000, 0xF800, 0xF800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07FF,   // 0x04A0 (1184) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x04B0 (1200) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x04C0 (1216) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0x0000,   // 0x04D0 (1232) pixels
  0x0000, 0x0000, 0xF800, 0xF800, 0xF800, 0xF800, 0x0000, 0x013F, 0x0000, 0xF800, 0x0000, 0x0000, 0xFF4F, 0xFF4F, 0xFF4F, 0x0000,   // 0x04E0 (1248) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x04F0 (1264) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0500 (1280) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0xFF4F, 0xFF4F,   // 0x0510 (1296) pixels
  0xFF4F, 0xFF4F, 0x0000, 0xF800, 0xF800, 0xF800, 0xF800, 0x0000, 0x013F, 0x0000, 0xF800, 0x0000, 0x0000, 0xFF4F, 0xFF4F, 0x0000,   // 0x0520 (1312) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0530 (1328) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0540 (1344) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0000, 0xFF4F, 0xFF4F,   // 0x0550 (1360) pixels
  0xFF4F, 0xFF4F, 0xFF4F, 0x0000, 0xF800, 0xF800, 0xF800, 0x0000, 0x013F, 0x0000, 0xF800, 0xF800, 0x0000, 0x0000, 0x0000, 0x07FF,   // 0x0560 (1376) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0570 (1392) pixels
  0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF,   // 0x0580 (1408) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0590 (1424) pixels
  0x0000, 0xFF4F, 0xFF4F, 0x0000, 0xF800, 0x0000, 0x0000, 0x013F, 0x013F, 0x0000, 0x0000, 0x0000, 0x013F, 0x0000, 0x4FE0, 0x4FE0,   // 0x05A0 (1440) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x05B0 (1456) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x05C0 (1472) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x0000, 0x0000, 0x6180, 0x6180, 0x6180, 0x6180,   // 0x05D0 (1488) pixels
  0x6180, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x013F, 0x013F, 0xFEC0, 0xFEC0, 0x013F, 0x013F, 0xFEC0, 0x0000, 0x4FE0, 0x4FE0,   // 0x05E0 (1504) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x05F0 (1520) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x0600 (1536) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x0000, 0x6180, 0x6180, 0x6180, 0x6180, 0x6180,   // 0x0610 (1552) pixels
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x013F, 0x013F, 0xFEC0, 0xFEC0, 0x013F, 0x013F, 0xFEC0, 0x0000, 0x4FE0, 0x4FE0,   // 0x0620 (1568) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x0630 (1584) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x0640 (1600) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x0000, 0x6180, 0x6180, 0x6180, 0x6180, 0x0000,   // 0x0650 (1616) pixels
  0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x0000, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x0660 (1632) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x0670 (1648) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x0680 (1664) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x0000, 0x0000, 0x6180, 0x6180, 0x6180, 0x0000,   // 0x0690 (1680) pixels
  0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x0000, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x06A0 (1696) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x06B0 (1712) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x06C0 (1728) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x0000, 0x0000, 0x0000, 0x0000, 0x013F,   // 0x06D0 (1744) pixels
  0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x0000, 0x0000, 0x013F, 0x013F, 0x0000, 0x0000, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x06E0 (1760) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x06F0 (1776) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x0700 (1792) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x0000, 0x013F,   // 0x0710 (1808) pixels
  0x013F, 0x013F, 0x0000, 0x0000, 0x0000, 0x0000, 0x013F, 0x013F, 0x0000, 0x0000, 0x0000, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x0720 (1824) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x0730 (1840) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x0740 (1856) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x0000, 0x013F,   // 0x0750 (1872) pixels
  0x013F, 0x0000, 0x6180, 0x6180, 0x0000, 0x0000, 0x0000, 0x0000, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x0760 (1888) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x0770 (1904) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x0780 (1920) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x0000,   // 0x0790 (1936) pixels
  0x0000, 0x4FE0, 0x0000, 0x0000, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x07A0 (1952) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x07B0 (1968) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x07C0 (1984) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x07D0 (2000) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x07E0 (2016) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x07F0 (2032) pixels
  0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,   // 0x0800 (2048) pixels
};


#ifdef ESP8266
// ISR for display refresh
void display_updater()
{
  display.display(display_draw_time);
}
#endif

#ifdef ESP32
void IRAM_ATTR display_updater(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  display.display(display_draw_time);
  portEXIT_CRITICAL_ISR(&timerMux);
}
#endif


void display_update_enable(bool is_enable)
{

#ifdef ESP8266
  if (is_enable)
    display_ticker.attach(0.002, display_updater);
  else
    display_ticker.detach();
#endif

#ifdef ESP32
  if (is_enable)
  {
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 2000, true);
    timerAlarmEnable(timer);
  }
  else
  {
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
#endif
}



void setup() {
 Serial.begin(9600);
  // Define your display layout here, e.g. 1/8 step
  display.begin(32);
  display.setDriverChip(FM6126A);

  // Define your scan pattern here {LINE, ZIGZAG, ZAGGIZ, WZAGZIG, VZAG} (default is LINE)
  //display.setScanPattern(LINE);

  // Define multiplex implemention here {BINARY, STRAIGHT} (default is BINARY)
  //display.setMuxPattern(BINARY);

  display.setFastUpdate(true);
  display.clearDisplay();
  display.setTextColor(myCYAN);
  display.setCursor(2,0);
  display.print("Pixel");
  display.setTextColor(myMAGENTA);
  display.setCursor(2,8);
  display.print("Time");
  display_update_enable(true);

  delay(3000);

}

void drawImage(int x, int y, uint16_t color)
{
 int imageHeight = 33;
 int imageWidth = 33;
 int counter = 0;
 for (int yy = 0; yy < imageHeight; yy++)
 {
   for (int xx = 0; xx < imageWidth; xx++)
   {
     display.drawPixel(xx + x , yy + y, color);
     counter++;
   }
 }
}

void drawImage2(int x, int y)
{
 int imageHeight = 32;
 int imageWidth = 64;
 int counter = 0;
 for (int yy = 0; yy < imageHeight; yy++)
 {
   for (int xx = 0; xx < imageWidth; xx++)
   {
     display.drawPixel(xx + x , yy + y, mario[counter]);
     counter++;
   }
 }
}

int cnt = 0;
void loop() {
  cnt++;
  delay(1000);
  display.clearDisplay();

  // drawImage2(0, 16);

  uint32_t x = 1;
  uint32_t y = 1;
  if(cnt == 1)
  {
    // drawImage2(0, 0);
    drawImage(x, y, myWHITE);
    Serial.println("test white");
  }
  else if(cnt == 2){
    drawImage2(0, 8);
    drawImage(x, y, myRED);
    Serial.println("test red");
    // cnt = 0;
  }
  else if(cnt == 3){
    // drawImage2(0, 16);
    drawImage(x, y, myGREEN);
    Serial.println("test green");
    // cnt = 0;
  }
  else if(cnt == 4){
    drawImage2(0, 32);
    drawImage(x, y, myBLUE);
    Serial.println("test blue");
    cnt = 0;
  }
  else if(cnt == 5){
    drawImage2(0, 0);
    Serial.println("test blue");
    // cnt = 0;
  }
  else if(cnt == 6){
    drawImage2(0, 16);
    Serial.println("test blue");
    cnt = 0;
  }

}