/**
 * Modern Vintage CNC Firmware
*/

// 100k hotend thermistor with 4.7k pull up to 3.3v and 220R to analog input as in GTM32 Pro vB
constexpr temp_entry_t temptable_22[] PROGMEM = {
  { OV(   1), 352 },
  { OV(   6), 341 },
  { OV(  11), 330 },
  { OV(  16), 319 },
  { OV(  20), 307 },
  { OV(  26), 296 },
  { OV(  31), 285 },
  { OV(  40), 274 },
  { OV(  51), 263 },
  { OV(  61), 251 },
  { OV(  72), 245 },
  { OV(  77), 240 },
  { OV(  82), 237 },
  { OV(  87), 232 },
  { OV(  91), 229 },
  { OV(  94), 227 },
  { OV(  97), 225 },
  { OV( 100), 223 },
  { OV( 104), 221 },
  { OV( 108), 219 },
  { OV( 115), 214 },
  { OV( 126), 209 },
  { OV( 137), 204 },
  { OV( 147), 200 },
  { OV( 158), 193 },
  { OV( 167), 192 },
  { OV( 177), 189 },
  { OV( 197), 163 },
  { OV( 230), 174 },
  { OV( 267), 165 },
  { OV( 310), 158 },
  { OV( 336), 151 },
  { OV( 379), 143 },
  { OV( 413), 138 },
  { OV( 480), 127 },
  { OV( 580), 110 },
  { OV( 646), 100 },
  { OV( 731),  88 },
  { OV( 768),  84 },
  { OV( 861),  69 },
  { OV( 935),  50 },
  { OV( 975),  38 },
  { OV(1001),  27 },
  { OV(1011),  22 },
  { OV(1015),  13 },
  { OV(1020),   6 },
  { OV(1023),   0 }
};
