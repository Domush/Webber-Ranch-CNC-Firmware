/**
 * Modern Vintage CNC Firmware
*/
#pragma once

#define OVM(V) OV((V)*(0.327/0.5))

// R25 = 100 kOhm, beta25 = 4092 K, 4.7 kOhm pull-up, bed thermistor
constexpr temp_entry_t temptable_331[] PROGMEM = {
  { OVM(  23), 300 },
  { OVM(  25), 295 },
  { OVM(  27), 290 },
  { OVM(  28), 285 },
  { OVM(  31), 280 },
  { OVM(  33), 275 },
  { OVM(  35), 270 },
  { OVM(  38), 265 },
  { OVM(  41), 260 },
  { OVM(  44), 255 },
  { OVM(  48), 250 },
  { OVM(  52), 245 },
  { OVM(  56), 240 },
  { OVM(  61), 235 },
  { OVM(  66), 230 },
  { OVM(  71), 225 },
  { OVM(  78), 220 },
  { OVM(  84), 215 },
  { OVM(  92), 210 },
  { OVM( 100), 205 },
  { OVM( 109), 200 },
  { OVM( 120), 195 },
  { OVM( 131), 190 },
  { OVM( 143), 185 },
  { OVM( 156), 180 },
  { OVM( 171), 175 },
  { OVM( 187), 170 },
  { OVM( 205), 165 },
  { OVM( 224), 160 },
  { OVM( 245), 155 },
  { OVM( 268), 150 },
  { OVM( 293), 145 },
  { OVM( 320), 140 },
  { OVM( 348), 135 },
  { OVM( 379), 130 },
  { OVM( 411), 125 },
  { OVM( 445), 120 },
  { OVM( 480), 115 },
  { OVM( 516), 110 },
  { OVM( 553), 105 },
  { OVM( 591), 100 },
  { OVM( 628),  95 },
  { OVM( 665),  90 },
  { OVM( 702),  85 },
  { OVM( 737),  80 },
  { OVM( 770),  75 },
  { OVM( 801),  70 },
  { OVM( 830),  65 },
  { OVM( 857),  60 },
  { OVM( 881),  55 },
  { OVM( 903),  50 },
  { OVM( 922),  45 },
  { OVM( 939),  40 },
  { OVM( 954),  35 },
  { OVM( 966),  30 },
  { OVM( 977),  25 },
  { OVM( 985),  20 },
  { OVM( 993),  15 },
  { OVM( 999),  10 },
  { OVM(1004),   5 },
  { OVM(1008),   0 },
  { OVM(1012),  -5 },
  { OVM(1016), -10 },
  { OVM(1020), -15 }
};
